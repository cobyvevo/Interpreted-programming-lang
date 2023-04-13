#include "tokendefs.hpp"

const char* TokenTypeStrings[] = {
	"NULLVALUE",
	"END OF LINE",

	"NUMBER",

	"ASSIGN",
	"EQUAL TO",
	"BIGGER THAN",
	"SMALLER THAN",
	"BIGGER EQUAL THAN",
	"SMALLER EQUAL THAN",

	"DATATYPE",

	"ADD",
	"MINUS",
	"MULTIPLY",
	"DIVIDE",
	"LBRACKET",
	"RBRACKET",

	"FUNCTION",
	"FOR",
	"TO",
	"IF",
	"ELSE",
	"WHILE",
	"LBRACE",
	"RBRACE",
	"COLON",
	"COMMA",

	"IDENTIFIER",
	"LSQUARE",
	"RSQUARE"
};

std::map<std::string, TokenType> TokenTypeMapping{

	{"NULL",TokenType::NULLVALUE},
	{";",TokenType::EOL},

	{"=",TokenType::ASSIGN},
	{"==",TokenType::EQUALTO},
	{">",TokenType::BIGGER},
	{"<",TokenType::SMALLER},
	{">=",TokenType::BIGGEREQUAL},
	{"<=",TokenType::SMALLEREQUAL},

	{"float",TokenType::DATATYPE}, //remove these datatypes, define in symbols later
	{"int",TokenType::DATATYPE}, //remove these datatypes, define in symbols later
	{"void",TokenType::DATATYPE}, //remove these datatypes, define in symbols later
	
	{"function",TokenType::FUNCTION},

	{"+",TokenType::ADD},
	{"-",TokenType::MINUS},
	{"*",TokenType::MULTIPLY},
	{"/",TokenType::DIVIDE},
	{"(",TokenType::LBRACKET},
	{")",TokenType::RBRACKET},	

	{"for",TokenType::FOR},
	{"to",TokenType::TO},
	{"if",TokenType::IF},
	{"else",TokenType::ELSE},
	{"while",TokenType::WHILE},
	{"{",TokenType::LBRACE},
	{"}",TokenType::RBRACE},
	{":",TokenType::COLON},
	{",",TokenType::COMMA},

	{"[",TokenType::LSQUARE},
	{"]",TokenType::RSQUARE},		

};

const char* TreeTypeStrings[] = {

	"NULLVALUE",
	"ROOT",

	"VALUE",
	"IDENTIFIER",
	"DATATYPE",
	
	"EXPR", // + -
	"TERM", // * DIV (/)
	"FACTOR", // NUM, VARIABLE, OR BRACKETS

	"ASSIGN",
	"CONDITION",
	"LOOP",
	"CALL",
	"FUNCTION",

	"CODEBLOCK"
};

const char* BinOpsStrings[] {
	"NULLVALUE",
	"ADD",
	"MINUS",
	"MULTIPLY",
	"DIVIDE",
	"UNARY_NEGATIVE",

	"EQUAL TO",
	"BIGGER",
	"SMALLER",
	"BIGGEREQUAL",
	"SMALLEREQUAL"
};

std::string tokenTypeToString(TokenType find_token) {
	return TokenTypeStrings[static_cast<int>(find_token)];
};
std::string treeTypeToString(TreeNodeType find_token) {
	return TreeTypeStrings[static_cast<int>(find_token)];
};
std::string binOpsToString(BinOps find_op) {
	return BinOpsStrings[static_cast<int>(find_op)];
};

bool isToken(std::string string) {
	return (TokenTypeMapping.find(string) != TokenTypeMapping.end());
};

Token::Token (std::string value) :value(value) {
	if (TokenTypeMapping.find(value) != TokenTypeMapping.end()) type = TokenTypeMapping[value];
};

std::ostream& operator<<(std::ostream& os, const Token& token) {
	return os << "< " << tokenTypeToString(token.type) << " = '" << token.value << "' >";
};

std::ostream& operator<<(std::ostream& os, const TokenType& tokentype) {
	return os << tokenTypeToString(tokentype);
};

std::ostream& operator<<(std::ostream& os, const BinOps& binop) {
	return os << binOpsToString(binop);
};