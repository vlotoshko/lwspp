/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include <libwebsockets.h>

#include "websocketpp/server/IEventHandler.hpp"
#include "LwsAdapter/ILwsCallbackNotifier.hpp"
#include "LwsAdapter/ILwsSession.hpp"
#include "LwsAdapter/LwsCallbackContext.hpp"
#include "LwsAdapter/LwsContext.hpp"
#include "LwsAdapter/LwsContextDeleter.hpp"
#include "LwsAdapter/LwsDataHolder.hpp"
#include "LwsAdapter/LwsMessageSender.hpp"
#include "LwsAdapter/LwsSessions.hpp"
#include "ServerContext.hpp"

namespace wspp::srv
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

LwsContext::LwsContext(const ServerContext& context)
    : _sessions(std::make_shared<LwsSessions>())
    , _callbackContext(std::make_shared<LwsCallbackContext>(context.eventHandler, _sessions))
    , _dataHolder(std::make_shared<LwsDataHolder>(context))
{
    auto lwsContextInfo = lws_context_creation_info{};

    lwsContextInfo.user = _callbackContext.get();
    lwsContextInfo.port = _dataHolder->port;
    lwsContextInfo.protocols = _dataHolder->protocols.data();

    _lowLevelContext = LowLevelContextPtr{lws_create_context(&lwsContextInfo), LwsContextDeleter{}};
    if (_lowLevelContext == nullptr)
    {
        throw std::runtime_error{"lws_context initialization failed"};
    }

    auto notifier = std::make_shared<LwsCallbackNotifier>(_dataHolder, _lowLevelContext);
    auto sender = std::make_shared<LwsMessageSender>(_sessions, std::move(notifier));
    _callbackContext->getEventHandler()->setMessageSender(std::move(sender));
}

void LwsContext::startListening()
{
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

} // namespace wspp::srv
