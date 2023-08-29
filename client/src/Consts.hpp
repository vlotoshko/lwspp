/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "easywebsocket/client/ClientVersions.hpp"
#include "easywebsocket/client/Types.hpp"

namespace ews::cli
{

const ClientVersion UNDEFINED_CLIENT_VERSION = ClientVersion::Undefined;
const Address UNDEFINED_ADDRESS = "UNDEFINED_ADDRESS";
const Port UNDEFINED_PORT = static_cast<int>(0U - 1);
const std::string UNDEFINED_FILE_PATH = "UNDEFINED_FILE_PATH";
const int UNDEFINED_UNSET = 0;

const Path DEFAULT_URI_PATH = static_cast<Path>("");
const std::string DEFAULT_PROTOCOL_NAME;
// 7 = LLL_ERR | LLL_WARN | LLL_NOTICE - default value for the libwebsockets 4.3.2
const int DEFAULT_LWS_LOG_LEVEL = 7;

} // namespace ews::cli
