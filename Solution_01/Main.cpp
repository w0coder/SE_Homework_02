#include <iostream>
#include "FractionCaculate.h"
#include "EquationGenerator.h"
using namespace std;

bool DoGenerate(int n, int maxVal, string exePath)
{
	EquationGenerator* gen = new EquationGenerator();
	vector<string>ans, exp;

	//++++ ʽ�Ӻʹ𰸴洢��·��
	string excSavePath = exePath + "\\Exercises.txt";
	string ansSavePath = exePath + "\\Answers.txt";

	gen->GenerateEquation(n, maxVal);
	
	exp = gen->GetEquations();				//ʽ�Ӻʹ𰸵��б�
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
	 
	 
	
	//**** �����б������е�ֵ�浽��Ӧ���ļ��У�ע����ļ�ʱ�������Ƿ�Ҫ���
	//***  �ļ�����ʧ��ʱ����Ҫ��� error����������ʾ
	//***  ����ú������ʾ�����λ��

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
	vector<string>excs, anss;					//�����ϰ��Ҫ���ĵĴ𰸵������б�

	//**** ��ȡ��Ӧ����Ϣ�����������У�ע�⣬���п��ܻ���⣨�п�����������ûд���п�������д�����ûд�𰸣�����ʱ��Ҫ������Ĵ�����Ϊ""�������


	
	vector<int> wrongIdx, rightIdx;				//�����ȷ��Ŀ�����ʹ�����Ŀ�����������б�
	for (int i = 0; i < excs.size(); i++)		//ͳ����ȷ�ʹ������Ŀ
	{
		if (anss[i] == "")wrongIdx.push_back(i + 1);
		else if (FractionCaculate::FractionToString(FractionCaculate::CaculateEquation(excs[i])) == anss[i])rightIdx.push_back(i + 1);
		else wrongIdx.push_back(i + 1);
	}

	//++++ ���Ľ����д��·��
	string gradePath = exePath + "\Grade.txt";
	//**** �����ĵĽ��д���ļ�
	//***  д��ɹ��������ʾд���λ��

	return true;
}

int HandlerInput(int argc, char* argv[])
{
	string exePath = argv[0];
	exePath = exePath.substr(0,exePath.find_last_of('\\'));		//��ȡ .exe �ļ���·��


	int i = 1, n = 20, maxVal = 200;
	int op = 0;								//�򵥵� 2 ����ö��  1�����ɲ���	2��������ҵ����
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
	//---BEG TEST PART---
	cout << "op = " << op << endl;
	//---END TEST PART---

	
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