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

/**
 * @brief Accepts an IActor instance from the server builder.
 *
 * The server builder provides an IActor instance when constructing the server.
 * To utilize the IActor, set your instance of IActorAcceptor to the server builder
 * and obtain the IActor to perform actions defined by the IActor interface.
 * Users of the library must implement this interface themselves.
 */
class IActorAcceptor
{
public:
    IActorAcceptor() = default;
    virtual ~IActorAcceptor() = default;

    IActorAcceptor(const IActorAcceptor&) = default;
    auto operator=(const IActorAcceptor&) noexcept -> IActorAcceptor& = default;

    IActorAcceptor(IActorAcceptor&&) = default;
    auto operator=(IActorAcceptor&&) noexcept -> IActorAcceptor& = default;

public:
    virtual void acceptActor(IActorPtr) noexcept = 0;
};

} // namespace srv
} // namespace lwspp
