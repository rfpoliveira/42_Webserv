#include <HandlerOutcome.hpp>

HandlerOutcome::HandlerOutcome()
{
    type = CGI_COMPLETE;
    response = "0";
    cgiSession = NULL;
}
HandlerOutcome::HandlerOutcome(int type, std::string response, CgiSession *cgiSession)
{
    this->type = type;
    this->response = response;
    this->cgiSession = cgiSession;
}
HandlerOutcome::~HandlerOutcome() {};