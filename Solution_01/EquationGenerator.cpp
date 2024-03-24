#include "EquationGenerator.h"
#include "FractionCaculate.h"

EquationGenerator::EquationGenerator()
{
	ops = { "+","-","×","÷" };
}

vector<string> EquationGenerator::GetAnsers()
{
	vector<string> res;
	for (auto str2vec : ans2EquaMap)
		for (auto eq : str2vec.second)
			res.push_back(str2vec.first);
	return res;
}

vector<string> EquationGenerator::GetEquations()
{
	vector<string>res;
	for (auto str2vec : ans2EquaMap)
		for (auto eq : str2vec.second)
			res.push_back(eq);
	return res;
}

bool EquationGenerator::GenerateEquation(int n, int maxVal)
{
	if (maxVal<100 && n >((maxVal * maxVal * maxVal * maxVal) << 6))return false;
	Clear();
	for (int i = 0; i < n;)
	{
		int cnt = rand() % 3 + 1;
		pair<int, int>res = FractionCaculate::identity;
		string val = GenerateOneEquation(cnt, maxVal, res);
		string key = FractionCaculate::FractionToString(res);
		if (ans2EquaMap.find(key) == ans2EquaMap.end())
		{
			vector<string> vec;
			ans2EquaMap[key] = vec;
		}
		if (Check(key, val))
		{
			ans2EquaMap[key].push_back(val);
			i++;
		}
	}
	return true;
}

string EquationGenerator::GenerateOneEquation(int opCnt, int max, pair<int, int>& outRes)
{
	int opcnt = opCnt;

	outRes = { rand() % max, 1 };
	if (rand() % 3 == 0)
	{
		outRes.second = rand() % (max - 1) + 1;
		outRes = FractionCaculate::FractionDivid({ outRes.first,1 }, { outRes.second,1 });
		if (outRes.second != 1 && outRes.first > outRes.second)		//交换
		{
			outRes.first ^= outRes.second;
			outRes.second ^= outRes.first;
			outRes.first ^= outRes.second;
		}
	}
	return Generate_1(opcnt, max, outRes.first, "+", true, outRes.second);
}

EquationGenerator::~EquationGenerator()
{
	Clear();
	ops.clear();
}

void EquationGenerator::Clear()
{
	for (auto p : ans2EquaMap)
		p.second.clear();
	ans2EquaMap.clear();
}

bool EquationGenerator::Check(const string key, const string val)
{
	if (ans2EquaMap[key].size() == 0)return true;
	for (auto str : ans2EquaMap[key])
		if (IsSimilarity(val, str))return false;
	return true;
}

bool EquationGenerator::IsSimilarity(string exp1, string exp2)
{
	if (exp1.size() != exp2.size())return false;
	int res = 0;
	for (int i = 0; i < exp1.size(); i++)
		res ^= (exp1[i] ^ exp2[i]);
	return res == 0;
}

string EquationGenerator::Generate_1(int& opCnt, int max, int target, string preOp, bool flag, int denominator)		//flag -->> left=false right=true , target is the pair's first 
{
	if (opCnt <= 0)
	{
		return to_string(target);
	}

	string op;
	if (denominator != 1)op = "÷";
	else
	{
		op = ops[rand() % ops.size()];
		while (op == preOp && rand() % 3)
		{
			op = ops[rand() % ops.size()];
		}
	}

	int ta = 0, tb = 0;
	if (op == "-")
	{
		tb = rand() % (max - target);
		ta = target + tb;
	}
	else if (op == "+")
	{
		tb = target == 0 ? 0 : rand() % target;
		ta = target - tb;
	}
	else if (op == "×")
	{
		tb = sqrt(target);
		while (tb > 1)
		{
			if (target % tb)tb--;
			else break;
		}
		ta = tb ? target / tb : 0;
	}
	else if (op == "÷")
	{
		tb = denominator;
		ta = target;
		int rate = 2;
		while (ta * rate < max - 1 && tb * rate < max - 1 && rand() % 4)
		{
			ta *= rate;
			tb *= rate;
			rate++;
		}
	}

	--opCnt;
	string right, left;
	if (rand() % 2)			//递归顺序导致括号偏向，左右先的概率均等以平衡
	{
		right = Generate_1(opCnt, max, tb, op, true, 1);
		left = Generate_1(opCnt, max, ta, op, false, 1);
	}
	else
	{
		left = Generate_1(opCnt, max, ta, op, false, 1);
		right = Generate_1(opCnt, max, tb, op, true, 1);
	}

	string res = left + " " + op + " " + right;
	//处理加括号（可优化）
	if (preOp == "×")
	{
		if (op == "+" || op == "-")res = "(" + res + ")";
	}
	else if (preOp == "÷")
	{
		if (op == "+" || op == "-")res = "(" + res + ")";
		else if ((op == "×" || op == "÷") && flag)res = "(" + res + ")";
	}
	else if (preOp == "-")
	{
		if ((op == "+" || op == "-") && flag)res = "(" + res + ")";
	}
	return res;
}
