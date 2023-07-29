/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "LwsAdapter/LwsDataHolder.hpp"
#include "LwsAdapter/LwsProtocolsFactory.hpp"
#include "ServerContext.hpp"

namespace ews::srv
{

LwsDataHolder::LwsDataHolder(const ServerContext& context)
    : port(context.port)
    , protocolName(context.protocolName)
    , protocols(createLwsProtocols(context.serverVersion, protocolName))
    , ssl(context.ssl)
    , vhostName(context.vhostName)
    , serverString(context.serverString)
    , lwsLogLevel(context.lwsLogLevel)
    , keepAliveTimeout(context.keepAliveTimeout)
    , keepAliveProbesInterval(context.keepAliveProbesInterval)
    , keepAliveProbes(context.keepAliveProbes)
{}

} // namespace ews::srv
