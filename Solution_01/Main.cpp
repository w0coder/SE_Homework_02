#include <iostream>
#include <fstream>
#include <algorithm>
#include <concepts>
#include "FractionCaculate.h"
#include "EquationGenerator.h"
using namespace std;

bool ReadStringsToVector(string path, vector<string>& vec)			//��txt�ļ��е�ÿ�д浽�б���
{
	fstream file;
	file.open(path, ios::in);
	if (!file.is_open())											//�ļ���ʧ��
	{
		cout << "[error] open " << path << " failed" << endl;
		return false;
	}
	string s;
	while (getline(file, s))				//��ȡÿһ�в��浽vec��
	{
		vec.push_back(s);
	}
	file.close();
	return true;
}

bool ReadStringsToUnorderedMap(string path, unordered_map<string, string>& map)			//ר���ڴ���涨��ʽ�Ĵ��кŵ�txt����ϰ.txt����.txt)
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
	while (getline(file, s))				//��ȡÿһ�У�������Ž��д���ע������� ". " ǰ
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

bool WriteGradToTxt(string path, vector<int>right, vector<int>wrong)					//д���Ľ���ĺ���
{
	fstream file;
	file.open(path, ios::out);
	if (!file.is_open())
	{
		cout << "[error] open " << path << " failed" << endl;
		return false;
	}

	size_t len = right.size();						//����ʽд����ȷ����Ŀ
	file << "Correct: " << len << " (";
	for (size_t i = 0; i < len - 1; i++)
	{
		file << right[i] << ", ";
	}
	file << right[len - 1] << ")" << endl;

	len = wrong.size();								//����ʽд��������Ŀ
	file << "Wrong: " << len << " (";
	for (size_t i = 0; i < len - 1; i++)
	{
		file << wrong[i] << ", ";
	}
	file << wrong[len - 1] << ")" << endl;
	file.close();
	return true;
}

bool WriteStringVectorToTxt(string path, vector<string>vec)			//�� vec �е��ַ���д���ı���������ʽ���
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
		file << i + 1 << ". " << vec[i] << endl;	//��ţ�д���ַ���
	}
	file.close();
	return true;
}

bool DoGenerate(int n, int maxVal, string exePath)					//ִ�����ɲ���
{
	EquationGenerator* gen = new EquationGenerator();
	vector<string>ans, exp;

	string excSavePath = exePath + "\\Exercises.txt";				//��ϰ�ʹ𰸴�ŵ�λ��
	string ansSavePath = exePath + "\\Answers.txt";

	if (gen->GenerateEquation(n, maxVal) == false)		//����ʽ��
	{
		cout << "[error] failed generate equations" << endl;
		return false;
	}	

	exp = gen->GetEquations();				//ʽ�Ӻʹ𰸵��б�
	ans = gen->GetAnsers();
	delete gen;

	
	
	if (WriteStringVectorToTxt(excSavePath, exp))		// ����ú������ʾ�����λ��
	{
		cout << "[success] exercises have been saved to " << excSavePath << endl;
	}
	else												// �ļ�����ʧ��ʱ����Ҫ��� error����������ʾ
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
	for (auto exc : excs)							//ͳ����ȷ�ʹ������Ŀ
	{

		if (anss.find(exc.first) == anss.end())		//������ûд����
		{
			wrongIdx.push_back(stoi(exc.first));
			continue;
		}
		else if (FractionCaculate::FractionToString(FractionCaculate::CaculateEquation(exc.second)) == anss[exc.first])		//д����
			rightIdx.push_back(stoi(exc.first));
		else wrongIdx.push_back(stoi(exc.first));
	}
	sort(wrongIdx.begin(), wrongIdx.end());			//��ϣ����ģ�˳�����
	sort(rightIdx.begin(), rightIdx.end());

	string gradePath = exePath + "\\Grade.txt";
	if (WriteGradToTxt(gradePath, rightIdx, wrongIdx))			//�����Ľ��д������ļ�
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
	while (i < argc)						//�����������
	{
		if (argv[i][0] != '-' || i + 1 >= argc)
		{
			i++;
			continue;
		}

		switch (argv[i][1])
		{
		case 'n':		//�������������˴�˵����Ҫ�������ɲ���
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
		case 'r':		//�������ֵ���˴�˵����Ҫ�������ɲ���
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
		
		case 'e':		//��ȡ��ϰ�ļ����˴�˵����Ҫ�������Ĳ���
		{
			excPath = argv[i + 1];
			op |= 2;	//����������ҵ����
			break;
		}
		case 'a':		//��ȡ���ļ����˴�˵����Ҫ�������Ĳ���
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

	return 0;
}

int main(int argc, char* argv[])
{
	srand(time(0));
	return HandlerInput(argc, argv);
}