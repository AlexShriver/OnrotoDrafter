/**************************************************************
 *                     pitcher.h
 *     Author: Alex Shriver
 *   
 *     Declaration of the class defined in pitcher.cpp. This is 
 *     a class to represent a pitcher and information about their
 *     projected season.
 *     
 **************************************************************/

#ifndef _PITCHER_H_
#define _PITCHER_H_

#include <iostream>

using namespace std;

class pitcher 
{
public:

        pitcher(string i_name, int i_innings, int i_wins, float i_era, 
                 float i_whip, int i_strikeouts, int i_HOSV, float i_value, bool i_starter, bool i_injured);
        pitcher();
        
        void print_stats();
        
        string get_name();
        bool get_starter();
        bool is_player();
        bool get_minors();
        int get_innings();
        int get_wins();
        float get_era();
        float get_whip();
        int get_strikeouts();
        int get_HOSV();
        int get_value();

        void set_injured();
        void set_starter(bool start); // true if starter
        void set_HOSV(int i_HOSV);
        void set_value(int new_val);
        void set_minors(bool league); // true is minors

private:

        // Member variables
        string name;
        bool starter;
        // stats
        int innings;
        int wins;
        float era;
        float whip;
        int strikeouts;
        int HOSV;
        int value;
        bool injured;
        bool player_here;
        bool minors;
};

#endif