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
#include "lwspp/client/IDataSender.hpp"
#include "lwspp/client/SslSettingsBuilder.hpp"

using namespace lwspp;

const cli::Port PORT = 9443;
const cli::Address ADDRESS = "stream.binance.com";
const std::string SUBSCRIBE = R"({"method": "SUBSCRIBE","params":["btcusdt@trade"],"id": 3})";
const int MESSAGES_LIMIT = 5;

class ClientEventHandler : public cli::EventHandlerBase
{
public:
    void onConnect(cli::IConnectionInfoPtr) noexcept override
    {
        _dataSender->sendTextData(SUBSCRIBE);
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
    // Configure and build client
    auto clientEventHandler = std::make_shared<ClientEventHandler>();
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setAddress(ADDRESS)
        .setPath("/ws")
        .setPort(PORT)
        .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
        .setEventHandler(clientEventHandler)
        .setDataSenderAcceptor(clientEventHandler)
        .setSslSettings(cli::SslSettingsBuilder{}.build())
        .setLwsLogLevel(0)
        ;
    auto client = clientBuilder.build();

    auto waitForMessages = clientEventHandler->getFuture();
    waitForMessages.wait();

    return 0;
}
