#include "../../includes/core/main.hpp"
#include "../../includes/http/Request.hpp"
#include "../../includes/http/ResponseBuilder.hpp"
#include "../../includes/cgi/CgiHandler.hpp"

std::string request_hanlder(Client client, Config config) //TODO
{
    Location location = config.getLocation(client.getPort, client.request.resource_path); //TODO

    if(!location.isMethodallowed(client.request.request_method)) //TODO
        return(/*some error*/);
    
    if(client.request.getHeader("Content-Length") > location.max_body_size) //TODO
        return(/*some error*/);
    
    if(client.request.resource_path.find(".py") != std::string::npos)
    {
        CgiHandler CGI_handler(client.request.resource_path);
        CGI_handler.execute(client.request);  //TODO
    }
    else
        ResponseBuilder::build_static_file(location.root + client.request.resource_path);
    
}