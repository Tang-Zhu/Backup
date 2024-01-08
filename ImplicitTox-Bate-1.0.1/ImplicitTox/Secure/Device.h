#pragma once
#include <string>

namespace secure
{
	namespace device
	{
		class GetDeviceID
		{
		public:
			static std::string GetCpuID();
			static std::string GetBiosID();
		private:
			class GetDeviceIDImpl;
			GetDeviceID() = delete;
			~GetDeviceID() = delete;
			GetDeviceID(const GetDeviceID&) = delete;
			GetDeviceID& operator = (const GetDeviceID&) = delete;
		};
	}
}

