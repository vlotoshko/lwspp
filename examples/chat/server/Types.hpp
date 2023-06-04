/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date March, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/server/Consts.hpp"
#include "easywebsocket/server/Types.hpp"

namespace ews::chat
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

} // namespace ews::chat
