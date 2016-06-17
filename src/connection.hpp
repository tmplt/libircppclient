#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <functional>
#include <string>

using boost::asio::ip::tcp;

namespace irc {

/* Handles data received from the server. */
typedef std::function<void (const std::string &content)> read_handler_t;

/* Used to keep the connection alive. */
typedef std::function<void (void)> ping_t;

using     boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
typedef   ssl::stream<tcp::socket> ssl_socket;

class connection {
public:
    connection(const bool use_ssl);

    /* The io_service_, and the loop itself; not the connection itself. */
    void run();
    void stop();

    void connect();

    /*
     * Asynchronously loop this function and push any read data to
     * read_handler()...
     */
    void read(const boost::system::error_code &error, std::size_t length);

    /* Only handles connection-specific requests, e.g., PING */
    void read_handler(const std::string &content);

    /* ... but write synchronously. */
    void write(const std::string &content);

    /*
     * "Binds" the external read handler, which handles everything
     * this class' read_handler() do not. Bound function may
     * exist in any class.
     */
    void set_ext_read_handler(const read_handler_t &handler)
    {
        ext_read_handler_ = handler;
    }

    /*
     * So that we do not get kicked from the server, and so that the
     * server itself does not have to ping us, which seems preferable
     * in the IRC specification.
     *
     * In a perfect implementation, this should only be used when no
     * command has been sent to the server for a specified amount of time.
     * Currently, the implementation does not adhere to this.
     */
    void ping();

    /* Fail-safe for ping(). */
    void pong();

    /* For a graceful shutdown. */
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

    /*
     * All received data that passed this class'
     * read_handler() goes through this object.
     */
    read_handler_t ext_read_handler_;

    /* Keeping the connection alive. */
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

