#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include "SyntaxAnalysis.h"


using namespace std;


SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex, Instructions& instrukcije) : lexicalAnalysis(lex), instructions(instrukcije),errorFound(false),sError(false),linija(0),
tokenIterator(lexicalAnalysis.getTokenList().begin()), brojacInstrukcija(0), brojacVariabli(0)
{
}
vector<string>& SyntaxAnalysis::getVectorOperandi()
{
	return naziviOperandi;
}
std::vector<std::string>& SyntaxAnalysis::getVectorInstrukcija()
{
	return naziviInstrukcija;
}
bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();

	//TO DO: Call function for the starting non-terminal symbol
	Q();
	return !errorFound;
}

Variables & SyntaxAnalysis::getRegVariables()
{
	return regVariables;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	if (sError)
	{
		cout << "Semanticka greska: " << token.getValue() << " neocekivana promenljiva" << endl;
	}
	else
	{
		cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
	}
	}

void SyntaxAnalysis::semanticError(int k)
{
	if (k == 0) 
	{
		if (find(vMem.begin(), vMem.end(), currentToken.getValue()) != vMem.end())
		{
			cout << "SEMANTICKA GRESKA NA LINIJI [" << linija + 1 << "] : DEKLARISANO VISE MEMORIJSKIH PROMENLJIVIH SA ISTIM IMENOM" << endl;
			sError = true;
			throw runtime_error("");
		}
	}
	else if( k == 1)
	{
		if (find(vRegistri.begin(), vRegistri.end(), currentToken.getValue()) != vRegistri.end())
		{
			cout << "SEMANTICKA GRESKA NA LINIJI [" << linija + 1 << "] : DEKLARISANO VISE REGISTRA SA ISTIM IMENOM" << endl;
			sError = true;
			throw runtime_error("");
		}
	}
	else if (k == 2)
	{
		/*if (!(currentToken.getValue().compare("r1") == 0 ||
			currentToken.getValue().compare("r2") == 0 ||
			currentToken.getValue().compare("r3") == 0 ||
			currentToken.getValue().compare("r4") == 0)
			)
		{
			cout << "SEMANTICKA111 GRESKA NA LINIJI [" << linija + 2 << "] : KORISTITE NEPOSTOJECI REGISTAR " << endl;
			throw runtime_error("");
		}*/
	}
	else if(k == 3)
	{
		if (find(vRegistri.begin(), vRegistri.end(), currentToken.getValue()) == vRegistri.end())
		{
			cout << "SEMANTICKA GRESKA NA LINIJI [" << linija + 1 << "] : NIJE DEKLARISAN REGISTAR" << endl;
			sError = true;
			throw runtime_error("");
		}
	}
	else if (k == 4)
	{
		if (find(vMem.begin(), vMem.end(), currentToken.getValue()) == vMem.end())
		{
			cout << "SEMANTICKA GRESKA NA LINIJI [" << linija + 1 << "] : NIJE DEKLARISANA MEMORIJSKA PROMENLJIVA" << endl;
			sError = true;
			throw runtime_error("");
		}
	}
	else if (k == 5)
	{
		if (find(labele.begin(), labele.end(), currentToken.getValue()) != labele.end())
		{
			cout << "SEMANTICKA GRESKA NA LINIJI [" << linija + 1 << "] :  DEKLARISANO VISE LABELA SA ISTIM IMENOM" << endl;
			sError = true;
			throw runtime_error("");
		}
	}
	else if (k == 6)
	{
		if (find(funkcije.begin(), funkcije.end(), currentToken.getValue()) != funkcije.end())
		{
			cout << "SEMANTICKA GRESKA NA LINIJI [" << linija + 1 << "] :  DEKLARISANO VISE FUNKCIJA SA ISTIM IMENOM" << endl;
			sError = true;
			throw runtime_error("");
		}
	}

}


void SyntaxAnalysis::dodajMemVariablu(Token& t, Token& value)
{
	string variableName = t.getValue();
	Variable* variable = new Variable(variableName, Variable::T_MEM,
	  					 brojacVariabli, stoi(value.getValue())) ;

	memVariables.push_back(variable);
}
void SyntaxAnalysis::dodajRegVariablu(Token& t)
{
	string variableName = t.getValue();
	Variable* variable = new Variable(variableName, Variable::T_REG,
								brojacVariabli);
	++brojacVariabli;

	regVariables.push_back(variable);
}
void SyntaxAnalysis::dodajFunc(Token& t)
{
	string imeFunkcije = t.getValue();

	funcs.push_back(imeFunkcije);
	dodajLabelu(imeFunkcije, brojacInstrukcija);
}
void SyntaxAnalysis::dodajLabelu(const string& labelName, int pos)
{
	labels.push_back(pair<string, int>(labelName, pos));
	sLabela = labelName;
}
Variable* SyntaxAnalysis::nadjiRegVariablu(const string& varName)
{
	for (Variables::iterator it = regVariables.begin();
		it != regVariables.end();
		it++)
	{
		if ((*it)->getName() == varName)
			return (*it);
	}

	return NULL;
}
Variable* SyntaxAnalysis::nadjiMemVariablu(const string& varName)
{
	for (Variables::iterator it = memVariables.begin();
		it != memVariables.end();
		it++)
	{
		if ((*it)->getName() == varName)
			return (*it);
	}

	return NULL;
}

void SyntaxAnalysis::makeInstruction(vector<Token>& dst, vector<Token>& src, InstructionType tip)
{
	Variables* destVars = new Variables;
	Variables* srcVars = new Variables;
	Variable* tVar;
	string labelName = "";
	Instruction* instr;

	switch (tip)
	{
	case I_ADD:  // add rid,rid,rid
	case I_SUB:  // sub rid,rid,rid

		tVar = nadjiRegVariablu(dst[0].getValue());
		destVars->push_back(tVar);
		tVar = nadjiRegVariablu(src[0].getValue());
		srcVars->push_back(tVar);
		tVar = nadjiRegVariablu(src[1].getValue());
		srcVars->push_back(tVar);

		instr = new Instruction(brojacInstrukcija, tip, *destVars, *srcVars, labelName, sLabela);
		break;

	case I_ADDI: // addi rid,rid,num

		tVar = nadjiRegVariablu(dst[0].getValue());
		destVars->push_back(tVar);
		tVar = nadjiRegVariablu(src[0].getValue());
		srcVars->push_back(tVar);

		instr = new ConstInstruction(brojacInstrukcija, tip, *destVars, *srcVars,
			stoi(src[1].getValue()), labelName, sLabela);
		break;

	case I_LA: // la rid,mid

		tVar = nadjiRegVariablu(dst[0].getValue());
		destVars->push_back(tVar);
		tVar = nadjiMemVariablu(src[0].getValue());
		srcVars->push_back(tVar);
		instr = new Instruction(brojacInstrukcija, tip, *destVars, *srcVars,
			labelName, sLabela);
		break;

	case I_LW: // lw rid,num(rid)
		tVar = nadjiRegVariablu(dst[0].getValue());
		destVars->push_back(tVar);

		tVar = nadjiRegVariablu(src[1].getValue());
		srcVars->push_back(tVar);

		instr = new ConstInstruction(brojacInstrukcija, tip, *destVars, *srcVars,
			stoi(src[0].getValue()), labelName, sLabela);
		break;
	case I_SW: // sw rid,num(rid)

		tVar = nadjiRegVariablu(dst[0].getValue());
		srcVars->push_back(tVar);

		tVar = nadjiRegVariablu(src[1].getValue());
		srcVars->push_back(tVar);

		instr = new ConstInstruction(brojacInstrukcija, tip, *destVars, *srcVars,
			stoi(src[0].getValue()), labelName, sLabela);
		break;

	case I_LI: // li rid,num

		tVar = nadjiRegVariablu(dst[0].getValue());
		destVars->push_back(tVar);

		instr = new ConstInstruction(brojacInstrukcija, tip, *destVars, *srcVars,
			stoi(src[0].getValue()), labelName, sLabela);
		break;

	case I_BLTZ: // bltz rid,id

		tVar = nadjiRegVariablu(src[0].getValue());
		srcVars->push_back(tVar);

		labelName = dst[0].getValue();


		instr = new Instruction(brojacInstrukcija, tip, *destVars, *srcVars,
			labelName, sLabela);
		break;

	case I_B:   // b id
		labelName = dst[0].getValue();


		instr = new Instruction(brojacInstrukcija, tip, *destVars, *srcVars,
			labelName, sLabela);
		break;

	case I_NOP: // nop
		instr = new Instruction(brojacInstrukcija, tip, *destVars, *srcVars,
			labelName, sLabela);
		break;
	}
	++brojacInstrukcija;
	instructions.push_back(instr);
}

int SyntaxAnalysis::pozicijaSledeceInst(std::string label)
{
	for (Labels::iterator it = labels.begin();
		it != labels.end();
		it++)
	{
		if (it->first == label)
			return it->second;
	}

	return -1;
}

void SyntaxAnalysis::dodajSucc()
{
	Instructions::iterator itn;
	for (Instructions::iterator it = instructions.begin();
		it != (instructions.end()-1);
		it++)
	{
		itn = it;
		itn++;
		// Instruction B has only 1 succ
		if ((*it)->getType() != I_B)
			(*it)->m_succ.push_back(*itn);
			
		// If instr. is BRANCH type then we need to add one more successor
		if ((*it)->getType() == I_B || (*it)->getType() == I_BLTZ)
		{
			int instrPosition = pozicijaSledeceInst((*it)->getLabelName());
			Instruction* instrToJump = instructions.at(instrPosition);
			(*it)->m_succ.push_back(instrToJump);

			// Add predecessor for instr. we jump to
			instrToJump->m_pred.push_back(*itn);
		}
	}
}

void SyntaxAnalysis::dodajPred()
{
	Instructions::reverse_iterator itp;
	for (Instructions::reverse_iterator it = instructions.rbegin();
		it != instructions.rend()-1 ;
		it++)
	{
		itp=it;
		itp++;
		(*it)->m_pred.push_back(*itp);
	}
}


void SyntaxAnalysis::printTokenInfo(Token token)
{
	 token.printTokenInfo() ; 
}


void SyntaxAnalysis::eat(TokenType t)
{
	if (errorFound == false)
	{
		if (currentToken.getType() == t)
		{
			
			if( currentToken.getType() > 7 && currentToken.getType() < 19)
			{
				naziviInstrukcija.push_back(currentToken.getValue());
			}
			else 
			{
				naziviOperandi.push_back(currentToken.getValue());
			}
			cout << currentToken.getValue() << endl;
			if (tokenIterator != lexicalAnalysis.getTokenList().end())
			{
				currentToken = getNextToken(); // trazi novi token ako nisi na kraju
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
	
	//if (tokenIterator == lexicalAnalysis.getTokenList().end())
	//throw runtime_error("End of input file reached");
	
	return *tokenIterator++;
}


void SyntaxAnalysis::Q()
{
	
	if (errorFound) return;

		S();
		if (currentToken.getType() == T_SEMI_COL)
		{
			linija++;
			eat(T_SEMI_COL);
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
		L();
}


void SyntaxAnalysis::S()
{
	
	if (errorFound) return;

	if (currentToken.getType() == T_MEM)
	{
		eat(T_MEM);
		if (currentToken.getType() == T_M_ID) 
		{
			Token temp;
			//sematicka greska dvostruka deklaracija
			semanticError(0);
			vMem.push_back(currentToken.getValue());
			temp = currentToken;
			eat(T_M_ID);
			if (currentToken.getType() == T_NUM)
			{
				dodajMemVariablu(temp,currentToken);
				eat(T_NUM);
			}
			else
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_REG)
	{
		eat(T_REG);
		if (currentToken.getType() == T_R_ID)
		{
			//sematicka greska dvostruka deklaracija
			semanticError(1);
			vRegistri.push_back(currentToken.getValue());
			semanticError(2);
			dodajRegVariablu(currentToken);
			eat(T_R_ID);
		}
		else 
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_FUNC) 
	{
		semanticError(6);
		funkcije.push_back(currentToken.getValue());
		eat(T_FUNC);
		if (currentToken.getType() == T_ID) 
		{
			dodajFunc(currentToken);
			eat(T_ID);
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_ID)
	{
		string lIme;
		semanticError(5);
		lIme = currentToken.getValue();
		labele.push_back(currentToken.getValue());
		eat(T_ID);
		if (currentToken.getType() == T_COL)
		{
			dodajLabelu(lIme,brojacInstrukcija);
			eat(T_COL);
			E();
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else
	{
		E();
	}



}


void SyntaxAnalysis::L()
{
	
	if (errorFound) return;

	if (currentToken.getType() == T_END_OF_FILE)
	{
		eat(T_END_OF_FILE);
	}
	else 
	{
		Q();
	}

}


void SyntaxAnalysis::E()
{
	vector<Token> src;

	vector<Token> dst;

	if (errorFound) return;

	if (currentToken.getType() == T_ADD)
	{
		eat(T_ADD);
		if (currentToken.getType() == T_R_ID)
		{
			semanticError(3);
			dst.push_back(currentToken);
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_R_ID)
				{
					semanticError(3);
					src.push_back(currentToken);
					eat(T_R_ID);
					if (currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);
						if (currentToken.getType() == T_R_ID)
						{
							semanticError(3);
							src.push_back(currentToken);
							eat(T_R_ID);
							makeInstruction(dst, src,I_ADD);
						}
						else
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_ADDI)
	{
		eat(T_ADDI);
		if (currentToken.getType() == T_R_ID)
		{
			semanticError(3);
			dst.push_back(currentToken);
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_R_ID)
				{
					semanticError(3);
					src.push_back(currentToken);
					eat(T_R_ID);
					if (currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);
						if (currentToken.getType() == T_NUM)
						{
							src.push_back(currentToken);
							eat(T_NUM);
							makeInstruction(dst, src, I_ADDI);
						}
						else
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_SUB)
	{
		eat(T_SUB);
		if (currentToken.getType() == T_R_ID)
		{
			semanticError(3);
			dst.push_back(currentToken);
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_R_ID)
				{
					semanticError(3);
					src.push_back(currentToken);
					eat(T_R_ID);
					if (currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);
						if (currentToken.getType() == T_R_ID)
						{
							semanticError(3);
							src.push_back(currentToken);
							eat(T_R_ID);
							makeInstruction(dst, src, I_SUB);
						}
						else
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_LA)
	{
		eat(T_LA);
		if (currentToken.getType() == T_R_ID)
		{
			semanticError(3);
			dst.push_back(currentToken);
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_M_ID)
				{
					semanticError(4);
					src.push_back(currentToken);
					eat(T_M_ID);
					makeInstruction(dst, src, I_LA);
				}
				else
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}

	}
	else if (currentToken.getType() == T_LW)
	{
		eat(T_LW);
		if (currentToken.getType() == T_R_ID)
		{
			semanticError(3);
			dst.push_back(currentToken);
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_NUM)
				{
					src.push_back(currentToken);
					eat(T_NUM);
					if (currentToken.getType() == T_L_PARENT)
					{
						eat(T_L_PARENT);
						if (currentToken.getType() == T_R_ID)
						{
							semanticError(3);
							src.push_back(currentToken);
							eat(T_R_ID);
							if (currentToken.getType() == T_R_PARENT)
							{
								eat(T_R_PARENT);
								makeInstruction(dst, src, I_LW);
							}
							else
							{
								errorFound = true;
								printSyntaxError(currentToken);
							}
						}
						else
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if(currentToken.getType() == T_LI)
	{
		eat(T_LI);
		if (currentToken.getType() == T_R_ID)
		{
			semanticError(3);
			dst.push_back(currentToken);
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_NUM)
				{
					src.push_back(currentToken);
					eat(T_NUM);
					makeInstruction(dst, src, I_LI);
				}
				else
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_SW)
	{
		eat(T_SW);
		
		if (currentToken.getType() == T_R_ID)
		{
			semanticError(3);
			dst.push_back(currentToken);
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_NUM)
				{
					src.push_back(currentToken);
					eat(T_NUM);
					if (currentToken.getType() == T_L_PARENT)
					{
						eat(T_L_PARENT);
						if (currentToken.getType() == T_R_ID)
						{
							semanticError(3);
							src.push_back(currentToken);
							eat(T_R_ID);
							if (currentToken.getType() == T_R_PARENT)
							{
								eat(T_R_PARENT);
								makeInstruction(dst, src, I_SW);
							}
							else
							{
								errorFound = true;
								printSyntaxError(currentToken);
							}
						}
						else
						{
							errorFound = true;
							printSyntaxError(currentToken);
						}
					}
					else
					{
						errorFound = true;
						printSyntaxError(currentToken);
					}
				}
				else
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_B)
	{
		eat(T_B);
		if (currentToken.getType() == T_ID)
		{
			dst.push_back(currentToken);
			eat(T_ID);
			makeInstruction(dst, src, I_B);
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else if (currentToken.getType() == T_BLTZ)
	{
		eat(T_BLTZ);
		if (currentToken.getType() == T_R_ID)
		{
			semanticError(3);
			src.push_back(currentToken);
			eat(T_R_ID);
			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);
				if (currentToken.getType() == T_ID)
				{
					dst.push_back(currentToken);
					eat(T_ID);
					makeInstruction(dst, src, I_BLTZ);
				}
				else
				{
					errorFound = true;
					printSyntaxError(currentToken);
				}
			}
			else
			{
				errorFound = true;
				printSyntaxError(currentToken);
			}
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}
	else
	{
		if (currentToken.getType() == T_NOP)
		{
			eat(T_NOP);
			makeInstruction(dst, src, I_NOP);
		}
		else
		{
			errorFound = true;
			printSyntaxError(currentToken);
		}
	}

}

void SyntaxAnalysis::CreateMIPSFile( std::string& filePath)
{
	ofstream outFile(filePath);

	// Fill .globl section with functions
	for (list<string>::iterator it = funcs.begin(); it != funcs.end(); it++)
		outFile << ".globl " << *it << "\n";

	// Fill .data section with memory variables and it's values
	outFile << "\n.data\n";

	for (Variables::iterator it = memVariables.begin();
		it != memVariables.end();
		it++)
	{
		outFile << (*it)->getName() << ": " << ".word "
			<< (*it)->getValue() << "\n";
	}

	// Fill .text section with instructions and labels
	outFile << "\n.text\n";


	// Print instructons by labels
	// (Every instructions contains in which label it should be)
	Instructions::iterator instr = instructions.begin();
	for (Labels::iterator it = labels.begin();
		it != labels.end();
		it++)
	{
		outFile << it->first << ":\n";

		// Print instructions for label *it
		while (instr != instructions.end())
		{
			// Exit condition, instruction is in another label
			if ((*instr)->getLabel() != it->first)
			{
				break;
			}

			outFile << "\t" << *(*instr) << "\n";
			cout << "\t" << *(*instr) << "\n";
			instr++;
		}
	}

	outFile.close();
	
}
