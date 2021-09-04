#pragma once
#include <asio.hpp>
#include <asio/buffer.hpp>
#include <asio/error_code.hpp>
#include <asio/read.hpp>
#include <asio/read_until.hpp>
#include <asio/streambuf.hpp>
#include <cstddef>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <asio/read_until.hpp>
class Connection:public std::enable_shared_from_this<Connection>{
public:
    static std::shared_ptr<Connection> create(asio::io_context& io_context, std::vector<std::string>& messages){
        return std::shared_ptr<Connection>(new Connection(io_context,messages));
    }
    void start(){
        std::thread read_thr(std::bind(&Connection::read,shared_from_this()));
        read_thr.detach();
        std::thread write_thr(std::bind(&Connection::write,shared_from_this()));
        write_thr.detach();
    }
    asio::ip::tcp::socket& getSocket(){
        return m_socket;
    }
private:
    Connection(asio::io_context& io_context, std::vector<std::string>& messages):m_socket(io_context),m_messages(messages),total_sent(0){}
    void read(){;
        //buffer.prepare(128);
        //std::cout << "READ" << std::endl;
        //asio::error_code err;
        asio::async_read_until(m_socket,m_sb,'\n',[self=shared_from_this()](asio::error_code ignored,std::size_t bytes_transferred){
            self->handleRead(bytes_transferred);
        });
    }
    void handleRead(size_t bytes_transferred){
        addMessage(bufToString());
        m_sb.consume(bytes_transferred);
        std::cout << "POG" << std::endl; 
        std::thread read_thr(std::bind(&Connection::read,shared_from_this()));
        read_thr.detach();
    }
    void addMessage(std::string to_add){
        //std::cout << "add message and everything" << std::endl;
        std::cout << to_add << std::endl;
        m_messages.push_back(to_add);
    }
    void write(){
        std::cout << m_messages.size()<< std::endl;
        if(total_sent<m_messages.size()){
            total_sent++;
            std::cout << "OUTPUT:" <<m_messages[total_sent-1] << std::endl;
            asio::async_write(m_socket,asio::buffer(m_messages[total_sent-1]),std::bind(&Connection::handleWrite,shared_from_this()));
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            handleWrite();
        }
    }
    void handleWrite(){
        std::thread write_thr(std::bind(&Connection::write,shared_from_this()));
        write_thr.detach();
    }
    std::string bufToString()
    {
        std::stringstream to_return;
        to_return << m_socket.remote_endpoint() << ": " << std::istream(&m_sb).rdbuf();
        return to_return.str();
    }    
    asio::ip::tcp::socket m_socket;
    asio::ip::tcp::endpoint m_endpoint;
    std::vector<std::string>& m_messages;
    int total_sent;
    asio::streambuf m_sb;
};
