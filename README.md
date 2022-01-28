## CSV Parser

CSV parser based on c++. Read, update, delete, create a existing/new csv file.

```c++
#include <iostream>

#include "csv_parser.h"
using namespace std;

int main()
{
    string file_name = "students.csv";
    CSVParser parser(file_name);
    parser.print_all();
    return 0;
}
```

#### Why I created this library?
I created this library as a final project while I was teaching in coding club in my college to give my students more insight view about a library.

https://onkeshonkar.github.io/cpp-classes -- syllabus I covered while teaching.

## Table of Contents

1. [Installation](#installation)
2. [Features](#features)
3. [Prerequisites](#prerequisites)
4. [Examples](#examples)
  1. [Read file with default column name](#read-file-with-default-column-name)
  2. [Read file with custom column name](#read-file-with-custom-column-name)
  3. [Edit a column](#edit-a-column)
  4. [Add a new row](#add-a-new-row)
  5. [Delete a row](#delete-a-row)
  6. [Filter a column](#filter-a-column)
  7. [Export to a new file](#export-to-a-new-file)
  8. [Download demo app]

## Installation

This is a c++ module and can be made available by including the `csv_parher.h` header in the source file. Copy `csv_parser.h` and `input_handler.h` header files in the source directory.

C++ 11 or higher version is required.

```c++
#include "csv_parser.h"
```

## Features

- Parse and print a _csv_ file in tabular form
- Create a _csv_ file
- Modify a _csv_ file
- Export modified data to new _csv_ file

## Prerequisites

Should have basic knowledge in a few C++ STL. If need a recap click the links.

- [vector](https://onkesh.hashnode.dev/array-and-vector-in-cpp)
- [Lambda function](https://onkesh.hashnode.dev/function-pointer-and-lambda-function-in-cpp)
- [Hash map](https://onkesh.hashnode.dev/hashmap-in-cpp)

## Examples

Consider the _students.csv_ file for example.

```
Onkesh, Male, onkeshkumaronkar315@gmail.com
Jessica, Female, jessica@gmail.com
Emily, Female, emmy20@gmail.com
```

#### Read file with default column name

If column name is not given while creating the instance of CSVparser then it will make the first row of CSV file as a default column name.

```c++
#include <iostream>

#include "csv_parser.h"
using namespace std;

int main()
{
    string file_name = "students.csv";  // existig csv file in currnet dir.
    CSVParser parser(file_name);    // create an instance of CSVParser class and pass file name
    parser.print_all();
    return 0;
}
```

#### Read file with custom column name

While giving a custom column name, delete column names(first row) from the file if already exists.

```c++
#include <iostream>
#include <vector>

#include "csv_parser.h"
using namespace std;

int main()
{
    vector<string> columns = {"name", "gender", "gmail"};   //  Defining a vector of name for each column
    string file_name = "students.csv";
    CSVParser parser(file_name, columns);
    parser.print_all();

    return 0;
}
```

#### Edit a column

Configure CSVParser with a validator function for each column to modify it. Create a hash map of the input validator function corresponding to each column name. Few basic functions are defined by default to take input and validate.
**Modify a row by its uid, unique id generated while parsing**.
_Return type of each validator function must be a string._

```c++
#include <iostream>
#include <vector>
#include <unordered_map>

#include "csv_parser.h"
using namespace std;

int main()
{
    vector<string> columns = {"name", "gender", "gmail"};
    string file_name = "students.csv";

    unordered_map<string, string (*)()> inputValidators;    // map of column name and fun of string type.
    inputValidators["name"] = []()
    { return ask_name(30); };

    inputValidators["gender"] = &ask_gender;
    inputValidators["gmail"] = &ask_gmail;
    // default fun from csv parser -> ask_name, ask_gender, ask_gmail.

    CSVParser parser(file_name, columns);
    parser.get_validators(inputValidators); //  configuring validator s
    parser.print_all();

    int uid = 2;    // uid
    parser.update_row(uid);

    return 0;
}
```

#### Add a new row

Validator functions must be configured as above to be able to add a new row. It saves the new row to memory only not on the actual file. Need to save the changes explicitly.

```c++
parser.insert_row();    // adds a new row (in memory only).
parser.save_modified(file_name);    // save new change to file.
```

#### Delete a row

Delete a row by its _uid_. It deletes from memory only, not from the actual file. _So save the changes manually_.

```c++
int uid = 2;    // uid is unique row id, generated while parsing file.
parser.delete_row(uid);
```

#### Filter a column

Filter for any specific column to the corresponding value.

```c++
string column = "gender";
string search_for = "Female";
parser.filter_by_column(column, search_for, true);
// third arg to ignore case sensitivity
```

#### Export to a new file

If you want to export a file with a column name then pass the second args `true`.

```c++
string new_file = "modified.csv";
parser.save_modified(new_file, true);

//  save_modified(new_file); saves without column name.
```

## License

[MIT](LICENSE)
