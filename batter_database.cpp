/**************************************************************
 *                     batter_database.cpp
 *     Author: Alex Shriver
 *   
 *     Definition of the class declared in batter_database.h. Utilizes
 *     the batter class to create a database of batters. Below are the 
 *     functions available to the client
 *     
 **************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "batter_database.h"

using namespace std;

/**********batter_database********
 * Empty constructor
 ************************/
batter_database::batter_database() {}

/**********batter_database********
 * Initializes a batter_database given an argued file formatted like the 
 * batters.txt file in this directory
 * Inputs:
 *      ifstream &file: the file containing batters' names, teams, 
 *                      positions, and projected stats
 * Return: n/a
 * Expects: a valid file
 ************************/
batter_database::batter_database(ifstream &file) 
{ 
        // TODO: throw file error exceptions
        string first;
        file >> first;
        while (true) {
                batter hitter = initialize_batter(file, first);
                // reads the first string in case it entails injury or minors
                file >> first;
                if (first == "(D)") { 
                        hitter.set_injured();
                        file >> first;
                } else if (first == "(I)") {
                        hitter.set_minors(true);
                        file >> first;
                }
                data.insert({ hitter.get_name(), hitter });
                if (file.eof()) {
                        break; 
                }
        }
}

// TODO: Think about writing a different constructor for Batter with a 
// stringstream or string input
/**********initialize_batter********
 * Instantiates a batter based on the first line read from a file
 * Inputs:
 *      ifstream &file: the file containing batters' names, teams, 
 *                      positions, and projected stats
 *      string   first: the batter's first name
 * Return: A Batter instance instantiated with their stats
 * Expects: a valid file
 ************************/
batter batter_database::initialize_batter(ifstream &file, string first) 
{
        string last, name, elig;
        int at_bats, runs, homeruns, rbi, stolen_bases;
        float value, average;
        file >> last;
        name = first + " " + last;

        // gets arbitrary number of positions
        file >> elig;
        string elig_final = "";
        bool stopper = true;
        while (stopper) {
                file >> elig;

                // shows this is a string, meaning we have another position
                if (elig[elig.size() - 1] < '0' or elig[elig.size() - 1] > '9')
                {
                        elig_final += (" " + elig);
                // otherwise, we have reached the at_bats field
                } else {
                        stopper = false;
                        at_bats = stoi(elig);
                }
        }
        file >> runs >> homeruns >> rbi >> stolen_bases >> average;
        // ignore next 2 values (irrelevant information)
        file >> value >> value >> value;

        if (value <= 1) {
                value = 1;
        }

        return batter(name, elig_final, at_bats, runs, homeruns, rbi, 
                      stolen_bases, average, value);
}

/**********get_player********
 * Returns the player argued by name
 * Inputs:
 *      string name: the desired batter's name
 * Return: A Batter instance related to the string name
 ************************/
batter batter_database::get_player(string name) 
{ 
        // TODO: think about rewriting this to check data.count(name)?
        batter player = data[name];
        if (not player.is_player()) {
                cerr << name << " is not available" << endl;
                data.erase(name);
        }
        return player;
}

/**********add_player********
 * Adds an argued batter to the database
 * Inputs:
 *      batter player: the player to add
 * Return: n/a
 * Expects: a valid batter
 ************************/
void batter_database::add_player(batter player)
{
        // TODO: think of checking is_player?
        data.insert({ player.get_name(), player });
}

/**********remove_player********
 * Removes a player from the database
 * Inputs:
 *      string player: the name of the player to remove
 * Return: n/a
 * Expects: a batter already in the database
 ************************/
void batter_database::remove_player(string player) 
{
        if (data.erase(player) == 0) {
                cerr << "Could not find: " + player << endl;
        } 
}

/**********print_map********
 * Prints all the players in the database
 * Inputs:  n/a
 * Return:  n/a
 * Expects: n/a
 ************************/
void batter_database::print_map() 
{
        for (auto it = data.begin(); it != data.end(); it++) {
                it->second.print_stats();
        }
}

/**********print_player********
 * Prints one specific player's stats
 * Inputs:
 *      string player: the name of the player to print
 * Return: n/a
 * Expects: a valid player
 ************************/
void batter_database::print_player(string name) 
{ 
        // TODO: think about rewriting this to check data.count(name)?
        batter player = data[name];
        if (not player.is_player()) {
                cout << name << " is not available" << endl;
                data.erase(name);
        } else {
                player.print_stats();
        }
}

// TODO: Think of a more efficient solution to sorting the data

/**********listify********
 * Returns the hash map of players as a list
 * Inputs:  n/a
 * Return:  n/a
 * Expects: n/a
 ************************/
list <batter> batter_database::listify()
{
        list <batter> datalist;
        for (auto it = data.begin(); it != data.end(); it++) {
                datalist.push_back(it->second);
        }
        return datalist;
}

// A comparator function for the sorting algorithm based on atbats
static bool compare_at_bats(batter &a, batter &b)
{
        return a.get_at_bats() > b.get_at_bats();
}

// prints the database based on at_bats
void batter_database::print_at_bats() 
{
        list <batter> sorted_ab = listify();
        sorted_ab.sort(compare_at_bats);
        for (auto it = sorted_ab.begin(); it != sorted_ab.end(); it++) {
                it->print_stats();
        }
}

/**********print_pos********
 * prints a list of the players in the database who play an argued position
 * Inputs:
 *      string pos: The desired position
 * Return: n/a
 * Expects: a valid position
 ************************/
void batter_database::print_pos(string pos) 
{
        list <batter> sorted_ab = listify();
        sorted_ab.sort(compare_at_bats); // all players sorted by atbats
        for (auto it = sorted_ab.begin(); it != sorted_ab.end(); it++) {

                stringstream ss(it->get_eligibility());
                string player_pos;
                // check if each of their positions is the desired one
                while (ss >> player_pos) {
                        // MI includes SS and 2B
                        if (pos == "MI") {
                                if (player_pos == "SS" or player_pos == "2B") {
                                        it->print_stats();
                                }
                        // CI includes 1B and 3B
                        } else if (pos == "CI") {
                                if (player_pos == "1B" or player_pos == "3B") {
                                        it->print_stats();
                                }
                        // if not 1B or 3B, just check the position normally
                        } else {
                                if (player_pos == pos) {
                                        it->print_stats();
                                }
                        }       
                }
        }
}

// comparison function for runs
static bool compare_runs(batter &a, batter &b)
{
        return a.get_runs() > b.get_runs();
}

// prints the database ordered by their runs
void batter_database::print_runs() 
{
        list <batter> sorted_runs = listify();
        sorted_runs.sort(compare_runs);
        for (auto it = sorted_runs.begin(); it != sorted_runs.end(); it++) {
                it->print_stats();
        }
}

// comparison function for homeruns
static bool compare_homeruns(batter &a, batter &b)
{
        return a.get_homeruns() > b.get_homeruns();
}

// prints the database ordered by homeruns
void batter_database::print_homeruns() 
{
        list <batter> sorted_homeruns = listify();
        sorted_homeruns.sort(compare_homeruns);
        for (auto it = sorted_homeruns.begin(); it != sorted_homeruns.end(); it++) {
                it->print_stats();
        }
}

// comparison function for rbis
static bool compare_rbis(batter &a, batter &b)
{
        return a.get_rbi() > b.get_rbi();
}

// prints the database ordered by rbis
void batter_database::print_rbis() 
{
        list <batter> sorted_rbi = listify();
        sorted_rbi.sort(compare_rbis);
        for (auto it = sorted_rbi.begin(); it != sorted_rbi.end(); it++) {
                it->print_stats();
        }
}

// comparison function for stolen bases
static bool compare_sb(batter &a, batter &b)
{
        return a.get_stolen_bases() > b.get_stolen_bases();
}

// prints the database ordered by stolen bases
void batter_database::print_stolen_bases() 
{
        list <batter> sorted_sb = listify();
        sorted_sb.sort(compare_sb);
        for (auto it = sorted_sb.begin(); it != sorted_sb.end(); it++) {
                it->print_stats();
        }
}

// comparison function for averages
static bool compare_averages(batter &a, batter &b)
{
        return a.get_average() > b.get_average();
}

// prints the database ordered by averages
void batter_database::print_averages() 
{
        list <batter> sorted_avg = listify();
        sorted_avg.sort(compare_averages);
        for (auto it = sorted_avg.begin(); it != sorted_avg.end(); it++) {
                it->print_stats();
        }
}

// comparison function for estimated monetary value 
static bool compare_values(batter &a, batter &b)
{
        return a.get_value() > b.get_value();
}

// prints the database ordered by player estimated values. 
void batter_database::print_values() 
{
        list <batter> sorted_values = listify();
        sorted_values.sort(compare_values);
        for (auto it = sorted_values.begin(); it != sorted_values.end(); it++) {
                it->print_stats();
        }
}
