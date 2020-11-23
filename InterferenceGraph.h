#pragma once


#include "IR.h"

class InterferenceGraph
{
public:
	Variables variables;
	InterferenceMatrix matrix;
	InterferenceGraph()
	{
	}
	void buildInterferenceGraph(Instructions instructions, Variables v);
	void printInterferenceMatrix();
};

