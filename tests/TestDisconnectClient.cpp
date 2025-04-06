/*
 * lwspp - C++ wrapper for the libwebsockets library
 *
 * Copyright (C) 2023 - 2023 Volodymyr Lotoshko <vlotoshko@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <catch2/catch_test_macros.hpp>
#include <future>

#include "MockedPtr.hpp"

#include "lwspp/client/ClientBuilder.hpp"
#include "lwspp/client/IClientControl.hpp" // IWYU pragma: keep
#include "lwspp/client/IClientControlAcceptor.hpp"
#include "lwspp/client/IClientLogic.hpp"

#include "lwspp/server/IConnectionInfo.hpp" // IWYU pragma: keep
#include "lwspp/server/IServerControl.hpp"  // IWYU pragma: keep
#include "lwspp/server/IServerControlAcceptor.hpp"
#include "lwspp/server/IServerLogic.hpp"
#include "lwspp/server/ServerBuilder.hpp"

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace lwspp
{
namespace tests
{

using namespace fakeit;

namespace
{

const srv::Port PORT = 9000;
const cli::Address ADDRESS = "localhost";
const int DISABLE_LOG = 0;
const std::chrono::seconds TIMEOUT{1};

void setupServerBehavior(Mock<srv::IServerLogic>& servreLogic,
                         std::promise<srv::ConnectionId>& promiseConnected,
                         std::promise<void>& promiseDisconnected)
{
    auto onConnect = [&](srv::IConnectionInfoPtr connectionInfo)
    {
        promiseConnected.set_value(connectionInfo->getConnectionId());
    };

    auto onDisconnect = [&](srv::ConnectionId)
    {
        promiseDisconnected.set_value();
    };

    When(Method(servreLogic, onConnect)).Do(onConnect);
    When(Method(servreLogic, onDisconnect)).Do(onDisconnect);
}

srv::IServerPtr setupServer(srv::IServerLogicPtr serverLogic,
                            srv::IServerControlAcceptorPtr serverControlAcceptor)
{
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
        .setPort(PORT)
        .setServerLogic(serverLogic)
        .setServerControlAcceptor(serverControlAcceptor)
        .setLwsLogLevel(DISABLE_LOG);

    return serverBuilder.build();
}

cli::IClientPtr setupClient(cli::IClientLogicPtr clientLogic,
                            cli::IClientControlAcceptorPtr clientControlAcceptor)
{
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
        .setAddress(ADDRESS)
        .setPort(PORT)
        .setClientLogic(clientLogic)
        .setClientControlAcceptor(clientControlAcceptor)
        .setLwsLogLevel(DISABLE_LOG);

    return clientBuilder.build();
}

} // namespace

//clazy:excludeall=non-pod-global-static

SCENARIO( "Server disconnects client", "[disconnect_client]" )
{
    std::promise<srv::ConnectionId> promiseConnected;
    auto waitForConnection = promiseConnected.get_future();

    std::promise<void> promiseDisconnected;
    auto waitForDisconnection = promiseDisconnected.get_future();

    auto srvLogic = MockedPtr<srv::IServerLogic>{};
    auto cliLogic = MockedPtr<cli::IClientLogic>{};
    
    auto srvControlAcceptor = MockedPtr<srv::IServerControlAcceptor>{};
    auto cliControlAcceptor = MockedPtr<cli::IClientControlAcceptor>{};

    srv::IServerControlPtr srvControl;

    setupServerBehavior(srvLogic.mock(), promiseConnected, promiseDisconnected);
    When(Method(srvControlAcceptor.mock(), acceptServerControl))
        .Do(
            [&srvControl](srv::IServerControlPtr a)
            {
                srvControl = a;
            });

    Fake(Method(cliLogic.mock(), onConnect), Method(cliLogic.mock(), onDisconnect),
         Method(cliLogic.mock(), onWarning),  Method(cliLogic.mock(), onError));

    Fake(Method(cliControlAcceptor.mock(), acceptClientControl));

    GIVEN( "Server and client are connected" )
    {
        auto server = setupServer(srvLogic.ptr(), srvControlAcceptor.ptr());
        auto client = setupClient(cliLogic.ptr(), cliControlAcceptor.ptr());

        REQUIRE(waitForConnection.wait_for(TIMEOUT) == std::future_status::ready);
        auto connectionId = waitForConnection.get();

        VerifyNoOtherInvocations(Method(srvLogic.mock(), onDisconnect));

        WHEN( "Server closes the connection with the client" )
        {
            srvControl->closeConnection(connectionId);

            THEN( "The connection is closed" )
            {
                REQUIRE(waitForDisconnection.wait_for(TIMEOUT*10) == std::future_status::ready);
                Verify(Method(srvLogic.mock(), onDisconnect)).Once();

                server.reset();
                client.reset();

                Verify(Method(srvLogic.mock(), onConnect)).Once();
                VerifyNoOtherInvocations(srvLogic.mock());

                Verify(Method(cliLogic.mock(), onConnect)).Once();
                Verify(Method(cliLogic.mock(), onDisconnect)).Once();
                VerifyNoOtherInvocations(cliLogic.mock());
            }
        }
    } // GIVEN
} // SCENARIO

} // namespace tests
} // namespace lwspp
// NOLINTEND (readability-function-cognitive-complexity)
