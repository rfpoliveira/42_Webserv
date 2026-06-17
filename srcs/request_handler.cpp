/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:30:33 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/06/17 16:43:45 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/main.hpp"
#include "../incs/Request.hpp"
#include "../incs/ResponseBuilder.hpp"
#include "../incs/CgiHandler.hpp"

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