#pragma once
#include "graph_class.h"
#include "isomorphism.h"
#include "utils.h"

void tree_solution(int elements, std::vector<Graph>& good_graphs);
void tree_solution_without_hydrogens(int elements, std::vector<Graph>& good_graphs);
void set_mode(int mode_to_set);