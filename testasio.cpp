// Testing ASIO
#include <iostream>
#ifdef _WIN32
#   include <sdkddkver.h>
#endif
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

int client_main(const std::string& addr, unsigned short port)
{
    try
    {
        namespace asio = boost::asio;
        asio::io_service service(1);
        asio::ip::port_type;
        asio::ip::tcp::endpoint ep(asio::ip::address_v4::from_string(addr), port);
        asio::ip::tcp::socket sock(service);
        sock.connect(ep);
        
        std::cout << "Connect to server " << addr << " on port " << port << std::endl;

        std::string in;
        while(true)
        {
            std::cout << "SEND: ";
            if(!(std::cin >> in))
                break;
            boost::system::error_code ec;
            sock.write_some(boost::asio::buffer(in), ec);
            if(ec == asio::error::eof)
            {
                std::cout << "Connection closed" << std::endl;
                return 0;
            }
            else if(ec)
            {
                throw boost::system::system_error(ec);
            }
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Client error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int server_main(unsigned short port)
{
    try
    {
        namespace asio = boost::asio;
        asio::io_service service(1);
        asio::ip::tcp::endpoint ep(asio::ip::address_v4(), port);
        std::cout << "Starting server on port " << port << std::endl;
        asio::ip::tcp::acceptor acc(service, ep);
        asio::ip::tcp::socket sock(service);
        acc.accept(sock);

        char buf[512];
        while(true)
        {
            auto len = sock.read_some(asio::buffer(buf));
            std::string_view view(buf, len);
            std::cout << "RECV: " << view << std::endl;
            if(view == "quit")
            {
                std::cout << "Quitting" << std::endl;
                return 0;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cout << "Server error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    namespace bpo = boost::program_options;
    bpo::options_description desc("Allowed Options");
    desc.add_options()
        ("help", "This message")
        ("ip", bpo::value<std::string>()->default_value("127.0.0.1"), "IP v4 address")
        ("port", bpo::value<unsigned short>()->default_value(514))
        ("mode", bpo::value<std::string>(), "Server/Client");

    bpo::variables_map vm;
    try
    {
        bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
        bpo::notify(vm);
    }
    catch(const std::exception& e)
    {
        std::cout << "Parsing error: " << e.what() << std::endl;
        return 0;
    }
    if(vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }
    if(!vm.count("mode"))
    {
        std::cout << "Missing mode!" << std::endl;
        return 0;
    }

    auto mode = vm["mode"].as<std::string>();
    if(mode == "server")
    {
        return server_main(vm["port"].as<unsigned short>());
    }
    else if(mode == "client")
    {
        return client_main(vm["ip"].as<std::string>(), vm["port"].as<unsigned short>());
    }
    else
    {
        std::cout << "Unknown mode: " << mode << std::endl;
    }
    return 0;
}
