#pragma once

#include "../core/string.hpp"

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

			void det_vendor() noexcept;
			void det_cpu() noexcept;

		public:
			/**
			* \brief Constructor
			*/
			cpuid() noexcept;

			/**
			* \brief Get the CPU vendor.
			*
			* \return A string containing the vendor CPU id.
			*
			* \threadsafe_member_yes
			*/
			const core::string<ascii>& vendor() noexcept;

			/**
			* \brief Get the number of CPUs.
			*
			* \return The number of CPUs on this machine.
			*
			* \threadsafe_member_yes
			*/
			int cpus() noexcept;

			/**
			* \brief Get the number of cores.
			*
			* \return The number of individual cores on this machine.
			*
			* \threadsafe_member_yes
			*/
			int cores() noexcept;
		};
	}
}
