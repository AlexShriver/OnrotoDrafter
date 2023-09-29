#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "batter_database.h"

using namespace std;

batter_database::batter_database() {}

batter_database::batter_database(ifstream &file) 
{ 
        string first;
        file >> first;
        while (true) {
                batter hitter = initialize_batter(file, first);
                file >> first;
                if (first == "(D)") {
                        hitter.set_injured();
                        file >> first;
                } else if (first == "(I)") {
                        hitter.set_minors(true);
                        file >> first;
                }
                data.insert({ hitter.get_name(), hitter });
                if (file.eof()) {
                        break; 
                }
        }
}

batter batter_database::initialize_batter(ifstream &file, string first) 
{
        string last, name, elig;
        int at_bats, runs, homeruns, rbi, stolen_bases;
        float value, average;
        file >> last;
        name = first + " " + last;

        // gets arbitrary number of positions
        file >> elig;
        string elig_final = "";
        bool stopper = true;
        while (stopper) {
                file >> elig;
                if (elig[elig.size() - 1] < '0' or elig[elig.size() - 1] > '9')
                {
                        elig_final += (" " + elig);
                } else {
                        stopper = false;
                        at_bats = stoi(elig);
                }
        }
        file >> runs >> homeruns >> rbi >> stolen_bases >> average;
        // ignore next 2 values
        file >> value >> value >> value;

        if (value <= 1) {
                value = 1;
        }

        return batter(name, elig_final, at_bats, runs, homeruns, rbi, 
                      stolen_bases, average, value);
}

batter batter_database::get_player(string name) 
{ 
        batter player = data[name];
        if (not player.is_player()) {
                cout << name << " is not available" << endl;
                data.erase(name);
        }
        return player;
}

void batter_database::add_player(batter player)
{
        data.insert({ player.get_name(), player });
}

void batter_database::remove_player(string player) 
{
        if (data.erase(player) == 0) {
                cout << "Could not find: " + player << endl;
        } 
}

void batter_database::print_map() 
{
        for (auto it = data.begin(); it != data.end(); it++) {
                it->second.print_stats();
        }
}

void batter_database::print_player(string name) 
{ 
        batter player = data[name];
        if (not player.is_player()) {
                cout << name << " is not available" << endl;
                data.erase(name);
        } else {
                player.print_stats();
        }
}

list <batter> batter_database::listify()
{
        list <batter> datalist;
        for (auto it = data.begin(); it != data.end(); it++) {
                datalist.push_back(it->second);
        }
        return datalist;
}

static bool compare_at_bats(batter &a, batter &b)
{
        return a.get_at_bats() > b.get_at_bats();
}

void batter_database::print_at_bats() 
{
        list <batter> sorted_ab = listify();
        sorted_ab.sort(compare_at_bats);
        for (auto it = sorted_ab.begin(); it != sorted_ab.end(); it++) {
                it->print_stats();
        }
}

void batter_database::print_pos(string pos) 
{
        list <batter> sorted_ab = listify();
        sorted_ab.sort(compare_at_bats);
        for (auto it = sorted_ab.begin(); it != sorted_ab.end(); it++) {
                stringstream ss(it->get_eligibility());
                string player_pos;
                while (ss >> player_pos) {
                        if (pos == "MI") {
                                if (player_pos == "SS" or player_pos == "2B") {
                                        it->print_stats();
                                }
                        } else if (pos == "CI") {
                                if (player_pos == "1B" or player_pos == "3B") {
                                        it->print_stats();
                                }
                        } else {
                                if (player_pos == pos) {
                                        it->print_stats();
                                }
                        }       
                }
        }
}

static bool compare_runs(batter &a, batter &b)
{
        return a.get_runs() > b.get_runs();
}

void batter_database::print_runs() 
{
        list <batter> sorted_runs = listify();
        sorted_runs.sort(compare_runs);
        for (auto it = sorted_runs.begin(); it != sorted_runs.end(); it++) {
                it->print_stats();
        }
}

static bool compare_homeruns(batter &a, batter &b)
{
        return a.get_homeruns() > b.get_homeruns();
}

void batter_database::print_homeruns() 
{
        list <batter> sorted_homeruns = listify();
        sorted_homeruns.sort(compare_homeruns);
        for (auto it = sorted_homeruns.begin(); it != sorted_homeruns.end(); it++) {
                it->print_stats();
        }
}

static bool compare_rbis(batter &a, batter &b)
{
        return a.get_rbi() > b.get_rbi();
}

void batter_database::print_rbis() 
{
        list <batter> sorted_rbi = listify();
        sorted_rbi.sort(compare_rbis);
        for (auto it = sorted_rbi.begin(); it != sorted_rbi.end(); it++) {
                it->print_stats();
        }
}

static bool compare_sb(batter &a, batter &b)
{
        return a.get_stolen_bases() > b.get_stolen_bases();
}

void batter_database::print_stolen_bases() 
{
        list <batter> sorted_sb = listify();
        sorted_sb.sort(compare_sb);
        for (auto it = sorted_sb.begin(); it != sorted_sb.end(); it++) {
                it->print_stats();
        }
}

static bool compare_averages(batter &a, batter &b)
{
        return a.get_average() > b.get_average();
}

void batter_database::print_averages() 
{
        list <batter> sorted_avg = listify();
        sorted_avg.sort(compare_averages);
        for (auto it = sorted_avg.begin(); it != sorted_avg.end(); it++) {
                it->print_stats();
        }
}

static bool compare_values(batter &a, batter &b)
{
        return a.get_value() > b.get_value();
}

void batter_database::print_values() 
{
        list <batter> sorted_values = listify();
        sorted_values.sort(compare_values);
        for (auto it = sorted_values.begin(); it != sorted_values.end(); it++) {
                it->print_stats();
        }
}
