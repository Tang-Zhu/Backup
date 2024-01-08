#include "Device.h"
#include <Windows.h>
#include <tchar.h>

namespace secure
{
	namespace device
	{
		class GetDeviceID::GetDeviceIDImpl
		{
		public:
			static std::string GetCpuID();
			static std::string GetBiosID();
		private:
			GetDeviceIDImpl() = delete;
			~GetDeviceIDImpl() = delete;
			GetDeviceIDImpl(const GetDeviceIDImpl&) = delete;
			GetDeviceIDImpl& operator = (const GetDeviceIDImpl&) = delete;
		};

		std::string GetDeviceID::GetDeviceIDImpl::GetCpuID()
		{
			const long MAX_COMMAND_SIZE = 10000;
			LPSTR szFetCmd = const_cast<LPSTR>("wmic cpu get processorid");
			const std::string strEnSearch = "ProcessorId";


			BOOL   bret = FALSE;
			HANDLE hReadPipe = NULL;
			HANDLE hWritePipe = NULL;
			PROCESS_INFORMATION pi;
			STARTUPINFO         si;
			SECURITY_ATTRIBUTES sa;

			char            szBuffer[MAX_COMMAND_SIZE + 1] = { 0 };
			std::string          strBuffer;
			unsigned long   count = 0;
			long            ipos = 0;

			memset(&pi, 0, sizeof(pi));
			memset(&si, 0, sizeof(si));
			memset(&sa, 0, sizeof(sa));

			pi.hProcess = NULL;
			pi.hThread = NULL;
			si.cb = sizeof(STARTUPINFO);
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = TRUE;

			bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
			if (!bret)
			{
				CloseHandle(hWritePipe);
				CloseHandle(hReadPipe);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				return "";
			}

			GetStartupInfo(&si);
			si.hStdError = hWritePipe;
			si.hStdOutput = hWritePipe;
			si.wShowWindow = SW_HIDE;
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

			bret = CreateProcess(NULL, _T(szFetCmd), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
			if (!bret)
			{
				CloseHandle(hWritePipe);
				CloseHandle(hReadPipe);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				return "";
			}

			WaitForSingleObject(pi.hProcess, 500);
			bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
			if (!bret)
			{
				CloseHandle(hWritePipe);
				CloseHandle(hReadPipe);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				return "";
			}

			bret = FALSE;
			strBuffer = szBuffer;
			ipos = strBuffer.find(strEnSearch);

			if (ipos < 0)
			{
				CloseHandle(hWritePipe);
				CloseHandle(hReadPipe);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				return "";
			}
			else
			{
				strBuffer = strBuffer.substr(ipos + strEnSearch.length());
			}

			memset(szBuffer, 0x00, sizeof(szBuffer));
			strcpy_s(szBuffer, strBuffer.c_str());

			std::string CPUID;
			for (size_t i = 0; i < strBuffer.size(); i++)
			{
				if (strBuffer[i] != ' ' && strBuffer[i] != '\n' && strBuffer[i] != '\r')
					CPUID += strBuffer[i];
			}

			bret = TRUE;

			CloseHandle(hWritePipe);
			CloseHandle(hReadPipe);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			return CPUID;
		}
		std::string GetDeviceID::GetDeviceIDImpl::GetBiosID()
		{
			const long MAX_COMMAND_SIZE = 10000;

			LPSTR szFetCmd = const_cast<LPSTR>("wmic csproduct get UUID");
			const std::string strEnSearch = "UUID";

			BOOL   bret = FALSE;
			HANDLE hReadPipe = NULL;
			HANDLE hWritePipe = NULL;
			PROCESS_INFORMATION pi;
			memset(&pi, 0, sizeof(pi));
			STARTUPINFO	si;
			memset(&si, 0, sizeof(si));
			SECURITY_ATTRIBUTES sa;
			memset(&sa, 0, sizeof(sa));

			char szBuffer[MAX_COMMAND_SIZE + 1] = { 0 };
			std::string	strBuffer;
			unsigned long count = 0;
			long ipos = 0;

			pi.hProcess = NULL;
			pi.hThread = NULL;
			si.cb = sizeof(STARTUPINFO);
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = TRUE;

			bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
			if (!bret) {
				CloseHandle(hWritePipe);
				CloseHandle(hReadPipe);

				return "";
			}

			GetStartupInfo(&si);
			si.hStdError = hWritePipe;
			si.hStdOutput = hWritePipe;
			si.wShowWindow = SW_HIDE;
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

			bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
			if (!bret) {
				CloseHandle(hWritePipe);
				CloseHandle(hReadPipe);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				return "";
			}

			WaitForSingleObject(pi.hProcess, 200);
			bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
			if (!bret) {
				CloseHandle(hWritePipe);
				CloseHandle(hReadPipe);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				return "";
			}

			bret = FALSE;
			strBuffer = szBuffer;
			ipos = strBuffer.find(strEnSearch);

			if (ipos < 0) {
				CloseHandle(hWritePipe);
				CloseHandle(hReadPipe);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				return "";
			}
			else {
				strBuffer = strBuffer.substr(ipos + strEnSearch.length());
			}

			memset(szBuffer, 0x00, sizeof(szBuffer));
			strcpy_s(szBuffer, strBuffer.c_str());

			std::string biosID;

			for (int i = 0; i < strlen(szBuffer); i++) {
				if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r')
					biosID += szBuffer[i];
			}

			CloseHandle(hWritePipe);
			CloseHandle(hReadPipe);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			return biosID;
		}

		std::string GetDeviceID::GetCpuID()
		{
			std::string cpuID = GetDeviceIDImpl::GetCpuID();
			if (cpuID == "")
				throw("[Error] Cannot get CPU ID!");
			return cpuID;
		}

		std::string GetDeviceID::GetBiosID()
		{
			std::string biosID = GetDeviceIDImpl::GetBiosID();
			if (biosID == "")
				throw("[Error] Cannot get Bios ID!");
			return biosID;
		}
	}
}

