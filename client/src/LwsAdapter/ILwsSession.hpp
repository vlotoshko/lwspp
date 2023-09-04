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

#include <queue>
#include <string>

#include "LwsAdapter/LwsTypes.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::cli
{

/**
 * @brief The ILwsSession class represents the connection to the server.
 */
class ILwsSession
{
public:
    ILwsSession() = default;
    virtual ~ILwsSession() = default;

    ILwsSession(const ILwsSession&) = default;
    auto operator=(const ILwsSession&) -> ILwsSession& = default;

    ILwsSession(ILwsSession&&) noexcept = default;
    auto operator=(ILwsSession&&) noexcept -> ILwsSession& = default;

public:
    virtual auto getLwsInstance() -> LwsInstanceRawPtr = 0;

    virtual void addBinaryDataToSend(const std::vector<char>&) = 0;
    virtual void addTextDataToSend(const std::string&) = 0;
    virtual auto getPendingData() -> std::queue<Message>& = 0;
};

} // namespace ews::cli
