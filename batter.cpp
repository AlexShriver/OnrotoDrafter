/**************************************************************
 *                     batter.cpp
 *     Author: Alex Shriver
 *   
 *     Definition of the class declared in batter.h. Defines the
 *     methods for manipulating the class, allowing clients to
 *     instantiate new batters and access their information.
 *     
 **************************************************************/

#include <iostream>
#include <iomanip>
#include "batter.h"

using namespace std;

/**********batter********
 * Initializes a batter based on their stats as argued
 * Inputs:
 *       each of the players stat projections, their name, position eligibility
 *       and projected monetary value
 * Return: n/a
 * Expects: n/a
 ************************/
batter::batter(string i_name, string i_eligibility, int i_at_bats, 
               int i_runs, int i_homeruns, int i_rbi, int i_stolen_bases, 
               float i_average, int i_value)
{
        name = i_name;
        eligibility = i_eligibility;
        at_bats = i_at_bats;
        runs = i_runs;
        homeruns = i_homeruns;
        rbi = i_rbi;
        stolen_bases = i_stolen_bases;
        average = i_average;
        value = i_value;

        // Initialized this way, changed by other class functions
        injured = false;
        player_here = true;
        minors = false;
}

/**********batter*******
 * Empty constructor used to make a placeholder player
 ************************/
batter::batter() 
{
        player_here = false;
}

/**********print_stats********
 * prints the batter's stats
 * Inputs:
 *       n/a
 * Return: n/a
 * Expects: the player to exist
 ************************/
void batter::print_stats() 
{
        if (not player_here) {
                cerr << "No player" << endl;
                return;
        }
        cout << setprecision(3) << fixed;
        cout << left << setw(25) << setfill(' ') << name;
        cout << left << setw(15) << setfill(' ') << eligibility;
        cout << left << setw(6)  << setfill(' ') << at_bats;
        cout << left << setw(6) << setfill(' ') << runs;
        cout << left << setw(6) << setfill(' ') << homeruns;
        cout << left << setw(6) << setfill(' ') << rbi;
        cout << left << setw(6) << setfill(' ') << stolen_bases;
        cout << left << setw(9) << setfill(' ') << average;
        cout << left << setw(8) << setfill(' ') << ("$$ " + to_string(value));
        if (injured) {
                cout << "IL";
        }
        if (minors) {
                cout << "MINORS";
        }
        cout << endl;
}

/*******************************************************************************
 *
 *          Public member variable access functions for client
 * 
 ******************************************************************************/
bool batter::get_minors() { return minors; }

string batter::get_name() { return name; }

string batter::get_eligibility() { return eligibility; }

int batter::get_at_bats()  { return at_bats; }

int batter::get_runs() { return runs; }

int batter::get_homeruns() { return homeruns; }

int batter::get_rbi() { return rbi; }

int batter::get_stolen_bases() { return stolen_bases; }

float batter::get_average() { return average; }

int batter::get_value() { return value; }

bool batter::is_player() { return player_here; }

/*******************************************************************************
 *
 *          Sets the fields initialized that are manipulatable and/or 
 *          consistently by the constructor
 * 
 ******************************************************************************/
void batter::set_minors(bool league) { minors = league; }

void batter::set_value(int new_val) { value = new_val; }

void batter::set_injured() { injured = true; }