#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

const string NAME = "name";
const string OPENING_TIME = "openingTime";
const string CLOSING_TIME = "closingTime";
const string RANK = "rank";
const char COMMA_SEPARATE = ',';
const char COLON = ':';

struct place_of_interest{
    string name;
    int opening_hour;
    int opening_min;
    int closing_hour;
    int closing_min;
    int rank;
};

vector<string> reading_from_file(const string file_name){
    vector<string> rows;
    string line;
    ifstream input_file(file_name);
    while(getline(input_file, line)){
        stringstream ss(line);
        string cell;
        while(getline(ss, cell, COMMA_SEPARATE)){
            rows.push_back(cell);
        }

    }
    input_file.close();
    return rows;
}

vector<int> inputs_indexes(const vector<string> rows){
    vector<int> indexes;
    vector<string> dict = {NAME, OPENING_TIME, CLOSING_TIME, RANK};
    for(auto word : dict){
        auto it = find(rows.begin(), rows.end(), word);
        indexes.push_back(distance(rows.begin(), it));
    }
    return indexes;
}

int colon_index(const string str){
    int index;
    for(int i = 0; i < str.size(); i++){
        if(str[i] == COLON){
            index = i;
            break;
        }
    }
    return index;
}

vector<int> seperate_hour_min(const string str){
    int index_of_colon = colon_index(str);
    vector<int> result;
    result.push_back(stoi(str.substr(0, index_of_colon))); 
    result.push_back(stoi(str.substr(index_of_colon + 1, str.size()-1)));
    return result;
}

place_of_interest filling_single_place(const vector<string> rows, const vector<int> indexes,const int iterator){
    place_of_interest result;
    vector<int> opening_hour_mm = seperate_hour_min(rows[iterator + indexes[1]]);
    vector<int> closing_hour_mm = seperate_hour_min(rows[iterator + indexes[2]]);
    result.name = rows[iterator + indexes[0]];
    result.opening_hour = opening_hour_mm[0];
    result.opening_min = opening_hour_mm[1];
    result.closing_hour = closing_hour_mm[0];
    result.closing_min = closing_hour_mm[1];
    result.rank = stoi(rows[iterator + indexes[3]]);
    return result;
}

vector<place_of_interest> getting_places(vector<string> rows){
    vector<int> indexes = inputs_indexes(rows);
    vector<place_of_interest> places;
    for(int i = 4; i < rows.size(); i = i + 4){
        place_of_interest temp = filling_single_place(rows, indexes, i);
        places.push_back(temp);
    }
    return places;
}

int main(int argc, char*argv[]){
    vector<place_of_interest> places = getting_places(reading_from_file(argv[1]));
    return 0;
}