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
#include <vector>

#include "lwspp/server/Types.hpp"

namespace lwspp
{
namespace srv
{

/**
 * @brief The IServerControl class defines an interface to perform actions with the server.
 * The instance of IServerControl can be obtained using IServerControlAcceptor when building the server.
 */
class IServerControl
{
public:
    IServerControl() = default;
    virtual ~IServerControl() = default;

    IServerControl(const IServerControl&) = default;
    auto operator=(const IServerControl&) -> IServerControl& = default;

    IServerControl(IServerControl&&) noexcept = default;
    auto operator=(IServerControl&&) noexcept -> IServerControl& = default;

public:
    // Sends text data to the specified client identified by ConnectionId.
    // The provided text data should be valid UTF-8 text.
    virtual void sendTextData(ConnectionId, const std::string&) = 0;

    // Sends binary data to the specified client identified by ConnectionId.
    virtual void sendBinaryData(ConnectionId, const std::vector<char>&) = 0;

    // Sends text data to all connected clients.
    //The provided text data should be valid UTF-8 text.
    virtual void sendTextData(const std::string&) = 0;

    // Sends binary data to all connected clients.
    virtual void sendBinaryData(const std::vector<char>&) = 0;

    // Closes the specified client connection.
    virtual void closeConnection(ConnectionId) = 0;
};

} // namespace srv
} // namespace lwspp
