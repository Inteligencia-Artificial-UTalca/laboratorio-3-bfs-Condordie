#include "Search.h"
#include <queue>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <limits>


namespace std
{
    //this is needed to store a pair<int,int> in an associative container
    //such as unorered_set and unordered_map
	template<> struct hash<std::pair<int,int>>
	{
		std::size_t operator()( const std::pair<int,int> & p) const noexcept
		{
            hash<int> hasher;
			return hasher(p.first) ^ (hasher(p.second)<<1);

		}
	};
}
struct CompareHeuristic{
    std::pair<int,int> goal;
    CompareHeuristic(std::pair<int,int> goal): goal(goal){}
    bool operator()(const std::pair<int,int>& a, const std::pair<int,int>& b ) const {
        float ha =std::abs(a.first-goal.first) + std::abs(a.second-goal.second);
        float hb =std::abs(b.first-goal.first) + std::abs(b.second-goal.second);
        return ha >hb;
    }
};

std::vector<std::pair<int,int>> Search::reconstruct(const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache, const std::pair<int,int> &start){
	std::deque<std::pair<int,int>> nodes;
	auto node = start;//make copy

    //traverse path from goal to start

    //
	while(true){
        nodes.push_front(node);
        auto it = pathCache.find(node);
        if(it==pathCache.end() || it->second==node) break;
        node = it->second;
	}

    //revert path and return it
    std::vector<std::pair<int,int>> vec;
    for(auto p:nodes){
        vec.push_back(p);
    }
    return vec;
}

std::vector<std::pair<int,int>> Search::BFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal){
    std::cout<<"===========================\nRunning BFS...\n";
	auto startTime = std::chrono::high_resolution_clock::now();

    //stores possible directions
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    bool visited[map.h][map.w];      //we'll just use a matrix og booleans to indicated if visited
    for(int i=0; i<map.h;i++){
        for(int j=0;j<map.w;j++){
            visited[i][j]=false;
        }
    }
    std::queue<std::pair<int,int>> OPEN;
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;    ////hashmap to reconstruct path: child -> parent

    //add firts node to open list
    OPEN.push(start);
    visited[start.first][start.second]=true;
    pathCache[start]= start;
    while(!OPEN.empty()){
        //get node
        auto pos = OPEN.front();
        OPEN.pop();
        //check if node is goal
		if(pos==goal){
			auto endTime = std::chrono::high_resolution_clock::now();
			int count=0;
            for(int i=0;i<map.h;i++){
                for(int j=0;j<map.w;j++){
                    if(visited[i][j])count++;
                }
            }
            std::cout<<"VISITED: "<<count<<std::endl;
			std::cout<<"OPEN: "<<OPEN.size()<<std::endl;
			std::cout<<"FOUND in "<<(endTime-startTime).count()/1000000.0<<"ms\n";
			return reconstruct(pathCache,pos);
		}

		for(auto dir:dirs){
			//copy the position
            auto child =pos;
            //then move it
            child.first += dir.first;
            child.second+= dir.second;
            //if illegal or visited, skip it
            if(child.first <0 || child.first >=map.h || child.second < 0 || child.second >= map.w){
                continue;
            }
            if(map._map[child.first][child.second]==1||visited[child.first][child.second]){
                continue;
            }
            //add child to open list
            OPEN.push(child);
            visited[child.first][child.second]=true;
            //register path
            pathCache[child]=pos;
		}
	}
	std::cout<<"NOT FOUND!!!!\n";
    
    //let's just return start and goal to draw them
    std::vector<std::pair<int,int>> path;
    path.push_back(start);
    path.push_back(goal);
    return path;
}
float Search::Heuristic(std::pair<int,int> start,std::pair<int,int> goal){
    return std::abs(start.first -goal.first) + std::abs(start.second - goal.second);
}
std::vector<std::pair<int,int>> Search::Greedy(const Map& map, std::pair<int,int> start,std::pair<int,int> goal){
    std::cout <<"==================================\nRunning Greedy...\n";
    auto startTime= std::chrono::high_resolution_clock::now();
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};
    
    bool visited[map.h][map.w];
    for(int i=0;i<map.h;i++){
        for(int j=0;j<map.w;j++){
            visited[i][j]=false;
        }
    }
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>,CompareHeuristic>OPEN{CompareHeuristic(goal)};
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;
    OPEN.push(start);
    visited[start.first][start.second]=true;
    pathCache[start]=start;
    while(!OPEN.empty()){
        auto pos = OPEN.top();
        OPEN.pop();
        if(pos==goal){
            auto endTime = std::chrono::high_resolution_clock::now();
            int count =0;
            for( int i =0; i<map.h;i++){
                for(int j=0;j<map.w;j++){
                    if(visited[i][j]) count ++;
                }
            }
            std::cout<<"Visited: "<<count<<std::endl;
            std::cout<<"OPEN: "<<OPEN.size()<<std::endl;
            std::cout<<"FOUND in"<<(endTime-startTime).count()/1000000.0<<"ms\n";
            return reconstruct(pathCache,pos);
        }
        for(auto dir:dirs){
            auto child = pos;
            child.first += dir.first;
            child.second+= dir.second;

            if(child.first<0|| child.first>=map.h||child.second<0||child.second>=map.w) continue;
            if(map._map[child.first][child.second]==1||visited[child.first][child.second])continue;

            OPEN.push(child);
            visited[child.first][child.second]=true;
            pathCache[child]=pos;
        }
    }
    std::cout<<"NOT FOUND!!!!\n";
    std::vector<std::pair<int,int>>path;
    path.push_back(start);
    path.push_back(goal);
    return path;
}
std::vector<std::pair<int,int>> Search::Astar(const Map& map, std::pair<int,int> start, std::pair<int,int> goal){
    std::cout<<"============================\nRunning A*...\n";
    auto startTime= std::chrono::high_resolution_clock::now();
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};
    //Guardar el coste de cada Nodo
    std::unordered_map<std::pair<int,int>,float> gCost;
    gCost[start]=0;
    //Nodos por ecplorar | Nodos ya explorados 'close'
    std::unordered_map<std::pair<int,int>, std::pair<int,int>> pathCache;
    std::unordered_map<std::pair<int,int>, bool> CLOSED;
    return{};
}