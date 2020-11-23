#pragma once

#include "InterferenceGraph.h"
#include "SimplificationStack.h"
using namespace std;

bool doResourceAllocation(SimplificationStack& ss, InterferenceGraph& ig, Instructions& all);
bool CheckInterference(vector<int>& interference, Variable* v);