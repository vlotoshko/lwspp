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

namespace ews::cli
{

LwsContext::LwsContext(const ClientContext& context)
    : _lwsConnectionInfo()
{
    _callbackContext = std::make_shared<LwsCallbackContext>(context.eventHandler);
    _dataHolder = std::make_shared<LwsDataHolder>(context);

    setupLowLeverContext();
    setupConnectionInfo();
}

LwsContext::~LwsContext()
{
    _isStopping = true;
    _callbackContext->setStopping();
}

void LwsContext::connect()
{
    {
        const std::lock_guard<std::mutex> guard(_mutex);
        if (!_isConnected)
        {
            _wsInstance = lws_client_connect_via_info(&_lwsConnectionInfo);
            _isConnected = true;
        }
        else
        {
            return;
        }
    }

    int n = 0;
    while (n >= 0 && !_isStopping)
    {
        n = lws_service(_lowLevelContext.get(), 0);
    }
    _isStopping = true;
}

void LwsContext::setupLowLeverContext()
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

void LwsContext::setupConnectionInfo()
{
    _lwsConnectionInfo.context = _lowLevelContext.get();
    _lwsConnectionInfo.address = _dataHolder->address.c_str();
    _lwsConnectionInfo.port = _dataHolder->port;
    _lwsConnectionInfo.path = _dataHolder->path.c_str();
    _lwsConnectionInfo.host = _lwsConnectionInfo.address;
    _lwsConnectionInfo.origin = _lwsConnectionInfo.address;
    //    _lwsClientConnectionInfo.ssl_connection = LCCSCF_USE_SSL;
    //    _lwsClientConnectionInfo.protocol = m_protocolName;
    _lwsConnectionInfo.pwsi = &_wsInstance;
}

} // namespace ews::cli
