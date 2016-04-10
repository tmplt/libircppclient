#include "connection.hpp"
#include <boost/thread.hpp>
#include "general.hpp"
#include <thread>

namespace irc {

void connection::connect(const std::string &addr, std::string &port, const bool ssl)
{
    using std::invalid_argument;

    if (addr.empty())
        throw invalid_argument("The adress is empty.");

    else {
        std::string ret = gen::valid_addr(addr);

        if (!ret.empty()) {
            std::string reason = "Invalid address, reason: " + ret;
            throw invalid_argument(reason);
        }
    }

    if (port.empty())
        port = (ssl ? "6697" : "6667");

    else if (!gen::is_integer(port))
        throw invalid_argument("The port contains one or more non-integer.");

    else {
        addr_ = addr;
        port_ = port;

        connect();
    }
}

void connection::connect()
{
    using boost::asio::ip::tcp;

    /*
     * Resolve the host and generate a list of endpoints.
     * An endpoint is the information used to connect to an address.
     * An address may have more than one endpoint.
     */
    tcp::resolver r(io_service_);
    tcp::resolver::query query(addr_, port_);
    tcp::resolver::iterator endpt_it = r.resolve(query);

    /* Denotes the end of the list of generated endpoints. */
    decltype(endpt_it) end;

    /* Default error. */
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

void connection::run()
{
    using namespace boost;

    std::thread ping_handler_thread(ping_handler_);

    /*
     * Start an asynchronous read thread going through connection::read().
     * Pass the arguments (this, _1, _2) to the handler.
     */
    socket_.async_read_some(asio::buffer(buffer_),
        bind(&connection::read,
            this, asio::placeholders::error,
            asio::placeholders::bytes_transferred()));

    io_service_.run();

    /* Remain at this point until we do not need the connection any more. */
    ping_handler_thread.join();
}

void connection::ping()
{
    using namespace std::literals;

    while (do_ping) {

        /*
         * Interval decided by mimicing WeeChat.
         * The standard does not seem to explicitly
         * state a ping inverval. Is it the server's
         * decision?
         */
        std::this_thread::sleep_for(1min + 30s);

        /* For debugging */
        std::cout << "[info] Pinging " << addr_ << '.' << std::endl;
        write("PING " + addr_);
    }
}

void connection::pong()
{
    write("PONG :" + addr_);
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
    using namespace boost;

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
        read_handler(std::string(buffer_.data(), length));

        /*
         * Start an asynchronous recursive read thread.
         *
         * Read data from the IRC server into the buffer, and
         * call this function again.
         *
         * Pass the eventual error and the message length.
         */
        socket_.async_read_some(asio::buffer(buffer_),
            bind(&connection::read,
                this, asio::placeholders::error,
                asio::placeholders::bytes_transferred));
    }
}

void connection::read_handler(const std::string &content)
{
    std::stringstream iss(content);
    std::string command;

    iss >> command;

    if (command == "PING")
        pong();
    else
        ext_read_handler_(content);
}

void connection::stop()
{
    socket_.close();
    io_service_.stop();
}

/* ns irc */
}
