#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <functional>
#include <string>

using boost::asio::ip::tcp;

namespace libircppclient {

/* void function_name(const std::string&) */
typedef std::function<void (const std::string&)> read_handler_t;

/* void function_name() */
typedef std::function<void (void)> write_handler_t;

class connection {
public:
    /* Bind used socket to the io_service */
    connection() : socket_(io_service_) { }

    connection(const std::string &addr, const std::string &port)
        : addr_(addr), port_(port), socket_(io_service_)
    {
        connect();
    }

    /*
     * Start the initial connection attempt and pass the rest of
     * the process to connection_handler().
     */
    void connect();
    void connect(const std::string &addr, const std::string &port);
    void connection_handler(const boost::system::error_code &error, tcp::resolver::iterator endpt_it);

    /*
     * Asynchronously loop this function and push any read data to
     * the object's read_handler.
     */
    void write(const std::string &content);
    void read(const boost::system::error_code &error, std::size_t length);

    void run();
    void close();

    /* Is the connection still alive? */
    bool alive() const
    {
        /*
         * Should io_service::stopped() be returned instead?
         * Although, we're just checking the connection
         * itself.
         */
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
    write_handler_t write_handler_;

    /*
     * 512B is the max message length within the IRC protocol.
     * Given that every message is terminated with CR-LF
     * (Carriage Return - Line Feed), we will have 510B of actual data
     * to process.
     *
     * It's worth noting that several messages can be caught within buffer.
     * Not all messages sent and received from the server are 512B in length.
     */
    std::array<char, 512> buffer_;
};

/* namespace libircppclient */
}

/* #endif CONNECTION_HPP */
#endif
