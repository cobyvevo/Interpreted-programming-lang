#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

#include "tokendefs.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

int main(int argc, char* args[]) {

	//std::cout << c <<std::endl;

	SimpleVariant v1(5);
	SimpleVariant v2(5.0f);

	SimpleVariant v3 = v1*v2;

	std::cout << v3.GetFloat() << std::endl;
	
 	while (true) {
		//char next[100];
		//std::cout << ">> ";
		//std::cin.get(next, 100);

		std::string next;
		std::cout << ">> ";
		std::getline(std::cin, next);
		//std::cin >> next;
		//std::cin.ignore();

		if (next == "!script") {
			std::string newnext = "";

			std::cout << "======NEW SCRIPT=======" << std::endl;
			
			int linenum = 1;
			while (true) {

				std::string line;
				std::cout << linenum << " | ";
				std::getline(std::cin, line);
				
				if (line == "eof") {
					break;
				} else {
					newnext = newnext + line;
					linenum++;
				}

				//std::cout << newnext << std::endl;
			}

			std::cout << "======RUNNING SCRIPT=======" << std::endl;
			std::cout << std::endl;
			
			next = newnext;

		} else if (next == "!clear") {
			system("cls");
			continue;
		} else if (next == "!run") {
			std::cout << "======LOADED SCRIPT=======" << std::endl;
			std::ifstream File("res/testfile.brn");
			std::stringstream buffer;
			buffer << File.rdbuf();

			std::cout << buffer.str() << std::endl;
			std::cout << "======RUNNING SCRIPT=======" << std::endl;
		
			Lexer lex1(buffer.str());
			std::vector<Token> alltokens1;
			lex1.Next(&alltokens1);

			Parser parse1(alltokens1);
			TreeNode* node1 = parse1.Begin();

			Interpreter interp1(node1);
			interp1.Begin();
			continue;
		}

		Lexer lex(next.c_str());
		std::vector<Token> alltokens;
		lex.Next(&alltokens);

		Parser parse(alltokens);
		TreeNode* node = parse.Begin();

		Interpreter interp(node);
		interp.Begin();

		std::cin.get();
	}
	return 0;
}