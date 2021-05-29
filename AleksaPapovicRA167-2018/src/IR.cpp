#include "IR.h"

using namespace std;

bool variableExists(Variable* j, Variables def) {
	bool ret=false;
	for(Variables::iterator it = def.begin(); it!= def.end();it++){
		if (j->getName() == (*it)->getName()) {
			ret = true;
			break;
		}
		
	}
	return ret;

}
void LivenessAnalysis(Instructions& instructions)
{
	Variables inprim,outprim;
	Variables in, out;
	do{
		for (Instructions::reverse_iterator ri = instructions.rbegin(); ri != instructions.rend(); ri++) {
			
			inprim = (*ri)->m_in;
			outprim = (*ri)->m_out;
			(*ri)->m_out.clear();
			for (Instructions::iterator i = (*ri)->m_succ.begin(); i != (*ri)->m_succ.end(); i++)
				for (Variables::iterator vi = (*i)->m_in.begin(); vi != (*i)->m_in.end(); vi++)
					(*ri)->m_out.push_back((*vi));
			
			(*ri)->m_out.sort();
			(*ri)->m_out.unique();
			(*ri)->m_in = (*ri)->m_use;

			for (Variables::iterator j = (*ri)->m_out.begin(); j != (*ri)->m_out.end(); j++)
				if (!variableExists(*j, (*ri)->m_def))  (*ri)->m_in.push_back((*j));

			(*ri)->m_in.sort();
			(*ri)->m_in.unique();
			in = (*ri)->m_in;
			out = (*ri)->m_out;

		}
	}
	while( in != inprim || out != outprim );
	
	
}


ostream& operator<<(ostream& out, Instruction& i)
{
	Variable* v;

	switch (i.m_type)
	{
	case I_ADD: // add rid,rid,rid
		out << "add $t" << i.m_dst.front()->getAssignment();
		for (Variables::iterator it = i.m_src.begin(); it != i.m_src.end(); it++)
			out << ", $t" << (*it)->getAssignment();

		break;

	case I_SUB: // sub rid,rid,rid
		out << "sub $t" << i.m_dst.front()->getAssignment();
		for (Variables::iterator it = i.m_src.begin(); it != i.m_src.end(); it++)
			out << ", $t" << (*it)->getAssignment();

		break;

	case I_ADDI: // addi rid,rid,num
		out << "addi $t" << i.m_dst.front()->getAssignment() << ", $t";
		out << i.m_src.front()->getAssignment() << ", "
			<< (reinterpret_cast<ConstInstruction*>(&i))->getNumValue();
		break;

	

	case I_LA: // la rid,mid
		out << "la $t" << i.m_dst.front()->getAssignment() << ", "
			<< i.m_src.front()->getName();
		break;

	case I_LW: // lw rid,num(rid)
		out << "lw $t" << i.m_dst.front()->getAssignment();
		v = i.m_src.front();
		out << ", " << (reinterpret_cast<ConstInstruction*>(&i))->getNumValue()
			<< "($t" << v->getAssignment() << ")";
		break;

	

	case I_SW: // sw rid,num(rid)
		out << "sw $t" << i.m_src.back()->getAssignment();
		v = i.m_src.front();
		out << ", " << (reinterpret_cast<ConstInstruction*>(&i))->getNumValue()
			<< "($t" << v->getAssignment() << ")";
		break;

	case I_LI: // li rid,num
		out << "li $t" << i.m_dst.front()->getAssignment() << ", ";
		out << (reinterpret_cast<ConstInstruction*>(&i))->getNumValue();
		break;

	case I_BLTZ: // bltz rid,id
		out << "bltz $t" << i.m_src.front()->getAssignment() << ", " << i.labelName;
		break;

	case I_B:   // b id
		out << "b " << i.labelName;
		break;

	case I_NOP: // nop
		out << "nop";
		break;
	}

	return out;
}

bool nadjiVarName(Variable& variable, Variables& variableList)
{
	for (Variables::const_iterator it = variableList.begin();
		it != variableList.end();
		it++)
	{
		if (variable.getName() == (*it)->getName())
			return true;
	}

	return false;
}

void PrintInstructions(Instructions& instructions)
{
	for (Instructions::iterator it = instructions.begin();
		it != instructions.end();
		it++)
	{
		(*it)->PrintInstruction();
		cout << "--------------------------------------" << endl;
	}
}

std::ostream& operator<<(std::ostream& out, const Variable& v)
{
	out << v.m_name;

	return out;
}
void PrintVariables(const Variables& vars)
{
	for (Variables::const_iterator it = vars.begin(); it != vars.end(); it++)
		cout << *(*it) << " ";
}
void Instruction::PrintInstruction()
{
	cout << "Instruction " << m_position << endl;

	cout << "SRC: ";
	PrintVariables(m_src);

	cout << endl << "DEST: ";
	PrintVariables(m_dst);

	cout << endl << "USE: ";
	PrintVariables(m_use);

	cout << endl << "DEF: ";
	PrintVariables(m_def);

	cout << endl << "IN: ";
	PrintVariables(m_in);

	cout << endl << "OUT: ";
	PrintVariables(m_out);

	cout << endl;
}
void Instruction::FillUseDefVariables()
{
	switch (m_type)
	{
	case I_ADD:  // add rid,rid,rid
	case I_SUB:  // sub rid,rid,rid
	case I_ADDI: // addi rid,rid,num
	case I_LW: // lw rid,num(rid)
	 // addu rid,rid,rid
		m_def.insert(m_def.end(), m_dst.begin(), m_dst.end());
		m_use.insert(m_use.end(), m_src.begin(), m_src.end());
		break;

	case I_LA: // la rid,mid
	case I_LI: // li rid,num
		m_def.insert(m_def.end(), m_dst.begin(), m_dst.end());
		break;

	case I_SW: // sw rid,num(rid)
		m_use.insert(m_use.end(), m_src.begin(), m_src.end());
		break;

	case I_BLTZ: // bltz rid,id
		m_use.insert(m_use.end(), m_src.begin(), m_src.end());
		break;

	case I_B:   // b id
	case I_NOP: // nop
		break;
	}
}