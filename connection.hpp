#pragma once
#include <asio.hpp>
#include <asio/error.hpp>
#include <asio/error_code.hpp>
#include <asio/streambuf.hpp>
#include <chrono>
#include <thread>
#include <iostream>
class Connection{
public:
    Connection(asio::io_context& io_context, std::vector<std::string>& messages):m_socket(io_context),m_messages(messages),total_sent(0){

    }
    void start(){
        read();
        write();
    }
    asio::ip::tcp::socket& getSocket(){
        return m_socket;
    }
private:
    void read(){
        asio::streambuf sb;
        asio::error_code error;
        asio::async_read(m_socket,sb,error,std::bind(&Connection::handleRead,this,error,sb));
    }
    void handleRead(asio::error_code error,asio::streambuf sb){
        if(error==asio::error::eof){
            addMessage(sb);
        }else{
            read();
        }
    }
    void addMessage(asio::streambuf& sb){
        std::cout << "add message and everything" << std::endl;
    }
    void write(){
        asio::error_code error;
        if(total_sent<m_messages.size()){
            asio::async_write(m_socket,asio::buffer(m_messages[total_sent]),error,std::bind(&Connection::handleWrite,this,error));
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            handleWrite(error);
        }
    }
    void handleWrite(asio::error_code error){
        if(error==asio::error::eof){
            total_sent++;
        }
        write();
    }
    asio::ip::tcp::socket m_socket;
    asio::ip::tcp::endpoint m_endpoint;
    std::vector<std::string>& m_messages;
    int total_sent;
};