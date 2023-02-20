/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include <iostream>

#include "LwsAdapter/LwsCallback.hpp"
#include "LwsAdapter/LwsProtocolsFactory.hpp"

namespace wspp::srv
{
namespace
{

const lws_protocols LWS_PROTOCOL_LIST_TERM { nullptr, nullptr, 0, 0, 0, nullptr, 0 };

} // namespace

auto createLwsProtocols(ServerVersion version) -> LwsProtocols
{
    LwsCallback* callback = nullptr;

    switch (version)
    {
    case ServerVersion::v1_Andromeda:
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
            0, // user data struct not used
            0,
            0,
            nullptr,
            0
        },
        LWS_PROTOCOL_LIST_TERM
    };
}

} // namespace wspp::srv
