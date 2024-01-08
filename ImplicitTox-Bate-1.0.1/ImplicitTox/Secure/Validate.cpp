#include <filesystem>
#include <iostream>
#include <fstream>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>
#include "Validate.h"
#include "Device.h"
#include "Secure.h"
#include "NTP.h"

namespace secure
{
	namespace validate
	{
		class LicenseValidate::LicenseValidateImpl
		{
		public:
			static void Validate();
		private:
			static void WriteRegistrationToFile(const std::string& registration);
			LicenseValidateImpl() = delete;
			~LicenseValidateImpl() = delete;
			LicenseValidateImpl(const LicenseValidateImpl&) = delete;
			LicenseValidateImpl& operator = (const LicenseValidateImpl&) = delete;
		};

		void LicenseValidate::LicenseValidateImpl::Validate()
		{

			std::string sourceCode;
			try
			{
				sourceCode = secure::Secure::EncryptStringByHash(
					device::GetDeviceID::GetCpuID() + device::GetDeviceID::GetBiosID() + "Powered by DLUT",
					secure::Secure::BLAKE2b, 64);
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				std::exit(-1);
			}

			std::filesystem::path licenseFile = std::filesystem::current_path() / "License" / "license";
			if (!std::filesystem::exists(licenseFile))
			{
				std::cout << "* Sorry, you do not have access to the software, please copy the (.\\License\\registration) file to administrator for access. *" << std::endl;
				WriteRegistrationToFile(sourceCode);
				std::cout << "Press any key to exit...";
				std::cin.ignore();
				std::exit(0);
			}

			std::ifstream file(licenseFile);
			std::string deviceCode;
			try
			{
				deviceCode = secure::Secure::DecryptStringFromeFileByComplex(file);
			}
			catch (const std::exception& e)
			{
				std::cout << "* Sorry, license mismatch, please copy the (.\\License\\registration) file to administrator for access. *" << std::endl;
				WriteRegistrationToFile(sourceCode);
				std::cout << "Press any key to exit...";
				std::cin.ignore();
				std::exit(0);
			}
			if (deviceCode != sourceCode)
			{
				std::cout << "* Sorry, license mismatch, please copy the (.\\License\\registration) file to administrator for access. *" << std::endl;
				WriteRegistrationToFile(sourceCode);
				std::cout << "Press any key to exit...";
				std::cin.ignore();
				std::exit(0);
			}

			std::string validityDateStr;
			try
			{
				validityDateStr = secure::Secure::DecryptStringFromeFileByComplex(file);
			}
			catch (const std::exception& e)
			{
				std::cout << "* Sorry, license mismatch, please copy the (.\\License\\registration) file to administrator for access. *" << std::endl;
				WriteRegistrationToFile(sourceCode);
				std::cout << "Press any key to exit...";
				std::cin.ignore();
				std::exit(0);
			}
			file.close();

			int timeZonedifferential = 65535;
			Poco::DateTime validityDate = Poco::DateTimeParser::parse(Poco::DateTimeFormat::HTTP_FORMAT, validityDateStr, timeZonedifferential);
			Poco::DateTime currentDate;
			try
			{
				currentDate = ntp::NTP::GetUtcTime();
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				std::exit(-1);
			}
			if (validityDate < currentDate)
			{
				std::cout << "* Sorry, your license has expired in {" +
					Poco::DateTimeFormatter::format(validityDate, Poco::DateTimeFormat::HTTP_FORMAT) +
					"}, to continue your subscription, please copy the (.\\License\\registration) file to administrator for access. *" << std::endl;
				WriteRegistrationToFile(sourceCode);
				std::cout << "Press any key to exit...";
				std::cin.ignore();
				std::exit(0);
			}
		}

		void LicenseValidate::LicenseValidateImpl::WriteRegistrationToFile(const std::string& registration)
		{
			std::filesystem::path filePath = std::filesystem::current_path() / "License";
			if (!std::filesystem::exists(filePath))
				std::filesystem::create_directories(filePath);

			std::ofstream file(filePath / "registration", std::ios::binary);
			file << secure::Secure::EncryptStringByBase64(
				secure::Secure::EncryptStringByAES(registration, "68D117d73402C034", "68D117d73402C034"));
			file.close();
		}

		void LicenseValidate::Validate()
		{
			LicenseValidateImpl::Validate();
		}
	}
}
