/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include <memory>

#include "libwebsockets.h"

namespace ews::cli
{

class LwsContextDeleter
{
public:
    void operator()(lws_context* p)
    {
        lws_context_destroy(p);
    }
};

} // namespace ews::cli
