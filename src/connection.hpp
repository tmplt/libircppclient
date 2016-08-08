#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <functional>
#include <string>
#include <experimental/string_view>

using boost::asio::ip::tcp;

namespace irc {

using ping_func = std::function<void (void)>;

/* A type which takes a lambda as argument.. */
using read_handler_t = std::function<void (const std::experimental::string_view &content)>;

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;

using ssl_socket = ssl::stream<tcp::socket>;
using nossl_socket = boost::asio::buffered_stream<tcp::socket>;

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

    void set_addr(const std::experimental::string_view &addr)
    {
        addr_ = addr.data();
    }

    void set_port(const std::experimental::string_view &port)
    {
        port_ = port.data();
    }

    void stop_ping()
    {
        do_ping = false;
    }

private:

    /*
     * For both ssl and non-ssl, which are a bit different,
     * but share the same interface when it comes to reading.
     */
    template<class S>
    void read_some(S &socket);

    /*
     * Handles RFC-specific requests, e.g. PING, VERSION, etc.
     * Everything else is passed onto the external read handler,
     * which resides in libircppclient.cpp.
     */
    void read_handler(const boost::system::error_code &error, std::size_t length);

    /*
     * So that we do not get kicked from the server, and so that the
     * server itself does not have to ping us, which seems preferable
     * according to the standard.
     *
     * In a perfect implementation, this should only be used when no
     * command has been sent to the server for a specified amount of time
     * (as the RFC explains), but boy are timers tricky!
     */
    void ping();
    void pong();

    /* For SSL connections. */
    boost::system::error_code verify_cert();
    boost::system::error_code shake_hands();

    /* Server information. */
    std::string addr_ = "";
    std::string port_ = "";

    read_handler_t ext_read_handler_;

    /*
     * Executed in its own thread within connection::run().
     * Practically the library's main loop.
     */
    ping_func ping_handler_ = std::bind(&connection::ping, this);
    bool do_ping = true;

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

    /*
     * Do not change this order, lest you want segfaults!
     *
     * ssl_socket_ requires both io_service_ and ctx_ to construct,
     * and for some apparent reason the order of these matter.
     */
    boost::asio::io_service   io_service_;
    boost::system::error_code ec_;

    bool         use_ssl_;
    nossl_socket socket_;
    ssl::context ctx_;
    ssl_socket   ssl_socket_;
};

/* ns irc */
}

