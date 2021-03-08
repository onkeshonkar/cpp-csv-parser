#include <iostream>
#include <vector>
#include <fstream> // file handling
#include <sstream> // stringstream
#include <iomanip> // setw()
#include <unordered_map>

#include "input_handlers.h"

using namespace std;

class CSVParser
{
private:
    string file_name;

    //  Stores size for each columns.
    vector<int> columns_width;

    //  Column(heading) name for each column.
    vector<string> columns_name;

    vector<vector<string>> parsed_data;
    bool use_new_column;
    bool validators_configured{false};

    //  UID, unique id generated for each row & stored while parsing csv.
    int prev_uid{0};

    // Map of column name and input validator functions of string retun type.
    unordered_map<string, string (*)()> input_validators;

    // gives vector of words, spliting at comma(,).
    vector<string> split_at_comma(string &);

    void print_columns_name();
    void print_row(vector<string> &);
    void parse_file();
    int find_uid_index(int uid);
    void set_column_width(vector<string> &);
    void set_file_name(string);

public:
    CSVParser(string file_name) : use_new_column(false)
    {
        set_file_name(file_name);
        parse_file();
    }

    CSVParser(string file_name, vector<string> &columns) : use_new_column(true)
    {
        set_file_name(file_name);

        columns_name.push_back("UID"); // UID column name.
        columns_width.push_back(5);    // default UID column width.
        for (auto &col : columns)
        {
            columns_name.push_back(col);
            columns_width.push_back(col.length() + 2); // Intialize column width a/c column text size
        }
        parse_file();
    }

    void get_validators(unordered_map<string, string (*)()> &);
    void print_all();
    void insert_row();

    // Find a row in parsed_data by uid and update(if exists) in memory only.
    int update_row(int);

    // Find a row in parsed_data by uid and delete(if exists) in memory only.
    int delete_row(int);

    // Find a row in parsed_data by uid
    void find_row(int);

    /**
     * @brief Filters from all rows for matching column value.
     * @param   column_name     Column for which you want to filter
     * @param   value           Column value that need to be matched
     * @param   ignore_case     Match with case insensitive?
     */
    void filter_by_column(string, string, bool);

    /**
     * @brief   Save/export the modified data to new file.
     * @param   new_file    file name to save with
     * @param   save_column Wheater to include column name or not in new file, defult is false
     */
    void save_modified(string, bool);
};

/* 
    Funtion defination goes here
*/

void CSVParser::get_validators(unordered_map<string, string (*)()> &validators)
{
    if (!validators.size())
    {
        cout << "Error: Validators map can't be empty!!!" << endl;
        exit(1);
    }
    if (columns_name.size() <= 1)
    {
        cout << "Error: Columns name is empty!!!" << endl;
        exit(1);
    }

    // check and intialize if validators exists for each column except UID.
    for (auto &column : columns_name)
    {
        if (column == "UID")
        {
            continue;
        }
        if (!validators[column])
        {
            cout << " Error: No validator function mapped for coumn \'" << column << "\'!!!" << endl;
            exit(1);
        }
        input_validators[column] = validators[column];
    }
    validators_configured = true;
}

void CSVParser::set_file_name(string file_name)
{

    int dot_pos = file_name.find('.'); // finds index of (.)
    string extension = file_name.substr(dot_pos + 1);
    extension = to_lower(extension);
    if (extension != "csv")
    {
        cout << "Error: File with csv extension required!!!" << endl;
        exit(1);
    }

    fstream file;
    file.open(file_name);
    if (!file.good())
    {
        cout << "Error: Cann't find file '" << file_name << "' !!!" << endl;
        file.close();
        exit(1);
    }
    file.close();
    this->file_name = file_name;
}

vector<string> CSVParser::split_at_comma(string &line)
{
    vector<string> splited;
    string word;
    stringstream ss(line);
    string new_uid = to_string(prev_uid + 1);
    splited.push_back(new_uid);
    while (getline(ss, word, ','))
    {
        splited.push_back(word);
    }
    return splited;
}

void CSVParser::set_column_width(vector<string> &row)
{
    int max_col_width = 30;
    for (int i = 0; i < row.size(); i++)
    {
        int len = row[i].length() + 2;
        if (len > max_col_width)
        {
            len = 35;
        }
        if (columns_width[i] < len)
        {
            columns_width[i] = len;
        }
    }
}

void CSVParser::print_columns_name()
{
    int total_width = 0;

    for (int i = 0; i < columns_name.size(); ++i)
    {
        cout << left << setw(columns_width[i]) << to_upper(columns_name[i]);
        total_width += columns_width[i];
    }
    cout << endl;
    while (total_width--)
    {
        cout << "=";
    }
    cout << endl;
}

void CSVParser::print_row(vector<string> &row)
{

    for (int i = 0; i < row.size(); ++i)
    {
        string word = row[i];
        if (word.length() > 30)
        {
            word = word.substr(0, 30) + "...";
        }
        cout << left << setw(columns_width[i]) << word;
    }

    cout << endl;
}

void CSVParser::parse_file()
{
    fstream file;
    string line;
    file.open(file_name);
    bool first_row = true;
    if (!file.good())
    {
        file.close();
        cout << "Error: Cann't read!!!" << file_name << endl;
        exit(1);
    }

    if (file.peek() == EOF)
    {
        return; // function returns if file is empty!
    }

    cout << "parsing csv file....." << endl;
    while (getline(file, line))
    {
        if (first_row && !use_new_column)
        {
            columns_name = split_at_comma(line);
            columns_name[0] = "UID";
            first_row = false;
            columns_width = vector<int>(columns_name.size(), 7);
        }
        else
        {
            vector<string> splited = split_at_comma(line);
            parsed_data.push_back(splited);
            set_column_width(splited);
            ++prev_uid;
        }
    }
    file.close();
    cout << "parsing complete." << endl;
}

void CSVParser::print_all()
{

    if (!parsed_data.size())
    {
        cout << "Warning: nothing to print, parsed data is empty!!!" << endl;
        return;
    }

    print_columns_name();
    int total_row = 0;
    for (auto &row : parsed_data)
    {
        print_row(row);
        ++total_row;
    }

    cout << "\n\nTotal rows: " << total_row << endl
         << endl;
}

int CSVParser::find_uid_index(int uid)
{
    // since data is stored insorted order, doing binary search.
    int left = 0;
    int right = parsed_data.size() - 1;
    int mid;
    while (left <= right)
    {
        mid = (left + right) / 2;
        int mid_value = stoi(parsed_data[mid][0]);
        if (mid_value > uid)
        {
            right = mid - 1;
        }
        else if (mid_value < uid)
        {
            left = mid + 1;
        }
        else
        {
            return mid;
        }
    }
    return -1;
}

void CSVParser::find_row(int uid)
{
    int uid_index = find_uid_index(uid);

    if (uid_index == -1)
    {
        cout << "Error: No record found for UID " << uid << " !!!" << endl;
        return;
    }

    print_columns_name();
    print_row(parsed_data[uid_index]);
    cout << endl;
}

void CSVParser::insert_row()
{
    if (!validators_configured)
    {
        cout << "Error: Input validator functions not configured !!!" << endl;
        exit(1);
    }

    cout << "Waiting for new record..." << endl;
    vector<string> new_row;
    string new_uid = to_string(prev_uid + 1);
    new_row.push_back(new_uid);
    for (int i = 1; i < columns_name.size(); ++i)
    {
        string new_cell = input_validators[columns_name[i]]();
        new_row.push_back(new_cell);
    }
    parsed_data.push_back(new_row);
    ++prev_uid;
    set_column_width(new_row);
    cout << "New record inserted!" << endl
         << endl;
}

int CSVParser::update_row(int uid)
{
    if (!validators_configured)
    {
        cout << "Error: Input validator functions not configured !!!" << endl;
        exit(1);
    }

    int uid_index = find_uid_index(uid), i = 0;

    if (uid_index == -1)
    {
        cout << "Error: No record found for UID " << uid << "!!!" << endl;
        return 0;
    }

    //print details for found uid.
    print_columns_name();
    print_row(parsed_data[uid_index]);
    cout << endl;
    // ask for each column to update.
    for (int i = 1; i < columns_name.size(); ++i)
    {
        string option;
        cout << "Update " << columns_name[i] << " for UID " << uid << " [Y/N]?:";
        option = ask_YN();

        if (option == "Y")
        {
            string newValue;
            newValue = input_validators[columns_name[i]]();
            parsed_data[uid_index][i] = newValue;
        }
    }
    set_column_width(parsed_data[uid_index]);

    cout << "UID " << uid << " updated." << endl
         << endl;
    return 1;
}

int CSVParser::delete_row(int uid)
{
    int uid_index = find_uid_index(uid);
    if (uid_index == -1)
    {
        cout << "Error: No record found for UID " << uid << " !!!" << endl;
        return 0;
    }
    parsed_data.erase(parsed_data.begin() + uid_index);

    cout << "UID " << uid << " removed." << endl;
    return 1;
}

void CSVParser::filter_by_column(string column_name, string value, bool ignore_case = false)
{

    int column_index = -1;
    for (int i = 0; i < columns_name.size(); ++i)
    {
        if (columns_name[i] == column_name)
        {
            column_index = i;
            break;
        }
    }

    if (column_index == -1)
    {
        cout << "Error: Invalid column Name!!!" << endl;
        return;
    }

    print_columns_name();
    int total = 0, matched = 0;
    for (auto &row : parsed_data)
    {
        string cell = row[column_index];
        if (ignore_case)
        {
            value = to_lower(value);
            cell = to_lower(cell);
        }

        if (cell == value)
        {
            print_row(row);
            ++matched;
        }
        ++total;
    }
    cout << "\nTotal matched: " << matched << endl;
    cout << "Total row: " << total << endl
         << endl;
}

void CSVParser::save_modified(string new_file, bool save_column = false)
{
    int delim = new_file.find('.');
    string extension = new_file.substr(delim + 1);
    extension = to_lower(extension);
    if (extension != "csv")
    {
        cout << "Warning: csv extension added !!!" << endl;
        new_file += ".csv";
    }
    fstream file;

    file.open(new_file, ios::out);

    if (!file.good())
    {
        cout << "Error: Cann't create file " << new_file << " !!!" << endl;
        return;
    }

    int last_index = parsed_data[0].size() - 1;

    if (save_column)
    {
        for (int i = 1; i < columns_name.size(); i++)
        {

            if (i == last_index)
            {
                file << columns_name[i] << endl;
            }
            else
            {
                file << columns_name[i] << ",";
            }
        }
    }

    for (auto &row : parsed_data)
    {
        for (int i = 1; i < row.size(); i++)
        {

            if (i == last_index)
            {
                file << row[i] << endl;
            }
            else
            {
                file << row[i] << ",";
            }
        }
    }
    file.close();

    cout << "Changes saved to " << new_file << endl;
}
