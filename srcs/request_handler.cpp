/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:30:33 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/06/15 12:00:29 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/main.hpp"
#include "../incs/Request.hpp"
#include "../incs/ResponseBuilder.hpp"
#include "../incs/CgiHandler.hpp"

std::string request_hanlder(Request request, Client client) //TODO
{
    Location location = config.getLocation(client.getPort, request.resource_path); //TODO

    if(!location.isMethodallowed(request.request_method)) //TODO
        return(/*some error*/);
    
    if(request.content_length > location.get_body_size()) //TODO
        return(/*some error*/);
    
    if(request.resource_path.find(".py") != std::string::npos)
    {
        CgiHandler CGI_handler(request.resource_path);
        CGI_handler.execute(request);  //TODO
    }
    else
        ResponseBuilder::build_static_file(location.root + request.resource_path);
    
}