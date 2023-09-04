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

#include <string>

#include "easywebsocket/server/CallbackVersions.hpp"
#include "easywebsocket/server/Types.hpp"

namespace ews::srv
{

const CallbackVersion UNDEFINED_CALLBACK_VERSION = CallbackVersion::Undefined;
const Port UNDEFINED_PORT = static_cast<int>(0U - 1);
const std::string UNDEFINED_FILE_PATH = "UNDEFINED_FILE_PATH";
const std::string UNDEFINED_NAME = "UNDEFINED_NAME";
const int UNDEFINED_UNSET = 0;

const SessionId ALL_SESSIONS = static_cast<int>(0U - 1);
const unsigned int MAX_PATH_SIZE = 1024 * 4;
const std::string DEFAULT_PROTOCOL_NAME = "/";
// 7 = LLL_ERR | LLL_WARN | LLL_NOTICE - default value for the libwebsockets 4.3.2
const int DEFAULT_LWS_LOG_LEVEL = 7;

} // namespace ews::srv
