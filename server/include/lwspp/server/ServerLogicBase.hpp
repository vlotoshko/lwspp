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

#include "lwspp/server/contract/IServerControlAcceptor.hpp"
#include "lwspp/server/contract/IServerLogic.hpp"

namespace lwspp
{
namespace srv
{
/**
 * @brief The ServerLogicBase class serves as a convenient base class for implementing the IServerLogic interface.
 * It provides stubs for all overridden methods in the IServerLogic interface and also implements
 * the IServerControlAcceptor to obtain the IServerControl.
 */
class ServerLogicBase : public contract::IServerLogic, public contract::IServerControlAcceptor
{
public:
    void onConnect(IConnectionInfoPtr) noexcept override;
    void onDisconnect(ConnectionId) noexcept override;

    void onFirstDataPacket(ConnectionId, size_t messageLength) noexcept override;
    void onBinaryDataReceive(ConnectionId, const DataPacket&) noexcept override;
    void onTextDataReceive(ConnectionId, const DataPacket&) noexcept override;
    void onError(ConnectionId, const std::string& errorMessage) noexcept override;
    void onWarning(ConnectionId, const std::string& warningMessage) noexcept override;
    
    void acceptServerControl(IServerControlPtr) noexcept override;

protected:
    IServerControlPtr _serverControl = nullptr; // NOLINT(*-non-private-member-variables-in-classes)
};

} // namespace srv
} // namespace lwspp
