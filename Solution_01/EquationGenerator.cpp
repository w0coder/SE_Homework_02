#include "EquationGenerator.h"
#include "FractionCaculate.h"

EquationGenerator::EquationGenerator()
{
	ops = { "+","-","��","��" };			//��ʼ������������
}

vector<string> EquationGenerator::GetAnsers()		//��ȡ����ʽ�ӵĴ�
{
	vector<string> res;						
	for (auto str2vec : ans2EquaMap)				//�������е�ʽ��
		for (auto eq : str2vec.second)
			res.push_back(str2vec.first);			//���Ӧ�Ĵ�
	return res;
}

vector<string> EquationGenerator::GetEquations()	//��ȡ���ɵ�ʽ��
{
	vector<string>res;
	for (auto str2vec : ans2EquaMap)				//��������ʽ��
		for (auto eq : str2vec.second)
			res.push_back(eq);						//�������
	return res;
}

bool EquationGenerator::GenerateEquation(int n, int maxVal)			//����ʽ��
{
	if (maxVal<100 && n >((maxVal * maxVal * maxVal * maxVal) << 4))return false;			//�жϸ��ķ�Χ�Ƿ��ܹ����� n ��ʽ�ӣ��˴����ڼ���Ч��ʽ�ӱȽϼ��ԣ���ȥ����࣬���Դ����ж�
	Clear();														//������-ʽ�ӵĹ�ϣ��
	int overTime = 0;
	for (int i = 0; i < n;)											
	{
		int cnt = rand() % 3 + 1;									//ȷ���˴����ɵ�ʽ���в������ĸ���
		pair<int, int>res = FractionCaculate::identity;				//���ڽ�������ʽ�ӵĽ���ı�������ʼֵΪ������ 0
		string val = GenerateOneEquation(cnt, maxVal, res);			//����һ��ʽ��
		string key = FractionCaculate::FractionToString(res);		
		if (ans2EquaMap.find(key) == ans2EquaMap.end())				//��-ʽ�ӱ���û�иô𰸵��б����һ���б�
		{
			vector<string> vec;
			ans2EquaMap[key] = vec;
		}
		if (Check(key, val))										//����Ƿ��Ѿ������Ƶ�ʽ��
		{
			ans2EquaMap[key].push_back(val);
			i++;													//û�����Ƶģ���ӵ�����
			overTime = 0;
		}
		else overTime++;
		if (overTime > 2000)										//���Եĳ�ʱ���
		{
			Clear();
			return false;											//����ʧ��
		}
	}
	return true;													//���ɳɹ�
}

string EquationGenerator::GenerateOneEquation(int opCnt, int max, pair<int, int>& outRes)			//����һ��ʽ��
{
	int opcnt = opCnt;												//���ں�ߵ����ô���

	outRes = { rand() % max, 1 };									//���ɽ��
	if (rand() % 3 == 0)											//������ɷ���
	{
		outRes.second = rand() % (max - 1) + 1;
		outRes = FractionCaculate::FractionDivid({ outRes.first,1 }, { outRes.second,1 });
		if (outRes.second != 1 && outRes.first > outRes.second)		//���Ӵ��ڷ�ĸ���ٷ������������ӷ�ĸ�������˴����赣�ķ���Ϊ 0����ΪΪ 0 ʱ����ĳ����Ὣ��ĸ��Ϊ 1
		{
			outRes.first ^= outRes.second;
			outRes.second ^= outRes.first;
			outRes.first ^= outRes.second;
		}
	}
	return Generate_1(opcnt, max, outRes.first, "+", true, outRes.second);		//���������ݹ������һ������������ʽ��
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

bool EquationGenerator::Check(const string key, const string val)	//����Ƿ������Ƶ�ʽ��
{
	if (ans2EquaMap[key].size() == 0)return true;					//Ŀǰ��û����ʽ�ӽ����ͬ��ʽ��
	for (auto str : ans2EquaMap[key])								//�������������ж��Ƿ�����
		if (IsSimilarity(val, str))return false;
	return true;
}

bool EquationGenerator::IsSimilarity(string exp1, string exp2)		//�ж� exp1 �� exp2 �Ƿ����ƣ����Ż���
{
	if (exp1.size() != exp2.size())return false;					//���Ȳ�ͬ����Ϊ������
	int res = 0;
	for (int i = 0; i < exp1.size(); i++)							//������ͬʱ���������ַ���ͬ����Ϊ������
		res ^= (exp1[i] ^ exp2[i]);
	return res == 0;
}

string EquationGenerator::Generate_1(int& opCnt, int max, int target, string preOp, bool flag, int denominator)		//�ݹ������һ��ʽ�ӣ����denominator��Ϊ1��target���ʾ����
{
	if (opCnt <= 0)										//��������Ŀ����
	{	
		return to_string(target);
	}

	string op;											
	if (denominator != 1)op = "��";						//��������һ������������Ҫ����
	else												//�������������ɲ�����
	{
		op = ops[rand() % ops.size()];
		while (op == preOp && rand() % 3)				//��Ҫ�������ȡ��ͬ�ķ��ţ����ʿ��Կ���������
		{
			op = ops[rand() % ops.size()];
		}
	}

	int ta = 0, tb = 0;									//����Ŀ��ֵ�Ͳ���������Ŀ��ֵ�ֳ��������������� a ��b
	if (op == "-")
	{
		tb = rand() % (max - target);					//��֤ tb С�� max��target�� max ȡģ�õ������ص��� % 0
		ta = target + tb;								//��֤������������в�����
	}
	else if (op == "+")
	{
		tb = target == 0 ? 0 : rand() % target;			
		ta = target - tb;
	}
	else if (op == "��")
	{
		tb = sqrt(target);								
		while (tb > 1)									//����� target ��һ������
		{
			if (target % tb)tb--;
			else break;
		}
		ta = tb ? target / tb : 0;						//ȷ���������
	}
	else if (op == "��")
	{
		tb = denominator;
		ta = target;
		int rate = 2;									//���ӷ�ĸ�����Ϊ rate ��
		while (ta * rate < max && tb * rate < max && rand() % 4)		//�ڷ��ӷ�ĸС�� max ������£���� rate ��ֵ
		{
			ta *= rate;
			tb *= rate;
			rate++;
		}
	}

	--opCnt;											//target���ֽ�һ�Σ���������һ��
	string right, left;
	if (rand() % 2)										//�ݹ�˳��������ƫ��һ�࣬�����ȵĸ��ʾ�����ƽ��
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
	//��������ţ����Ż���
	if (preOp == "��")
	{
		if (op == "+" || op == "-")res = "(" + res + ")";
	}
	else if (preOp == "��")
	{
		if (op == "+" || op == "-")res = "(" + res + ")";
		else if ((op == "��" || op == "��") && flag)res = "(" + res + ")";
	}
	else if (preOp == "-")
	{
		if ((op == "+" || op == "-") && flag)res = "(" + res + ")";
	}
	return res;
}
