#include "tokendefs.hpp"
#include "interpreter.hpp"

bool Interpreter::Next() {
	return true;
}

int Interpreter::Start(TreeNode* node) {
	if (node == nullptr) return 0; //make sure no tomfoolery happens here

	int number; 
	
	switch(node->type) {

		case TreeNodeType::VALUE: {
			number = std::stoi(node->value); //converts string to integer
			return number;
		}

		case TreeNodeType::CODEBLOCK: {

			for (size_t i = 0; i < node->children.size(); i++) { //run codeblock contents
				Start(node->children[i]);
			}

			return 1;
		}

		case TreeNodeType::CALL: {

			if (node->children[0]->value == "print") {
				number = Start(node->children[1]);
				std::cout << number << std::endl;
			} else {

				for (Symbol* sym : global_vars) { // look in global memory for function definition
					if (sym->key == node->children[0]->value) {
						number = Start(sym->subroutine); //once found, start at the defined treenode
					}
				}

			}

			return number;

		}

		case TreeNodeType::FUNCTION: {

			Symbol* storage = new Symbol();
			storage->key = node->children[0]->value;
			storage->subroutine = node->children[1];
			global_vars.push_back(storage);

			return 1;

		}

		case TreeNodeType::IDENTIFIER: {

			for (Symbol* sym : global_vars) {
				if (sym->key == node->value) {
					number = std::stoi(sym->value);
				}
			}
			
			return number;
		}

		case TreeNodeType::ASSIGN: {

			number = Start(node->children[1]);

			//changing already existing variable, only do this if no datatype is provided, though datatypes do not get parsed currently, so all assignments are equal.

			bool found = false;
			for (Symbol* sym : global_vars) {
				if (sym->key == node->children[0]->value) {

					sym->value = std::to_string(number);
					found = true;
					break;
				}
			}
			if (found) return number;

			//

			Symbol* storage = new Symbol();
			storage->key = node->children[0]->value;
			storage->value = std::to_string(number);

			global_vars.push_back(storage);

			return number;

		}

		case TreeNodeType::EXPR: {
			int inputs[2];

			for (size_t i = 0; i < node->children.size(); i++) {
				inputs[i] = Start(node->children[i]);
			}

			switch(node->op) {
				case BinOps::ADD:
					number = inputs[0]+inputs[1]; 
					break;
				case BinOps::MINUS:
					number = inputs[0]-inputs[1]; 
					break;
				case BinOps::MULTIPLY:
					number = inputs[0]*inputs[1]; 
					break;
				case BinOps::DIVIDE:
					number = inputs[0]/inputs[1]; 
					break;
				case BinOps::UNARY_NEGATIVE:
					number = -inputs[0];
					break;

				case BinOps::EQUALTO:
					number = (inputs[0] == inputs[1]) ? 1 : 0;
					break;
				case BinOps::BIGGER:
					number = (inputs[0] > inputs[1]) ? 1 : 0;
					break;
				case BinOps::SMALLER:
					number = (inputs[0] < inputs[1]) ? 1 : 0;
					break;
				case BinOps::BIGGEREQUAL:
					number = (inputs[0] >= inputs[1]) ? 1 : 0;
					break;
				case BinOps::SMALLEREQUAL:
					number = (inputs[0] <= inputs[1]) ? 1 : 0;
					break;

				default:
					number = 0;
					break;
			}

			return number;
		}

		case TreeNodeType::CONDITIONAL: {
			int condition = Start(node->children[0]);

			if (condition == 1) {
				number = Start(node->children[1]);
			}

			return number;
		}

		case TreeNodeType::LOOP: {
			while (Start(node->children[0]) == 1) {
				number = Start(node->children[1]);
			}

			return number;
		}


		default: 
			break;
	}

	for (size_t i = 0; i < node->children.size(); i++) {
		number = Start(node->children[i]);
	}

	return number;

}

void Interpreter::Begin() {
	//std::cout << "==BEGIN INTERPRETER==" << std::endl;
	Start(ASTNodes);
	//std::cout << "END:" << Start(ASTNodes) << std::endl;

	//std::cout << "==END INTERPRETER==" << std::endl;
}

//

SimpleVariant SimpleVariant::operator+(SimpleVariant& other) {
	SimpleVariant out;

	if (datatype == BaseDataType::FLOATING && other.datatype == BaseDataType::FLOATING) {
		out.SetFloat(
			GetFloat() + other.GetFloat()
		);
	} else if (datatype == BaseDataType::INTEGER && other.datatype == BaseDataType::FLOATING) {
		out.SetFloat(
			GetInt() + other.GetFloat()
		);
	} else if (datatype == BaseDataType::FLOATING && other.datatype == BaseDataType::INTEGER) {
		out.SetFloat(
			GetFloat() + other.GetInt()
		);
	} else {
		out.SetInt(
			GetInt() + other.GetInt()
		);		
	}

	return out;
}

SimpleVariant SimpleVariant::operator-(SimpleVariant& other) {
	SimpleVariant out;

	if (datatype == BaseDataType::FLOATING && other.datatype == BaseDataType::FLOATING) {
		out.SetFloat(
			GetFloat() - other.GetFloat()
		);
	} else if (datatype == BaseDataType::INTEGER && other.datatype == BaseDataType::FLOATING) {
		out.SetFloat(
			GetInt() - other.GetFloat()
		);
	} else if (datatype == BaseDataType::FLOATING && other.datatype == BaseDataType::INTEGER) {
		out.SetFloat(
			GetFloat() - other.GetInt()
		);
	} else {
		out.SetInt(
			GetInt() - other.GetInt()
		);		
	}

	return out;
}


SimpleVariant SimpleVariant::operator*(SimpleVariant& other) {
	SimpleVariant out;

	if (datatype == BaseDataType::FLOATING && other.datatype == BaseDataType::FLOATING) {
		out.SetFloat(
			GetFloat() * other.GetFloat()
		);
	} else if (datatype == BaseDataType::INTEGER && other.datatype == BaseDataType::FLOATING) {
		out.SetFloat(
			GetInt() * other.GetFloat()
		);
	} else if (datatype == BaseDataType::FLOATING && other.datatype == BaseDataType::INTEGER) {
		out.SetFloat(
			GetFloat() * other.GetInt()
		);
	} else {
		out.SetInt(
			GetInt() * other.GetInt()
		);		
	}

	return out;
}

SimpleVariant SimpleVariant::operator/(SimpleVariant& other) {
	SimpleVariant out;

	if (datatype == BaseDataType::FLOATING && other.datatype == BaseDataType::FLOATING) {
		out.SetFloat(
			GetFloat() / other.GetFloat()
		);
	} else if (datatype == BaseDataType::INTEGER && other.datatype == BaseDataType::FLOATING) {
		out.SetFloat(
			GetInt() / other.GetFloat()
		);
	} else if (datatype == BaseDataType::FLOATING && other.datatype == BaseDataType::INTEGER) {
		out.SetFloat(
			GetFloat() / other.GetInt()
		);
	} else {
		out.SetInt(
			GetInt() / other.GetInt()
		);		
	}

	return out;
}