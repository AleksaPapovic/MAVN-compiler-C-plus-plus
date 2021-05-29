#include <iostream>
#include <exception>

#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"

//dodlea instrukcija
#include "IzborInstrukcija.h"
#include "Munch.h"
#include "Registers.h"
#include <cassert>
#include <string>
#include <sstream>
#include "IR.h"
#include "InterferenceGraph.h"

using namespace std;



int main()
{
	try
	{
		std::string fileName = ".\\..\\examples\\multiply.mavn";
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
		//Sintaksni analizator
		Instructions instructions;
		SyntaxAnalysis syntax(lex, instructions);
		
		
		if (syntax.Do())
		{
			cout << "Syntax analysis finished successfully!" << endl;
		}
		else
		{
			cout << "Syntax analysis failed!" << endl;
		}

		syntax.dodajSucc();
		syntax.dodajPred();

		LivenessAnalysis(instructions);

		cout << endl << "Instruction list after liveness analysis" << endl << endl;

		PrintInstructions(instructions);

		InterferenceGraph ig(syntax.getRegVariables());
		ig.BuildInterferenceGraph(instructions);
		ig.BuildVariableStack();

		// Print interference matrix
		cout << endl << "Interference matrix:" << endl;
		ig.PrintInterferenceMatrix();
		string outputFile = ".\\..\\examples\\multiply.s";
		
		if (ig.ResourceAllocation())
			cout << endl << "Resource allocation successful!" << endl;
		else
			throw runtime_error("\nException! Resource allocation failed!!\n");
		
		syntax.CreateMIPSFile(outputFile);
		
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
