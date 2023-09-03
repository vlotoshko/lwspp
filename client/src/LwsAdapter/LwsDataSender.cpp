/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include "LwsAdapter/ILwsSession.hpp"
#include "LwsAdapter/LwsDataSender.hpp"

namespace ews::cli
{

LwsDataSender::LwsDataSender(const ILwsSessionPtr& s)
    : _session(s)
{}

void LwsDataSender::sendTextData(const std::string& message)
{
    if (auto session = _session.lock())
    {
        session->addTextDataToSend(message);
        lws_callback_on_writable(session->getLwsInstance());
    }
}

void LwsDataSender::sendBinaryData(const std::vector<char>& data)
{
    if (auto session = _session.lock())
    {
        session->addBinaryDataToSend(data);
        lws_callback_on_writable(session->getLwsInstance());
    }
}

} // namespace ews::cli
