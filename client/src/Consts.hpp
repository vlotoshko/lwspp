/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "websocketpp/client/ClientVersions.hpp"
#include "websocketpp/client/Types.hpp"

namespace wspp::cli
{

const ClientVersion UNDEFINED_CLIENT_VERSION = ClientVersion::Undefined;
const Address UNDEFINED_ADDRESS = "UNDEFINED_ADDRESS";
const Port UNDEFINED_PORT = static_cast<int>(0U - 1);
const Path UNDEFINED_PATH = "UNDEFINED_PATH";

} // namespace wspp::cli
