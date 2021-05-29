#include "IzborInstrukcija.h"
#include "SyntaxAnalysis.h"

static std::list<Instruction *> selectedInstructions;
using namespace std;

/*list<Instruction *>::iterator isc;
IzborInstrukcija::IzborInstrukcija(SyntaxAnalysis& syn, Instructions& ins): syntaxAnalysis(syn), instructions(ins)
{
	vectorOperandi = syntaxAnalysis.getVectorOperandi();
	vectorInstrukcija = syntaxAnalysis.getVectorInstrukcija();
	
}
bool IzborInstrukcija::odradi()
{
	std::vector<std::string>::iterator ini;
	std::vector<std::string>::iterator ino;
	ino = vectorOperandi.begin();
	for (ini = vectorInstrukcija.begin(); ini != vectorInstrukcija.end(); ini++) {
		int srcReg;
		int dstReg;
		cout << "INSTRUKCIJA" << *ini << endl;
		if ( *ini == "_mem")
		{
			string instructionString;
			Instruction* instr;
			
			srcReg = stoi(ino->substr(1,1));
		    ino++;
			dstReg = stoi(ino->substr(0,1));
			instructionString = "_mem `d, " + ino->substr(0, 1) + ";";
			ino+=2;
			

	       // instructions = new Instruction(I_ADD, srcReg , dstReg);
			//instr->m_src.push_back(srcReg);
			//instr->m_dst.push_back(dstReg);
			//cout << "ovo je instrukcija " << instr->toString();

		}
	//	isc = selectedInstructions.begin();
//	cout << "Izabrali ste selektovanu instrukciju " << (*isc)->toString()<< endl;
	}
	return true;

}

void IzborInstrukcija::emit(Instruction* instr)
{
	selectedInstructions.push_back(instr);
}


list<Instruction *>& IzborInstrukcija::getInstructionList()
{
	return selectedInstructions;
}
*/
