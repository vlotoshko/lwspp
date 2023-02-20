/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include <iostream>

#include "framework/catch.hpp"

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
    void onConnect(srv::SessionId id) noexcept override
    {
        try
        {
            std::cout << "Client connected, session id: " << id << std::endl;
        }
        catch (...)
        {

        }
    }

    void onDisconnect(srv::SessionId) noexcept override
    {}
    void onError(srv::SessionId, const std::string&) noexcept override
    {}
    void setMessageSender(srv::IMessageSenderPtr) override
    {}
};

SCENARIO( "Simple test cases", "[]" )
{
    GIVEN( "Server and client" )
    {
        const srv::Port PORT = 9000;

        auto eventHandler = std::make_shared<EventHandler>();

        auto serverBuilder = srv::ServerContextBuilder{};
        serverBuilder
                .setVersion(srv::ServerVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(eventHandler)
                .setLogging(1)
                ;

        auto clientBuilder = cli::ClientContextBuilder{PORT};
        clientBuilder
                .setBehavior(std::string{""})
                .setLogging(1)
                ;

        auto server = srv::createServer(*serverBuilder.build());
        server->start();
        auto client = cli::createClient(*clientBuilder.build());

//        WHEN( "Client sends message to server" )
//        {
//            THEN( "Server receives message from client" )
//            {

//            }
//        }

    } // GIVEN
} // SCENARIO

} // namespace wspp::tests
