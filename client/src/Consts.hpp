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
const Path UNDEFINED_PATH = "UNDEFINED_PATH";
const unsigned int MAX_MESSAGE_SIZE = 1024 * 64;

} // namespace ews::cli
