#include <cpuid.h>
#include "cpuid.h"

namespace z
{
	namespace system
	{
		void cpuid::det_vendor()
		{
			uint32_t eax, ebx, ecx, edx;

			good = __get_cpuid(0, &eax, &ebx, &ecx, &edx);
			/*asm volatile
			(
			 "cpuid" :
			 "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3]) :
			 "a" (cmd), "c" (0)
			 );*/
				// ECX is set to zero for CPUID function 4

			if (good)
			{
				char vendor[13];
				vendor[12] = 0;

				*((uint32_t*)&vendor[0]) = ebx;
				*((uint32_t*)&vendor[4]) = edx;
				*((uint32_t*)&vendor[8]) = ecx;

				_vendor = vendor;
			}
		}

		void cpuid::det_cpu()
		{
			uint32_t eax, ebx, ecx, edx;

			__get_cpuid(1, &eax, &ebx, &ecx, &edx);

			_cpus = (ebx >> 16) & 0xff;
			_cpuFeatures = edx;

			if (_vendor == "GenuineIntel")
			{
				__get_cpuid(4, &eax, &ebx, &ecx, &edx);
				_cores = ((eax >> 26) & 0x3f) + 1;
			}
			else if (_vendor == "AuthenticAMD")
			{
				__get_cpuid(0x80000008, &eax, &ebx, &ecx, &edx);
				_cores = (ecx & 0xff) + 1;
			}
			else
			{
				_cores = _cpus;
			}

			_allow_smt = _cpuFeatures & (1 << 28) && _cores < _cpus;
		}

		cpuid::cpuid()
		{
			det_vendor(); //'good' will be determined after vendorID

			if (good)
			{
				det_cpu();
			}
		}

		const core::string<ascii>& cpuid::vendor()
		{
			return _vendor;
		}

		int cpuid::cpus()
		{
			return _cpus;
		}

		int cpuid::cores()
		{
			return _cores;
		}
	}
}
