/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "framework/catch.hpp"

#include "websocketpp/client/ClientFactory.hpp"
#include "websocketpp/client/ClientContextBuilder.hpp"
#include "websocketpp/client/types/IClient.hpp"
#include "websocketpp/server/types/IServer.hpp"
#include "websocketpp/server/ServerFactory.hpp"
#include "websocketpp/server/ServerContextBuilder.hpp"

namespace wspp::tests
{

SCENARIO( "Simple test cases", "[]" )
{
    GIVEN( "Server and client" )
    {
        auto serverBuilder = srv::ClientContextBuilder{9000};
        serverBuilder
                .setBehavior(std::string{""})
                .setLogging(5)
                ;

        auto clientBuilder = cli::ClientContextBuilder{9000};
        clientBuilder
                .setBehavior(std::string{""})
                .setLogging(5)
                ;

        auto server = srv::createServer(*serverBuilder.build());
        auto client = cli::createClient(*clientBuilder.build());

        WHEN( "Server is in the initial state" )
        {
            THEN( "Server has not active sessions" )
            {
                REQUIRE(server->getPort() == 9000);
            }
        }

        WHEN( "Client is in the initial state" )
        {
            THEN( "Client is not connected" )
            {
                REQUIRE_FALSE(client->isConnected());
            }
        }

        WHEN( "Client connects to the server" )
        {
            client->connect();
            THEN( "Connection is established" )
            {
                REQUIRE(server->getPort() == 9000);
                REQUIRE(client->isConnected());
            }
        }

//        WHEN( "Client sends message to server" )
//        {
//            THEN( "Server receives message from client" )
//            {

//            }
//        }

    } // GIVEN
} // SCENARIO

} // namespace wspp::tests
