#pragma once

#include <z/core/string.h>

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

			core::string<ascii> _vendor;
			int _cpus;
			int _cores;
			int _threads;
			uint32_t _cpuFeatures;
			bool _allow_smt;

			void det_vendor();
			void det_cpu();

		public:
			/**
			 * \brief Constructor
			 */
			cpuid();

			/**
			 * \brief Get the CPU vendor.
			 *
			 * \return A string containing the vendor CPU id.
			 *
			 * \threadsafe_member_yes
			 */
			const core::string<ascii>& vendor();

			/**
			 * \brief Get the number of CPUs.
			 *
			 * \return The number of CPUs on this machine.
			 *
			 * \threadsafe_member_yes
			 */
			int cpus();

			/**
			 * \brief Get the number of cores.
			 *
			 * \return The number of individual cores on this machine.
			 *
			 * \threadsafe_member_yes
			 */
			int cores();
		};
	}
}
