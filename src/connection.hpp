#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <functional>
#include <string>

using boost::asio::ip::tcp;

namespace irc {

/* A seperate thread which pings. */
typedef std::function<void (void)> ping_t;

/* A seperate thread which reads. */
typedef std::function<void (const std::string &content)> read_handler_t;

using     boost::asio::ip::tcp;
using     boost::system::error_code;
namespace ssl = boost::asio::ssl;
typedef   ssl::stream<tcp::socket> ssl_socket;

class connection {
public:
    connection(const bool use_ssl);

    void connect();
    error_code try_handshake();

    void run();
    void stop();

    /*
     * Asynchronosouly looping. This function pushes any read data
     * to read_handler().
     */
    void read(const boost::system::error_code &error, std::size_t length);

    /*
     * Handles RFC-specific requests, e.g. PING, VERSION, etc.
     * Everything else is passed onto the external read handler,
     * which resides in libircppclient.cpp.
     */
    void read_handler(const std::string &content);

    /* CR-LF is appended */
    void write(std::string &content);

    /*
     * "Binds" the external read handler, which handles everything
     * this class' read_handler() do not. Bound function
     * resides within libircppclient.cpp.
     */
    void set_ext_read_handler(const read_handler_t &handler)
    {
        ext_read_handler_ = handler;
    }

    /*
     * So that we do not get kicked from the server, and so that the
     * server itself does not have to ping us, which seems preferable
     * according to the RFC.
     *
     * In a perfect implementation, this should only be used when no
     * command has been sent to the server for a specified amount of time
     * (as the RFC explains), but boy are timers tricky!
     */
    void ping();

    /* Fail-safe for ping(). */
    void pong();

    void stop_ping()
    {
        do_ping = false;
    }

    /* Is the connection still alive? */
    bool is_alive() const
    {
        return socket_.is_open();
    }

    void set_addr(const std::string &addr)
    {
        addr_ = addr;
    }

    void set_port(const std::string &port)
    {
        port_ = port;
    }

private:
    /* Server information. */
    std::string addr_ = "";
    std::string port_ = "";

    read_handler_t ext_read_handler_;

    ping_t ping_handler_ = std::bind(&connection::ping, this);
    bool   do_ping = true;

    /*
     * 512B is the max message length within the IRC protocol.
     * Given that every message is terminated with CR-LF
     * (Carriage Return - Line Feed), we will have 510B of actual data
     * to process.
     *
     * It's worth noting that several messages can be caught within buffer.
     * Not all messages sent and received from the server are 512B in length.
     *
     * Might change this at a later date, given how IRCv3 seems to work.
     */
    std::array<char, 512> buffer_;

    boost::asio::io_service io_service_;

    bool         use_ssl_;
    tcp::socket  socket_;
    ssl::context ctx_;
    ssl_socket   ssl_socket_;
};

/* ns irc */
}

