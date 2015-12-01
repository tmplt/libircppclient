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

    /* Copy the first possible endpoint from the generated list. */
    tcp::endpoint endpoint = *endpt_it;

    /*
     * Asynchronously connect to the server via connection::connection_handler.
     * Pass the arguments (this, _1, _2) to the handler.
     */
    socket_.async_connect(endpoint,
                          boost::bind(&connection::connection_handler,
                                      this, boost::asio::placeholders::error,
                                      ++endpt_it));
}

void connection::connect(const std::string &addr, const std::string &port)
{
    addr_ = addr;
    port_ = port;

    connect();
}

void connection::connection_handler(const boost::system::error_code &error,
                                    tcp::resolver::iterator endpt_it)
{
    if (!error) {
        /* Successful connection. */

    } else if (tcp::resolver::iterator() != endpt_it) {
        /* If we have yet to reach the end of generated endpoints: */
        socket_.close();

        /* Use next endpoint in list. */
        tcp::endpoint endpoint = *endpt_it;

        /*
         * Iterate through this function until we have a valid connection
         * or until we have run out of possible endpoints.
         *
         * This process is asynchronous; other blocks of code will run
         * during the same time as this.
         */
        socket_.async_connect(endpoint,
                              boost::bind(&connection::connection_handler,
                                          this, boost::asio::placeholders::error,
                                          ++endpt_it));
    } else {
        /* Unable to connect; we have run out of endpoints. */

        /*
         * Handle these thrown errors somewhere. Possibly
         * in libircppclient.cpp?
         */
        throw error;
    }
}

void connection::run()
{
    std::thread write_handler_thread(write_handler_);

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
        /* Unable to read. */
        throw error;
    } else {
        /*
         * Works in synergy with socket::async_read_some.
         *
         * Copy the data within the buffer and the length of it
         * and pass it to the object's read_handler.
         *
         * Should this not be placed after the next statement?
         * Otherwise the first run through this function
         * will pass useless data to the read_handler.
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

/* namespace end */
}