#include "../include/Time.hpp"

Time::Time() :  min(0) {}
Time::Time(const std::string& time)
{
    std::stringstream ss(time);
    int h, m;
    ss >> h;
    ss.ignore(1, ':');
    ss >> m;
    min = h * 60 + m;
}
Time::Time(int minutes) : min(minutes) {}

std::string Time::to_string() const
{
    int h = min / 60;
    int m = min % 60;
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << h << ":" << std::setw(2) << std::setfill('0') << m;
    return ss.str();
}

bool Time::operator<(const Time& time) const { return min < time.min; }
bool Time::operator<=(const Time& time) const { return min <= time.min; }
bool Time::operator>(const Time& time) const { return min > time.min; }
bool Time::operator>=(const Time& time) const { return min >= time.min; }