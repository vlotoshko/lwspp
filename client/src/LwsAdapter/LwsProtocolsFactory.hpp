/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "easywebsocket/client/CallbackVersions.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::cli
{

auto createLwsProtocols(CallbackVersion) -> LwsProtocols;

} // namespace ews::cli
