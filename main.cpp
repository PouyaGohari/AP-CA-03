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
const int ONE_HOUR = 60;
const int QUARTAR_HOUR = 15;
const int HALF_HOUR = 30;

struct place_of_interest{
    string name;
    int opening_hour;
    int opening_min;
    int closing_hour;
    int closing_min;
    int rank;
};

struct visited_place{
    string name;
    vector<int> reach_time;
    vector<int> left_time;
    visited_place(string n, vector<int> time1, vector<int> time2) : name(n) , reach_time(time1), 
    left_time(time2) {};
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

vector<place_of_interest> getting_places(const vector<string> rows){
    vector<int> indexes = inputs_indexes(rows);
    vector<place_of_interest> places;
    for(int i = 4; i < rows.size(); i = i + 4){
        place_of_interest temp = filling_single_place(rows, indexes, i);
        places.push_back(temp);
    }
    return places;
}

vector<int> time_subtraction(const vector<int> time1,const vector<int> time2){
    int hour_distance = (time1[0] - time2[0]) * ONE_HOUR;
    int min_distance = hour_distance + time1[1] - time2[1];
    return {min_distance / 60, min_distance % 60};
}

vector<int> first_place_time(const vector<place_of_interest> places){
    vector<int> result = {places[0].opening_hour, places[0].opening_min};
    for(auto place : places){
        if(result[0] * ONE_HOUR + result[1] > place.opening_hour * ONE_HOUR + place.opening_min){
            result = {place.opening_hour, place.opening_min};
        }
    } 
    return result;
}

bool comparator(const place_of_interest place1, const place_of_interest place2, const vector<int> time){
    vector<int> sub1 = time_subtraction(time, {place1.opening_hour, place1.opening_min});
    vector<int> sub2 = time_subtraction(time, {place2.opening_hour, place2.opening_min});
    int x = sub1[0] * ONE_HOUR + sub1[1];
    int y = sub2[0] * ONE_HOUR + sub2[1];
    if(x >= 0 && y >= 0) return (place1.rank < place2.rank);
    if(x >= 0 && y < 0) return true;
    if(x < 0 && y >= 0) return false;
    if(x < 0 && y < 0) return false; 
    return false;
}

void sorting_places(vector<place_of_interest>& places, const vector<int> time){
    sort(places.begin(), places.end(), [time](const place_of_interest p1, const place_of_interest p2){
        return comparator(p1, p2, time);
    });
}

vector<place_of_interest> sort_opening_time(vector<place_of_interest> places){
    vector<place_of_interest> sorted_time = places;
    sort(sorted_time.begin(), sorted_time.end(), [](const place_of_interest p1, const place_of_interest p2){
        return (p1.opening_hour * ONE_HOUR + p1.opening_min < p2.opening_hour * ONE_HOUR + p2.opening_min);
    });
    return sorted_time;
}

vector<int> time_adding(const vector<int> now, const int adding_time){
    int result = now[0] * ONE_HOUR + now[1] + adding_time;
    return {result / 60, result % 60};
}

int time_spending(const vector<int> subtracted){
    int time_spent = subtracted[0] * ONE_HOUR + subtracted[1];
    if(time_spent > ONE_HOUR){
        return ONE_HOUR;
    }
    return time_spent;
}

bool worth_to_visit(const vector<int> subtracted,const vector<int> now){
    if(subtracted[0] * ONE_HOUR + subtracted[1] < QUARTAR_HOUR + HALF_HOUR){
        return false;
    }
    return true;
}

bool check_duplicate_visiting(vector<visited_place> visiting_places, const string name){
    for(auto visit_place : visiting_places){
        if(visit_place.name == name) return true;
    }
    return false;
}

visited_place visit_push_back(const place_of_interest place, vector<int>& now, const int iterator,const vector<place_of_interest> sorted_time, const vector<visited_place> vistied_places){
    vector<int> subtracted = time_subtraction({place.closing_hour, place.closing_min}, now);
    vector<int> spended_time = time_adding(now, time_spending(subtracted));
    if(iterator == 0){
        if(now[0] * ONE_HOUR + now[1] == place.opening_hour * ONE_HOUR + place.opening_min){
            visited_place result(place.name, now, spended_time);
            now = spended_time;
            return result;
        }
        if(now[0] * ONE_HOUR + now[1] + HALF_HOUR > place.opening_hour * ONE_HOUR + place.opening_min){
            now = time_adding(now, HALF_HOUR);
            spended_time = time_adding(now, time_spending(time_subtraction(subtracted, {0, HALF_HOUR})));
            visited_place result(place.name, now, spended_time);
            now = spended_time;
            return result;
        }
        else{
            now = {place.opening_hour, place.opening_min};
            subtracted = time_subtraction({place.closing_hour, place.closing_min}, now);
            spended_time = time_adding(now, time_spending(subtracted));
            visited_place result(place.name, now, spended_time);
            now = spended_time;
            return result;
        }
    }
    else if(worth_to_visit(subtracted, now)){
        if(now[0] * ONE_HOUR + now[1] < place.opening_hour * ONE_HOUR + place.opening_min){
            for(auto it = sorted_time.begin(); it != sorted_time.end();){
                if(!check_duplicate_visiting(vistied_places, it -> name) && worth_to_visit(subtracted, now)){
                    return visit_push_back(*it, now, 0, sorted_time, vistied_places);
                }
                else{
                    ++it;
                }
            }
        }
        else{
                now = time_adding(now, HALF_HOUR);
                spended_time = time_adding(now, time_spending(time_subtraction(subtracted, {0, HALF_HOUR})));
                visited_place result(place.name, now, spended_time);
                now = spended_time;
                return result;
        } 
    }
    return {"", {0, 0}, {0, 0}};
}

vector<visited_place> place_to_visit(vector<place_of_interest>& places){
    vector<int> now = first_place_time(places);
    vector<visited_place> visited_places;
    sorting_places(places, now);
    visited_places.push_back(visit_push_back(*(places.begin()), now, 0, {}, {}));
    for(auto it = places.begin(); it != places.end();){
        sorting_places(places, now);
        vector<place_of_interest> time_sorted = sort_opening_time(places);
        if(!check_duplicate_visiting(visited_places, it -> name)){
            visited_place temp = visit_push_back(*it, now, 1, time_sorted, visited_places);
            if(temp.name.size() != 0){
                visited_places.push_back(temp);
                it = places.begin();
            }
            else{
                ++it;
            }
        }
        else{
            ++it;
        }
    }
    return visited_places;
}

int main(int argc, char*argv[]){
    vector<place_of_interest> places = getting_places(reading_from_file(argv[1]));
    vector<visited_place> visiting_places = place_to_visit(places);
    return 0;
}