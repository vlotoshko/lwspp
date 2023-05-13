/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "catch2/catch.hpp"

#include "websocketpp/client/IEventHandler.hpp"
#include "websocketpp/client/ClientContextBuilder.hpp"
#include "IContextVisitor.hpp"
#include "ClientContext.hpp"

namespace wspp::tests
{
using namespace cli;

const Port PORT = 9000;

auto toString(ClientVersion version) -> std::string
{
    switch (version)
    {
    case ClientVersion::v1_Amsterdam:
        return "v1_Andromeda";
    default:
        return "Undefined";
    }
}

class EventHandler : public IEventHandler
{
public:
//    void onConnect(SessionId) noexcept override
//    {}
//    void onDisconnect(SessionId) noexcept override
//    {}
//    void onMessageReceive(srv::SessionId, const std::string&) noexcept override
//    {}
//    void onError(srv::SessionId, const std::string&) noexcept override
//    {}
//    void onWarning(srv::SessionId, const std::string&) noexcept override
//    {}
    void setMessageSender(cli::IMessageSenderPtr) override
    {}
};

class ClientContextVisitor : public IContextVisitor
{
public:
    void VisitClientContext(const ClientContext& context) override
    {
        // Mandatory parameters
        REQUIRE(toString(context.clientVersion) == toString(expected.clientVersion));
        REQUIRE(context.eventHandler == expected.eventHandler);
        REQUIRE(context.address == expected.address);
        REQUIRE(context.port == expected.port);
        REQUIRE(context.path == expected.path);

        // Non-mandatory parameters
        //        REQUIRE(context.protocolName == expected.protocolName);
    }

    ClientContext expected;
};

SCENARIO( "ClientContext construction", "[client_context_parameters]" )
{
    GIVEN( "ClientContextBuilder" )
    {
        auto clientContextBuilder = ClientContextBuilder{};

        WHEN( "Client version is not set" )
        {
            THEN( "Exception is thrown on context build" )
            {
                REQUIRE_THROWS_WITH(clientContextBuilder.build(),
                                    "Required parameter is undefined: client version");
            }
        }

        WHEN( "Port is not set" )
        {
            clientContextBuilder.setVersion(ClientVersion::v1_Amsterdam);

            THEN( "Exception is thrown on context build" )
            {
                REQUIRE_THROWS_WITH(clientContextBuilder.build(),
                                    "Required parameter is undefined: port");
            }
        }

        WHEN( "Event handler is not set" )
        {
            clientContextBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setPort(PORT);

            THEN( "Exception is thrown on context build" )
            {
                REQUIRE_THROWS_WITH(clientContextBuilder.build(),
                                    "Required parameter is undefined: event handler");
            }
        }

        WHEN( "All mandatory parameters are set" )
        {
            auto handler = std::make_shared<EventHandler>();
            clientContextBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setPort(PORT)
                .setEventHandler(handler);

            THEN( "Context builds successfully" )
            {
                IClientContextPtr clientContext;
                auto expression = [&clientContext, &clientContextBuilder] {clientContext = clientContextBuilder.build();};
                REQUIRE_NOTHROW(expression());

                AND_THEN( "Client context has correct data" )
                {
                    auto checker = std::make_shared<ClientContextVisitor>();
                    checker->expected.clientVersion = ClientVersion::v1_Amsterdam;
                    checker->expected.eventHandler = handler;
                    checker->expected.port = PORT;

                    clientContext->accept(*checker);
                }
            }

            AND_WHEN( "All non-mandatory parameters are set" )
            {
                auto clientContext = clientContextBuilder
                                         //                        .setProtocolName("TestProtocolName")
                                         .build();

                THEN( "Client context has correct data" )
                {
                    auto checker = std::make_shared<ClientContextVisitor>();
                    checker->expected.clientVersion = ClientVersion::v1_Amsterdam;
                    checker->expected.eventHandler = handler;
                    checker->expected.port = PORT;
                    //                    checker->expected.protocolName = "TestProtocolName";

                    clientContext->accept(*checker);
                }
            }
        }
    } // GIVEN
} // SCENARIO
} // namespace wspp::tests
