#include <iostream>
#include <vector>
#include <map>
#include<set>
#include <algorithm>

#include "isomorphism.h"
#include "graph_class.h"
#include "utils.h"


bool neighbours_recursive(std::vector<std::vector<int>>&graph1, std::vector<std::vector<int>>&graph2, std::vector<int>&vertexes1, std::vector<int>&vertexes2, int vertex1, int vertex2) {
	std::vector<int> neighbours1, neighbours2;
	int multiplicity_of_vertex = 1;//krotnoœæ krawêdzi do danego s¹siada 
	int checked1 = 0;//przechowuje ile s¹siadów wierzcho³ka grafu1 by³o ju¿ wczeœniej sprawdzanych (ile razy continue bo s¹siad w vertexes1) //NEW

	//próba czy to dobre
	//if (vertexes1.end() == (std::find(vertexes1.begin(), vertexes1.end(), vertex1)))//je¿eli tego z którego startujemy nie ma jeszcze a liœcie rozpatzronych
	//	vertexes1.push_back(vertex1);
	//koniec próby
	
	for (int i = 0; i < graph1[vertex1].size(); i++) {//dla ka¿dego s¹siada wierzcho³ka grafu1
		if (vertexes1.end() != (std::find(vertexes1.begin(), vertexes1.end(), graph1[vertex1][i]))) {//je¿eli ten wierzcho³ek by³ ju¿ rozpatrzony
			checked1++;
			continue;
		}
		vertexes1.push_back(graph1[vertex1][i]); //dodanie do listy rozpatrywanych
		multiplicity_of_vertex = 1;//nowo rozpatrywany -> jedno wyst¹pienie ma napewno

		if (neighbours1.end() == (std::find(neighbours1.begin(), neighbours1.end(), graph1[vertex1][i]))) {//je¿eli nie ma takiego wierzcho³ka jeszcze wpisanego na listê s¹siadów1

			neighbours1.push_back(graph1[vertex1][i]);

			while (i != graph1[vertex1].size() - 1 && graph1[vertex1][i] == graph1[vertex1][i + 1]) {//jeœli to nie koniec listy i nastêpny wierzcho³ek to ten sam
				multiplicity_of_vertex++;
				i++;
			}

			int multiplicity_of_vertex2 = 1;//krotnoœæ wierzcho³ka w drugim grafie

			for (int j = 0; j < graph2[vertex2].size(); j++) {//dla ka¿dego s¹siada wierzcho³ka grafu2
				if (vertexes2.end() == (std::find(vertexes2.begin(), vertexes2.end(), graph2[vertex2][j]))){//¿eby nie dwa razy ten sam
					vertexes2.push_back(graph2[vertex2][j]); //dodanie do listy rozpatrywanych, musi byæ tu ¿eby w kolejnych wywo³aniach
															//rekurencji by³o widaæ ¿e ju¿ jest u¿yty do czegoœ
					multiplicity_of_vertex2 = 1;

					if (neighbours2.end() == (std::find(neighbours2.begin(), neighbours2.end(), graph2[vertex2][j]))) {//je¿eli nie ma takiego wierzcho³ka jeszcze wpisanego na listê s¹siadów1

						neighbours2.push_back(graph2[vertex2][j]);

						while (j != graph2[vertex2].size() - 1 && graph2[vertex2][j] == graph2[vertex2][j + 1]) {//jeœli to nie koniec listy i nastêpny wierzcho³ek to ten sam
							multiplicity_of_vertex2++;
							j++;
						}
						if (multiplicity_of_vertex == multiplicity_of_vertex2 && graph1[vertex1].size() == graph2[vertex2].size() && assign_element(graph1[vertex1][i]) == assign_element(graph2[vertex2][j])) {
							if (neighbours_recursive(graph1, graph2, vertexes1, vertexes2, graph1[vertex1][i], graph2[vertex2][j])) {
								break;//wierzcho³ek 2 grafu dopasowany do wierzcho³ka z pierwszego
							}
							else if (j == graph2[vertex2].size() - 1) {
								if (vertexes2.size() > 0) {
									vertexes2.pop_back();
									neighbours2.pop_back();//NEW
								}
								return false;//rozpatrzamy go tutaj, ale skoro siê nie dopasowa³ to ¿eby móg³ z innym v1 siê dopasowaæ to trzeba go zdjac z vec
							}
							else {
								if (vertexes2.size() > 0) {
									vertexes2.pop_back();//bo niedopasowane
									vertexes1.pop_back();//by³ rozpatrzany, ale nic siê do niego nei dopasowa³o, wiêc musi mieæ mo¿liwoœæ jeszcze raz próbowaæ sie dopasowaæ//NEW
									neighbours2.pop_back();//NEW
								}
							}
						}
						else if (j == graph2[vertex2].size() - 1) {
							/*vertexes1.pop_back();
							vertexes2.pop_back();*/
							if (vertexes2.size() > 0) {
								vertexes2.pop_back();//bo niedopasowane
								neighbours2.pop_back();//NEW
							}
							return false;
						}
						else {
							if (vertexes2.size() > 0) {
								vertexes2.pop_back();//bo niedopasowane
								neighbours2.pop_back();//NEW
							}
						}
					}
					else if (j == graph2[vertex2].size() - 1) {//jeœli ¿aden z s¹siadów2 nie dopasowa³ siê do s¹siada1
							/*vertexes1.pop_back();
							vertexes2.pop_back();*/
						if(vertexes2.size() > 0)
							vertexes2.pop_back();
						return false;
					}
					else {
						if (vertexes2.size() > 0)
							vertexes2.pop_back();
					}
				}				
				else if (j == graph2[vertex2].size() - 1) {//jeœli ostatni s¹siad ju¿ by³ rozpatrywany i nic sie nie dopasowa³o
					return false;
				}
			}
		}
	}
	if (checked1 == graph1[vertex1].size()) {//skoro wszyscy s¹siedzi wierzcho³ka grafu1 byli ju¿ sprawdzeni, to trzeba sprawdziæ czy w grafie2 te¿ jest tak samo//NEW
		if (checked1 == graph2[vertex2].size()) {
			for (int j = 0; j < graph2[vertex2].size(); j++) {//dla ka¿dego s¹siada wierzcho³ka grafu2
				if (vertexes2.end() == (std::find(vertexes2.begin(), vertexes2.end(), graph2[vertex2][j]))) {
					return false;//skoro wszyscy s¹siedzi wg1 byli sprawdzenie to czemu nie byli wszyscy wg2?! grafy nie s¹ takie same!
				}
			}
		}
		else {
			return false;
		}
	}
	return true; 
}

int used_vertices(std::vector<std::vector<int>>&graph) {
	/*int used_vertices = 0;
	for (int i = 0; i < graph.size(); i++) {
		if (graph.size() > 0) {
			used_vertices++;
		}
	}
	return used_vertices; */
	int used_vertices = 0;
	std::set<int> set1;
	for (int i = 0; i < graph.size(); i++) {
		set1.insert(graph[i][0]);
		set1.insert(graph[i][1]);
	}
	return set1.size();
}


void multiplicity_of_elements(std::vector<std::vector<int>> &graph, std::map<std::string, int> &map_of_degrees, std::vector<std::string> &keys) {
	for (int i = 0; i < graph.size(); i++) {
		///poprawnoœæ przekazywanych parametrów
		if (graph.size() > 0) {//jeœli wierzcho³ek jest po³¹czony w jaki kolwiek sposób z grafem
			std::string element = assign_element(i);
			if (keys.end() == (std::find(keys.begin(), keys.end(), element))){
				map_of_degrees[element] = 1;
				//map_of_degrees.insert(std::make_pair(element, 1));
				keys.push_back(element);
			}
			else {
				map_of_degrees[element]++;
			}
		}
	}
}

void multiplicity_of_degrees(std::vector<std::vector<int>> &graph, std::map<int,int>&map_of_degrees, std::vector<int>&keys) {
	for (int i = 0; i < graph.size(); i++) {
		if (graph[i].size() != 0) {//jeœli wierzcho³ek ma wgl s¹siadów, bo inaczej go traktujemy jako nie istniej¹cy jeszcze w grafie
			if (keys.end() == (std::find(keys.begin(), keys.end(), graph[i].size()))) {//jeœli w kluczach nie ma jeszcze zadanego stopnia
				map_of_degrees[graph[i].size()] = 1;//dodaj taki do mapy
				keys.push_back(graph[i].size());
			}
			else {
				map_of_degrees[graph[i].size()]++;//inkrementuj ju¿ istniej¹cy
			}
		}
	}
}

bool isomorphism24(Graph& done, Graph& undone) {
 	std::vector<int> vertexes1, vertexes2;
	std::vector<std::vector<int>> graph_lvl = obtain_level(done.single_graph, undone.level);

		if (used_vertices(graph_lvl) == used_vertices(undone.single_graph)) {//jeœli oba grafy maj¹ tyle samo wierzcho³ków (przy wielokrotnych mo¿e byæ inna liczba
		//if (true) {
			std::map <std::string, int> elements_usage1;
			std::vector<std::string>keys_elements1;
			std::map <std::string, int> elements_usage2;
			std::vector<std::string>keys_elements2;

			
			std::vector<std::vector<int>> graph_pom = transform_edges_to_neighbours(graph_lvl);
			multiplicity_of_elements(graph_pom, elements_usage1, keys_elements1);
			graph_pom = transform_edges_to_neighbours(undone.single_graph);
			multiplicity_of_elements(graph_pom, elements_usage2, keys_elements2);
			//delete &graph_pom;

			if (keys_elements1.size() == keys_elements2.size()) {//jeœli maj¹ równ¹ liczbê rodzai pierwiastków
				for (int i = 0; i < keys_elements1.size(); i++) {
					if (elements_usage2.find(keys_elements1[i]) == elements_usage2.end())//jeœli pierwiastka z pierwszego grafu nie ma w drugim
						return false;
					if (elements_usage1[keys_elements1[i]] != elements_usage1[keys_elements1[i]])//jeœli nie zgadzaj¹ siê liczebnoœci pierwiastków w grafach
						return false;
				}
				std::map < int, int > degrees_usage1;
				std::vector<int>keys_degrees1;
				std::map < int, int > degrees_usage2;
				std::vector<int>keys_degrees2;

				graph_pom = transform_edges_to_neighbours(graph_lvl);
				multiplicity_of_degrees(graph_pom, degrees_usage1, keys_degrees1);
				graph_pom = transform_edges_to_neighbours(undone.single_graph);
				multiplicity_of_degrees(graph_pom, degrees_usage2, keys_degrees2);
				//delete &graph_pom;

				if (keys_degrees1.size() == keys_degrees2.size() && keys_degrees1.size() > 0) {//jeœli taka sama iloœæ ró¿nych stopni
					//int min = keys_degrees1.size();//krotnoœæ stopnia
					//int minv = keys_degrees1.size();//stopieñ
					int min = degrees_usage1[keys_degrees1[0]];
					int minv = keys_degrees1[0];
					for (int i = 0; i < keys_degrees1.size(); i++) {
						if (min > degrees_usage1[keys_degrees1[i]]) {
							min = degrees_usage1[keys_degrees1[i]];
							minv = keys_degrees1[i];
						}
						if (degrees_usage2.find(keys_degrees1[i]) == degrees_usage2.end())//jeœli pierwiastka z pierwszego grafu nie ma w drugim
							return false;
						if (degrees_usage1[keys_degrees1[i]] != degrees_usage2[keys_degrees1[i]])//jeœli nie zgadzaj¹ siê liczebnoœci pierwiastków w grafach
							return false;
					}
					int v1; //przechowuje wierzcho³ek startowy pierwszego grafu

					graph_pom = transform_edges_to_neighbours(graph_lvl);
					for (int i = 0; i < graph_pom.size(); i++) {
						if (graph_pom[i].size() == minv) {
							v1 = i;
							break;
						}
					}
					graph_pom = transform_edges_to_neighbours(undone.single_graph);
					for (int i = 0; i < graph_pom.size(); i++) {
						if (graph_pom[i].size() == minv) {//wierzcho³ki na tym samym st
							if (assign_element(v1) == assign_element(i)){//te same pierwiastki
								std::vector<std::vector<int>> done_pom = transform_edges_to_neighbours(graph_lvl);								
								std::vector<std::vector<int>> undone_pom = transform_edges_to_neighbours(undone.single_graph);
								vertexes1.push_back(v1); vertexes2.push_back(i);
								if (neighbours_recursive(done_pom, undone_pom, vertexes1, vertexes2, v1, i))//i bo index rep wierzcho³ek to którek jest przyp vector s¹siadów
									return true;//jeœli rekurencyjna zwróci³a true
								vertexes1.clear();//¿eby na nowo mog³o siê liczyæ dla kolejnej pary
								vertexes2.clear();
							}
						}
					}
				}
			}
		}
	return false;
}
