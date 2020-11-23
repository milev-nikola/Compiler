#pragma once

#include "Types.h"
#include "IR.h"
#include "InterferenceGraph.h"


SimplificationStack doSimplification(InterferenceGraph ig, int degree);
