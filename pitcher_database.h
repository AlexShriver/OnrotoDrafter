#ifndef _PITCHER_DATABASE_H_
#define _PITCHER_DATABASE_H_

#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <unordered_map>
#include "pitcher.h"

using namespace std;

class pitcher_database 
{
public:
        pitcher_database();
        pitcher_database(ifstream &file);

        pitcher get_player(string player);
        void add_player(pitcher player);
        void remove_player(string player);

        void print_player(string name);
        void print_map();
        void print_innings();
        void print_wins();
        void print_eras();
        void print_whips();
        void print_strikeouts();
        void print_values();

private:
        pitcher initialize_pitcher(ifstream &file, string first);

        list <pitcher> listify();

        unordered_map <string, pitcher> data;
        
};

#endif