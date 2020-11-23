#ifndef __IR__
#define __IR__
using namespace std;
#include <iostream>
#include "Types.h"


/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(std::string name, VariableType vt) : m_type(vt), m_name(name), m_position(-1), m_assignment(no_assign) {}
	VariableType m_type;
	std::string m_name;
	int m_position;
	string m_value;
	Regs m_assignment;
};


/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;


/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction() : m_position(0), m_type(I_NO_TYPE), number("") {}
	Instruction(int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src), number("") {}
	Instruction(int position, InstructionType it) : m_position(position), m_type(it), number("") {}
	string number;
	int m_position;
	InstructionType m_type;
	string lab;

	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;

class Label
{
public:
	Label() : position(0), l_name("") {}
	Label(int pos, string name) : position(pos), l_name(name) {}
	int position;
	string l_name;
};

typedef std::list<Label*> Labels;
typedef std::vector<vector<int>> InterferenceMatrix;
typedef std::stack<Variable*> SimplificationStack;

#endif
