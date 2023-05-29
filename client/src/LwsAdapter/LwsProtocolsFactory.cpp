/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include "LwsAdapter/LwsCallback.hpp"
#include "LwsAdapter/LwsProtocolsFactory.hpp"

namespace wspp::cli
{
namespace
{

const lws_protocols LWS_PROTOCOL_LIST_TERM { nullptr, nullptr, 0, 0, 0, nullptr, 0 };

} // namespace

auto createLwsProtocols(ClientVersion version) -> LwsProtocols
{
    LwsCallback* callback = nullptr;

    switch (version)
    {
    case ClientVersion::v1_Amsterdam:
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
        LWS_PROTOCOL_LIST_TERM
    };
}

} // namespace wspp::cli
