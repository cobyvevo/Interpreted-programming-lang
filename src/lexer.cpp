#include "tokendefs.hpp"
#include "lexer.hpp"

#include <iostream>

bool isNumber(char target) {
	return (target >= '0' && target <= '9') ; // chars all have number representations in ASCII
}

bool isCharacter(char target) {
	return (target >= 'A' && target <= 'Z') ||  (target >= 'a' && target <= 'z');
}

bool isWhitespace(char target) {
	return (target == ' ' || target == '	' || target == 10); // 10 is new line
}

bool Lexer::IncrementCharacter() { // move to the next character in the input
	input_position++;
	if ((unsigned int) input_position >= input.length()) {
		input_position--;
		return false;
	}

	input_current_character = input.c_str()[input_position];
	return true;
}

bool Lexer::PeekCharacter() { // look at the next character in the input
	if ((unsigned int) (input_position+1) >= input.length()) {
		return false;
	}
	return true;
}

char Lexer::GetPeekCharacter() { // look at the next character in the input
	return input.c_str()[input_position+1];
}

void Lexer::GetFullNumber() { // recursively get each number
	if (isWhitespace(input_current_character)) return;
	if (isNumber(input_current_character) == false) return;
	current_element += input_current_character;

	if (PeekCharacter() == false) return;
	if (isNumber(GetPeekCharacter())) {
		IncrementCharacter(); 
		GetFullNumber();
	}
}

void Lexer::GetFullWord() { // recursively get each letter or number (starting with letter of course)
	if (isWhitespace(input_current_character)) return;
	if (isCharacter(input_current_character) == false && isNumber(input_current_character) == false) return;
	current_element += input_current_character;

	if (PeekCharacter() == false) return;
	if (isNumber(GetPeekCharacter()) || isCharacter(GetPeekCharacter())) {
		IncrementCharacter(); 
		GetFullWord();
	}
}

void Lexer::GetFullToken() { // recursively expand token element until it is not defined (for differentiating between = and ==)
	if (isWhitespace(input_current_character)) return;
	current_element += input_current_character;

	if (PeekCharacter() == false) return;
	if (isToken(current_element + GetPeekCharacter())) {
		IncrementCharacter();
		GetFullToken();
	}
}


bool Lexer::Next(std::vector<Token>* alltokens) {

	current_element = "";
	bool success = false;
	while (IncrementCharacter()) {

		success = true;
		if (isWhitespace(input_current_character)) continue;

		//go through each character and establish groups of words, numbers, and individual tokens and identify them correctly

		bool is_number = false;
		bool is_word = false;

		if (isNumber(input_current_character)) {
			GetFullNumber(); 
			is_number = true;
		} else if (isCharacter(input_current_character)) {
			GetFullWord(); 
			is_word = true;
		}
		else {
			GetFullToken();
		}

		bool is_token = isToken(current_element);

		if (is_token || is_number || is_word) {

			Token new_token;

 			if (is_token) { // if its an internal token such as for, while, if etc
 
				new_token = Token(current_element);

			} else {

				if (is_number) new_token = Token(TokenType::NUMBER, current_element); // new integer
				else if (is_word) new_token = Token(TokenType::IDENTIFIER, current_element); // new identifier / word	

			}
			alltokens->push_back(new_token);
		}

		current_element = "";
	}

	return success;

}