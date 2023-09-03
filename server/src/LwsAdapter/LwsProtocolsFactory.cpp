/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include <string>

#include "LwsAdapter/LwsCallback.hpp"
#include "LwsAdapter/LwsProtocolsFactory.hpp"

namespace ews::srv
{
namespace
{

const lws_protocols LWS_PROTOCOL_TERM_LIST { nullptr, nullptr, 0, 0, 0, nullptr, 0 };

} // namespace

auto createLwsProtocols(CallbackVersion version, const std::string& protocolName) -> LwsProtocols
{
    LwsCallback* callback = nullptr;

    switch (version)
    {
    case CallbackVersion::v1_Andromeda:
        callback = lwsCallback_v1;
        break;
    default:
        // TODO: throw exception unsupported version
        break;
    }

    return LwsProtocols {
        {
            protocolName.c_str(),
            callback,
            0, // per session data size, not used
            0, // rx buffer size
            static_cast<unsigned int>(version), // id
            nullptr, // pointer on user data
            0 // tx packet size
        },
        LWS_PROTOCOL_TERM_LIST
    };
}

} // namespace ews::srv
