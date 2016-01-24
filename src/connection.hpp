#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <functional>
#include <string>

using boost::asio::ip::tcp;

namespace libircppclient {

/*
 * Type which refer to a function that handles
 * specific data received from the server.
 */
typedef std::function<void (const std::string&)> read_handler_t;

/* Only used to keep the connection alive. */
typedef std::function<void (void)> ping_t;

class connection {
public:
    /* Bind used socket to the io_service. */
    connection() : socket_(io_service_) { }

    /*
     * Check if given data is valid, then attempt to connect to the
     * server. Throw an error if unsuccessful.
     */
    void connect(const std::string &addr, const std::string &port);

    /*
     * While not directly the connection itself, these functions refer to
     * the io_service and the loop itself.
     */
    void run();
    void stop();

    /*
     * Asynchronously loop this function and push any read data to
     * the object's read_handler.
     */
    void read(const boost::system::error_code &error, std::size_t length);
    void write(const std::string &content);

    /*
     * Binds the read handler to a function not necessarily in the
     * current class.
     */
    void set_read_handler(const read_handler_t &handler)
    {
        read_handler_  = handler;
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
     * control variable for this?
     */
    void ping();

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

    read_handler_t  read_handler_;

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

/* ns libircppclient */
}

/* #endif CONNECTION_HPP */
#endif
