#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <algorithm>

#include "Event.hpp"
#include "Table.hpp"
#include "Time.hpp"

class ComputerClub
{
    int tables_num;
    Time open_time, close_time;
    int price;
    std::vector<Table> tables;
    std::map<std::string, int> clients; // client name -> table number
    std::queue<std::string> waiting_queue;
    std::vector<std::string> output;

    void add_output_event(const Time& time, int id, const std::string& message);

    void handle_event(const Event& event);
    void handle_client_arrives(const Event& event);
    void handle_client_sits(const Event& event);
    void handle_client_waits(const Event& event);
    void handle_client_leaves(const Event& event);
    void handle_remaining_clients_leave();
    
public:
    ComputerClub(int n, Time open, Time close, int price);
    void process_events(const std::vector<Event> &events);
    void generate_report();
};