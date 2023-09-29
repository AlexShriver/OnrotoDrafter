#include <iostream>
#include <iomanip>
#include "pitcher.h"

using namespace std;

pitcher::pitcher(string i_name, int i_innings, int i_wins, float i_era, 
                 float i_whip, int i_strikeouts, float i_value, bool i_injured) 
{
        name = i_name;
        innings = i_innings;
        wins = i_wins;
        era = i_era;
        whip = i_whip;
        strikeouts = i_strikeouts;
        value = i_value;
        injured = i_injured;
        starter = true;
        HOSV = 0;
        player_here = true;
        minors = false;
}

pitcher::pitcher() 
{
        player_here = false;
}

void pitcher::print_stats() 
{
        if (not player_here) {
                cout << "No player" << endl;
                return;
        }
        cout << setprecision(3) << fixed;
        cout << left << setw(25) << setfill(' ') << name;
        cout << left << setw(6)  << setfill(' ') << innings;
        cout << left << setw(6)  << setfill(' ') << wins;
        cout << left << setw(9)  << setfill(' ') << era;
        cout << left << setw(9)  << setfill(' ') << whip;
        cout << left << setw(6)  << setfill(' ') << strikeouts;
        if (not starter) {
                cout << left << setw(6)  << setfill(' ') << HOSV;
        }
        cout << left << setw(8)  << setfill(' ') << ("$$ " + to_string(value));
        if (injured) {
                cout << "IL";
        }
        if (minors) {
                cout << "MINORS";
        }
        cout << endl;
}

bool pitcher::get_minors() { return minors; }

bool pitcher::is_player() { return player_here; }

string pitcher::get_name() { return name; }

bool pitcher::get_starter() { return starter; }
 
int pitcher::get_innings() { return innings; }

int pitcher::get_wins() { return wins; }

float pitcher::get_era() { return era; }

float pitcher::get_whip() { return whip; }

int pitcher::get_strikeouts() { return strikeouts; }

int pitcher::get_HOSV() { return HOSV; }

int pitcher::get_value() { return value; }

void pitcher::set_injured() { injured = true; }

void pitcher::set_starter(bool start) { starter = start; }

void pitcher::set_HOSV(int i_HOSV) { HOSV = i_HOSV; }

void pitcher::set_value(int new_val) { value = new_val; }

void pitcher::set_minors(bool league) { minors = league; }