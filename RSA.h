#pragma once
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/random.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
//typedef long DataType;

typedef boost::multiprecision::int1024_t DataType;
namespace brdm = boost::random;
#define NUMOF 256
struct Key
{
	//(e,n) (d,n)
	DataType _ekey;
	DataType _dkey;
	DataType _pkey;
};
//1���������� p,q
//2����n��n=p*q
//3����f(n) = f(p)*f(q)=(p-1)*(q-1)
//4����e 1 < e <f(n),e��f(n) ���ɻ��ʣ����Լ��Ϊ1�������ѡ��e
//5����d  e*d %f(n) = 1
// (e,n)��Կ (d,n)˽Կ
class RSA
{
private:
	Key _key;
public:
	RSA();
	void getKeys();
	DataType getPrime();
	bool isPrimeBigInt(DataType data);
	DataType getPkey(DataType prime1, DataType prime2);
	DataType getOrla(DataType prime1, DataType prime2);
	DataType getEkey(DataType orla);
	DataType getDkey(DataType ekey, DataType orla);
	DataType getGcd(DataType data1, DataType data2);
	bool isPrime(DataType data);
	DataType ecrept(DataType data, DataType ekey, DataType pkey);//���� m=c^e % modn
	DataType decrept(DataType data, DataType dkey, DataType pkey);//���� c=m^d % modn
	Key getallKey();
	void ecrept(const char * filein, const char * fileout);
	void decrept(const char * filein, const char * fileout);
	DataType exGcd(DataType a, DataType b, DataType &x, DataType &y);

};