#include "FractionCaculate.h"


const pair<int, int> FractionCaculate::identity = { 0,1 };

 int FractionCaculate::Gcd(int a, int b)
 {
	if (b == 0)return 0;
	a = a < 0 ? -a : a;
	b = b < 0 ? -b : b;
	int t;
	while (t = a % b)
	{
		a = b;
		b = t;
	}
	return b;
}

int FractionCaculate::Lcm(int a, int b)
{
	return  a * b / Gcd(a, b);
}

pair<int, int> FractionCaculate::FractionAdd(pair<int, int> a, pair<int, int> b)
{
	pair<int, int>res;
	res.second = Lcm(a.second, b.second);
	res.first = a.first * res.second / a.second + b.first * res.second / b.second;

	int g = Gcd(res.first, res.second);

	res.first /= g;
	res.second /= g;
	return res;
}

pair<int, int> FractionCaculate::FractionMinus(pair<int, int> a, pair<int, int> b)
{
	pair<int, int>res;
	res.second = Lcm(a.second, b.second);
	res.first = a.first * res.second / a.second - b.first * res.second / b.second;

	int g = Gcd(res.first, res.second);

	res.first /= g;
	res.second /= g;
	return res;
}

pair<int, int> FractionCaculate::FractionMulti(pair<int, int> a, pair<int, int> b)
{
	pair<int, int>res;
	res.first = a.first * b.first;
	res.second = a.second * b.second;
	int g = Gcd(res.first, res.second);
	res.first /= g;
	res.second /= g;
	return res;
}

pair<int, int> FractionCaculate::FractionDivid(pair<int, int> a, pair<int, int> b)
{
	pair<int, int>res;
	res.first = a.first * b.second;
	res.second = a.second * b.first;
	int g = Gcd(res.first, res.second);
	res.first /= g;
	res.second /= g;
	return res;
}

pair<int, int> FractionCaculate::IntToFraction(int a)
{
	return { a,1 };
}

pair<int, int> FractionCaculate::StringToFraction(string str)
{
	if (str == "")return identity;
	pair<int, int> res;
	int idx = str.find('/');
	if (idx != -1)
	{
		res.first = stoi(str.substr(0, idx));
		res.second = stoi(str.substr(idx + 1, str.size() - 1 - idx));
	}
	else
	{
		res.first = stoi(str);
		res.second = 1;
	}
	return res;
}

pair<int, int> FractionCaculate::CaculateEquation(string str)
{
	stack<pair<int, int>> nums;
	string s, op = "+";
	pair<int, int>cur = identity, pre = identity;
	bool flag = false;
	int len = str.size();
	char c;
	for (int i = 0; i < len; i++)
	{
		c = str[i];
		if (c >= '0' && c <= '9')
		{
			s += c;
		}
		else if (c < 0 || c == '+' || c == '-')
		{
			op += c;
		}
		else if (c == '(')
		{
			int j = i, cnt = 0;
			for (; i < len; i++)
			{
				if (str[i] == '(')cnt++;
				else if (str[i] == ')')cnt--;
				if (cnt == 0)break;
			}
			cur = CaculateEquation(str.substr(j + 1, i - j - 1));
			flag = true;
		}
		if ((c == ' ' && (s != "" || flag)) || i == len - 1)
		{
			if (op == "+")
			{
				if (flag)nums.push(cur);
				else nums.push(StringToFraction(s));
			}
			else if (op == "-")
			{
				if (flag)
				{
					cur.first *= -1;
					nums.push(cur);
				}
				else nums.push(StringToFraction("-" + s));
			}
			else if (op == "¡Á")
			{
				pre = nums.top();
				nums.pop();
				if (!flag)cur = StringToFraction(s);
				nums.push(FractionMulti(pre, cur));
			}
			else if (op == "¡Â")
			{
				pre = nums.top();
				nums.pop();
				if (!flag)cur = StringToFraction(s);
				nums.push(FractionDivid(pre, cur));
			}
			flag = false;
			op.clear();
			s.clear();
		}
	}

	pair<int, int>res = identity, top = identity;
	while (!nums.empty())
	{
		top = nums.top();
		nums.pop();
		res = FractionAdd(res, top);
	}
	return res;
}

bool FractionCaculate::BigThan(pair<int, int> a, pair<int, int> b)
{
	return a.first / a.second > b.first / b.second;
}

string FractionCaculate::FractionToString(pair<int, int> a)
{
	if (a.first % a.second == 0)return to_string(a.first);
	return to_string(a.first) + "/" + to_string(a.second);
}

