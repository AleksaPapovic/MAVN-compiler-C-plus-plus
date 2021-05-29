#include "InterferenceGraph.h"
#include <iomanip>
#include "Constants.h"

using namespace std;

InterferenceGraph::InterferenceGraph(Variables& vars)
	: regVariables(vars)
{
}

InterferenceGraph::~InterferenceGraph()
{
}

/* Creates interference graph (matrix) from instructions. */
void InterferenceGraph::BuildInterferenceGraph(Instructions& instructions)
{
	// Initialize interference matrix to the size*size of reg vars
	ResizeInterferenceMatrix(regVariables.size());

	// In every instructions, for all defined variables add
	// interference between defined variable and all out
	// variables from instruction.
	for (Instructions::iterator it = instructions.begin();
		it != instructions.end();
		it++)
	{
		for (Variables::iterator defVar = (*it)->m_def.begin();
			defVar != (*it)->m_def.end();
			defVar++)
		{
			for (Variables::iterator outVar = (*it)->m_out.begin();
				outVar != (*it)->m_out.end();
				outVar++)
			{
				int defPos = (*defVar)->getPos();
				int outPos = (*outVar)->getPos();

				// Variable can't be in interference with itself
				if (defPos != outPos)
				{
					im[defPos][outPos] = __INTERFERENCE__;
					im[outPos][defPos] = __INTERFERENCE__;
				}
				else
				{
					im[defPos][outPos] = __EMPTY__;
					im[outPos][defPos] = __EMPTY__;
				}
			}
		}
	}
}

/* Resizes matrix to the passed size. */
void InterferenceGraph::ResizeInterferenceMatrix(size_t size)
{
	im.resize(size);

	for (unsigned i = 0; i < im.size(); ++i)
		im[i].resize(size);
}

/* Creates stack from list of register variables. */
void InterferenceGraph::BuildVariableStack()
{
	for (Variables::iterator it = regVariables.begin();
		it != regVariables.end();
		it++)
	{
		varStack.push(*it);
	}
}

/* Applies reg to the variable that has varPos for it's position. */
void InterferenceGraph::ApplyRegToVariable(int varPos, Regs reg)
{
	for (Variables::iterator it = regVariables.begin();
		it != regVariables.end();
		it++)
	{
		if ((*it)->getPos() == varPos)
			(*it)->setAssignment(reg);
	}
}
typedef list<Variable*>::iterator ITV;
typedef list<Instruction*>::iterator ITI;
/* Allocates real registers to variables according to the interference. */
bool InterferenceGraph::ResourceAllocation()
{	
	
		list<Regs> registri; //boje
		list<Variable*>  promenljive;

		for (int i = 0; i < __REG_NUMBER__; i++) {
			registri.push_back((Regs)i); //boje  r0 -r3
		}

		while (!varStack.empty())
		{
			Variable* vrh = varStack.top();
			bool smetnja = false;

			for (ITV it = promenljive.begin(); it != promenljive.end(); it++) {
				if (im[vrh->getPos()][(*it)->getPos()] == __INTERFERENCE__) {
					smetnja = true;
				}
			}

			if (!smetnja) {
				if (registri.empty())
					return false;
				vrh->m_assignment = registri.front();
			}
			else {
				if (!registri.empty()) {
					registri.pop_front();
					if (registri.empty())
						return false;
					vrh->m_assignment = registri.front();
				}
				else { return false; }
			}
			promenljive.push_back(vrh);  //obojeni
			varStack.pop(); //skini promenljivu sa steka
		}


		return true;

}

/* Prints interference matrix to the console. */
void InterferenceGraph::PrintInterferenceMatrix()
{
	// Print first row (names of registers) as header
	printf("   ");
	for (size_t i = 0; i < im.size(); ++i)
		printf("r%-3d", i);

	printf("\n");

	// Print first column (names of registers) and actual data
	for (size_t i = 0; i < im.size(); ++i)
	{
		printf("r%d ", i);

		for (size_t j = 0; j < im[i].size(); ++j)
			printf("%-4d", im[i][j]);

		printf("\n");
	}
}