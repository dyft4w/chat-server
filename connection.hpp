#pragma once
#include <asio.hpp>
#include <asio/error.hpp>
#include <asio/error_code.hpp>
#include <asio/streambuf.hpp>
#include <boost/bind.hpp>
#include <chrono>
#include <memory>
#include <thread>
#include <iostream>
class Connection:public std::enable_shared_from_this<Connection>{
public:
    static std::shared_ptr<Connection> create(asio::io_context& io_context, std::vector<std::string>& messages){
        return std::shared_ptr<Connection>(new Connection(io_context,messages));
    }
    void start(){
        read();
        write();
    }
    asio::ip::tcp::socket& getSocket(){
        return m_socket;
    }
private:
    Connection(asio::io_context& io_context, std::vector<std::string>& messages):m_socket(io_context),m_messages(messages),total_sent(0){

    }
    void read(){
        char* buffer;
        asio::async_read(m_socket,asio::buffer(buffer, strlen(buffer)),std::bind(&Connection::handleRead,this,buffer));
    }
    void handleRead(std::string string){
        if(string.size()>0)
            addMessage(string);
        read();
    }
    void addMessage(std::string to_add){
        std::cout << "add message and everything" << std::endl;
        m_messages.push_back(to_add);
    }
    void write(){
        asio::error_code error;
        if(total_sent<m_messages.size()){
            asio::async_write(m_socket,asio::buffer(m_messages[total_sent]),std::bind(&Connection::handleWrite,shared_from_this()));
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            handleWrite();
        }
    }
    void handleWrite(){
        total_sent++;
        write();
    }
    asio::ip::tcp::socket m_socket;
    asio::ip::tcp::endpoint m_endpoint;
    std::vector<std::string>& m_messages;
    int total_sent;
};