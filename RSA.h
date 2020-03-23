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
//1、产生素数 p,q
//2、求n，n=p*q
//3、求f(n) = f(p)*f(q)=(p-1)*(q-1)
//4、求e 1 < e <f(n),e和f(n) 构成互质（最大公约数为1），随机选择e
//5、求d  e*d %f(n) = 1
// (e,n)公钥 (d,n)私钥
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
	DataType ecrept(DataType data, DataType ekey, DataType pkey);//加密 m=c^e % modn
	DataType decrept(DataType data, DataType dkey, DataType pkey);//解密 c=m^d % modn
	Key getallKey();
	void ecrept(const char * filein, const char * fileout);
	void decrept(const char * filein, const char * fileout);
	DataType exGcd(DataType a, DataType b, DataType &x, DataType &y);

};