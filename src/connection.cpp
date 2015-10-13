#include "connection.hpp"

using boost::asio::ip::tcp;

namespace irc_client {

void connection::connect()
{
    tcp::resolver(m_io_service);
    tcp::query query(m_addr, m_port);
    boost::system::error_code error = boost::asio::error::host_not_found;
}

} // ns irc_client
