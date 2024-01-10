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

#include "LwsAdapter/LwsCallback.hpp"
#include "LwsAdapter/LwsProtocolsFactory.hpp"

namespace lwspp
{
namespace cli
{
namespace
{

const lws_protocols LWS_PROTOCOL_TERM_LIST { nullptr, nullptr, 0, 0, 0, nullptr, 0 };

} // namespace

auto createLwsProtocols(CallbackVersion version) -> LwsProtocols
{
    LwsCallback* callback = nullptr;

    switch (version)
    {
    case CallbackVersion::v1_Amsterdam:
        callback = lwsCallback_v1;
        break;
    default:
        // TODO: throw exception unsupported version
        break;
    }

    return LwsProtocols {
        {
            "/",
            callback,
            0, // per connection data size, not used
            0, // rx buffer size
            static_cast<unsigned int>(version), // id
            nullptr, // pointer on user data
            0 // tx packet size
        },
        LWS_PROTOCOL_TERM_LIST
    };
}

} // namespace cli
} // namespace lwspp
