#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "graph_class.h"

int label_len(int elements);
void change_matrix(int** matrix, std::vector<int>elements_order);
int compare_labels(std::vector<std::vector<int>>& graph1, std::vector<int>& graph, int elements);
bool is_label_the_biggest(std::vector<std::vector<int>>& graph_to_check, int elements);