#pragma once
#include <vector>
#include <string>

struct Symbol {
	std::string key;
	std::string value;

	BaseDataType datatype = BaseDataType::NULLVALUE;
	TreeNode* subroutine = nullptr;
};

//later
union Variant {
	int intvar;
	float floatvar;
};

struct SimpleVariant {
	BaseDataType datatype = BaseDataType::NULLVALUE;

	SimpleVariant() {};

	SimpleVariant(int value) {
		storedvalue.intvar = value;
		datatype = BaseDataType::INTEGER;
	}

	SimpleVariant(float value) {
		storedvalue.floatvar = value;
		datatype = BaseDataType::FLOATING;
	}

	int GetInt() { return storedvalue.intvar; };
	float GetFloat() { return storedvalue.floatvar; };

	void SetInt(int v) { storedvalue.intvar = v; datatype = BaseDataType::INTEGER; };
	void SetFloat(float v) { storedvalue.floatvar = v; datatype = BaseDataType::FLOATING; };

	SimpleVariant operator+(SimpleVariant& other);
	SimpleVariant operator-(SimpleVariant& other);
	SimpleVariant operator*(SimpleVariant& other);
	SimpleVariant operator/(SimpleVariant& other);

private:
	Variant storedvalue;
};
//

class Interpreter {
	TreeNode* ASTNodes;

	std::vector<Symbol*> global_vars;
	
	bool Next();

	int Start(TreeNode* node);

	public:
	Interpreter(TreeNode* targ) :ASTNodes(targ) {};
	void Begin();
};