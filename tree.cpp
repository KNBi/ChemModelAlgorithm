#include <iostream>
#include <string>
#include "graph_class.h"
#include "isomorphism.h"
#include "utils.h"
#include "tree.h"
#include "labeling.h"
#include <set>
#include <vector>


std::map<std::string, std::vector<int>> elements_bonds = {
	//https://www.thoughtco.com/valences-of-the-elements-chemistry-table-606458
	//https://scientiaeetdidactics.files.wordpress.com/2015/03/surrwcob_setd_2015_ii.pdf
	{ "H", {1} },
	{ "C", {4} },
	{ "O", {2} },
	{ "N", {3, 5} }, //?
	{ "P", {3, 5} }, 
	{ "Na", {1} },
	{ "K", {1} },
	{ "Br", {1} },
	{ "I", {1} },
	{ "F", {1} },
	{ "Cl", {1} },
	{ "S", {2} },
	{ "Li", {1} }
};

int hydrogens = 0;
int mode = 0;

void set_mode(int mode_to_set) {
	mode = mode_to_set;
}

int count_hydrogens(int elements) {//H musz� by� na ko�cu wzoru!
	int number = 0;
	for (int i = elements - 1; i >= 0; i--) {
		if (assign_element(i) != "H") {
			break;
		}
		number++;
	}
	return number;
}

int is_it_promissing(int elements, int hydrogens) {//sprawdzanie czy ilo�� wodor�w �apie si� mi�dzy najmniejsz� a najwi�ksz� mo�liw� ilo�ci� dost�pnych wi�za�
	int min = 0;
	int max = 0;

	for (int i = 0; i < elements; i++) {
		min =+ elements_bonds[assign_element(i)][0];
		max =+ elements_bonds[assign_element(i)][elements_bonds[assign_element(i)].size() - 1];
	}

	if (hydrogens < min) {//wodor�w jest mniej ni� najmniejsza ilo�� wi�za�
		return -1;
	}
	else if (hydrogens > min && hydrogens < max) {//ilo�� wodor�w mo�e by� ok
		return 0;
	}
	else {//wodor�w jest wi�cej ni� najwi�ksza ilo�� wi�za�
		return 1;
	}
}

bool is_it_good(std::vector<int>&solution, int hydrogens) {
	int sum = 0;
	for (int i = 0; i < solution.size(); i++) {
		sum += solution[i];
	}
	if (sum == hydrogens) {
		return true;
	}
	return false;
}

void delet_from_set(std::set<int>&vertices, std::vector<int>&vectored, int delet) {
	if (vectored[delet] == 1) {
		vertices.erase(delet);
		vectored[delet]--;
	}
	else {
		vectored[delet]--;
	}
}


void tree_solution(int elements, std::vector<Graph>& good_graphs) {
	int tab[2] = { 0, 1 };
	int kroki = 0;
	bool possible_multiple_edge = true;
	Graph current;//= new Graph();
	std::vector<int> edge;
	std::set<int> used_vertices;
	std::vector<int> vertices;//przechowuje ile razy dodawano go do zbioru
	vertices.reserve(elements);

	for (int i = 0; i < elements; i++) {
		vertices.push_back(0);
	}

	while (1) {
		
		/*if (good_graphs.size() == 2 && current.single_graph.size() == 7 && current.single_graph[6][0] == 2 && current.single_graph[6][1] == 7) {
			std::cout << "dupa\n";
		}
		if (current.single_graph.size() == 4) {
			std::cout << "dupa4\n";
		}
		if (current.single_graph.size() == 5) {
			std::cout << "dupa5\n";
		}
		if (current.single_graph.size() == 6) {
			std::cout << "dupa6\n";
		}
		if (current.single_graph.size() == 7) {
			std::cout << "dupa7\n";
		}
		if (current.single_graph.size() == 8) {
			std::cout << "dupa8\n";
		}*/

		if (possible_multiple_edge) {
			std::string elem1 = assign_element(tab[1]);//sprawdzamy czy aktualna mo�e by� zwielokrotniona
			std::string elem2 = assign_element(tab[0]);
			int degree1 = 0;
			int degree2 = 0;

			for (int i = 0; i < current.single_graph.size(); i++) {
				if (current.single_graph[i][0] == tab[1] || current.single_graph[i][1] == tab[1]) {
					degree1++;
				}
				if (current.single_graph[i][0] == tab[0] || current.single_graph[i][1] == tab[0]) {
					degree2++;
				}
			}

			std::vector<int> elem_degrees = elements_bonds[elem2];
			if (elem_degrees[elem_degrees.size() - 1] > degree2) {//je�li nie przekroczymy stopnia pierwszej cz�ci kraw�dzi
				elem_degrees = elements_bonds[elem1];//to sprawd� drug�
				if (elem_degrees[elem_degrees.size() - 1] > degree1) {//je�li nie przekroczy
					edge.clear();
					edge.push_back(tab[0]); edge.push_back(tab[1]);
					used_vertices.insert(tab[0]); used_vertices.insert(tab[1]);
					vertices[tab[0]]++; vertices[tab[1]]++;
					kroki++;
					current.single_graph.push_back(edge);//dodaj tak� kraw�d�
					current.level++;
				}
				else {
					possible_multiple_edge = false;//je�li aktualnie kraw�d� nie mo�e by� zwielokrotniona ustaw boolowska na fa�sz
					continue;//�eby rozpatrzy� kolejn�
				}
			}
			else {
				possible_multiple_edge = false;
				continue;
			}
		}
		else {
			bool next_edge_possible = true;
			//DODAWANIE KRAW�DZI---------------------------------------------------------------------------------------------------------------------------------------------------------
			if (tab[1] < elements - 1) {//elements to ilo��, vector numeruje od zera st�d -1 bo to opcja zeby dodac jeszcze jakas
				if (tab[0] > tab[1] + 1) {//�eby pierwszy sk�adnik kraw�dzi by� mniejszy
					tab[1]++;
					possible_multiple_edge = false;
					continue;
				}
				if (tab[0] == tab[1] + 1) {
					tab[1]++;
					possible_multiple_edge = false;
					continue;
				}
				//�eby nie tworzy�y si� strukturki typy H-H
				if (elements_bonds[assign_element(tab[0])] == std::vector<int>{1} && elements_bonds[assign_element(tab[1] + 1)] == std::vector<int>{1}) {
					tab[1]++;
					possible_multiple_edge = false;
					continue;
				}
				std::string elem1 = assign_element(tab[1] + 1);//sprawdza jaki pierwiastek kryje si� pod kolejnym wierzcho�kiem
				std::string elem2 = assign_element(tab[0]);//pierwiastek w pierwszej cz�ci kraw�dzi
				int degree1 = 0;
				int degree2 = 0;

				for (int i = 0; i < current.single_graph.size(); i++) {
					if (current.single_graph[i][0] == tab[1] + 1 || current.single_graph[i][1] == tab[1] + 1) {//je�li ten wierzcho�ek pojawi� sie w kt�rej� z kraw�dzi;
						//kraw�dzie nie maj� powt�rze� dlatego trzeba czasem pierwszy elem a czasem drugi sprawdza�
						degree1++;
					}
					if (current.single_graph[i][0] == tab[0] || current.single_graph[i][1] == tab[0]) {//je�li ten wierzcho�ek pojawi� sie w kt�rej� z kraw�dzi; 
						//kraw�dzie nie maj� powt�rze� dlatego trzeba czasem pierwszy elem a czasem drugi sprawdza�
						degree2++;
					}
				}

				std::vector<int> elem_degrees = elements_bonds[elem2];
				if (elem_degrees[elem_degrees.size() - 1] > degree2) {//je�li nie przekroczymy stopnia pierwszej cz�ci kraw�dzi
					elem_degrees = elements_bonds[elem1];//to sprawd� drug�
					if (elem_degrees[elem_degrees.size() - 1] > degree1) {//je�li nie przekroczy
						edge.clear();
						edge.push_back(tab[0]); edge.push_back(tab[1] + 1);
						used_vertices.insert(tab[0]); used_vertices.insert(tab[1] + 1);
						vertices[tab[0]]++; vertices[tab[1] + 1]++;
						current.single_graph.push_back(edge);//dodaj tak� kraw�d�
						current.level++;
						kroki++;
						tab[1]++;//zaktualizuj ost dod
						possible_multiple_edge = true;
					}
					else {
						tab[1]++;//zmieni� �eby wiadomo co ostatnio rozpatrywane, ale nie dodawa�
						continue;//�eby rozpatrzy� kolejn�
					}
				}
				else {
					tab[1]++;
					continue;
				}
			}
			else if (tab[0] + 1 < elements) {//skoro drugiego elementu kraw�dzi nie da si� zmieni� bo ju� ostatni to zmie� pierwszy
				if (tab[0] + 1 > 0) {//�eby pierwszy sk�adnik kraw�dzi by� mniejszy
					tab[0]++;
					tab[1] = 0;
					possible_multiple_edge = false;
					continue;
				}
				std::string elem1 = assign_element(tab[0] + 1);//sprawdza jaki pierwiastek kryje si� pod kolejnym wierzcho�kiem
				std::string elem2 = assign_element(0);//pierwiastek w pierwszej cz�ci kraw�dzi
				int degree1 = 0;
				int degree2 = 0;

				for (int i = 0; i < current.single_graph.size(); i++) {
					if (current.single_graph[i][0] == tab[0] + 1 || current.single_graph[i][1] == tab[0] + 1) {//je�li ten wierzcho�ek pojawi� sie w kt�rej� z kraw�dzi; 
						//kraw�dzie nie maj� powt�rze� dlatego trzeba czasem pierwszy elem a czasem drugi sprawdza�
						degree1++;
					}
					if (current.single_graph[i][0] == 0 || current.single_graph[i][1] == 0) {//je�li ten wierzcho�ek pojawi� sie w kt�rej� z kraw�dzi; kraw�dzie nie maj� powt�rze� dlatego trzeba czasem pierwszy elem a czasem drugi sprawdza�
						degree2++;
					}
				}

				std::vector<int> elem_degrees = elements_bonds[elem1];
				if (elem_degrees[elem_degrees.size() - 1] > degree1) {//je�li nie przekroczymy stopnia pierwszej cz�ci kraw�dzi
					elem_degrees = elements_bonds[elem2];//to sprawd� drug�
					if (elem_degrees[elem_degrees.size() - 1] > degree2) {//je�li nie przekroczy
						edge.clear();
						edge.push_back(tab[0] + 1); edge.push_back(0);
						used_vertices.insert(tab[0] + 1); used_vertices.insert(0);
						vertices[tab[0] + 1]++; vertices[0]++;
						current.single_graph.push_back(edge);//dodaj tak� kraw�d�
						current.level++;
						kroki++;
						tab[0]++;//zaktualizuj ost dod
						tab[1] = 0;
						possible_multiple_edge = true;
					}
					else {
						tab[0]++;//zmieni� �eby wiadomo co ostatnio rozpatrywane, ale nie dodawa�
						tab[1] = 0;
						continue;//�eby rozp kolejn�
					}
				}
				else {
					tab[0]++;
					tab[1] = 0;
					continue;
				}
			}
			else {
				if (tab[0] == elements - 1 && tab[1] == elements - 1 && current.single_graph.empty())
					break;

				tab[0] = current.single_graph[current.single_graph.size() - 1][0];//przypisz pierwsz� cz�� kraw�dzi ostatniej jaka jest zapisana w grafie
				tab[1] = current.single_graph[current.single_graph.size() - 1][1];
				delet_from_set(used_vertices, vertices, tab[0]); delet_from_set(used_vertices, vertices, tab[1]);
				current.single_graph.pop_back();
				current.level--;

				if (tab[0] == elements - 1 && tab[1] == elements - 2) {//skoro to by�a ostatnia mo�liwa to tzreba nie tylko j� ale i poprzedni�
					//usun��, �eby da�o si� rozpatrywa� dalej, a do tab wpisa� rzeczy z przedostatniej �eby to j� sprawdza� dalej
					tab[0] = current.single_graph[current.single_graph.size() - 1][0];
					tab[1] = current.single_graph[current.single_graph.size() - 1][1];
					delet_from_set(used_vertices, vertices, tab[0]); delet_from_set(used_vertices, vertices, tab[1]);
					current.single_graph.pop_back();
					current.level--;
				}

				continue;//nie by�� dodana �adna kraw�d�, wi�c pr�buj dalej
			}
		}

		//---------------------------------------------------------------------------------------------------------------------------------------------------------
		bool sth_isomorphic = false;

		//std::cout << "before switch case "; MemoryUsage(); std::cout << "\n";
		switch (mode)
		{
		case(0)://IZOMORFIZM
			for (int i = 0; i < good_graphs.size(); i++) {
				if (isomorphism24(good_graphs[i], current)) {
					sth_isomorphic = true;
					break;
				}
			}
			break;
		case(1)://ETKIETOWANIE
			if (!is_label_the_biggest(current.single_graph, elements)) {
				sth_isomorphic = true;
			}
			break;
		default:
			std::cout << "wrong mode!\n";
			break;
		}
		//std::cout << "after switch case "; MemoryUsage(); std::cout << "\n\n";

		if (sth_isomorphic) {
			tab[0] = current.single_graph[current.single_graph.size() - 1][0];//przypisz pierwsz� cz�� kraw�dzi ostatniej jaka jest zapisana w grafie
			tab[1] = current.single_graph[current.single_graph.size() - 1][1];
			delet_from_set(used_vertices, vertices, tab[0]); delet_from_set(used_vertices, vertices, tab[1]);
			current.single_graph.pop_back();//usu� ostatni� kraw�d�
			possible_multiple_edge = false;//�eby nie podkusi�o programu si� zap�tli�
			current.level--;
			if (current.single_graph.empty())
				break;
			continue;//nie by�a dodana �adna kraw�d�, wi�c pr�buj dalej
		}
		else if(used_vertices.size() == elements && chech_graph_consistency(current.single_graph)){//nie jest izomorficzny z �adnym z powsta�ych
			bool done = true;

			for (int i = 0; i < elements; i++) {//dla ka�dego pierwiastka w grafie
				int degree = 0;
				bool proper_degree = false;//moga by� problemy bo zmieni�am
				for (int j = 0; j < current.single_graph.size(); j++) {//sprawd� na kt�rym jest stopniu obecnie w grafie
					if (current.single_graph[j][0] == i || current.single_graph[j][1] == i) {
						degree++;
					}
					if (j == current.single_graph.size() - 1) {
						std::vector<int> possible_degrees = elements_bonds[assign_element(i)];
						//bool proper_degree = false;
						for (int k = 0; k < possible_degrees.size(); k++) {
							if (degree == possible_degrees[k]) {//je�li jest to jeden z dopuszczalnych stopni
								proper_degree = true;
								break;
							}
						}

						if (!proper_degree) {//jesli jest na niew�a�ciwym stopniu
							done = false;
							break;
						}
					}
				}
				if (!proper_degree) {//jesli jest na niew�a�ciwym stopniu
					break;
				}
			}

			if (done) {
				Graph good_graph;// = new Graph();//dodawanie poprawnego na list�
				good_graph = current;//
				std::vector<std::vector<int>> good_graph_neighbours = transform_edges_to_neighbours(good_graph.single_graph);
				if (check_rules(good_graph_neighbours)) {//sprawdzanie zasad chemicznych
					good_graphs.push_back(good_graph);
				}
				//good_graphs.push_back(good_graph);
				//usuwanie kraw�dzi z poprawnego �eby szuka� dalej rozwi�za�
				tab[0] = current.single_graph[current.single_graph.size() - 1][0];
				tab[1] = current.single_graph[current.single_graph.size() - 1][1];
				delet_from_set(used_vertices, vertices, tab[0]); delet_from_set(used_vertices, vertices, tab[1]);
				possible_multiple_edge = false; //bo skoro j� odrzucamy to nie mo�na jej jeszcze raz sprawdza� bo p�tla!
				current.single_graph.pop_back();
				current.level--;
				good_graph.~Graph();
			}
		}
	}
	edge.~vector();
	vertices.~vector();
	current.~Graph();
}



void tree_solution_without_hydrogens(int elements, std::vector<Graph>& good_graphs) {
	hydrogens = count_hydrogens(elements);
	int actual_elements_to_consider = elements - hydrogens;
	int tab[2] = { 0, 1 };
	int kroki = 0;
	bool possible_multiple_edge = true;
	Graph current; // = new Graph();
	std::vector<int> edge;
	std::set<int> used_vertices;
	std::vector<int> vertices;//przechowuje ile razy dodawano go do zbioru
	vertices.reserve(actual_elements_to_consider);

	if (actual_elements_to_consider == 1) {//tak czasem bywa
		std::vector<int> possible_degrees = elements_bonds[assign_element(0)];
		bool good = false;

		for (int i = 0; i < possible_degrees.size(); i++) {
			if (hydrogens == possible_degrees[i]) {//je�li jest to jeden z dopuszczalnych stopni
				good = true;
				break;
			}
		}

		if (good) {
			Graph good_graph = current;
			int hydrogen_number = 0;//bo w sumie to wierzcho�i s� ponumerowane <0,actual_ele_to_cons>, wi�c wodory powinny zaczyna� od act_elem...
			for (int j = 0; j < hydrogens; j++) {
				std::vector<int> edge;
				edge.push_back(0);
				edge.push_back(actual_elements_to_consider + hydrogen_number);
				good_graph.single_graph.push_back(edge);
				hydrogen_number++;
			}
			std::vector<std::vector<int>> good_graph_neighbours = transform_edges_to_neighbours(good_graph.single_graph);
			if (check_rules(good_graph_neighbours)) {
				good_graphs.push_back(good_graph);
			}
			//good_graphs.push_back(good_graph);
		}
		return;
	}

	for (int i = 0; i < actual_elements_to_consider; i++) {
		vertices.push_back(0);
	}

	while (1) {
		if (possible_multiple_edge) {
			std::string elem1 = assign_element(tab[1]);//sprawdzamy czy aktualna mo�e by� zwielokrotniona
			std::string elem2 = assign_element(tab[0]);
			int degree1 = 0;
			int degree2 = 0;

			for (int i = 0; i < current.single_graph.size(); i++) {
				if (current.single_graph[i][0] == tab[1] || current.single_graph[i][1] == tab[1]) {
					degree1++;
				}
				if (current.single_graph[i][0] == tab[0] || current.single_graph[i][1] == tab[0]) {					
					degree2++;
				}
			}

			std::vector<int> elem_degrees = elements_bonds[elem2];
			if (elem_degrees[elem_degrees.size() - 1] > degree2) {//je�li nie przekroczymy stopnia pierwszej cz�ci kraw�dzi
				elem_degrees = elements_bonds[elem1];//to sprawd� drug�
				if (elem_degrees[elem_degrees.size() - 1] > degree1) {//je�li nie przekroczy
					edge.clear();
					edge.push_back(tab[0]); edge.push_back(tab[1]);
					used_vertices.insert(tab[0]); used_vertices.insert(tab[1]);
					vertices[tab[0]]++; vertices[tab[1]]++;
					kroki++;
					current.single_graph.push_back(edge);//dodaj tak� kraw�d�
					current.level++;
				}
				else {
					possible_multiple_edge = false;//je�li aktualnie kraw�d� nie mo�e by� zwielokrotniona ustaw boolowska na fa�sz
					continue;//�eby rozpatrzy� kolejn�
				}
			}
			else {
				possible_multiple_edge = false;
				continue;
			}
		}
		else {
			bool next_edge_possible = true;
			//DODAWANIE KRAW�DZI---------------------------------------------------------------------------------------------------------------------------------------------------------
			if (tab[1] < actual_elements_to_consider - 1) {//elements to ilo��, vector numeruje od zera st�d -1 bo to opcja zeby dodac jeszcze jakas
				if (tab[0] > tab[1] + 1) {//�eby pierwszy sk�adnik kraw�dzi by� mniejszy
					tab[1]++;
					possible_multiple_edge = false;
					continue;
				}
				if (tab[0] == tab[1] + 1) {
					tab[1]++;
					possible_multiple_edge = false;
					continue;
				}
				//�eby nie tworzy�y si� strukturki typy H-H
				if (elements_bonds[assign_element(tab[0])] == std::vector<int>{1}&& elements_bonds[assign_element(tab[1] + 1)] == std::vector<int>{1}) {
					tab[1]++;
					possible_multiple_edge = false;
					continue;
				}
				std::string elem1 = assign_element(tab[1] + 1);//sprawdza jaki pierwiastek kryje si� pod kolejnym wierzcho�kiem
				std::string elem2 = assign_element(tab[0]);//pierwiastek w pierwszej cz�ci kraw�dzi
				int degree1 = 0;
				int degree2 = 0;

				for (int i = 0; i < current.single_graph.size(); i++) {
					if (current.single_graph[i][0] == tab[1] + 1 || current.single_graph[i][1] == tab[1] + 1) {//je�li ten wierzcho�ek pojawi� sie w kt�rej� z kraw�dzi;
						//kraw�dzie nie maj� powt�rze� dlatego trzeba czasem pierwszy elem a czasem drugi sprawdza�
						degree1++;
					}
					if (current.single_graph[i][0] == tab[0] || current.single_graph[i][1] == tab[0]) {//je�li ten wierzcho�ek pojawi� sie w kt�rej� z kraw�dzi; 
						//kraw�dzie nie maj� powt�rze� dlatego trzeba czasem pierwszy elem a czasem drugi sprawdza�
						degree2++;
					}
				}

				std::vector<int> elem_degrees = elements_bonds[elem2];
				if (elem_degrees[elem_degrees.size() - 1] > degree2) {//je�li nie przekroczymy stopnia pierwszej cz�ci kraw�dzi
					elem_degrees = elements_bonds[elem1];//to sprawd� drug�
					if (elem_degrees[elem_degrees.size() - 1] > degree1) {//je�li nie przekroczy
						edge.clear();
						edge.push_back(tab[0]); edge.push_back(tab[1] + 1);
						used_vertices.insert(tab[0]); used_vertices.insert(tab[1] + 1);
						vertices[tab[0]]++; vertices[tab[1] + 1]++;
						current.single_graph.push_back(edge);//dodaj tak� kraw�d�
						current.level++;
						kroki++;
						tab[1]++;//zaktualizuj ost dod
						possible_multiple_edge = true;
					}
					else {
						tab[1]++;//zmieni� �eby wiadomo co ostatnio rozpatrywane, ale nie dodawa�
						continue;//�eby rozpatrzy� kolejn�
					}
				}
				else {
					tab[1]++;
					continue;
				}
			}
			else if (tab[0] + 1 < actual_elements_to_consider) {//skoro drugiego elementu kraw�dzi nie da si� zmieni� bo ju� ostatni to zmie� pierwszy
				if (tab[0] + 1 > 0) {//�eby pierwszy sk�adnik kraw�dzi by� mniejszy
					tab[0]++;
					tab[1] = 0;
					possible_multiple_edge = false;
					continue;
				}
				std::string elem1 = assign_element(tab[0] + 1);//sprawdza jaki pierwiastek kryje si� pod kolejnym wierzcho�kiem
				std::string elem2 = assign_element(0);//pierwiastek w pierwszej cz�ci kraw�dzi
				int degree1 = 0;
				int degree2 = 0;

				for (int i = 0; i < current.single_graph.size(); i++) {
					if (current.single_graph[i][0] == tab[0] + 1 || current.single_graph[i][1] == tab[0] + 1) {//je�li ten wierzcho�ek pojawi� sie w kt�rej� z kraw�dzi; 
						//kraw�dzie nie maj� powt�rze� dlatego trzeba czasem pierwszy elem a czasem drugi sprawdza�
						degree1++;
					}
					if (current.single_graph[i][0] == 0 || current.single_graph[i][1] == 0) {//je�li ten wierzcho�ek pojawi� sie w kt�rej� z kraw�dzi; kraw�dzie nie maj� powt�rze� dlatego trzeba czasem pierwszy elem a czasem drugi sprawdza�
						degree2++;
					}
				}

				std::vector<int> elem_degrees = elements_bonds[elem1];
				if (elem_degrees[elem_degrees.size() - 1] > degree1) {//je�li nie przekroczymy stopnia pierwszej cz�ci kraw�dzi
					elem_degrees = elements_bonds[elem2];//to sprawd� drug�
					if (elem_degrees[elem_degrees.size() - 1] > degree2) {//je�li nie przekroczy
						edge.clear();
						edge.push_back(tab[0] + 1); edge.push_back(0);
						used_vertices.insert(tab[0] + 1); used_vertices.insert(0);
						vertices[tab[0] + 1]++; vertices[0]++;
						current.single_graph.push_back(edge);//dodaj tak� kraw�d�
						current.level++;
						kroki++;
						tab[0]++;//zaktualizuj ost dod
						tab[1] = 0;
						possible_multiple_edge = true;
					}
					else {
						tab[0]++;//zmieni� �eby wiadomo co ostatnio rozpatrywane, ale nie dodawa�
						tab[1] = 0;
						continue;//�eby rozp kolejn�
					}
				}
				else {
					tab[0]++;
					tab[1] = 0;
					continue;
				}
			}
			else {
				if (tab[0] == actual_elements_to_consider - 1 && tab[1] == actual_elements_to_consider - 1 && current.single_graph.empty())
					break;

				tab[0] = current.single_graph[current.single_graph.size() - 1][0];//przypisz pierwsz� cz�� kraw�dzi ostatniej jaka jest zapisana w grafie
				tab[1] = current.single_graph[current.single_graph.size() - 1][1];
				delet_from_set(used_vertices, vertices, tab[0]); delet_from_set(used_vertices, vertices, tab[1]);
				current.single_graph.pop_back();
				current.level--;

				if (tab[0] == actual_elements_to_consider - 1 && tab[1] == actual_elements_to_consider - 2) {//skoro to by�a ostatnia mo�liwa to tzreba nie tylko 
					//j� ale i poprzedni� usun��, �eby da�o si� rozpatrywa� dalej, a do tab wpisa� rzeczy z przedostatniej �eby to j� sprawdza� dalej
					tab[0] = current.single_graph[current.single_graph.size() - 1][0];
					tab[1] = current.single_graph[current.single_graph.size() - 1][1];
					delet_from_set(used_vertices, vertices, tab[0]); delet_from_set(used_vertices, vertices, tab[1]);
					current.single_graph.pop_back();
					current.level--;
				}

				continue;//nei by�� dodana �adna kraw�d�, wi�c pr�buj dalej
			}
		}
		//zosta� ju� dodana jaka� kraw�d� sprawd�my czy graf jest ok
		//---------------------------------------------------------------------------------------------------------------------------------------------------------
		bool sth_isomorphic = false;
		
		switch (mode)
		{
		case(0)://IZOMORFIZM
			for (int i = 0; i < good_graphs.size(); i++) {
				if (isomorphism24(good_graphs[i], current)) {
					sth_isomorphic = true;
					break;
				}
			}
			break;
		case(1)://ETKIETOWANIE
			if (!is_label_the_biggest(current.single_graph, elements)) {
				sth_isomorphic = true;
			}
			break;
		default:
			std::cout << "wrong mode!\n";
			break;
		}
		
		if (sth_isomorphic) {
			tab[0] = current.single_graph[current.single_graph.size() - 1][0];//przypisz pierwsz� cz�� kraw�dzi ostatniej jaka jest zapisana w grafie
			tab[1] = current.single_graph[current.single_graph.size() - 1][1];
			delet_from_set(used_vertices, vertices, tab[0]); delet_from_set(used_vertices, vertices, tab[1]);
			current.single_graph.pop_back();//usu� ostatni� kraw�d�
			possible_multiple_edge = false;//�eby nie podkusi�o programu si� zap�tli�
			current.level--;
			if (current.single_graph.empty())
				break;
			continue;//nei by�� dodana �adna kraw�d�, wi�c pr�buj dalej
		}
		else if(used_vertices.size() == actual_elements_to_consider && chech_graph_consistency(current.single_graph)){//nie jest izomorficzny z �adnym z powsta�ych
			//jest sp�jny i zawiera ju� wszystkie pierwiastki
			//TO SPRAWDZAJ CZY PROMISSING
			
			bool done = true;
			std::vector<std::vector<int>> possible_boundaries;
			std::vector<int> biggest_possible;
			biggest_possible.reserve(actual_elements_to_consider);
			std::vector<int> actual_sol;
			actual_sol.reserve(actual_elements_to_consider);
			int last_used = -1;
			int act_vertex = 0;

			for (int i = 0; i < actual_elements_to_consider; i++) {//dla ka�dego pierwiastka w grafie
				std::vector<int> possibilities;
				int degree = 0;

				for (int j = 0; j < current.single_graph.size(); j++) {//sprawd� na kt�rym jest stopniu obecnie w grafie

					if (current.single_graph[j][0] == i || current.single_graph[j][1] == i) {
						degree++;
					}
					if (j == current.single_graph.size() - 1) {
						std::vector<int> possible_degrees = elements_bonds[assign_element(i)];

						for (int k = 0; k < possible_degrees.size(); k++) {
							if (possible_degrees[k] - degree >= 0) {//je�li na takim stopniu mo�na jeszcze co� do��czy� lub do��czy� 0 (ale nie mniej)
								possibilities.push_back(possible_degrees[k] - degree);
							}
						}
						if (possibilities.size() == 0) {
							possibilities.push_back(0);
						}
						possible_boundaries.push_back(possibilities);
					}
				}
				//dodanie najwi�kszej ilo�ci wodor�w do vectora najwi�kszych
				biggest_possible.push_back(possibilities[possibilities.size() - 1]);
			}

			bool still_can = true;
			bool vert_to_check = true;
			int actual_good_graphs_number = good_graphs.size();

			while (still_can) {
				if (possible_boundaries[act_vertex].size() == 1 && possible_boundaries[act_vertex][0] == 0 && vert_to_check) {//je�li nic ni mo�na doda� do tego pierwiastka
					actual_sol.push_back(0);
					last_used = -1;
					act_vertex++;
				}//je�li wierzcho�ek jest zbyt du�y lub nie ma ju� co dodda� to wykona si� else
				else if (act_vertex < actual_elements_to_consider && possible_boundaries[act_vertex][possible_boundaries[act_vertex].size() - 1] > last_used) {
					for (int i = 0; i < possible_boundaries[act_vertex].size(); i++) {
						if (possible_boundaries[act_vertex][i] > last_used) {
							actual_sol.push_back(possible_boundaries[act_vertex][i]);
							last_used = -1;
							vert_to_check = true;
							act_vertex++;
							break;
						}
					}
				}
				else {
					if (actual_sol.size() == 0) {
						last_used = -1;
					}
					else {
						last_used = actual_sol[actual_sol.size() - 1];//0-1!!!
						vert_to_check = false;
						actual_sol.pop_back();
						act_vertex--;
					}
				}
				if (actual_sol.size() == actual_elements_to_consider) {
					//dodawanie rozwi�zania do dobrych rozwi�za� je�li jest dobre xD
					if (is_it_good(actual_sol, hydrogens)) {
						//dodanie wodor�w do grafu
						Graph good_graph = current;
						int hydrogen_number = 0;//bo w sumie to wierzcho�i s� ponumerowane <0,actual_ele_to_cons>, wi�c wodory powinny zaczyna� od act_elem...
						for (int i = 0; i < actual_sol.size(); i++) {
							for (int j = 0; j < actual_sol[i]; j++) {
								std::vector<int> edge;
								edge.push_back(i);
								edge.push_back(actual_elements_to_consider + hydrogen_number);
								good_graph.single_graph.push_back(edge);
								hydrogen_number++;
							}
						}
						///////////podw�jne sprawdzanie	
						switch (mode)
						{
						case(0)://IZOMORFIZM
							if (actual_good_graphs_number == 0) {
								std::vector<std::vector<int>> good_graph_neighbours = transform_edges_to_neighbours(good_graph.single_graph);
								if (check_rules(good_graph_neighbours)){
									good_graphs.push_back(good_graph);
								}
							}
							else {
								if (actual_good_graphs_number == good_graphs.size()) {
									std::vector<std::vector<int>> good_graph_neighbours = transform_edges_to_neighbours(good_graph.single_graph);
									if (check_rules(good_graph_neighbours)) {
										good_graphs.push_back(good_graph);
									}
								}
								else {
									for (int i = actual_good_graphs_number; i < good_graphs.size(); i++) {//por�wnuje grafy tylko tego coru - mniej por�wna�
										if (isomorphism24(good_graphs[i], current)) {
											std::vector<std::vector<int>> good_graph_neighbours = transform_edges_to_neighbours(good_graph.single_graph);
											if (check_rules(good_graph_neighbours)) {
												good_graphs.push_back(good_graph);
											}
										}
									}
								}								
							}
							break;
						case(1)://ETKIETOWANIE
							if (is_label_the_biggest(good_graph.single_graph, elements)) {
								std::vector<std::vector<int>> good_graph_neighbours = transform_edges_to_neighbours(good_graph.single_graph);
								if (check_rules(good_graph_neighbours)) {
									good_graphs.push_back(good_graph);
								}
							}
							break;
						default:
							std::cout << "wrong mode!\n";
							break;
						}
						////////bez sprawdzania
						//good_graphs.push_back(good_graph);
					}
					//sprawdzanie czy to ju� ostatnia mo�liwo��, jak tak to koniec whilea
					for (int i = 0; i < actual_elements_to_consider; i++) {
						if (actual_sol[i] != biggest_possible[i]) {//usuwa�sko ostatniej pozycji
							last_used = actual_sol[actual_sol.size() - 1];
							vert_to_check = false;
							actual_sol.pop_back();
							act_vertex--;//�eby nie przekracza� granicy
							break;
						}
						if (i == actual_elements_to_consider - 1) {
							still_can = false;
						}
					}
				}
			}
			
		}
	}
}

