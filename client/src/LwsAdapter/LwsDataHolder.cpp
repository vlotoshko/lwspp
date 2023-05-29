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
    : protocols(createLwsProtocols(context.clientVersion))
{}

} // namespace wspp::cli
