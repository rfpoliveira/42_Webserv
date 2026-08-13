#pragma once

#include "Client.hpp"
#include "../config/Config.hpp"

std::string requestHandler(const Client& client, Config& config);
static std::string buildMinimalResponse(int code, const std::string& reason, const std::string& body);

std::string handleDelete(const Request& req, std::string root);
