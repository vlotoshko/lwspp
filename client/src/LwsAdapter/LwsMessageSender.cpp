/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include "LwsAdapter/ILwsSession.hpp"
#include "LwsAdapter/LwsMessageSender.hpp"

namespace ews::cli
{

LwsMessageSender::LwsMessageSender(const ILwsSessionPtr& s)
    : _session(s)
{}

void LwsMessageSender::sendMessage(const std::string& message)
{
    if (auto session = _session.lock())
    {
        session->addTextData(message);
        lws_callback_on_writable(session->getLwsInstance());
    }
}

void LwsMessageSender::sendData(const std::vector<char>& data)
{
    if (auto session = _session.lock())
    {
        session->addBinaryData(data);
        lws_callback_on_writable(session->getLwsInstance());
    }
}

} // namespace ews::cli
