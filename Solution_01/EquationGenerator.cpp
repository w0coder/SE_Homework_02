#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include "FractionCaculate.h"

#include <ctime>
#include <iostream>
using namespace std;

class EquationGenerator
{
public:
	EquationGenerator()
	{
		ops = { "+","-","×","÷" };
	}

	vector<string> GetAnsers()
	{
		vector<string> res;
		for (auto str2vec : ans2EquaMap)
			for (auto eq : str2vec.second)
				res.push_back(str2vec.first);
		return res;
	}

	vector<string> GetEquations()
	{
		vector<string>res;
		for (auto str2vec : ans2EquaMap)
			for (auto eq : str2vec.second)
				res.push_back(eq);
		return res;
	}

	void GenerateEquation(int n = 10, int maxVal = 1000)
	{
		for (int i = 0; i < n; i++)
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
			ans2EquaMap[key].push_back(val);
		}
	}

	string GenerateOneEquation(int opCnt, int max, pair<int, int>&outRes)
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
		cout << FractionCaculate::FractionToString(outRes) << endl;
		return Generate_1(opcnt, max, outRes.first, "+", true, outRes.second);
	}

private:
	string Generate_1(int& opCnt, int max, int target, string preOp, bool flag, int denominator)		//flag -->> left=false right=true , target is the pair's first 
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

		int ta, tb;
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

	vector<string> ops;

	unordered_map<string, vector<string>> ans2EquaMap;
};



int main()
{
	srand(time(0));
	pair<int, int>res = FractionCaculate::identity;
	EquationGenerator g = EquationGenerator();


	//for (int i = 0; i < 10000; i++)
	//{
	//	cout << "------" << i << "------" << endl;
	//	string s = g.GenerateOneEquation(3 , 100, res);
	//	//if (s[s.size() - 1] == ')')
	//		cout << s << endl;
	//	cout << FractionCaculate::FractionToString(FractionCaculate::CaculateEquation(s)) << endl;
	//}

	return 0;
}