#pragma once

#include "../http/Request.hpp"
#include "../server/Client.hpp"
#include "CgiHandler.hpp"
#include <string>

class CgiSession
{
    public:
        std::string scriptPath;
        int clientFd;
        std::string requestBody;
        size_t writeOffset;
        std::string responseBuf;
        bool writeDone;
        bool readDone;
        time_t startTime;
        bool killSent;

        CgiHandler handler;
        CgiSession(std::string& scriptPath, const Client& client, const Config& config, const Request& request);
        ~CgiSession();
};