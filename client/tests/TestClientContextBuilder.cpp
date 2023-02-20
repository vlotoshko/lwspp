/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "framework/catch.hpp"

//#include "websocketpp/client/ClientFactory.hpp"

namespace wspp::tests
{

SCENARIO( "Client construction", "[client_construction]" )
{
    WHEN( "Client is in the initial state" )
    {
//        auto client = cli::createClient(cli::SimpleContext{});
        THEN( "Client is not connected" )
        {
//            REQUIRE_FALSE(client->isConnected());
        }
    }
} // SCENARIO

} // namespace wspp::tests
