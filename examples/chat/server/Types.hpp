/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date March, 2023
 */

#pragma once

#include <string>

#include "websocketpp/server/Consts.hpp"
#include "websocketpp/server/Types.hpp"

namespace wspp::chat
{

struct User
{
    srv::SessionId sessionId = srv::UNDEFINED_SESSION_ID;
    std::string userName;
};

struct Message
{
    User from;
    User to;
    std::string text;
};

} // namespace wspp::chat
