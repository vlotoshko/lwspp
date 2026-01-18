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

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
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
    explicit ClientLogic(std::atomic<int>& received, std::condition_variable& cv, std::mutex& mutex)
        : _received(received), _cv(cv), _mutex(mutex)
    {}

    void onConnect(cli::IConnectionInfoPtr) noexcept override
    {
        _clientControl->sendTextData("hello server!");
    }

    void onTextDataReceive(const cli::DataPacket& dataPacket) noexcept override
    {
        std::cout << "client received the message: " << std::string{dataPacket.data, dataPacket.length} << std::endl;
        _received.fetch_add(1);
        _cv.notify_all();
    }


private:
    std::atomic<int>& _received;
    std::condition_variable& _cv;
    std::mutex& _mutex;
};

class ServerLogic : public srv::ServerLogicBase
{
public:
    explicit ServerLogic(std::atomic<int>& received, std::condition_variable& cv, std::mutex& mutex)
        : _received(received), _cv(cv), _mutex(mutex)
    {}

    void onTextDataReceive(srv::ConnectionId, const srv::DataPacket& dataPacket) noexcept override
    {
        std::cout << "server received the message: " << std::string{dataPacket.data, dataPacket.length}  << std::endl;
        _serverControl->sendTextData("hello client!");
        _received.fetch_add(1);
        _cv.notify_all();
    }

private:
    std::atomic<int>& _received;
    std::condition_variable& _cv;
    std::mutex& _mutex;
};

auto main() -> int
{
    const srv::Port PORT = 9000;
    const std::string protocolName = "lwspp";
    std::atomic<int> messagesReceived{0};
    std::mutex mutex;
    std::condition_variable cv;

    // Configure and build server
    auto serverLogic = std::make_shared<ServerLogic>(messagesReceived, cv, mutex);
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setPort(PORT)
        .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
        .setProtocolName(protocolName)
        .setServerLogic(serverLogic)
        .setServerControlAcceptor(serverLogic)
        .setLwsLogLevel(0)
        ;
    auto server = serverBuilder.build();

    // Configure and build client
    auto clientLogic = std::make_shared<ClientLogic>(messagesReceived, cv, mutex);
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setAddress("localhost")
        .setPort(PORT)
        .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
        .setProtocolName(protocolName)
        .setClientLogic(clientLogic)
        .setClientControlAcceptor(clientLogic)
        .setLwsLogLevel(0)
        ;
    auto client = clientBuilder.build();

    // Wait for both messages (server + client) or time out.
    const auto timeout = std::chrono::seconds(5);
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait_for(lock, timeout, [&messagesReceived] { return messagesReceived.load() >= 2; });
    }

    return 0;
}
