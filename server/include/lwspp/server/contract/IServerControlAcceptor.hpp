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

#include "lwspp/server/TypesFwd.hpp"

namespace lwspp
{
namespace srv
{
namespace contract
{

/**
 * @brief Accepts an IServerControl instance from the server builder.
 *
 * The server builder provides an IServerControl instance when constructing the server.
 * To utilize the IServerControl, set your instance of IServerControlAcceptor to the server builder
 * and obtain the IServerControl to perform actions defined by the IServerControl interface.
 * Users of the library must implement this interface themselves.
 */
class IServerControlAcceptor
{
public:
    IServerControlAcceptor() = default;
    virtual ~IServerControlAcceptor() = default;

    IServerControlAcceptor(const IServerControlAcceptor&) = default;
    auto operator=(const IServerControlAcceptor&) noexcept -> IServerControlAcceptor& = default;

    IServerControlAcceptor(IServerControlAcceptor&&) = default;
    auto operator=(IServerControlAcceptor&&) noexcept -> IServerControlAcceptor& = default;

public:
    virtual void acceptServerControl(IServerControlPtr) noexcept = 0;
};

} // namespace contract
} // namespace srv
} // namespace lwspp
