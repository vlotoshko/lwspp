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

#include <iostream>
#include <thread>

#include "lwspp/client/ClientBuilder.hpp"
#include "lwspp/client/ClientLogicBase.hpp"
#include "lwspp/client/IClientControl.hpp" // IWYU pragma: keep

#include "lwspp/server/IServerControl.hpp" // IWYU pragma: keep
#include "lwspp/server/ServerBuilder.hpp"
#include "lwspp/server/ServerLogicBase.hpp"

using namespace lwspp;

class ClientLogic : public cli::ClientLogicBase
{
public:
    void onConnect(cli::IConnectionInfoPtr) noexcept override
    {
        _clientControl->sendTextData("hello server!");
    }

    void onTextDataReceive(const cli::DataPacket& dataPacket) noexcept override
    {
        std::cout << "client received the message: " << std::string{dataPacket.data, dataPacket.length} << std::endl;
    }
};

class ServerLogic : public srv::ServerLogicBase
{
public:
    void onTextDataReceive(srv::ConnectionId, const srv::DataPacket& dataPacket) noexcept override
    {
        std::cout << "server received the message: " << std::string{dataPacket.data, dataPacket.length}  << std::endl;
        _serverControl->sendTextData("hello client!");
    }
};

auto main() -> int
{
    const srv::Port PORT = 9000;

    // Configure and build server
    auto serverLogic = std::make_shared<ServerLogic>();
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setPort(PORT)
        .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
        .setServerLogic(serverLogic)
        .setServerControlAcceptor(serverLogic)
        .setLwsLogLevel(0)
        ;
    auto server = serverBuilder.build();

    // Configure and build client
    auto clientLogic = std::make_shared<ClientLogic>();
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setAddress("localhost")
        .setPort(PORT)
        .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
        .setClientLogic(clientLogic)
        .setClientControlAcceptor(clientLogic)
        .setLwsLogLevel(0)
        ;
    auto client = clientBuilder.build();

    // Give some time for the client and server to exchange messages.
    const auto timeout = 100U;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));

    return 0;
}
