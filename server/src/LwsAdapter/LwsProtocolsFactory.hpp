/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "easywebsocket/server/ServerVersions.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::srv
{

auto createLwsProtocols(ServerVersion) -> LwsProtocols;

} // namespace ews::srv
