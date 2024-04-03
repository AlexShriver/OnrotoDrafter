/**************************************************************
 *                     pitcher_database.cpp
 *     Author: Alex Shriver
 *   
 *     Definition of the class declared in pitcher_database.h. Utilizes
 *     the pitcher class to create a database of pitchers. Below are the 
 *     functions available to the client
 *     
 **************************************************************/
#include <iostream>
#include "pitcher_database.h"
using namespace std;

/**********pitcher_database********
 * Empty constructor
 ************************/
pitcher_database::pitcher_database() {}

/**********pitcher_database********
 * Initializes a pitcher_database given an argued file formatted like the 
 * pitchers.txt file in this directory
 * Inputs:
 *      ifstream &file: the file containing pitchers' names, teams, 
 *                      positions, and projected stats
 * Return: n/a
 * Expects: a valid file
 ************************/
pitcher_database::pitcher_database(ifstream &file) 
{
        // TODO: throw file error exceptions
        string first;
        file >> first;
        while (true) {
                pitcher hurler = initialize_pitcher(file, first);
                // reads the first string in case it entails injury or minors
                file >> first;
                if (first == "(D)") {
                        hurler.set_injured();
                        file >> first;
                } else if (first == "(I)") {
                        hurler.set_minors(true);
                        file >> first;
                }
                data.insert({ hurler.get_name(), hurler });
                if (file.eof()) {
                        break; 
                }
        }
}

// TODO: Think about writing a different constructor for Pitcher with a 
// stringstream or string input
/**********initialize_pitcher********
 * Instantiates a pitcher based on the first line read from a file
 * Inputs:
 *      ifstream &file: the file containing batters' names, teams, 
 *                      positions, and projected stats
 *      string   first: the pitcher's first name
 * Return: A Pitcher instance instantiated with their stats
 * Expects: a valid file
 ************************/
pitcher pitcher_database::initialize_pitcher(ifstream &file, string first)
{
        // TODO: throw file error exceptions
        string last, name, dummy;
        int innings_pitched, wins, strikeouts;
        float value, era, whip, HOSV;
        file >> last;
        name = first + " " + last;

        file >> dummy;  // reads team. Could some day track teams too
        file >> innings_pitched >> wins >> era >> whip >> strikeouts 
             >> HOSV >> value >> value >> value;

        if (value <= 1) {
                value = 1;
        }

        return pitcher(name, innings_pitched, wins, era, whip, strikeouts, HOSV, 
                       value, HOSV == 0, false);
}

/**********add_player********
 * Adds an argued pitcher to the database
 * Inputs:
 *      pitcher player: the player to add
 * Return: n/a
 * Expects: a valid pitcher
 ************************/
void pitcher_database::add_player(pitcher player) 
{
        data.insert({ player.get_name(), player });
}

/**********remove_player********
 * Removes a player from the database
 * Inputs:
 *      string player: the name of the player to remove
 * Return: n/a
 * Expects: a pitcher already in the database
 ************************/
void pitcher_database::remove_player(string player) 
{
        if (data.erase(player) == 0) {
                cerr << "Could not find: " + player << endl;
        } 
}

/**********get_player********
 * Returns the player argued by name
 * Inputs:
 *      string name: the desired pitcher's name
 * Return: A pitcher instance related to the string name
 ************************/
pitcher pitcher_database::get_player(string name) 
{ 
        pitcher player = data[name];
        if (not player.is_player()) {
                cout << name << " is not available" << endl;
                data.erase(name);
        }
        return player;
}

/**********print_player********
 * Prints one specific player's stats
 * Inputs:
 *      string player: the name of the player to print
 * Return: n/a
 * Expects: a valid player
 ************************/
void pitcher_database::print_player(string name) 
{
        // TODO: think about rewriting this to check data.count(name)?
        pitcher player = data[name];
        if (not player.is_player()) {
                cout << name << " is not available" << endl;
                data.erase(name);
        } else {
                player.print_stats();
        }
}

/**********print_map********
 * Prints all the players in the database
 * Inputs:  n/a
 * Return:  n/a
 * Expects: n/a
 ************************/
void pitcher_database::print_map() 
{
        for (auto it = data.begin(); it != data.end(); it++) {
                it->second.print_stats();
        }
}

// TODO: Think of a more efficient solution to sorting the data

/**********listify********
 * Returns the hash map of players as a list
 * Inputs:  n/a
 * Return:  n/a
 * Expects: n/a
 ************************/
list <pitcher> pitcher_database::listify()
{
        list <pitcher> datalist;
        for (auto it = data.begin(); it != data.end(); it++) {
                datalist.push_back(it->second);
        }
        return datalist;
}

// A comparison function for innings pitched
static bool compare_ip(pitcher &a, pitcher &b)
{
        return a.get_innings() > b.get_innings();
}

// prints the database ordered by innings pitched
void pitcher_database::print_innings() 
{
        list <pitcher> sorted_ip = listify();
        sorted_ip.sort(compare_ip);
        for (auto it = sorted_ip.begin(); it != sorted_ip.end(); it++) {
                it->print_stats();
        }
}

// comparison function for wins
static bool compare_wins(pitcher &a, pitcher &b)
{
        return a.get_wins() > b.get_wins();
}

// prints the database ordered by wins
void pitcher_database::print_wins() 
{
        list <pitcher> sorted_wins = listify();
        sorted_wins.sort(compare_wins);
        for (auto it = sorted_wins.begin(); it != sorted_wins.end(); it++) {
                it->print_stats();
        }
}

// comparison function for era
static bool compare_era(pitcher &a, pitcher &b)
{
        return a.get_era() < b.get_era();
}

// prints the database ordered by era
void pitcher_database::print_eras() 
{
        list <pitcher> sorted_era = listify();
        sorted_era.sort(compare_era);
        for (auto it = sorted_era.begin(); it != sorted_era.end(); it++) {
                it->print_stats();
        }
}

// comparison function for whip
static bool compare_whip(pitcher &a, pitcher &b)
{
        return a.get_whip() < b.get_whip();
}

// prints the databse ordered by whip
void pitcher_database::print_whips() 
{
        list <pitcher> sorted_whip = listify();
        sorted_whip.sort(compare_whip);
        for (auto it = sorted_whip.begin(); it != sorted_whip.end(); it++) {
                it->print_stats();
        }
}

// comparison function for strikeouts
static bool compare_strikeouts(pitcher &a, pitcher &b)
{
        return a.get_strikeouts() > b.get_strikeouts();
}

// prints the database ordered by strikeouts
void pitcher_database::print_strikeouts() 
{
        list <pitcher> sorted_so = listify();
        sorted_so.sort(compare_strikeouts);
        for (auto it = sorted_so.begin(); it != sorted_so.end(); it++) {
                it->print_stats();
        }
}

// comparison function for values
static bool compare_values(pitcher &a, pitcher &b)
{
        return a.get_value() > b.get_value();
}

// prints the database ordered by values
void pitcher_database::print_values() 
{
        list <pitcher> sorted_values = listify();
        sorted_values.sort(compare_values);
        for (auto it = sorted_values.begin(); it != sorted_values.end(); it++) {
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
void pitcher_database::check_similar_players(string first, string last) 
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
