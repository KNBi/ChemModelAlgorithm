#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

//std::vector<std::string> ELEMENTS = { "H", "O", "C" };
//std::vector<int> QUANTITY = { 2, 2, 4 };



//potrzebne wektory
std::vector<std::string> ELEMENTS;
std::vector<int> QUANTITY;

std::vector<std::string> ELEMENTS_TABLE;

std::vector<std::string> HALOGENS = { "F", "Cl", "Br", "I" };
std::vector<std::string> NO_HALOGENS = { "O", "N", "S" };

std::vector<std::string> SN_RULE = { "S", "N" };


std::vector<std::vector<int>> obtain_level(std::vector<std::vector<int>>& graph, int lvl) {
	std::vector<std::vector<int>> graph_on_lvl;
	int counter = 0;

	for (int i = 0; i < graph.size(); i++) {
		std::vector<int> help_list;
		for (int j = 0; j < graph[i].size(); j++) {
			if (counter == lvl)
				break;

			help_list.push_back(graph[i][j]);
		}
		graph_on_lvl.push_back(help_list);
		help_list.clear();

		counter++;

		if (counter == lvl)
			break;
	}

	return graph_on_lvl;
}

//----------------------------------------------------------------------------------------------------------------------------

std::vector<std::vector<int>> transform(std::vector<std::vector<int>>& neighbors_list) {
	std::vector<std::vector<int>> edges_list;

	for (int i = 0; i < neighbors_list.size(); i++) {
		std::vector<int> help_list;

		for (int j = 0; j < neighbors_list[i].size(); j++) {
			help_list.push_back(i + 1);
			help_list.push_back(neighbors_list[i][j]);
			edges_list.push_back(help_list);
			help_list.clear();
		}
	}

	return edges_list;
}

//----------------------------------------------------------------------------------------------------------------------------

bool check(int number, std::vector<int> u) {
	bool outcome = true;

	for (int i = 0; i < u.size(); i++)
		if (u[i] == number) {
			outcome = false;
			break;
		}

	return outcome;
}


void recursive(int X, std::vector<int>& w_one, std::vector<int>& w_two, 
	std::vector<bool>& bool_w_one, std::vector<bool>& bool_w_two, std::vector<int>& used) {
	int index = 0;

	for (int i = 0; i < w_one.size(); i++) {
		if (w_one[i] == X && bool_w_one[i] == false) {
			bool_w_one[i] = true;
			bool_w_two[i] = true;

			if (check(w_one[i], used))
				used.push_back(w_one[i]);
			recursive(w_two[i], w_one, w_two, bool_w_one, bool_w_two, used);
		}

		if (w_two[i] == X && bool_w_two[i] == false) {
			bool_w_one[i] = true;
			bool_w_two[i] = true;

			if (check(w_two[i], used))
				used.push_back(w_two[i]);
			recursive(w_one[i], w_one, w_two, bool_w_one, bool_w_two, used);
		}
	}

	if (check(X, used))
		used.push_back(X);

	for (int i = 0; i < used.size(); i++)
		if (used[i] == X)
			index = i - 1;

	if (index > -1) {
		recursive(used[index], w_one, w_two, bool_w_one, bool_w_two, used);
	}
}

bool chech_graph_consistency(std::vector<std::vector<int>>& graph) {
	int outcome = true;
	std::vector<int> w_one;
	std::vector<int> w_two;
	std::vector<int> used;

	for (int i = 0; i < graph.size(); i++) {
		w_one.push_back(graph[i][0]);
		w_two.push_back(graph[i][1]);
	}

	std::vector<bool> bool_w_one(w_one.size(), false);
	std::vector<bool> bool_w_two(w_two.size(), false);


	bool_w_one[0] = true;
	bool_w_two[0] = true;

	used.push_back(w_one[0]);
	recursive(w_two[0], w_one, w_two, bool_w_one, bool_w_two, used);

	for (int i = 0; i < bool_w_one.size(); i++) {
		if (bool_w_one[i] == false or bool_w_two[i] == false) {
			outcome = false;
			break;
		}
	}

	return outcome;
}

//----------------------------------------------------------------------------------------------------------------------------
/*
std::string assign_element(int number) {
	std::string element = "";
	int count = 0;

	for (int i = 0; i < QUANTITY.size(); i++) {
		if (i == 0) {
			if (number <= QUANTITY[0]) {
				element = ELEMENTS[0];
				break;
			}
			count += QUANTITY[0];
		}
		else {
			if (number > QUANTITY[i - 1] and number <= QUANTITY[i] + count) {
				element = ELEMENTS[i];
				break;
			}
			count += QUANTITY[i];
		}
	}

	return element;
}
*/

std::string assign_element(int number) {
	return ELEMENTS_TABLE[number];
}

std::vector<std::string> create_elements_table() {
	std::vector<std::string> tmp;

	for (int i = 0; i < QUANTITY.size(); i++) {
		for (int j = 0; j < QUANTITY[i]; j++) {
			tmp.push_back(ELEMENTS[i]);
		}
	}

	return tmp;
}

//----------------------------------------------------------------------------------------------------------------------------

std::vector<std::vector<int>> transform_edges_to_neighbours(std::vector<std::vector<int>>& edges) {
	std::vector<std::vector<int>> neighbours;
	int max_edge = 0;

	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < edges[i].size(); j++) {
			if (edges[i][j] > max_edge)
				max_edge = edges[i][j];
		}
	}


	for (int i = 0; i <= max_edge; i++) {
		int vertex = i;
		std::vector<int> tmp_vector;

		for (int j = 0; j < edges.size(); j++) {
			for (int k = 0; k < edges[j].size(); k++) {
				if (vertex == edges[j][k]) {
					if (k == 0)
						tmp_vector.push_back(edges[j][k + 1]);
					else
						tmp_vector.push_back(edges[j][k - 1]);
				}
			}
		}

		std::sort(tmp_vector.begin(), tmp_vector.end());
		neighbours.push_back(tmp_vector);
		tmp_vector.clear();
	}

	return neighbours;
}

//----------------------------------------------------------------------------------------------------------------------------

int check_occurence_of_element(std::string element, std::vector<std::string>& vec) {
	int occurs = 0;

	if (std::find(vec.begin(), vec.end(), element) != vec.end())
		occurs = 1;

	return occurs;
}

int check_rules(std::vector<std::vector<int>>& neighbours) {
	int allow = 1;
	int flag = 0;

	for (int i = 0; i < neighbours.size(); i++) {
		if (flag)
			break;

		if (check_occurence_of_element(assign_element(i), NO_HALOGENS)) { //regu³a z halogenkami
			for (int j = 0; j < neighbours[i].size(); j++) {
				if (flag)
					break;

				if (check_occurence_of_element(assign_element(neighbours[i][j]), HALOGENS)) {
					allow = 0;
					flag = 1;
					break;
				}
			}
		}

		if (check_occurence_of_element(assign_element(i), SN_RULE)) { //regu³a S nie ³¹czy siê z N 
			for (int j = 0; j < neighbours[i].size(); j++) {
				if (flag)
					break;

				if (check_occurence_of_element(assign_element(neighbours[i][j]), SN_RULE) && neighbours[i][j] != i) {
					allow = 0;
					flag = 1;
					break;
				}
			}
		}

		if (assign_element(i) == "O") {//regu³a C-O-N
			int C_counter = 0;
			int N_counter = 0;

			for (int j = 0; j < neighbours[i].size(); j++) {
				if (assign_element(neighbours[i][j]) == "C")
					C_counter++;

				if (assign_element(neighbours[i][j]) == "N")
					N_counter++;
			}

			if (C_counter && N_counter) {
				allow = 0;
				flag = 1;
				break;
			}
		}
	}

	return allow;
}

/////////////////////////BA£AGAN BAR/////////////////
int freeing = 0;
int allocating = 0;

struct AllocationMatrics {
	uint32_t TotalAllocated = 0;
	uint32_t TotalFreed = 0;

	uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }
};
static AllocationMatrics s_AllocationMetrics;


void MemoryUsage() {
	std::cout << "Using " << s_AllocationMetrics.CurrentUsage() << " bytes\n";
}

void* operator new(size_t size) {
	s_AllocationMetrics.TotalAllocated += size;
	return malloc(size);
}

void operator delete(void* memory, size_t size) {
	s_AllocationMetrics.TotalFreed += size;
	free(memory);
}