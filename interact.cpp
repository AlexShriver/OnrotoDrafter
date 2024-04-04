/**************************************************************
 *                     interact.cpp
 *     Author: Alex Shriver
 *   
 *     This file defines functions declared in interact.h. 
 *     These functions are used in the OnRoto_Drafter program to 
 *     allow user interaction with their team and the databases
 *     of available players.
 * 
 *     Full disclosure, I wrote this file at 3 am, just hours before
 *     the start of the draft.
 * 
 *     TODO: Clean up this file
 *     
 **************************************************************/

#include <iostream>
#include <fstream>
#include <istream>
#include <iomanip>
#include "interact.h"

using namespace std;

// Attempts to open and store a file to an ofstream based on its name
void open_ofstream(ofstream &stream, string file_name) {
        stream.open(file_name);
        if (not stream.is_open()) {
                cerr << "Error: could not open file " << file_name << endl;
                exit(EXIT_FAILURE);
        }
}

/**********query_loop********
 * Runs a query loop which allows an OnRoto_Drafter user to interact with 
 * available players and their team
 * Inputs:
 *      batter_database *avail_batters: a full database of available hitters
 *      pitcher_database *avail_pitchers: a full database of available pitchers
 *      team *my_team: the user's team
 * Return: n/a
 * Expects: nonnull databases and team
 ************************/
void query_loop(batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team)
{
        // TODO: Assert arguments are nonnull

        // Create the drafted players file
        ofstream drafted_players;
        open_ofstream(drafted_players, "drafted_players.txt");

        string command = "", sure;

        // This is the general loop which will end when the command is to quit
        while (command != "quit" and command[0] != 'q') 
        {
                cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                         HOME DIRECTORY                                         ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n";
                cout << "***   draft player - d   ***   see available - a   "
                     << "***   manipulate team - t   ***   autodraft - auto   ***   quit - quit\n";
                
                cout << "query ---- ";
                cin >> command;
                if (command == "d") {
                        // draft a player
                        draft_loop(avail_batters, avail_pitchers, my_team, drafted_players);
                } else if (command == "a") {
                        // print all the available players
                        print(avail_batters, avail_pitchers);
                } else if (command == "t") {
                        // manipulate the team
                        manip_team(my_team);
                } else if (command == "auto") {
                        // automatically drafts the players listed in the file
                        auto_draft(avail_batters, avail_pitchers, my_team, drafted_players);
                } else if (command == "quit" or command[0] == 'q') {
                        cout << "\n\nARE YOU SURE??? YOU WILL LOSE ALL PROGRESS: [y/n] ";
                        cin >> sure;
                        if (sure != "y") {
                                command = "keep going";
                        }
                } else {
                        cout << "unrecognized\n";
                }
        }

        drafted_players.close();
}

/**********draft_loop********
 * Runs a query loop giving the user the ability to draft a player to their
 *      team or another team
 * Inputs:
 *      batter_database *avail_batters:   a full database of available hitters
 *      pitcher_database *avail_pitchers: a full database of available pitchers
 *      team *my_team:                    the user's team
 *      ofstream &drafted_players:        the file to output all drafted players to
 * Return: n/a
 * Expects: nonnull databases and team
 ************************/
void draft_loop(batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team, ofstream &drafted_players)
{
        // TODO: Assert nonnull arguments
        string hit_pit, draft, pos, first, last, name = "";
        int price;
        batter hitter;
        pitcher hurler;

        // while loop to continue drafting unless the user asks to quit
        while (name[0] != 'q') {
                cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                      DRAFTING DIRECTORY                                        ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n";
                cout << "What is the player's name? ";
                cin >> first;
                if (first == "q") { break; }
                cin >> last;
                name = first + " " + last;

                cout << "Is a pitcher or hitter being drafted? [p/h] ";
                cin >> hit_pit;

                // Removes the player
                // TODO: Implement different behavior if the player doesn't exist
                if (hit_pit == "p") {
                        hurler = avail_pitchers->get_player(name);
                        if (hurler.is_player()) {
                                avail_pitchers->remove_player(name);
                        } else { 
                                avail_pitchers->check_similar_players(first, last);
                                continue; 
                        }
                } else if (hit_pit == "h") {
                        hitter = avail_batters->get_player(name);
                        if (hitter.is_player()) {
                                avail_batters->remove_player(name);
                        } else { 
                                avail_batters->check_similar_players(first, last);
                                continue; 
                        }
                } else {
                        cout << hit_pit << ": not recognized\n";
                        continue;
                }

                cout << "Did you draft this player? [y/n] ";
                cin >> draft;

                // add the player to the user's team
                if (draft == "y") {      
                        cout << "How much did you spend? ";
                        cin >> price;
                        if (hit_pit == "h") {  // determine where to play hitter
                                hitter.set_value(price);
                                cout << "What position will he play? Here are your current hitters:\n";
                                my_team->print_hitters();
                                cout << "\n\nHere are your position options:\n";
                                hitter.print_stats();
                                cout << "C1, C2, DH, 1B, 2B, SS, 3B, CI, MI, OF1, OF2, OF3, OF4, OF5, BENCH\n";
                                cout << "position: ";
                                cin >> pos;
                                my_team->add_new_batter(hitter, pos);
                        } else {        // determine how to add pitcher to team
                                hurler.set_value(price);
                                my_team->add_new_pitcher(hurler);
                                cout << "Your pitcher has been added to your team. Here is your new pitching staff: \n";
                                my_team->print_pitchers();
                                cout << "\n\nWould you like to change his spot? ";
                                cin >> pos;
                                if (pos == "y") {
                                        cout << "Tell me where to put him [b/s] ";
                                        cin >> pos;
                                        if (pos == "b") { // bench him
                                                my_team->bench_player(name, false);
                                        } else if (pos == "s") {
                                                my_team->make_pitcher_starter(name);
                                        }
                                }
                        }
                        drafted_players << hit_pit + " d " << price << " " + pos + " " + name << endl;
                } else {
                        // output that the player was drafted
                        drafted_players << hit_pit + " " + name << endl;
                }

                cout << "****************************************\n"
                     << "****       Player Drafted           ****\n"
                     << "****************************************\n\n\n\n";

                cout << "would you like to navigate to your team? [y/n] ";
                cin >> name;
                // manipulate the team
                if (name == "y") {
                        manip_team(my_team);
                        break;
                } else if (name == "q") {
                        break;
                }
        }
}

/**********print********
 * Prints the database of players by position, stat, or value based on user 
 *      inputted requests from cin
 * Inputs:
 *      batter_database *avail_batters: a full database of available hitters
 *      pitcher_database *avail_pitchers: a full database of available pitchers
 * Return: n/a
 * Expects: nonnull databases
 ************************/
void print(batter_database *avail_batters, pitcher_database *avail_pitchers) 
{
        // pit_hit used to designate looking for pitchers or hitters
        // TODO: find a way to organize this so you don't have to pick between 
                // pitchers and hitters so early, or if at all
        string pit_hit = "", filter, first, last, name, stat, pos;

        // this loops until the user enters a word starting with Q or q
        while (pit_hit[0] != 'q' && pit_hit[0] != 'Q') {
                cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                AVAILABLE PLAYERS DIRECTORY                                     ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n";
                cout << "Would you like to look at pitchers or hitters [p/h] ";
                cin >> pit_hit;

                // if they choose to quit right away, get out of here
                if (pit_hit[0] == 'q' || pit_hit[0] == 'Q') {
                        break;
                }

                cout << "Grab a player [pl] or filter by stat [s] or position [po]? [pl/s/po] ";
                cin >> filter;

                // print a player
                if (filter == "pl") {
                        cout << "What is the players name? ";
                        cin >> first >> last;
                        name += first + " " + last;
                        if (pit_hit == "p") {
                                avail_pitchers->get_player(name).print_stats();
                        } else if (pit_hit == "h") {
                                avail_batters->get_player(name).print_stats();
                        }
                // print the batters or pitchers by stat
                } else if (filter == "s") {
                        cout << "What stat would you like to sort by?\n";
                        // for pitchers
                        if (pit_hit == "p") {
                                cout << "Options: [ip, w, era, whip, hosv, so, $] (Please type the entire stat lower case)\n";
                                cout << "stat: ";
                                cin >> stat;
                                // TODO: integrate tolower() to accept upper and lower case spellings of the options
                                cout << endl << endl;
                                print_pitcher_categories(); // header of the stats before list of players
                                if (stat == "ip") {
                                        avail_pitchers->print_innings();
                                } else if (stat == "w") {
                                        avail_pitchers->print_wins();
                                } else if (stat == "era") {
                                        avail_pitchers->print_eras();
                                } else if (stat == "whip") {
                                        avail_pitchers->print_whips();
                                } else if (stat == "hosv") {
                                        avail_pitchers->print_HOSV();
                                } else if (stat == "so") {
                                        avail_pitchers->print_strikeouts();
                                } else if (stat == "$") {
                                        avail_pitchers->print_values();
                                } else if (stat[0] == 'q') {
                                        break;
                                } else {
                                        cout << "unrecognized: " << stat << endl;
                                }
                                cout << endl << endl;
                        // for hitters
                        } else if (pit_hit == "h") {
                                cout << "options: [ab, r, hr, rbi, sb, avg, $] (Please type the entire stat lower case)\n";
                                cout << "stat: ";
                                cin >> stat;
                                if (stat[0] == 'q') { break; }
                                // TODO: integrate tolower() to accept upper and lower case spellings of the options
                                cout << endl << endl;

                                print_hitter_categories(); // header of the stats before list of players
                                avail_batters->print_database(stat);
                        }
                } else if (filter == "po") {  // filter by position
                        if (pit_hit == "p") {
                                avail_pitchers->print_values();
                        } else {
                                cout << "What position would you like to filter? Here are your options: \n";
                                cout << "[C, 1B, 2B, SS, 3B, CI, MI, OF]\n";
                                cout << "position: ";
                                cin >> pos;
                                cout << endl;
                                cout << "What stat would you like to filter the " + pos + " position by?" << endl;
                                cout << "options: [ab, r, hr, rbi, sb, avg, $] (Please type the entire stat lower case)" << endl;
                                cout << "stat: ";
                                cin >> stat;
                                print_hitter_categories();
                                avail_batters->print_pos(pos, stat);
                                // TODO: allow user to filter by stat within positions
                        }
                        cout << endl << endl;
                } else if (filter[0] == 'q') {
                        break;
                } else {
                        cout << "unrecognized: " << filter << endl;
                }
                name = "";
        }
        cout << "******************************************\n"
             << "**** Exiting Available Players Module ****\n"
             << "******************************************\n\n\n\n";

}

/**********manip_team********
 * Allows the user to manipulate their own team by changing their lineup positions
 * Inputs:
 *      team *my_team: the user's team
 * Return: n/a
 * Expects: nonnull team
 ************************/
void manip_team(team *my_team) 
{
        // pit_hit designates pitcher or hitter 
        string command = "", pit_hit, first, last, name = "", pos;

        // query until asked to quit
        while (command[0] != 'q' && command[0] != 'Q') {
                cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                         TEAM DIRECTORY                                         ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++                                                                                                ++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                     << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n";
                cout << "Your options:    [p:  print options]  [a:  adjust lineup/rotation]  [x:  change extra position]\n";
                cout << "command: ";
                cin >> command;
                if (pit_hit[0] == 'q' || pit_hit[0] == 'Q') {
                        break;
                } else if (command == "p") { // print
                        cout << "Your options:    [a:  print entire team]  [h:  print hitters]  [p:  print pitchers]  [s:  print salary summary]\n";
                        cout << "command: ";
                        cin >> command;
                        if (command[0] == 'q' || command[0] == 'Q') {
                                break;
                        // print entire team
                        // TODO: restructure this with less conditionals
                        } else if (command == "a") {
                               cout << endl << endl;
                               my_team->print_hitters();
                               cout << endl << endl << endl;
                               my_team->print_pitchers(); 
                               cout << endl << endl << endl;
                        // just print the hitters
                        } else if (command == "h") {
                                cout << endl << endl;
                                my_team->print_hitters();
                                cout << endl << endl << endl;
                        // just print the pitchers
                        } else if (command == "p") {
                                cout << endl << endl;
                                my_team->print_pitchers();
                                cout << endl << endl << endl;
                        // print the salary summary for the team (how much money we left and where we spent it)
                        } else if (command == "s") {
                                cout << endl << endl;
                                my_team->salary_summary();
                                cout << endl << endl << endl;
                        }
                // adjust lineup
                } else if (command == "a") {
                        cout << "Are you moving a pitcher or hitter? [p/h] ";
                        cin >> pit_hit;
                        if (pit_hit[0] == 'q' || pit_hit[0] == 'Q') {
                                break;
                        }
                        cout << "What is the players name? ";
                        cin >> first >> last;
                        name += first + " " + last;
                        if (pit_hit == "p") {
                                cout << "Are we benching or starting " << name << " [b/s] ";
                                cin >> pos;
                                if (pos == "b") {
                                        my_team->bench_player(name, false);
                                } else if (pos == "s") {
                                        my_team->make_pitcher_starter(name);      
                                } else {
                                        cout << "unrecognized" << endl;
                                }
                        } else if (pit_hit == "h") { // TODO: Implement this
                                cout << "I'm sorry, but this is not yet implemented" << endl;
                        } 
                // In this league, we have one extra position outside the normal lineup. These are the options
                } else if (command == "x") {
                        cout << "What would you like to make the extra position? Your options are: CI, OF5, P\n";
                        cout << "command: ";
                        cin >> pos;
                        my_team->set_extra_pos(pos);
                } else {
                        cout << command << ": not recognized\n";
                }
        }
}

/*******************************************************************************
 *
 *          These two functions print the stat categories lineup up with 
 *          how they are printed for individual players
 * 
 ******************************************************************************/
void print_hitter_categories()
{
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

void print_pitcher_categories()
{
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

// Attempts to open and store a file to an ifstream based on its name
void open_or_die(ifstream &stream, string file_name) {
        stream.open(file_name);
        if (not stream.is_open()) {
                cerr << "Error: could not open file " << file_name << endl;
                exit(EXIT_FAILURE);
        }
}

/**********auto_draft********
 * Automatically drafts the players from a list to set the state of the draft.
 *      This is useful if the program ends unexpectedly or I miss a few players
 * Inputs:
 *      batter_database *avail_batters:   a full database of available hitters
 *      pitcher_database *avail_pitchers: a full database of available pitchers
 *      team *my_team:                    the user's team
 *      ofstream &drafted_players:        the file to output all drafted players to
 * Return: n/a
 * Expects: nonnull team
 ************************/
void auto_draft(batter_database *avail_batters, pitcher_database *avail_pitchers, 
                team *my_team, ofstream &drafted_players) {
        cout << "******************************************\n"
             << "****            AUTODRAFTER           ****\n"
             << "******************************************\n\n";

        cout << "Please provide the filename with the players to draft below." << endl
             << "WARNING: do not name the file \"drafted_players.txt\". You must rename the file first.\n\n"
             << "filename: ";

        // Open the file
        string file_name;
        cin >> file_name;
        ifstream file;
        open_or_die(file, file_name);

        // important variables
        pitcher hurler;
        batter hitter;
        string hit_pit, first, last, name, pos;
        int sal;
        bool mine = false;

        // draft loop
        while (true) {
                hit_pit = "";
                file >> hit_pit >> first;
                if (first == "d") { // the player was drafted to my team
                        file >> sal >> pos >> first >> last;
                        name = first + " " + last;
                        mine = true;
                } else {
                        file >> last;
                        name = first + " " + last;
                        mine = false;
                }

                // Draft the player
                if (hit_pit == "p") {
                        hurler = avail_pitchers->get_player(name);
                        if (hurler.is_player()) {
                                avail_pitchers->remove_player(name);
                        } else { continue; }
                } else if (hit_pit == "h") {
                        hitter = avail_batters->get_player(name);
                        if (hitter.is_player()) {
                                avail_batters->remove_player(name);
                        } else { continue; }
                } else {
                        if (file.eof()) { break; }
                        cout << hit_pit << ": not recognized, so " << name 
                            << " was not drafted." << endl; 
                        continue;
                }

                // add the player to the user's team
                if (mine) {    
                        if (hit_pit == "h") {  // determine where to play hitter
                                hitter.set_value(sal);
                                my_team->add_new_batter(hitter, pos);
                        } else {        // determine how to add pitcher to team
                                hurler.set_value(sal);
                                my_team->add_new_pitcher(hurler);
                                if (pos == "b") { // bench him
                                        my_team->bench_player(name, false);
                                } else if (pos == "s") {
                                        my_team->make_pitcher_starter(name);
                                }
                        }
                        drafted_players << hit_pit + " d " << sal << " " + pos + " " + name << endl;
                } else {
                        // output that the player was drafted
                        drafted_players << hit_pit + " " + name << endl;
                }   

                if (file.eof()) { break; }
        }
        file.close();
}