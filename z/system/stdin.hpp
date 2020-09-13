#pragma once

#include "../encoding.hpp"
#include "../core/stream.hpp"

#ifdef _WIN32
#undef stdin
#endif

namespace z
{
	namespace system
	{
		/**
		 * \brief Class for getting input from stdin.
		 *
		 * This class provides an implementation of core::inputStream.
		 * All read-related methods read from stdin. Note that
		 * because of how stdin behaves, you cannot
		 * unget characters or seek a position in the stream;
		 * those are just dummy methods.
		 * As a result, this class is not compatible with the
		 * util::regex class.
		 */
		class stdin : public core::inputStream
		{
		public:
			uint8_t get() override;
			uint8_t peek() override;
			uint32_t getChar() override;

			bool empty() override;
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
		};
	}
}
