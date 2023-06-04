/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include <libwebsockets.h>

#include "easywebsocket/server/IEventHandler.hpp"
#include "LwsAdapter/ILwsCallbackNotifier.hpp"
#include "LwsAdapter/ILwsSession.hpp"
#include "LwsAdapter/LwsCallbackContext.hpp"
#include "LwsAdapter/LwsContext.hpp"
#include "LwsAdapter/LwsContextDeleter.hpp"
#include "LwsAdapter/LwsDataHolder.hpp"
#include "LwsAdapter/LwsMessageSender.hpp"
#include "LwsAdapter/LwsSessions.hpp"
#include "ServerContext.hpp"

namespace ews::srv
{

namespace
{

class LwsCallbackNotifier : public ILwsCallbackNotifier
{
public:
    LwsCallbackNotifier(const LwsDataHolderPtr& d, const LowLevelContextPtr& c)
        : _dataHolder(d)
        , _lowLevelContext(c)
    {}

    void sendSessionMessages(const ILwsSessionPtr& session) override
    {
        lws_callback_on_writable(session->getLwsInstance());
    }

    void sendPendingMessages() override
    {
        if (auto context = _lowLevelContext.lock())
        {
            if (auto dataHolder = _dataHolder.lock())
            {
                lws_callback_on_writable_all_protocol(
                    context.get(), dataHolder->protocols.data());
            }
        }
    }

private:
    LwsDataHolderWeak _dataHolder;
    LowLevelContextWeak _lowLevelContext;
};

auto setupLowLeverContext(const ILwsCallbackContextPtr& callbackContext, const LwsDataHolderPtr& dataHolder)
    -> LowLevelContextPtr
{
    auto lwsContextInfo = lws_context_creation_info{};

    lwsContextInfo.user = callbackContext.get();
    lwsContextInfo.port = dataHolder->port;
    lwsContextInfo.protocols = dataHolder->protocols.data();

    auto lowLevelContext = LowLevelContextPtr{lws_create_context(&lwsContextInfo), LwsContextDeleter{}};
    if (lowLevelContext == nullptr)
    {
        throw std::runtime_error{"lws_context initialization failed"};
    }
    return lowLevelContext;
}

}

LwsContext::LwsContext(const ServerContext& context)
{
    auto sessions = std::make_shared<LwsSessions>();
    _callbackContext = std::make_shared<LwsCallbackContext>(context.eventHandler, sessions);
    _dataHolder = std::make_shared<LwsDataHolder>(context);
    _lowLevelContext = setupLowLeverContext(_callbackContext, _dataHolder);

    auto notifier = std::make_shared<LwsCallbackNotifier>(_dataHolder, _lowLevelContext);
    auto sender = std::make_shared<LwsMessageSender>(sessions, std::move(notifier));
    _callbackContext->getEventHandler()->setMessageSender(std::move(sender));
}

void LwsContext::startListening()
{
    {
        const std::lock_guard<std::mutex> guard(_mutex);
        if (!_isStarted)
        {
            _isStarted = true;
        }
        else
        {
            return;
        }
    }

    int n = 0;
    while (n >= 0 && !_stopListening)
    {
        n = lws_service(_lowLevelContext.get(), 0);
    }
    _stopListening = true;
}

void LwsContext::stopListening()
{
    _stopListening = true;
    _callbackContext->setStopping();
}

auto LwsContext::getPort() const -> Port
{
    return _dataHolder->port;
}

} // namespace ews::srv
