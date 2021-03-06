#pragma once

#include <vector>
#include <stack>
#include "IR.h"

typedef std::vector<std::vector<int>> interferenceMatrix;
typedef std::stack<Variable*> variableStack;


/* Class that represents interference graph.
This class build graph(matrix) and does resource allocation.*/
class InterferenceGraph
{
public:
	/* Constructor, destructor */
	InterferenceGraph(Variables& vars);
	~InterferenceGraph();

	/* Creates interference graph (matrix) from instructions. */
	void BuildInterferenceGraph(Instructions& instructions);

	/* Creates stack from list of register variables. */
	void BuildVariableStack();

	/* Allocates real registers to variables according to the interference. */
	bool ResourceAllocation();

	/* Prints interference matrix to the console. */
	void PrintInterferenceMatrix();

	interferenceMatrix im;
	variableStack varStack;  // same as list of instructions, just in form of stack

private:

	/* Resizes matrix to the passed size. */
	void ResizeInterferenceMatrix(size_t size);

	/* Applies reg to the variable that has varPos for it's position. */
	void ApplyRegToVariable(int varPos, Regs reg);

	Variables& regVariables;  // register variables
};
