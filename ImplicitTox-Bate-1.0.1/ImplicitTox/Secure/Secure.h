#pragma once
#include <string>
#include <fstream>

namespace secure
{
	class Secure
	{
	public:
		enum HashTypes
		{
			SHA128,
			SHA256,
			BLAKE2b
		};
		enum StringTypes
		{
			ONLY_ALPHABET,
			ONLY_LOWERCASE,
			ONLY_UPPERCASE,
			ONLY_NUMBER,
			ONLY_SYMBOL,
			HEX,
			COMPLEX_CHAR
		};
		enum NumberTypes
		{
			ONLY_SINGULAR,
			ONLY_EVEN,
			COMPLEX_NUM
		};
		static std::string EncryptStringByAES(const std::string& code, const std::string& key, const std::string& iv);
		static std::string DecryptStringByAES(const std::string& code, const std::string& key, const std::string& iv);
		static std::string EncryptStringByHash(const std::string& code, HashTypes type = HashTypes::BLAKE2b, const unsigned int length = 64);
		static std::string EncryptStringByBase64(const std::string& code);
		static std::string DecryptStringByBase64(const std::string& code);
		static std::string EncryptNumberToAlphabet(const int num);
		static int DecryptNumberFromAlphabet(const std::string& num);
		static std::string GenerateRandomString(int length, StringTypes type = StringTypes::COMPLEX_CHAR);
		static std::vector<int> GenerateRandomNumbers(int num, int start, int end, NumberTypes type = NumberTypes::COMPLEX_NUM);
		static void EncryptStringToFileByComplex(std::ofstream& file, const std::string& code);
		static std::string DecryptStringFromeFileByComplex(std::ifstream& file);
		static void GenerateStringConfusion(std::ofstream& file, const int start, const int end);
		static void SkipStringConfusion(std::ifstream& file);
	private:
		class SecureImpl;
		Secure() = delete;
		~Secure() = delete;
		Secure(const Secure&) = delete;
		Secure& operator = (const Secure&) = delete;
	};
}

