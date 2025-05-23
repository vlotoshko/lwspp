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

#include <string>

#include "lwspp/server/Types.hpp"
#include "lwspp/server/TypesFwd.hpp"

namespace lwspp
{
namespace srv
{
namespace contract
{

/**
 * @brief The IServerLogic class defines an interface for implementing server behavior.
 * Users of the library must implement this interface themselves.
 */
class IServerLogic
{
public:
    IServerLogic() = default;
    virtual ~IServerLogic() = default;

    IServerLogic(IServerLogic&&) = default;
    auto operator=(IServerLogic&&) noexcept -> IServerLogic& = default;

    IServerLogic(const IServerLogic&) = delete;
    auto operator=(const IServerLogic&) noexcept -> IServerLogic& = delete;

public:
    // Invoked when the server receives first data packet of the message.
    virtual void onFirstDataPacket(ConnectionId, size_t messageLength) noexcept = 0;
    // Invoked when the server receives binary data from the client.
    virtual void onBinaryDataReceive(ConnectionId, const DataPacket&) noexcept = 0;
    // Invoked when the server receives text data from the client. This method expects valid UTF-8 text.
    virtual void onTextDataReceive(ConnectionId, const DataPacket&) noexcept = 0;

    virtual void onConnect(IConnectionInfoPtr) noexcept = 0;
    virtual void onDisconnect(ConnectionId) noexcept = 0;
    virtual void onError(ConnectionId, const std::string& errorMessage) noexcept = 0;
    virtual void onWarning(ConnectionId, const std::string& errorMessage) noexcept = 0;
};

} // namespace contract
} // namespace srv
} // namespace lwspp
