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

#pragma once

#include <condition_variable>
#include <mutex>

#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace lwspp
{
namespace cli
{

class LwsClient
{
public:
    explicit LwsClient(const ClientContext&);
    ~LwsClient();

    LwsClient(LwsClient&&) = delete;
    auto operator=(LwsClient&&) noexcept -> LwsClient& = delete;

    LwsClient(const LwsClient&) = delete;
    auto operator=(const LwsClient&) noexcept -> LwsClient& = delete;

    // NOTE: the 'connect' method blocks the thread
    void connect();
    void disconnect();

private:
    void setupLowLevelContext_();
    void setupConnectionInfo_();
    void waitForClientStopping_();

private:
    enum class State : std::uint8_t
    {
        Initial,
        Started,
        Stopping,
        Stopped,
    };

    ILwsCallbackContextPtr _callbackContext;
    LwsDataHolderPtr _dataHolder;

    LowLevelContextPtr _lowLevelContext;
    LwsInstanceRawPtr _wsInstance = nullptr;
    LwsConnectInfo _lwsConnectionInfo;

    std::condition_variable _isStoppedCV;
    State _state = State::Initial;
    std::mutex _mutex;
};

} // namespace cli
} // namespace lwspp
