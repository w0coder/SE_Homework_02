#include <iostream>
#include <fstream>
#include <algorithm>
#include <concepts>
#include "FractionCaculate.h"
#include "EquationGenerator.h"
using namespace std;

bool ReadStringsToVector(string path, vector<string>& vec)			//将txt文件中的每行存到列表中
{
	fstream file;
	file.open(path, ios::in);
	if (!file.is_open())											//文件打开失败
	{
		cout << "[error] open " << path << " failed" << endl;
		return false;
	}
	string s;
	while (getline(file, s))				//读取每一行并存到vec中
	{
		vec.push_back(s);
	}
	file.close();
	return true;
}

bool ReadStringsToUnorderedMap(string path, unordered_map<string, string>& map)			//专用于处理规定格式的带行号的txt（练习.txt，答案.txt)
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
	while (getline(file, s))				//读取每一行，并对序号进行处理，注意序号在 ". " 前
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

bool WriteGradToTxt(string path, vector<int>right, vector<int>wrong)					//写批改结果的函数
{
	fstream file;
	file.open(path, ios::out);
	if (!file.is_open())
	{
		cout << "[error] open " << path << " failed" << endl;
		return false;
	}

	size_t len = right.size();						//按格式写入正确的题目
	file << "Correct: " << len << " (";
	for (size_t i = 0; i < len - 1; i++)
	{
		file << right[i] << ", ";
	}
	file << right[len - 1] << ")" << endl;

	len = wrong.size();								//按格式写入错误的题目
	file << "Wrong: " << len << " (";
	for (size_t i = 0; i < len - 1; i++)
	{
		file << wrong[i] << ", ";
	}
	file << wrong[len - 1] << ")" << endl;
	file.close();
	return true;
}

bool WriteStringVectorToTxt(string path, vector<string>vec)			//将 vec 中的字符串写入文本，并按格式标号
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
		file << i + 1 << ". " << vec[i] << endl;	//标号，写入字符串
	}
	file.close();
	return true;
}

bool DoGenerate(int n, int maxVal, string exePath)					//执行生成操作
{
	EquationGenerator* gen = new EquationGenerator();
	vector<string>ans, exp;

	string excSavePath = exePath + "\\Exercises.txt";				//练习和答案存放的位置
	string ansSavePath = exePath + "\\Answers.txt";

	if (gen->GenerateEquation(n, maxVal) == false)		//生成式子
	{
		cout << "[error] failed generate equations" << endl;
		return false;
	}	

	exp = gen->GetEquations();				//式子和答案的列表
	ans = gen->GetAnsers();
	delete gen;

	
	
	if (WriteStringVectorToTxt(excSavePath, exp))		// 保存好后，输出提示保存的位置
	{
		cout << "[success] exercises have been saved to " << excSavePath << endl;
	}
	else												// 文件操作失败时，需要输出 error，并给出提示
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
	for (auto exc : excs)							//统计正确和错误的题目
	{

		if (anss.find(exc.first) == anss.end())		//根本就没写这题
		{
			wrongIdx.push_back(stoi(exc.first));
			continue;
		}
		else if (FractionCaculate::FractionToString(FractionCaculate::CaculateEquation(exc.second)) == anss[exc.first])		//写对了
			rightIdx.push_back(stoi(exc.first));
		else wrongIdx.push_back(stoi(exc.first));
	}
	sort(wrongIdx.begin(), wrongIdx.end());			//哈希表处理的，顺序会乱
	sort(rightIdx.begin(), rightIdx.end());

	string gradePath = exePath + "\\Grade.txt";
	if (WriteGradToTxt(gradePath, rightIdx, wrongIdx))			//将批改结果写入相关文件
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
	while (i < argc)						//输入参数处理
	{
		if (argv[i][0] != '-' || i + 1 >= argc)
		{
			i++;
			continue;
		}

		switch (argv[i][1])
		{
		case 'n':		//设置生成数，此处说明需要进行生成操作
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
		case 'r':		//设置最大值，此处说明需要进行生成操作
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
		
		case 'e':		//读取练习文件，此处说明需要进行批改操作
		{
			excPath = argv[i + 1];
			op |= 2;	//加上批改作业操作
			break;
		}
		case 'a':		//读取答案文件，此处说明需要进行批改操作
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

	return 0;
}

int main(int argc, char* argv[])
{
	srand(time(0));
	return HandlerInput(argc, argv);
}