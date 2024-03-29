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

#include "LwsAdapter/LwsTypesFwd.hpp"

namespace lwspp
{
namespace srv
{

/**
 * @brief The ILwsCallbackNotifier class notifies the lws service that there are
 * pending data to send
 */
class ILwsCallbackNotifier
{
public:
    ILwsCallbackNotifier() = default;
    virtual ~ILwsCallbackNotifier() = default;

    ILwsCallbackNotifier(const ILwsCallbackNotifier&) = default;
    auto operator=(const ILwsCallbackNotifier&) -> ILwsCallbackNotifier& = default;

    ILwsCallbackNotifier(ILwsCallbackNotifier&&) noexcept = default;
    auto operator=(ILwsCallbackNotifier&&) noexcept -> ILwsCallbackNotifier& = default;

public:
    // Notifies that pending data was added to send to the specific connection.
    virtual void notifyPendingDataAdded(const ILwsConnectionPtr&) = 0;
    // Notifies that pending data was added to send to the all connections.
    virtual void notifyPendingDataAdded() = 0;
    // Notifies that this connection should be closed.
    virtual void notifyCloseConnection(const ILwsConnectionPtr&) = 0;
};

} // namespace srv
} // namespace lwspp
