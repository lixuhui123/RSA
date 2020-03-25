#include "RSA.h"
#include <iostream>
#include <fstream>
 
using namespace std;
void testfile()
{
	ifstream fin("test.txt");
	//以文本形式打开，按照空格或者换行读取数据
	char c;
	int a;
	double d;
	fin >> c;
	fin >> a;
	fin >> d;
	cout << c << " " << a << " " << d << " ";


}

void testBoostBigInt()
{
	boost::multiprecision::cpp_int ci;
	ci = 1024;
	cout << ci << endl;
	string num = "156154351341524515151451321";
	boost::multiprecision::cpp_int cpp(num);
	cout << cpp << endl;

}
void testrandom()
{
	boost::random::mt19937 gen(time(NULL));
	string num = "156154351341524515151451321";
	boost::multiprecision::cpp_int cpp(num);
	//boost::random::uniform_real_distribution<DataType> dist(3, INT_FAST32_MAX);
	boost::random::uniform_int_distribution <DataType> dist(2, DataType(1)<<1000);
	for (int i = 0; i < 10; ++i)
	{
		DataType num = dist(gen);
		cout << num << endl<<endl;
	}
	//boost::random::mt19937 rng;         // produces randomness out of thin air
	//							   // see pseudo-random number generators
	//boost::random::uniform_int_distribution<DataType> six(1, 6);
	//// distribution that maps to 1..6
	//// see random number distributions
	//DataType x = six(rng);
	//cout << x<<endl;
}
int main()
{
	/*RSA test;
	test.getKeys();
	Key key;
	key = test.getallKey();
	cout << key._dkey << endl << key._ekey << endl << key._pkey << endl;
	DataType m = test.ecrept(22, key._ekey, key._pkey);
	cout << m << endl;
	cout << test.decrept(m, key._dkey, key._pkey) << endl;*/

	//testfile();

	RSA test;
	test.ecrept("test.txt", "ercept.txt");
	test.decrept("ercept.txt", "dercept.txt");
	 
	//testBoostBigInt();

	//testrandom();
	system("pause");

	return 0;
}