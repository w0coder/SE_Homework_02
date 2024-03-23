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

	/*string GenerateOneEquation_1(int opCnt, int maxVal, pair<int, int>& outRes)
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
	}*/
	
	bool GetMinusVal(const pair<int, int>& pre, pair<int, int>&val, int maxVal)
	{
		int t = pre.first / pre.second;
		if (t > maxVal) t = maxVal;
		if (t <= 0)return false;
		val.first = rand() % t;
		return true;
	}

	bool GetDividVal(const pair<int, int>& pre, pair<int, int>&val, int maxVal)
	{
		int t = maxVal - pre.first;
		if (t <= 0)
		{
			return false;
		}
		val.first = rand() % t + pre.first;
		if (val.first <= 0)return false;
		return true;
	}

	string Gen_1(int opCnt, int max, pair<int, int>&outRes)
	{
		int opcnt = opCnt;

		outRes = { rand() % max, 1 };
		if (rand() % 3 == 0)
		{
			outRes.second = rand() % (max - 1) + 1;
			outRes = FractionCaculate::FractionDivid({ outRes.first,1 }, { outRes.second,1 });
			if (outRes.second != 1 && outRes.first > outRes.second)		//½»»»
			{
				outRes.first ^= outRes.second;
				outRes.second ^= outRes.first;
				outRes.first ^= outRes.second;
			}
		}
		cout << FractionCaculate::FractionToString(outRes) << endl;
		return Generate_1(opcnt, max, outRes.first, "+", true, outRes.second);
	}

	string Generate_1(int& opCnt, int max, int target,string preOp,bool flag,int denominator)		//left : false   right : true
	{
		if (opCnt <= 0)
		{
			return to_string(target);
		}

		string op;
		if (denominator != 1)op = "¡Â";
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
		else if (op == "¡Á")
		{
			tb = sqrt(target);
			while (tb > 1)
			{
				if (target % tb)tb--;
				else break;
			}
			ta = tb ? target / tb : 0;
		}
		else if (op == "¡Â")
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
		string right = Generate_1(opCnt , max, tb, op, true, 1);
		string left = Generate_1(opCnt , max, ta, op, false, 1);
		
		string res = left + " " + op + " " + right;
		if (preOp == "¡Á")
		{
			if (op == "+" || op == "-")res = "(" + res + ")";
		}
		else if (preOp == "¡Â")
		{
			if (op == "+" || op == "-")res = "(" + res + ")";
			else if ((op == "¡Á" || op == "¡Â") && flag)res = "(" + res + ")";
		}
		else if (preOp == "-")
		{
			if((op=="+"||op=="-")&&flag)res = "(" + res + ")";
		}
		return res;
	}


	string GenerateOneEquation(int opCnt, int maxVal, pair<int, int>& outRes)
	{
		if (opCnt < 0 || maxVal == 0)return "";
		if (opCnt==0)
		{
			outRes = { rand() % maxVal,1 };
			return to_string(outRes.first);
		}
		if (rand() % 3 == 0)
		{
			return GenProcess(opCnt, maxVal, outRes);
		}
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
				if (!GetMinusVal(pre, val, maxVal))continue;

				res += " " + op + " " + to_string(val.first);
				val.first *= -1;
				nums.push(val);
			}
			else if (op == "+")
			{
				val.first = rand() % maxVal;
				res += " " + op + " " + to_string(val.first);
				nums.push(val);
			}
			else if (op == "¡Â")
			{
				pre = nums.top();
				if (!GetDividVal(pre, val, maxVal))continue;
				if (val.first == 0)continue;
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

	string GenProcess(int opCnt, int maxVal, pair<int, int>& outRes)
	{
		if (opCnt <= 0)return "";
		int subOpCnt = 0;
		if (rand() % 2 && opCnt>1)
		{
			subOpCnt = rand() % (--opCnt) + 1;
		}
		opCnt -= (subOpCnt);
		pair<int, int> res = FractionCaculate::identity, subRes = FractionCaculate::identity;


		string s = GenerateOneEquation(opCnt, maxVal, res);
		if (opCnt >= 1&&subOpCnt)
		{
			s = "(" + s + ")";
		}
		if (subOpCnt)
		{
			string subEquation = '(' + GenerateOneEquation(subOpCnt, maxVal, subRes) + ')';

			string op = ops[rand() % ops.size()];

			if (FractionCaculate::BigThan(res, subRes))
			{
				if (op == "-")
				{
					s = s + " " + op + " " + subEquation;
					outRes = FractionCaculate::FractionMinus(res, subRes);
				}
				else 
				{
					s = subEquation + " " + op + " " + s;
					outRes = FractionCaculate::FractionMinus(subRes, res);
				}
			}
			if (subRes.first == 0 || res.first==0)
				while (op == "¡Â")
					op = ops[rand() % ops.size()];
			else s = s + " " + op + " " + subEquation;
		}
		return s;
	}


private:
	vector<string> ops;

	unordered_map<string, string> ans2EquaMap;
};



int main()
{
	srand(time(0));
	pair<int, int>res = FractionCaculate::identity;
	EquationGenerator g = EquationGenerator();

	for (int i = 0; i < 10000; i++)
	{
		cout << "------" << i << "------" << endl;
		string s = g.Gen_1(3 , 100, res);
		//if (s[s.size() - 1] == ')')
			cout << s << endl;
		cout << FractionCaculate::FractionToString(FractionCaculate::CaculateEquation(s)) << endl;
	}
	return 0;
}