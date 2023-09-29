#ifndef _BATTER_H_
#define _BATTER_H_

#include <iostream>
#include <vector>

using namespace std;

class batter 
{
public:
        batter(string i_name, string i_eligibility, int i_at_bats, 
               int i_runs, int i_homeruns, int i_rbi, int i_stolen_bases, 
               float i_average, int i_value);

        batter();
        
        void print_stats();

        bool get_minors();
        string get_name();
        string get_eligibility();
        int get_at_bats();
        int get_runs();
        int get_homeruns();
        int get_rbi();
        int get_stolen_bases();
        float get_average();
        int get_value();
        bool is_player();

        void set_minors(bool league); // true = minors
        void set_value(int new_val);
        void set_injured();

private:

        // Member variables
        string name;
        string eligibility;  // maybe just make this a string...
        // stats
        int at_bats;
        int runs;
        int homeruns;
        int rbi;
        int stolen_bases;
        float average;
        int value;
        bool injured;
        bool player_here;
        bool minors;
};

#endif