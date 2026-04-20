#pragma once
#include <string>
#include <vector>

class Map{

protected:
    int h;
    int w;
    bool isHeightMap;
    std::vector<std::vector<int>> _map;

public:
    Map();                              //constructs empty map
    Map(std::string filename);          //constructs from data in a file
    Map(const Map& rhs);                //copy constructor
    ~Map();                             //destructor
    Map& operator=(const Map& rhs);     //assignment operator
   
    void print() const;                 //prints map
    void print(std::vector<std::pair<int,int>>) const; //prints map and path 
    int getH() const{return h;}
    int getW() const{return w;}
    bool getIsHeightMap() const{return isHeightMap;}
    friend bool operator==(const Map& lhs, const Map& rhs); //declares global operator friend 
    friend class Search;                //to access protected members. Otherwise, need accessors.
};

bool operator==(const Map& lhs, const Map& rhs);    //returns true if both maps are equivalent