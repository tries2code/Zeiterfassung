#include "TEA.hpp"


TEA::TEA(){
	//TEA-Key von xml-Datei laden. (Vor Release in Quellcode verschieben?)
	xml_reader = new XML("Resources/crypt_key.xml");
	str_key = xml_reader->get_XML_field("key");
    delete xml_reader;

	while (str_key.size() < kchar)str_key += '0';	//füllt key mit 0en auf
	int_key = reinterpret_cast<const unsigned int*>(str_key.data());
}

std::string TEA::decrypt(std::stringstream& str_input) {

	std::string str_result = "";

	char output_buffer[nchar + 1];
	output_buffer[nchar] = 0;        // terminator
	unsigned int* out_ptr = reinterpret_cast<unsigned int*>(output_buffer);
	unsigned int in_ptr[2];

	str_input.setf(std::ios_base::hex, std::ios_base::basefield); // hex input

	while(str_input >> in_ptr[0] >> in_ptr[1]){
		decipher(in_ptr, out_ptr);
		str_result = str_result + output_buffer;
	}
	while(str_result.find_last_of('0')== str_result.length()-1)str_result.pop_back();
	return str_result;
}

std::string TEA::encrypt(std::stringstream& str_input) {

	std::stringstream str_result;
	str_result << std::hex;
	
	char input_buffer[nchar];
	unsigned int* in_ptr = reinterpret_cast<unsigned int*>(input_buffer);
	unsigned int out_ptr[2];
	int count = 0;

	while(str_input.get(input_buffer[count])){
		if (++count == nchar) {
			encipher(in_ptr, out_ptr);
			count = 0;
			str_result << std::setw(8) << std::setfill('0') << out_ptr[0] << ' ' << std::setw(8) << std::setfill('0') << out_ptr[1] << ' ';
		}
	}
	if(count){
		while (count != nchar)input_buffer[count++] = '0';
		encipher(in_ptr, out_ptr);
		str_result << out_ptr[0] << ' ' << out_ptr[1] << ' ';
	}
	return str_result.str();
}




void TEA::encipher(const unsigned int* const v, unsigned int* const w) {

	unsigned int y = v[0];
	unsigned int z = v[1];
	unsigned int sum = 0;
	const unsigned int delta = 0x9E3779B9;

	for (unsigned int n = 32; n-- > 0;) {
		y += (z << 4 ^ z >> 5) + z ^ sum + int_key[sum&3];
		sum += delta;
		z += (y << 4 ^ y >> 5) + y ^ sum + int_key[sum >> 11 & 3];
	}
	w[0] = y;
	w[1] = z;
}
void TEA::decipher(const unsigned int* const v, unsigned int* const w) {
	
	unsigned int y = v[0];
	unsigned int z = v[1];
	unsigned int sum = 0xC6EF3720;
	const unsigned int delta = 0x9E3779B9;

	for (unsigned int n = 32; n-- > 0;) {
		z -= (y << 4 ^ y >> 5) + y ^ sum + int_key[sum >> 11 & 3];
		sum -= delta;
		y -= (z << 4 ^ z >> 5) + z ^ sum + int_key[sum & 3];
	}
	w[0] = y;
	w[1] = z;
}





