/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "framework/catch.hpp"

#include "websocketpp/server/IEventHandler.hpp"
#include "websocketpp/server/ServerContextBuilder.hpp"
#include "IContextVisitor.hpp"
#include "ServerContext.hpp"

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace wspp::tests
{
using namespace srv;

const Port PORT = 9000;

auto toString(ServerVersion version) -> std::string
{
    switch (version)
    {
    case ServerVersion::v1_Andromeda:
        return "v1_Andromeda";
    default:
        return "Undefined";
    }
}

class EventHandler : public IEventHandler
{
public:
    void onConnect(SessionId) noexcept override
    {
        try
        {
//            std::cout << "event handler connection invoked " << std::endl;
        }
        catch (...)
        {

        }
    }
    void onDisconnect(SessionId) noexcept override
    {}
    void onError(srv::SessionId, const std::string&) noexcept override
    {}
    void onWarning(srv::SessionId, const std::string&) noexcept override
    {}
    void setMessageSender(srv::IMessageSenderPtr) override
    {}
};

class ServerContextVisitor : public IContextVisitor
{
public:
    void VisitServerContext(const ServerContext& context) override
    {
        // Mandatory parameters
        REQUIRE(toString(context.serverVersion) == toString(expected.serverVersion));
        REQUIRE(context.eventHandler == expected.eventHandler);
        REQUIRE(context.port == expected.port);

        // Non-mandatory parameters
        REQUIRE(context.protocolName == expected.protocolName);
    }

    ServerContext expected;
};

SCENARIO( "ServerContext construction", "[server_context_parameters]" )
{
    GIVEN( "ServerContextBuilder" )
    {
        auto serverContextBuilder = ServerContextBuilder{};

        WHEN( "Server version is not set" )
        {
            THEN( "Exception is thrown on context build" )
            {
                REQUIRE_THROWS_WITH(serverContextBuilder.build(),
                                    "Required parameter is undefined: server version");
            }
        }

        WHEN( "Port is not set" )
        {
            serverContextBuilder.setVersion(ServerVersion::v1_Andromeda);

            THEN( "Exception is thrown on context build" )
            {
                REQUIRE_THROWS_WITH(serverContextBuilder.build(),
                                    "Required parameter is undefined: port");
            }
        }

        WHEN( "Event handler is not set" )
        {
            serverContextBuilder
                    .setVersion(ServerVersion::v1_Andromeda)
                    .setPort(PORT);

            THEN( "Exception is thrown on context build" )
            {
                REQUIRE_THROWS_WITH(serverContextBuilder.build(),
                                    "Required parameter is undefined: event handler");
            }
        }

        WHEN( "All mandatory parameters are set" )
        {
            auto handler = std::make_shared<EventHandler>();
            serverContextBuilder
                    .setVersion(ServerVersion::v1_Andromeda)
                    .setPort(PORT)
                    .setEventHandler(handler);

            THEN( "Context builds successfully" )
            {
                IServerContextPtr serverContext;
                auto expression = [&serverContext, &serverContextBuilder] {serverContext = serverContextBuilder.build();};
                REQUIRE_NOTHROW(expression());

                AND_THEN( "Server context has correct data" )
                {
                    auto checker = std::make_shared<ServerContextVisitor>();
                    checker->expected.serverVersion = ServerVersion::v1_Andromeda;
                    checker->expected.eventHandler = handler;
                    checker->expected.port = PORT;

                    serverContext->accept(*checker);
                }
            }

            AND_WHEN( "All non-mandatory parameters are set" )
            {
                auto serverContext = serverContextBuilder
                        .setProtocolName("TestProtocolName")
                        .build();

                THEN( "Server context has correct data" )
                {
                    auto checker = std::make_shared<ServerContextVisitor>();
                    checker->expected.serverVersion = ServerVersion::v1_Andromeda;
                    checker->expected.eventHandler = handler;
                    checker->expected.port = PORT;
                    checker->expected.protocolName = "TestProtocolName";

                    serverContext->accept(*checker);
                }
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace wspp::tests
// NOLINTEND (readability-function-cognitive-complexity)
