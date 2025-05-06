#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <algorithm>

#include "ComputerClub.hpp"
#include "Event.hpp"
#include "Table.hpp"
#include "Time.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Only 1 argument can be passed";
        return 1;
    }

    std::ifstream file;
    file.open(argv[1]);

    if (file.is_open() == false) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    int tables_num = 0;
    Time open_time, close_time;
    int price = 0;
    std::vector<Event> events;

    try
    {   
        int line_num = 0;
        std::string line;
        std::string tables_num_str, open_time_str, close_time_str, price_str;

        // Tables
        if (!(file >> tables_num_str))
            throw std::runtime_error("Missing tables number");
        tables_num = std::stoi(tables_num_str);
        if (tables_num <= 0)
            throw std::runtime_error("Invalid format on line " + std::to_string(line_num) + ":" + line);
        file.ignore(1, '\n');
        line_num++;
        // std::cout << tables_num << std::endl;
        
        // Time
        std::string open_str, close_str;
        if (!(file >> open_str >> close_str))
            throw std::runtime_error("Missing opening or closing time");
        open_time = Time(open_str);
        close_time = Time(close_str);
        file.ignore(1, '\n');
        line_num++;
        // std::cout << open_time.to_string();

        // Price
        if (!(file >> price_str))
            throw std::runtime_error("Missing price");
        price = std::stoi(price_str);
        if (price <= 0)
        throw std::runtime_error("Invalid format on line " + std::to_string(line_num) + ":" + line);
        file.ignore(1, '\n');
        line_num++;
        // std::cout << price << std::endl;

        // Events
        while (std::getline(file, line))
        {
            line_num++;
            if (line.empty())
                continue;

            std::stringstream event_ss(line);
            std::string time_str;
            int event_id;
            event_ss >> time_str >> event_id;

            if (time_str.size() != 5 || time_str[2] != ':')
                throw std::runtime_error("Invalid format on line " + std::to_string(line_num) + ":" + line);

            Event event;
            event.time = Time(time_str);
            event.id = event_id;

            std::string arg;
            while (event_ss >> arg)
                event.args.push_back(arg);
            
            events.push_back(event);

            // std::cout << line << std::endl;
        }

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    ComputerClub comp_club(tables_num, open_time, close_time, price);
    comp_club.process_events(events);
    comp_club.generate_report();

    file.close();
}