/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "websocketpp/client/ClientVersions.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace wspp::cli
{

auto createLwsProtocols(ClientVersion) -> LwsProtocols;

} // namespace wspp::cli
