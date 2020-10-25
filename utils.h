#pragma once
#include <vector>
#include <memory>


std::vector<std::vector<int>> obtain_level(std::vector<std::vector<int>>& graph, int lvl);

std::vector<std::vector<int>> transform(std::vector<std::vector<int>>& neighbors_list);

bool chech_graph_consistency(std::vector<std::vector<int>>& graph);

std::string assign_element(int number);

std::vector<std::vector<int>> transform_edges_to_neighbours(std::vector<std::vector<int>>& edges);

std::vector<std::string> create_elements_table();

int check_rules(std::vector<std::vector<int>>& neighbours);

extern std::vector<std::string> ELEMENTS_TABLE;

extern std::vector<std::string> ELEMENTS;
extern std::vector<int> QUANTITY;

///////BA£AGAN BAR///////////
void MemoryUsage();
void* operator new(size_t size);
void operator delete(void* memory, size_t size);
