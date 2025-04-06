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

#include "lwspp/client/IClientControlAcceptor.hpp"
#include "lwspp/client/IClientLogic.hpp"

namespace lwspp
{
namespace cli
{

/**
 * @brief The ClientLogicBase class serves as a convenient base class for implementing the IClientLogic interface.
 * It provides stubs for all overridden methods in the IClientLogic interface and also implements
 * the IClientControlAcceptor to obtain the IClientControl.
 */
class ClientLogicBase : public IClientLogic, public IClientControlAcceptor
{
public:
    void onConnect(IConnectionInfoPtr) noexcept override;
    void onDisconnect() noexcept override;

    void onFirstDataPacket(size_t messageLength) noexcept override;
    void onBinaryDataReceive(const DataPacket&) noexcept override;
    void onTextDataReceive(const DataPacket&) noexcept override;
    void onError(const std::string& errorMessage) noexcept override;
    void onWarning(const std::string& warningMessage) noexcept override;
    
    void acceptClientControl(IClientControlPtr) noexcept override;

protected:
    IClientControlPtr _clientControl = nullptr; // NOLINT(*-non-private-member-variables-in-classes)
};

} // namespace cli
} // namespace lwspp
