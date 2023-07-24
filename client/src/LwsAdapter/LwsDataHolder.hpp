/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "easywebsocket/client/Types.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace ews::cli
{

/**
 * @brief The LwsDataHolder class holds the set of data low level context points on
 */
struct LwsDataHolder
{
    explicit LwsDataHolder(const ClientContext&);

    Address address;
    Port port;
    Path path;
    std::string protocolName;
    LwsProtocols protocols;
    SslSettingsPtr ssl;

    int keepAliveTimeout = 0;
    int keepAliveProbesInterval = 0;
    int keepAliveProbes = 0;
};

} // namespace ews::cli
