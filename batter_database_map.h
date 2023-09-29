#ifndef _BATTER_DATABASE_H_
#define _BATTER_DATABASE_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "batter.h"

using namespace std;

class batter_database 
{
public:
        batter_database(ifstream &file);

        void remove_player(string player);

        void print_map();
        void print_player(string name);
        void print_at_bats();
        void print_runs();
        void print_homeruns();
        void print_rbis();
        void print_stolen_bases();
        void print_averages();
        void print_values();

        void change_value(int new_val);


private:

        batter initialize_batter(ifstream &file, string first);

        map <string, batter> data;
        int num_players;

        batter *front_at_bats;
        batter *front_runs;
        batter *front_homeruns;
        batter *front_rbi;
        batter *front_stolen_bases;
        batter *front_average;
        batter *front_value;
        
};

#endif