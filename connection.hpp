#pragma once
#include <asio.hpp>
#include <asio/buffer.hpp>
#include <asio/error.hpp>
#include <asio/error_code.hpp>
#include <asio/read_until.hpp>
#include <asio/streambuf.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
class Connection:public std::enable_shared_from_this<Connection>{
public:
    static std::shared_ptr<Connection> create(asio::io_context& io_context, std::vector<std::string>& messages){
        return std::shared_ptr<Connection>(new Connection(io_context,messages));
    }
    void start(){
        std::stringstream message;
        message << m_socket.remote_endpoint() << " joined" << std::endl;
        addMessage(message.str());
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
    void read(){
        asio::async_read_until(m_socket,m_sb,'\n',[self=shared_from_this()](asio::error_code err_code,std::size_t bytes_transferred){
            if(err_code){
                std::stringstream message;
                message << self->m_socket.remote_endpoint() << " left" << std::endl;
                self->addMessage(message.str());
                //shutdown socket
                self->m_socket.shutdown(asio::ip::tcp::socket::shutdown_both);
            }else{
                self->handleRead(bytes_transferred);
            }
        });
    }
    void handleRead(size_t bytes_transferred){
        //get string from m_sb
        addMessage(bufToString());
        m_sb.consume(bytes_transferred);
        //creates a new thread because they exit all the time
        std::thread read_thr(std::bind(&Connection::read,shared_from_this()));
        read_thr.detach();
    }
    void addMessage(std::string to_add){
        //no std::endl because there is already a \n in the string
        std::cout << to_add;
        m_messages.push_back(to_add);
    }
    void write(){
        if(total_sent<m_messages.size()){
            total_sent++;
            asio::async_write(m_socket,asio::buffer(m_messages[total_sent-1]),std::bind(&Connection::handleWrite,shared_from_this()));
        }else{
            //can be removed, but i leave it to unload the CPU a bit
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            handleWrite();
        }
    }
    void handleWrite(){
        std::thread write_thr(std::bind(&Connection::write,shared_from_this()));
        write_thr.detach();
    }
    std::string bufToString(){
        //converts asio::streambuf to std::string
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
