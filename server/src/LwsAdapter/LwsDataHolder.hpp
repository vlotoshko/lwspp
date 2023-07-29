/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/server/Types.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace ews::srv
{

/**
 * @brief The LwsDataHolder class holds the set of data low level context points on
 */
struct LwsDataHolder
{
    explicit LwsDataHolder(const ServerContext&);

    Port port;
    std::string protocolName;
    LwsProtocols protocols;
    SslSettingsPtr ssl;
    std::string vhostName;
    std::string serverString;
    int lwsLogLevel = 0;

    int keepAliveTimeout = 0;
    int keepAliveProbesInterval = 0;
    int keepAliveProbes = 0;
};

} // namespace ews::srv
