#include "EquationGenerator.h"
#include "FractionCaculate.h"

EquationGenerator::EquationGenerator()
{
	ops = { "+","-","×","÷" };			//初始化操作符数组
}

vector<string> EquationGenerator::GetAnsers()		//获取生成式子的答案
{
	vector<string> res;						
	for (auto str2vec : ans2EquaMap)				//遍历所有的式子
		for (auto eq : str2vec.second)
			res.push_back(str2vec.first);			//存对应的答案
	return res;
}

vector<string> EquationGenerator::GetEquations()	//获取生成的式子
{
	vector<string>res;
	for (auto str2vec : ans2EquaMap)				//遍历所有式子
		for (auto eq : str2vec.second)
			res.push_back(eq);						//加入表中
	return res;
}

bool EquationGenerator::GenerateEquation(int n, int maxVal)			//生成式子
{
	if (maxVal<100 && n >((maxVal * maxVal * maxVal * maxVal) << 4))return false;			//判断给的范围是否能够生成 n 条式子，此处由于检查等效的式子比较简略，会去掉许多，所以粗略判断
	Clear();														//清理存答案-式子的哈希表
	int overTime = 0;
	for (int i = 0; i < n;)											
	{
		int cnt = rand() % 3 + 1;									//确定此次生成的式子中操作符的个数
		pair<int, int>res = FractionCaculate::identity;				//用于接收生成式子的结果的变量，初始值为分数的 0
		string val = GenerateOneEquation(cnt, maxVal, res);			//生成一条式子
		string key = FractionCaculate::FractionToString(res);		
		if (ans2EquaMap.find(key) == ans2EquaMap.end())				//答案-式子表中没有该答案的列表，添加一个列表
		{
			vector<string> vec;
			ans2EquaMap[key] = vec;
		}
		if (Check(key, val))										//检查是否已经有相似的式子
		{
			ans2EquaMap[key].push_back(val);
			i++;													//没有相似的，添加到表中
			overTime = 0;
		}
		else overTime++;
		if (overTime > 2000)										//简略的超时检测
		{
			Clear();
			return false;											//生成失败
		}
	}
	return true;													//生成成功
}

string EquationGenerator::GenerateOneEquation(int opCnt, int max, pair<int, int>& outRes)			//生成一条式子
{
	int opcnt = opCnt;												//用于后边的引用传参

	outRes = { rand() % max, 1 };									//生成结果
	if (rand() % 3 == 0)											//随机生成分数
	{
		outRes.second = rand() % (max - 1) + 1;
		outRes = FractionCaculate::FractionDivid({ outRes.first,1 }, { outRes.second,1 });
		if (outRes.second != 1 && outRes.first > outRes.second)		//分子大于分母，假分数，交换分子分母交换，此处无需担心分子为 0，因为为 0 时上面的除法会将分母置为 1
		{
			outRes.first ^= outRes.second;
			outRes.second ^= outRes.first;
			outRes.first ^= outRes.second;
		}
	}
	return Generate_1(opcnt, max, outRes.first, "+", true, outRes.second);		//由随机结果递归地生成一条满足条件的式子
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

bool EquationGenerator::Check(const string key, const string val)	//检查是否有相似的式子
{
	if (ans2EquaMap[key].size() == 0)return true;					//目前还没有与式子结果相同的式子
	for (auto str : ans2EquaMap[key])								//有则遍历，逐个判断是否相似
		if (IsSimilarity(val, str))return false;
	return true;
}

bool EquationGenerator::IsSimilarity(string exp1, string exp2)		//判断 exp1 和 exp2 是否相似（待优化）
{
	if (exp1.size() != exp2.size())return false;					//长度不同，认为不相似
	int res = 0;
	for (int i = 0; i < exp1.size(); i++)							//长度相同时，所含的字符不同，认为不相似
		res ^= (exp1[i] ^ exp2[i]);
	return res == 0;
}

string EquationGenerator::Generate_1(int& opCnt, int max, int target, string preOp, bool flag, int denominator)		//递归的生成一条式子，如果denominator不为1，target则表示分子
{
	if (opCnt <= 0)										//操作符数目到了
	{	
		return to_string(target);
	}

	string op;											
	if (denominator != 1)op = "÷";						//期望的是一个分数，则需要除法
	else												//其他情况随机生成操作符
	{
		op = ops[rand() % ops.size()];
		while (op == preOp && rand() % 3)				//不要连续多次取相同的符号（概率可以看着来调）
		{
			op = ops[rand() % ops.size()];
		}
	}

	int ta = 0, tb = 0;									//依据目标值和操作符，将目标值分成左右两个操作数 a ，b
	if (op == "-")
	{
		tb = rand() % (max - target);					//保证 tb 小于 max，target由 max 取模得到，不必担心 % 0
		ta = target + tb;								//保证左操作数大于有操作数
	}
	else if (op == "+")
	{
		tb = target == 0 ? 0 : rand() % target;			
		ta = target - tb;
	}
	else if (op == "×")
	{
		tb = sqrt(target);								
		while (tb > 1)									//随机找 target 的一个因数
		{
			if (target % tb)tb--;
			else break;
		}
		ta = tb ? target / tb : 0;						//确定左操作数
	}
	else if (op == "÷")
	{
		tb = denominator;
		ta = target;
		int rate = 2;									//分子分母随机变为 rate 倍
		while (ta * rate < max && tb * rate < max && rand() % 4)		//在分子分母小于 max 的情况下，随机 rate 的值
		{
			ta *= rate;
			tb *= rate;
			rate++;
		}
	}

	--opCnt;											//target被分解一次，操作符少一个
	string right, left;
	if (rand() % 2)										//递归顺序导致括号偏向一侧，左右先的概率均等以平衡
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
