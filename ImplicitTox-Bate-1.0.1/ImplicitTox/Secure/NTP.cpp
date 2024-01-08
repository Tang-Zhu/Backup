#include <string>
#include <vector>
#include <Poco/Net/NTPClient.h>
#include <Poco/Net/NTPEventArgs.h>
#include <Poco/Delegate.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>
#include "NTP.h"

namespace secure
{
	namespace ntp
	{
		class NTP::NTPImpl
		{
		public:
			static Poco::DateTime GetUtcTime();
		private:
			static constexpr int Timeout = 3000000;
			int timeZoneDifferential = 65535;
			void OnResponse(const void* pSender, Poco::Net::NTPEventArgs& args);
			Poco::DateTime UTCTime;
			NTPImpl() = default;
			~NTPImpl() = default;
			NTPImpl(const NTPImpl&) = delete;
			NTPImpl& operator = (const NTPImpl&) = delete;
		};

		Poco::DateTime NTP::NTPImpl::GetUtcTime()
		{
			std::vector<std::string> servers = {
				"ntp.ntsc.ac.cn",
				"cn.ntp.org.cn",
				"ntp1.nim.ac.cn",
				"ntp2.nim.ac.cn",
				"cn.pool.ntp.org",
				"ntp.aliyun.com",
				"ntp1.aliyun.com",
				"ntp2.aliyun.com",
				"ntp3.aliyun.com",
				"ntp4.aliyun.com",
				"ntp5.aliyun.com",
				"ntp6.aliyun.com",
				"ntp7.aliyun.com",
				"ntp.tencent.com",
				"ntp1.tencent.com",
				"ntp2.tencent.com",
				"ntp3.tencent.com",
				"ntp4.tencent.com",
				"ntp5.tencent.com",
				"edu.ntp.org.cn",
				"ntp.sjtu.edu.cn",
				"ntp.neu.edu.cn",
				"ntp.bupt.edu.cn",
				"ntp.shu.edu.cn",
				"pool.ntp.org",
				"0.pool.ntp.org",
				"1.pool.ntp.org",
				"2.pool.ntp.org",
				"3.pool.ntp.org",
				"asia.pool.ntp.org",
				"time1.google.com",
				"time2.google.com",
				"time3.google.com",
				"time4.google.com",
				"time.apple.com",
				"time1.apple.com",
				"time2.apple.com",
				"time3.apple.com",
				"time4.apple.com",
				"time5.apple.com",
				"time6.apple.com",
				"time.windows.com"
			};

			NTPImpl instance;
			bool connected = false;
			for (int n = 0; n < servers.size(); ++n)
			{
				Poco::Net::NTPClient ntpClient(Poco::Net::AddressFamily::IPv4, Timeout);
				ntpClient.response += Poco::Delegate<NTPImpl, Poco::Net::NTPEventArgs>(&instance, &NTPImpl::OnResponse);
				try
				{
					connected = (ntpClient.request(servers[n]) == 1 ? true : false);
				}
				catch (const std::exception&)
				{
					continue;
				}
				ntpClient.response -= Poco::Delegate<NTPImpl, Poco::Net::NTPEventArgs>(&instance, &NTPImpl::OnResponse);
				if (connected)
					break;

			}
			if (!connected)
				throw ("[Error] All services timeout, please check your network for authorization!");
			return instance.UTCTime;
		}

		void NTP::NTPImpl::OnResponse(const void* pSender, Poco::Net::NTPEventArgs& args)
		{
			UTCTime = Poco::DateTimeParser::parse(
				Poco::DateTimeFormat::HTTP_FORMAT,
				Poco::DateTimeFormatter::format(args.packet().receiveTime(),
					Poco::DateTimeFormat::HTTP_FORMAT, timeZoneDifferential),
				timeZoneDifferential);
		}

		Poco::DateTime NTP::GetUtcTime()
		{
			return NTPImpl::GetUtcTime();
		}
	}
}
