#include "connection.hpp"
#include <boost/thread.hpp>
#include "general.hpp"
#include <thread>
#include <string>

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
    using boost::asio::connect;

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
        connect(ssl_socket_.lowest_layer(), r.resolve(query), error);

        if (!error) {
            /*
             * While not expected, one _could_ arise.
             * (naming is hard)
             */
            error = try_handshake();
        }
    }

    else
        connect(socket_, r.resolve(query), error);

    if (error)
        throw error;
}

template<class S>
void connection::run(S &socket)
{
    using namespace boost;

    std::thread ping_handler_thread(ping_handler_);

    /*
     * Start an asynchronous read thread going through connection::read().
     * Pass the arguments (this, _1, _2) to the handler.
     */
    socket.async_read_some(asio::buffer(buffer_),
        bind(&connection::read(socket),
            this, asio::placeholders::error,
            asio::placeholders::bytes_transferred()));

    io_service_.run();

    /* Remain at this point until we do not need the connection any more. */
    ping_handler_thread.join();
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

template<class S>
void connection::write(T &socket, std::string &content)
{
    /*
     * The IRC protocol specifies that all messages sent to the server
     * must be terminated with CR-LF (Carriage Return - Line Feed)
     */
    content.append("\r\n");

    error_code error;
    boost::asio::write(socket, boost::asio::buffer(content), error);

    if (error)
        throw error;
}

template<class S>
void connection::read(S &socket, const error_code &error, std::size_t length)
{
    using namespace boost;

    if (error)
        /* Unable to read from server. */
        throw error;

    else {

        /*
         * Works in synergy with socket::async_read_some().
         *
         * Copy the data within the buffer and the length of it
         * and pass it to the class' read_handler.
         */
        read_handler(std::string(buffer_.data(), length));

        /*
         * Start an asynchronous recursive read thread.
         *
         * Read data from the IRC server into the buffer, and
         * call this function again.
         *
         * Pass the eventual error and the message length.
         */
        socket.async_read_some(asio::buffer(buffer_),
            bind(&connection::read(socket),
                this, asio::placeholders::error,
                asio::placeholders::bytes_transferred));
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

template<class S>
void connection::stop(S &socket)
{
    socket.close();
    io_service_.stop();
}

/* ns irc */
}
