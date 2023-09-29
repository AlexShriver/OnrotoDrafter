#ifndef _BATTER_DATABASE_H_
#define _BATTER_DATABASE_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <list>
#include "batter.h"

using namespace std;

class batter_database 
{
public:
        batter_database(ifstream &file);
        batter_database();

        batter get_player(string player);
        void add_player(batter player);
        void remove_player(string player);

        void print_map();
        void print_pos(string pos);
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

        list <batter> listify();
        
        unordered_map <string, batter> data;
        
};

#endif