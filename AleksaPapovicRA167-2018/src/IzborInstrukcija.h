#pragma once

#include "SyntaxAnalysis.h"
#include "Token.h"
#include <string>
#include "IR.h"

class IzborInstrukcija
{
public:

	/**
	* Constructor
	*/
	IzborInstrukcija(SyntaxAnalysis& syn, Instructions& instructions);
	bool odradi();
	void emit(Instruction * instr);
	std::list<Instruction *> & getInstructionList();
	/**
	* Reference to syntax analysis module
	*/
	
	SyntaxAnalysis& syntaxAnalysis;
	Instructions& instructions;

	std::vector<std::string> vectorOperandi;
	std::vector<std::string> vectorInstrukcija;
private:

	

	




	std::vector<std::string> naziviOperandi;

	std::vector<std::string> naziviInstrukcija;

};