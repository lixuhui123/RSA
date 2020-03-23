#include "RSA.h"
#include <iostream>
#include <algorithm>
#include <time.h>
#include <fstream>
using namespace std;
RSA::RSA()
{
	getKeys();
}
DataType RSA::getPrime()
{
	//srand(time(NULL));
	//DataType prime;
	//while (true)
	//{
	//	prime = rand() % 100 + 2;//不要0 1
	//	if (isPrime(prime))
	//	{
	//		break;
 //		}
	//}
	//return prime;
	boost::random::mt19937 gen(time(NULL));

	boost::random::uniform_int_distribution <DataType> dist(2, DataType(1) << 200);
	DataType prime;
	while (true)
	{
		cout << "get prime" << endl;
		prime = dist(gen);
		if (isPrimeBigInt(prime))
		{
			cout << "get prime finsh" << endl;
			break;
		}
	}
	return prime; 
	
}
bool RSA::isPrimeBigInt(DataType data)
{
	 
	brdm::mt11213b gen(time(nullptr));
	if (miller_rabin_test(data, 25, gen))
	{
		if (miller_rabin_test((data - 1) / 2, 25, gen))
		{
			return true;
		}
	}
	return false;
}
DataType RSA::getPkey(DataType prime1, DataType prime2)
{
	//求n
	return prime1 * prime2;
}
DataType RSA::getOrla(DataType prime1, DataType prime2)
{
	//求f(n)
	return (prime1 - 1)*(prime2 - 1);
}
DataType RSA::getEkey(DataType orla)
{
	//加密秘密钥1< e < f(n) e 与f(n)互质
	//srand(time(NULL));
	brdm::mt19937 gen(time(NULL));
	brdm::uniform_int_distribution<DataType> dist(2, orla);

	DataType ekey;
	while (true)
	{
		cout << "get ekey" << endl;
		ekey = dist(gen);//保证小于f(n)
		if (ekey>1 && (getGcd(ekey,orla)==1))//两个数互质最大公约数为1
		{
			cout << "get ekey finsh" << endl;
			return ekey;
		}
	}

}
DataType RSA::getDkey(DataType ekey, DataType orla)
{
	//解密秘钥 e*d%f(n) =1，模反元素	
	DataType x = 0, y = 0;
	cout << "get dkey" << endl;
	exGcd(ekey, orla, x, y);//形参是引用，在调用函数中直接改变值
	return (x%orla + orla) % orla;//处理成较小的正值,主要用于负数
	//根据同余定理，(x%orla + orla) % orla = x%orla%orla,


	/*DataType dkey;
	dkey = orla / ekey;
	while (true)
	{
		if (((ekey*dkey) % orla) == 1)
			return dkey;
		++dkey;
	}*/
}
DataType RSA::getGcd(DataType data1, DataType data2)
{
	//求最大公约数，两个质数的最大公约数为1
	//使用辗转相除法
	DataType residdual;
	while (residdual = data1%data2)
	{
		data1 = data2;
		data2 = residdual;
	}
	return data2;
}

DataType RSA::exGcd(DataType a, DataType b, DataType &x, DataType &y)
{
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}
	DataType gcd = exGcd(b, a%b, x, y);
	DataType x1 = x, y1 = y;
	x = y1;
	y = x1 - a / b * y1;
	return gcd;
}

bool RSA::isPrime(DataType data)
{

	for (int i=2;i <= sqrt(data) ; ++i)
	{
		if (data % i == 0)
		{
			return false;
		}
	}
	return true;
}
void RSA::getKeys()
{
	DataType prime1 = getPrime();
	DataType prime2 = getPrime();
	while (prime1==prime2)
	{
		prime1 = getPrime();
	}
	DataType orla = getOrla(prime1, prime2);
	_key._pkey = getPkey(prime1, prime2);
	_key._ekey = getEkey(orla);
	_key._dkey = getDkey(_key._ekey, orla);
}
Key RSA::getallKey()
{
	return _key;
}
DataType RSA::ecrept(DataType data, DataType ekey, DataType pkey)//加密 m=d^e % modp
{
	//i :0....n
	DataType Ai = data;// data^(b0*2^0),因为bi=1的时候才计算，bi=0的时候不计算，所以Ai的初始值为data
	DataType msgE = 1;//第一次密文初始化为1，因为A0=A0没有谁和它乘给msgE为1
	//data ^ ekey %pkey
	while (ekey)//计算多少位
	{
		if (ekey & 1)
		{
			msgE = (msgE *Ai) % pkey;
		}
		ekey >>= 1;
		Ai = (Ai*Ai) % pkey;
	}
	return msgE;
}
DataType RSA::decrept(DataType data, DataType dkey, DataType pkey)//解密 c=m^d % modn
{
	return ecrept(data, dkey, pkey);
}

void RSA::ecrept(const char * filein, const char * fileout)
{
	ifstream fin(filein,ifstream::binary);
	ofstream fout(fileout,ofstream::binary);

	if (!fin.is_open())
	{
		perror("input file open failed!");
		return;
	}

	char * buffer = new char[NUMOF];
	DataType * bufferout = new DataType[NUMOF];
	while (!fin.eof())
	{
		fin.read(buffer, NUMOF);
		//返回最近一次读取的字节数
		int curNum = fin.gcount();//只用于字节流的读写
		for (int i = 0; i < curNum; ++i)
		{
			bufferout[i] = ecrept((DataType)buffer[i], _key._ekey, _key._pkey);

		}
		fout.write((char *)bufferout, curNum * sizeof(DataType));//最大是NUMOOF
	}
	fin.close();
	fout.close();
	delete[] buffer;
	delete[] bufferout;
}
void RSA::decrept(const char * filein, const char * fileout)
{
	ifstream fin(filein, ifstream::binary);
	ofstream fout(fileout, ofstream::binary);
	if (!fin.is_open())
	{
		perror("open filein error");
		return;
	}
	DataType * buffer = new DataType[NUMOF];
	char * bufferout = new char[NUMOF];
	//int * bufferout = new int[NUMOF];

	while (!fin.eof())
	{
		fin.read((char*)buffer, NUMOF * sizeof(DataType));//以二进制的方式读取，一个字节转换成字符的ASCII码
		int num = fin.gcount();
		num /= sizeof(DataType);
		for (int i=0;i<num;++i)
		{
			bufferout[i] = (char)decrept(buffer[i], _key._dkey, _key._pkey);
			//此处为什么精度截断警告也没关系呢，因为加密的时候是一个字节加密的，解密返回的原信息
			//肯定不会超过一个字节
		}
		fout.write((char *)bufferout, num);
	}
	delete[] buffer;
	delete[] bufferout;
	fin.close();
	fout.clear();
}