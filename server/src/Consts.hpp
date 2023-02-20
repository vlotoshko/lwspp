/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "websocketpp/server/ServerVersions.hpp"
#include "websocketpp/server/Types.hpp"

namespace wspp::srv
{

const ServerVersion UNDEFINED_SERVER_VERSION = ServerVersion::Undefined;
const int UNDEFINED_PORT = static_cast<int>(0U - 1);
const SessionId ALL_SESSIONS = static_cast<int>(0U - 1);
const unsigned int MAX_MESSAGE_SIZE = 1024 * 64;

} // namespace wspp::srv
