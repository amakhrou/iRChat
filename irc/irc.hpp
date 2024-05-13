#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
class channels {
        public:
            std::map<std::string, std::vector<std::string> > channel;
            std::map<std::string, std::vector<int> > users_fd;
    
            channels();
            ~channels();
            void    set_fd_users(const std::string &user_name,int fd);
            bool    if_member(std::string user_inchannel, std::string channels_name);
            void    send_msg(const std::string &channel_msg);
            int     get_fd_users(std::string &users_in_channel);
            bool    creat_channel(const std::string &channel_name,const std::string& user_name);
};
#endif
