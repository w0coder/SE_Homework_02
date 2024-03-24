#include <iostream>
#include <fstream>
#include <algorithm>
#include <concepts>
#include "FractionCaculate.h"
#include "EquationGenerator.h"
using namespace std;

bool ReadStringsToVector(string path, vector<string>& vec)
{
	fstream file;
	file.open(path, ios::in);
	if (!file.is_open())
	{
		cout << "[error] open " << path << " failed" << endl;
		return false;
	}
	string s;
	while (getline(file, s))
	{
		vec.push_back(s);
	}
	file.close();
	return true;
}

bool ReadStringsToUnorderedMap(string path, unordered_map<string, string>& map)
{
	fstream file;
	file.open(path, ios::in);
	if (!file.is_open())
	{
		cout << "[error] open " << path << " failed" << endl;
		return false;
	}
	string s, key, value;
	int idx;
	while (getline(file, s))
	{
		idx = s.find(". ");
		if (idx == string::npos)continue;
		key = s.substr(0, idx);
		value = s.substr(idx + 2, s.size() - idx - 1);
		map[key] = value;
	}
	file.close();
	return true;
}

bool WriteGradToTxt(string path, vector<int>right, vector<int>wrong)
{
	fstream file;
	file.open(path, ios::out);
	if (!file.is_open())
	{
		cout << "[error] open " << path << " failed" << endl;
		return false;
	}

	size_t len = right.size();
	file << "Correct: " << len << " (";
	for (size_t i = 0; i < len - 1; i++)
	{
		file << right[i] << ", ";
	}
	file << right[len - 1] << ")" << endl;

	len = wrong.size();
	file << "Wrong: " << len << " (";
	for (size_t i = 0; i < len - 1; i++)
	{
		file << wrong[i] << ", ";
	}
	file << wrong[len - 1] << ")" << endl;
	file.close();
	return true;
}

bool WriteStringVectorToTxt(string path, vector<string>vec)
{
	fstream file;
	file.open(path, ios::out);
	if (!file.is_open())
	{
		cout << "[error] open " << path << " failed" << endl;
		return false;
	}
	size_t len = vec.size();
	for (size_t i = 0; i < len; i++)
	{
		file << i + 1 << ". " << vec[i] << endl;
	}
	file.close();
	return true;
}

bool DoGenerate(int n, int maxVal, string exePath)
{
	EquationGenerator* gen = new EquationGenerator();
	vector<string>ans, exp;

	string excSavePath = exePath + "\\Exercises.txt";
	string ansSavePath = exePath + "\\Answers.txt";

	gen->GenerateEquation(n, maxVal);

	exp = gen->GetEquations();				//式子和答案的列表
	ans = gen->GetAnsers();
	delete gen;
	//---BEG TEST PART---
	/*for (int i = 0; i < exp.size(); i++)
	{
		cout << "_____________________________________" << i + 1 << endl;
		cout << exp[i] << " = " << ans[i] << endl;
		cout << "Caculate by fractionCaculate class ans = " << FractionCaculate::FractionToString(FractionCaculate::CaculateEquation(exp[i])) << endl;
	}
	cout << "exc = " << excSavePath << endl;
	cout << "ans = " << ansSavePath << endl;*/
	//---END TEST PART---

	// 文件操作失败时，需要输出 error，并给出提示
	// 保存好后，输出提示保存的位置
	if (WriteStringVectorToTxt(excSavePath, exp))
	{
		cout << "[success] exercises have been saved to " << excSavePath << endl;
	}
	else
	{
		cout << "[error] failed save exercises to " << excSavePath << endl;
		return false;
	}
	if (WriteStringVectorToTxt(ansSavePath, ans))
	{
		cout << "[success] ansers have been saved to " << ansSavePath << endl;
	}
	else
	{
		cout << "[error] failed save answers to " << ansSavePath << endl;
		return false;
	}
	return true;
}

bool DoCheckAnswer(string excPath, string ansPath, string exePath)
{
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
	unordered_map<string, string> excs, anss;		//存放练习和要批改的答案的两个列表

	// 读取相应的信息到这两个表中
	if (!ReadStringsToUnorderedMap(excPath, excs))
	{
		cout << "[error] failed to read " << excPath << endl;
		return false;
	}
	if (!ReadStringsToUnorderedMap(ansPath, anss))
	{
		cout << "[error] failed to read " << ansPath << endl;
		return false;
	}


	vector<int> wrongIdx, rightIdx;					//存放正确题目索引和错误题目索引的两个列表
	int j = 0;
	for (auto exc : excs)		//统计正确和错误的题目
	{

		if (anss.find(exc.first) == anss.end())			//根本就没写这题
		{
			wrongIdx.push_back(stoi(exc.first));
			continue;
		}
		else if (FractionCaculate::FractionToString(FractionCaculate::CaculateEquation(exc.second)) == anss[exc.first])		//写对了
			rightIdx.push_back(stoi(exc.first));
		else wrongIdx.push_back(stoi(exc.first));
	}
	sort(wrongIdx.begin(), wrongIdx.end());
	sort(rightIdx.begin(), rightIdx.end());

	string gradePath = exePath + "\\Grade.txt";
	if (WriteGradToTxt(gradePath, rightIdx, wrongIdx))
		cout << "[success] grade have been saved to " << gradePath << endl;
	else return false;

	return true;
}

int HandlerInput(int argc, char* argv[])
{
	string exePath = argv[0];
	exePath = exePath.substr(0, exePath.find_last_of('\\'));		//获取 .exe 文件的路径

	int i = 1, n = 20, maxVal = 200;
	int op = 0;								//简单的 2 进制枚举  1：生成操作	2：批改作业操作
	string excPath = "", ansPath = "";
	while (i < argc)
	{
		if (argv[i][0] != '-' || i + 1 >= argc)
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
	srand(time(0));
	return HandlerInput(argc, argv);
}