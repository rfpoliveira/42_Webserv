#include "../../includes/cgi/CgiSession.hpp"

CgiSession::CgiSession(std::string& scriptPath, const Client& client, const Config& config, const Request& request): handler(scriptPath, client, config, request)
{
    this->scriptPath = scriptPath;
    clientFd = client.getFd();
    requestBody = request.body;
    writeOffset = 0;
    writeDone = false;
    readDone = false;
    startTime = time(NULL);
    killSent = false;
}

CgiSession::~CgiSession() {}
