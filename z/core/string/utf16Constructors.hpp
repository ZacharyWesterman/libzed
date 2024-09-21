#pragma once

namespace z {
namespace core {
template <>
string<utf16>::string() noexcept {
	data = new uint8_t[2];
	*((uint16_t *)data) = 0;

	data_len = 2;
	character_ct = 0;
}

template <>
string<utf16>::string(char chr) noexcept {
	data = new uint8_t[4];
	uint16_t *data16 = (uint16_t *)data;

	data16[0] = chr;
	data16[1] = 0;

	data_len = 4;
	character_ct = 1;
}

template <>
string<utf16>::string(wchar_t chr) noexcept {
	data = new uint8_t[4];
	uint16_t *data16 = (uint16_t *)data;

#ifdef _WIN32
	data16[0] = chr; // In Windows, wchar_t is 16 bits
#else
	data16[0] = (chr > 0xFFFF) ? '?' : chr;
#endif
	data16[1] = 0;

	data_len = 4;
	character_ct = 1;
}

template <>
string<utf16>::string(const uint32_t &chr) noexcept {
	data = new uint8_t[4];
	uint16_t *data16 = (uint16_t *)data;

	data16[0] = (chr > 0xFFFF) ? '?' : chr;
	data16[1] = 0;

	data_len = 4;
	character_ct = 1;
}

template <>
string<utf16>::string(const char *str) noexcept {
	if (str) {
		int len = 0;
		while (str[len]) {
			len++;
		}

		character_ct = len;
		data = new uint8_t[(++len) << 1];
		data_len = len << 1;

		uint16_t *data16 = (uint16_t *)data;

		for (int i = 0; i < len; i++) {
			data16[i] = str[i];
		}
	} else {
		data = new uint8_t[2];
		*((uint16_t *)data) = 0;

		data_len = 2;
		character_ct = 0;
	}
}

template <>
string<utf16>::string(const char *str, size_t len) noexcept {
	if (str) {
		character_ct = len;
		data = new uint8_t[(len + 1) << 1];
		data_len = (len + 1) << 1;

		uint16_t *data16 = (uint16_t *)data;

		for (size_t i = 0; i < len; i++) {
			data16[i] = str[i];
		}

		data16[len] = '\0';
	} else {
		data = new uint8_t[2];
		*((uint16_t *)data) = 0;

		data_len = 2;
		character_ct = 0;
	}
}

template <>
string<utf16>::string(const wchar_t *str) noexcept {
	if (str) {
		int len = 0;
		while (str[len]) {
			len++;
		}

		character_ct = len;
		data = new uint8_t[(++len) << 1];
		data_len = len << 1;

		uint16_t *data16 = (uint16_t *)data;

		for (int i = 0; i < len; i++) {
#ifdef _WIN32
			data16[i] = str[i]; // In Windows, wchar_t is 16 bits
#else
			data16[i] = (str[i] > 0xFFFF) ? '?' : str[i];
#endif
		}
	} else {
		data = new uint8_t[2];
		*((uint16_t *)data) = 0;

		data_len = 2;
		character_ct = 0;
	}
}

template <>
string<utf16>::string(const wchar_t *str, size_t len) noexcept {
	if (str) {
		character_ct = len;
		data = new uint8_t[(len + 1) << 1];
		data_len = (len + 1) << 1;

		uint16_t *data16 = (uint16_t *)data;

		for (size_t i = 0; i < len; i++) {
#ifdef _WIN32
			data16[i] = str[i]; // In Windows, wchar_t is 16 bits
#else
			data16[i] = (str[i] > 0xFFFF) ? '?' : str[i];
#endif
		}

		data16[len] = '\0';
	} else {
		data = new uint8_t[2];
		*((uint16_t *)data) = 0;

		data_len = 2;
		character_ct = 0;
	}
}

template <>
string<utf16>::string(const string<ascii> &other) noexcept {
	data_len = (other.character_ct + 1) << 1;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];
	uint16_t *data16 = (uint16_t *)data;

	for (int i = 0; i <= character_ct; i++) {
		data16[i] = other.data[i];
	}
}

template <>
string<utf16>::string(const string<utf8> &other) noexcept {
	character_ct = 0;
	for (int i = 0; i < other.character_ct; i += lenFromUTF8(&other.data[i])) {
		character_ct++;
	}

	data_len = (character_ct + 1) << 1;
	data = new uint8_t[data_len];

	uint16_t *data16 = (uint16_t *)data;
	data16[character_ct] = 0;

	int pos = 0;
	for (int i = 0; i <= character_ct; i++) {
		uint32_t val = fromUTF8(&other.data[pos]);
		pos += lenFromUTF8(&other.data[pos]);

		data16[i] = (val > 0xFFFF) ? '?' : val;
	}
}

template <>
string<utf16>::string(const string<utf16> &other) noexcept {
	data_len = (other.character_ct + 1) << 1;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];

	uint16_t *data16 = (uint16_t *)data;
	uint16_t *other16 = (uint16_t *)other.data;

	for (int i = 0; i <= character_ct; i++) {
		data16[i] = other16[i];
	}
}

template <>
string<utf16>::string(const string<utf32> &other) noexcept {
	data_len = (other.character_ct + 1) << 1;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];

	uint16_t *data16 = (uint16_t *)data;
	uint32_t *data32 = (uint32_t *)other.data;

	for (int i = 0; i <= character_ct; i++) {
		data16[i] = (data32[i] > 0xFFFF) ? '?' : data32[i];
	}
}
} // namespace core
} // namespace z
