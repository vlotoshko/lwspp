/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "Consts.hpp"
#include "Session.hpp"

namespace wspp::srv
{
namespace
{

// Split the message into pieces if it does not fit into the max size
auto splitMessages(const std::string& message, unsigned int maxSize) -> std::vector<std::string>
{
    std::vector<std::string> result;
    for (unsigned i = 0; i < message.size(); i += maxSize)
    {
        auto itBegin = message.cbegin() + i;
        auto itEnd = (i + maxSize > message.size()) ? message.cend() : itBegin + maxSize;
        result.emplace_back(itBegin, itEnd);
    }

    return result;
}

} // namespace

void Session::addMessage(const std::string& message)
{
    auto splitedMessage = splitMessages(message, MAX_MESSAGE_SIZE);

    const std::lock_guard<std::mutex> guard(_mutex);
    for(auto& entry : splitedMessage)
    {
        _messages.emplace(std::move(entry));
    }
}

auto Session::getMessages() -> std::queue<std::string>
{
    std::queue<std::string> messages;
    {
        const std::lock_guard<std::mutex> guard(_mutex);
        _messages.swap(messages);
    }
    return messages;
}

} // namespace wspp::srv
