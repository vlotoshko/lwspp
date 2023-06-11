/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include <future>
#include <thread>

#include "catch2/catch.hpp"

#include "easywebsocket/server/IEventHandler.hpp"
#include "easywebsocket/server/IServer.hpp"
#include "easywebsocket/server/ServerContextBuilder.hpp"
#include "easywebsocket/server/ServerFactory.hpp"

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace ews::tests
{
using namespace srv;
namespace
{

const Port PORT = 9000;

class EventHandler : public IEventHandler
{
public:
    void onConnect(SessionId) noexcept override
    {}
    void onDisconnect(SessionId) noexcept override
    {}
    void onMessageReceive(srv::SessionId, const std::string&) noexcept override
    {}
    void onError(srv::SessionId, const std::string&) noexcept override
    {}
    void onWarning(srv::SessionId, const std::string&) noexcept override
    {}
    void setMessageSender(srv::IMessageSenderPtr) override
    {}
};

srv::IServerPtr setupServer()
{
    auto serverContextBuilder = srv::ServerContextBuilder{};
    serverContextBuilder
        .setVersion(srv::ServerVersion::v1_Andromeda)
        .setPort(PORT)
        .setEventHandler(std::make_shared<EventHandler>())
        ;

    return srv::createServer(*serverContextBuilder.build());
}

} // namespace

void waitForStarting()
{
    const auto timeout = 10U;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
}

SCENARIO( "Starting-stopping server", "[server_start_stop]" )
{
    GIVEN( "Server" )
    {
        // NOTE: server stops on destruction
        auto server = setupServer();

        WHEN( "Server was not started and it stops" )
        {
            server.reset();
            THEN( "It stops properly" )
            {
            }
        }

        WHEN( "Server was started and it stops" )
        {
            auto asyncServerStart = std::async(std::launch::async, [&]{server->start();});
            waitForStarting();

            server.reset();
            asyncServerStart.wait();
            THEN( "It stops properly" )
            {
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
