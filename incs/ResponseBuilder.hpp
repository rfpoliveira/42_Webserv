/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:46:35 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/06/15 11:52:04 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Request.hpp"

#pragma once

class ResponseBuilder
{
    public:
        static std::string build_static_file(const std::string& file_path);
        
        //TODO ERROS?

};