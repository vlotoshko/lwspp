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

#include "Consts.hpp"
#include "lwspp/client/TypesFwd.hpp"

namespace lwspp
{
namespace cli
{

/**
 * @brief The ClientContext class is the 'high-level' context to create the
 * 'low-level' lws_context of the libwebsocket
 */
class ClientContext
{
public:
    CallbackVersion callbackVersion = UNDEFINED_CALLBACK_VERSION;
    contract::IClientLogicPtr clientLogic;
    contract::IClientControlAcceptorPtr clientControlAcceptor;

    Address address = UNDEFINED_ADDRESS;
    Port port = UNDEFINED_PORT;

    Path path = DEFAULT_URI_PATH;
    std::string protocolName = DEFAULT_PROTOCOL_NAME;
    int keepAliveTimeout = UNDEFINED_UNSET;
    int keepAliveProbes = UNDEFINED_UNSET;
    int keepAliveProbesInterval = UNDEFINED_UNSET;
    int lwsLogLevel = DEFAULT_LWS_LOG_LEVEL;
    SslSettingsPtr ssl;
};

} // namespace cli
} // namespace lwspp
