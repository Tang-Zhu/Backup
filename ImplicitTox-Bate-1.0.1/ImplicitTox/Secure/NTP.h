#pragma once
#include <Poco/DateTime.h>

namespace secure
{
	namespace ntp
	{
		class NTP
		{
		public:
			static Poco::DateTime GetUtcTime();
		private:
			class NTPImpl;
			NTP() = delete;
			~NTP() = delete;
			NTP(const NTP&) = delete;
			NTP& operator = (const NTP&) = delete;
		};
	}
}


