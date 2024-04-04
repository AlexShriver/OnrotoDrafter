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

// comparison function for runs
static bool compare_runs(batter &a, batter &b)
{
        return a.get_runs() > b.get_runs();
}

// comparison function for homeruns
static bool compare_homeruns(batter &a, batter &b)
{
        return a.get_homeruns() > b.get_homeruns();
}

// comparison function for rbis
static bool compare_rbis(batter &a, batter &b)
{
        return a.get_rbi() > b.get_rbi();
}

// comparison function for stolen bases
static bool compare_sb(batter &a, batter &b)
{
        return a.get_stolen_bases() > b.get_stolen_bases();
}

// comparison function for averages
static bool compare_averages(batter &a, batter &b)
{
        return a.get_average() > b.get_average();
}

// comparison function for estimated monetary value 
static bool compare_values(batter &a, batter &b)
{
        return a.get_value() > b.get_value();
}

/**********print_by_stat********
 * prints all batters in the argued list sorted by the argued stat
 * Inputs:
 *      list <batter> lst: the players to print
 *      string stat: The desired stat
 * Return: n/a
 * Expects: a valid stat and a nonnull list
 ************************/
void batter_database::print_by_stat(list <batter> lst, string stat) {
        if (stat == "ab") {
                lst.sort(compare_at_bats);
                print_players(lst);
        } else if (stat == "r") {
                lst.sort(compare_runs);
                print_players(lst);
        } else if (stat == "hr") {
                lst.sort(compare_homeruns);
                print_players(lst);
        } else if (stat == "rbi") {
                lst.sort(compare_rbis);
                print_players(lst);
        } else if (stat == "sb") {
                lst.sort(compare_sb);
                print_players(lst);
        } else if (stat == "avg") {
                lst.sort(compare_averages);
                print_players(lst);
        } else if (stat == "$") {
                lst.sort(compare_values);
                print_players(lst);
        } else {
                cout << "unrecognized: " << stat << endl;
        }
        cout << endl << endl;
}

/**********print_database********
 * prints all batters in the database sorted by a stat
 * Inputs:
 *      string stat: The desired stat
 * Return: n/a
 * Expects: a valid stat
 ************************/
void batter_database::print_database(string stat) 
{
        print_by_stat(listify(), stat);
        
}

/**********print_pos********
 * prints a list of the players in the database who play an argued position sorted
 *      by an argued stat
 * Inputs:
 *      string pos: The desired position
 *      string stat: The desired stat
 * Return: n/a
 * Expects: a valid position and stat
 ************************/
void batter_database::print_pos(string pos, string stat) 
{
        list <batter> lst;
        for (auto it = data.begin(); it != data.end(); it++) {
                stringstream ss(it->second.get_eligibility());
                string player_pos;
                // check if each of their positions is the desired one
                while (ss >> player_pos) {
                        // MI includes SS and 2B
                        if (pos == "MI") {
                                if (player_pos == "SS" or player_pos == "2B") {
                                        lst.push_back(it->second);
                                }
                        // CI includes 1B and 3B
                        } else if (pos == "CI") {
                                if (player_pos == "1B" or player_pos == "3B") {
                                        lst.push_back(it->second);
                                }
                        // if not 1B or 3B, just check the position normally
                        } else {
                                if (player_pos == pos) {
                                        lst.push_back(it->second);
                                }
                        }       
                }
        }
        print_by_stat(lst, stat);
}

/**********print_players********
 * prints all batters in the argued list of players
 * Inputs:
 *      list <batter> lst: the list of batters to print
 * Return: n/a
 * Expects: a nonnull list
 ************************/
void batter_database::print_players(list <batter> lst)
{
        for (auto it = lst.begin(); it != lst.end(); it++) {
                it->print_stats();
        }
}

/**********check_similar_players********
 * Checks for players who have similar names to the user's input. Does so by 
 *      comparing the first 3 letters of the input first and last names against
 *      the database
 * Inputs:
 *      string first: the user's inputted first name
 *      string last:  the user's inputted last name
 * Return: n/a 
 ************************/
void batter_database::check_similar_players(string first, string last) 
{
        // get the reference lengths and substrings
        int first_len = 3;
        if (first_len > first.size()) { first_len = first.size(); }

        int last_len = 3;
        if (last_len > last.size()) { last_len = last.size(); }

        first = first.substr(0, first_len);
        last = last.substr(0, last_len);

        cout << endl << "Did you mean: " << endl;
        // loop through the list and suggest names
        string r_name, r_first, r_last;
        int split;
        for (auto it = data.begin(); it != data.end(); it++) {
                r_name = it->first;
                
                // get the reference player's first and last name
                split = 0;
                for (int i = 0; (i < r_name.size() and split == 0); i++) {
                        if (r_name[i] == ' ') { split = i; }
                }
                r_first = r_name.substr(0, first_len); 
                r_last = r_name.substr(split + 1, last_len);

                // ensure enough letters to find a match
                if (split >= first_len and (r_name.size() - (split + 1) >= last_len)) {
                        if (r_first == first or r_last == last) {
                                cout << r_name << endl;
                        }
                }
        }
        cout << endl;
}
