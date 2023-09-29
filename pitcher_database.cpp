#include <iostream>
#include "pitcher_database.h"
using namespace std;


pitcher_database::pitcher_database() {}


pitcher_database::pitcher_database(ifstream &file) 
{
        string first;
        file >> first;
        while (true) {
                pitcher hurler = initialize_pitcher(file, first);
                file >> first;
                if (first == "(D)") {
                        hurler.set_injured();
                        file >> first;
                } else if (first == "(I)") {
                        hurler.set_minors(true);
                        file >> first;
                }
                data.insert({ hurler.get_name(), hurler });
                if (file.eof()) {
                        break; 
                }
        }
}


pitcher pitcher_database::initialize_pitcher(ifstream &file, string first)
{
        string last, name, dummy;
        int innings_pitched, wins, strikeouts;
        float value, era, whip, HOSV;
        file >> last;
        name = first + " " + last;

        file >> dummy;
        file >> innings_pitched >> wins >> era >> whip >> strikeouts 
             >> HOSV >> value >> value >> value;

        if (value <= 1) {
                value = 1;
        }

        return pitcher(name, innings_pitched, wins, era, whip, strikeouts, 
                       value, false);
}


void pitcher_database::add_player(pitcher player) 
{
        data.insert({ player.get_name(), player });
}

void pitcher_database::remove_player(string player) 
{
        if (data.erase(player) == 0) {
                cout << "Could not find: " + player << endl;
        } 
}

pitcher pitcher_database::get_player(string name) 
{ 
        pitcher player = data[name];
        if (not player.is_player()) {
                cout << name << " is not available" << endl;
                data.erase(name);
        }
        return player;
}

void pitcher_database::print_player(string name) 
{
        pitcher player = data[name];
        if (not player.is_player()) {
                cout << name << " is not available" << endl;
                data.erase(name);
        } else {
                player.print_stats();
        }
}

void pitcher_database::print_map() 
{
        for (auto it = data.begin(); it != data.end(); it++) {
                it->second.print_stats();
        }
}

list <pitcher> pitcher_database::listify()
{
        list <pitcher> datalist;
        for (auto it = data.begin(); it != data.end(); it++) {
                datalist.push_back(it->second);
        }
        return datalist;
}

static bool compare_ip(pitcher &a, pitcher &b)
{
        return a.get_innings() > b.get_innings();
}

void pitcher_database::print_innings() 
{
        list <pitcher> sorted_ip = listify();
        sorted_ip.sort(compare_ip);
        for (auto it = sorted_ip.begin(); it != sorted_ip.end(); it++) {
                it->print_stats();
        }
}

static bool compare_wins(pitcher &a, pitcher &b)
{
        return a.get_wins() > b.get_wins();
}

void pitcher_database::print_wins() 
{
        list <pitcher> sorted_wins = listify();
        sorted_wins.sort(compare_wins);
        for (auto it = sorted_wins.begin(); it != sorted_wins.end(); it++) {
                it->print_stats();
        }
}

static bool compare_era(pitcher &a, pitcher &b)
{
        return a.get_era() < b.get_era();
}

void pitcher_database::print_eras() 
{
        list <pitcher> sorted_era = listify();
        sorted_era.sort(compare_era);
        for (auto it = sorted_era.begin(); it != sorted_era.end(); it++) {
                it->print_stats();
        }
}

static bool compare_whip(pitcher &a, pitcher &b)
{
        return a.get_whip() < b.get_whip();
}

void pitcher_database::print_whips() 
{
        list <pitcher> sorted_whip = listify();
        sorted_whip.sort(compare_whip);
        for (auto it = sorted_whip.begin(); it != sorted_whip.end(); it++) {
                it->print_stats();
        }
}

static bool compare_strikeouts(pitcher &a, pitcher &b)
{
        return a.get_strikeouts() > b.get_strikeouts();
}

void pitcher_database::print_strikeouts() 
{
        list <pitcher> sorted_so = listify();
        sorted_so.sort(compare_strikeouts);
        for (auto it = sorted_so.begin(); it != sorted_so.end(); it++) {
                it->print_stats();
        }
}

static bool compare_values(pitcher &a, pitcher &b)
{
        return a.get_value() > b.get_value();
}

void pitcher_database::print_values() 
{
        list <pitcher> sorted_values = listify();
        sorted_values.sort(compare_values);
        for (auto it = sorted_values.begin(); it != sorted_values.end(); it++) {
                it->print_stats();
        }
}