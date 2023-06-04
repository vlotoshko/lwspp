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
    , protocols(createLwsProtocols(context.serverVersion))
{}

} // namespace ews::srv
