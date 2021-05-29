#ifndef __IR__
#define __IR__

#include "Types.h"
#include "IR.h"
#include <vector>
/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	enum VariableType
	{
		T_MEM,
		T_REG,
		NO_TYPE
	};
	
	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign){}
	Variable(std::string name) : m_name(name), m_type(NO_TYPE), m_position(-1), m_assignment(no_assign) {}
	//konstruktor sa pozicijom
	Variable(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign) {}
	//konstruktor bez pozicije
	Variable(std::string name, VariableType tip) : m_type(tip), m_name(name), m_assignment(no_assign) {}
	Variable(std::string name, VariableType tip,int pozicija, int vrednost = 0) : m_type(tip), m_name(name), m_position(pozicija),
		m_assignment(no_assign), value(vrednost) {}

	~Variable();

	friend std::ostream& operator<<(std::ostream& out, const Variable& v);

	std::string getName() 
	{
		return m_name;
	}
	int getPos() 
	{
		return m_position;
	}
	int getValue() 
	{
		return value;
	}
	Regs getAssignment()
	{
		return m_assignment;
	}
	VariableType getType()
	{
		return m_type;
	}
	void setAssignment(Regs r)
	{
		m_assignment = r;
	}
	Regs m_assignment;
private:
	int value;
	VariableType m_type;
	std::string m_name;
	int m_position;
	
};


/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;
//Variables listaVariabli;

/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	//konstruktor sa pozicijom
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src) {}
	//konstruktor bez pozicije
	Instruction(InstructionType type, Variables& dst, Variables& src) :
		m_type(type), m_dst(dst), m_src(src) {}

	Instruction(int pos, InstructionType type, Variables& dst, Variables& src,
		const std::string& imeLabele, const std::string& labela) : m_position(pos),
		m_type(type), m_dst(dst), m_src(src), labelName(imeLabele), label(labela) {
		FillUseDefVariables();
	}

	~Instruction();

	friend std::ostream& operator<<(std::ostream& out, Instruction& i);

	void PrintInstruction();


	InstructionType getType()
	{
		return m_type;
	}
	std::string getLabelName()
	{
		return labelName;
	}
	std::string getLabel()
	{
		return label;
	}
	

	int m_position;
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::vector<Instruction*> m_succ;
	std::vector<Instruction*> m_pred;

	std::string label;
	std::string labelName;
private:
	void FillUseDefVariables();
	std::list<Instruction*> m_predi;
};
class ConstInstruction : public Instruction
{
public:
	ConstInstruction(int pos, InstructionType type, Variables& dst, Variables& src,
		int num, const std::string& imeLabele, const std::string& labela) : 
		Instruction(pos, type, dst, src, imeLabele, labela), numValue(num) {}
	~ConstInstruction();

	
	int getNumValue()
	{
		return numValue;
	}

private:
	int numValue;
};

/**
 * This type represents list of instructions from program code.
 */
typedef std::vector<Instruction*> Instructions;
//Instructions listaInstrukcija;

/* Checks if variableList contains variable. */
bool nadjiVarName(Variable& variable, Variables& variableList);

/* Does liveness analysis and created in and out variable lists. */
void LivenessAnalysis(Instructions& instructions);

/* Prints all instructions. */
void PrintInstructions(Instructions& instructions);

/* Prints all variables. */
void PrintVariables(const Variables& vars);

bool variableExists(Variable*, Variables);

#endif
