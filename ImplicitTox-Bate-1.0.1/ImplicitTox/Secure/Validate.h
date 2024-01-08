#pragma once

namespace secure
{
	namespace validate
	{
		class LicenseValidate
		{
		public:
			static void Validate();
		private:
			class LicenseValidateImpl;
			LicenseValidate() = delete;
			~LicenseValidate() = delete;
			LicenseValidate(const LicenseValidate&) = delete;
			LicenseValidate& operator = (const LicenseValidate&) = delete;
		};
	}
}

