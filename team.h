/**************************************************************
 *                     team.h
 *     Author: Alex Shriver
 *   
 *     Declaration of the class defined in team.cpp. This is 
 *     a class to represent a team and the methods associated with 
 *     changing that team.
 * 
 *     Check further down the file for more information on the 
 *     data structures of the class
 *     
 **************************************************************/

#ifndef _TEAM_H_
#define _TEAM_H_

#include <iostream>
#include "pitcher.h"
#include "batter.h"
#include <list>
#include <vector>

// Constants for the team
const int STARTING_SALARY = 254;
const int NUM_HITTERS = 14;
const int NUM_PLAYERS = 40;
const int SALARY_PLAYERS = 25;

using namespace std;

// used to index the array of starting batters
typedef enum lineup {
        C1 = 0, C2, DH, FIRST, SECOND, SHORT, THIRD, CORNER, INNER, OF1, OF2, 
        OF3, OF4, OF5, BENCH, P
} position;

class team 
{
public:
        int players_drafted;
        int print_reports;

        team(ifstream &file);

        void print_hitters();
        void print_pitchers();
        
        void change_position(lineup og_pos, lineup new_pos);
        void add_new_pitcher(pitcher player);
        void add_new_batter(batter player, string position);

        void set_extra_pos(string pos);
        void make_batter_starter(string name, lineup new_pos);
        void make_pitcher_starter(string name);
        void bench_player(string name, bool hitter);
        void print_pitcher_stats(bool start);
        void print_batter_stats(bool start);
        void salary_summary();

private:
        bool check_extra_spot(lineup pos);
        void add_pitcher(pitcher player);
        void add_batter(batter player, lineup position);
        void print_hitter_categories();
        void print_pitcher_categories();
        void print_bench_hitters();
        void print_bench_pitchers();
        bool player_here(bool hitter, lineup position);
        int check_team_size();
        lineup convert_position(string pos);  
        string convert_lineup_to_string(int pos);   

        bool check_eligibility(batter player, lineup new_pos);

        batter initialize_batter(ifstream &file, string name);
        pitcher initialize_pitcher(ifstream &file, string first);

/*******************************************************************************
 *
 *          Private member variables
 * 
 ******************************************************************************/
        // This is an array representing the starting hitters, indexed by the 
        // lineup enum
        batter hitters[NUM_HITTERS];

        // A linked list of the benched batters (in no particular order)
        list <batter> bench_hitters;

        int num_pitchers;   // the number of pitchers (reliever or starter) that
                            // can be started
        
        // A linked list of the starting pitchers (in no particular order)
        list <pitcher> hurlers;

        // A linked list of the benched pitchers (in no particular order)
        list <pitcher> bench_hurlers;

        string extra_pos;      // the extra position not used (CI, P10, or OF5)
        int salary_left;
        int batter_salary;
        int pitcher_salary;

};

#endif