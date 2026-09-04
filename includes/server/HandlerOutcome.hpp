#pragma once

#include <CgiSession.hpp>
#include <string>

#define CGI_COMPLETE 0
#define CGI_PENDING 1

class HandlerOutcome
{
    public:
        int type;
        std::string response; //for the COMPLETE type
        CgiSession *cgiSession; //for the CGI_PENDING type

        HandlerOutcome();
        HandlerOutcome(int type, std::string response, CgiSession *cgiSession);
        ~HandlerOutcome();
};