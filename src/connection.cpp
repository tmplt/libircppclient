#include <thread>
#include <string>
#include <boost/bind.hpp>
#include "connection.hpp"
#include "general.hpp"

#include <iostream>
using std::cout;
using std::endl;

namespace irc {

connection::connection(const bool use_ssl)
    : socket_(io_service_),
      /* SSL */
      use_ssl_(use_ssl), ctx_(ssl::context::sslv23), ssl_socket_(io_service_, ctx_)
{
    if (use_ssl_) {
        boost::system::error_code error;
        ctx_.set_default_verify_paths(error);

        if (error)
            throw error;
    }
}

/*
 * error_code would here imply that a fail is expected, but
 * it is not; it could be aliased, but that would only make
 * matters worse, I believe.
 */
error_code connection::try_handshake()
{
    error_code ret;

    ssl_socket_.set_verify_mode(ssl::verify_peer);
    ssl_socket_.set_verify_callback(ssl::rfc2818_verification(addr_));
    ssl_socket_.handshake(ssl_socket::client, ret);

    return ret;
}

void connection::connect()
{
    using boost::asio::ip::tcp;

    /*
     * Resolve the host and generate a list of endpoints.
     * An endpoint is the information used to connect to an address.
     * An address may have more than one endpoint.
     */
    tcp::resolver r(io_service_);
    tcp::resolver::query query(addr_, port_);

    /* default error. */
    error_code error = boost::asio::error::host_not_found;

    if (use_ssl_) {
        boost::asio::connect(ssl_socket_.lowest_layer(), r.resolve(query), error);

        if (!error) {

            /*
             * While not requested, one _could_ arise.
             * Non-zero equals successful handshake.
             */
            error = try_handshake();
        }
    }

    else
        boost::asio::connect(socket_.lowest_layer(), r.resolve(query), error);

    if (error)
        throw error;
}

template<class S>
void connection::read_some(S &s)
{
    using namespace boost::asio;

    /*
     * Start an asynchronous read thread going through connection::read().
     * Pass the arguments (this, [...], [...]) to the it.
     */
    s.async_read_some(buffer(read_buffer_),
        boost::bind(&connection::read,
            this, placeholders::error,
            placeholders::bytes_transferred()));
}

void connection::run()
{
    using namespace boost::asio;

    std::thread ping_thread(ping_handler_);

    if (use_ssl_)
        read_some(ssl_socket_);
    else
        read_some(socket_);

    error_code error;
    io_service_.run(error);

    if (error)
        throw error;

    /* Remain at this point until we do not need the connection any more. */
    ping_thread.join();
}

void connection::ping()
{
    using namespace std::literals;

    while (do_ping) {

        /*
         * Interval decided by mimicing WeeChat.
         * The standard does not seem to explicitly
         * state a ping inverval. Is it the server's
         * decision?
         */
        std::this_thread::sleep_for(1min + 30s);

        write("PING " + addr_);
    }
}

void connection::pong()
{
    write("PONG :" + addr_);
}

void connection::write(std::string content)
{
    using boost::asio::write;
    using boost::asio::buffer;

    /*
     * The IRC protocol specifies that all messages sent to the server
     * must be terminated with CR-LF (Carriage Return - Line Feed)
     */
    content.append("\r\n");

    error_code error;
    cout << "[debug] writing: " << content;

    if (use_ssl_)
        write(ssl_socket_.next_layer(), buffer(content), error);
    else
        write(socket_.next_layer(), buffer(content), error);

    if (error)
        throw error;
}

void connection::read(const error_code &error, std::size_t length)
{
    using namespace boost;

    if (error)
        /* Unable to read from server. */
        throw error;

    else {

        /*
         * Works in synergy with socket::read_some().
         *
         * Copy the data within the buffer and the length of it
         * and pass it to the class' read_handler.
         */
        read_handler(std::string(read_buffer_.data(), length));

        /*
         * Start an asynchronous recursive read thread.
         *
         * Read data from the IRC server into the buffer, and
         * call this function again.
         *
         * Pass the eventual error and the message length.
         */
        if (use_ssl_)
            read_some(ssl_socket_);
        else
            read_some(socket_);
    }
}

void connection::read_handler(const std::string &content)
{
    std::stringstream iss(content);
    std::string command;

    iss >> command;

    if (command == "PING")
        pong();
    else
        ext_read_handler_(content);
}

void connection::stop()
{
    if (use_ssl_)
        ssl_socket_.lowest_layer().close();
    else
        socket_.lowest_layer().close();

    io_service_.stop();
}

/* ns irc */
}

