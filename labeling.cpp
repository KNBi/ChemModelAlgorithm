#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

#include "labeling.h"
#include "graph_class.h"
#include "utils.h"

int** matrix;
bool is_table_done = false;

int label_len(int elements) {
	int sum = 0;
	int iteration = elements;
	for (int i = 0; i < iteration; i++) {
		sum += elements;
		elements--;
	}
	return sum;
}

void change_matrix(int** matrix, std::vector<int>elements_order) {
	int elements = elements_order.size();
	std::vector<int> no_need_change_again;
	for (int i = 0; i < elements; i++) {
		if (i == elements_order[i]) {//jeœli order mówi ¿e element jest w dobrym miejscu, to patrz kolejny
			continue;
		}
		if (i == elements_order[elements_order[i]]) {
			no_need_change_again.push_back(elements_order[i]);
		}
		std::vector<int>::iterator it;

		it = find(no_need_change_again.begin(), no_need_change_again.end(), i);
		if (it == no_need_change_again.end()) {
			int* tmp1 = new int[elements];
			int* tmp2 = new int[elements];
			for (int m = 0; m < elements; m++) {
				if ((m == i || m == elements_order[i]) && matrix[i][elements_order[i]] > 0) {
					tmp1[m] = matrix[elements_order[i]][m];
					tmp2[m] = matrix[i][m];
					continue;
				}
				tmp1[m] = matrix[i][m];
				tmp2[m] = matrix[elements_order[i]][m];
			}
			for (int m = 0; m < elements; m++) {
				matrix[i][m] = tmp2[m];
				matrix[m][i] = tmp2[m];
				matrix[elements_order[i]][m] = tmp1[m];
				matrix[m][elements_order[i]] = tmp1[m];
			}
			delete[] tmp1;
			delete[] tmp2;
		}
	}
	/*std::cout << "\nprzed zmiania\n";
	for (int i = 0; i < elements; i++) {
		for (int j = 0; j < elements; j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << "\n";
	}*/
}

//1.3
int compare_labels(std::vector<std::vector<int>>& graph1, std::vector<int>& graph, int elements) {//return: -1(left graph1), 0 equals, 1 (graph) 																							 
	//graph1 to lista krawedzi, graph to vector kolejnosci pierwiastkow
	//std::cout << "compare labels start "; MemoryUsage(); std::cout << "\n";
	int actual_elements_considered = elements;
	int label_length = label_len(actual_elements_considered);
	std::vector<int> label1;
	label1.reserve(label_length);
	std::vector<int> label2;
	label2.reserve(label_length);
	std::vector<int> label3;
	label3.reserve(label_length);

	if (!is_table_done) {
		matrix = new int* [actual_elements_considered];
		for (int i = 0; i < actual_elements_considered; i++) {
			matrix[i] = new int[actual_elements_considered];
			for (int j = 0; j < actual_elements_considered; j++) {
				matrix[i][j] = 0;
			}
		}
		is_table_done = true;
	}
	else {
		for (int i = 0; i < actual_elements_considered; i++) {
			for (int j = 0; j < actual_elements_considered; j++) {
				matrix[i][j] = 0;
			}
		}
	}
	
	//tworzenie macierzy aktualnego grafu
	for (int i = 0; i < graph1.size(); i++) {
		matrix[graph1[i][0]][graph1[i][1]]++;//bo mog¹ byæ krawêdzie wielokrotne
		matrix[graph1[i][1]][graph1[i][0]]++;//bo macierz jest symetryczna
	}

	//zczytanie z matrycy etykiety
	for (int i = 0; i < actual_elements_considered; i++) {
		for (int j = i; j < actual_elements_considered; j++) {//skoro matryca jest symetryczna to wystarczy zczytaæ po³owê
			label1.push_back(matrix[i][j]);
		}
	}

	//zczytanie z matrycy etykiety sposobem piotra
	//for (int i = 0; i < actual_elements_considered; i++) {
	//	for (int j = i; j < actual_elements_considered; j++) {//skoro matryca jest symetryczna to wystarczy zczytaæ po³owê
	//		label3.push_back(matrix[graph[i]][graph[j]]);
	//	}
	//}

	/*std::cout << "\n";
	for (int i = 0; i < actual_elements_considered; i++) {
		for (int j = 0; j < actual_elements_considered; j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "taka zmiana\n";
	for (int i = 0; i < graph.size(); i++) {
		std::cout << graph[i] << " ";
	}
	std::cout << "\n";*/

	//std::cout << "before change matrix"; MemoryUsage(); std::cout << "\n";
	change_matrix(matrix, graph);//dzia³a na oryginalnej matrix, dlatego nie potrzebne przypisanie
	//std::cout << "after change matrix"; MemoryUsage(); std::cout << "\n";

	//zczytanie z matrycy zmienionej etykiety
	for (int n = 0; n < actual_elements_considered; n++) {
		for (int m = n; m < actual_elements_considered; m++) {//jak wywali b³¹d to zmieñ spowrotem na 0 xD
			label2.push_back(matrix[n][m]);
		}
	}

	/*for (int n = 0; n < label_length; n++) {
		if (label2 != label3) {
			std::cout << "sth wrong\n";

			for (int i = 0; i < label_length; i++) {
				std::cout<<label2[i]<<" ";
			}
			std::cout << "\n";
			for (int i = 0; i < label_length; i++) {
				std::cout << label3[i] << " ";
			}
			std::cout << "\n";
			break;
		}
	}*/

	//sprawdzenie czy etykieta jest mniejsza czy wiêksza
	//std::cout << "\n" << label1 << "\n" << label2 << "\n";
	for (int n = 0; n < label_length; n++) {
		if (label1[n] > label2[n]) {
			//std::cout << "lewa wiêksza\n";
			label1.clear();
			label2.clear();
			return -1;
		}
		else if (label1[n] < label2[n]) {
			label1.clear();
			label2.clear();
			return 1;//skoro nasza obecna etykieta jest mniejsza, to graf nie jest kanoniczny, koniec dyskusji
			//std::cout << "prawa wiêksza\n";
		}
	}

	//usuwanko matrix
	//std::cout << "b matrix delete "; MemoryUsage(); std::cout << std::endl;
	//for (int i = 0; i < actual_elements_considered; i++) {
	//	delete[] matrix[i];
	//}
	//delete[] matrix;
	//std::cout << "a matrix delete "; MemoryUsage(); std::cout << std::endl;

	//std::cout << "before clearing labels in compare labesls "; MemoryUsage(); std::cout << "\n";
	label1.~vector();
	label2.~vector();
	//std::cout << "after clearing labels in compare labesls "; MemoryUsage(); std::cout << "\n";

	//std::cout << "end of compare labels"; MemoryUsage(); std::cout << "\n";
	return 0;//skoro ¿adna zmiana nie spowodowa³a wiêkszej etykiety to znaczy ¿e nasz jest najwiêksza
}

bool is_label_the_biggest(std::vector<std::vector<int>>&graph_to_check, int elements) {//graph_to_check jest list¹ krawêdzi
	//std::cout << "dupa\n";
	std::vector<int>solution;
	solution.reserve(elements);
	std::vector<int>to_use;
	int last_used = -1;//-1 bo to liczba zawsze mniejsza ni¿ jakikolwiek wierzcho³ek w grafie
	bool still_counting = true;

	do {
		if (to_use.size() == 0) {
			if (solution.size() == elements) {//gdy nie ma ju¿ wiêcej pierwiastków do dodania
				last_used = solution[solution.size() - 1];
				solution.pop_back();
				to_use.push_back(last_used);
			}
			else {//kolejny rodzaj do doadania
				int element = solution.size();
				std::string type_of_elements = assign_element(element);//przypisanie nastêpnego typu; size() wiêc nie trzeba +1
				while (element < elements && assign_element(element) == type_of_elements) {
					to_use.push_back(element);
					element++;
				}
			}
		}
		int index = -1;
		std::vector<int>::iterator it = to_use.begin();
		for (int i = 0; i < to_use.size(); i++) {//znajdŸ na liscie to_use co dalej u¿ywamy i zapisz index
			if (to_use[i] > last_used) {
				index = i;
				break;
			}
			it++;
		}
		if (index > -1) {//jeœli zosta³ znaleziony jakiœ pierwiastek do dodania do rozwi¹zania
			solution.push_back(to_use[index]);
			to_use.erase(it);//ju¿ wykorzystany, wiêc usuwanko
			last_used = -1;//¿eby na nowym poziomie drzewa zaczynaæ od pocz¹tku
		}
		else {
			if (solution.size() == elements || (solution.size() > 1 && assign_element(solution.size() - 1) != assign_element(solution.size()))) {
				//w takim razie zaraz usuwaj¹c zmienimy blok rodzaju pierwiastków
				to_use.clear();//skoro zmieniamy blok to trzeba zrobiæ porz¹dki
				last_used = solution[solution.size() - 1];
				to_use.push_back(last_used);
				solution.pop_back();
			}
			else if (solution.size() > 0) {
				//tu poprostu usuwamy
				last_used = solution[solution.size() - 1];
				it = to_use.begin();
				for (int i = 0; i < to_use.size(); i++) {
					if (to_use[i] > last_used) {
						break;
					}
					it++;
				}
				to_use.insert(it, last_used);
				solution.pop_back();
			}
			//jeœli solution.size()==0 to nie mamy co usun¹æ a mo¿e tu takie wpaœæ (chyba xD)
		}
		if (solution.size() == elements) {
			/*std::cout << "\n";
			for (int n = 0; n < solution.size(); n++) {
				std::cout << solution[n] << " ";
			}
			std::cout << "\n";*/
			//std::cout << "before compare labels"; MemoryUsage(); std::cout << "\n";
			if (compare_labels(graph_to_check, solution, elements) == 1) {//1-czyli znaleziono takie ustawienie pierw, ze jest lepsze niz to co prysz³o do funkcji
				solution.clear();
				solution.~vector();
				to_use.clear();
				to_use.~vector();
				return false;
			}//jeœli znaleziona przez nas jest wiêksza ni¿ grafu przes³anego do funkcji to nie jest on kanoniczny, wiêc zwracamy false
			//jeœli etykiety s¹ równe lub przes³anego grafu jest wiêksza to szukamy dalej czy istnieje wiêksza
			//std::cout << "after compare labels "; MemoryUsage(); std::cout << "\n";
		}
		if (solution.size() == 0 && assign_element(last_used) != assign_element(last_used + 1)) {
			still_counting = false;
		}

	} while (still_counting);

	solution.clear();
	solution.~vector();
	to_use.clear();
	to_use.~vector();

	//std::cout << "after delete in is_label_the_biggest "; MemoryUsage(); std::cout << "\n";

	return true;
}