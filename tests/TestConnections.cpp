/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include <iostream>

#include "catch2/catch.hpp"
#include "fakeit.hpp"

#include "websocketpp/client/ClientFactory.hpp"
#include "websocketpp/client/ClientContextBuilder.hpp"
#include "websocketpp/server/IEventHandler.hpp"
#include "websocketpp/server/IServer.hpp"
#include "websocketpp/server/ServerFactory.hpp"
#include "websocketpp/server/ServerContextBuilder.hpp"

namespace wspp::tests
{


class EventHandler : public srv::IEventHandler
{
public:
    void onConnect(srv::SessionId) noexcept override
    {}
    void onDisconnect(srv::SessionId) noexcept override
    {}
    void onError(srv::SessionId, const std::string&) noexcept override
    {}
    void onWarning(srv::SessionId, const std::string&) noexcept override
    {}
    void setMessageSender(srv::IMessageSenderPtr) override
    {}
};

class EventHandlerReferenceWrapper : public srv::IEventHandler
{
public:
    explicit EventHandlerReferenceWrapper(srv::IEventHandler& e) : _eventHandler(e)
    {}
    void onConnect(srv::SessionId id) noexcept override
    {
        _eventHandler.onConnect(id);
    }
    void onDisconnect(srv::SessionId id) noexcept override
    {
        _eventHandler.onConnect(id);
    }
    void onMessageReceive(srv::SessionId id, const std::string& s) noexcept override
    {
        _eventHandler.onMessageReceive(id ,s);
    }
    void onError(srv::SessionId id, const std::string& s) noexcept override
    {
        _eventHandler.onError(id ,s);
    }
    void onWarning(srv::SessionId id , const std::string& s) noexcept override
    {
        _eventHandler.onWarning(id ,s);
    }
    void setMessageSender(srv::IMessageSenderPtr ms) override
    {
        _eventHandler.setMessageSender(ms);
    }

private:
    srv::IEventHandler& _eventHandler;
};

SCENARIO( "Clients conect to the server", "[connection]" )
{
    GIVEN( "Server and client" )
    {
        const srv::Port PORT = 9000;

        fakeit::Mock<srv::IEventHandler> eventHandlerMock;
        srv::IEventHandlerPtr eventHandler { new EventHandlerReferenceWrapper{eventHandlerMock.get()}};

        auto serverBuilder = srv::ServerContextBuilder{};
        serverBuilder
                .setVersion(srv::ServerVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(eventHandler)
                ;

//        auto clientBuilder = cli::ClientContextBuilder{PORT};
//        clientBuilder
//                .setBehavior(std::string{""})
//                ;

//        auto server = srv::createServer(*serverBuilder.build());
//        server->start();
//        auto client = cli::createClient(*clientBuilder.build());

//        WHEN( "Client sends message to server" )
//        {
//            THEN( "Server receives message from client" )
//            {

//            }
//        }

    } // GIVEN
} // SCENARIO

} // namespace wspp::tests
