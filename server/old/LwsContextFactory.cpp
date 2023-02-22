/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */


//#include "LwsAdapter/LwsCallbackContext.hpp"
#include "LwsAdapter/LwsContextFactory.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"
//#include "CallbackContext.hpp"
#include "ServerContext.hpp"

namespace wspp::srv
{

auto createLwsContext(const ServerContext& serverContext, ILwsCallbackContext& callbackContext) -> LwsContext
{
    LwsContext lwsContext{serverContext};
    const auto& dataHolder = lwsContext.dataHolder;

    auto lwsContextInfo = lws_context_creation_info{};

    lwsContextInfo.user = &callbackContext;
    lwsContextInfo.port = dataHolder.port;
    lwsContextInfo.protocols = dataHolder.protocols.data();

    lwsContext.lowLeveContext = LowLevelContext{lws_create_context(&lwsContextInfo), LwsContextDeleter{}};
    return lwsContext;
}

} // namespace wspp::srv
