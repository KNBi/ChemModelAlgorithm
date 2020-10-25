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
	int multiplicity_of_vertex = 1;//krotno�� kraw�dzi do danego s�siada 
	int checked1 = 0;//przechowuje ile s�siad�w wierzcho�ka grafu1 by�o ju� wcze�niej sprawdzanych (ile razy continue bo s�siad w vertexes1) //NEW

	//pr�ba czy to dobre
	//if (vertexes1.end() == (std::find(vertexes1.begin(), vertexes1.end(), vertex1)))//je�eli tego z kt�rego startujemy nie ma jeszcze a li�cie rozpatzronych
	//	vertexes1.push_back(vertex1);
	//koniec pr�by
	
	for (int i = 0; i < graph1[vertex1].size(); i++) {//dla ka�dego s�siada wierzcho�ka grafu1
		if (vertexes1.end() != (std::find(vertexes1.begin(), vertexes1.end(), graph1[vertex1][i]))) {//je�eli ten wierzcho�ek by� ju� rozpatrzony
			checked1++;
			continue;
		}
		vertexes1.push_back(graph1[vertex1][i]); //dodanie do listy rozpatrywanych
		multiplicity_of_vertex = 1;//nowo rozpatrywany -> jedno wyst�pienie ma napewno

		if (neighbours1.end() == (std::find(neighbours1.begin(), neighbours1.end(), graph1[vertex1][i]))) {//je�eli nie ma takiego wierzcho�ka jeszcze wpisanego na list� s�siad�w1

			neighbours1.push_back(graph1[vertex1][i]);

			while (i != graph1[vertex1].size() - 1 && graph1[vertex1][i] == graph1[vertex1][i + 1]) {//je�li to nie koniec listy i nast�pny wierzcho�ek to ten sam
				multiplicity_of_vertex++;
				i++;
			}

			int multiplicity_of_vertex2 = 1;//krotno�� wierzcho�ka w drugim grafie

			for (int j = 0; j < graph2[vertex2].size(); j++) {//dla ka�dego s�siada wierzcho�ka grafu2
				if (vertexes2.end() == (std::find(vertexes2.begin(), vertexes2.end(), graph2[vertex2][j]))){//�eby nie dwa razy ten sam
					vertexes2.push_back(graph2[vertex2][j]); //dodanie do listy rozpatrywanych, musi by� tu �eby w kolejnych wywo�aniach
															//rekurencji by�o wida� �e ju� jest u�yty do czego�
					multiplicity_of_vertex2 = 1;

					if (neighbours2.end() == (std::find(neighbours2.begin(), neighbours2.end(), graph2[vertex2][j]))) {//je�eli nie ma takiego wierzcho�ka jeszcze wpisanego na list� s�siad�w1

						neighbours2.push_back(graph2[vertex2][j]);

						while (j != graph2[vertex2].size() - 1 && graph2[vertex2][j] == graph2[vertex2][j + 1]) {//je�li to nie koniec listy i nast�pny wierzcho�ek to ten sam
							multiplicity_of_vertex2++;
							j++;
						}
						if (multiplicity_of_vertex == multiplicity_of_vertex2 && graph1[vertex1].size() == graph2[vertex2].size() && assign_element(graph1[vertex1][i]) == assign_element(graph2[vertex2][j])) {
							if (neighbours_recursive(graph1, graph2, vertexes1, vertexes2, graph1[vertex1][i], graph2[vertex2][j])) {
								break;//wierzcho�ek 2 grafu dopasowany do wierzcho�ka z pierwszego
							}
							else if (j == graph2[vertex2].size() - 1) {
								if (vertexes2.size() > 0) {
									vertexes2.pop_back();
									neighbours2.pop_back();//NEW
								}
								return false;//rozpatrzamy go tutaj, ale skoro si� nie dopasowa� to �eby m�g� z innym v1 si� dopasowa� to trzeba go zdjac z vec
							}
							else {
								if (vertexes2.size() > 0) {
									vertexes2.pop_back();//bo niedopasowane
									vertexes1.pop_back();//by� rozpatrzany, ale nic si� do niego nei dopasowa�o, wi�c musi mie� mo�liwo�� jeszcze raz pr�bowa� sie dopasowa�//NEW
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
					else if (j == graph2[vertex2].size() - 1) {//je�li �aden z s�siad�w2 nie dopasowa� si� do s�siada1
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
				else if (j == graph2[vertex2].size() - 1) {//je�li ostatni s�siad ju� by� rozpatrywany i nic sie nie dopasowa�o
					return false;
				}
			}
		}
	}
	if (checked1 == graph1[vertex1].size()) {//skoro wszyscy s�siedzi wierzcho�ka grafu1 byli ju� sprawdzeni, to trzeba sprawdzi� czy w grafie2 te� jest tak samo//NEW
		if (checked1 == graph2[vertex2].size()) {
			for (int j = 0; j < graph2[vertex2].size(); j++) {//dla ka�dego s�siada wierzcho�ka grafu2
				if (vertexes2.end() == (std::find(vertexes2.begin(), vertexes2.end(), graph2[vertex2][j]))) {
					return false;//skoro wszyscy s�siedzi wg1 byli sprawdzenie to czemu nie byli wszyscy wg2?! grafy nie s� takie same!
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
		///poprawno�� przekazywanych parametr�w
		if (graph.size() > 0) {//je�li wierzcho�ek jest po��czony w jaki kolwiek spos�b z grafem
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
		if (graph[i].size() != 0) {//je�li wierzcho�ek ma wgl s�siad�w, bo inaczej go traktujemy jako nie istniej�cy jeszcze w grafie
			if (keys.end() == (std::find(keys.begin(), keys.end(), graph[i].size()))) {//je�li w kluczach nie ma jeszcze zadanego stopnia
				map_of_degrees[graph[i].size()] = 1;//dodaj taki do mapy
				keys.push_back(graph[i].size());
			}
			else {
				map_of_degrees[graph[i].size()]++;//inkrementuj ju� istniej�cy
			}
		}
	}
}

bool isomorphism24(Graph& done, Graph& undone) {
 	std::vector<int> vertexes1, vertexes2;
	std::vector<std::vector<int>> graph_lvl = obtain_level(done.single_graph, undone.level);

		if (used_vertices(graph_lvl) == used_vertices(undone.single_graph)) {//je�li oba grafy maj� tyle samo wierzcho�k�w (przy wielokrotnych mo�e by� inna liczba
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

			if (keys_elements1.size() == keys_elements2.size()) {//je�li maj� r�wn� liczb� rodzai pierwiastk�w
				for (int i = 0; i < keys_elements1.size(); i++) {
					if (elements_usage2.find(keys_elements1[i]) == elements_usage2.end())//je�li pierwiastka z pierwszego grafu nie ma w drugim
						return false;
					if (elements_usage1[keys_elements1[i]] != elements_usage1[keys_elements1[i]])//je�li nie zgadzaj� si� liczebno�ci pierwiastk�w w grafach
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

				if (keys_degrees1.size() == keys_degrees2.size() && keys_degrees1.size() > 0) {//je�li taka sama ilo�� r�nych stopni
					//int min = keys_degrees1.size();//krotno�� stopnia
					//int minv = keys_degrees1.size();//stopie�
					int min = degrees_usage1[keys_degrees1[0]];
					int minv = keys_degrees1[0];
					for (int i = 0; i < keys_degrees1.size(); i++) {
						if (min > degrees_usage1[keys_degrees1[i]]) {
							min = degrees_usage1[keys_degrees1[i]];
							minv = keys_degrees1[i];
						}
						if (degrees_usage2.find(keys_degrees1[i]) == degrees_usage2.end())//je�li pierwiastka z pierwszego grafu nie ma w drugim
							return false;
						if (degrees_usage1[keys_degrees1[i]] != degrees_usage2[keys_degrees1[i]])//je�li nie zgadzaj� si� liczebno�ci pierwiastk�w w grafach
							return false;
					}
					int v1; //przechowuje wierzcho�ek startowy pierwszego grafu

					graph_pom = transform_edges_to_neighbours(graph_lvl);
					for (int i = 0; i < graph_pom.size(); i++) {
						if (graph_pom[i].size() == minv) {
							v1 = i;
							break;
						}
					}
					graph_pom = transform_edges_to_neighbours(undone.single_graph);
					for (int i = 0; i < graph_pom.size(); i++) {
						if (graph_pom[i].size() == minv) {//wierzcho�ki na tym samym st
							if (assign_element(v1) == assign_element(i)){//te same pierwiastki
								std::vector<std::vector<int>> done_pom = transform_edges_to_neighbours(graph_lvl);								
								std::vector<std::vector<int>> undone_pom = transform_edges_to_neighbours(undone.single_graph);
								vertexes1.push_back(v1); vertexes2.push_back(i);
								if (neighbours_recursive(done_pom, undone_pom, vertexes1, vertexes2, v1, i))//i bo index rep wierzcho�ek to kt�rek jest przyp vector s�siad�w
									return true;//je�li rekurencyjna zwr�ci�a true
								vertexes1.clear();//�eby na nowo mog�o si� liczy� dla kolejnej pary
								vertexes2.clear();
							}
						}
					}
				}
			}
		}
	return false;
}
