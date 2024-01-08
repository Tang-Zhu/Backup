#include <cryptopp/aes.h>
#include <cryptopp/base64.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/blake2.h>
#include <cryptopp/hex.h>
#include <iostream>
#include <random>
#include "Secure.h"

namespace secure
{
	class Secure::SecureImpl
	{
	public:
		static std::string EncryptStringByAES(const std::string& code, const std::string& key, const std::string& iv);
		static std::string DecryptStringByAES(const std::string& code, const std::string& key, const std::string& iv);
		static std::string EncryptStringByHash(const std::string& code, HashTypes type, const unsigned int length);
		static std::string EncryptStringByBase64(const std::string& code);
		static std::string DecryptStringByBase64(const std::string& code);
		static std::string EncryptNumberToAlphabet(const int num);
		static int DecryptNumberFromAlphabet(const std::string& num);
		static std::string GenerateRandomString(int length, StringTypes type);
		static std::vector<int> GenerateRandomNumbers(int num, int start, int end, NumberTypes type);
		static void EncryptStringToFileByComplex(std::ofstream& file, const std::string& code);
		static std::string DecryptStringFromFileByComplex(std::ifstream& file);
		static void GenerateStringConfusion(std::ofstream& file, const int start, const int end);
		static void SkipStringConfusion(std::ifstream& file);
	private:
		static std::string EncryptHexString(std::ofstream& file, int length);
		static std::string DecryptStringBySize(std::ifstream& file);
		static void WriteEncodedStringToFile(std::ofstream& file, const std::string& code, int length);
		
		
		SecureImpl() = delete;
		~SecureImpl() = delete;
		SecureImpl(const Secure&) = delete;
		SecureImpl& operator = (const SecureImpl&) = delete;
	};

	std::string Secure::SecureImpl::EncryptStringByAES(const std::string& code, const std::string& key, const std::string& iv)
	{
		std::string byteCode;
		try
		{
			CryptoPP::CBC_Mode< CryptoPP::AES >::Encryption e;
			e.SetKeyWithIV((const CryptoPP::byte*)key.c_str(), key.size(), (const CryptoPP::byte*)iv.c_str());

			CryptoPP::StringSource s(code, true,
				new CryptoPP::StreamTransformationFilter(e,
					new CryptoPP::StringSink(byteCode)
				) 
			); 
		}
		catch (const CryptoPP::Exception& e) 
		{
			throw("[Error] AES encryption error: " + std::string(e.what()));
		}

		return byteCode;
	}

	std::string Secure::SecureImpl::DecryptStringByAES(const std::string& code, const std::string& key, const std::string& iv)
	{
		std::string recovered;
		try
		{
			CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption d;
			d.SetKeyWithIV((const CryptoPP::byte*)key.c_str(), key.size(), (const CryptoPP::byte*)iv.c_str());

			CryptoPP::StringSource s(code, true,
				new CryptoPP::StreamTransformationFilter(d,
					new CryptoPP::StringSink(recovered)
				)
			); 
		}
		catch (const CryptoPP::Exception& e)
		{
			throw("[Error] AES decryption error: " + std::string(e.what()));
		}

		return recovered;
	}

	std::string Secure::SecureImpl::EncryptStringByHash(const std::string& code, HashTypes type, const unsigned int length)
	{
		std::string signature;

		try 
		{
			if (type == BLAKE2b)
				{
					CryptoPP::BLAKE2b hash(false, length);
					CryptoPP::StringSource(code, true,
						new CryptoPP::HashFilter(hash,
							new CryptoPP::HexEncoder(
								new CryptoPP::StringSink(signature))));
				}
		}
		catch (const CryptoPP::Exception& e) {
			throw("[Error] Eecrypt string error: " + std::string(e.what()));
		}
		return signature;
	}

	std::string Secure::SecureImpl::EncryptStringByBase64(const std::string& code)
	{
		std::string encoded;
		CryptoPP::StringSource(code, true,
			new CryptoPP::Base64Encoder(
				new CryptoPP::StringSink(encoded)
			)
		);

		encoded.erase(std::remove_if(encoded.begin(), encoded.end(), [](unsigned char c) { return std::isspace(c); }),
			encoded.end());

		return encoded;
	}

	std::string Secure::SecureImpl::DecryptStringByBase64(const std::string& code)
	{
		std::string decoded;
		CryptoPP::StringSource(code, true, new CryptoPP::Base64Decoder(
			new CryptoPP::StringSink(decoded)
		));
		return decoded;
	}

	std::string Secure::SecureImpl::EncryptNumberToAlphabet(const int num)
	{
		std::string s = std::to_string(num);
		std::string res;
		for(auto c: s)
			res += (c + 16);
		return res;
	}

	int Secure::SecureImpl::DecryptNumberFromAlphabet(const std::string& num)
	{
		std::string s;
		for (auto c : num)
			s += (c - 16);
		return std::stoi(s);
	}

	std::string Secure::SecureImpl::GenerateRandomString(int length, StringTypes type)
	{
		std::string charset;
		switch (type)
		{
		case(StringTypes::ONLY_ALPHABET):
			charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
			break;
		case(StringTypes::ONLY_LOWERCASE):
			charset = "abcdefghijklmnopqrstuvwxyz";
			break;
		case(StringTypes::ONLY_UPPERCASE):
			charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			break;
		case(StringTypes::ONLY_NUMBER):
			charset = "0123456789";
			break;
		case(StringTypes::ONLY_SYMBOL):
			charset = "-+=@#$%";
			break;
		case(StringTypes::HEX):
			charset = "abcdefABCDEF0123456789";
			break;
		default:
		case(StringTypes::COMPLEX_CHAR):
			charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-+=@#$%&";
		}
		
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<int> distribution(0, charset.length() - 1);

		std::string randomString;
		for (int i = 0; i < length; ++i) {
			randomString += charset[distribution(generator)];
		}

		return randomString;
	}

	std::vector<int> Secure::SecureImpl::GenerateRandomNumbers(int num, int start, int end, NumberTypes type)
	{
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<int> distribution(start , end);
		std::vector<int> randomNumbers;
		switch (type)
		{
		case(NumberTypes::ONLY_SINGULAR):
			while (randomNumbers.size() < num)
			{
				int randomNumber = distribution(generator);
				if (randomNumber % 2 != 0)
					randomNumbers.push_back(randomNumber);
			}
			break;
		case(NumberTypes::ONLY_EVEN):
			while (randomNumbers.size() < num)
			{
				int randomNumber = distribution(generator);
				if (randomNumber % 2 == 0)
					randomNumbers.push_back(randomNumber);
			}
			break;
		default:
		case(NumberTypes::COMPLEX_NUM):
			while (randomNumbers.size() < num)
				randomNumbers.push_back(distribution(generator));
		}
		
		return randomNumbers;
	}

	void Secure::SecureImpl::EncryptStringToFileByComplex(std::ofstream& file, const std::string& code)
	{
		GenerateStringConfusion(file, 1000, 2000);
		//std::string offset = GenerateRandomString(3, ONLY_UPPERCASE);
		const int KeyIVLength[3] = { 16, 24, 32 };
		std::vector<int> length = GenerateRandomNumbers(2, 16, 32, COMPLEX_NUM);
		std::string key = EncryptHexString(file, KeyIVLength[length[0] % 3]);
		//file << offset[0];
		//file << GenerateRandomString(int(offset[0]), COMPLEX_CHAR);
		GenerateStringConfusion(file, 30, 40);
		std::string iv =  EncryptHexString(file, KeyIVLength[length[1] % 3]);
		//file << offset[1];
		//file << GenerateRandomString(int(offset[1]), COMPLEX_CHAR);
		GenerateStringConfusion(file, 10, 20);
		std::string encodedCode;
			encodedCode = EncryptStringByBase64(
				EncryptStringByAES(code, key, iv));

		WriteEncodedStringToFile(file, encodedCode, encodedCode.size());
		//file << offset[2];
		//file << GenerateRandomString(int(offset[2]), COMPLEX_CHAR);
		GenerateStringConfusion(file, 5, 15);
		encodedCode = EncryptStringByBase64(EncryptStringByHash(encodedCode, BLAKE2b, 64));
		WriteEncodedStringToFile(file, encodedCode, encodedCode.size());
		GenerateStringConfusion(file, 800, 1600);
	}

	std::string Secure::SecureImpl::DecryptStringFromFileByComplex(std::ifstream& file)
	{
		SkipStringConfusion(file);
		std::string key = DecryptStringBySize(file);
		//char offset;
		//file >> offset;
		//file.seekg(int(offset), std::ios::cur);
		SkipStringConfusion(file);
		std::string iv = DecryptStringBySize(file);
		//file >> offset;
		//file.seekg(int(offset), std::ios::cur);
		SkipStringConfusion(file);
		std::string code = DecryptStringBySize(file);
		//file >> offset;
		//file.seekg(int(offset), std::ios::cur);
		SkipStringConfusion(file);
		std::string signature = DecryptStringBySize(file);
		if (DecryptStringByBase64(signature) != EncryptStringByHash(code, BLAKE2b, 64))
			throw("[Error] Code miss match!");
		SkipStringConfusion(file);

		return DecryptStringByAES(
			DecryptStringByBase64(code), key, iv);
		
	}

	std::string Secure::SecureImpl::EncryptHexString(std::ofstream& file, int length)
	{
		std::string code = GenerateRandomString(length, StringTypes::HEX);
		WriteEncodedStringToFile(file, code, length);
		return code;
	}

	void Secure::SecureImpl::WriteEncodedStringToFile(std::ofstream& file, const std::string& code, int length)
	{
		file << (EncryptNumberToAlphabet(length) + char(38));
		file << code;
	}

	std::string Secure::SecureImpl::DecryptStringBySize(std::ifstream& file)
	{
		std::string dataSizeStr;
		for (char byte; file.read(&byte, 1);)
		{
			if (byte == '&')
				break;
			dataSizeStr += byte;
		}
		if (file.eof())
			throw("[Error] Code miss match!");
		int dataSize = DecryptNumberFromAlphabet(dataSizeStr);
		std::string res(dataSize, '\0');
		file.read(res.data(), dataSize);
		return res;
	}

	void Secure::SecureImpl::GenerateStringConfusion(std::ofstream& file, const int start, const int end)
	{
		std::vector<int> offset = secure::Secure::GenerateRandomNumbers(1, start, end);
		file << (EncryptNumberToAlphabet(offset[0]) + char(38));
		file << GenerateRandomString(offset[0], COMPLEX_CHAR);
	}

	void Secure::SecureImpl::SkipStringConfusion(std::ifstream& file)
	{
		std::string dataSizeStr;
		for (char byte; file.read(&byte, 1);)
		{
			if (byte == '&')
				break;
			dataSizeStr += byte;
		}
		if (file.eof())
			throw("[Error] Code miss match!");
		int dataSize = DecryptNumberFromAlphabet(dataSizeStr);
		file.seekg(dataSize, std::ios::cur);
	}

	std::string Secure::EncryptStringByAES(const std::string& code, const std::string& key, const std::string& iv)
	{
		return SecureImpl::EncryptStringByAES(code, key, iv);
	}
	std::string Secure::DecryptStringByAES(const std::string& code, const std::string& key, const std::string& iv)
	{
		return SecureImpl::DecryptStringByAES(code, key, iv);
	}

	std::string Secure::EncryptStringByHash(const std::string& code, HashTypes type, const unsigned int length)
	{
		return SecureImpl::EncryptStringByHash(code, type, length);
	}

	std::string Secure::EncryptStringByBase64(const std::string& code)
	{
		return SecureImpl::EncryptStringByBase64(code);
	}

	std::string Secure::DecryptStringByBase64(const std::string& code)
	{
		return SecureImpl::DecryptStringByBase64(code);
	}

	std::string Secure::EncryptNumberToAlphabet(const int num)
	{
		return SecureImpl::EncryptNumberToAlphabet(num);
	}

	int Secure::DecryptNumberFromAlphabet(const std::string& num)
	{
		return SecureImpl::DecryptNumberFromAlphabet(num);
	}

	std::string Secure::GenerateRandomString(int length, StringTypes type)
	{
		return SecureImpl::GenerateRandomString(length, type);
	}

	std::vector<int> Secure::GenerateRandomNumbers(int num, int start, int end, NumberTypes type)
	{
		return SecureImpl::GenerateRandomNumbers(num, start, end, type);
	}

	void Secure::EncryptStringToFileByComplex(std::ofstream& file, const std::string& code)
	{
		SecureImpl::EncryptStringToFileByComplex(file, code);
	}

	std::string Secure::DecryptStringFromeFileByComplex(std::ifstream& file)
	{
		return SecureImpl::DecryptStringFromFileByComplex(file);
	}

	void Secure::GenerateStringConfusion(std::ofstream& file, const int start, const int end)
	{
		SecureImpl::GenerateStringConfusion(file, start, end);
	}

	void Secure::SkipStringConfusion(std::ifstream& file)
	{
		SecureImpl::SkipStringConfusion(file);
	}

}
