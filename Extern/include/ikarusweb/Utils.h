#pragma once
namespace ikarusweb {

	inline const double utils_getTimeAsDouble(){
		return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	inline bool utils_extractContentLength(DynamicBuffer::BufferPtr data, DynamicBuffer::BufferSize size, long long* outLength, DWORD* outHeaderSize, bool* useNoEndSocket) {
		//* relevant constants
		static constexpr char ContentHeader[] = "Content-Length:";
		static constexpr char EndHeader[]	  = "\r\n\r\n";
		
		if(size ==0)
			return false;


		//* calculating header string
		auto headerptr = (const char*) data;
		auto headerlen = size;

		if(headerlen > DynamicBuffer::DynamicInitSize)
			headerlen = DynamicBuffer::DynamicInitSize;

		//* allocating header string
		std::string header(headerptr, headerlen);

		//* searching the length
		auto lpos = header.find(ContentHeader);
		//* searching the end of the header
		auto eheader = header.find(EndHeader);

		//* check if exists
		if (lpos == std::string::npos ) {
			*useNoEndSocket = eheader != std::string::npos;
			return false;
		}

		
		

		//* check if exists
		if (eheader == std::string::npos) {
			return false;
		}

		//* upgrading position to skip the 'search value'
		lpos    += strlen(ContentHeader);
		eheader += strlen(EndHeader);

		//* search the end of the line
		auto lend = header.find( '\n' , lpos);

		//* checking if exist and if yes decrease to skip end line character
		if (lend == std::string::npos || lend-- >= eheader) {
			return false;
		}

		else {
			*outLength = std::stoll( header.substr(lpos, lend-lpos), NULL, 10) + eheader;
			*outHeaderSize = (DWORD) eheader;
			return true;
		}
	}

	inline bool utils_IsEndBufferMessage(const void* data, const size_t leng) {
		static constexpr char EndHeader[] = "\r\n\r\n";
		std::string finder((char*) data, leng);
		size_t first = 0;
		return (first = finder.find(EndHeader)) != std::string::npos && first != finder.length() - 1 && finder.find(EndHeader, first+1) != std::string::npos;

	}

	inline bool utils_checkInternetConnection() {
		return InternetCheckConnectionW(L"http://www.google.com",FLAG_ICC_FORCE_CONNECTION,0) || GetLastError() == 0;
	}

	inline bool utils_checkInternetConnectionTo(std::wstring url) {
		return InternetCheckConnectionW(url.c_str(), FLAG_ICC_FORCE_CONNECTION, 0) || GetLastError() == 0;
	}

	inline bool utils_checkInternetConnectionTo(std::string url) {
		return InternetCheckConnectionA(url.c_str(), FLAG_ICC_FORCE_CONNECTION, 0) || GetLastError() == 0;
	}
}