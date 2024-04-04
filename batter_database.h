/**************************************************************
 *                     batter_database.h
 *     Author: Alex Shriver
 *   
 *     Declarations for the batter_database class defined in 
 *     batter_database.cpp. Utilizes the batter class to create a database 
 *     of batters. Below are the functions available to the client
 *     
 **************************************************************/

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
        void print_player(string name);
        void print_pos(string pos, string stat);
        void print_database(string stat);

        void check_similar_players(string first, string last);

private:

        batter initialize_batter(ifstream &file, string first);

        void print_by_stat(list <batter> lst, string stat);
        void print_players(list <batter> lst);

        list <batter> listify();
        
        // hash map for batter data. Key: player name, Data: Batter instance
        unordered_map <string, batter> data;
        
};

#endif