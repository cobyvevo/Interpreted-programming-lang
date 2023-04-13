#pragma once

#include <iostream>
#include <vector>
#include <map>

enum class TokenType {
	NULLVALUE,
	EOL,

	NUMBER,

	ASSIGN,
	EQUALTO,
	BIGGER,
	SMALLER,
	BIGGEREQUAL,
	SMALLEREQUAL,

	DATATYPE,

	ADD,
	MINUS,
	MULTIPLY,
	DIVIDE,
	LBRACKET,
	RBRACKET,

	FUNCTION,
	FOR,
	TO,
	IF,
	ELSE,
	WHILE,
	LBRACE,
	RBRACE,
	COLON,
	COMMA,

	IDENTIFIER,
	LSQUARE,
	RSQUARE,
};

enum class TreeNodeType {

	NULLVALUE,
	ROOT,

	VALUE,
	IDENTIFIER,
	DATATYPE,

	EXPR, // + -
	TERM, // * DIV (/)
	FACTOR, // NUM, VARIABLE, OR BRACKETS

	ASSIGN,
	CONDITIONAL,
	LOOP,
	CALL,
	FUNCTION,

	CODEBLOCK,

};

enum class BaseDataType {
	NULLVALUE,
	INTEGER,
	FLOATING,
};

enum class BinOps {
	NULLVALUE,
	ADD,
	MINUS,
	MULTIPLY,
	DIVIDE,
	UNARY_NEGATIVE,

	EQUALTO,
	BIGGER,
	SMALLER,
	BIGGEREQUAL,
	SMALLEREQUAL

};

std::string tokenTypeToString(TokenType find_token);
std::string treeTypeToString(TreeNodeType find_token);
std::string binOpsToString(BinOps find_op);

bool isToken(std::string string);

struct TreeNode {
	std::vector<TreeNode*> children;
	TreeNodeType type = TreeNodeType::NULLVALUE;

	std::string value = "NULL";
	BinOps op = BinOps::NULLVALUE;

	TreeNode() {};
	TreeNode(TreeNodeType type) :type(type) {};
	TreeNode(TreeNodeType type, std::string value) :type(type), value(value) {};
};

struct Token {
	TokenType type = TokenType::NULLVALUE;
	std::string value = "NULL";

	Token() {};
	Token(TokenType desired) :type(desired) {};
	Token(TokenType desired, std::string value) :type(desired),value(value) {};
	Token(std::string value);
};

std::ostream& operator<<(std::ostream& os, const Token& token);
std::ostream& operator<<(std::ostream& os, const TokenType& tokentype);
std::ostream& operator<<(std::ostream& os, const BinOps& binop);
