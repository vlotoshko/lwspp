/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include "LwsAdapter/LwsDataHolder.hpp"
#include "LwsAdapter/LwsProtocolsFactory.hpp"
#include "ClientContext.hpp"

namespace ews::cli
{

LwsDataHolder::LwsDataHolder(const ClientContext& context)
    : address(context.address)
    , port(context.port)
    , path(context.path)
    , protocolName(context.protocolName)
    , protocols(createLwsProtocols(context.callbackVersion))
    , ssl(context.ssl)
    , lwsLogLevel(context.lwsLogLevel)
    , keepAliveTimeout(context.keepAliveTimeout)
    , keepAliveProbesInterval(context.keepAliveProbesInterval)
    , keepAliveProbes(context.keepAliveProbes)
{}

} // namespace ews::cli
