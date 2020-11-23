#include <iostream>
#include <exception>
#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "IR.h"
#include "LivenessAnalysis.h"
#include "InterferenceGraph.h"
#include "SimplificationStack.h"
#include "ResourceAllocation.h"
using namespace std;

void writeToFile(SyntaxAnalysis syntax, string name)
{
	string arr[5] = { "$t0","$t1","$t2","$t3","$t4" };
	ofstream ofs(name);
	if (ofs)
	{
		ofs << ".globl main" << endl;
		ofs << endl;
		ofs << ".data" << endl;
		for (auto it = syntax.variables.begin(); it != syntax.variables.end(); it++)
		{
			if ((*it)->m_type == Variable::VariableType::MEM_VAR)
			{
				ofs << (*it)->m_name << "\t" << ".word " << (*it)->m_value << endl;;
			}
		}
		ofs << ".text" << endl;
		for (auto it = syntax.instructions.begin(); it != syntax.instructions.end(); it++)
		{
			for (auto it1 = syntax.labels.begin(); it1 != syntax.labels.end(); it1++)
			{
				if ((*it)->m_position == (*it1)->position)
				{
					ofs << (*it1)->l_name << ":" << endl;
				}
			}
			if ((*it)->m_type == I_ADD)
			{
				ofs << "add \t" << arr[(*it)->m_dst.front()->m_assignment] << ", " << arr[(*it)->m_src.front()->m_assignment] << ", " << arr[(*it)->m_src.back()->m_assignment] << endl;
			}
			else if ((*it)->m_type == I_ADDI)
			{
				ofs << "addi \t" << arr[(*it)->m_dst.front()->m_assignment] << ", " << arr[(*it)->m_src.front()->m_assignment] << ", " << (*it)->number << endl;
				cout << (*it)->number << endl;
			}
			else if ((*it)->m_type == I_SUB)
			{
				ofs << "sub \t" << arr[(*it)->m_dst.front()->m_assignment] << ", " << arr[(*it)->m_src.front()->m_assignment] << ", " << arr[(*it)->m_src.back()->m_assignment] << endl;
			}
			else if ((*it)->m_type == I_LA)
			{
				ofs << "la  \t" << arr[(*it)->m_dst.front()->m_assignment] << ", " << (*it)->m_src.front()->m_name << endl;
			}
			else if ((*it)->m_type == I_LI)
			{
				ofs << "li  \t" << arr[(*it)->m_dst.front()->m_assignment] << ", " << (*it)->number << endl;
			}
			else if ((*it)->m_type == I_LW)
			{
				ofs << "lw  \t" << arr[(*it)->m_dst.front()->m_assignment] << ", " << "(" << (*it)->number << ")" << arr[(*it)->m_src.front()->m_assignment] << endl;
			}
			else if ((*it)->m_type == I_SW)
			{
				ofs << "sw  \t" << arr[(*it)->m_src.front()->m_assignment] << ", " << "(" << (*it)->number << ")" << arr[(*it)->m_src.back()->m_assignment] << endl;
			}
			else if ((*it)->m_type == I_BLTZ)
			{
				ofs << "bltz    " << arr[(*it)->m_src.front()->m_assignment] << ", " << (*it)->lab << endl;
			}
			else if ((*it)->m_type == I_B)
			{
				ofs << "b  " << (*it)->lab << endl;
			}
			else if ((*it)->m_type == I_NOP)
			{
				ofs << "nop; " << (*it)->lab << endl;
			}
			else if ((*it)->m_type == I_AND)
			{
				ofs << "and \t" << arr[(*it)->m_dst.front()->m_assignment] << ", " << arr[(*it)->m_src.front()->m_assignment] << ", " << arr[(*it)->m_src.back()->m_assignment] << endl;
			}
			else if ((*it)->m_type == I_OR)
			{
				ofs << "or   \t" << arr[(*it)->m_dst.front()->m_assignment] << ", " << arr[(*it)->m_src.front()->m_assignment] << ", " << arr[(*it)->m_src.back()->m_assignment] << endl;
			}
			else if ((*it)->m_type == I_NOR)
			{
				ofs << "nor \t" << arr[(*it)->m_dst.front()->m_assignment] << ", " << arr[(*it)->m_src.front()->m_assignment] << ", " << arr[(*it)->m_src.back()->m_assignment] << endl;
			}
			else if ((*it)->m_type == I_BGTZ)
			{
				ofs << "bgtz    " << arr[(*it)->m_src.front()->m_assignment] << ", " << (*it)->lab << endl;
			}
			else if ((*it)->m_type == I_SEQ)
			{
				ofs << "seq \t" << arr[(*it)->m_dst.front()->m_assignment] << ", " << arr[(*it)->m_src.front()->m_assignment] << ", " << arr[(*it)->m_src.back()->m_assignment] << endl;
			}

			
		}
	}
	ofs.close();
}


int main()
{
	try
	{
		std::string fileName = ".\\..\\examples\\multiply.mavn";
		std::string outFile = ".\\..\\examples\\multiply.s";

		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}
		SyntaxAnalysis syntax(lex);
		retVal = syntax.Do();
		if (retVal)
		{
			cout << "Syntax analysis finished successfully!" << endl;
		}
		else
		{
			throw runtime_error("\nException! Syntax analysis failed!\n");
		}
		Instructions::iterator it = syntax.instructions.begin();
		for (; it != syntax.instructions.end(); it++)
		{
			cout << (*it)->m_position << " " << (*it)->m_type << endl;
		}
		syntax.CreatePredAndSucc();
		Instructions ins = syntax.instructions;
		livenessAnalysis(ins);
		syntax.instructions = ins;
		syntax.PrintInstructions(&syntax.instructions);
		Variables vv;
		for (auto itt = syntax.variables.begin(); itt != syntax.variables.end(); itt++)
		{
			if ((*itt)->m_type == Variable::VariableType::REG_VAR)
			{
				vv.push_back(*itt);
			}
		}
		InterferenceGraph ig;
		ig.buildInterferenceGraph(syntax.instructions, vv);
		ig.printInterferenceMatrix();
		SimplificationStack ss = doSimplification(ig, __REG_NUMBER__);
		doResourceAllocation(ss, ig, syntax.instructions);
		syntax.PrintInstructions(&syntax.instructions);
		writeToFile(syntax, outFile);

	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
