#include "TEA.hpp"


TEA::TEA(){
	//TEA-Key von xml-Datei laden. (Vor Release in Quellcode verschieben?)
	xml_reader = new XML("Resources/crypt_key.xml");
	str_key = xml_reader->get_XML_field("key");
    delete xml_reader;

	while (str_key.size() < kchar)str_key += '0';	//füllt key mit 0en auf
	int_key = reinterpret_cast<const unsigned int*>(str_key.data());
}

std::string TEA::decrypt(std::stringstream& sstr_in){

	std::string str_output = "";

	char output_buffer[nchar + 1];
	output_buffer[nchar] = 0;        // terminator
	unsigned int* out_ptr = reinterpret_cast<unsigned int*>(output_buffer);
	unsigned int in_ptr[2];

	sstr_in.setf(std::ios_base::hex, std::ios_base::basefield); // hex input

	while(sstr_in >> in_ptr[0] >> in_ptr[1]){
		decipher(in_ptr, out_ptr);
		str_output = str_output + output_buffer;
	}
	
	str_output = str_output.substr(0,str_output.find("#~;EOS:~#"));
	return str_output;
}

std::string TEA::encrypt(std::stringstream& sstr_in){

	std::stringstream sstr_input,sstr_output;
	sstr_input << sstr_in.str() << "#~;EOS:~#";
	sstr_output << std::hex;
	
	char input_buffer[nchar];
	unsigned int* in_ptr = reinterpret_cast<unsigned int*>(input_buffer);
	unsigned int out_ptr[2];
	int count = 0;

	while(sstr_input.get(input_buffer[count])){
		if (++count == nchar) {
			encipher(in_ptr, out_ptr);
			count = 0;
			sstr_output << std::setw(8) << std::setfill('0') << out_ptr[0] << ' ' << std::setw(8) << std::setfill('0') << out_ptr[1] << ' ';
		}
	}
	if(count){
		while (count != nchar)input_buffer[count++] = '0';
		encipher(in_ptr, out_ptr);
		sstr_output << out_ptr[0] << ' ' << out_ptr[1] << ' ';
	}
	return sstr_output.str();
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





