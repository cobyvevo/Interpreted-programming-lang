#pragma once
#include <string>
#include <vector>

enum class ParseGrammar {
	ASSIGNMENT,
	CONDITIONAL,
	CODEBLOCK,
	LOOP,
};
 
struct PeekData { //data used for when peeking next token, allowing for easier grammar detection too
	bool exists = false;
	TokenType type = TokenType::NULLVALUE;

	PeekData(bool e, TokenType t);
	PeekData(bool e);
};

class Parser {

	std::vector<Token> tokens;
	Token current_token;
	int token_index = -1;

	bool Next();
	bool AcceptCurrentType(TokenType type);
	bool AcceptNextType(TokenType type);
	PeekData Peek();

	TreeNode* startgrammar(bool& terminating_line);
	TreeNode* startgrammar();
	

	TreeNode* getgrammar(ParseGrammar type);
	TreeNode* getexpr();
	TreeNode* getterm();
	TreeNode* getfactor();

public:
	Parser(std::vector<Token> &tokens) :tokens(tokens) {};
	TreeNode* Begin();

};