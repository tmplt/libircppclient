#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <functional>
#include <string>

using boost::asio::ip::tcp;

namespace irc {

/* A seperate thread which pings. */
using ping_th =  std::function<void (void)>;

/* A type which takes a lambda as argument.. */
using read_handler_t =  std::function<void (const std::string &content)>;

using     boost::asio::ip::tcp;
using     boost::system::error_code;
using     boost::asio::buffered_stream;
namespace ssl = boost::asio::ssl;

using ssl_socket = ssl::stream<tcp::socket>;
using nossl_socket = buffered_stream<tcp::socket>;

class connection {
public:
    connection(const bool use_ssl);

    void connect();

    void run();
    void stop();

    /* CR-LF is appended */
    void write(std::string content);

    /*
     * "Binds" the external read handler, which handles everything
     * this class' read_handler() do not. Bound function
     * resides within libircppclient.cpp.
     */
    void set_ext_read_handler(const read_handler_t &handler)
    {
        ext_read_handler_ = handler;
    }

    void set_addr(const std::string &addr)
    {
        addr_ = addr;
    }

    void set_port(const std::string &port)
    {
        port_ = port;
    }

    void stop_ping()
    {
        do_ping = false;
    }

private:

    /*
     * For both ssl and non-ssl, which are quite different,
     * but share the same interface.
     */
    template<class S>
    void read_some(S &socket);

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

    /* For SSL connections. */
    error_code try_handshake();

    /* Server information. */
    std::string addr_ = "";
    std::string port_ = "";

    read_handler_t ext_read_handler_;

    /* The library's life line. */
    ping_th ping_handler_ = std::bind(&connection::ping, this);
    bool    do_ping = true;

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
    std::array<char, 512> read_buffer_;

    boost::asio::io_service io_service_;

    bool         use_ssl_;
    nossl_socket socket_;
    ssl::context ctx_;
    ssl_socket   ssl_socket_;
};

/* ns irc */
}

