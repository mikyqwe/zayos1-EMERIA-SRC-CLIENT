#pragma once
#ifndef __INCLUDE_FORMAT_IKEX_HEADER__
#define __INCLUDE_FORMAT_IKEX_HEADER__

#include "std.h"

namespace ikex {
	inline std::string string_value(const std::string& v) { return v; }
	inline std::string string_value(const std::wstring& v) {
		if (v.empty())
				return "";

		const auto leng = v.length() * 2;
		std::unique_ptr<char[]> buffer(new char[leng]);

		wcstombs(buffer.get(), v.c_str(), leng - 1);
		buffer.get()[leng - 1] = 0;

		return std::string(buffer.get());
	}

	template <class T>
	std::string string_value(const T& v) { return std::to_string(v); }

	inline std::string format(const char* _fmt) { return _fmt; }

	template <class... S, class T>
	std::string format(const char* _fmt, const T& _val, S... _args);

	template <class... S>
	std::string format(const char* _fmt, const char* _val, S... _args) {
		auto next = _fmt;
		while (next) {
			next = std::strchr(next, '{');
			if (next && next[1] == '}') {
				return std::string(_fmt, static_cast<size_t>(next - _fmt)) +
					std::string(_val) + format(next + 2, _args...);
			}
			else {
				next++;
			}
		}

		return format(_fmt);
	}

	template <class... S>
	std::string format(const char* _fmt, char* _val, S... _args) {
		auto next = _fmt;
		while (next) {
			next = std::strchr(next, '{');
			if (next && next[1] == '}') {
				return std::string(_fmt, static_cast<size_t>(next - _fmt)) +
					std::string(_val) + format(next + 2, _args...);
			}
			else {
				next++;
			}
		}

		return format(_fmt);
	}

	template <class... S, class T>
	std::string format(const char* _fmt, const T& _val, S... _args) {
		auto next = _fmt;
		while (next) {
			next = std::strchr(next, '{');
			if (next && next[1] == '}') {
				return std::string(_fmt, static_cast<size_t>(next - _fmt)) +
					string_value(_val) + format(next + 2, _args...);
			} else {
				next++;
			}
		} return format(_fmt);
	}

	inline size_t snformat(char* _buffer, const size_t _buffer_len, const char* _fmt) {
		const auto fmtlen = strlen(_fmt);
		const auto allowed_copy_size =
			fmtlen > _buffer_len - 1 ? _buffer_len - 1 : fmtlen;
		std::strncpy(_buffer, _fmt, allowed_copy_size);
		return allowed_copy_size;
	}

	template <class... S, class T>
	size_t snformat(char* _buffer, const size_t _buffer_len, const char* _fmt,
		const T& _val, S... _args) {
		auto next = _fmt;
		while (next) {
			next = strchr(next, '{');
			if (next && next[1] == '}') {
				const size_t jump_len = next - _fmt;
				std::memcpy(_buffer, _fmt, jump_len);
				const auto st = std::to_string(_val);
				const auto st_len = st.length();
				const size_t remain_size = _buffer_len - jump_len;
				const size_t allowed_copy_size =
					remain_size - 1 > st_len ? st_len : remain_size - 1;
				std::memcpy(_buffer + jump_len, st.c_str(), allowed_copy_size);
				const size_t write_len = allowed_copy_size + jump_len;
				return write_len + snformat(_buffer + write_len, _buffer_len - write_len,
					next + 2, _args...);
			}
			else {
				next++;
			}
		}

		return snformat(_buffer, _buffer_len, _fmt);
	}

	template <class... S>
	size_t snformat(char* _buffer, const size_t _buffer_len, const char* fmt,
		const char* val, S... args) {
		auto next = fmt;
		while (next) {
			next = strchr(next, '{');
			if (next && next[1] == '}') {
				const size_t jump_len = next - fmt;
				std::memcpy(_buffer, fmt, jump_len);
				const auto st_len = strlen(val);
				const size_t remain_size = _buffer_len - jump_len;
				const size_t allowed_copy_size =
					remain_size - 1 > st_len ? st_len : remain_size;
				std::memcpy(_buffer + jump_len, val, allowed_copy_size);
				const size_t write_len = allowed_copy_size + jump_len;
				return write_len + snformat(_buffer + write_len, _buffer_len - write_len,
					next + 2, args...);
			}
			else {
				next++;
			}
		}

		return snformat(_buffer, _buffer_len, fmt);
	}

	template <class... S>
	size_t snformat(char* _buffer, const size_t _buffer_len, const char* fmt,
		const std::string& val, S... args) {
		auto next = fmt;
		while (next) {
			next = strchr(next, '{');
			if (next && next[1] == '}') {
				const size_t jump_len = next - fmt;
				std::memcpy(_buffer, fmt, jump_len);
				const auto st_len = val.length();
				const size_t remain_size = _buffer_len - jump_len;
				const size_t allowed_copy_size =
					remain_size - 1 > st_len ? st_len : remain_size;
				std::memcpy(_buffer + jump_len, val.c_str(), allowed_copy_size);
				const size_t write_len = allowed_copy_size + jump_len;
				return write_len + snformat(_buffer + write_len, _buffer_len - write_len,
					next + 2, args...);
			}
			else {
				next++;
			}
		}

		return snformat(_buffer, _buffer_len, fmt);
	}


	template <class ...S>
	size_t fprintf(std::ofstream& _file, const char* _fmt, S... args) {
		auto st = format(_fmt, args...);
		_file.write(st.c_str(), st.length());
		return st.length();
	}

	template <class ...S>
	size_t fprintf(std::fstream& _file, const char* _fmt, S... args) {
		auto st = format(_fmt, args...);
		_file.write(st.c_str(), st.length());
		return st.length();
	}

	template <class ...S>
	size_t fprintf(std::FILE* _file, const char* _fmt, S... args) {
		if (_file) {
			auto st = format(_fmt, args...);
			std::fwrite(st.c_str(), sizeof(char), st.length(), _file);
			return st.length();
		}return 0;
	}

	template <class ...S>
	size_t fprintf(std::fstream& _file, const std::string& out) {
		_file.write(out.c_str(), out.length());
		return out.length();
	}
	
	template <class ...S>
	size_t fprintf(std::ofstream& _file, const std::string& out) {
		_file.write(out.c_str(), out.length());
		return out.length();
	}

	template <class ...S>
	size_t fprintf(std::FILE* _file, const std::string& out) {
		if (_file) {
			std::fwrite(out.c_str(), sizeof(char), out.length(), _file);
			return out.length();
		}return 0;
	}


}

#endif //__INCLUDE_FORMAT_IKEX_HEADER__