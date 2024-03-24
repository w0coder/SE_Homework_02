#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class FractionCaculate;

class EquationGenerator
{
public:
	EquationGenerator();

	vector<string> GetAnsers();

	vector<string> GetEquations();

	bool GenerateEquation(int n = 10, int maxVal = 1000);

	string GenerateOneEquation(int opCnt, int max, pair<int, int>& outRes);

	~EquationGenerator();

	void Clear();

private:
	bool Check(const string key, const string val);

	bool IsSimilarity(string exp1, string exp2);

	string Generate_1(int& opCnt, int max, int target, string preOp, bool flag, int denominator);

	vector<string> ops;

	unordered_map<string, vector<string>> ans2EquaMap;
};