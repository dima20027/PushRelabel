#include "pch.h"
#include "CppUnitTest.h"
#include "push-relabel.h"
#include "OURvec.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(FileNotOpenVertexTest)
		{
			Graph myGr;
			try
			{
				myGr.addVertex("test.txt");
			}
			catch (invalid_argument error)
			{
				Assert::AreEqual("Error - file not open", error.what());
			}
			
		}
		TEST_METHOD(FileNoCorrectTest)
		{
			Graph myGr;
			try
			{
				myGr.addVertex("C:\\Users\\hv200\\Desktop\\SCIENCE\\Programms\\4 sem - Kursach\\Kursach\\test.txt");
				myGr.addEdge("C:\\Users\\hv200\\Desktop\\SCIENCE\\Programms\\4 sem - Kursach\\Kursach\\test.txt");
			}
			catch (invalid_argument error)
			{
				Assert::AreEqual("Your file not correct!", error.what());
			}

		}


		TEST_METHOD(EdgesTestErrorVer)
		{
			Graph myGr;
			try
			{
				myGr.addEdge("C:\\Users\\hv200\\Desktop\\SCIENCE\\Programms\\4 sem - Kursach\\Kursach\\test.txt");
			}
			catch (out_of_range error)
			{
				Assert::AreEqual("Error - Vertex no creates", error.what());
			}
		}
		TEST_METHOD(FileNotOpenEdgesTest)
		{
			Graph myGr;
			myGr.addVertex("C:\\Users\\hv200\\Desktop\\SCIENCE\\Programms\\4 sem - Kursach\\Kursach\\testTrue.txt");
			try
			{
				myGr.addEdge("test.txt");
			}
			catch (invalid_argument error)
			{
				Assert::AreEqual("Error - file not open", error.what());
			}
		}

		TEST_METHOD(VertexNoCreateTestMax)
		{
			Graph myGr;
			try
			{
				myGr.getMaxFlow();
			}
			catch (out_of_range error)
			{
				Assert::AreEqual("Error - Vertex no create", error.what());
			}
		}

		TEST_METHOD(EdgesNoCreateTestMax)
		{
			Graph myGr;
			myGr.addVertex("C:\\Users\\hv200\\Desktop\\SCIENCE\\Programms\\4 sem - Kursach\\Kursach\\testTrue.txt");
			try
			{
				myGr.getMaxFlow();
			}
			catch (out_of_range error)
			{
				Assert::AreEqual("Error - Edges no create", error.what());
			}
		}
		TEST_METHOD(TrueWorkTest)
		{
			Graph myGr;
			myGr.addVertex("C:\\Users\\hv200\\Desktop\\SCIENCE\\Programms\\4 sem - Kursach\\Kursach\\testTrue.txt");
			myGr.addEdge("C:\\Users\\hv200\\Desktop\\SCIENCE\\Programms\\4 sem - Kursach\\Kursach\\testTrue.txt");
			Assert::AreEqual(5, myGr.getMaxFlow());
		}
	};
}
