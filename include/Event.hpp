#pragma once
#include <string>

#include "Time.hpp"

struct Event
{
    Time time;
    int id;
    std::vector<std::string> args; // client (always) - table id (optional)
};