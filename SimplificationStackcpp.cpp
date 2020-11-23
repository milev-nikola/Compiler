
#include "SimplificationStack.h"
SimplificationStack doSimplification(InterferenceGraph ig, int degree)
{
	SimplificationStack ss;
	int maxPosition; 
	int maxnumberOfNeghioubrous;
	int size = ig.variables.size();
	vector<int> addedVariables(ig.variables.size());
	while (size > 0)
	{
		maxnumberOfNeghioubrous = 0;
		maxPosition = 0;

		for (int i = 0; i < ig.matrix.size(); i++)
		{
			int counter = 0;
			for (int j = 0; j < ig.matrix.size(); j++)
			{
				if (ig.matrix[i][j] == __INTERFERENCE__)
					counter++;
			}
			if (counter > maxnumberOfNeghioubrous)
			{
				maxnumberOfNeghioubrous = counter;
				maxPosition = i; 
			}
		}

		if (maxnumberOfNeghioubrous > degree)
		{
			throw runtime_error("Error: Spill happened");
		}

		for (int i = 0; i < ig.matrix.size(); i++)
		{
			ig.matrix[maxPosition][i] = 0;
			ig.matrix[i][maxPosition] = 0;
		}

		if (maxnumberOfNeghioubrous != 0) 
		{
			for (auto it = ig.variables.begin(); it != ig.variables.end(); it++)
			{
				if ((*it)->m_position == maxPosition)
					ss.push(*it);
			}
			addedVariables.push_back(maxPosition);
		}
		else 
		{
			bool onStack;
			for (auto it = ig.variables.begin(); it != ig.variables.end(); it++)
			{
				onStack = false;
				for (int i = 0; i < addedVariables.size(); i++)
					if ((*it)->m_position == addedVariables[i])
						onStack = true;
				if (!onStack)
					ss.push(*it);
			}

			return ss;
		}
		size--;
	}
}