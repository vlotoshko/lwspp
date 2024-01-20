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

#include <queue>
#include <string>

#include "lwspp/server/Types.hpp"
#include "LwsAdapter/LwsTypes.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace lwspp
{
namespace srv
{

/**
 * @brief The ILwsConnection class represents the connection to the client.
 */
class ILwsConnection
{
public:
    ILwsConnection() = default;
    virtual ~ILwsConnection() = default;

    ILwsConnection(const ILwsConnection&) = default;
    auto operator=(const ILwsConnection&) -> ILwsConnection& = default;

    ILwsConnection(ILwsConnection&&) noexcept = default;
    auto operator=(ILwsConnection&&) noexcept -> ILwsConnection& = default;

public:
    virtual auto getConnectionId() const -> ConnectionId = 0;
    virtual auto getLwsInstance() -> LwsInstanceRawPtr = 0;

    virtual void addBinaryDataToSend(const std::vector<char>&) = 0;
    virtual void addTextDataToSend(const std::string&) = 0;
    virtual auto getPendingData() -> std::queue<Message>& = 0;

    virtual auto markedToClose() -> bool = 0;
    virtual void markToClose() = 0;
};

} // namespace srv
} // namespace lwspp
