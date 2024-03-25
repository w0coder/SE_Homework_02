#pragma once
#include <string>
#include <stack>
using namespace std;

static class FractionCaculate
{
public:
	static const pair<int, int> identity;			//������ 0

	static int Gcd(int a, int b);					//�����Լ��

	static int Lcm(int a, int b);					//����С������

	static pair<int, int> FractionAdd(pair<int, int>a, pair<int, int>b);		//�����ӷ�

	static pair<int, int> FractionMinus(pair<int, int>a, pair<int, int>b);		//��������

	static pair<int, int> FractionMulti(pair<int, int>a, pair<int, int>b);		//�����˷�

	static pair<int, int> FractionDivid(pair<int, int>a, pair<int, int>b);		//��������

	static pair<int, int> IntToFraction(int a);		//��intת��Ϊ����

	static pair<int, int> StringToFraction(string str);			//���ַ���ת��Ϊ����

	static pair<int, int> CaculateEquation(string str);			//����ʽ�� str ��ֵ

	static bool BigThan(pair<int, int> a, pair<int, int>b);		//�����Ƚϴ�С

	static string FractionToString(pair<int, int> a);			//������ת��Ϊ�ַ���
};