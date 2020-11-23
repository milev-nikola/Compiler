#include <iostream>
#include <iomanip>

#include <cctype>

#include "SyntaxAnalysis.h"

using namespace std;
int counter = 0;
int counter1 = 0;

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex)
	: lexicalAnalysis(lex), errorFound(false), tokenIterator(lexicalAnalysis.getTokenList().begin())
{
}


bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();

	//TO DO: Call function for the starting non-terminal symbol
	Q();

	return !errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}


void SyntaxAnalysis::printTokenInfo(Token token)
{
	cout << setw(20) << left << token.tokenTypeToString(token.getType());
	cout << setw(25) << right << token.getValue() << endl;
}


void SyntaxAnalysis::eat(TokenType t)
{
	if (errorFound == false)
	{
		if (currentToken.getType() == t)
		{
			if (currentToken.getType() != T_END_OF_FILE) {
				cout << currentToken.getValue() << endl;
				currentToken = getNextToken();
			}
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
}


Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}


void SyntaxAnalysis::Q()
{
	if (errorFound == false)
	{
		S();
		eat(T_SEMI_COL);
		L();
	}
}


void SyntaxAnalysis::S()
{

	if (currentToken.getType() == T_MEM) {
		eat(T_MEM);
		if (currentToken.getValue()[0] != 'm')
		{
			errorFound = true;
			throw runtime_error("Error: memory variable is not named properly");
		}
		for (int i = 0; i < currentToken.getValue().length(); i++)
		{
			if ((i > 0 && !isdigit(currentToken.getValue()[i])))
			{
				errorFound = true;
				throw runtime_error("Error: memory variable is not named properly");
			}
		}
		Variable* var = new Variable(currentToken.getValue(), Variable::VariableType::MEM_VAR);
		eat(T_M_ID);
		var->m_value = currentToken.getValue();
		eat(T_NUM);
		if (variableExists(var))
		{
			errorFound = true;
			throw runtime_error("Error: Memory variable is already defined");
		}
		variables.push_back(var);
	}
	else if (currentToken.getType() == T_REG) {
		eat(T_REG);
		if (currentToken.getValue()[0] != 'r')
		{
			errorFound = true;
			throw runtime_error("Error: registry variable is not named properly");
		}
		for (int i = 0; i < currentToken.getValue().length(); i++)
		{
			if ((i > 0 && !isdigit(currentToken.getValue()[i])))
			{
				errorFound = true;
				throw runtime_error("Error: registry variable is not named properly");
			}
		}
		Variable* var = new Variable(currentToken.getValue(), Variable::VariableType::REG_VAR);
		////////ADD IFVARIABLE EXISTS///////
		if (variableExists(var))
		{
			errorFound = true;
			throw runtime_error("Error: Registry variable is already defined");
		}
		eat(T_R_ID);
		var->m_position = counter1;
		variables.push_back(var);
		counter1++;
	}
	else if (currentToken.getType() == T_FUNC) {
		eat(T_FUNC);
		for (int i = 0; i < currentToken.getValue().length(); i++)
		{
			if ((i == 0 && !isalpha(currentToken.getValue()[i])) || i > 0 && !isalnum(currentToken.getValue()[i]))
			{
				errorFound = true;
				throw runtime_error("Error: Function is not named properly");
			}
		}
		Label* lab = new Label(counter, "main");
		labels.push_back(lab);
		eat(T_ID);
	}
	else if (currentToken.getType() == T_ID) {
		Label* lab = new Label(counter, currentToken.getValue());
		/////ADD IFLABEL EXISTS///////
		if (labelExists(lab))
		{
			throw runtime_error("ERROR: Label is already defined");
			errorFound = true;
		}
		eat(T_ID);
		eat(T_COL);
		E();
		labels.push_back(lab);
	}
	else {
		E();
	}
}


void SyntaxAnalysis::L()
{
	if (currentToken.getType() == T_END_OF_FILE) {
		eat(T_END_OF_FILE);
	}
	else {
		Q();
	}
}


void SyntaxAnalysis::E()
{
	if (currentToken.getType() == T_ADD) {
		eat(T_ADD);
		Instruction* inst = new Instruction(counter, I_ADD);
		AddVariablesInInstruction(inst, false, true);
		eat(T_R_ID);
		eat(T_COMMA);
		AddVariablesInInstruction(inst, true, true);
		eat(T_R_ID);
		eat(T_COMMA);
		AddVariablesInInstruction(inst, true, true);
		eat(T_R_ID);
		instructions.push_back(inst);
		counter++;
	}
	else if (currentToken.getType() == T_ADDI) {
		eat(T_ADDI);
		Instruction* inst = new Instruction(counter, I_ADDI);
		AddVariablesInInstruction(inst, false, true);
		eat(T_R_ID);
		eat(T_COMMA);
		AddVariablesInInstruction(inst, true, true);
		eat(T_R_ID);
		eat(T_COMMA);
		inst->number = currentToken.getValue();
		eat(T_NUM);
		instructions.push_back(inst);
		counter++;
	}
	else if (currentToken.getType() == T_SUB) {
		eat(T_SUB);
		Instruction* inst = new Instruction(counter, I_SUB);
		AddVariablesInInstruction(inst, false, true);
		eat(T_R_ID);
		eat(T_COMMA);
		AddVariablesInInstruction(inst, true, true);
		eat(T_R_ID);
		eat(T_COMMA);
		AddVariablesInInstruction(inst, true, true);
		eat(T_R_ID);
		instructions.push_back(inst);
		counter++;
	}
	else if (currentToken.getType() == T_LA) {
		eat(T_LA);
		Instruction* inst = new Instruction(counter, I_LA);
		AddVariablesInInstruction(inst, false, true);
		eat(T_R_ID);
		eat(T_COMMA);
		AddVariablesInInstruction(inst, true, false);
		eat(T_M_ID);
		instructions.push_back(inst);
		counter++;
	}
	else if (currentToken.getType() == T_LW) {
		eat(T_LW);
		Instruction* inst = new Instruction(counter, I_LW);
		AddVariablesInInstruction(inst, false, true);
		eat(T_R_ID);
		eat(T_COMMA);
		inst->number = currentToken.getValue();
		eat(T_NUM);
		eat(T_L_PARENT);
		AddVariablesInInstruction(inst, true, true);
		eat(T_R_ID);
		eat(T_R_PARENT);
		instructions.push_back(inst);
		counter++;
	}
	else if (currentToken.getType() == T_LI) {
		eat(T_LI);
		Instruction* inst = new Instruction(counter, I_LI);
		AddVariablesInInstruction(inst, false, true);
		eat(T_R_ID);
		eat(T_COMMA);
		inst->number = currentToken.getValue();
		eat(T_NUM);
		instructions.push_back(inst);
		counter++;
	}
	else if (currentToken.getType() == T_SW) {
		eat(T_SW);
		Instruction* inst = new Instruction(counter, I_SW);
		AddVariablesInInstruction(inst, true, true);
		eat(T_R_ID);
		eat(T_COMMA);
		inst->number = currentToken.getValue();
		eat(T_NUM);
		eat(T_L_PARENT);
		AddVariablesInInstruction(inst, true, true);
		eat(T_R_ID);
		eat(T_R_PARENT);
		instructions.push_back(inst);
		counter++;
	}
	else if (currentToken.getType() == T_B) {
		eat(T_B);
		Instruction* inst = new Instruction(counter, I_B);
		inst->lab = currentToken.getValue();
		eat(T_ID);
		instructions.push_back(inst);
		counter++;
	}
	else if (currentToken.getType() == T_BLTZ) {
		eat(T_BLTZ);
		Instruction* inst = new Instruction(counter, I_BLTZ);
		AddVariablesInInstruction(inst, true, true);
		eat(T_R_ID);
		eat(T_COMMA);
		inst->lab = currentToken.getValue();
		eat(T_ID);
		instructions.push_back(inst);
		counter++;
	}
	else if (currentToken.getType() == T_AND) {
			eat(T_AND);
			Instruction* inst = new Instruction(counter, I_AND);
			AddVariablesInInstruction(inst, false, true);
			eat(T_R_ID);
			eat(T_COMMA);
			AddVariablesInInstruction(inst, true, true);
			eat(T_R_ID);
			eat(T_COMMA);
			AddVariablesInInstruction(inst, true, true);
			eat(T_R_ID);
			instructions.push_back(inst);
			counter++;
	}
	else if (currentToken.getType() == T_OR) {
		eat(T_OR);
		Instruction* inst = new Instruction(counter, I_OR);
		AddVariablesInInstruction(inst, false, true);
		eat(T_R_ID);
		eat(T_COMMA);
		AddVariablesInInstruction(inst, true, true);
		eat(T_R_ID);
		eat(T_COMMA);
		AddVariablesInInstruction(inst, true, true);
		eat(T_R_ID);
		instructions.push_back(inst);
		counter++;
	}
	else if (currentToken.getType() == T_NOR) {
	eat(T_NOR);
	Instruction* inst = new Instruction(counter, I_NOR);
	AddVariablesInInstruction(inst, false, true);
	eat(T_R_ID);
	eat(T_COMMA);
	AddVariablesInInstruction(inst, true, true);
	eat(T_R_ID);
	eat(T_COMMA);
	AddVariablesInInstruction(inst, true, true);
	eat(T_R_ID);
	instructions.push_back(inst);
	counter++;
	}
	else if (currentToken.getType() == T_BGTZ) {
	eat(T_BGTZ);
	Instruction* inst = new Instruction(counter, I_BGTZ);
	AddVariablesInInstruction(inst, true, true);
	eat(T_R_ID);
	eat(T_COMMA);
	inst->lab = currentToken.getValue();
	eat(T_ID);
	instructions.push_back(inst);
	counter++;
	}
	else if(currentToken.getType() == T_SEQ) {
	eat(T_SEQ);
	Instruction* inst = new Instruction(counter, I_SEQ);
	AddVariablesInInstruction(inst, false, true);
	eat(T_R_ID);
	eat(T_COMMA);
	AddVariablesInInstruction(inst, true, true);
	eat(T_R_ID);
	eat(T_COMMA);
	AddVariablesInInstruction(inst, true, true);
	eat(T_R_ID);
	instructions.push_back(inst);
	counter++;

	}
	else {
		eat(T_NOP);
		Instruction* inst = new Instruction(counter, I_NOP);
		instructions.push_back(inst);
		counter++;
	}

}

void SyntaxAnalysis::AddVariablesInInstruction(Instruction* inst, bool isSource, bool isReg)
{
	Variable* var = new Variable();
	var->m_name = currentToken.getValue();

	if (isReg)
		var->m_type = Variable::VariableType::REG_VAR;
	else
		var->m_type = Variable::VariableType::MEM_VAR;
	if (!variableExists(var)) {
		errorFound = true;
		throw runtime_error("Error: Variable has not been defined");
	}
	if (isSource)
	{
		inst->m_src.push_back(GetVariable(var));
		inst->m_use.push_back(GetVariable(var));
	}
	else
	{
		inst->m_def.push_back(GetVariable(var));
		inst->m_dst.push_back(GetVariable(var));
	}

}

void SyntaxAnalysis::CreatePredAndSucc()
{
	for (auto it = instructions.begin(); it != instructions.end(); ++it)
	{
		if ((*it)->m_type == InstructionType::I_B)
		{
			for (auto it2 = labels.begin(); it2 != labels.end(); it2++)
				if ((*it2)->l_name == (*it)->lab)
				{
					for (auto it3 = instructions.begin(); it3 != instructions.end(); ++it3)
					{
						if ((*it2)->position == (*it3)->m_position)
						{
							(*it)->m_succ.push_back(*it3);
							(*it3)->m_pred.push_back(*it);
						}
					}
				}
		}
		else
		{
			for (auto it2 = instructions.begin(); it2 != instructions.end(); ++it2)
			{
				if ((*it)->m_position + 1 == (*it2)->m_position)
				{
					(*it)->m_succ.push_back((*it2));
					(*it2)->m_pred.push_back((*it));


					if ((*it)->m_type == InstructionType::I_BLTZ || (*it)->m_type == InstructionType::I_BGTZ)
					{
						for (auto it3 = labels.begin(); it3 != labels.end(); ++it3)
						{
							if ((*it3)->l_name == (*it)->lab)
							{
								for (auto it4 = instructions.begin(); it4 != instructions.end(); ++it4)
								{
									if ((*it3)->position == (*it4)->m_position)
									{
										(*it)->m_succ.push_back(*it4);
										(*it4)->m_pred.push_back(*it);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void SyntaxAnalysis::PrintInstructions(Instructions* instruc)
{
	string InstType[16] = { "NO_TYPE", "ADD", "ADDI", "SUB", "LA", "LI", "LW", "SW", "BLTZ", "B", "NOP","OR","AND","NOR","BGTZ","SEQ" };

	for (Instructions::iterator it = instruc->begin(); it != instruc->end(); it++)
	{
		int counter = 1;
		cout << "Type: " << InstType[(*it)->m_type] << "	Pos: " << (*it)->m_position << endl;
		cout << "------------------------------------------" << endl;
		for (Variables::iterator i = (*it)->m_dst.begin(); i != (*it)->m_dst.end(); i++)
		{
			cout << "Dest" << counter++ << ":	Naziv: " << (*i)->m_name << ",  Pos: " << (*i)->m_position
				<< ",  Assignment: " << (*i)->m_assignment << endl;
		}
		counter = 1;
		for (Variables::iterator i = (*it)->m_src.begin(); i != (*it)->m_src.end(); i++)
		{
			cout << "Src" << counter++ << ":	Naziv: " << (*i)->m_name << ",  Pos: " << (*i)->m_position
				<< ",  Assignment: " << (*i)->m_assignment << endl;
		}
		cout << "------------------------------------------" << endl;
		cout << "PRED:	";
		for (Instructions::iterator itt = (*it)->m_pred.begin(); itt != (*it)->m_pred.end(); itt++)
		{
			cout << (*itt)->m_position << " ";
		}
		cout << endl;
		cout << "SUCC:	";
		for (Instructions::iterator itt = (*it)->m_succ.begin(); itt != (*it)->m_succ.end(); itt++)
		{
			cout << (*itt)->m_position << " ";

		}
		cout << endl;
		cout << "USE:	";
		for (Variables::iterator i = (*it)->m_use.begin(); i != (*it)->m_use.end(); i++)
		{
			if ((*i)->m_type != Variable::VariableType::MEM_VAR)
				cout << (*i)->m_name << '\t';
		}
		cout << endl;
		cout << "DEF:	";
		for (Variables::iterator i = (*it)->m_def.begin(); i != (*it)->m_def.end(); i++)
		{
			if ((*i)->m_type != Variable::VariableType::MEM_VAR)
				cout << (*i)->m_name << '\t';
		}
		cout << endl;
		cout << "IN:	";
		for (Variables::iterator i = (*it)->m_in.begin(); i != (*it)->m_in.end(); i++)
		{
			if ((*i)->m_type != Variable::VariableType::MEM_VAR)
				cout << (*i)->m_name << '\t';
		}
		cout << endl;
		cout << "Out:	";
		for (Variables::iterator i = (*it)->m_out.begin(); i != (*it)->m_out.end(); i++)
		{
			if ((*i)->m_type != Variable::VariableType::MEM_VAR)
				cout << (*i)->m_name << '\t';
		}
		cout << endl << "==========================================" << endl;
		cout << "==========================================" << endl;
	}
}

Variable* SyntaxAnalysis::GetVariable(Variable* temp)
{
	for (auto it = variables.begin(); it != variables.end(); it++)
	{
		if ((*it)->m_name == temp->m_name)
			return *it;
	}
}

bool SyntaxAnalysis::variableExists(Variable* var)
{
	for (auto it = variables.begin(); it != variables.end(); it++)
	{
		if ((*it)->m_name == var->m_name)
		{
			return true;
		}
	}
	return false;

}

bool SyntaxAnalysis::labelExists(Label* lab)
{
	for (auto it = labels.begin(); it != labels.end(); it++)
	{
		if ((*it)->l_name == lab->l_name)
		{
			return true;
		}
	}
	return false;
}