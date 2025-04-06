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

namespace lwspp
{
namespace cli
{
/**
 * @brief The IClientControl class defines an interface to perform actions with the client.
 * The instance of IClientControl can be obtained using IClientControlAcceptor when building the client.
 */
class IClientControl
{
public:
    IClientControl() = default;
    virtual ~IClientControl() = default;

    IClientControl(const IClientControl&) = default;
    auto operator=(const IClientControl&) -> IClientControl& = default;

    IClientControl(IClientControl&&) noexcept = default;
    auto operator=(IClientControl&&) noexcept -> IClientControl& = default;

public:
    // Sends text data to the server. The provided text data should be valid UTF-8 text.
    virtual void sendTextData(const std::string&) = 0;

    // Sends binary data to the server
    virtual void sendBinaryData(const std::vector<char>&) = 0;
};

} // namespace cli
} // namespace lwspp
