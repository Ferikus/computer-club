#include "../include/ComputerClub.hpp"

void ComputerClub::add_output_event(const Time& time, int id, const std::string& message = "") {
    std::string event = time.to_string() + " " + std::to_string(id);
    if (!message.empty()) {
        event += " " + message;
    }
    output.push_back(event);
}

void ComputerClub::handle_event(const Event& event) {
    switch (event.id) {
        case 1:
            handle_client_arrives(event);
            break;
        case 2:
            handle_client_sits(event);
            break;
        case 3:
            handle_client_waits(event);
            break;
        case 4:
            handle_client_leaves(event);
            break;
    }
}

void ComputerClub::handle_client_arrives(const Event &event) {
    const std::string& client = event.args[0];
    
    if (clients.find(client) != clients.end()) {
        add_output_event(event.time, 13, "YouShallNotPass");
        return;
    }
    
    if (close_time > open_time) { // overnight check
        if (event.time < open_time || event.time >= close_time) {
            add_output_event(event.time, 13, "NotOpenYet");
            return;
        }
    }
    else {
        if (event.time < open_time && event.time >= close_time) {
            add_output_event(event.time, 13, "NotOpenYet");
            return;
        }
    }
    
    clients[client] = 0; // not sitting
}

void ComputerClub::handle_client_sits(const Event &event) {
    const std::string& client = event.args[0];
    int new_table = std::stoi(event.args[1]);
    
    if (tables[new_table].is_occupied) {
        add_output_event(event.time, 13, "PlaceIsBusy");
        return;
    }

    if (clients.find(client) == clients.end()) {
        add_output_event(event.time, 13, "ClientUnknown");
        return;
    }
    
    if (clients[client] != 0) { // if client's at the table
        int old_table = clients[client];
        int duration;
        if (event.time.min > tables[old_table].start_time.min) {
            duration = event.time.min - tables[old_table].start_time.min;
        }
        else {
            duration = event.time.min + (24 * 60 - tables[old_table].start_time.min);
        }
        int hours = (duration + 59) / 60;
        tables[old_table].revenue += hours * price;
        tables[old_table].occupied_time.min += duration;
        tables[old_table].is_occupied = false;
    }
    
    tables[new_table].is_occupied = true;
    tables[new_table].client = client;
    tables[new_table].start_time = event.time;
    clients[client] = new_table;
}

void ComputerClub::handle_client_waits(const Event &event) {
    const std::string& client = event.args[0];
    
    for (int i = 1; i <= tables_num; i++) {
        if (tables[i].is_occupied == false) {
            add_output_event(event.time, 13, "ICanWaitNoLonger!");
            return;
        }
    }
    
    if (waiting_queue.size() > tables_num) {
        clients.erase(client);
        add_output_event(event.time, 11, client);
        return;
    }
    
    waiting_queue.push(client);
}

void ComputerClub::handle_client_leaves(const Event &event) {
    const std::string& client = event.args[0];
    
    if (clients.find(client) == clients.end()) {
        add_output_event(event.time, 13, "ClientUnknown");
        return;
    }
    
    int table_num = clients[client];
    if (table_num != 0) {
        // free the table + calculate revenue
        int duration;
        if (event.time.min > tables[table_num].start_time.min) {
            duration = event.time.min - tables[table_num].start_time.min;
        }
        else {
            duration = event.time.min + (24 * 60 - tables[table_num].start_time.min);
        }
        int hours = (duration + 59) / 60;
        tables[table_num].occupied_time.min += duration;
        tables[table_num].revenue += hours * price;
        tables[table_num].is_occupied = false;
        
        // next client from the queue sits at the table
        if (waiting_queue.empty() == false) {
            std::string next_client = waiting_queue.front();
            waiting_queue.pop();
            tables[table_num].is_occupied = true;
            tables[table_num].client = next_client;
            tables[table_num].start_time = event.time;
            clients[next_client] = table_num;
            add_output_event(event.time, 12, next_client + " " + std::to_string(table_num));
        }
    }
    
    clients.erase(client);
}

void ComputerClub::handle_remaining_clients_leave() {
    std::vector<std::string> remaining_clients;
    for (const auto& [client, table] : clients) { // c++11 range based for
        remaining_clients.push_back(client);
    }
    std::sort(remaining_clients.begin(), remaining_clients.end());
    
    for (const std::string& client : remaining_clients) {
        if (clients[client] != 0) {
            int table_num = clients[client];
            int duration;
            if (close_time.min > tables[table_num].start_time.min) {
                duration = close_time.min - tables[table_num].start_time.min;
            }
            else {
                duration = close_time.min + (24 * 60 - tables[table_num].start_time.min);
            }
            int hours = (duration + 59) / 60;
            tables[table_num].revenue += hours * price;
            tables[table_num].occupied_time.min += duration;
            tables[table_num].is_occupied = false;
        }
        add_output_event(close_time, 11, client);
    }
    clients.clear();
}

ComputerClub::ComputerClub(int n, Time open, Time close, int price)
    : tables_num(n), open_time(open), close_time(close), price(price)
{
    tables.resize(n + 1); // from 1 to n
}

void ComputerClub::process_events(const std::vector<Event> &events) {
    output.push_back(open_time.to_string());

    for (const Event& event : events) {
        if (event.id >= 1 && event.id <= 4) {
            std::string event_str = event.time.to_string() + " " + std::to_string(event.id);
            for (const std::string& arg : event.args) {
                event_str += " " + arg;
            }
            output.push_back(event_str);
        }
        
        handle_event(event);
    }

    handle_remaining_clients_leave();
    output.push_back(close_time.to_string());
}

void ComputerClub::generate_report() {
    for (const std::string& line : output)
        std::cout << line << std::endl;

    for (int i = 1; i <= tables_num; i++) {
        int total_time = tables[i].occupied_time.min;
        int h = total_time / 60;
        int m = total_time % 60;
        
        std::cout << i << " " << tables[i].revenue << " "
                  << std::setw(2) << std::setfill('0') << h << ":"
                  << std::setw(2) << std::setfill('0') << m << std::endl;
    }
}