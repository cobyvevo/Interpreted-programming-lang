#pragma once
#include <string>
#include <vector>

class Lexer {

	std::string input;

	int input_position = -1;
	char input_current_character;

	std::string current_element = "";

	bool IncrementCharacter();
	bool PeekCharacter();
	char GetPeekCharacter();
	
	void GetFullWord();
	void GetFullNumber();
	void GetFullToken();

	public:
	Lexer(std::string input) :input(input) {};
	bool Next(std::vector<Token>* alltokens);

};