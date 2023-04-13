#include "tokendefs.hpp"
#include "parser.hpp"

#include <stdexcept>
#include <map>

PeekData::PeekData(bool e, TokenType t) :exists(e),type(t) {};
PeekData::PeekData(bool e) :exists(e) {};

PeekData Parser::Peek() {
	if ((unsigned int) token_index+1 >= tokens.size()) {
		return PeekData(false);
	}
	return PeekData(true, tokens[token_index+1].type);
}

bool Parser::Next() {
	if (Peek().exists == true) {
		token_index++;
		current_token = tokens[token_index];
		return true;
	}	
	current_token = TokenType::NULLVALUE;
	return false;
}

bool Parser::AcceptCurrentType(TokenType type) {
	if (current_token.type != type) {
		std::cerr << "Unexpected token " << current_token.type << " at " << token_index << ", expected " << type << std::endl;
		throw std::runtime_error("Unexpected token");
	}
	return true;
}

bool Parser::AcceptNextType(TokenType type) {
	PeekData next = Peek();
	if (next.exists == false) return false;

	if (next.type != type) {
		std::cerr << "Unexpected token " << next.type << " at " << token_index+1 << ", expected " << type << std::endl;
		throw std::runtime_error("Unexpected token");
	}
	Next();
	return true;
}

TokenType EXPRTokens[2] = {TokenType::ADD,TokenType::MINUS};
TokenType TERMTokens[7] = {
	TokenType::MULTIPLY,TokenType::DIVIDE,

	TokenType::EQUALTO,
	TokenType::BIGGER,
	TokenType::SMALLER,
	TokenType::BIGGEREQUAL,
	TokenType::SMALLEREQUAL
};

TokenType FACTORTokens[1] = {TokenType::LBRACKET};


bool findInTokenArray(TokenType* types, int count, TokenType target) {
	for (int i = 0; i < count; i++) {
		if (types[i] == target) {
			return true;
		}
	}
	return false;
}

void printTreeNode(TreeNode* node, int white = 0) {
	std::string whitespace;
	for (int i = 0; i < white; i++) {
		whitespace += " ";
	}

	std::cout << whitespace << treeTypeToString(node->type) << "{";

	if (node->value != "NULL") {
		std::cout << whitespace << node->value;
	} else if (node->op != BinOps::NULLVALUE) {
		std::cout << whitespace << "operator: " << node->op << std::endl;
	}

	for (size_t i = 0; i < node->children.size(); i++) {
		std::cout << std::endl << whitespace << i << " = ";
		printTreeNode(node->children[i], white+1);
		std::cout << std::endl;
	}
	std::cout << whitespace << "}"  << std::endl;
}

TreeNode* blank_AST = new TreeNode();

TreeNode* Parser::getexpr() {
	TreeNode* node = getterm();

	while (findInTokenArray(EXPRTokens, 2, current_token.type)) {
		TokenType prev_type = current_token.type;

		TreeNode* next = getterm(); //get next number
		TreeNode* op_node = new TreeNode(TreeNodeType::EXPR); //create EXPR node
		op_node->children.push_back(node); //push back both the current node, and the next node (both should be factors)
		op_node->children.push_back(next);

		if (prev_type == TokenType::ADD) op_node->op = BinOps::ADD;
		else if (prev_type == TokenType::MINUS) op_node->op = BinOps::MINUS;

		node = op_node; //make the op node the new node root

	}

	return node;
}

TreeNode* Parser::getterm() {
	TreeNode* node = getfactor();

	while (findInTokenArray(TERMTokens, 7, current_token.type)) {
		TokenType prev_type = current_token.type;

		TreeNode* next = getfactor(); //get next number
		TreeNode* op_node = new TreeNode(TreeNodeType::EXPR); //create EXPR node
		op_node->children.push_back(node); //push back both the current node, and the next node (both should be factors)
		op_node->children.push_back(next);

		switch (prev_type) {
			case TokenType::MULTIPLY:
				op_node->op = BinOps::MULTIPLY;
				break;
			case TokenType::DIVIDE:
				op_node->op = BinOps::DIVIDE;
				break;
			case TokenType::EQUALTO:
				op_node->op = BinOps::EQUALTO;
				break;
			case TokenType::BIGGER:
				op_node->op = BinOps::BIGGER;
				break;
			case TokenType::SMALLER:
				op_node->op = BinOps::SMALLER;
				break;
			case TokenType::BIGGEREQUAL:
				op_node->op = BinOps::BIGGEREQUAL;
				break;
			case TokenType::SMALLEREQUAL:
				op_node->op = BinOps::SMALLEREQUAL;
				break;
			default:
				break;		
		}

		node = op_node; //make the op node the new node root

	}

	return node;	
}

TreeNode* Parser::getfactor() {

	TreeNode* node = blank_AST;
	Next();

	if (current_token.type == TokenType::NUMBER) { //this is currently only an integer
		node = new TreeNode(TreeNodeType::VALUE, current_token.value.c_str());
		Next();
		return node;
	} else if (current_token.type == TokenType::IDENTIFIER) { //this is currently only an integer
		node = new TreeNode(TreeNodeType::IDENTIFIER, current_token.value.c_str());
		Next();
		return node;
	} else if (current_token.type == TokenType::MINUS) { //unary negative sign

		TreeNode* current = getfactor(); //all this does is creates a single tree node for this specific operator
		node = new TreeNode(TreeNodeType::EXPR);
		node->op = BinOps::UNARY_NEGATIVE;
		node->children.push_back(current);

		return node;
	}

	while (findInTokenArray(FACTORTokens, 1, current_token.type)) {
		if (current_token.type == TokenType::LBRACKET) {
			node = getexpr();
			AcceptCurrentType(TokenType::RBRACKET);
			Next();
		}
	}

	return node;
}

TreeNode* Parser::getgrammar(ParseGrammar type) {

	TreeNode* node = nullptr;
	TreeNode* exprnode;

	switch (type) {
		case ParseGrammar::ASSIGNMENT: {

			if (Peek().type == TokenType::LBRACKET) { //REALLY BAD FUNCTION CALL DETECTION, REFACTOR ALL THIS
				node = new TreeNode(TreeNodeType::CALL);

				AcceptCurrentType(TokenType::IDENTIFIER); // function name
				TreeNode* identifier = new TreeNode(TreeNodeType::IDENTIFIER); 
				identifier->value = current_token.value;

				AcceptNextType(TokenType::LBRACKET); // begin arguement
				
				exprnode = getexpr(); // number / math expression / etc
				node->children.push_back(identifier); //add function name
				node->children.push_back(exprnode); //add arguement

				AcceptCurrentType(TokenType::RBRACKET); // end argumeents
				Next();
				
				break;
			} else if (Peek().type == TokenType::FUNCTION) {

				//we are assigning a function

				node = new TreeNode(TreeNodeType::FUNCTION);

				AcceptCurrentType(TokenType::DATATYPE); // function return datatype
				AcceptNextType(TokenType::FUNCTION); // function keyword
				AcceptNextType(TokenType::IDENTIFIER); // function name		

				TreeNode* identifier = new TreeNode(TreeNodeType::IDENTIFIER); 
				identifier->value = current_token.value;
				node->children.push_back(identifier); //add function name


				AcceptNextType(TokenType::LBRACKET); // parameter bracket begin
				//get argumetns here, but theres no scopes, so this isnt entirely possible yet
				AcceptNextType(TokenType::RBRACKET); // end argumeents

				AcceptNextType(TokenType::LBRACE); // codeblock beginning
				TreeNode* func_codeblock = getgrammar(ParseGrammar::CODEBLOCK); //get codeblock
				node->children.push_back(func_codeblock); //add codeblock to tree

				//we now should be at the character right after the last right brace. {...}_ (there would be a semicolon here)
				
				break;
			}


			node = new TreeNode(TreeNodeType::ASSIGN);

			//BaseDataType datatype = BaseDataType::NULLVALUE;
			TreeNode* datatypenode = new TreeNode(TreeNodeType::DATATYPE); 
			
			if (current_token.type == TokenType::DATATYPE) {
				//assigning new variable
				AcceptCurrentType(TokenType::DATATYPE);

				if (current_token.value == "int") {
					datatypenode->value = "INT";

					//datatype = BaseDataType::INTEGER;
				} else if (current_token.value == "float") {
					datatypenode->value = "FLOAT";

					//datatype = BaseDataType::FLOATING;
				}

				AcceptNextType(TokenType::IDENTIFIER); // variable name
 			} else {
 				//changing already assigned variable
				AcceptCurrentType(TokenType::IDENTIFIER);
 			}

			TreeNode* identifier = new TreeNode(TreeNodeType::IDENTIFIER); 
			identifier->value = current_token.value;

			AcceptNextType(TokenType::ASSIGN); // is equal to

			exprnode = getexpr(); // number / math expression / etc

			node->children.push_back(identifier);
			node->children.push_back(exprnode); //add to assign node
			node->children.push_back(datatypenode);

			break;
		}

		case ParseGrammar::CODEBLOCK: {

			node = new TreeNode(TreeNodeType::CODEBLOCK);

			AcceptCurrentType(TokenType::LBRACE); // left brace
			Next();

			while (current_token.type != TokenType::RBRACE) {
				bool is_term = false; 
				TreeNode* codeblock_within = startgrammar(is_term); //get all code nodes within braces
				node->children.push_back(codeblock_within); //add all of the code into the codeblock
				if (is_term == false) {
					AcceptCurrentType(TokenType::EOL);
				}
				Next();
					 // get end of line character if aplicable
			

			} 

			AcceptCurrentType(TokenType::RBRACE);
			//Next();

			break;
		}
		
		case ParseGrammar::CONDITIONAL: {

			node = new TreeNode(TreeNodeType::CONDITIONAL);

			AcceptCurrentType(TokenType::IF); // if 
			AcceptNextType(TokenType::LBRACKET); // ( ... 

			exprnode = getexpr(); // number / math expression / etc
			node->children.push_back(exprnode); //add to assign node

			AcceptCurrentType(TokenType::RBRACKET);
			AcceptNextType(TokenType::LBRACE);
			TreeNode* if_codeblock = getgrammar(ParseGrammar::CODEBLOCK); //get if statement block

			node->children.push_back(if_codeblock); //add to assign node

			//same as function definition
			break;
		}

		case ParseGrammar::LOOP: {

			node = new TreeNode(TreeNodeType::LOOP);
			if (current_token.type == TokenType::WHILE) {
				AcceptNextType(TokenType::LBRACKET); // ( ... 
				exprnode = getexpr(); // number / math expression / etc
				node->children.push_back(exprnode); //add to LOOP node
				AcceptCurrentType(TokenType::RBRACKET);

				AcceptNextType(TokenType::LBRACE);
				TreeNode* loop_codeblock = getgrammar(ParseGrammar::CODEBLOCK); //get loop block
				node->children.push_back(loop_codeblock); //add to assign node
			}

			break;
		}
	}

	return node;

}

TreeNode* Parser::startgrammar() {
	TreeNode* nextnode = nullptr;

	switch (current_token.type) {
		case TokenType::LBRACE: {
			nextnode = getgrammar(ParseGrammar::CODEBLOCK);
			break;
		}

		case TokenType::IF: {
			nextnode = getgrammar(ParseGrammar::CONDITIONAL);
			break;
		}

		case TokenType::WHILE: {
			nextnode = getgrammar(ParseGrammar::LOOP);
			break;
		}

		default: nextnode = getgrammar(ParseGrammar::ASSIGNMENT);
	}

	return nextnode;
}

TreeNode* Parser::startgrammar(bool& terminating_line) {

	//everything that uses braces are self terminating, so we can ignore EOL chars for simplicity.
	
	terminating_line = (
		(Peek().type == TokenType::FUNCTION) ||
		(current_token.type == TokenType::IF) ||
		(current_token.type == TokenType::LBRACE) ||
		(current_token.type == TokenType::WHILE)
		);	

	TreeNode* nextnode = startgrammar();

	return nextnode;
}

TreeNode* Parser::Begin() {	
	TreeNode* full;

	/*debug prints
	int count = 0;
	for (auto token : tokens) {
		std::cout << token << " - " << count << std::endl ;
		count++;
	}*/

	try {
		//std::cout << "==BEGIN PARSER==" << std::endl;

		full = new TreeNode(TreeNodeType::ROOT);
		Next();

		while (true) {	
			bool is_term = false; 
			TreeNode* nextnode = startgrammar(is_term);

			if (nextnode != nullptr) {
				full->children.push_back(nextnode);
			}

			if (is_term == false) {
				AcceptCurrentType(TokenType::EOL);
			}
			Next();

			if (Peek().exists == false) break;
		}

		//printTreeNode(full);
		//std::cout << "==END PARSER==" << std::endl;
	}
	catch (const std::runtime_error& error) {
		full = new TreeNode();
		std::cout << "error" << std::endl;
	}

	return full;
}