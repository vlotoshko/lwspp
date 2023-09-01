/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "easywebsocket/client/IMessageSender.hpp"

#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::cli
{

class LwsMessageSender : public IMessageSender
{
public:
    explicit LwsMessageSender(const ILwsSessionPtr&);

    void sendMessage(const std::string&) override;
    void sendData(const std::vector<char>&) override;

private:
    ILwsSessionWeak _session;
};

} // namespace ews::cli
