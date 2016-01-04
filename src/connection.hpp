#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <functional>
#include <string>

using boost::asio::ip::tcp;

namespace libircppclient {

/*
 * Function pointer which will refer to the function that handles
 * all data received from the server.
 */
typedef std::function<void (const std::string&)> read_handler_t;

/*
 * Function pointer which will refer to ...
 */
typedef std::function<void (void)> write_handler_t;

class connection {
public:
    /* Bind used socket to the io_service */
    connection() : socket_(io_service_) { }

    connection(const std::string &addr, const std::string &port)
        : addr_(addr), port_(port), socket_(io_service_)
    {
        //connect();
    }

    /* Attempt to connect to the server. Throw an error if unsuccessful. */
    void connect();
    void connect(const std::string &addr, const std::string &port);

    /*
     * Asynchronously loop this function and push any read data to
     * the object's read_handler.
     */
    void write(const std::string &content);
    void read(const boost::system::error_code &error, std::size_t length);

    /* TODO explain these */
    void set_read_handler(const read_handler_t &handler)
    {
        read_handler_  = handler;
    }

    void set_write_handler(const write_handler_t &handler)
    {
        write_handler_ = handler;
    }

    void run();
    void close();

    /* Is the connection still alive? */
    bool is_alive() const { return socket_.is_open(); }

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
     *
     * Might change this at a later date, given how IRCv3 seems to work.
     */
    std::array<char, 512> buffer_;
};

/* ns libircppclient */
}

/* #endif CONNECTION_HPP */
#endif
