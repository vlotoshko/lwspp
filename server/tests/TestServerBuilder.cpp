/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "catch2/catch.hpp"

#include "easywebsocket/server/EventHandlerBase.hpp"
#include "easywebsocket/server/ServerBuilder.hpp"
#include "ServerContext.hpp"

namespace ews::srv
{

class TestServerBuilder
{
public:
    explicit TestServerBuilder(const ServerBuilder& serverBuiler)
        : _serverBuilder(serverBuiler)
    {}

    auto getServerContext() const -> const ServerContext&
    {
        return *_serverBuilder._serverContext;
    }

private:
    const ServerBuilder& _serverBuilder;
};

} // namespace ews::srv

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace ews::tests
{
using namespace srv;

const Port PORT = 9000;
const std::string PROTOCOL_NAME = "PROTOCOL_NAME";

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

void compareServerContexts(const ServerContext& actual, const ServerContext& expected)
{
    // Mandatory parameters
    REQUIRE(toString(actual.serverVersion) == toString(expected.serverVersion));
    REQUIRE(actual.eventHandler == expected.eventHandler);
    REQUIRE(actual.port == expected.port);

    // Non-mandatory parameters
    REQUIRE(actual.protocolName == expected.protocolName);
}


SCENARIO( "ServerContext setup", "[server_builder]" )
{
    GIVEN( "ServerBuilder" )
    {
        auto serverBuilder = ServerBuilder{};

        WHEN( "All parameters are set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            serverBuilder
                .setVersion(ServerVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(handler)
                .setMessageSenderAcceptor(handler)
                .setProtocolName(PROTOCOL_NAME)

                ;

            const ServerContext& actual = TestServerBuilder{serverBuilder}.getServerContext();

            THEN( "Server context has correct data" )
            {
                ServerContext expected;
                expected.serverVersion = ServerVersion::v1_Andromeda;
                expected.eventHandler = handler;
                expected.port = PORT;
                expected.protocolName = PROTOCOL_NAME;

                compareServerContexts(actual, expected);
            }
        }
    } // GIVEN
} // SCENARIO

SCENARIO( "Server construction", "[server_builder]" )
{
    GIVEN( "ServerBuilder" )
    {
        auto serverBuilder = ServerBuilder{};

        WHEN( "Server version is not set" )
        {
            THEN( "Exception is thrown on server build" )
            {
                REQUIRE_THROWS_WITH(serverBuilder.build(),
                                    "Required parameter is undefined: server version");
            }
        }

        WHEN( "Port is not set" )
        {
            serverBuilder.setVersion(ServerVersion::v1_Andromeda);

            THEN( "Exception is thrown on server build" )
            {
                REQUIRE_THROWS_WITH(serverBuilder.build(),
                                    "Required parameter is undefined: port");
            }
        }

        WHEN( "Event handler is not set" )
        {
            serverBuilder
                .setVersion(ServerVersion::v1_Andromeda)
                .setPort(PORT);

            THEN( "Exception is thrown on server build" )
            {
                REQUIRE_THROWS_WITH(serverBuilder.build(),
                                    "Required parameter is undefined: event handler");
            }
        }

        WHEN( "MessageSenderAcceptor is not set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            serverBuilder
                .setVersion(ServerVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(handler);

            THEN( "Exception is thrown on server build" )
            {
                REQUIRE_THROWS_WITH(serverBuilder.build(),
                                    "Required parameter is undefined: message sender acceptor");
            }
        }

        WHEN( "All mandatory parameters are set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            serverBuilder
                .setVersion(ServerVersion::v1_Andromeda)
                .setPort(PORT)
                .setEventHandler(handler)
                .setMessageSenderAcceptor(handler);

            auto expression = [&serverBuilder] {serverBuilder.build();};

            THEN( "Server builds successfully" )
            {
                REQUIRE_NOTHROW(expression());
            }

            AND_WHEN( "All non-mandatory parameters are set" )
            {
                serverBuilder.setProtocolName(PROTOCOL_NAME);

                THEN( "Server builds successfully" )
                {
                    REQUIRE_NOTHROW(expression());
                }
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
