#include "../../includes/core/main.hpp"
#include "../../includes/http/Request.hpp"
#include "../../includes/http/ResponseBuilder.hpp"
#include "../../includes/cgi/CgiHandler.hpp"

std::string request_handler(Client client, Config config)
{
    Location location = config.getLocation(client.getPort, client.request.resource_path);

    location.isMethodallowed(client.request.request_method)
    
    if(client.request.getHeader("Content-Length") > location.max_body_size)
        throw HttpException("Content Lenght bigger than max body size");
    
    if(client.request.resource_path.find(".py") != std::string::npos)
    {
        CgiHandler CGI_handler(client.request.resource_path);
        if (!(CGI_handler.execute(client.request)))
            throw HttpException("CGI Failed");
    }
    else
        ResponseBuilder::build_static_file(location.root + client.request.resource_path); //TODO
    
}