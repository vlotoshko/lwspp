/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "framework/catch.hpp"

//#include "websocketpp/server/ServerFactory.hpp"

namespace wspp::tests
{

SCENARIO( "Server construction", "[server_construction]" )
{
    WHEN( "Server is in the initial state" )
    {
//        auto server = srv::createServer(srv::SimpleContext{});
        THEN( "Server has not active sessions" )
        {
//            REQUIRE(server->activeSessions() == 0U);
        }
    }
} // SCENARIO

} // namespace wspp::tests
