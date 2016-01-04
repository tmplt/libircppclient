#include "connection.hpp"
#include <boost/thread.hpp>
#include <thread>

using boost::asio::ip::tcp;

namespace libircppclient {

void connection::connect()
{
    /* Resolve the host and generate a list of endpoints. */
    tcp::resolver r(io_service_);
    tcp::resolver::query query(addr_, port_);
    tcp::resolver::iterator endpt_it = r.resolve(query);

    /* Denotes the end of the list of generated endpoints. */
    decltype(endpt_it) end;

    /* Default error */
    boost::system::error_code error = boost::asio::error::host_not_found;

    /* Iterate until we've reached the end of the list. */
    while (endpt_it != end) {
        if (!error)
            break;

        socket_.close();
        socket_.connect(*endpt_it++, error);
    }

    if (error)
        throw error;
}

void connection::connect(const std::string &addr, const std::string &port)
{
    addr_ = addr;
    port_ = port;

    connect();
}

void connection::run()
{
    std::thread write_handler_thread(write_handler_);

    /*
     * Start an asynchronous read thread going through connection::read().
     * Pass the arguments (this, _1, _2) to the handler.
     */
    socket_.async_read_some(boost::asio::buffer(buffer_),
                            boost::bind(&connection::read,
                                        this, boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred()));

    io_service_.run();

    /*
     * Blocks the continuation of the function until
     * write_handler_thread is done.
     */
    write_handler_thread.join();
}

void connection::write(const std::string &content)
{
    /*
     * The IRC protocol specifies that all messages sent to the server
     * must be terminated with CR-LF (Carriage Return - Line Feed)
     */
    boost::asio::write(socket_, boost::asio::buffer(content + "\r\n"));
}

void connection::read(const boost::system::error_code &error, std::size_t length)
{
    if (error) {
        /* Unable to read from server. */
        throw error;
    } else {
        /*
         * Works in synergy with socket::async_read_some().
         *
         * Copy the data within the buffer and the length of it
         * and pass it to the class' read_handler.
         */
        read_handler_(std::string(buffer_.data(), length));

        /*
         * Start an asynchronous recursive read thread.
         *
         * Read data from the IRC server into the buffer, and
         * call this function again.
         *
         * Pass the eventual error and the message length.
         */
        socket_.async_read_some(boost::asio::buffer(buffer_),
                                boost::bind(&connection::read,
                                            this, boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }
}

void connection::close()
{
    socket_.close();
    io_service_.stop();
}

/* ns libircppclient */
}
