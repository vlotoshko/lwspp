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

#include <vector>

#include "lwspp/server/Types.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace lwspp
{
namespace srv
{

/**
 * @brief The ILwsConnections class represents a container for storing and accessing all ILwsConnection instances.
 */
class ILwsConnections
{
public:
    ILwsConnections() = default;
    virtual ~ILwsConnections() = default;

    ILwsConnections(const ILwsConnections&) = default;
    auto operator=(const ILwsConnections&) -> ILwsConnections& = default;

    ILwsConnections(ILwsConnections&&) noexcept = default;
    auto operator=(ILwsConnections&&) noexcept -> ILwsConnections& = default;

public:
    virtual void add(ILwsConnectionPtr) = 0;
    virtual void remove(ConnectionId) = 0;
    virtual auto get(ConnectionId) -> ILwsConnectionPtr = 0;
    virtual auto getAllConnections() -> std::vector<ILwsConnectionPtr> = 0;
};

} // namespace srv
} // namespace lwspp
