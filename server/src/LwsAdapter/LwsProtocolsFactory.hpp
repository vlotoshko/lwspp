/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "websocketpp/server/ServerVersions.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace wspp::srv
{

auto createLwsProtocols(ServerVersion) -> LwsProtocols;

} // namespace wspp::srv
