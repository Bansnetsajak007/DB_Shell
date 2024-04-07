#include<iostream>
#include <dirent.h>
#include <fstream>
#include<cstring>
#include<string>
#include <sstream>
#include<vector>
#include <iomanip>
#include <algorithm>

using namespace std;

//global vector
  string selectedDB;
  string DbMode = "normal";  //view only mode can't edit database
  vector<string> globalDbs;
  vector<string> useDB;

// used to read file in tabular format
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

string removeQuotes(const string& s) {
    if (s.size() >= 2 && s.front() == '\'' && s.back() == '\'') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

void copy(const vector<string>& source, vector<string>& destination) {
    destination.clear();

    for (const auto& str : source) {
        destination.push_back(str);
    }
}

void readDirectory(){
    string directory = "."; // Current directory
    DIR *dir;
    struct dirent *entry;
    vector<string> dbs;

    if ((dir = opendir(directory.c_str())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string filename = entry->d_name;

            if (filename == "." || filename == "..") {
              continue;
            }
            if (filename.length() > 3 && filename.substr(filename.length() - 3) == ".db") {
                dbs.push_back(filename); // Store filename in dbs vector
                copy(dbs , globalDbs);
            }
        }
        closedir(dir);
    } else {
        cerr << "Failed to open directory.\n";
    }

    cout << "All the Created Databases:\n";
    for (const auto& file : dbs) {
        cout << file <<"\n";
    }

};

void createDatabase() {
    string dbname;
    string fileFormat = ".db";
    cout<<"Enter database name: ";
    cin>>dbname;
    ofstream file(dbname + fileFormat); 
    cout<<"Database "<<dbname <<" created successfully\n";
}

void listCommands() {
  cout<<"createDatabase\t----->" << "To Create a database\n";
  cout<<"showDbs\t----->" << "list all the created databases\n";
  cout<<"useDatabase\t----->" << "List all Existing databases also select which one to use\n";
  cout<<"checkmode\t----->" << "To Check the mode of Database \n";
  cout<<"showDb\t----->" << "To Check the selected Database \n";
  cout<<"query\t----->" << "To Enter in a Query (Editing mode)\n";
  cout<<"normal\t----->" << "To Enter in a Normal (View only mode)\n";
}

void listDatabase() {
  readDirectory();
}


//some issue here will fix
void useDatabase(){
  string userChoice;
  readDirectory();
  cout<<"\n";
  cout<<"Enter Your Choice: ";
  cin>>userChoice;
  string final = userChoice + ".db";
  auto it = find(globalDbs.begin(), globalDbs.end(), final);
    if (it != globalDbs.end()) {
        selectedDB = final;
        cout <<"Database Selected: " <<selectedDB << "\n";
        //yeta
        useDB.push_back(final);
        ifstream input(final);
        /* if in normal mode, show the contents of database in tabular format(just show DB in tabular format)
            basic file handeling concepts 
        */

        if(! input.is_open()) {
          cout<<"Error opening database file"<<endl;
        }

        cout << "ID" << setw(10) << "Name" << setw(10) << "Age" << setw(10) << "Grade" << endl;
        cout << "---------------------------------------------" << endl;

      string line;
    while (getline(input, line)) {
        vector<string> fields = split(line, ',');

        if (fields.size() == 4) {

            cout << fields[0].substr(1) << setw(10) << fields[1] << setw(10) << fields[2] << setw(10) << fields[3].substr(0,1) << endl;
        }
    }
      input.close();

    } else {
      cout << "Database Doesnot exist!.\n";
    }
}

static void repl() {
  string line;
  for (;;) {
    cout<<"DB >> ";

    if (!getline(cin, line)) {
      cout << endl;
        break;
      }

    //command processor

    if(line == "createDatabase") {
      createDatabase();
    }

    if(line == "useDatabase") {
      useDatabase();
    }

    if(line == "checkmode") {
      cout<<"Database is in: " <<DbMode <<"\n";
    }

    if(line == "showDbs") {
      listDatabase();
    }

    if(line == "showDb") {
      cout <<"Database Selected: " <<selectedDB << "\n";
    }

    if(line == "help") {
      listCommands();
    }

    if(line == "query") {
      DbMode = "query";
      listCommands();
    }

    if(line == "exit") {
        break;
    }

  }
}

int main() {
    if(selectedDB.empty()) {
      cout <<"Database Selected: " << "None\n";
    } else {
      cout <<"Database Selected: " <<selectedDB << "\n";
    }
    cout<<"Database Mode: " << DbMode << "\n";
    cout<<'\n';

    repl();
    return 0;
}
