#include "ResourceAllocation.h"

bool CheckInterference(vector<int>& interference, Variable* var)
{
	for (int i = 0; i < interference.size(); i++)
		if (var->m_position == i && interference[i] == __INTERFERENCE__)
			return true;
	return false;
}

bool doResourceAllocation(SimplificationStack& ss, InterferenceGraph& ig, Instructions& all)
{
	vector<Variable*> colored;

	while (!ss.empty())
	{
		vector<Regs> usedColors;
		Variable* variable = ss.top();
		ss.pop();
		vector<int>interference = ig.matrix[variable->m_position];
		for (int i = 0; i < colored.size(); i++)
			if (CheckInterference(interference, colored[i]))
				usedColors.push_back(colored[i]->m_assignment);

		for (int i = 0; i < __REG_NUMBER__; i++)
		{
			bool freeColor = true;
			for (int j = 0; j < usedColors.size(); j++)
				if (usedColors[j] == i)
					freeColor = false;

			if (freeColor)
			{
				variable->m_assignment = (Regs)i;
				colored.push_back(variable);
				break;
			}
		}
	}

	return true;
}