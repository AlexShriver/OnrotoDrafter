/**************************************************************
 *                     interact.h
 *     Author: Alex Shriver
 *   
 *     This file declares functions defined in interact.cpp. 
 *     These functions are used in the OnRoto_Drafter program to 
 *     allow user interaction with their team and the databases
 *     of available players. See interact.cpp for function contracts
 *     
 **************************************************************/

#include "team.h"
#include "pitcher_database.h"
#include "batter_database.h"
#include <iostream>
#include <fstream>
#include <istream>

using namespace std;

void open_or_die(ifstream &stream, string file_name);

void query_loop(batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team);

void draft_loop(batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team, ofstream &drafted_players);

void print(batter_database *avail_batters, pitcher_database *avail_pitchers);

void manip_team(team *my_team);

void auto_draft(batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team, ofstream &drafted_players);

void print_hitter_categories();
void print_pitcher_categories();