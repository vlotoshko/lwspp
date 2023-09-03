/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include "LwsAdapter/LwsCallback.hpp"
#include "LwsAdapter/LwsProtocolsFactory.hpp"

namespace ews::cli
{
namespace
{

const lws_protocols LWS_PROTOCOL_TERM_LIST { nullptr, nullptr, 0, 0, 0, nullptr, 0 };

} // namespace

auto createLwsProtocols(CallbackVersion version) -> LwsProtocols
{
    LwsCallback* callback = nullptr;

    switch (version)
    {
    case CallbackVersion::v1_Amsterdam:
        callback = lwsCallback_v1;
        break;
    default:
        // TODO: throw exception unsupported version
        break;
    }

    return LwsProtocols {
        {
            "/",
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

} // namespace ews::cli
