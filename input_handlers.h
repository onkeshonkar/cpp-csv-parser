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
string ask_YN()
{
    string input;
repeat:
    cin >> input;
    input = toupper(input[0]);
    if (input.empty())
    {
        cout << "Error: Invalid!!!" << endl;
        goto repeat;
    }

    return (input == "Y" ? "Y" : "N");
}

string ask_string(string prompt = "string", int size = 20)
{
    string input;
repeat:
    cin.ignore(1024, '\n');
    cout << "Enter " << prompt << " (max " << size << " char) : ";
    getline(cin, input);
    if (input.empty())
    {
        cout << "Error: " << prompt << " cann't be empty!!!" << endl;
        goto repeat;
    }

    return input.size() > size ? input.substr(0, size) : input;
}

string ask_option_string(const vector<string> &options, string options_name)
{

    int i = 1;
    cout << "Choose " << options_name << endl;
    for (auto &option : options)
    {
        cout << "\t" << i << ". " << option << endl;
        ++i;
    }
    int option_index = ask_option(options.size()) - 1;
    return options[option_index];
}

string ask_name(int size = 20, bool upper_case = false)
{
    string name = ask_string("name", size);
    name[0] = toupper(name[0]);
    return upper_case ? to_upper(name) : name;
}

string ask_gmail()
{
    string gmail;

repeat:
    cout
        << "Enter your gmail : ";
    cin >> gmail;
    gmail = to_lower(gmail);
    int at_pos = gmail.find('@'); // position of @ in gmail string
    string domain = gmail.substr(at_pos + 1);

    for (auto &c : gmail.substr(0, at_pos))
    {
        if (!isalnum(c) && c != '.')
        {
            cout << "Error: Invalid gmail!!!" << endl;
            goto repeat;
        }
    }
    if (gmail.empty() || domain != "gmail.com")
    {
        cout << "Error: Incorrect gmail!!!" << endl;
        goto repeat;
    }

    return gmail;
}

string ask_mob()
{
    string mob;
repeat:
    bool is_valid = true;
    cout << "Enter Your mobile number: ";
    cin >> mob;
    for (auto &c : mob)
    {
        if (!isdigit(c))
        {
            is_valid = false;
        }
    }
    if (mob.length() > 10 || !is_valid)
    {
        cout << "Error: Invalid mobile number!!!" << endl;
        goto repeat;
    }
    return mob;
}

string ask_gender()
{
    string gender;
    vector<string> genders{"Male", "Female", "Others"};
    string options_name = "gender";
    gender = ask_option_string(genders, options_name);
    return gender;
}

string ask_date(string msg = "date")
{
    string date;
    string dd, mm, yy;
    string err = "Error: Invalid date!!!";
    int d, m, y;
repeat:
    cout << "Enter " << msg << "(dd/mm/yyyy) : ";
    cin >> date;

    stringstream ss(date);
    getline(ss, dd, '/');
    getline(ss, mm, '/');
    getline(ss, yy, '/');
    try
    {
        if (dd.length() != 2 || mm.length() != 2 || yy.length() != 4)
        {
            throw err;
        }
        d = stoi(dd);
        m = stoi(mm);
        y = stoi(yy);
    }
    catch (...)
    {
        cout << "Error: Invalid format!!!" << endl;
        goto repeat;
    }

    if ((d < 1 || d > 31) || (m < 1 || m > 12) || (y < 1800 || y > 2200))
    {

        cout << err << endl;
        goto repeat;
    }

    bool isLeap = (y % 4 == 0);

    if (m == 2)
    {
        if ((!isLeap && d > 28) || (isLeap && d > 29))
        {
            cout << err << endl;
            goto repeat;
        }
    }
    int month_30[] = {4, 6, 9, 11};
    for (int x : month_30)
    {
        if (m == x && d > 30)
        {
            cout << err << endl;
            goto repeat;
        }
    }

    dd = d < 10 ? "0" + to_string(d) : to_string(d);
    mm = m < 10 ? "0" + to_string(m) : to_string(m);
    date = dd + "/" + mm + "/" + to_string(y);
    return date;
}

string ask_time()
{
    string time;
    string hh, mm;
    int h, m;
repeat:
    cout << "Enter Time(HH:MM) : ";
    getline(cin, time);

    stringstream ss(time);
    getline(ss, hh, ':');
    getline(ss, mm, ':');
    try
    {
        if (hh.length() != 2 || mm.length() != 2)
        {
            throw "Invalid time!!!";
        }
        h = stoi(hh);
        m = stoi(mm);
    }
    catch (...)
    {
        cout << "Error: Invalid format!!!" << endl;
        goto repeat;
    }

    if ((h < 0 || h > 23) || (m < 0 || m > 59))
    {

        cout << "Error: Invalid time!!!" << endl;
        goto repeat;
    }
    hh = h < 10 ? "0" + to_string(h) : to_string(h);
    mm = m < 10 ? "0" + to_string(m) : to_string(m);
    time = hh + ":" + mm;
    return time;
}

string ask_int(int min, int max)
{
    string input;
    int num;
    bool range_based = (min != INT32_MIN && max != INT32_MAX);

repeat:

    if (range_based)
    {
        cout << "Enter a integer [" << min << " to " << max << "] : ";
    }
    else
    {
        cout << "Enter a integer : ";
    }

    cin >> input;
    try
    {
        num = stoi(input);
    }
    catch (...)
    {
        cout << "Error: Not a number!!!" << endl;
        goto repeat;
    }

    if (range_based && (num < min || num > max))
    {
        cout << "Error: integer is out of range!!!" << endl;
        goto repeat;
    }

    return to_string(num);
}

string ask_int(int max)
{
    return ask_int(0, max);
}

string ask_int()
{
    return ask_int(INT32_MIN, INT32_MAX);
}

int ask_option(int option_size)
{
    int op;
    string s;
repeat:
    cin >> s;
    try
    {
        op = stoi(s);
    }
    catch (...)
    {
        cout << "Error: Not a number!!!" << endl;
        goto repeat;
    }

    if (op < 1 || op > option_size)
    {
        cout << "Error: Invalid option!!!" << endl;
        goto repeat;
    }

    return op;
}

string to_upper(string &text)
{

    string upper_case = "";
    for (int i = 0; i < text.length(); ++i)
    {
        upper_case += toupper(text[i]);
    }
    return upper_case;
}

string to_lower(string &text)
{

    string lower_case = "";
    for (int i = 0; i < text.length(); ++i)
    {
        lower_case += tolower(text[i]);
    }
    return lower_case;
}
