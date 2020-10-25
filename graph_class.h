//#ifndef graph_class.h
//#define graph_class.h
#pragma once
#include<vector>



class Graph {
public:
	std::vector<std::vector<int>> single_graph;
	unsigned int level;//potrzebuj� wiedzie� ile kraw�dzi by�o wstawianych, to jeden int a liczy� to trzebaby wiele razy bo troch� u�ywam, nie krzycz
	//void create_graph(std::vector<std::vector<int>>& single_graph);
	Graph() {

	}
	~Graph() {
		for (int i = 0; i < single_graph.size(); i++) {
			single_graph[i].~vector();
		}
		single_graph.~vector();
	}
};

//#endif