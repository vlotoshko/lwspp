/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "websocketpp/server/IEventHandler.hpp"
#include "websocketpp/server/IMessageSender.hpp"
#include "CallbackContext.hpp"
#include "ISession.hpp"
#include "Server.hpp"
#include "Sessions.hpp"

namespace wspp::srv
{
namespace
{

std::map<int, Server*> registeredServers;
std::mutex registeredServersMutex;

void signalHandler(int signum)
{
    const std::lock_guard<std::mutex> lock{registeredServersMutex};
    for (auto server : registeredServers)
    {
        server.second->stop();
    }
    exit(signum);
}

void registerSingnalHandling(int port, Server* server)
{
    const std::lock_guard<std::mutex> lock{registeredServersMutex};
    signal(SIGINT, signalHandler);

    auto it = registeredServers.find(port);
    if (it == registeredServers.end())
    {
        registeredServers[port] = server;
    }
    else
    {
        throw std::runtime_error{"Server with such port already exists: " + std::to_string(port)};
    }
}

void unregisterSingnalHandling(int port)
{
    const std::lock_guard<std::mutex> lock{registeredServersMutex};
    registeredServers.erase(port);
}

} // namespace

class MessageSender : public IMessageSender
{
public:
    explicit MessageSender(LwsContext& l, ISessionsPtr s)
        : _lwsContext(l)
        , _sessions(std::move(s))
    {}

    void sendMessage(SessionId sessionId, const std::string& message) override
    {
        if (auto session = _sessions->get(sessionId))
        {
            session->addMessage(message);
        }
    }
    void sendMessage(const std::string& message) override
    {
        for(auto& entry : _sessions->getAllSessions())
        {
            entry->addMessage(message);
        }

        lws_callback_on_writable_all_protocol(_lwsContext.lowLeveContext.get(),
                                              _lwsContext.dataHolder.protocols.data());
    }
private:
    LwsContext& _lwsContext; // TODO replace with some notifier
    ISessionsPtr _sessions;
};

Server::Server(const ServerContext& context)
    : _sessions(std::make_shared<Sessions>())
    , _callbackContext(std::make_shared<CallbackContext>(context.eventHandler, _sessions))
    , _lwsContext(context, *_callbackContext)
{
    if (_lwsContext.lowLeveContext == nullptr)
    {
        throw std::runtime_error{"lws_context initialization failed"};
    }

    auto sender = std::make_shared<MessageSender>(_lwsContext, _sessions);
    _callbackContext->getEventHandler()->setMessageSender(std::move(sender));
    registerSingnalHandling(_lwsContext.dataHolder.port, this);
}

void Server::start()
{
    int n = 0;
    while (n >= 0 && !_callbackContext->isServerStopped())
    {
        n = lws_service(_lwsContext.lowLeveContext.get(), 0);
    }
    _callbackContext->isServerStopped();    
}

void Server::stop()
{
    stop_();
}

void Server::stop_()
{
    if (!_callbackContext->isServerStopped())
    {
        _callbackContext->setServerStopped();
    }
}

Server::~Server()
{
    stop_();
    unregisterSingnalHandling(_lwsContext.dataHolder.port);
}

} // namespace wspp::srv
