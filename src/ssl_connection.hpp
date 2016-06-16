#pragma once
#include "connection.hpp"
#include <boost/asio/ssl.hpp>
#include "general.hpp"

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
typedef ssl::stream<tcp::socket> ssl_socket;

namespace irc {

class ssl_connection : protected connection {
public:
    ssl_connection()
        : ctx_(ssl::context::sslv23), socket_(io_service_, ctx_)
    {
        /* TODO: Catch possible error */
        ctx_.set_default_verify_paths();
    }

    void connect(const std::string &addr, std::string &port, const bool ssl)
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
        throw invalid_argument("The port contains one or more non-integers.");

    else {
        addr_ = addr;
        port_ = port;

        connect();
    }

    }

private:
    boost::asio::io_service io_service_;
    ssl::context ctx_;
    ssl_socket socket_;

    void connect();
};

/* ns irc */
}
