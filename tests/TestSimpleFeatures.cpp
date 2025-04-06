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
#include <condition_variable>
#include <thread>

#include "MockedPtr.hpp"

#include "lwspp/client/ClientBuilder.hpp"
#include "lwspp/client/IClientControlAcceptor.hpp"
#include "lwspp/client/IClientLogic.hpp"

#include "lwspp/server/IConnectionInfo.hpp"
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

const std::string CUSTOM_PROTOCOL_NAME = "CUSTOM_PROTOCOL_NAME";
const std::string CUSTOM_PROTOCOL_NAME_2 = "CUSTOM_PROTOCOL_NAME_2";

const std::string SPECIFIC_PATH = "/SPECIFIC_PATH";
const std::string SPECIFIC_PATH_2 = "/SPECIFIC_PATH_2";

void waitForInitialization()
{
    const auto timeout = 100U;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
}

void setupServerBehavior(Mock<srv::IServerLogic>& serverLogic,
                         Mock<srv::IServerControlAcceptor>& serverControlAcceptor,
                         srv::IServerControlPtr& serverControl)
{
    Fake(Method(serverLogic, onConnect), Method(serverLogic, onDisconnect));

    When(Method(serverControlAcceptor, acceptServerControl))
        .Do([&serverControl](srv::IServerControlPtr ms){ serverControl = ms; });
}

void setupClientBehavior(Mock<cli::IClientLogic>& clientLogic,
                         Mock<cli::IClientControlAcceptor>& clientControlAcceptor,
                         cli::IClientControlPtr& clientControl)
{
    Fake(Method(clientLogic, onConnect), Method(clientLogic, onError),
         Method(clientLogic, onDisconnect));

    When(Method(clientControlAcceptor, acceptClientControl))
        .Do([&clientControl](cli::IClientControlPtr a){ clientControl = a; });
}

} // namespace


SCENARIO( "IP address feature testing", "[ip_address]" )
{
    auto srvLogic = MockedPtr<srv::IServerLogic>{};
    auto cliLogic = MockedPtr<cli::IClientLogic>{};

    auto srvControlAcceptor = MockedPtr<srv::IServerControlAcceptor>{};
    auto cliControlAcceptor = MockedPtr<cli::IClientControlAcceptor>{};

    srv::IServerControlPtr srvControl;
    cli::IClientControlPtr cliControl;

    setupServerBehavior(srvLogic.mock(), srvControlAcceptor.mock(), srvControl);
    setupClientBehavior(cliLogic.mock(), cliControlAcceptor.mock(), cliControl);

    srv::IP expectedClientIPv4 = "127.0.0.1";
    srv::IP expectedClientIPv6 = "::1";
    srv::IP actualClientIP;
    auto onConnect = [&](srv::IConnectionInfoPtr connectionInfo)
    {
        if (connectionInfo != nullptr)
        {
            actualClientIP = connectionInfo->getIP();
        }
    };

    When(Method(srvLogic.mock(), onConnect)).Do(onConnect);

    GIVEN( "Server and client" )
    {
        auto serverBuilder = srv::ServerBuilder{};
        serverBuilder
            .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
            .setPort(PORT)
            .setServerLogic(srvLogic.ptr())
            .setServerControlAcceptor(srvControlAcceptor.ptr());

        auto clientBuilder = cli::ClientBuilder{};
        clientBuilder
            .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
            .setAddress(ADDRESS)
            .setPort(PORT)
            .setClientLogic(cliLogic.ptr())
            .setClientControlAcceptor(cliControlAcceptor.ptr());



        WHEN( "Server uses default protocol name" )
        {
            auto server = serverBuilder.build();
            auto client = clientBuilder.build();

            THEN( "Client cann't connect to the server" )
            {
                waitForInitialization();
                server.reset();
                client.reset();
                CHECK((actualClientIP == expectedClientIPv4 || actualClientIP == expectedClientIPv6));
            }
        }
    }
}

SCENARIO( "Protocol name feature testing", "[protocol_name]" )
{
    auto srvLogic = MockedPtr<srv::IServerLogic>{};
    auto cliLogic = MockedPtr<cli::IClientLogic>{};
    
    auto srvControlAcceptor = MockedPtr<srv::IServerControlAcceptor>{};
    auto cliControlAcceptor = MockedPtr<cli::IClientControlAcceptor>{};

    srv::IServerControlPtr srvControl;
    cli::IClientControlPtr cliControl;

    setupServerBehavior(srvLogic.mock(), srvControlAcceptor.mock(), srvControl);
    setupClientBehavior(cliLogic.mock(), cliControlAcceptor.mock(), cliControl);

    GIVEN( "Server and client" )
    {
        auto serverBuilder = srv::ServerBuilder{};
        serverBuilder
            .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
            .setPort(PORT)
            .setServerLogic(srvLogic.ptr())
            .setServerControlAcceptor(srvControlAcceptor.ptr())
            .setLwsLogLevel(DISABLE_LOG);

        auto clientBuilder = cli::ClientBuilder{};
        clientBuilder
            .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
            .setAddress(ADDRESS)
            .setPort(PORT)
            .setClientLogic(cliLogic.ptr())
            .setClientControlAcceptor(cliControlAcceptor.ptr())
            .setLwsLogLevel(DISABLE_LOG);

        WHEN( "Server uses default protocol name" )
        {
            auto server = serverBuilder.build();

            AND_WHEN("Client uses default protocol name")
            {
                auto client = clientBuilder.build();

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(srvLogic.mock(), onConnect),
                           Method(srvLogic.mock(), onDisconnect)).Once();
                    Verify(Method(cliLogic.mock(), onConnect),
                           Method(cliLogic.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvLogic.mock());
                    VerifyNoOtherInvocations(cliLogic.mock());
                }
            }

            AND_WHEN("Client uses custom protocol name")
            {
                auto client = clientBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME).build();

                THEN( "Client cann't connect to the server" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(cliLogic.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvLogic.mock());
                    VerifyNoOtherInvocations(cliLogic.mock());
                }
            }
        }

        WHEN( "Server uses custom protocol name" )
        {
            auto server = serverBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME).build();

            AND_WHEN("Client uses default protocol name")
            {
                auto client = clientBuilder.build();
                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(srvLogic.mock(), onConnect),
                           Method(srvLogic.mock(), onDisconnect)).Once();
                    Verify(Method(cliLogic.mock(), onConnect),
                           Method(cliLogic.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvLogic.mock());
                    VerifyNoOtherInvocations(cliLogic.mock());
                }
            }

            AND_WHEN("Client uses a custom protocol name that differs from the servers protocol name")
            {
                auto client = clientBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME_2).build();
                THEN( "Client cann't connect to the server" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(cliLogic.mock(), onError)).Once();
                    VerifyNoOtherInvocations(srvLogic.mock());
                    VerifyNoOtherInvocations(cliLogic.mock());
                }
            }

            AND_WHEN("Client uses a custom protocol name that equals the servers protocol name")
            {
                clientBuilder.setProtocolName(CUSTOM_PROTOCOL_NAME);
                auto client = clientBuilder.build();

                THEN( "Client connects to the server successfully" )
                {
                    waitForInitialization();
                    server.reset();
                    client.reset();

                    Verify(Method(srvLogic.mock(), onConnect),
                           Method(srvLogic.mock(), onDisconnect)).Once();
                    Verify(Method(cliLogic.mock(), onConnect),
                           Method(cliLogic.mock(), onDisconnect)).Once();
                    VerifyNoOtherInvocations(srvLogic.mock());
                    VerifyNoOtherInvocations(cliLogic.mock());
                }
            }
        }
    } // GIVEN
} // SCENARIO

SCENARIO( "Path feature testing", "[path]" )
{
    auto srvLogic = MockedPtr<srv::IServerLogic>{};
    auto cliLogic = MockedPtr<cli::IClientLogic>{};
    
    auto srvControlAcceptor = MockedPtr<srv::IServerControlAcceptor>{};
    auto cliControlAcceptor = MockedPtr<cli::IClientControlAcceptor>{};

    srv::IServerControlPtr srvControl;
    cli::IClientControlPtr cliControl;

    setupServerBehavior(srvLogic.mock(), srvControlAcceptor.mock(), srvControl);
    setupClientBehavior(cliLogic.mock(), cliControlAcceptor.mock(), cliControl);

    GIVEN( "Server and client" )
    {
        auto serverBuilder = srv::ServerBuilder{};
        serverBuilder
            .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
            .setPort(PORT)
            .setServerLogic(srvLogic.ptr())
            .setServerControlAcceptor(srvControlAcceptor.ptr());

        auto clientBuilder = cli::ClientBuilder{};
        clientBuilder
            .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
            .setAddress(ADDRESS)
            .setPort(PORT)
            .setClientLogic(cliLogic.ptr())
            .setClientControlAcceptor(cliControlAcceptor.ptr());

        bool actualUseSpecificBehaviour = false;
        auto onConnect = [&](srv::IConnectionInfoPtr connectionInfo)
        {
            if (connectionInfo != nullptr && connectionInfo->getPath() == SPECIFIC_PATH)
            {
                actualUseSpecificBehaviour = true;
            }
        };

        When(Method(srvLogic.mock(), onConnect)).Do(onConnect);

        WHEN( "Client uses default uri path" )
        {
            auto server = serverBuilder.build();
            auto client = clientBuilder.build();

            THEN("Server uses default behavoiur")
            {
                waitForInitialization();
                server.reset();
                client.reset();

                Verify(Method(srvLogic.mock(), onConnect),
                       Method(srvLogic.mock(), onDisconnect)).Once();
                Verify(Method(cliLogic.mock(), onConnect),
                       Method(cliLogic.mock(), onDisconnect)).Once();
                VerifyNoOtherInvocations(srvLogic.mock());
                VerifyNoOtherInvocations(cliLogic.mock());

                const bool expectedUseSpecificBehaviour = false;
                REQUIRE(actualUseSpecificBehaviour == expectedUseSpecificBehaviour);
            }
        }

        WHEN( "Client uses specific uri path" )
        {
            auto server = serverBuilder.build();
            clientBuilder.setPath(SPECIFIC_PATH);
            auto client = clientBuilder.build();

            THEN("Server uses specific behavoiur")
            {
                waitForInitialization();
                server.reset();
                client.reset();

                Verify(Method(srvLogic.mock(), onConnect),
                       Method(srvLogic.mock(), onDisconnect)).Once();
                Verify(Method(cliLogic.mock(), onConnect),
                       Method(cliLogic.mock(), onDisconnect)).Once();
                VerifyNoOtherInvocations(srvLogic.mock());
                VerifyNoOtherInvocations(cliLogic.mock());

                const bool expectedUseSpecificBehaviour = true;
                REQUIRE(actualUseSpecificBehaviour == expectedUseSpecificBehaviour);
            }
        }

        WHEN( "Client uses specific uri path, but other than specified by server" )
        {
            auto server = serverBuilder.build();
            clientBuilder.setPath(SPECIFIC_PATH_2);
            auto client = clientBuilder.build();

            THEN("Server uses default behavoiur")
            {
                waitForInitialization();
                server.reset();
                client.reset();

                Verify(Method(srvLogic.mock(), onConnect),
                       Method(srvLogic.mock(), onDisconnect)).Once();
                Verify(Method(cliLogic.mock(), onConnect),
                       Method(cliLogic.mock(), onDisconnect)).Once();
                VerifyNoOtherInvocations(srvLogic.mock());
                VerifyNoOtherInvocations(cliLogic.mock());

                const bool expectedUseSpecificBehaviour = false;
                REQUIRE(actualUseSpecificBehaviour == expectedUseSpecificBehaviour);
            }
        }
    } // GIVEN
} // SCENARIO

// NOTE: This test case is disabled. It was executed manually by disabling the network connection
// on the VM with the server.
SCENARIO( "Keep alive feature testing", "[.keep_alive]" )
{
    auto cliLogic = MockedPtr<cli::IClientLogic>{};
    auto cliControlAcceptor = MockedPtr<cli::IClientControlAcceptor>{};
    cli::IClientControlPtr cliControl;

    std::mutex mutex;
    std::condition_variable condVar;
    bool clientStopped = false;

    auto stopClient = [&condVar, &clientStopped]()
    {
        clientStopped = true;
        condVar.notify_one();
    };

    Fake(Method(cliLogic.mock(), onConnect), Method(cliLogic.mock(), onError));
    When(Method(cliLogic.mock(), onDisconnect)).Do(stopClient);
    When(Method(cliControlAcceptor.mock(), acceptClientControl))
        .Do([&cliControl](cli::IClientControlPtr ms){ cliControl = ms; });

    GIVEN( "Server and client" )
    {
        auto clientBuilder = cli::ClientBuilder{};
        clientBuilder
            .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
            .setAddress("192.168.1.7")
            .setPort(PORT)
            .setClientLogic(cliLogic.ptr())
            .setClientControlAcceptor(cliControlAcceptor.ptr());

        WHEN( "Client sets up keep alive feature" )
        {
            clientBuilder.setKeepAliveTimeout(10);
            clientBuilder.setKeepAliveProbes(3);
            clientBuilder.setKeepAliveProbesInterval(5);

            auto client = clientBuilder.build();

            std::unique_lock<std::mutex> guard(mutex);
            condVar.wait(guard, [&clientStopped]{ return clientStopped; });

            Verify(Method(cliLogic.mock(), onConnect),
                   Method(cliLogic.mock(), onDisconnect)).Once();
            VerifyNoOtherInvocations(cliLogic.mock());
        }
    } // GIVEN
}

} // namespace tests
} // namespace lwspp
// NOLINTEND (readability-function-cognitive-complexity)
