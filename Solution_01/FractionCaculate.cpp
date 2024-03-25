#include "FractionCaculate.h"

const pair<int, int> FractionCaculate::identity = { 0,1 };

 int FractionCaculate::Gcd(int a, int b)			//辗转相除法求最大公约数
 {
	if (b == 0)return 0;
	int sign = (a > 0 && b > 0) || (a < 0 && b < 0) ? 1 : -1;
	a = a < 0 ? -a : a;
	b = b < 0 ? -b : b;
	int t;
	while (t = a % b)
	{
		a = b;
		b = t;
	}
	return b * sign;
}

int FractionCaculate::Lcm(int a, int b)			//求最小公倍数，此处可能会除以 0，注意输入的数字
{
	return  a * b / Gcd(a, b);
}

pair<int, int> FractionCaculate::FractionAdd(pair<int, int> a, pair<int, int> b)		//分数加法
{
	pair<int, int>res;
	res.second = Lcm(a.second, b.second);												//算加数的最小公倍数，求得分母
	res.first = a.first * res.second / a.second + b.first * res.second / b.second;		//计算分子

	int g = Gcd(res.first, res.second);

	res.first /= g;
	res.second /= g;											//约分	
	if (res.second < 0)											//统一表示，分母为整数
	{
		res.first *= -1;
		res.second *= -1;
	}
	return res;
}

pair<int, int> FractionCaculate::FractionMinus(pair<int, int> a, pair<int, int> b)		//分数减法，类似加法
{
	pair<int, int>res;
	res.second = Lcm(a.second, b.second);
	res.first = a.first * res.second / a.second - b.first * res.second / b.second;

	int g = Gcd(res.first, res.second);

	res.first /= g;
	res.second /= g;
	if (res.second < 0)
	{
		res.first *= -1;
		res.second *= -1;
	}
	return res;
}

pair<int, int> FractionCaculate::FractionMulti(pair<int, int> a, pair<int, int> b)		//分数乘法
{
	pair<int, int>res;
	res.first = a.first * b.first;								//分子分母对应相乘
	res.second = a.second * b.second;
	int g = Gcd(res.first, res.second);
	res.first /= g;
	res.second /= g;											//约分
	if (res.second < 0)											//规整化
	{
		res.first *= -1;
		res.second *= -1;
	}
	return res;
}

pair<int, int> FractionCaculate::FractionDivid(pair<int, int> a, pair<int, int> b)		//分数除法，类似乘法，此处并没有判断 b 是否为 0，输入参数需注意
{
	pair<int, int>res;
	res.first = a.first * b.second;							
	res.second = a.second * b.first;
	int g = Gcd(res.first, res.second);
	res.first /= g;
	res.second /= g;
	if (res.second < 0)
	{
		res.first *= -1;
		res.second *= -1;
	}
	return res;
}

pair<int, int> FractionCaculate::IntToFraction(int a)		//整数转分数
{
	return { a,1 };
}

pair<int, int> FractionCaculate::StringToFraction(string str)			//字符串转分数
{
	if (str == "")return identity;							//空字符串，返回 0
	pair<int, int> res;
	size_t idx = str.find('/');								//找除号
	if (idx != -1)											//有除号，是分数
	{
		res.first = stoi(str.substr(0, idx));				//转换分子，分母
		res.second = stoi(str.substr(idx + 2, str.size() - 1 - idx));
	}
	else													//没出号，是整数
	{
		res.first = stoi(str);								
		res.second = 1;
	}
	return res;
}

pair<int, int> FractionCaculate::CaculateEquation(string str)		//计算算式 str
{
	stack<pair<int, int>> nums;								//用于存储中间结果得栈
	string s, op = "+";										//操作数 和 操作符
	pair<int, int>cur = identity, pre = identity;
	bool flag = false;										//是否递归进入了括号
	size_t len = str.size();
	char c;
	for (int i = 0; i < len; i++)							//遍历字符串
	{
		c = str[i];
		if (c >= '0' && c <= '9')							//操作数字符串拼接
		{
			s += c;
		}
		else if (c < 0 || c == '+' || c == '-')				//操作符字符串拼接，此处用了 "÷" "×" 而不是 "/" "*"，所以需要拼接
		{
			op += c;
		}
		else if (c == '(')									//遇到括号，递归先计算括号里的子算式
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
		if ((c == ' ' && (s != "" || flag)) || i == len - 1)	//操作符和操作数都有了，注意初始操作符为 + ，只需要获取一个操作数即可，每次进入 if，结构都是： 操作符 右操作数
		{
			if (op == "+")									//加法，直接把操作数压入栈中
			{
				if (flag)nums.push(cur);
				else nums.push(StringToFraction(s));
			}
			else if (op == "-")								//减法，把操作数的相反数要入栈中
			{
				if (flag)
				{
					cur.first *= -1;
					nums.push(cur);
				}
				else nums.push(StringToFraction("-" + s));
			}
			else if (op == "×")								//乘法，取出之前的数，作为左操作数，与当前操作数经行乘法后压入栈中
			{
				pre = nums.top();
				nums.pop();
				if (!flag)cur = StringToFraction(s);
				nums.push(FractionMulti(pre, cur));
			}
			else if (op == "÷")								//除法类似乘法
			{
				pre = nums.top();
				nums.pop();
				if (!flag)cur = StringToFraction(s);
				nums.push(FractionDivid(pre, cur));
			}
			flag = false;
			op.clear();										//每次经行操作运算后，清空操作符和操作数
			s.clear();
		}
	}

	pair<int, int>res = identity, top = identity;			
	while (!nums.empty())									//最后将栈中的所有数做加法，得到结果
	{
		top = nums.top();
		nums.pop();
		res = FractionAdd(res, top);
	}
	return res;
}

bool FractionCaculate::BigThan(pair<int, int> a, pair<int, int> b)		//分数比大小
{
	return a.first / a.second > b.first / b.second;
}

string FractionCaculate::FractionToString(pair<int, int> a)				//分数转字符串
{
	if (a.first % a.second == 0)return to_string(a.first);		//可以化整
	return to_string(a.first) + "/" + to_string(a.second);		//不能化整
}