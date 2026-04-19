#include "Map.h"
#include "Search.h"
#include "ColorMap.h"
#include <cassert>
#include <iostream>

bool isNumber(const std::string& s){
    for(char c: s){
        if(!isdigit(c)) return false;
    }
    return !s.empty();
}
int main(int argc, char *argv[]){

    //Verify that the amount of arguments is correct
    //6 arguments: program name, map file, x1, y1, x2, y2
    if(argc !=6){
        std::cerr <<"Error | Se espera 6 argumentos"<<std::endl;
        return 1;
    }

    for(int i=2;i<=5; i++){
        if(!isNumber(argv[i])){
            std::cerr <<"Error | el "<<i<<" debe ser numero entero"<<std::endl;
            return 1;
        }
    }
    //Load map with class Map
    Map map(argv[1]);
    ColorMap colorMap(map);
    colorMap.print();

    int x1=atoi(argv[2]),x2=atoi(argv[4]);
    int y1=atoi(argv[3]),y2=atoi(argv[5]);

    if(x1 <0 || x1 >=map.getH()|| y1 <0||y1 >=map.getW()|| x2 <0 || x2 >=map.getH()|| y2 <0||y2 >=map.getW()){
        std::cerr<<"Error:coordenadas fuera del amapa"<<std::endl;
        return 1;
    }
    // Forma BFS
    auto path = Search::BFS(map,{atoi(argv[2]),atoi(argv[3])},{atoi(argv[4]),atoi(argv[5])}); 
    //Calculate path distance
    //Print path distance
    std::cout<<"Dist: "<<path.size() - 1<<std::endl;
    colorMap.print(path);
    // Forma Greedy
    auto pathGreedy = Search::Greedy(map,{x1,y1},{x2,y2});
    std:: cout <<"Dist: "<<pathGreedy.size()-1<< std::endl;
    colorMap.print(pathGreedy);
    // Forma Astar
    auto pathAstar = Search::Astar(map,{x1,y1},{x2,y2});
    std:: cout<<"Dist: "<<pathAstar.size() - 1<<std::endl;
    colorMap.print(pathAstar);
    //Forma WAstar
    auto pathWAstar = Search::WAstar(map,{x1,y1},{x2,y2},1.5f);
    std::cout<<"Dist: "<<pathWAstar.size()-1<<std::endl;
    colorMap.print(pathWAstar);

    return 0;
}