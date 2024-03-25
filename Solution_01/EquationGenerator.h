#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class FractionCaculate;

class EquationGenerator						//��ʽ������
{
public:
	EquationGenerator();

	vector<string> GetAnsers();				//��ȡ���ɵ�ʽ�ӵĴ�

	vector<string> GetEquations();			//��ȡ���ɵ�ʽ��

	bool GenerateEquation(int n = 10, int maxVal = 1000);						//���� n ��ʽ�ӣ�ʽ���е���ֵ���ֵ������ maxVal����Сֵ��С�� 0

	string GenerateOneEquation(int opCnt, int max, pair<int, int>& outRes);		//���� 1 ��ʽ�ӣ�ʽ�ӵĽ���� outRes ���գ�����ʽ���ַ���

	~EquationGenerator();					
		
	void Clear();							//����𰸵�ʽ�ӵĹ�ϣ��

private:
	bool Check(const string key, const string val);			//����Ƿ������Ƶ�ʽ��

	bool IsSimilarity(string exp1, string exp2);			//��� exp1 exp2 �Ƿ����ƣ����Ľ���

	string Generate_1(int& opCnt, int max, int target, string preOp, bool flag, int denominator);		//���� 1 ��ʽ�ӵĵݹ麯��

	vector<string> ops;						//����������

	unordered_map<string, vector<string>> ans2EquaMap;		//�𰸵�ʽ�ӵĹ�ϣ��
};