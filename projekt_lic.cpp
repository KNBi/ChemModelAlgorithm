
#include <iostream>
#include <vector>
#include <utility> 
#include <map>
#include <Windows.h>
#include <time.h>
#include <fstream>

#include "isomorphism.h"
#include "utils.h"
#include "graph_class.h"
#include "tree.h"

//////////////////////////////UWAGI/////////////////////////////////////////////////////////////
//
//WODORY NA KOŃCU WZORU, W SENSIE TABELI Z KROTNOŚCIAMI POSZCEGÓLNYCH PIERWIASTKÓW!!!
////////////////////////////////////////////////////////////////////////////////////////////////

int quantity_of_elements_in_formula = 0;
clock_t start, stop;
enum modes {isomorphism = 0, labeling = 1};
std::string filename = "testy";

struct test_case {
	std::vector<std::string>elements;
	std::vector<int> quantities;
};

std::vector<test_case> test_cases{
	//0
		test_case{{ "O","H" },
				{ 1, 2 }},
	//1
		test_case{{ "C","H" },
				{ 2, 2 }},
	//2
		test_case{{ "C","O","H" },
				{ 1, 1, 2 }},
	//3
		test_case{{ "C","H" },
				{ 1, 4 }},
	//4
		test_case{{ "C", "Cl", "O", "H" },
				{ 2, 1, 1, 1 }},
	//5
		test_case{{ "C", "Cl", "H" },
				{ 1, 2, 2 }},
	//6
		test_case{{ "C","I","H" },
				{ 1, 3,  1 }},
	//7
		test_case{{ "C", "O", "H" },
				{ 2, 1, 2 }},
	//8
		test_case{{ "C", "N", "O", "H" },
				{ 1, 1, 1, 3 }},
	//9
		test_case{{ "C","I","H" },
				{ 2, 3,  1 }},
	//10
		test_case{{ "C","O","H" },
				{ 1, 1, 4 }},
	//11
		test_case{{ "C", "H" },
				{ 2, 4 }},
	//12
		test_case{{ "C", "Cl", "H" },
				{ 2, 2, 2 }},
	//13
		test_case{{ "C","O","N","H" },
				{ 1, 2, 1, 3 }},
	//14
		test_case{{ "C", "O", "H" },
				{ 2, 3, 2 }},
	//15
		test_case{{ "C", "O", "H" },
				{ 2, 2, 3 }},
	//16
		test_case{{ "C", "O", "H" },
				{ 2, 1, 4 }},
	//17
		test_case{{ "C", "Cl", "O", "H" },
				{ 2, 3, 1, 1 }},
	//18
		test_case{{ "C", "S", "H" },
				{ 2, 2, 4 }},
	//19
		test_case{{ "C", "Br", "H" },
				{ 2, 4, 2 }},
	//20
		test_case{{ "C", "H" },
				{ 2, 6 }},
	//21
		test_case{{ "C", "O", "H" },
				{ 2, 2, 4 }},
	//22
		test_case{{ "C", "Cl", "O", "H" },
				{ 2, 2, 2, 2 }},
	//23
		test_case{{ "C", "N", "O", "H" },
				{ 1, 1, 2, 5 }},
	//24
		test_case{{ "C", "O", "H" },
				{ 2, 3, 4 }},
	//25
		test_case{{ "C", "H" },
				{ 3, 6 }},
	//26//ten do poprawy bo redyundantny
		test_case{{ "C", "N", "O", "H" },
				{ 1, 1, 2, 5 }},
	//27
		test_case{{ "C", "N", "S", "H" },
				{ 2, 1, 1, 5 }},
	//28
		test_case{{ "C", "N", "H" },
				{ 2, 2, 6 }},
	//29
		test_case{{ "C", "O", "H" },
				{ 3, 3, 4 }},
	//30
		test_case{{ "C", "Cl", "O", "H" },
			{ 3, 3, 1, 3 }},
	//31
		test_case{{ "C", "O", "H" },
				{ 3, 1, 6 }},
	//32
		test_case{{ "C", "Cl", "O", "H" },
				{ 3, 1, 1, 5 }},
	//33
		test_case{{ "C", "O", "H" },
				{ 3, 2, 6 }},
	//34
		test_case{ { "C", "N", "H" },
				{ 3, 1, 7 } },
	//35
		test_case{ { "C", "H" },
				{ 3, 8 } },
	//36
		test_case{ { "C", "O", "H" },
				{ 4, 1, 6 } },
	//37
		test_case{ { "C", "Cl", "H" },
				{ 3, 1, 7 } },
	//38
		test_case{{ "C","O","N","H" },
				{ 2, 2, 1, 7 }},
	//39
		test_case{ { "C", "O", "H" },
				{ 3, 3, 6 } },
	//40
		test_case{ { "C","O","N","H" },
				{ 3, 1, 1, 7 } },
	//41
		test_case{ { "C", "O", "H" },
				{ 4, 4, 4 } },
	//42
		test_case{ { "C","O","S","H" },
				{ 3, 2, 1, 6 } },
};

void show_graphs(std::vector<Graph>& good_graphs) {
	for (int s = 0; s < good_graphs.size(); s++) {
				//std::cout << "GRAF " << s << std::endl;
		std::cout << "-" << std::endl;
		for (int i = 0; i < good_graphs[s].single_graph.size(); i++) {
			for (int j = 0; j < good_graphs[s].single_graph[i].size(); j++) {
				std::cout << good_graphs[s].single_graph[i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	std::cout << "-" << std::endl;
	for (int i = 0; i < ELEMENTS_TABLE.size(); i++) {
		std::cout << i << " " << ELEMENTS_TABLE[i];
		std::cout << "\n";
	}
}

int main()
{
	//odczyt z pliku
	int sum_of_elements = 0;
	std::fstream file;
	std::string line;
	int counter = 0;
	//odczyt z pliku
	file.open("formula.txt", std::ios::in);
	//zapis do pliku
	std::ofstream outfile;
	outfile.open("wyniki.txt", std::ios_base::app); // append instead of overwrite



	/*std::ofstream file_out;*/
	//can't enable exception now because of gcc bug that raises ios_base::failure with useless message
	//file.exceptions(file.exceptions() | std::ios::failbit);
	/*file_out.open(filename + ".txt", std::ios::out | std::ios::app);
	if (file_out.fail())
		file_out << "fail to open file\n";*/

	//make sure write fails with exception if something is wrong
	//file_out.exceptions(file_out.exceptions() | std::ios::failbit | std::ifstream::badbit);

	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			//file_out << line << std::endl;
			outfile << line << '\n';
			std::string tmp;
			for (int i = 0; i < line.size(); i++) {
				if (line[i] != ' ') {
					tmp += line[i];
				}
				else {
					if (counter == 0) {
						ELEMENTS.push_back(tmp);
					}
					else {
						QUANTITY.push_back(std::stoi(tmp));
					}
					tmp.clear();
				}

			}
			if (tmp.size() != 0) {
				if (counter == 0) {
					ELEMENTS.push_back(tmp);
				}
				else {
					QUANTITY.push_back(std::stoi(tmp));
				}
				tmp.clear();
			}
			counter++;
		}
		file.close();
	}
	
	//ZAUTOMATYZOWANE TEST CASEY/////////////////////////////////////////////////////////////////////////////////////////////////////

		//std::ofstream file;
		////can't enable exception now because of gcc bug that raises ios_base::failure with useless message
		////file.exceptions(file.exceptions() | std::ios::failbit);
		//file.open(filename+".txt", std::ios::out | std::ios::app);
		//if (file.fail())
		//	throw std::ios_base::failure(std::strerror(errno));

		////make sure write fails with exception if something is wrong
		//file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);



	for (auto& n : QUANTITY)
		sum_of_elements += n;
	quantity_of_elements_in_formula = sum_of_elements;
	ELEMENTS_TABLE = create_elements_table();


		//try{
		//	std::vector<Graph> good_graphs;
		//	outfile << "ZWYKLE DRZEWO - IZOMORFIZM:\n";
		//	start = clock();
		//	set_mode(isomorphism);
		//	tree_solution(quantity_of_elements_in_formula, good_graphs);
		//	stop = clock();

		//	outfile << "CZAS SZUKANIA: " << ((double)(stop - start) / CLOCKS_PER_SEC) << "s" << std::endl;
		//	outfile << "ILOŚĆ GRAFOW NIEIZOMORFICZNYCH: " << good_graphs.size() << std::endl << std::endl;
		//	show_graphs(good_graphs);
		//}
		//catch (const std::exception&) {
		//	outfile << "ERROR IN ZWYKLE DRZEWO - IZOMORFIZM:\n\n";
		//}
		////std::cout << "-----------------------------------------------\n";
		//try{
		//	std::vector<Graph> good_graphs;
		//	outfile << "ZWYKLE DRZEWO - ETYKIETOWANIE:\n";
		//	start = clock();
		//	set_mode(labeling);
		//	//tu zapisz ze zwykłe drzewo i label
		//	tree_solution(quantity_of_elements_in_formula, good_graphs);
		//	stop = clock();

		//	outfile << "CZAS SZUKANIA: " << ((double)(stop - start) / CLOCKS_PER_SEC) << "s" << std::endl;
		//	outfile << "ILOŚĆ GRAFOW NIEIZOMORFICZNYCH: " << good_graphs.size() << std::endl << std::endl;
		//	show_graphs(good_graphs);
		//}
		//catch (const std::exception&) {
		//	outfile << "ERROR IN ZWYKLE DRZEWO - ETYKIETOWANIE:\n\n";
		//}
		////std::cout << "-----------------------------------------------\n";
		//try{
		//	std::vector<Graph> good_graphs;
		//	outfile << "WODORFREE DRZEWO - IZOMORFIZM:\n";
		//	start = clock();
		//	set_mode(isomorphism);
		//	//tu zapisz ze wodórfree drzewo i izo
 	//		tree_solution_without_hydrogens(quantity_of_elements_in_formula, good_graphs);
		//	stop = clock();

		//	outfile << "CZAS SZUKANIA: " << ((double)(stop - start) / CLOCKS_PER_SEC) << "s" << std::endl;
		//	outfile << "ILOŚĆ GRAFOW NIEIZOMORFICZNYCH: " << good_graphs.size() << std::endl << std::endl;
		//	show_graphs(good_graphs);
		//}
		//catch (const std::exception&) {
		//	outfile << "ERROR IN WODORFREE DRZEWO - IZOMORFIZM:\n\n";
		//}

		//try{
		//	std::vector<Graph> good_graphs;
		//	outfile << "WODORFREE DRZEWO - ETYKIETOWANIE:\n";
		//	start = clock();
		//	set_mode(labeling);
		//	//tu zapisz ze wodórfree drzewo i label
		//	tree_solution_without_hydrogens(quantity_of_elements_in_formula, good_graphs);
		//	stop = clock();

		//	outfile << "CZAS SZUKANIA: " << ((double)(stop - start) / CLOCKS_PER_SEC) << "s" << std::endl;
		//	outfile << "ILOŚĆ GRAFOW NIEIZOMORFICZNYCH: " << good_graphs.size() << std::endl << std::endl;
		//	show_graphs(good_graphs);
		//}
		//catch (const std::exception&) {
		//	outfile << "ERROR IN WODORFREE DRZEWO - ETYKIETOWANIE:\n\n";
		//}

	std::vector<Graph> good_graphs;
	set_mode(labeling);
	tree_solution_without_hydrogens(quantity_of_elements_in_formula, good_graphs);
	show_graphs(good_graphs);
	
}