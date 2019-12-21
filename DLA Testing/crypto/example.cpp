#include <iostream>

#include "dll.h"
#include "cryptopp_wrapper.h"

using namespace std;

int main()
{
	cout<<"Space Wrapper TEST!"<<endl<<endl;
	SpaceCrypto::CryptBlowFish hello;
	hello.setPlainString("HelloWorld!");
	hello.setKey("mySecUreKey!!");
	std::string crypt;
	crypt = hello.Encrypt();
	cout<<"Plain Text: HelloWorld!"<<endl;
	cout<<"Crypt Text: "<<crypt<<endl;
	hello.reset();
	hello.setEncString(crypt);
	hello.setInputMode(SpaceCrypto::HEX);
	hello.setKey("mySecUreKey!!");
	hello.setOutputMode(SpaceCrypto::NORMAL);
	cout<<"Decry Text: "<<hello.Decrypt()<<endl;
}