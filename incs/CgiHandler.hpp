/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpedrosa <rpedrosa@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:52:48 by rpedrosa          #+#    #+#             */
/*   Updated: 2026/06/15 12:00:17 by rpedrosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"
#include "Request.hpp"

class CgiHandler
{
    private:
        int pipeIn[2];
        int pipeOut[2];
        pid_t pid;
        std::string script_path;

    public:
        CgiHandler(const std::string script_path);
        ~CgiHandler();

        bool execute(Request request);

        int getReadFd() const;
        int getWriteFd() const;
        pid_t getPid() const;
};
