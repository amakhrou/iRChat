#include "multuplixing.hpp"
#include "../Server.hpp"
#include "../parse/parse.hpp"
#define welcome "      \\  \\     |\\  \\|\\  ___ \\ |\\  \\     |\\   ____|\\|\\   __  \\|\\   _ \\  _   \\|\\  ___ \\     \\\n" \
                " \\  \\    \\ \\  \\ \\   __/|\\ \\  \\    \\ \\  \\___|\\ \\  \\|\\  \\ \\  \\\\\\__\\ \\  \\ \\   __/|    \\\n" \
                "  \\ \\  \\  __\\ \\  \\ \\  \\_|/_\\ \\  \\    \\ \\  \\    \\ \\  \\\\\\  \\ \\  \\|__| \\  \\ \\  \\_|/__  \\\n" \
                "   \\ \\  \\|\\__\\_\\  \\ \\  \\_|\\ \\ \\  \\____\\ \\  \\____\\ \\  \\\\\\  \\ \\  \\    \\ \\  \\ \\  \\_|\\ \\ \\\n" \
                "    \\ \\____________\\ \\_______\\ \\_______\\ \\_______\\ \\_______\\ \\__\\    \\ \\__\\ \\_______\\\n" \
                "     \\|____________|\\|_______|\\|_______|\\|_______|\\|_______|\\|__|     \\|__|\\|_______|\r\n"


void send_message(int client_socket, const char* message) {
    send(client_socket, message, std::strlen(message), 0);
}

void send_welcome_message(int client_socket, server *ser, std::string nick) {
    (void )ser;
    std::string tmp = ":irc.ChatWladMina 001 " + nick + " :Welcome to the ChatWladMina Network, " + nick + "\r\n";
    send_message(client_socket, tmp.c_str());
}

void send_host_info(int client_socket, server *ser, std::string nick) {
    std::string tmp = ":irc.ChatWladMina 002 " + nick + " :Your host is ChatWladMina, running version 4.5" + "\r\n";
    (void )ser;
    send_message(client_socket, tmp.c_str());
}

void send_creation_date(int client_socket, server *ser, std::string nick) {
    (void )ser;
    std::string tmp = ":irc.ChatWladMina 003 " + nick + " :This server was created " + "\r\n";
    send_message(client_socket, tmp.c_str());
}

void send_server_info(int client_socket, server *ser, std::string nick) {
    (void )ser;
    std::string tmp = ":irc.ChatWladMina 004 " + nick + " ChatWladMina ChatWladMina(enterprise)-2.3(12)-netty(5.4c)-proxy(0.9) ChatWladMina" + "\r\n";
    send_message(client_socket, tmp.c_str());
}

void poll_fds(int fd, server *ser) {
    struct pollfd newPollfd;
    newPollfd.fd = fd;
    newPollfd.events = POLLIN;
    newPollfd.revents = 0;
    ser->_fd.push_back(newPollfd);
}

std::vector<client> removeClient(std::vector<client> mycl, int fd)
{
    for (std::vector<client>::iterator it = mycl.begin(); it != mycl.end(); it++)
    {
        if (fd == it->fd){
            mycl.erase(it);
            close (fd);
            return mycl;
        }
    }
    close (fd);
    return mycl;
}
client *AddClient(int newFd, server *ser)
{
    client *tmp = new client;
    poll_fds(newFd, ser);
    tmp->fd = newFd;
    return tmp;
}

client *getClient(int fd, std::vector<client> &mycl)
{
    std::vector<client>::iterator it = mycl.begin();
    for (; it != mycl.end(); it++)
    {
        if (fd == it->fd)
            return &(*it);
    }
    return &(*it);
}

void handleCtrlZ(int signum) {
    std::cout << "Received signal: " << signum << std::endl;
    if(signum == SIGTSTP) {
        std::cout << "Ctrl+Z pressed. Suspending process..." << std::endl;
        std::cout << "Process resumed after suspension." << std::endl;
    }
}

void mult(server *ser) {
    Server s;
    (void )s;
    char buff[1024];
    ser->indice = 0;
    std::vector <client> mycl;
    std::cout << "waiting connection.." << std::endl; 
    signal(SIGTSTP, handleCtrlZ);
    while (1) {
        if (poll(ser->_fd.data(), ser->_fd.size(), -1) == -1)
            throw std::runtime_error("poll");
        for (size_t i = 0; i < ser->_fd.size(); i++) {
            if (ser->_fd[i].revents & POLLIN) {
                if (ser->sock == ser->_fd[i].fd)
                {
                    int newFd = accept(ser->sock , NULL, NULL);
                    if (newFd == -1)
                        throw std::runtime_error("ERROR IN ACCEPTING\n");
                    if (fcntl(newFd, F_SETFL, O_NONBLOCK) == -1)
                        throw std::runtime_error("Error: fcntl() failed");
                    std::string r = welcome;
                    send_message(newFd, r.c_str());
                    client *tmp = AddClient(newFd, ser);
                    tmp->setIndice(0);
                    send_message(newFd, "pleas enter ur pass:(in this format /quote pass [PASS]\r\n");
                    mycl.push_back(*tmp);
                    parseBuff(newFd, buff, ser, tmp, &s, mycl);
                    delete tmp;
                }      
                else {
                    memset(buff, 0, std::strlen(buff));
                    int nbytes = recv(ser->_fd[i].fd, buff, sizeof(buff), 0);
                        s.process_data(ser->_fd[i].fd, nbytes, buff, ser, getClient(ser->_fd[i].fd, mycl), mycl, &s);
                }
            }
        }
    }
}

void getSocket(server *ser)
{
    if ((ser->sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        throw ("creating socket");
    }
    int optval = 1;
    if (setsockopt(ser->sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        throw std::runtime_error("Error: setsockopt() failed");
    fcntl(ser->sock, F_SETFL, O_NONBLOCK);
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(ser->port);
    sa.sin_addr.s_addr = INADDR_ANY;
    if (bind(ser->sock, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        throw std::runtime_error("Error: failed to bind socket");
    };
    if ((ser->listen_fd = listen(ser->sock, SOMAXCONN)) == -1){
        close(ser->sock);
        throw std::runtime_error("error in listen");
    }
    poll_fds(ser->sock, ser);
}
