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

#include <future>

#include "TypesFwd.hpp"
#include "lwspp/client/IClient.hpp"

namespace lwspp
{
namespace cli
{

/**
 * @brief The Client class wraps the internal LwsClient implementation.
 *
 * The client automatically connects upon construction and disconnects upon destruction.
 * It operates within a separate thread to establish and maintain the connection with the server.
 */
class Client : public IClient
{
public:
    explicit Client(const ClientContext&);
    ~Client() override;

    Client(Client&&) noexcept = default;
    auto operator=(Client&&) noexcept -> Client& = default;

    Client(const Client&) = delete;
    auto operator=(const Client&) -> Client& = delete;

private:
    LwsClientPtr _lwsClient;
    std::future<void> _clientStop;
};

} // namespace cli
} // namespace lwspp
