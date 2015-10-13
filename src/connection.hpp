#ifdef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>sdasdasd
#include <string>

using boost::asio::ip::tcp;

namespace irc_client {

class connection {
    public:
        // Setting up the required variables.
        void config();

        // Connecting to the server.
        connection() : m_socket(m_io_service) {}

        connection(const std::string &addr, const std::string &port)
            : m_addr(addr), m_port(port), m_socket(m_io_service)
        {
            connect();
        }

        void connect();

        // set constants such as address, port, nick, channels, etc
        void config();
        void read();
        void write(const std::string &content);
        
    private:
        std::string m_addr;
        std::string m_port;

        // Any program that uses asio needs to have at least one io_service object.
        boost::asio::io_service  m_io_service;
        
        // 
        tcp::socket m_socket;
        
        // Convert the server name to a TCP endpoint.
        //
        // "A resolver takes a query object and turns it into a list of endpoints.
        // We construct a query using the name of the server specified elsewhere
        // (i.e. in argv[1]) and the name of the service, in this case "irc".
        
        // Probably a good idea to create this thing in-line as it's only used
        //tcp::resolver;
};

} // ns irc

#endif
