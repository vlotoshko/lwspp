/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/server/ServerVersions.hpp"
#include "easywebsocket/server/Types.hpp"

namespace ews::srv
{

const ServerVersion UNDEFINED_SERVER_VERSION = ServerVersion::Undefined;
const Port UNDEFINED_PORT = static_cast<int>(0U - 1);
const SessionId ALL_SESSIONS = static_cast<int>(0U - 1);
const std::string DEFAULT_PROTOCOL_NAME = "/";

const std::string UNDEFINED_FILE_PATH = "UNDEFINED_FILE_PATH";
const std::string UNDEFINED_NAME = "UNDEFINED_NAME";

const unsigned int MAX_MESSAGE_SIZE = 1024 * 64;
const unsigned int MAX_PATH_SIZE = 1024 * 4;
const int UNDEFINED_UNSET = 0;

} // namespace ews::srv
