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
	//	prime = rand() % 100 + 2;//��Ҫ0 1
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
	//��n
	return prime1 * prime2;
}
DataType RSA::getOrla(DataType prime1, DataType prime2)
{
	//��f(n)
	return (prime1 - 1)*(prime2 - 1);
}
DataType RSA::getEkey(DataType orla)
{
	//��������Կ1< e < f(n) e ��f(n)����
	//srand(time(NULL));
	brdm::mt19937 gen(time(NULL));
	brdm::uniform_int_distribution<DataType> dist(2, orla);

	DataType ekey;
	while (true)
	{
		cout << "get ekey" << endl;
		ekey = dist(gen);//��֤С��f(n)
		if (ekey>1 && (getGcd(ekey,orla)==1))//�������������Լ��Ϊ1
		{
			cout << "get ekey finsh" << endl;
			return ekey;
		}
	}

}
DataType RSA::getDkey(DataType ekey, DataType orla)
{
	//������Կ e*d%f(n) =1��ģ��Ԫ��	
	DataType x = 0, y = 0;
	cout << "get dkey" << endl;
	exGcd(ekey, orla, x, y);//�β������ã��ڵ��ú�����ֱ�Ӹı�ֵ
	return (x%orla + orla) % orla;//����ɽ�С����ֵ,��Ҫ���ڸ���
	//����ͬ�ඨ��(x%orla + orla) % orla = x%orla%orla,


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
	//�����Լ�����������������Լ��Ϊ1
	//ʹ��շת�����
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
DataType RSA::ecrept(DataType data, DataType ekey, DataType pkey)//���� m=d^e % modp
{
	//i :0....n
	DataType Ai = data;// data^(b0*2^0),��Ϊbi=1��ʱ��ż��㣬bi=0��ʱ�򲻼��㣬����Ai�ĳ�ʼֵΪdata
	DataType msgE = 1;//��һ�����ĳ�ʼ��Ϊ1����ΪA0=A0û��˭�����˸�msgEΪ1
	//data ^ ekey %pkey
	while (ekey)//�������λ
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
DataType RSA::decrept(DataType data, DataType dkey, DataType pkey)//���� c=m^d % modn
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
		//�������һ�ζ�ȡ���ֽ���
		int curNum = fin.gcount();//ֻ�����ֽ����Ķ�д
		for (int i = 0; i < curNum; ++i)
		{
			bufferout[i] = ecrept((DataType)buffer[i], _key._ekey, _key._pkey);

		}
		fout.write((char *)bufferout, curNum * sizeof(DataType));//�����NUMOOF
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
		fin.read((char*)buffer, NUMOF * sizeof(DataType));//�Զ����Ƶķ�ʽ��ȡ��һ���ֽ�ת�����ַ���ASCII��
		int num = fin.gcount();
		num /= sizeof(DataType);
		for (int i=0;i<num;++i)
		{
			bufferout[i] = (char)decrept(buffer[i], _key._dkey, _key._pkey);
			//�˴�Ϊʲô���ȽضϾ���Ҳû��ϵ�أ���Ϊ���ܵ�ʱ����һ���ֽڼ��ܵģ����ܷ��ص�ԭ��Ϣ
			//�϶����ᳬ��һ���ֽ�
		}
		fout.write((char *)bufferout, num);
	}
	delete[] buffer;
	delete[] bufferout;
	fin.close();
	fout.clear();
}