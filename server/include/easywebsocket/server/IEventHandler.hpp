/*
 * EasyWebsockets - C++ wrapper for the libwebsockets library
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
#include <vector>

#include "easywebsocket/server/Types.hpp"
#include "easywebsocket/server/TypesFwd.hpp"

namespace ews::srv
{

/**
 * @brief The IEventHandler class defines an interface for implementing server behavior.
 * Users of the library must implement this interface themselves.
 */
class IEventHandler
{
public:
    IEventHandler() = default;
    virtual ~IEventHandler() = default;

    IEventHandler(IEventHandler&&) = default;
    auto operator=(IEventHandler&&) noexcept -> IEventHandler& = default;

    IEventHandler(const IEventHandler&) = delete;
    auto operator=(const IEventHandler&) noexcept -> IEventHandler& = delete;

public:
    // Invoked when the server receives binary data from the client.
    virtual void onBinaryDataReceive(SessionId, const std::vector<char>& data, size_t bytesRemains) noexcept = 0;
    // Invoked when the server receives text data from the client. This method expects valid UTF-8 text.
    virtual void onTextDataReceive(SessionId, const std::string& message, size_t bytesRemains) noexcept = 0;

    virtual void onConnect(ISessionInfoPtr) noexcept = 0;
    virtual void onDisconnect(SessionId) noexcept = 0;
    virtual void onError(SessionId, const std::string& errorMessage) noexcept = 0;
    virtual void onWarning(SessionId, const std::string& errorMessage) noexcept = 0;
};

} // namespace ews::srv
