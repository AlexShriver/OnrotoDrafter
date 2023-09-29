#include <iostream>
#include <fstream>
#include <istream>
#include <iomanip>
#include "interact.h"

using namespace std;

void query_loop(batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team)
{
        string command = "", sure;
        while (command != "quit" and command != "q") 
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
                     << "***   manipulate team - t   ***   quit - quit\n";
                
                cout << "query ---- ";
                cin >> command;
                if (command == "d") {
                        draft_loop(avail_batters, avail_pitchers, my_team);
                } else if (command == "a") {
                        print(avail_batters, avail_pitchers);
                } else if (command == "t") {
                        manip_team(my_team);
                } else if (command == "quit" or command == "q") {
                        cout << "\n\nARE YOU SURE??? YOU WILL LOSE ALL PROGRESS: [y/n] ";
                        cin >> sure;
                        if (sure != "y") {
                                command = "keep going";
                        }
                } else {
                        cout << "unrecognized\n";
                }
        }
}

// void draft(batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team) 
// {
//         string file;
//         cout << "Would you like to draft from a file? [y/n] ";
//         cin >> file;
//         if (file == "y") {
//                 ifstream stream;
//                 cout << "Give me a filename: ";
//                 cin >> file;
//                 while (not open_up(stream, file) and file != "n" and file != "quit") {
//                         cout << "Would you like to try another file? [filename/n] ";
//                         cin >> file;
//                 }
//                 if (file != "n") {
//                         draft_file(stream, avail_batters, avail_pitchers, my_team);
//                 } else if (file == "quit") {
//                         return;
//                 }
//         } else {
//                 draft_loop(avail_batters, avail_pitchers, my_team);
//         }
// }

void draft_loop(batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team)
{
        string hit_pit, draft, pos, first, last, name = "";
        int price;
        batter hitter;
        pitcher hurler;

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
                cin >> first >> last;
                name += first + " " + last;

                cout << "Is a pitcher or hitter being drafted? [p/h] ";
                cin >> hit_pit;

                if (hit_pit == "p") {
                        hurler = avail_pitchers->get_player(name);
                        if (hurler.is_player()) {
                                avail_pitchers->remove_player(name);
                        } else { break; }
                } else if (hit_pit == "h") {
                        hitter = avail_batters->get_player(name);
                        if (hitter.is_player()) {
                                avail_batters->remove_player(name);
                        } else { break; }
                } else {
                        cout << hit_pit << ": not recognized\n";
                        break;
                }

                cout << "Did you draft this player? [y/n] ";
                cin >> draft;

                if (draft == "y") {
                        cout << "How much did you spend? ";
                        cin >> price;
                        if (hit_pit == "h") {
                                hitter.set_value(price);
                                cout << "What position will he play? Here are your current hitters:\n";
                                my_team->print_hitters();
                                cout << "\n\nHere are your position options:\n";
                                hitter.print_stats();
                                cout << "C1, C2, DH, 1B, 2B, SS, 3B, CI, MI, OF1, OF2, OF3, OF4, OF5, BENCH\n";
                                cout << "position: ";
                                cin >> pos;
                                my_team->add_new_batter(hitter, pos);
                        } else {
                                hurler.set_value(price);
                                cout << "Is your pitcher a starter or reliever? [r/s] ";
                                cin >> pos;
                                if (pos == "r") {
                                        hurler.set_starter(false);
                                }
                                my_team->add_new_pitcher(hurler);
                                cout << "Your pitcher has been added to your team. Here is your new pitching staff: \n";
                                my_team->print_pitchers();
                                cout << "\n\nWould you like to change his spot? ";
                                cin >> pos;
                                if (pos == "y") {
                                        cout << "Tell me where to put him [b/s] ";
                                        cin >> pos;
                                        if (pos == "b") {
                                                my_team->bench_player(name, false);
                                        } else if (pos == "s") {
                                                my_team->make_pitcher_starter(name);
                                        }
                                }
                        }
                }
                cout << "****************************************\n"
                     << "****       Player Drafted           ****\n"
                     << "****************************************\n\n\n\n";

                cout << "would you like to navigate to your team? [y/n] ";
                cin >> name;
                if (name == "y") {
                        manip_team(my_team);
                        break;
                } else if (name == "q") {
                        break;
                }
                name = "";
        }
}

void print(batter_database *avail_batters, pitcher_database *avail_pitchers) 
{
        string pit_hit = "", filter, first, last, name, stat, pos;

        while (pit_hit[0] != 'q') {
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

                if (pit_hit[0] == 'q') {
                        break;
                }
                cout << "Grab a player or filter by stat/position? [pl/s/po] ";
                cin >> filter;

                if (filter == "pl") {
                        cout << "What is the players name? ";
                        cin >> first >> last;
                        name += first + " " + last;
                        if (pit_hit == "p") {
                                avail_pitchers->get_player(name).print_stats();
                        } else if (pit_hit == "h") {
                                avail_batters->get_player(name).print_stats();
                        }
                } else if (filter == "s") {
                        cout << "What stat would you like to sort by?\n";
                        if (pit_hit == "p") {
                                cout << "options: ip, w, era, whip, so, $\n";
                                cout << "stat: ";
                                cin >> stat;
                                cout << endl << endl;
                                print_pitcher_categories();
                                if (stat == "ip") {
                                        avail_pitchers->print_innings();
                                } else if (stat == "w") {
                                        avail_pitchers->print_wins();
                                } else if (stat == "era") {
                                        avail_pitchers->print_eras();
                                } else if (stat == "whip") {
                                        avail_pitchers->print_whips();
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
                        } else if (pit_hit == "h") {
                                cout << "options: ab, r, hr, rbi, sb, avg, $\n";
                                cout << "stat: ";
                                cin >> stat;
                                cout << endl << endl;
                                print_hitter_categories();
                                if (stat == "ab") {
                                        avail_batters->print_at_bats();
                                } else if (stat == "r") {
                                        avail_batters->print_runs();
                                } else if (stat == "hr") {
                                        avail_batters->print_homeruns();
                                } else if (stat == "rbi") {
                                        avail_batters->print_rbis();
                                } else if (stat == "sb") {
                                        avail_batters->print_stolen_bases();
                                } else if (stat == "avg") {
                                        avail_batters->print_averages();
                                } else if (stat == "$") {
                                        avail_batters->print_values();
                                } else if (stat[0] == 'q') {
                                        break;
                                } else {
                                        cout << "unrecognized: " << stat << endl;
                                }
                                cout << endl << endl;
                        }
                } else if (filter == "po") {
                        if (pit_hit == "p") {
                                avail_pitchers->print_values();
                        } else {
                                cout << "What position would you like to filter? Here are your options: \n";
                                cout << "C, 1B, 2B, SS, 3B, CI, MI, OF\n";
                                cout << "position: ";
                                cin >> pos;
                                cout << endl << endl;
                                print_hitter_categories();
                                avail_batters->print_pos(pos);
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

void manip_team(team *my_team) 
{
        string command = "", pit_hit, first, last, name = "", pos;
        while (command[0] != 'q') {
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
                if (command[0] == 'q') {
                        break;
                } else if (command == "p") {
                        cout << "Your options:    [a:  print entire team]  [h:  print hitters]  [p:  print pitchers]  [s:  print salary summary]\n";
                        cout << "command: ";
                        cin >> command;
                        if (command[0] == 'q') {
                                break;
                        } else if (command == "a") {
                               cout << endl << endl;
                               my_team->print_hitters();
                               cout << endl << endl << endl;
                               my_team->print_pitchers(); 
                               cout << endl << endl << endl;
                        } else if (command == "h") {
                                cout << endl << endl;
                                my_team->print_hitters();
                                cout << endl << endl << endl;
                        } else if (command == "p") {
                                cout << endl << endl;
                                my_team->print_pitchers();
                                cout << endl << endl << endl;
                        } else if (command == "s") {
                                cout << endl << endl;
                                my_team->salary_summary();
                                cout << endl << endl << endl;
                        }
                } else if (command == "a") {
                        cout << "Are you moving a pitcher or hitter? [p/h] ";
                        cin >> pit_hit;
                        if (pit_hit[0] == 'q') {
                                break;
                        }
                        cout << "What is the players name? ";
                        cin >> first >> last;
                        name += first + " " + last;
                        if (pit_hit == "p") {
                                cout << "Are we benching, starting " << name << " [b/s] ";
                                cin >> pos;
                                if (pos == "b") {
                                        my_team->bench_player(name, false);
                                } else if (pos == "s") {
                                        my_team->make_pitcher_starter(name);      
                                } else {
                                        cout << "unrecognized" << endl;
                                }
                        }
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

// bool open_up(ifstream &stream, string file_name) {
//         stream.open(file_name);
//         if (not stream.is_open()) {
//                 cerr << "Error: could not open file " << file_name << endl;
//                 return false;
//         }
//         return true;
// }

// void draft_file(ifstream &file, batter_database *avail_batters, pitcher_database *avail_pitchers, team *my_team) 
// {}

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