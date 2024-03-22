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
		ops = { "+","-","¡Á","¡Â" };
	}

	vector<pair<int, int>> GetAnsers()
	{

	}

	vector<string> GetEquations()
	{

	}

	void GenerateEquation(int n = 10, int maxVal = 1000)
	{


	}



	string GenerateOneEquation_1(int opCnt, int maxVal, pair<int, int>& outRes)
	{
		if (opCnt <= 0 || maxVal == 0)return "";
		string res = "";
		string op;
		stack<pair<int, int>> nums;
		int j = 1;
		pair<int, int> val = FractionCaculate::identity, pre = FractionCaculate::identity;
		val.first = rand() % maxVal;
		res += to_string(val.first);
		nums.push(val);
		while (opCnt)
		{
			op = ops[rand() % ops.size()];
			j = 1;
			if (op == "-")
			{
				pre = nums.top();
				int t = pre.first / pre.second;
				if (t > maxVal) t = maxVal;
				if (rand() % 2 == 0)
				{
					j = rand() % opCnt;
					string s = GenerateOneEquation(j, t, val);
					if (s == "")continue;
					res += " " + op + " (" + s + ")";
				}
				else if (t)
				{
					val.first = rand() % t;
					res += " " + op + " " + to_string(val.first);
				}
				else continue;
				val.first *= -1;
				nums.push(val);
			}
			else if (op == "¡Â")
			{
				pre = nums.top();
				int t = maxVal - pre.first;
				if (t <= 0)
				{
					op = ops[rand() % ops.size()];
					continue;
				}
				if (rand() % 2 == 0)
				{
					j = rand() % opCnt;
					string s = GenerateOneEquation(j, t, val);
					if (s == "")continue;
					if (val.first == 0)continue;
					res += " " + op + " (" + s + ")";
				}
				else
				{
					val.first = rand() % t + pre.first;
					if (val.first <= 0)continue;
					res += " " + op + " " + to_string(val.first);
				}
				nums.pop();
				nums.push(FractionCaculate::FractionDivid(pre, val));
			}
			else if (op == "¡Á")
			{
				pre = nums.top();
				if (rand() % 2 == 0)
				{
					j = rand() % opCnt;
					string s = GenerateOneEquation(j, maxVal, val);
					if (s == "")continue;
					res += " " + op + " (" + s + ")";
				}
				else
				{
					val.first = rand() % maxVal;
					res += " " + op + " " + to_string(val.first);
				}
				nums.pop();
				nums.push(FractionCaculate::FractionMulti(pre, val));
			}
			else
			{
				if (rand() % 2 == 0)
				{
					j = rand() % opCnt;
					string s = GenerateOneEquation(j, maxVal, val);
					if (s == "")continue;
					res += " " + op + " (" + s + ")";
				}
				else
				{
					val.first = rand() % maxVal;
					res += " " + op + " " + to_string(val.first);
				}
				nums.push(val);
			}

			opCnt -= j;
		}

		outRes = FractionCaculate::identity;
		while (!nums.empty())
		{
			pre = nums.top();
			nums.pop();
			outRes = FractionCaculate::FractionAdd(pre, outRes);
		}
		return res;
	}

	string GenerateOneEquation(int opCnt, int maxVal, pair<int, int>& outRes)
	{
		if (opCnt <= 0 || maxVal == 0)return "";
		string res = "";
		string op;
		stack<pair<int, int>> nums;
		int j = 1;
		pair<int, int> val = FractionCaculate::identity, pre = FractionCaculate::identity;
		val.first = rand() % maxVal;
		res += to_string(val.first);
		nums.push(val);
		while (opCnt)
		{
			op = ops[rand() % ops.size()];
			if (op == "-")
			{
				pre = nums.top();
				int t = pre.first / pre.second;
				if (t > maxVal) t = maxVal;

				if (rand() % 2)
				{
					string s = RandProcess(opCnt, maxVal, val);
					if (s == "")continue;
					res += " " + op + " (" + s + ")";
				}

				if (!t)continue;
				val.first = rand() % t;
				res += " " + op + " " + to_string(val.first);
				val.first *= -1;
				nums.push(val);
			}
			else if (op == "¡Â")
			{
				pre = nums.top();
				int t = maxVal - pre.first;
				if (t <= 0)
				{
					op = ops[rand() % ops.size()];
					continue;
				}
				val.first = rand() % t + pre.first;
				if (val.first <= 0)continue;
				res += " " + op + " " + to_string(val.first);

				nums.pop();
				nums.push(FractionCaculate::FractionDivid(pre, val));
			}
			else if (op == "¡Á")
			{
				pre = nums.top();
				val.first = rand() % maxVal;
				res += " " + op + " " + to_string(val.first);

				nums.pop();
				nums.push(FractionCaculate::FractionMulti(pre, val));
			}
			else
			{

				val.first = rand() % maxVal;
				res += " " + op + " " + to_string(val.first);
				nums.push(val);
			}

			opCnt -= 1;
		}

		outRes = FractionCaculate::identity;
		while (!nums.empty())
		{
			pre = nums.top();
			nums.pop();
			outRes = FractionCaculate::FractionAdd(pre, outRes);
		}
		return res;
	}

	string RandProcess(int& opCnt, int maxVal, pair<int, int>& outRes)
	{
		int j = rand() % opCnt;
		if (j >= 1)
		{
			opCnt -= j;
			return GenerateOneEquation(j, maxVal, outRes);
		}
		return "";
	}
private:
	vector<string> ops;

	unordered_map<string, string> ans2EquaMap;
};



int main()
{

	srand(time(0));
	pair<int, int>res;
	EquationGenerator g = EquationGenerator();
	for (int i = 0; i < 20; i++)
	{
		cout << "------" << i << "------" << endl;
		string s = g.GenerateOneEquation(3, 10, res);
		cout << s << endl;
		cout << FractionCaculate::FractionToString(res) << endl;
		res = FractionCaculate::CaculateEquation(s);
		cout << FractionCaculate::FractionToString(res) << endl;

	}

	/*
		string s1 = "7 - 4 - 5 ¡Â 2";
		pair<int,int>res = FractionCaculate::CaculateEquation(s1);
		cout << res.first << "/" << res.second << endl;
	*/
	return 0;
}