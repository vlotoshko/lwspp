/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <memory>

#include "libwebsockets.h"

namespace ews::srv
{

class LwsContextDeleter
{
public:
    void operator()(lws_context* p)
    {
        lws_context_destroy(p);
    }
};

} // namespace ews::srv
