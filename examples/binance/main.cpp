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

#include <future>
#include <iostream>

#include "lwspp/client/ClientBuilder.hpp"
#include "lwspp/client/EventHandlerBase.hpp"
#include "lwspp/client/IActor.hpp" // IWYU pragma: keep
#include "lwspp/client/SslSettingsBuilder.hpp"

using namespace lwspp;

const cli::Address ADDRESS = "ws-api.testnet.binance.vision";
const cli::Port PORT = 443;
const std::string PATH = "/ws-api/v3";
const std::string SUBSCRIBE = R"({"method": "SUBSCRIBE","params":["btcusdt@trade"],"id": 3})";
const std::string PING = R"({"id": "922bcc6e-9de8-440d-9e84-7c80933a8d0d","method": "ping"})";

const int MESSAGES_LIMIT = 5;

class ClientEventHandler : public cli::EventHandlerBase
{
public:
    void onConnect(cli::IConnectionInfoPtr) noexcept override
    {
        _actor->sendTextData(PING);
    }

    void onTextDataReceive(const cli::DataPacket& dataPacket) noexcept override
    {
        if (_messageCounter < MESSAGES_LIMIT)
        {
            std::cout << "client received the message: " << std::string{dataPacket.data, dataPacket.length} << std::endl;
        }

        if (++_messageCounter == MESSAGES_LIMIT)
        {
            _promise.set_value();
        }
    }

    void onError(const std::string& errorMessage) noexcept override
    {
        std::cout << "Got error: " << errorMessage << std::endl;
        _promise.set_value();
    }

    void onWarning(const std::string& warningMessage) noexcept override
    {
        std::cout << "Got warning: " << warningMessage << std::endl;
    }

    auto getFuture() -> std::shared_future<void>
    {
        return _promise.get_future();
    }

private:
    std::promise<void> _promise;
    int _messageCounter = 0;
};

auto main() -> int
{
    auto sslSettingsBuilder = cli::SslSettingsBuilder{};
    sslSettingsBuilder.allowSelfSignedServerCert().ignoreServerCaSert();

    // Configure and build client
    auto clientEventHandler = std::make_shared<ClientEventHandler>();
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setAddress(ADDRESS)
        .setPath(PATH)
        .setPort(PORT)
        .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
        .setEventHandler(clientEventHandler)
        .setActorAcceptor(clientEventHandler)
        .setSslSettings(sslSettingsBuilder.build())
        .setLwsLogLevel(0)
        ;
    auto client = clientBuilder.build();

    auto waitForMessages = clientEventHandler->getFuture();
    waitForMessages.wait();

    return 0;
}
