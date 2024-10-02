#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Channel
{
    public:

        Channel();
        ~Channel();

        void modif_topic(const std::string &topic);
        

    private:
        const std::string _name;
        std::string _topic;
        std::map<std::string, int> _client;
        std::vector<std::string> _operator;

};

