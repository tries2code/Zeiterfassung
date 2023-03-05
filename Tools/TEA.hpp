#include<iomanip>
#include<string>

#include "XML.hpp"

class TEA{
	private:
		const int nchar = 2 * sizeof(int);			//64 bits
		const int kchar = 2 * nchar;				//128bits
		const unsigned int* int_key;
		
		std::string str_key = "";	
		XML* xml_reader;

	public:
		TEA();
		std::string decrypt(std::stringstream& sstr_in);
		std::string encrypt(std::stringstream& sstr_in);

	private:
		void encipher(const unsigned int* const v, unsigned int* const w);
		void decipher(const unsigned int* const v, unsigned int* const w);

	
 
};