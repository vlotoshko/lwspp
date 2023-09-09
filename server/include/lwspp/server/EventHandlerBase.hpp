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

#include "lwspp/server/IEventHandler.hpp"
#include "lwspp/server/IDataSenderAcceptor.hpp"

namespace lwspp
{
namespace srv
{
/**
 * @brief The EventHandlerBase class serves as a convenient base class for implementing the IEventHandler interface.
 * It provides stubs for all overridden methods in the IEventHandler interface and also implements
 * the IDataSenderAcceptor to obtain the IDataSender.
 */
class EventHandlerBase : public IEventHandler, public IDataSenderAcceptor
{
public:
    void onConnect(ISessionInfoPtr) noexcept override;
    void onDisconnect(SessionId) noexcept override;

    void onBinaryDataReceive(SessionId, const std::vector<char>& data, size_t bytesRemains) noexcept override;
    void onTextDataReceive(SessionId, const std::string& message, size_t bytesRemains) noexcept override;
    void onError(SessionId, const std::string& errorMessage) noexcept override;
    void onWarning(SessionId, const std::string& warningMessage) noexcept override;

    void acceptDataSender(IDataSenderPtr) noexcept override;

protected:
    IDataSenderPtr _dataSender;
};

} // namespace srv
} // namespace lwspp
