/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <libwebsockets.h>
#include <stdexcept>

#include "ClientContext.hpp"
#include "LwsAdapter/LwsCallbackContext.hpp"
#include "LwsAdapter/LwsContext.hpp"
#include "LwsAdapter/LwsContextDeleter.hpp"
#include "LwsAdapter/LwsDataHolder.hpp"

namespace wspp::cli
{

LwsContext::LwsContext(const ClientContext& context)
    : _callbackContext(std::make_shared<LwsCallbackContext>(context.eventHandler))
    , _dataHolder(std::make_shared<LwsDataHolder>(context))
{
    auto lwsContextInfo = lws_context_creation_info{};
    lwsContextInfo.user = _callbackContext.get();
    lwsContextInfo.port = CONTEXT_PORT_NO_LISTEN;
    lwsContextInfo.protocols = _dataHolder->protocols.data();

    _lowLevelContext = LowLevelContextPtr{lws_create_context(&lwsContextInfo), LwsContextDeleter{}};
    if (_lowLevelContext == nullptr)
    {
        throw std::runtime_error{"lws_context initialization failed"};
    }
}


} // namespace wspp::cli
