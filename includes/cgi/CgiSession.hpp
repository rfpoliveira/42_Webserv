#pragma once

#include <Common.hpp>

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