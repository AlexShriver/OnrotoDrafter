/**************************************************************
 *                     draft.cpp
 *     Author: Alex Shriver
 *   
 *     This file serves as the driver for the OnRoto_Drafter 
 *     executable, containing the main function, opening the 
 *     starter files, and calling the query loop.
 *     
 **************************************************************/

#include <iostream>
#include "team.h"
#include "batter_database.h"
#include "pitcher_database.h"
#include "interact.h"
#include <fstream>

using namespace std;

void open_or_die(ifstream &stream, string file_name);

int main(int argc, char *argv[])
{
        if (argc != 4) {
                cerr << "Invalid usage. Format: ./OnRoto_Drafter "
                     << "[batter_file] [pitcher_file] [team_file]\n";
                return 1;
        }

        // Open the files
        ifstream batter_file, pitcher_file, team_file;
        open_or_die(batter_file, argv[1]);
        open_or_die(pitcher_file, argv[2]);
        open_or_die(team_file, argv[3]);

        // initialize the team and databases
        // TODO: Think about using heap initialization
        batter_database batter_db(batter_file);
        pitcher_database pitcher_db(pitcher_file);
        team my_team(team_file);

        // query loop as defined in interact.cpp
        query_loop(&batter_db, &pitcher_db, &my_team);

        batter_file.close();
        pitcher_file.close();
        team_file.close();

        return 0;
}

// Attempts to open and store a file to an ifstream based on its name
void open_or_die(ifstream &stream, string file_name) {
        stream.open(file_name);
        if (not stream.is_open()) {
                cerr << "Error: could not open file " << file_name << endl;
                exit(EXIT_FAILURE);
        }
}