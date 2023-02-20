/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "LwsContext.hpp"

namespace wspp::srv
{

LwsContext::LwsContext(const ServerContext& context, ICallbackContext& callbackContext)
    : dataHolder(context)
{
    auto lwsContextInfo = lws_context_creation_info{};

    lwsContextInfo.user = &callbackContext;
    lwsContextInfo.port = dataHolder.port;
    lwsContextInfo.protocols = dataHolder.protocols.data();
    lowLeveContext = LowLevelContext{lws_create_context(&lwsContextInfo), LwsContextDeleter{}};
}

} // namespace wspp::srv
