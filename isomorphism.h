//#ifndef isomorphism.h //jeœli ta biblioteka by³a ju¿ dodana to nie doda siê kolejny raz, wykonanie endif (pomija wszystko), sprawdza czy taka zmienna procesora ju¿ istnieje
//#define isomorphism.h //tworzy zmienn¹ procesora
#pragma once //da³oby radê, ale jest tylko w popularnych kompilatorach i nie jest standardem c++, a to co dopisa³am zadzia³a zawsze

#include <vector>
#include <map>
#include <string>
#include "graph_class.h"

bool isomorphism24(Graph& done, Graph& undone);//takes two graphs and check if they are isomorphic
bool neighbours_recursive(std::vector<std::vector<int>>&graph1, std::vector<std::vector<int>>&graph2, std::vector <int>& vertexes1, std::vector <int>& vertexes2, int vertex1, int vertex2);//check compatybility of two vertices
int used_vertices(std::vector<std::vector<int>>&graph);//how many vertices is used in graph
void multiplicity_of_degrees(std::vector<std::vector<int>>& graph, std::map<int, int>& map_of_degrees, std::vector<int>& keys);
void multiplicity_of_elements(std::vector<std::vector<int>>& graph, std::map<std::string, int>& map_of_degrees, std::vector<std::string>& keys);
//#endif