#pragma once

#include <string_view>

struct i_logger
{
    virtual ~i_logger()                           = default;
    virtual void log(const std::string_view& msg) = 0;
};
