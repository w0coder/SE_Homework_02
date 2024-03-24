#include <iostream>
#include "FractionCaculate.h"
#include "EquationGenerator.h"
using namespace std;

bool DoGenerate(int n, int maxVal, string exePath)
{
	EquationGenerator* gen = new EquationGenerator();
	vector<string>ans, exp;

	//++++ 式子和答案存储的路径
	string excSavePath = exePath + "\\Exercises.txt";
	string ansSavePath = exePath + "\\Answers.txt";

	gen->GenerateEquation(n, maxVal);
	
	exp = gen->GetEquations();				//式子和答案的列表
	ans = gen->GetAnsers();

	//---BEG TEST PART---
	for (int i = 0; i < exp.size(); i++)
	{
		cout << "_____________________________________" << i + 1 << endl;
		cout << exp[i] << " = " << ans[i] << endl;
		cout << "Caculate by fractionCaculate class ans = " << FractionCaculate::FractionToString(FractionCaculate::CaculateEquation(exp[i])) << endl;
	}
	cout << "exc = " << excSavePath << endl;
	cout << "ans = " << ansSavePath << endl;
	//---END TEST PART---
	 
	 
	
	//**** 遍历列表，将表中的值存到对应的文件中，注意存文件时需求中是否要编号
	//***  文件操作失败时，需要输出 error，并给出提示
	//***  保存好后，输出提示保存的位置

	delete gen;
	return true;
}

bool DoCheckAnswer(string excPath, string ansPath, string exePath)
{
	//---BEG TEST PART---
	cout << "Cexc = " << excPath << endl;
	cout << "Cans = " << ansPath << endl;
	//---END TEST PART---

	if (excPath == "")
	{
		cout << "[error] -e <exercisefile>.txt the path of the exercise shoud not be empty" << endl;
		return false;
	}
	if (ansPath == "")
	{
		cout << "[error] -e <answerfile>.txt the path of the answer shoud not be empty" << endl;
		return false;
	}
	vector<string>excs, anss;					//存放练习和要批改的答案的两个列表

	//**** 读取相应的信息到这两个表中，注意，答案中可能会空题（有可能那题的序号没写，有可能那题写了序号没写答案），此时需要将该题的答案设置为""存入表中


	
	vector<int> wrongIdx, rightIdx;				//存放正确题目索引和错误题目索引的两个列表
	for (int i = 0; i < excs.size(); i++)		//统计正确和错误的题目
	{
		if (anss[i] == "")wrongIdx.push_back(i + 1);
		else if (FractionCaculate::FractionToString(FractionCaculate::CaculateEquation(excs[i])) == anss[i])rightIdx.push_back(i + 1);
		else wrongIdx.push_back(i + 1);
	}

	//++++ 批改结果的写入路径
	string gradePath = exePath + "\Grade.txt";
	//**** 把批改的结果写入文件
	//***  写入成功后，输出提示写入的位置

	return true;
}

int HandlerInput(int argc, char* argv[])
{
	string exePath = argv[0];
	exePath = exePath.substr(0,exePath.find_last_of('\\'));		//获取 .exe 文件的路径


	int i = 1, n = 20, maxVal = 200;
	int op = 0;								//简单的 2 进制枚举  1：生成操作	2：批改作业操作
	string excPath = "", ansPath = "";
	while (i < argc)
	{
		if (argv[i][0] != '-'|| i + 1 >= argc)
		{
			i++;
			continue;
		}

		switch (argv[i][1])
		{
		case 'n':		//设置生成数
		{
			if (maxVal < 0)
			{
				cout << "[error] -n count : the count should bigger than 0 !" << endl;
				return -1;
			}
			op |= 1;	//加上生成操作
			n = atoi(argv[i + 1]);
			break;
		}
		case 'r':		//设置最大值
		{
			if (maxVal <= 0)
			{
				cout << "[error] -r maxVal : the maxVal should bigger than 0 !" << endl;
				return -1;
			}
			op |= 1;	//加上生成操作
			maxVal = atoi(argv[i + 1]);
			break;
		}
		//读取文件
		case 'e':
		{
			excPath = argv[i + 1];
			op |= 2;	//加上批改作业操作
			break;
		}
		case 'a':
		{
			ansPath = argv[i + 1];
			op |= 2;	//加上批改作业操作
			break;
		}
		default:
			break;
		}
		i += 2;
	}
	//---BEG TEST PART---
	cout << "op = " << op << endl;
	//---END TEST PART---

	
	while (op)								//依据之前读取到的参数得到的操作符，经行相应的操作
	{
		int subOp = op & ((~(op)) + 1);		//取出 op 中的最后一个二进制 1
		op = op & (op - 1);					//去掉 op 中的最后一个二进制 1
		switch (subOp)
		{
		case 1:
		{
			if (DoGenerate(n, maxVal, exePath) == false)			//生成式子
			{
				cout << "[error] Generate whit max value = " << maxVal << " and count = " << n << " failed" << endl;
				return -1;
			}
			break;
		}
		case 2:
		{
			if (DoCheckAnswer(excPath, ansPath, exePath) == false)	//批改作业
			{
				cout << "[error] Check answers in " << ansPath << "of exercises in " << excPath << " failed" << endl;
				return -1;
			}
			break;	
		}
		default:
			break;
		}
	}

	return 1;
}

int main(int argc, char* argv[])
{
	HandlerInput(argc, argv);
	/*
	EquationGenerator* gen = new EquationGenerator();
	if (gen->GenerateEquation(n) == false)
	{
		cout << "the n is too big but the max is to small" << endl;
		return -1;
	}
	vector<string>ans, exp;
	exp = gen->GetEquations();
	ans = gen->GetAnsers();
	*/
	
	return 0;
}