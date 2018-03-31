#pragma once
#ifndef CPUID_H_INCLUDED
#define CPUID_H_INCLUDED

#include <z/core/string.h>

#include <cpuid.h>

namespace z
{
    namespace system
    {
		/**
		 * \brief A class for easily getting CPU info.
		 */
        class cpuid
        {
        private:
            bool good;

            core::string<char> _vendor;
            int _cpus;
            int _cores;
            int _threads;
            uint32_t _cpuFeatures;
            bool _allow_smt;

            void det_vendor();
            void det_cpu();

        public:
            cpuid();

            const core::string<char>& vendor();

            int cpus();
            int cores();
        };

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
                vendor[12] = null;

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

		/**
		 * \brief Constructor
		 */
        cpuid::cpuid()
        {
            det_vendor(); //'good' will be determined after vendorID

            if (good)
            {
                det_cpu();
            }
        }

		/**
		 * \brief Get the CPU vendor.
		 *
		 * \return A string containing the vendor CPU id.
		 */
        const core::string<char>& cpuid::vendor()
        {
            return _vendor;
        }

		/**
		 * \brief Get the number of CPUs.
		 *
		 * \return The number of CPUs on this machine.
		 */
        int cpuid::cpus()
        {
            return _cpus;
        }

		/**
		 * \brief Get the number of cores.
		 *
		 * \return The number of individual cores on this machine.
		 */
        int cpuid::cores()
        {
            return _cores;
        }
    }
}

#endif // CPUID_H_INCLUDED
