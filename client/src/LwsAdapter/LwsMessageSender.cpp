/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include "LwsAdapter/ILwsSession.hpp"
#include "LwsAdapter/LwsMessageSender.hpp"

namespace wspp::cli
{

LwsMessageSender::LwsMessageSender(const ILwsSessionPtr& s)
    : _session(s)
{}

void LwsMessageSender::sendMessage(const std::string& message)
{
    if (auto session = _session.lock())
    {
        session->addMessage(message);
        lws_callback_on_writable(session->getLwsInstance());
    }
}

} // namespace wspp::cli
