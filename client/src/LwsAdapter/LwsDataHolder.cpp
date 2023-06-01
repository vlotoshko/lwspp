/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include "LwsAdapter/LwsDataHolder.hpp"
#include "LwsAdapter/LwsProtocolsFactory.hpp"
#include "ClientContext.hpp"

namespace wspp::cli
{

LwsDataHolder::LwsDataHolder(const ClientContext& context)
    : address(context.address)
    , port(context.port)
    , path(context.path)
    , protocols(createLwsProtocols(context.clientVersion))
{}

} // namespace wspp::cli
