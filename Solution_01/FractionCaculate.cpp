#include "FractionCaculate.h"

const pair<int, int> FractionCaculate::identity = { 0,1 };

 int FractionCaculate::Gcd(int a, int b)			//շת����������Լ��
 {
	if (b == 0)return 0;
	int sign = (a > 0 && b > 0) || (a < 0 && b < 0) ? 1 : -1;
	a = a < 0 ? -a : a;
	b = b < 0 ? -b : b;
	int t;
	while (t = a % b)
	{
		a = b;
		b = t;
	}
	return b * sign;
}

int FractionCaculate::Lcm(int a, int b)			//����С���������˴����ܻ���� 0��ע�����������
{
	return  a * b / Gcd(a, b);
}

pair<int, int> FractionCaculate::FractionAdd(pair<int, int> a, pair<int, int> b)		//�����ӷ�
{
	pair<int, int>res;
	res.second = Lcm(a.second, b.second);												//���������С����������÷�ĸ
	res.first = a.first * res.second / a.second + b.first * res.second / b.second;		//�������

	int g = Gcd(res.first, res.second);

	res.first /= g;
	res.second /= g;											//Լ��	
	if (res.second < 0)											//ͳһ��ʾ����ĸΪ����
	{
		res.first *= -1;
		res.second *= -1;
	}
	return res;
}

pair<int, int> FractionCaculate::FractionMinus(pair<int, int> a, pair<int, int> b)		//�������������Ƽӷ�
{
	pair<int, int>res;
	res.second = Lcm(a.second, b.second);
	res.first = a.first * res.second / a.second - b.first * res.second / b.second;

	int g = Gcd(res.first, res.second);

	res.first /= g;
	res.second /= g;
	if (res.second < 0)
	{
		res.first *= -1;
		res.second *= -1;
	}
	return res;
}

pair<int, int> FractionCaculate::FractionMulti(pair<int, int> a, pair<int, int> b)		//�����˷�
{
	pair<int, int>res;
	res.first = a.first * b.first;								//���ӷ�ĸ��Ӧ���
	res.second = a.second * b.second;
	int g = Gcd(res.first, res.second);
	res.first /= g;
	res.second /= g;											//Լ��
	if (res.second < 0)											//������
	{
		res.first *= -1;
		res.second *= -1;
	}
	return res;
}

pair<int, int> FractionCaculate::FractionDivid(pair<int, int> a, pair<int, int> b)		//�������������Ƴ˷����˴���û���ж� b �Ƿ�Ϊ 0�����������ע��
{
	pair<int, int>res;
	res.first = a.first * b.second;							
	res.second = a.second * b.first;
	int g = Gcd(res.first, res.second);
	res.first /= g;
	res.second /= g;
	if (res.second < 0)
	{
		res.first *= -1;
		res.second *= -1;
	}
	return res;
}

pair<int, int> FractionCaculate::IntToFraction(int a)		//����ת����
{
	return { a,1 };
}

pair<int, int> FractionCaculate::StringToFraction(string str)			//�ַ���ת����
{
	if (str == "")return identity;							//���ַ��������� 0
	pair<int, int> res;
	size_t idx = str.find('/');								//�ҳ���
	if (idx != -1)											//�г��ţ��Ƿ���
	{
		res.first = stoi(str.substr(0, idx));				//ת�����ӣ���ĸ
		res.second = stoi(str.substr(idx + 2, str.size() - 1 - idx));
	}
	else													//û���ţ�������
	{
		res.first = stoi(str);								
		res.second = 1;
	}
	return res;
}

pair<int, int> FractionCaculate::CaculateEquation(string str)		//������ʽ str
{
	stack<pair<int, int>> nums;								//���ڴ洢�м�����ջ
	string s, op = "+";										//������ �� ������
	pair<int, int>cur = identity, pre = identity;
	bool flag = false;										//�Ƿ�ݹ����������
	size_t len = str.size();
	char c;
	for (int i = 0; i < len; i++)							//�����ַ���
	{
		c = str[i];
		if (c >= '0' && c <= '9')							//�������ַ���ƴ��
		{
			s += c;
		}
		else if (c < 0 || c == '+' || c == '-')				//�������ַ���ƴ�ӣ��˴����� "��" "��" ������ "/" "*"��������Ҫƴ��
		{
			op += c;
		}
		else if (c == '(')									//�������ţ��ݹ��ȼ��������������ʽ
		{
			int j = i, cnt = 0;
			for (; i < len; i++)
			{
				if (str[i] == '(')cnt++;
				else if (str[i] == ')')cnt--;
				if (cnt == 0)break;
			}
			cur = CaculateEquation(str.substr(j + 1, i - j - 1));
			flag = true;
		}
		if ((c == ' ' && (s != "" || flag)) || i == len - 1)	//�������Ͳ����������ˣ�ע���ʼ������Ϊ + ��ֻ��Ҫ��ȡһ�����������ɣ�ÿ�ν��� if���ṹ���ǣ� ������ �Ҳ�����
		{
			if (op == "+")									//�ӷ���ֱ�ӰѲ�����ѹ��ջ��
			{
				if (flag)nums.push(cur);
				else nums.push(StringToFraction(s));
			}
			else if (op == "-")								//�������Ѳ��������෴��Ҫ��ջ��
			{
				if (flag)
				{
					cur.first *= -1;
					nums.push(cur);
				}
				else nums.push(StringToFraction("-" + s));
			}
			else if (op == "��")								//�˷���ȡ��֮ǰ��������Ϊ����������뵱ǰ���������г˷���ѹ��ջ��
			{
				pre = nums.top();
				nums.pop();
				if (!flag)cur = StringToFraction(s);
				nums.push(FractionMulti(pre, cur));
			}
			else if (op == "��")								//�������Ƴ˷�
			{
				pre = nums.top();
				nums.pop();
				if (!flag)cur = StringToFraction(s);
				nums.push(FractionDivid(pre, cur));
			}
			flag = false;
			op.clear();										//ÿ�ξ��в����������ղ������Ͳ�����
			s.clear();
		}
	}

	pair<int, int>res = identity, top = identity;			
	while (!nums.empty())									//���ջ�е����������ӷ����õ����
	{
		top = nums.top();
		nums.pop();
		res = FractionAdd(res, top);
	}
	return res;
}

bool FractionCaculate::BigThan(pair<int, int> a, pair<int, int> b)		//�����ȴ�С
{
	return a.first / a.second > b.first / b.second;
}

string FractionCaculate::FractionToString(pair<int, int> a)				//����ת�ַ���
{
	if (a.first % a.second == 0)return to_string(a.first);		//���Ի���
	return to_string(a.first) + "/" + to_string(a.second);		//���ܻ���
}