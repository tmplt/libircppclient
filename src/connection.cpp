#include "connection.hpp"
#include "general.hpp"
#include <boost/bind.hpp>
#include <thread>
#include <string>
#include <iostream>

namespace irc {

connection::connection(const bool use_ssl)
    : socket_(io_service_),
      use_ssl_(use_ssl), ctx_(ssl::context::sslv23), ssl_socket_(io_service_, ctx_)
{
    if (use_ssl_) {
        ctx_.set_default_verify_paths(ec_);

        if (ec_)
            throw ec_;
    }
}

boost::system::error_code connection::verify_cert()
{
    boost::system::error_code ec;
    ssl_socket_.set_verify_mode(ssl::verify_peer | ssl::verify_fail_if_no_peer_cert);
    ssl_socket_.set_verify_callback(ssl::rfc2818_verification(addr_), ec);

    return ec;
}

boost::system::error_code connection::shake_hands()
{
    boost::system::error_code ec;
    ssl_socket_.handshake(ssl_socket::client, ec);

    return ec;
}

void connection::connect()
{
    using boost::asio::ip::tcp;

    tcp::resolver r(io_service_);
    tcp::resolver::query query(addr_, port_);

    /* default error. */
    ec_ = boost::asio::error::host_not_found;

    if (use_ssl_) {
        boost::asio::connect(ssl_socket_.lowest_layer(), r.resolve(query), ec_);

        if (!ec_) {
            ec_ = verify_cert();

            if (!ec_)
                ec_ = shake_hands();
        }
    } else {
        boost::asio::connect(socket_.lowest_layer(), r.resolve(query), ec_);
    }

    if (ec_)
        throw ec_;
}

void connection::read_handler(const boost::system::error_code &ec, std::size_t length)
{
    using std::experimental::string_view;

    if (ec) {
        /* Unable to read from server. */
        throw ec;
    } else {
        const string_view content(read_buffer_.data(), length);

        std::stringstream iss(content.data());
        std::string command;
        iss >> command;

        if (command == "PING")
            pong();
        else
            ext_read_handler_(content);

        if (use_ssl_)
            read_some(ssl_socket_);
        else
            read_some(socket_);
    }
}


template<class S>
void connection::read_some(S &s)
{

    s.async_read_some(boost::asio::buffer(read_buffer_),
        boost::bind(&connection::read_handler,
            this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred()));
}

void connection::run()
{
    std::thread ping_thread(ping_handler_);

    if (use_ssl_)
        read_some(ssl_socket_);
    else
        read_some(socket_);

    boost::system::error_code ec;
    io_service_.run(ec);

    /*
     * Remain at this point until we
     * do not need the connection any more.
     */
    ping_thread.join();

    if (ec)
        throw ec;
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
    /*
     * The IRC protocol specifies that all messages sent to the server
     * must be terminated with CR-LF (Carriage Return - Line Feed)
     */
    content.append("\r\n");

    std::cout << "[debug] writing: " << content;

    if (use_ssl_)
        boost::asio::write(ssl_socket_, boost::asio::buffer(content), ec_);
    else
        boost::asio::write(socket_.next_layer(), boost::asio::buffer(content), ec_);

    if (ec_)
        throw ec_;
}

void connection::stop()
{
    /*
     * For a proper shutdown, we first need to terminate
     * the ping thread, which might have to be done while
     * it's in nanosleep. After that, we are free to stop
     * the io_service.
     */

    if (use_ssl_)
        ssl_socket_.lowest_layer().close();
    else
        socket_.lowest_layer().close();

    io_service_.stop();
}

/* ns irc */
}

