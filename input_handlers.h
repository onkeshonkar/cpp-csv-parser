#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// Validate user input to Y/N
string ask_YN();

//  Takes string from user
string ask_string(string, int);

//   Asks & validate from options (vector) of string
string ask_option_string(const vector<string> &, string);

string ask_name(int, bool);
string ask_gmail();
string ask_mob();
string ask_gender();
string ask_date(string &);
string ask_time();
string ask_int(int, int);
int ask_option(int);

// converts a string upper case
string to_upper(string &);

// converts a string lower case
string to_lower(string &);

/**
 *  Function definations
 */