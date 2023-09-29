#include <iostream>
#include <fstream>
#include <string>
#include "batter_database.h"

using namespace std;


batter_database::batter_database(ifstream &file) 
{ 
        num_players = 0;
        string first;
        file >> first;
        while (true) {
                batter hitter = initialize_batter(file, first);
                file >> first;
                if (first == "(D)") {
                        hitter.set_injured();
                        file >> first;
                } else if (first == "(I)") {
                        file >> first;
                }
                data.insert({ hitter.get_name(), hitter });
                num_players++;
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

static bool compare_at_bats(batter &a, batter &b)
{
        return a.get_at_bats() < b.get_at_bats();
}



void batter_database::remove_player(string player) 
{
        if (data.erase(player) == 0) {
                cout << "Could not find: " + player << endl;
        } else {
                num_players--;
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
        if (player.get_name() == "No player with this name") {
                cout << name << " is not available" << endl;
                data.erase(name);
        } else {
                player.print_stats();
        }
}

// sorting comparator
struct at_bat_cmp {
        bool operator()(pair<string, batter> &a, pair<string, batter> &b) const {
                return a.second.get_at_bats() > b.second.get_at_bats();
        }
};

void batter_database::print_at_bats() 
{
        map <string, batter, at_bat_cmp> at_bat_data(data.begin(), data.end());
}

void batter_database::print_runs() {}
void batter_database::print_homeruns() {}
void batter_database::print_rbis() {}
void batter_database::print_stolen_bases() {}
void batter_database::print_averages() {}
void batter_database::print_values() {}

void batter_database::change_value(int new_val) {}