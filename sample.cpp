#include<iostream>
#include <dirent.h>
#include <fstream>
#include<cstring>
#include<string>
#include <sstream>
#include<vector>
#include <iomanip>
#include <algorithm>
#include <map>
#include <functional>

#define DB_FILE_EXTENSION ".db"
using namespace std;

//global vector
  string selectedDB;
  string DbMode = "normal";  //view only mode can't edit database
  vector<string> fieldArray;  //stores all the database fields
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

void initializeDB(string DbName) {
  int fieldsNum ;
  string fieldName;
  cout<<"Initializing " <<DbName<<"\n";
  cout<<"How many fields you want in your DataBase: "<<endl;
  cin>>fieldsNum;

  for(int i=0; i<fieldsNum;) {
    cout<<"Enter field: " << ++i<<endl;
    cin>>fieldName;
    fieldArray.push_back(fieldName);
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
            if (filename.length() > 3 && filename.substr(filename.length() - 3) == DB_FILE_EXTENSION) {
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
    cout<<"Enter database name: ";
    cin>>dbname;
    dbname+=DB_FILE_EXTENSION;

    ofstream file(dbname); 
    cout<<"Database "<<dbname<<" created successfully\n";
    selectedDB = dbname; //this db will get selected

    //will perform an operation

    initializeDB(selectedDB); 
    
}

void listCommands() {
  cout << left << setw(15) << "Command" << "Description" << endl;
  cout << setw(15) << "--------------" << "-----------------------------" << endl;
    // Display commands and descriptions
  cout << setw(15) << "createDatabase" << "To Create a database" << endl;
  cout << setw(15) << "showDbs" << "list all the created databases" << endl;
  cout << setw(15) << "useDatabase" << "List all Existing databases also select which one to use" << endl;
  cout << setw(15) << "checkmode" << "To Check the mode of Database" << endl;
  cout << setw(15) << "showDb" << "To Check the selected Database" << endl;
  cout << setw(15) << "query" << "To Enter in a Query (Editing mode)" << endl;
  cout << setw(15) << "normal" << "To Enter in a Normal (View only mode)" << endl;
  cout << setw(15) << "clear" << "To clear console" << endl << endl;
}

void listDatabase() {
  readDirectory();
}


//some issue here will fix
void useDatabase(){
  string userChoice;
  readDirectory();
  cout<< endl <<"Enter Your Choice: ";
  cin>>userChoice;
  string final = userChoice + DB_FILE_EXTENSION;
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


        //testingggggg
        // temp code testinggg (manuall process will make automatic)
        if(fieldArray.size() > 1) {
          if(fieldArray.size() == 2) {
            cout << fieldArray[0] << setw(10) << fieldArray[2] << endl;
        }

        else if(fieldArray.size() == 3) {
          cout << fieldArray[0] << setw(10) << fieldArray[2] <<fieldArray[3] << endl;
        } else {
          cout<< "Invalid number of fields"<< endl;
        }
        }

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

// helperFunction
void clearScreen(){
  system("cls"); 
}

static void repl() {
     map<string, function<void()>> cmdMap = {
      {"createDatabase", createDatabase},
      {"useDatabase", useDatabase},
      {"checkmode", []()-> void{cout<<"Database is in: " <<DbMode <<"\n";}},
      {"showDbs", listDatabase},
      {"showDb", []()-> void{cout <<"Database Selected: " <<selectedDB << "\n";}},
      {"query", []()->void{ DbMode="query"; listCommands(); }},
      {"help", listCommands},
      {"clear", clearScreen},
      {"exit", []()->void{exit(0);}}
    };

  string line;
  for (;;) {
    cout<<"DB >> ";
    cin >> line;
    if (line.empty()) {
      cout << endl;
        break;
      }
    
    auto cmdIterator = cmdMap.find(line);
    if(cmdIterator != cmdMap.end()) {
      cmdIterator->second();
    }  else {
      cout<< "Invalid Cmd given. Type <help> to see available cmds \n"<< endl;
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
