/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "easywebsocket/server/CallbackVersions.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::srv
{

auto createLwsProtocols(CallbackVersion, const std::string& protocolName) -> LwsProtocols;

} // namespace ews::srv
