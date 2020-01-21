#pragma once

#include <z/encoding.hpp>
#include <z/core/stream.hpp>

namespace z
{
	namespace system
	{
		/**
		 * \brief Class for sending output to stdout.
		 *
		 * This class provides an implementation of core::outputStream.
		 * All write-related methods write to stdout. Note that
		 * because of how stdout behaves, you cannot
		 * seek a position in the stream; that is just a dummy method.
		 * As a result, this class is not compatible with the
		 * util::regex class.
		 */
		class stdout : public core::outputStream
		{
		public:
			void put(uint8_t ch) override;
			void put(uint8_t* str, size_t count, encoding format = ascii) override;

			bool good() override;
			bool bad() override;
			bool binary() override;
			bool seekable() override;

			void seek(size_t index) override;
			size_t tell() override;
			size_t end() override;

			/**
			 * \brief Get the expected format of the stream.
			 *
			 * Analyzes a portion of the stream in an attempt to determine how
			 * Characters on it are formatted. Note that this only gives an assumption of
			 * the most likely encoding so it may not be correct in some cases.
			 * Additionally results will be inaccurate if the stream contains
			 * raw binary data.
			 *
			 * \return utf8, the default formatting of the console.
			 *
			 * \threadsafe_member_yes
			 */
			encoding format() override;

			void setFormat(encoding enc, bool force = false) override;

			void flush() override;

			bool istty() const override;
		};
	}
}
