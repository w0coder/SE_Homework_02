#include "pch.h"
#include "CppUnitTest.h"
#include "..\Solution_01\FractionCaculate.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SolutionUnitTest
{
	TEST_CLASS(FractionCaculateUnitTest)
	{
	public:
		TEST_METHOD(FractionAdd_Test)		//测试分数加法，输入合理的情况下（分子必不为0）
		{
			int len = 7;
			pair<int, int> args[] =			//相加的两个数
			{
				{1,3},{2,9},
				{2,1},{3,4},
				{10,7},{7,4},
				{0,7},{9,1},
				{0,8},{0,9},
				{-7,2},{-13,4},
				{-1,6},{5,-6}
			};
			pair<int, int> exps[] =			//期望的值
			{
				{5,9},
				{11,4},
				{89,28},
				{9,1},
				{0,1},
				{-27,4},
				{-1,1}
			};
			for (int i = 0; i < len; i++)
			{
				Assert::AreEqual(
					FractionCaculate::FractionToString(exps[i]),
					FractionCaculate::FractionToString(FractionCaculate::FractionAdd(args[i << 1], args[(i << 1) | 1]))
				);
			}
		}

		TEST_METHOD(FractionMinus_Test)		//测试分数减法，输入合理的情况下（分子必不为0）
		{
			int len = 5;
			pair<int, int> args[] =
			{
				{1,5},{1,1},
				{0,6},{7,3},
				{100,13},{270,103},
				{99,11},{88,22},
				{73,2},{0,1}
			};
			pair<int, int> exps[] =
			{
				{-4,5},
				{-7,3},
				{6790,1339},
				{5,1},
				{73,2}
			};
			for (int i = 0; i < len; i++)
			{
				Assert::AreEqual(
					FractionCaculate::FractionToString(exps[i]),
					FractionCaculate::FractionToString(FractionCaculate::FractionMinus(args[i << 1], args[(i << 1) | 1]))
				);
			}
		}

		TEST_METHOD(FractionMulti_Test)
		{
			int len = 7;
			pair<int, int> args[] =
			{
				{1,5},{1,1},
				{0,6},{7,3},
				{100,9},{270,103},
				{99,11},{88,22},
				{73,2},{0,1},
				{-3,4},{4,3},
				{3,7},{-3,7}
			};
			pair<int, int> exps[] =
			{
				{1,5},
				{0,1},
				{3000,103},
				{36,1},
				{0,1},
				{-1,1},
				{-9,49}
			};
			for (int i = 0; i < len; i++)
			{
				Assert::AreEqual(
					FractionCaculate::FractionToString(exps[i]),
					FractionCaculate::FractionToString(FractionCaculate::FractionMulti(args[i << 1], args[(i << 1) | 1]))
				);
			}
		}

		TEST_METHOD(FractionDivid_Test)
		{
			int len = 6;
			pair<int, int> args[] =
			{
				{1,3},{2,9},
				{2,1},{3,4},
				{-10,7},{7,4},
				{0,7},{9,1},
				{0,8},{-1,9},
				{-1,22},{-7,88}
			};
			pair<int, int> exps[] =
			{
				{3,2},
				{8,3},
				{-40,49},
				{0,1},
				{0,1},
				{4,7}
			};
			for (int i = 0; i < len; i++)
			{
				Assert::AreEqual(
					FractionCaculate::FractionToString(exps[i]),
					FractionCaculate::FractionToString(FractionCaculate::FractionDivid(args[i << 1], args[(i << 1) | 1]))
				);
			}
		}

		TEST_METHOD(CaculateEquation_Test)
		{
			int len = 15;
			string args[] =
			{
				"29 × (280 ÷ 2 - 133)",
				"347 - 12 × 12",
				"84 ÷ 1 + 119",
				"277 - (33 + 41)",
				"406 ÷(2 + 379 - 379)",
				"29 × 168 ÷ 24",
				"497 - 294",
				"(378 + 28 ÷ 1) ÷ 2",
				"203 ÷ 1",
				"414 - 211",
				"111 + 244 ÷ 2 - 30",
				"456 - (121 + 132)",
				"463 + 7 × 3 - 281",
				"29 × 7",
				"91 ÷ 408"
			};
			string exps[] =
			{
				"203",
				"203",
				"203",
				"203",
				"203",
				"203",
				"203",
				"203",
				"203",
				"203",
				"203",
				"203",
				"203",
				"203",
				"91/408"
			};
			for (int i = 0; i < len; i++)
			{
				Assert::AreEqual(
					exps[i],
					FractionCaculate::FractionToString(FractionCaculate::CaculateEquation(args[i]))
				);
			}
		}
	};
}
