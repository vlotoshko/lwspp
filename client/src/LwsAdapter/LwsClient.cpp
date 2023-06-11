/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <libwebsockets.h>
#include <stdexcept>

#include "ClientContext.hpp"
#include "LwsAdapter/LwsCallbackContext.hpp"
#include "LwsAdapter/LwsClient.hpp"
#include "LwsAdapter/LwsContextDeleter.hpp"
#include "LwsAdapter/LwsDataHolder.hpp"

namespace ews::cli
{

LwsClient::LwsClient(const ClientContext& context)
    : _lwsConnectionInfo()
{
    _callbackContext = std::make_shared<LwsCallbackContext>(context.eventHandler);
    _dataHolder = std::make_shared<LwsDataHolder>(context);

    setupLowLevelContext_();
    setupConnectionInfo_();
}

LwsClient::~LwsClient()
{
    disconnect();
    waitForClientStopping_();
}

void LwsClient::connect()
{
    {
        const std::lock_guard<std::mutex> guard(_mutex);
        if (_state == State::Initial)
        {
            _wsInstance = lws_client_connect_via_info(&_lwsConnectionInfo);
           _state = State::Started;
        }
        else
        {
            return;
        }
    }

    int res = 0;
    while (res >= 0 && _state != State::Stopping)
    {
        res = lws_service(_lowLevelContext.get(), 0);
    }

    if (res < 0)
    {
        throw std::runtime_error{
            std::string{"lws_service stopped with the error code: "}.append(std::to_string(res))};
    }

    _state = State::Stopped;
    _isStoppedCV.notify_one();
}

void LwsClient::disconnect()
{
    const std::lock_guard<std::mutex> guard(_mutex);
    if (_state == State::Initial)
    {
        _state = State::Stopped;
    }
    else if (_state == State::Started)
    {
        _state = State::Stopping;
        _callbackContext->setStopping();
    }
}

void LwsClient::setupLowLevelContext_()
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

void LwsClient::setupConnectionInfo_()
{
    _lwsConnectionInfo.context = _lowLevelContext.get();
    _lwsConnectionInfo.pwsi = &_wsInstance;

    _lwsConnectionInfo.address = _dataHolder->address.c_str();
    _lwsConnectionInfo.port = _dataHolder->port;
    _lwsConnectionInfo.path = _dataHolder->path.c_str();
    _lwsConnectionInfo.host = _lwsConnectionInfo.address;
    _lwsConnectionInfo.origin = _lwsConnectionInfo.address;
    //    _lwsClientConnectionInfo.ssl_connection = LCCSCF_USE_SSL;
    if (_dataHolder->protocolName != DEFAULT_PROTOCOL_NAME)
    {
        _lwsConnectionInfo.protocol = _dataHolder->protocolName.c_str();
    }
}

void LwsClient::waitForClientStopping_()
{
    std::unique_lock<std::mutex> guard(_mutex);
    _isStoppedCV.wait(guard, [this]{ return _state == State::Stopped || _state == State::Initial; });
}

} // namespace ews::cli
