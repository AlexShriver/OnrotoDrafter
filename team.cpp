/**************************************************************
 *                     team.cpp
 *     Author: Alex Shriver
 *   
 *     Definition of the class declared in team.h. This is 
 *     a class to represent a team and the methods associated with 
 *     changing that team.
 *  
 *     Note: This league supports 23 starters, 21 of which are defined.
 *           For the next two positions, the user can use two of the
 *           following three options: a tenth pitcher, a fifth outfielder
 *           or a corner indielder (1B or 3B).
 * 
 *     Note: See team.h for detailed explanation of data structures 
 * 
 *     Full disclosure, I wrote this file at 3 am, just hours before
 *     the start of the draft.
 *     
 **************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include "team.h"

using namespace std;

/**********team********
 * Initializes a team based on information from a specifically formatted file. 
 *      For this format, see the text file my_team.txt
 * Inputs:
 *       ifstream &file: The file holding the team
 * Return: n/a
 * Expects: file to be nonnull
 ************************/
team::team(ifstream &file) 
{       
        // TODO: assert file nonnull

        // Default initialization of these member variables
        extra_pos = "P";
        num_pitchers = 9;   // the number of pitchers (reliever or starter) that
                            // can be started
        salary_left = STARTING_SALARY;
        batter_salary = 0;
        pitcher_salary = 0;
        string first;
        file >> first;
        bool add_sal = true;

        position slot;  // the position for the player to fill on the team
        string name, last, pos;

        // while loop to initialize and store the hitters on the team
        while (true) {
                add_sal = true;

                // Done with hitters
                if (first == "Pitchers") {
                        break;
                }

                // initialize the hitter
                file >> last >> pos;
                name = first + " " + last;
                batter hitter = initialize_batter(file, name);
                
                file >> first;

                // handles injured players and players in the minors
                if (first == "(D)") {
                        hitter.set_injured();
                        file >> first;
                } else if (first == "(I)") {
                        // players in the minors don't hurt salary
                        add_sal = false;  
                        hitter.set_minors(true);
                        file >> first;
                } 
                
                add_batter(hitter, convert_position(pos));

                if (add_sal) {
                        batter_salary += hitter.get_value();
                }

                if (file.eof()) {
                        break; 
                }
        }

        file >> first;
        bool bench = false;
        // handles initializing and storing pitchers
        while (true) {
                bench = false;
                add_sal = true;

                // initialize the pitcher
                pitcher hurler = initialize_pitcher(file, first);
                file >> first;

                // handles pitchers who are injured or in the minors
                if (first == "(D)") {
                        hurler.set_injured();
                        file >> first;
                } else if (first == "(I)") {
                        // Automatically bench the pitcher and don't add salary
                        add_sal = false;
                        bench = true;
                        hurler.set_minors(true);
                        file >> first;
                } 

                add_pitcher(hurler);
                if (bench) {
                        bench_player(hurler.get_name(), false);
                }
                if (add_sal) {
                        pitcher_salary += hurler.get_value();
                }

                if (file.eof()) {
                        break; 
                }
        }

        salary_left -= (batter_salary + pitcher_salary);
}

// Converts a string pos to its equivalent lineup enum
lineup team::convert_position(string pos) 
{
        if (pos == "C1") {
                return C1;
        } else if (pos == "C2") {
                return C2;
        } else if (pos == "DH") {
                return DH;
        } else if (pos == "1B") {
                return FIRST;
        } else if (pos == "2B") {
                return SECOND;
        } else if (pos == "SS") {
                return SHORT;
        } else if (pos == "3B") {
                return THIRD;
        } else if (pos == "CI") {
                return CORNER;
        } else if (pos == "MI") {
                return INNER;
        } else if (pos == "OF1") {
                return OF1;
        } else if (pos == "OF2") {
                return OF2;
        } else if (pos == "OF3") {
                return OF3;
        } else if (pos == "OF4") {
                return OF4;
        } else if (pos == "OF5") {
                return OF5;
        } else  {
                return BENCH;
        }
}

/**********initialize_batter********
 * Initializes a batter based on the immediate line in a team file
 * Inputs:
 *       ifstream &file: The file holding the batter to be initialized
 *       string name:    The player's name
 * Return: the batter instance
 * Expects: file to be nonnull
 ************************/
batter team::initialize_batter(ifstream &file, string name) 
{
        // TODO: assert file nonnull
        string elig;
        int at_bats, runs, homeruns, rbi, stolen_bases;
        float value, average;

        // gets arbitrary number of positions
        file >> elig;
        string elig_final = elig;
        bool stopper = true;
        while (stopper) {
                file >> elig;
                if (elig[elig.size() - 1] < '0' or elig[elig.size() - 1] > '9')
                {
                        elig_final += (" " + elig);
                } else {
                        stopper = false;
                        at_bats = stoi(elig);
                }
        }
        file >> runs >> homeruns >> rbi >> stolen_bases >> average >> value;

        if (value <= 1) {
                value = 1;
        }

        return batter(name, elig_final, at_bats, runs, homeruns, rbi, 
                      stolen_bases, average, value);
}

/**********initialize_pitcher********
 * Initializes a pitcher based on the immediate line in a team file
 * Inputs:
 *       ifstream &file: The file holding the batter to be initialized
 *       string first:    The player's first name
 * Return: the pitcher instance
 * Expects: file to be nonnull
 ************************/
pitcher team::initialize_pitcher(ifstream &file, string first)
{
        // TODO: assert file nonnull
        string last, name, pos;
        int innings_pitched, wins, strikeouts, HOSV;
        float value, era, whip;

        file >> last >> pos >> innings_pitched >> wins >> era >> whip 
             >> strikeouts >> HOSV >> value;
        
        name = first + " " + last;

        if (value <= 1) {
                value = 1;
        }

        pitcher p = pitcher(name, innings_pitched, wins, era, whip, strikeouts, 
                            value, false);

        // says if pitcher is a starting pitcher or a reliever
        if (pos == "SP") {
                p.set_starter(true);
        } else {
                p.set_starter(false);
        }
        return p;
}

/**********player_here********
 * Checks if there is a player at the argued lineup position.
 *      If it is a pitcher, checks if there are open slots for starters (SP or RP)
 * Inputs:
 *       bool hitter:       True if looking for a hitter
 *       linup position:    The position of the hitter, P otherwise for pitcher
 * Return:  false if there is a spot open at the position and true if there is
 *          a player there already
 * Expects: n/a
 ************************/
bool team::player_here(bool hitter, lineup position)
{
        if (hitter) {
                return hitters[position].is_player();
        } else {
                return (hurlers.size() >= num_pitchers);
        }
}

// returns the number of players on the team
int team::check_team_size()
{
        int num_hitters = bench_hitters.size();
        for (int i = 0; i < NUM_HITTERS; i++) {
                if (hitters[i].is_player()) {
                        num_hitters++;
                }
        }
        return bench_hurlers.size() + hurlers.size() + num_hitters;
}

// converts and argued position, int or lineup enum, and returns the string conversion
string team::convert_lineup_to_string(int pos)
{
        if (pos == 0) {
                return "C1";
        } else if (pos == 1) {
                return "C2";
        } else if (pos == 2) {
                return "DH";
        } else if (pos == 3) {
                return "1B";
        } else if (pos == 4) {
                return "2B";
        } else if (pos == 5) {
                return "SS";
        } else if (pos == 6) {
                return "3B";
        } else if (pos == 7) {
                return "CI";
        } else if (pos == 8) {
                return "MI";
        } else if (pos == 9) {
                return "OF1";
        } else if (pos == 10) {
                return "OF2";
        } else if (pos == 11) {
                return "OF3";
        } else if (pos == 12) {
                return "OF4";
        } else if (pos == 13) {
                return "OF5";
        } else if (pos == 15) {
                return "P";
        } else {
                return "B";
        }
}

// prints the formatted stat line for the hitters in the team interface
void team::print_hitter_categories()
{
        cout << left << setw(8)  << setfill(' ') << "POS";
        cout << left << setw(25) << setfill(' ') << "NAME";
        cout << left << setw(15) << setfill(' ') << "ELIG";
        cout << left << setw(6)  << setfill(' ') << "ABs";
        cout << left << setw(6) << setfill(' ')  << "RUNS";
        cout << left << setw(6) << setfill(' ')  << "HRs";
        cout << left << setw(6) << setfill(' ')  << "RBI";
        cout << left << setw(6) << setfill(' ')  << "SBs";
        cout << left << setw(9) << setfill(' ')  << "AVG";
        cout << left << setw(8) << setfill(' ')  << "VAL";
        cout << endl
<< "--------------------------------------------------------------------------------------------\n";
}

/**********print_hitters********
 * Prints the list of hitters on the team, starters and bench
 * Inputs:  n/a
 * Return:  n/a
 * Expects: n/a
 ************************/
void team::print_hitters() 
{
        cout 
<< "*************************************************************************\n"
<< "*********                   STARTING HITTERS                    *********\n"
<< "*************************************************************************\n";

        print_hitter_categories();

        // prints the starting hitters and their stats
        for (int i = 0; i < NUM_HITTERS; i++) {
                // this if statement handles the extra position (see file header)
                if (not (((i == 7) and extra_pos == "CI") or 
                         (i == 13 and extra_pos == "OF5"))) 
                {
                        cout << convert_lineup_to_string(i);
                        if (i < 9) {
                                cout << "  ... ";
                        } else {
                                cout << " ... ";
                        }
                        hitters[i].print_stats();
                }
        }

cout << "--------------------------------------------------------------------------------------------\n";
        print_batter_stats(true);  // prints the total stat projections for the starting hitters
        cout << endl
<< "*************************************************************************\n"
<< "*********                    BENCH HITTERS                      *********\n"
<< "*************************************************************************\n";
        if (bench_hitters.size() != 0) {
                print_hitter_categories();
                print_bench_hitters();
                cout << "--------------------------------------------------------------------------------------------\n";
                print_batter_stats(false); // prints the total stat projections for the benched hitters
        }
}

// prints the statlines for each benched hitter
void team::print_bench_hitters()
{
        int count = 1;
        for (auto it = bench_hitters.begin(); it != bench_hitters.end(); it++) {
                cout << "B" << count << ": ... ";
                it->print_stats();
                count++;
        }
}

// prints the header stats to go above listed pitchers
void team::print_pitcher_categories()
{
        cout << left << setw(8)  << setfill(' ') << "POS";
        cout << left << setw(25) << setfill(' ') << "NAME";
        cout << left << setw(6)  << setfill(' ') << "IP";
        cout << left << setw(6)  << setfill(' ') << "W";
        cout << left << setw(9)  << setfill(' ') << "ERA";
        cout << left << setw(9)  << setfill(' ') << "WHIP";
        cout << left << setw(6)  << setfill(' ') << "SO";
        cout << left << setw(8) << setfill(' ')  << "VAL";
        cout << endl
<< "--------------------------------------------------------------------------\n";
}

/**********print_pitchers********
 * Prints the list of pitchers on the team, starters and bench
 * Inputs:  n/a
 * Return:  n/a
 * Expects: n/a
 ************************/
void team::print_pitchers() 
{
        cout 
<< "*************************************************************************\n"
<< "*********                   STARTED PITCHERS                    *********\n"
<< "*************************************************************************\n";

        print_pitcher_categories();

        // prints the starting pitchers' stats
        for (auto it = hurlers.begin(); it != hurlers.end(); it++) {
                if (it->get_starter()) {
                        cout << "SP .... ";
                } else {
                        cout << "RP .... ";
                }
                it->print_stats();
        }

        cout << "--------------------------------------------------------------------------\n";
        print_pitcher_stats(true);  // print the total stats of all starting pitchers
        cout << endl
<< "*************************************************************************\n"
<< "*********                    BENCH PITCHERS                     *********\n"
<< "*************************************************************************\n";

        if (bench_hurlers.size() != 0) {
                print_pitcher_categories();
                print_bench_pitchers();
                cout << "--------------------------------------------------------------------------\n";
                print_pitcher_stats(false);
        }
}

// prints all the bench players' stats
void team::print_bench_pitchers()
{
        for (auto it = bench_hurlers.begin(); it != bench_hurlers.end(); it++) {
                if (it->get_starter()) {
                        cout << "SP .... ";
                } else {
                        cout << "RP .... ";
                }
                it->print_stats();
        }
}

/**********check_extra_spot********
 * checks if the extra spot is the argued position and prints an appropriate 
 *      response, thus meaning the position cannot be used
 * Inputs:  
 *      lineup pos: the position to check
 * Return:  true if the position is the same as the argued option,
 *       false if it is different or if the position is P
 * Expects: n/a
 ************************/
bool team::check_extra_spot(lineup pos)
{
        if (pos == CORNER and extra_pos == "CI") {
                cout << "Must change extra position" << endl;
                return true;
        } else if (pos == OF5 and extra_pos == "OF5") {
                cout << "Must change extra position" << endl;
                return true;
        } 
        return false;
}

/**********change_position********
 * Swtiches a batter's position based on their original position and their new
 *      position
 * Inputs:  
 *      lineup og_pos: the current position of the batter
 *      lineup new_pos: the new position for the batter
 * Return: n/a
 * Expects: two batter lineup positions for arguments
 ************************/
void team::change_position(lineup og_pos, lineup new_pos) 
{
        // check if the extra spot must be switched
        if (check_extra_spot(new_pos)) { return; }

        batter player = hitters[og_pos];
        if (not player.is_player()) {
                cout << "No player at original position" << endl;
                return;
        }
        // give original position an empty batter
        hitters[og_pos] = batter();

        add_batter(player, new_pos);
}

/**********make_batter_starter********
 * Moves an argued batter from the bench to a starting position
 * Inputs:  
 *      string name: the name of the player to be started
 *      lineup new_pos: the new position for the player
 * Return:  n/a
 * Expects: a player on the bench and a valid new position
 ************************/
void team::make_batter_starter(string name, lineup new_pos)
{
        // check if the extra spot must be changed
        if (check_extra_spot(new_pos)) { return; }

        batter player;
        for (auto it = bench_hitters.begin(); it != bench_hitters.end(); it++) {
                if (it->get_name() == name) {
                        player = *it;
                        // remove player from bench
                        it = bench_hitters.erase(it); 
                }  
        }

        // couldn't find batter on bench
        if (not player.is_player()) {
                cout << "Could not find " << name << " on your bench" << endl;
                cout << "here is your bench: " << endl;
                print_bench_hitters();
                return;
        }

        // start the batter
        add_batter(player, new_pos);
}

/**********make_pitcher_starter********
 * Moves an argued pitcher from the bench to a starting position
 * Inputs:  
 *      string name: the name of the player to be started
 * Return:  n/a
 * Expects: a player on the bench
 ************************/
void team::make_pitcher_starter(string name) 
{
        // find the pitcher
        pitcher player;
        for (auto it = bench_hurlers.begin(); it != bench_hurlers.end(); it++) {
                if (it->get_name() == name) {
                        player = *it;
                        it = bench_hurlers.erase(it);
                }  
        }

        if (not player.is_player()) {
                cout << "Could not find " << name << " on your bench" << endl;
                cout << "here is your bench: " << endl;
                print_bench_pitchers();
                return;
        }

        // start the pitcher
        add_pitcher(player);
}

/**********bench_player********
 * Moves an argued player (pitcher or batter) from their starting spot
 *          to the bench
 * Inputs:  
 *      string name: the name of the player to be benched
 *      bool hitter: tells if the player is a pitcher or hitter
 * Return:  n/a
 * Expects: a player on the starting lineup
 ************************/
void team::bench_player(string name, bool hitter)
{
        if (hitter) {
                for (int i = 0; i < NUM_HITTERS; i++) {
                        if (hitters[i].get_name() == name) {
                                // bench and replace spot with empty batter
                                bench_hitters.push_back(hitters[i]);
                                hitters[i] = batter();  
                        }
                }
        } else {
                for (auto it = hurlers.begin(); it != hurlers.end(); it++) {
                        if (it->get_name() == name) {
                                // bench pitcher and remove from hurlers list
                                bench_hurlers.push_back(*it);
                                it = hurlers.erase(it);
                        }  
                }
        }
}

/**********check_eligibility********
 * checks if an argued batter can be played at an argued position
 * Inputs:  
 *      batter player: the hitter to check eligibility for
 *      lineup new_pos: their potential new position
 * Return:  true if they are eligible, false otherwise
 * Expects: a non-empty player
 ************************/
bool team::check_eligibility(batter player, lineup new_pos)
{
        // TODO: check if player is real
        stringstream ss(player.get_eligibility());
        string elig_pos, str_new_pos;
        bool elig = false;
        str_new_pos = convert_lineup_to_string(new_pos);

        // converts the new_pos lineup enum to match eligibility field of batter
        if (str_new_pos[0] == 'O') {
                str_new_pos = "OF";
        } else if (str_new_pos[0] == 'C') {
                str_new_pos = "C";
        }

        // loop through the players elibility to check for a match
        while ((ss >> elig_pos) and not elig) {
                if (new_pos == CORNER) {
                        if (elig_pos == "3B" or elig_pos == "1B") {
                                elig = true;
                        }
                } else if (new_pos == INNER) {
                        if (elig_pos == "2B" or elig_pos == "SS") {
                                elig = true;
                        }
                } else {
                        if (elig_pos == str_new_pos) {
                                elig = true;
                        }
                }
        }
        return elig;
}

/**********add_new_pitcher********
 * adjusts the team salary and adds a new pitcher
 * Inputs:  
 *      pitcher player: the pitcher to add
 * Return:  n/a
 * Expects: a valid pitcher
 ************************/
void team::add_new_pitcher(pitcher player) 
{
        pitcher_salary += player.get_value();
        salary_left -= player.get_value();
        add_pitcher(player);
}

/**********add_pitcher********
 * places a new pitcher onto the team, ideally starting
 * Inputs:  
 *      pitcher player: the pitcher to add to the team
 * Return:  n/a
 * Expects: a valid pitcher
 ************************/
void team::add_pitcher(pitcher player) 
{
        if (check_team_size() >= NUM_PLAYERS) {
                cout << "TEAM IS FULL!!" << endl;
                return;
        }

        // if there are too many starters, bench the pitcher
        if (hurlers.size() >= num_pitchers) {
                cout << "Too many pitchers started; " << player.get_name() 
                     << " benched" << endl;
                bench_hurlers.push_back(player);
        } else {
                hurlers.push_back(player);
        }
}

/**********add_new_pitcher********
 * adjusts the team salary and adds a new batter to their argued position
 * Inputs:  
 *      batter player: the batter to add
 *      string position: the position to give them
 * Return:  n/a
 * Expects: a valid batter
 ************************/
void team::add_new_batter(batter player, string position) 
{
        batter_salary += player.get_value();
        salary_left -= player.get_value();
        add_batter(player, convert_position(position));
}

/**********add_batter********
 * places a batter pitcher onto the team, ideally in their argued position
 *      and benches the previous started if necessary
 * Inputs:  
 *      batter player: the batter to add to the team
 *      lineup position: the position to start them at
 * Return:  n/a
 * Expects: a valid batter and position
 ************************/
void team::add_batter(batter player, lineup position) 
{
        if (check_team_size() >= NUM_PLAYERS) {
                cout << "TEAM IS FULL!!" << endl;
                return;
        }
        if (check_extra_spot(position)) { return; }

        // triggers if argued position is bench
        if (not check_eligibility(player, position)) {
                bench_hitters.push_back(player);
        // bench the old starter
        } else if (hitters[position].is_player()) {
                batter bench = hitters[position];
                bench_hitters.push_back(bench);
                hitters[position] = player;
        } else {
                hitters[position] = player;
        }
}

/**********set_extra_pos********
 * changes the extra position, i.e. which of OF5, P10, or CI will not be used
 * Inputs:  
 *      string pos: the new extra position
 * Return:  n/a
 * Expects: pos = OF5, P, or CI
 ************************/
void team::set_extra_pos(string pos) 
{ 
        extra_pos = pos;
        if (extra_pos != "P") {
                num_pitchers = 10;
                // changes the batter's position if they are at the previous
                // extra position
                if (player_here(true, convert_position(extra_pos))) {
                        cout << "new position: ";
                        string new_pos;
                        cin >> new_pos;
                        lineup xlin_pos = convert_position(extra_pos);
                        lineup nlin_pos = convert_position(new_pos);
                        change_position(xlin_pos, nlin_pos);
                } 
        } else { 
                num_pitchers = 9;
                // benches the 10th pitcher
                if (player_here(false, P)) {
                        pitcher back = hurlers.back();
                        hurlers.pop_back();
                        bench_hurlers.push_back(back);
                }
        }
}

/**********print_batter_stats********
 * prints the stats of all the starting or benched batters on the team
 * Inputs:  
 *      bool start: indicates to print the starter data or bench data
 * Return:  n/a
 * Expects: n/a
 ************************/
void team::print_batter_stats(bool start) 
{
        int ab = 0;
        int runs = 0;
        int homeruns = 0;
        int rbi = 0; 
        int sb = 0;
        float avg = 0.0;

        // print starter data
        if (start) {
                int count = 0;
                for (int i = 0; i < NUM_HITTERS; i++) {
                        if (hitters[i].is_player()) {
                                count++;
                                ab += hitters[i].get_at_bats();
                                runs += hitters[i].get_runs();
                                homeruns += hitters[i].get_homeruns();
                                rbi += hitters[i].get_rbi();
                                sb += hitters[i].get_stolen_bases();
                                avg += hitters[i].get_average() * 
                                       hitters[i].get_at_bats();
                        }
                }
                avg = avg / ab;
                cout << "######  ";
                // prints the total stats of the starters
                batter("TOTAL", "NA", ab, runs, homeruns, rbi, sb, avg, batter_salary).print_stats();
                cout << "######  ";
                // prints the average stats of the starters
                batter("AVERAGE", "NA", ab / count, runs / count, 
                       homeruns / count, rbi / count, sb / count, avg, 
                       batter_salary).print_stats();
        // print the bench hitters' stats
        } else {
                int count = bench_hitters.size();
                for (auto it = bench_hitters.begin(); it != bench_hitters.end(); it++) {
                        ab += it->get_at_bats();
                        runs += it->get_runs();
                        homeruns += it->get_homeruns();
                        rbi += it->get_rbi();
                        sb += it->get_stolen_bases();
                        avg += it->get_average() * it->get_at_bats();
                }
                avg = avg / ab;
                cout << "######  ";
                batter("TOTAL", "NA", ab, runs, homeruns, rbi, sb, avg, 
                               batter_salary).print_stats();
                cout << "######  ";
                batter("AVERAGE", "NA", ab / count, runs / count, 
                       homeruns / count, rbi / count, sb / count, avg, 
                       batter_salary).print_stats();
        }
}

/**********print_pitcher_stats********
 * prints the stats of all the starting or benched pitchers on the team
 * Inputs:  
 *      bool start: indicates to print the starter data or bench data
 * Return:  n/a
 * Expects: n/a
 * Note: this function follows the same format as print_batter_stats
 ************************/
void team::print_pitcher_stats(bool start) 
{
        int innings_pitched = 0;
        int wins = 0;
        float era = 0.0;
        float whip = 0.0; 
        int strikeouts = 0;

        if (start) {
                int tot = hurlers.size();
                for (auto it = hurlers.begin(); it != hurlers.end(); it++) {
                        innings_pitched += it->get_innings();
                        wins += it->get_wins();
                        era += it->get_era() * it->get_innings();
                        whip += it->get_whip() * it->get_innings();
                        strikeouts += it->get_strikeouts();
                }
                era = era / innings_pitched;
                whip = whip / innings_pitched;
                cout << "######  ";
                pitcher("TOTAL", innings_pitched, wins, era, whip, 
                                strikeouts, pitcher_salary, false).print_stats();
                cout << "######  ";
                pitcher("AVERAGE", innings_pitched / tot, wins / tot, era, whip, 
                        strikeouts / tot, pitcher_salary, false).print_stats();
        } else {
                int tot = bench_hurlers.size();
                for (auto it = bench_hurlers.begin(); it != bench_hurlers.end(); it++) {
                        innings_pitched += it->get_innings();
                        wins += it->get_wins();
                        era += it->get_era() * it->get_innings();
                        whip += it->get_whip() * it->get_innings();
                        strikeouts += it->get_strikeouts();
                }
                era = era / innings_pitched;
                whip = whip / innings_pitched;
                cout << "######  ";
                pitcher("TOTAL", innings_pitched, wins, era, whip, 
                                strikeouts, pitcher_salary, false).print_stats();
                cout << "######  ";
                pitcher("AVERAGE", innings_pitched / tot, wins / tot, era, whip, 
                        strikeouts / tot, pitcher_salary, false).print_stats();
        }

}

/**********salary_summary********
 * Prints data about the team's salary and where it has been allocated
 ************************/
void team::salary_summary() 
{
        cout 
<< "*************************************************************************\n"
<< "*********                    SALARY SUMMARY                     *********\n"
<< "*************************************************************************\n"
<< endl
<< "----------------- BATTER SALARY: " << batter_salary << endl
<< "---------------- PITCHER SALARY: " << pitcher_salary << endl
<< endl
<< "------------------- SALARY LEFT: " << salary_left << endl
<< "------------------ PLAYERS LEFT: " << SALARY_PLAYERS - check_team_size() << endl
<< "------------------- $$ / PLAYER: " << salary_left / (SALARY_PLAYERS - check_team_size()) << endl
<< "------------------- LARGEST BID: " << salary_left - (SALARY_PLAYERS - check_team_size()) + 1
<< endl;

}