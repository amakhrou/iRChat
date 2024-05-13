#include "irc.hpp"

channels::channels(){
    std::cout << "welcome to the channel" << std::endl;
};
channels::~channels(){
    std::cout << "bye bye" << std::endl;
}
bool channels::creat_channel(const std::string &channel_name,const std::string& user_name)
{
    if(channel.find(channel_name) == channel.end())
    {
        channel[channel_name].push_back(user_name);
        return true;
    }
    else
        {
            channel[channel_name].push_back(user_name);
            std::cout << "channel already exist" << std::endl;
            return false;
        }
}

void channels::set_fd_users(const std::string &user_name,int fd)
{
    if(users_fd.find(user_name) == users_fd.end())
    {
        users_fd[user_name].push_back(fd);
    }
    else
    {
        std::cout << "user already exist" << std::endl;
    }
}
bool channels::if_member(std::string user_inchannel, std::string channels_name)
{
    std::vector<std::string>::iterator it1;
    std::map<std::string, std::vector<std::string> >::iterator it;
    it = channel.find(channels_name);
    if(it != channel.end()){
        for(it1 = it->second.begin(); it1 != it->second.end(); it1++)
        {
            // std::cout << "fd: " << fd << std::endl;
            // std::cout << "it1 : " << *it1 << std::endl;
            if(*it1 == user_inchannel)
                return true;
        }
    }
    return(false);
}
int channels::get_fd_users(std::string &users_in_channel)
{
    int fd;
    std::vector<int>::iterator it1;
    std::map<std::string, std::vector<int> >::iterator it;
    it = users_fd.find(users_in_channel);
    for(it1 = it->second.begin(); it1 != it->second.end(); it1++)
    {
        fd = *it1;
    }
    return fd;
}
void channels::send_msg(const std::string &channel_msgs)
{
    std::vector<std::string>::iterator it1;
    std::map<std::string, std::vector<std::string> >::iterator it;
    it = channel.find(channel_msgs);
    for(it1 = it->second.begin(); it1 != it->second.end(); it1++)
    {
        int fd = get_fd_users(*it1);
        std::cout << "fd user : " << fd << std::endl;
    }

}
// int main() {
//     channels server;
//     std::map<std::string, std::vector<int> >::iterator it;
//     std::vector<int>::iterator it1;

//     // Create a channel
//     server.creat_channel("#general", "aymen");

//     // Try to create the same channel again
//     server.creat_channel("#gene", "taha");
//     server.creat_channel("#ger", "user");
//     // server.creat_channel("#general", "asee");
//     // server.creat_channel("#gene");
//     // for(it = server.channel.begin(); it != server.channel.end(); it++) {
//     //     std::cout << "Channel: " << it->first << std::endl;
//     //     std::cout << "Users: ";
//     //     for(it1 = it->second.begin(); it1 != it->second.end(); it1++) {
//     //         std::cout << *it1 << " ";
//     //     }
//     //     std::cout << std::endl;
//     // }

//     server.get_fd_users("aymen", 1);
//     server.get_fd_users("taha", 2);
//     server.get_fd_users("ss", 3); // Try adding existing user
//     server.get_fd_users("user3", 4);
//     for(it = server.users_fd.begin(); it != server.users_fd.end(); it++) {
//         std::cout << "user: " << it->first << std::endl;
//         std::cout << "fd: ";
//         for(it1 = it->second.begin(); it1 != it->second.end(); it1++) {
//             std::cout << *it1 << " ";
//         }
//         std::cout << std::endl;
//     }
//     return 0;
// }
int main() {
    channels manager;

    // Test create_channel function
    std::cout << "Testing create_channel function:" << std::endl;
    bool result1 = manager.creat_channel("#channel1", "user1");
    bool result2 = manager.creat_channel("#channel2", "user2");
    bool result3 = manager.creat_channel("#channel1", "user3"); // Try creating existing channel
    std::cout << "Result 1: " << (result1 ? "Success" : "Failed") << std::endl;
    std::cout << "Result 2: " << (result2 ? "Success" : "Failed") << std::endl;
    std::cout << "Result 3: " << (result3 ? "Success" : "Failed") << std::endl;

    // Test get_fd_users function
    std::cout << "\nTesting get_fd_users function:" << std::endl;
    manager.set_fd_users("user1", 1);
    manager.set_fd_users("user2", 2);
    manager.set_fd_users("user1", 3); // Try adding existing user
    manager.set_fd_users("user3", 4);

    // Test if_member function
    std::cout << "\nTesting if_member function:" << std::endl;
    bool result4 = manager.if_member("user1", "#channel1"); // Check if user with fd 1 is a member of #channel1
    bool result5 = manager.if_member("user2", "#channel1"); // Check if user with fd 2 is a member of #channel1
    bool result6 = manager.if_member("user1", "#channel2"); // Check if user with fd 3 is a member of #channel2
    std::cout << "Result 4: " << (result4 ? "User is a member" : "User is not a member") << std::endl;
    std::cout << "Result 5: " << (result5 ? "User is a member" : "User is not a member") << std::endl;
    std::cout << "Result 6: " << (result6 ? "User is a member" : "User is not a member") << std::endl;
    manager.send_msg("#channel1");
    manager.send_msg("#channel2");

    return 0;
}
//msg from the user check the channel where the msg sent and check the users and fds to send msg to everyone 