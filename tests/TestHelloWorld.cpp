/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <iostream>

#include "catch2/catch.hpp"
#include "fakeit.hpp"

#include "websocketpp/client/ClientFactory.hpp"
#include "websocketpp/client/ClientContextBuilder.hpp"

#include "websocketpp/server/IEventHandler.hpp"
#include "websocketpp/server/IMessageSender.hpp"
#include "websocketpp/server/IServer.hpp"
#include "websocketpp/server/ServerFactory.hpp"
#include "websocketpp/server/ServerContextBuilder.hpp"

namespace wspp::tests
{

class ServerEventHandler : public srv::IEventHandler
{
public:
    void onConnect(srv::SessionId) noexcept override
    {}    
    void onDisconnect(srv::SessionId) noexcept override
    {}

    void onMessageReceive(srv::SessionId sessionId, const std::string& message) noexcept override
    {
        _messageSender->sendMessage(sessionId, message);
    }

    void onError(srv::SessionId, const std::string&) noexcept override
    {}    
    void onWarning(srv::SessionId, const std::string&) noexcept override
    {}

    void setMessageSender(srv::IMessageSenderPtr ms) override
    {
        _messageSender = ms;
    }

private:
    srv::IMessageSenderPtr _messageSender;
};

SCENARIO( "Clients sends 'hello world' to the server", "[hello_world]" )
{
    GIVEN( "Server and client" )
    {
        const srv::Port PORT = 9000;

        auto serverEventHandler = std::make_shared<ServerEventHandler>();
        auto serverBuilder = srv::ServerContextBuilder{};
        serverBuilder
                .setVersion(srv::ServerVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(serverEventHandler)
                ;

        auto server = srv::createServer(*serverBuilder.build());
        server->start();

//        WHEN( "Client sends message to server" )
//        {
//            THEN( "Server receives message from client" )
//            {

//            }
//        }



    } // GIVEN
} // SCENARIO

} // namespace wspp::tests
