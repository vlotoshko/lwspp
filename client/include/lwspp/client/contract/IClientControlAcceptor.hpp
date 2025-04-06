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

#include "lwspp/client/TypesFwd.hpp"

namespace lwspp
{
namespace cli
{
namespace contract
{

/**
 * @brief Accepts an IClientControl instance from the client builder.
 *
 * The client builder provides an IClientControl instance when constructing the client.
 * To utilize the IClientControl, set your instance of IClientControlAcceptor to the client builder
 * and obtain the IClientControl to perform actions defined by the IClientControl interface.
 * Users of the library must implement this interface themselves.
 */
class IClientControlAcceptor
{
public:
    IClientControlAcceptor() = default;
    virtual ~IClientControlAcceptor() = default;

    IClientControlAcceptor(const IClientControlAcceptor&) = default;
    auto operator=(const IClientControlAcceptor&) noexcept -> IClientControlAcceptor& = default;

    IClientControlAcceptor(IClientControlAcceptor&&) = default;
    auto operator=(IClientControlAcceptor&&) noexcept -> IClientControlAcceptor& = default;

public:
    virtual void acceptClientControl(IClientControlPtr) noexcept = 0;
};

} // namespace contract
} // namespace cli
} // namespace lwspp
