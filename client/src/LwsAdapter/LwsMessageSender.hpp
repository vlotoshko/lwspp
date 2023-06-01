/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "websocketpp/client/IMessageSender.hpp"

#include "LwsAdapter/LwsTypesFwd.hpp"

namespace wspp::cli
{

class LwsMessageSender : public IMessageSender
{
public:
    explicit LwsMessageSender(const ILwsSessionPtr&);

    void sendMessage(const std::string& message) override;

private:
    ILwsSessionWeak _session;
};

} // namespace wspp::cli
