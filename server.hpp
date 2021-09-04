#pragma once
#include <asio.hpp>
#include <asio/buffer.hpp>
#include <asio/connect.hpp>
#include <asio/error_code.hpp>
#include <asio/io_context.hpp>
#include <memory>
#include <vector>
#include "connection.hpp"
class Server{
public:
    Server(asio::io_context& io,size_t port):m_io(io),m_acceptor(io,asio::ip::tcp::endpoint(asio::ip::tcp::v4(),port)){
        accept();
    }
private:
    void accept(){
        std::shared_ptr<Connection> c=Connection::create(m_io,m_messages);
        m_acceptor.async_accept(c->getSocket(),std::bind(&Server::handleAccept,this,c));
    }
    void handleAccept(std::shared_ptr<Connection> connection){
        //should add error code handler
        std::cout << "Started Connection" << std::endl;
        std::thread thr(std::bind(&Connection::start,connection));
        thr.detach();
        accept();
    }
    std::vector<std::string> m_messages;
    asio::io_context &m_io;
    asio::ip::tcp::acceptor m_acceptor;
};
