#pragma once
#include <string>
#include <stack>
using namespace std;

static class FractionCaculate
{
public:
	static const pair<int, int> identity;			//分数的 0

	static int Gcd(int a, int b);					//求最大公约数

	static int Lcm(int a, int b);					//求最小公倍数

	static pair<int, int> FractionAdd(pair<int, int>a, pair<int, int>b);		//分数加法

	static pair<int, int> FractionMinus(pair<int, int>a, pair<int, int>b);		//分数减法

	static pair<int, int> FractionMulti(pair<int, int>a, pair<int, int>b);		//分数乘法

	static pair<int, int> FractionDivid(pair<int, int>a, pair<int, int>b);		//分数除法

	static pair<int, int> IntToFraction(int a);		//将int转化为分数

	static pair<int, int> StringToFraction(string str);			//将字符串转化为分数

	static pair<int, int> CaculateEquation(string str);			//计算式子 str 的值

	static bool BigThan(pair<int, int> a, pair<int, int>b);		//分数比较大小

	static string FractionToString(pair<int, int> a);			//将分数转化为字符串
};