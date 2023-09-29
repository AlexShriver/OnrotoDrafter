#include "team.h"
#include "pitcher_database.h"
#include "batter_database.h"
#include <iostream>
#include <fstream>
#include <istream>

using namespace std;

void query_loop(batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team);

// void draft(batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team);

// void draft_file(ifstream &file, batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team);

void draft_loop(batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team);

void print(batter_database *avail_batters, pitcher_database *avail_pitchers);

void manip_team(team *my_team);

void print_hitter_categories();
void print_pitcher_categories();
// bool open_up(ifstream &stream, string file_name);