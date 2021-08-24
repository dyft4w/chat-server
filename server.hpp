#pragma once
#include <asio.hpp>
#include <asio/error_code.hpp>
#include <asio/io_context.hpp>
#include <memory>
#include <vector>
#include "connection.hpp"
class Server{
public:
    Server(asio::io_context& io,int port):m_io(io),m_acceptor(m_io,asio::ip::tcp::v4(),port),m_socket(m_io){
        accept();
    }
private:
    void accept(){
        std::shared_ptr<Connection> c=Connection::create(m_io,m_messages);
        m_acceptor.async_accept(c->getSocket(),std::bind(&Server::handleAccept,this,c));
    }
    void handleAccept(std::shared_ptr<Connection> connection){
        //should add error code handler
        connection->start();
        accept();
    }
    std::vector<std::string> m_messages;
    asio::io_context &m_io;
    asio::ip::tcp::acceptor m_acceptor;
    asio::ip::tcp::socket m_socket;
};