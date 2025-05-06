#pragma once

struct Table
{
    int revenue = 0;
    Time occupied_time;
    Time start_time;
    bool is_occupied = false;
    std::string client;
};