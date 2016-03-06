#pragma once
#include <boost/asio.hpp>
#include <functional>
#include <string>

using boost::asio::ip::tcp;

namespace irc {

/*
 * Type which refer to a function that handles
 * specific data received from the server.
 */
typedef std::function<void (const std::string &content)> read_handler_t;

/* Only used to keep the connection alive. */
typedef std::function<void (void)> ping_t;

class connection {
public:
    /* Bind used socket to the io_service. */
    connection() : socket_(io_service_) { }

    /*
     * Check if given data is valid, then attempt to connect to the
     * server. Throw an error if unsuccessful or if the data is invalid.
     */
    void connect(const std::string &addr, const std::string &port);

    /*
     * While not the connection itself, these functions refer to
     * the io_service and the loop itself.
     */
    void run();
    void stop();

    /*
     * Asynchronously loop this function and push any read data to
     * read_handler().
     */
    void read(const boost::system::error_code &error, std::size_t length);

    /* Only handles connection-specific requests, e.g., PING */
    void read_handler(const std::string &content);

    /* But write synchronously. */
    void write(const std::string &content);

    /*
     * Binds the external read handler, which handles everything
     * this class' read_handler() do not. Bound function may
     * exist in any class.
     */
    void set_ext_read_handler(const read_handler_t &handler)
    {
        ext_read_handler_  = handler;
    }

    /*
     * So that we do not get kicked from the server, and so that the
     * server itself does not have to ping us, which seems preferable
     * in the specification.
     *
     * In a perfect implementation, this should only be used when no
     * data has been sent to the server for a specified amount of time.
     * Currently it will ping disregarding if a message to the server has
     * been sent or not within the time-frame. Should we implement a
     * timer for this?
     */
    void ping();

    /* TODO: implement */
    void reset_timer();

    /* Fail-safe for ping() */
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

private:
    /* Server information. */
    std::string addr_;
    std::string port_;

    /* Required by any program using boost::asio. */
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::socket socket_;

    /*
     * All received data that passed this class'
     * read_handler() goes through this object.
     */
    read_handler_t ext_read_handler_;

    /* Keeping the connection alive. */
    ping_t ping_handler = std::bind(&connection::ping, this);
    bool   do_ping = true;

    /*
     * As the over-loaded function assigns addr and port,
     * and then calls base connect(), this is a private
     * member function.
     */
    void connect();

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
};

/* ns irc */
}
