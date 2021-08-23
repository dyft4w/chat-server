//https://www.github.com/dyft4w
//WARNING : ACTUALLY USING THIS IS THE SINGLE WORST THING YOU COULD POSSIBLY DO AS IT HAS MANY SECURITY FLAWS (for now)
#include "server.hpp"
#include <asio.hpp>
#include <asio/io_context.hpp>
#include <exception>
#include <iostream>
#include <string>
int main(int argc, char* argv[]){
    if(argc!=2){
        std::cout << "Usage: chat-server <port>\n";
    }
    try{
        asio::io_context io;
        Server server(io,std::stoi(argv[1]));
        io.run();
    }catch(std::exception& e){
        std::cerr << "Exception : " << e.what() << std::endl;
    }
}