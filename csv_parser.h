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
