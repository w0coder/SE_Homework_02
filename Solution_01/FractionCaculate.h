#pragma once
#include <string>
#include <stack>
using namespace std;

static class FractionCaculate
{
public:
	static const pair<int, int> identity;

	static int Gcd(int a, int b);

	static int Lcm(int a, int b);

	static pair<int, int> FractionAdd(pair<int, int>a, pair<int, int>b);

	static pair<int, int> FractionMinus(pair<int, int>a, pair<int, int>b);

	static pair<int, int> FractionMulti(pair<int, int>a, pair<int, int>b);

	static pair<int, int> FractionDivid(pair<int, int>a, pair<int, int>b);

	static pair<int, int> IntToFraction(int a);

	static pair<int, int> StringToFraction(string str);

	static pair<int, int> CaculateEquation(string str);

	static bool BigThan(pair<int, int> a, pair<int, int>b);

	static string FractionToString(pair<int, int> a);
};