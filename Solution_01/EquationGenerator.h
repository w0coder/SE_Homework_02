#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class FractionCaculate;

class EquationGenerator						//算式生成类
{
public:
	EquationGenerator();

	vector<string> GetAnsers();				//获取生成的式子的答案

	vector<string> GetEquations();			//获取生成的式子

	bool GenerateEquation(int n = 10, int maxVal = 1000);						//生成 n 条式子，式子中的数值最大值不超过 maxVal，最小值不小于 0

	string GenerateOneEquation(int opCnt, int max, pair<int, int>& outRes);		//生成 1 条式子，式子的结果用 outRes 接收，返回式子字符串

	~EquationGenerator();					
		
	void Clear();							//清理答案到式子的哈希表

private:
	bool Check(const string key, const string val);			//检查是否有相似的式子

	bool IsSimilarity(string exp1, string exp2);			//检查 exp1 exp2 是否相似（待改进）

	string Generate_1(int& opCnt, int max, int target, string preOp, bool flag, int denominator);		//生成 1 条式子的递归函数

	vector<string> ops;						//操作符数组

	unordered_map<string, vector<string>> ans2EquaMap;		//答案到式子的哈希表
};