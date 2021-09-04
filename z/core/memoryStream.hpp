#pragma once

#include "stream.hpp"

namespace z
{
	namespace core
	{
		/**
		* \brief A class for streaming directly to and from memory.
		*
		* This class provides an implementation of core::stream for raw data in memory.
		*/
		class memoryStream : public stream
		{
		private:
			uint8_t* data;
			size_t dataSize;
			size_t streamIndex;
			encoding streamFormat;
			bool initialized;

		public:
			/**
			* \brief Generic constructor.
			*
			* Optionally specify the section of memory that this stream may access.
			*
			* \param streamData The first data element this stream may access.
			* \param count The total number of elements this stream may access.
			*/
			template<typename T>
			memoryStream(T* streamData = 0, int count = 1)
			{
				if (count < 0) count = 1;
				data = (uint8_t*)streamData;
				dataSize = data ? (sizeof(T)*count) : 0;
				streamIndex = 0;
				streamFormat = ascii;
				initialized = false;
			}

			/**
			* \brief Set the section of memory that this stream may access.
			*
			* \param streamData The first data element this stream may access.
			* \param count The total number of elements this stream may access.
			*/
			template<typename T>
			void set(T* streamData, int count = 1)
			{
				data = (uint8_t*)streamData;
				dataSize = count;
			}

			void put(uint8_t ch) override;
			void put(uint8_t* str, int count, encoding format = ascii) override;

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

			encoding format() override;
			void setFormat(encoding enc, bool force = false) override;

			void flush() override;
		};
	}
}
