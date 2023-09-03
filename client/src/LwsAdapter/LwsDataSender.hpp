/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "easywebsocket/client/IDataSender.hpp"

#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::cli
{

class LwsDataSender : public IDataSender
{
public:
    explicit LwsDataSender(const ILwsSessionPtr&);

    void sendTextData(const std::string&) override;
    void sendBinaryData(const std::vector<char>&) override;

private:
    ILwsSessionWeak _session;
};

} // namespace ews::cli
