#include "../../includes/core/main.hpp"
#include "../../includes/http/Request.hpp"
#include "../../includes/http/ResponseBuilder.hpp"
#include "../../includes/cgi/CgiHandler.hpp"

<<<<<<< HEAD
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
=======
// ============================================================================
// PARKED until the Client class exists (build step 1).
//
// The sketch below is kept verbatim as the design intent. It does not compile
// yet because `Client` is not defined, and it has known defects to fix when
// reviving it:
//   - typo in the name: request_hanlder -> request_handler
//   - client.getPort  -> missing parentheses: client.getPort()
//   - getLocation now returns Location* (NULL = no match) -> must null-check
//   - getHeader("Content-Length") is a std::string; convert to a number before
//     comparing against location.max_body_size
//   - the empty `return(/*some error*/)` statements need real error responses
//   - take (const Client&, const Config&) instead of copying both by value
//   - no return on the final path; every branch must produce a std::string
//
// std::string request_handler(const Client& client, const Config& config)
// {
//     Location* location = config.getLocation(client.getPort(),
//                                             client.request.resource_path);
//     if (!location)
//         return (/* 404 */);
//     if (!location->isMethodallowed(client.request.request_method))
//         return (/* 405 */);
//     if (body_size > location->max_body_size)
//         return (/* 413 */);
//     if (client.request.resource_path.find(".py") != std::string::npos)
//     {
//         CgiHandler CGI_handler(client.request.resource_path);
//         CGI_handler.execute(client.request);
//     }
//     else
//         return (ResponseBuilder::build_static_file(location->root +
//                                                    client.request.resource_path));
// }
// ============================================================================
>>>>>>> tmatheusdiniz
