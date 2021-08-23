#pragma once
#include <asio.hpp>
class Connection{
public:
    Connection(asio::io_context& io_context, std::vector<std::string>& messages):m_socket(io_context),m_messages(messages){

    }
    void start(){

    }
    asio::ip::tcp::socket& getSocket(){
        return m_socket;
    }
private:
    asio::ip::tcp::socket m_socket;
    asio::ip::tcp::endpoint m_endpoint;
    std::vector<std::string>& m_messages;
};