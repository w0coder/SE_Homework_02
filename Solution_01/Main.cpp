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

	exp = gen->GetEquations();				//ʽ�Ӻʹ𰸵��б�
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

	// �ļ�����ʧ��ʱ����Ҫ��� error����������ʾ
	// ����ú������ʾ�����λ��
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
	unordered_map<string, string> excs, anss;		//�����ϰ��Ҫ���ĵĴ𰸵������б�

	// ��ȡ��Ӧ����Ϣ������������
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


	vector<int> wrongIdx, rightIdx;					//�����ȷ��Ŀ�����ʹ�����Ŀ�����������б�
	int j = 0;
	for (auto exc : excs)		//ͳ����ȷ�ʹ������Ŀ
	{

		if (anss.find(exc.first) == anss.end())			//������ûд����
		{
			wrongIdx.push_back(stoi(exc.first));
			continue;
		}
		else if (FractionCaculate::FractionToString(FractionCaculate::CaculateEquation(exc.second)) == anss[exc.first])		//д����
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
	exePath = exePath.substr(0, exePath.find_last_of('\\'));		//��ȡ .exe �ļ���·��

	int i = 1, n = 20, maxVal = 200;
	int op = 0;								//�򵥵� 2 ����ö��  1�����ɲ���	2��������ҵ����
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
		case 'n':		//����������
		{
			if (maxVal < 0)
			{
				cout << "[error] -n count : the count should bigger than 0 !" << endl;
				return -1;
			}
			op |= 1;	//�������ɲ���
			n = atoi(argv[i + 1]);
			break;
		}
		case 'r':		//�������ֵ
		{
			if (maxVal <= 0)
			{
				cout << "[error] -r maxVal : the maxVal should bigger than 0 !" << endl;
				return -1;
			}
			op |= 1;	//�������ɲ���
			maxVal = atoi(argv[i + 1]);
			break;
		}
		//��ȡ�ļ�
		case 'e':
		{
			excPath = argv[i + 1];
			op |= 2;	//����������ҵ����
			break;
		}
		case 'a':
		{
			ansPath = argv[i + 1];
			op |= 2;	//����������ҵ����
			break;
		}
		default:
			break;
		}
		i += 2;
	}

	while (op)								//����֮ǰ��ȡ���Ĳ����õ��Ĳ�������������Ӧ�Ĳ���
	{
		int subOp = op & ((~(op)) + 1);		//ȡ�� op �е����һ�������� 1
		op = op & (op - 1);					//ȥ�� op �е����һ�������� 1
		switch (subOp)
		{
		case 1:
		{
			if (DoGenerate(n, maxVal, exePath) == false)			//����ʽ��
			{
				cout << "[error] Generate whit max value = " << maxVal << " and count = " << n << " failed" << endl;
				return -1;
			}
			break;
		}
		case 2:
		{
			if (DoCheckAnswer(excPath, ansPath, exePath) == false)	//������ҵ
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