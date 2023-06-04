/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "easywebsocket/client/ClientVersions.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::cli
{

auto createLwsProtocols(ClientVersion) -> LwsProtocols;

} // namespace ews::cli
