/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "catch2/catch.hpp"

#include "easywebsocket/client/ClientBuilder.hpp"
#include "easywebsocket/client/EventHandlerBase.hpp"
#include "ClientContext.hpp"

namespace ews::cli
{

class TestClientBuilder
{
public:
    explicit TestClientBuilder(const ClientBuilder& clientBuiler)
        : _clientBuilder(clientBuiler)
    {}

    auto getClientContext() const -> const ClientContext&
    {
        return *_clientBuilder._clientContext;
    }

private:
    const ClientBuilder& _clientBuilder;
};

}
// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace ews::tests
{
using namespace cli;

const Port PORT = 9000;
const Address ADDRESS = "localhost";
const std::string PROTOCOL_NAME = "PROTOCOL_NAME";
const Path PATH = "PATH";

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

void compareClientContexts(const ClientContext& actual, const ClientContext& expected)
{
    // Mandatory parameters
    REQUIRE(toString(actual.clientVersion) == toString(expected.clientVersion));
    REQUIRE(actual.eventHandler == expected.eventHandler);
    REQUIRE(actual.address == expected.address);
    REQUIRE(actual.port == expected.port);

    // Non-mandatory parameters
    REQUIRE(actual.protocolName == expected.protocolName);
    REQUIRE(actual.path == expected.path);
}


SCENARIO( "ClientContext setup", "[client_builder]" )
{
    GIVEN( "ClientContextBuilder" )
    {
        auto clientBuilder = ClientBuilder{};

        WHEN( "All parameters are set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT)
                .setEventHandler(handler)
                .setMessageSenderAcceptor(handler)
                .setProtocolName(PROTOCOL_NAME)
                .setPath(PATH)
                ;

            const ClientContext& actual = TestClientBuilder{clientBuilder}.getClientContext();

            THEN( "Client context has correct data" )
            {
                ClientContext expected;
                expected.clientVersion = ClientVersion::v1_Amsterdam;
                expected.address = ADDRESS;
                expected.port = PORT;
                expected.eventHandler = handler;
                expected.protocolName = PROTOCOL_NAME;
                expected.path = PATH;

                compareClientContexts(actual, expected);
            }
        }
    } // GIVEN
} // SCENARIO

SCENARIO( "Client construction", "[client_builder]" )
{
    GIVEN( "ClientBuilder" )
    {
        auto clientBuilder = ClientBuilder{};

        WHEN( "Client version is not set" )
        {
            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: client version");
            }
        }

        WHEN( "Address is not set" )
        {
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setPort(PORT);

            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: address");
            }
        }

        WHEN( "Port is not set" )
        {
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setAddress(ADDRESS);

            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: port");
            }
        }

        WHEN( "Event handler is not set" )
        {
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT);

            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: event handler");
            }
        }

        WHEN( "MessageSenderAcceptor is not set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT)
                .setEventHandler(handler);

            THEN( "Exception is thrown on client build" )
            {
                REQUIRE_THROWS_WITH(clientBuilder.build(),
                                    "Required parameter is undefined: message sender acceptor");
            }
        }

        WHEN( "All mandatory parameters are set" )
        {
            auto handler = std::make_shared<EventHandlerBase>();
            clientBuilder
                .setVersion(ClientVersion::v1_Amsterdam)
                .setAddress(ADDRESS)
                .setPort(PORT)
                .setEventHandler(handler)
                .setMessageSenderAcceptor(handler);

            THEN( "Client builds successfully" )
            {
                auto expression = [&clientBuilder] {clientBuilder.build();};
                REQUIRE_NOTHROW(expression());
            }

            AND_WHEN( "All non-mandatory parameters are set" )
            {
                clientBuilder
                    .setProtocolName(PROTOCOL_NAME)
                    .setPath(PATH);

                THEN( "Client builds successfully" )
                {
                    auto expression = [&clientBuilder] {clientBuilder.build();};
                    REQUIRE_NOTHROW(expression());
                }
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
