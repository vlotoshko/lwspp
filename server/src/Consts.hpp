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
const std::string UNDEFINED_FILE_PATH = "UNDEFINED_FILE_PATH";
const std::string UNDEFINED_NAME = "UNDEFINED_NAME";
const int UNDEFINED_UNSET = 0;

const SessionId ALL_SESSIONS = static_cast<int>(0U - 1);
const unsigned int MAX_PATH_SIZE = 1024 * 4;
const std::string DEFAULT_PROTOCOL_NAME = "/";
// 7 = LLL_ERR | LLL_WARN | LLL_NOTICE - default value for the libwebsockets 4.3.2
const int DEFAULT_LWS_LOG_LEVEL = 7;

} // namespace ews::srv
