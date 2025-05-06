#pragma once
#include <string>
#include <sstream>
#include <iomanip>

struct Time
{
    int min;

    Time();
    Time(const std::string& time);
    Time(int minutes);

    std::string to_string() const;

    bool operator<(const Time& time) const;
    bool operator<=(const Time& time) const;
    bool operator>(const Time& time) const;
    bool operator>=(const Time& time) const;
};