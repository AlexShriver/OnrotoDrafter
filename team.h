#ifndef _TEAM_H_
#define _TEAM_H_

#include <iostream>
#include "pitcher.h"
#include "batter.h"
#include <list>
#include <vector>

const int STARTING_SALARY = 254;
const int NUM_HITTERS = 14;
const int NUM_PLAYERS = 40;
const int SALARY_PLAYERS = 25;

using namespace std;

typedef enum lineup {
        C1 = 0, C2, DH, FIRST, SECOND, SHORT, THIRD, CORNER, INNER, OF1, OF2, 
        OF3, OF4, OF5, BENCH, P
} position;

class team 
{
public:
        team(ifstream &file);

        void print_hitters();
        void print_pitchers();
        // takes a starter and moves them or benches them
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
        lineup convert_position(string pos);  // doesn't work for P
        string convert_lineup_to_string(int pos);   // neither does this

        bool check_eligibility(batter player, lineup new_pos);

        batter initialize_batter(ifstream &file, string name);
        pitcher initialize_pitcher(ifstream &file, string first);

        // Member variables
        batter hitters[NUM_HITTERS];
        list <batter> bench_hitters;

        int num_pitchers;   // the number of pitchers (reliever or starter) that
                            // can be started
        list <pitcher> hurlers;
        list <pitcher> bench_hurlers;

        string extra_pos;
        int salary_left;
        int batter_salary;
        int pitcher_salary;

};

// still need to write make_starter(), compare against other stats

#endif