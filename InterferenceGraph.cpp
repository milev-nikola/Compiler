#include "InterferenceGraph.h"



void InterferenceGraph::buildInterferenceGraph(Instructions instructions, Variables var)
{
	variables = var;
	for (auto itt = var.begin(); itt != var.end(); itt++)
	{
		vector<int> is;
		for (auto itt1 = var.begin(); itt1 != var.end(); itt1++)
		{
			is.push_back(0);
		}
		matrix.push_back(is);
	}
	for (auto it = instructions.begin(); it != instructions.end(); it++)
	{
		if ((*it)->m_type != InstructionType::I_LI)
		{
			for (auto it2 = (*it)->m_def.begin(); it2 != (*it)->m_def.end(); it2++)
			{
				for (auto it3 = (*it)->m_out.begin(); it3 != (*it)->m_out.end(); it3++)
				{
					if ((*it2)->m_name.c_str() != (*it3)->m_name.c_str() && (*it2)->m_type == (*it3)->m_type)
					{
						matrix[(*it2)->m_position][(*it3)->m_position] = __INTERFERENCE__;
						matrix[(*it3)->m_position][(*it2)->m_position] = __INTERFERENCE__;
					}
				}
			}

		}
	}


}

void InterferenceGraph::printInterferenceMatrix()
{
	if (matrix.size() == 0)
	{
		cout << "There is nothing to print!" << endl;
		return;
	}

	cout << endl << "Interference matrix:" << endl;
	cout << "==========================================" << endl;

	for (auto varIt = variables.begin(); varIt != variables.end(); varIt++)
	{
		cout << "\t" << (*varIt)->m_name;
	}
	cout << endl;

	auto varIt = variables.begin();
	for (auto it1 = matrix.begin(); it1 != matrix.end(); it1++)
	{
		cout << (*varIt++)->m_name;
		for (auto it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
		{
			cout << "\t" << *it2;
		}
		cout << endl;
	}
	cout << "==========================================" << endl;
}