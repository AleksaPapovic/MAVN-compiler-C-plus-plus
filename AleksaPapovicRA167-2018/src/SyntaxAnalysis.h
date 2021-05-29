#pragma once

#include "LexicalAnalysis.h"
#include "Token.h"
#include "IR.h"

typedef std::vector<std::pair<std::string, int>> Labels;

class SyntaxAnalysis
{
public:
	
	/**
	* Constructor
	*/
	SyntaxAnalysis(LexicalAnalysis& lex, Instructions& inst);

	std::vector<std::string>& getVectorOperandi();

	std::vector<std::string>& getVectorInstrukcija();
	/**
	* Method which performs lexical analysis
	*/
	bool Do();

	Variables& getRegVariables();

	Variables& getMemVariables();

	 std::vector<std::string> vRegistri;

	 std::vector<std::string> vMem;

	 std::vector<std::string> vOperandi;
	 
	 
	 std::vector<std::string> naziviOperandi;

	 std::vector<std::string> naziviInstrukcija;
	 
	 std::vector<std::string> labele;
	 
	 std::vector<std::string> funkcije;

	 int brojacInstrukcija;
	 int brojacVariabli;

	 std::string sLabela;
	 Labels labels;
	 void dodajSucc();
	 void dodajPred();
	 void CreateMIPSFile( std::string & filePath);
private:
	/**
	* Prints the error message, and token that caused the syntax error
	*/
	void printSyntaxError(Token token);
	void semanticError(int k);
	/**
	* Prints the token info
	*/
	void printTokenInfo(Token token);

	/**
	* Eats the current token if its type is "t"
	* otherwise reports syntax error
	*
	* param[in] - t - the expected token type
	*/
	void eat(TokenType t);

	/**
	* Returns the next token from the token list
	*/
	Token getNextToken();

	/**
	* Nonterminal Q
	*/
	void Q();

	/**
	* Nonterminal S
	*/
	void S();

	/**
	* Nonterminal L
	*/
	void L();

	/**
	* Nonterminal E
	*/
	void E();


	//pravljenje funckije
	void dodajMemVariablu(Token&, Token& );
	void dodajRegVariablu(Token& );
	void dodajFunc(Token&);
	void dodajLabelu(const std::string& , int );
	void makeInstruction(std::vector<Token>&, std::vector<Token>&, InstructionType);
	int pozicijaSledeceInst(std::string);
	

	Variable* nadjiRegVariablu(const std::string& );
	Variable* nadjiMemVariablu(const std::string&);

	
	/**
	* Reference to lexical analysis module
	*/
	LexicalAnalysis& lexicalAnalysis;

	//instrukcije
	Instructions& instructions;

	/**
	* Syntax error indicator
	*/
	bool errorFound;

	/**
	* Iterator to the token list which represents the output of the lexical analysis
	*/
	TokenList::iterator tokenIterator;

	/**
	* Current token that is being analyzed
	*/
	Token currentToken;
	
	int linija;
	
	bool sError;
	Variables memVariables;       
	Variables regVariables;

	std::list<std::string> funcs;

};