#pragma once


namespace ikaruspack {
	inline std::vector<std::string> utils_get_split(const std::string& s, const std::string& tok) {
		std::vector<std::string> v;
		size_t pos = 0, prev_pos = 0;
		while ((pos = s.find(tok, prev_pos)) != std::string::npos) {
			v.emplace_back(s.substr(prev_pos, pos - prev_pos));
			prev_pos = pos + 1;
		} if (prev_pos < s.size()) { v.emplace_back(s.substr(prev_pos)); }
		return v;
	}

	inline std::vector<std::wstring> utils_get_split(const std::wstring& s, const std::wstring& tok) {
		std::vector<std::wstring> v;
		size_t pos = 0, prev_pos = 0;
		while ((pos = s.find(tok, prev_pos)) != std::string::npos) {
			v.emplace_back(s.substr(prev_pos, pos - prev_pos));
			prev_pos = pos + 1;
		} if (prev_pos < s.size()) { v.emplace_back(s.substr(prev_pos)); }
		return v;
	}

	inline std::wstring utils_tolower(std::wstring in) {
		std::transform(in.begin(), in.end(), in.begin(), ::towlower);
		return in;
	}

	inline std::string utils_tolower(std::string in) {
		std::transform(in.begin(), in.end(), in.begin(), ::tolower);
		return in;
	}

	inline std::wstring utils_concat_path(const std::wstring& path, const std::wstring& file) {
		if (path.empty()) return file;
		if (path.back() == L'\\' || path.back() == L'/') return path + file;
		else return path + L'\\' + file;
	}
	
	inline std::string utils_concat_path(const std::string& path, const std::string& file) {
		if (path.empty()) return file;
		if (path.back() == '\\' || path.back() == '/') return path + file;
		else return path + '\\' + file;			
	}

	inline archive::hash32 utils_get_hash32(const std::string& in_s) {
		auto checked = utils_tolower(in_s);
		std::replace(checked.begin(), checked.end(), '/', '\\');
		return archive::hash32(std::hash<std::string>()(checked));
	}

	inline archive::hash32 utils_get_hash32(const std::wstring& in_s) {
		auto checked = utils_tolower(in_s);
		std::replace(checked.begin(), checked.end(), L'/', L'\\');
		return archive::hash32(std::hash<std::string>()(utils_to_string(checked)));
	}

	bool utils_get_buffer_file(std::wstring dir, dynamic_buffer& buff);
	void utils_easy_decrypt_ppl(dynamic_buffer& buff, const archive::key& key, const int _block_size);
	void utils_easy_encrypt_ppl(dynamic_buffer& buff, const archive::key& key, const int _block_size);
	void utils_easy_decrypt(dynamic_buffer& buff, const archive::key& key);
	void utils_easy_encrypt(dynamic_buffer& buff, const archive::key& key);
	bool utils_easy_compress(dynamic_buffer& buff, lzo_align_t* workmemin_ = nullptr);
	bool utils_easy_decompress(dynamic_buffer& buff, const size_t in_len, const size_t out_len);
	void utils_reverse_buffer_by_magic_ppl(dynamic_buffer& buff, archive::magic magic, const int blocksize);
	void utils_reverse_buffer_by_magic(dynamic_buffer& buff, archive::magic magic);
	std::string utils_get_cryptopp_key_from_key(const ikaruspack::__Key& key);
	std::string utils_get_iv_by_magic(const ikaruspack::mapped_archive::magic _magic);
	void utils_cryptopp_encrypt_ppl(dynamic_buffer& buff, ikaruspack::__Key& __key, const ikaruspack::mapped_archive::magic _magic, const int block_size);
	void utils_cryptopp_encrypt(dynamic_buffer& buff, ikaruspack::__Key& __key, const ikaruspack::mapped_archive::magic _magic);
	void utils_cryptopp_decrypt_ppl(dynamic_buffer& buff, ikaruspack::__Key& __key, const ikaruspack::mapped_archive::magic _magic, const int block_size);
	void utils_cryptopp_decrypt(dynamic_buffer& buff, ikaruspack::__Key& __key, const ikaruspack::mapped_archive::magic _magic);
	
	inline std::string utils_to_string(const std::wstring& wstr)
	{
		using convert_t = std::codecvt_utf8<wchar_t>;
		static std::wstring_convert<convert_t, wchar_t> strconverter;
		return strconverter.to_bytes(wstr);
	}

	inline std::wstring utils_to_wstring(const std::string& str)
	{
		using convert_t = std::codecvt_utf8<wchar_t>;
		static std::wstring_convert<convert_t, wchar_t> strconverter;
		return strconverter.from_bytes(str);
	}

	char utils_confuse1_byte(char in_);
	char utils_confuse2_byte(char in_);
	char utils_confuse3_byte(char in_);
	char utils_confuse4_byte(char in_);

	inline bool utils_directory_exists(const std::wstring& folder) {
		uint32_t attribs = ::GetFileAttributesW(folder.c_str());
		return attribs != INVALID_FILE_ATTRIBUTES && (attribs & FILE_ATTRIBUTE_DIRECTORY);
	}

	inline bool utils_directory_exists(const std::string& folder) {
		uint32_t attribs = ::GetFileAttributesA(folder.c_str());
		return attribs != INVALID_FILE_ATTRIBUTES && (attribs & FILE_ATTRIBUTE_DIRECTORY);
	}

	inline bool utils_create_dirs(std::wstring path) {
		const auto colon = path.find(':');
		const auto disk  = colon != std::wstring::npos ? path.substr(0, colon + 2) : L"";

		auto split = utils_get_split(path.c_str() + disk.length(), L"\\");
		auto wrkp = disk.substr(0, disk.find_first_of(L"\\/"));
		const std::wstring slash = L"\\";

		for (const auto& s : split) {
			wrkp += slash + s;
			if (utils_directory_exists(wrkp))
				continue;
			if (!::CreateDirectoryW(wrkp.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
				return false;
		}return true;
	}

	inline bool utils_create_dirs(std::string path) {
		const auto colon = path.find(':');
		const std::string disk = colon != std::string::npos ? path.substr(0, colon+2) : "";

		auto split = utils_get_split(path.c_str() + disk.length() , "\\");
		auto wrkp = disk.substr(0, disk.find_first_of("\\/"));
		const std::string slash = "\\";

		for (const auto& s : split) {
			wrkp += slash + s;
			if (utils_directory_exists(wrkp))
				continue;
			if (!::CreateDirectoryA(wrkp.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS){
				std::printf("cannot create dir %s ", wrkp.c_str());
				return false;
			}
		}return true;
	}

	inline bool utils_create_dirs_for_file(std::string filename) {
		size_t pos = filename.find_last_of("/\\");
		if (pos == std::string::npos)
			return true;
		std::string path = filename.substr(0, pos);
		return utils_create_dirs(path);
	}

	inline bool utils_create_dirs_for_file(std::wstring filename) {
		size_t pos = filename.find_last_of(L"/\\");
		if (pos == std::wstring::npos)
			return true;
		std::wstring path = filename.substr(0, pos);
		return utils_create_dirs(path);
	}

	template <class T>
	void utils_get_filelist(std::wstring folder, T& out, bool recursivity=true)
	{
		std::replace(folder.begin() , folder.end(), '/' , '\\');
		
		if(folder!=L"*" && folder.back() != '\\')
			folder+=L'\\';

		WIN32_FIND_DATA ffd; HANDLE hFind = FindFirstFile((folder+L'*').c_str(), &ffd);
		if (INVALID_HANDLE_VALUE == hFind)
			return;

		std::vector<std::wstring> pathlist;
		
		do{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if (recursivity) {
					std::wstring subpath(ffd.cFileName);
					if (subpath == L"." || subpath == L"..")
						continue;

					if (folder != L"*")
						subpath = folder + subpath;

					std::replace(subpath.begin(), subpath.end(), '/', '\\');
					pathlist.push_back(subpath);
				}
			}
			else{
				std::wstring stfilename(ffd.cFileName);
				if(folder!=L"*")
					stfilename = folder+stfilename;

				std::replace(stfilename.begin(), stfilename.end(), '/', '\\');
				out.push_back(stfilename);
			}
		}while (FindNextFile(hFind, &ffd) != 0);
		FindClose(hFind);

		if(recursivity)
			for(auto& path : pathlist)
				utils_get_filelist(path , out, recursivity);
	}
	   
	inline void* utils_malloc(size_t size) {
#ifdef __USE_DEBUG_ALLOC__
		auto result = malloc(size);
		std::cout << "allocating " << size << " bytes, ptr " << result << std::endl;
		return result;
#else
		return (void*)new char[size];
#endif
	}
	   	 
	inline void utils_free(void* ptr) {
#ifdef __USE_DEBUG_ALLOC__
		std::cout << "free on ptr " << ptr <<std::endl;
#endif
		delete[] ptr;
	}
	   
	inline void* utils_realloc(void* ptr, const size_t size, const size_t oldsize) {
#ifdef __USE_DEBUG_ALLOC__
		auto result = realloc(ptr, size);
		std::cout << "reallocating " << ptr << " for new size " << size << " bytes, ptr " << result << std::endl;
		return result;
#else
		auto* nptr = utils_malloc(size);
		std::memcpy(nptr, ptr, oldsize);
		delete(ptr);
		return nptr;
#endif
	}
	
	inline void utils_memcpy(void* dest, const void* src, const size_t len) {
#ifdef __USE_DEBUG_ALLOC__
		std::cout << "memcpy to " << dest << " from " << src << " ( leng " << len << ")" << std::endl;
#endif
		memcpy(dest, src, len);
	}

	template <class T>
	void utils_delete(T* ptr) {
#ifdef __USE_DEBUG_ALLOC__
		std::cout << "deleting pointer " <<ptr<<std::endl;
#endif
		delete(ptr);
	}

	template <class T>
	T* utils_new() {
#ifdef __USE_DEBUG_ALLOC__
		auto res = new T();
		std::cout << "new pointer " <<res<<std::endl;
		return res;
#else
		return new T();
#endif
	}

	template <class T, class Ptr>
	Ptr utils_decode_from_buffer(Ptr memptr, T* objptr) {
		utils_memcpy(objptr, memptr, sizeof(T));
		return (Ptr) ((char*)memptr + sizeof(T));
	}

	template <class Ptr>
	Ptr utils_decode_from_buffer(Ptr memptr, void* dest, size_t size) {
		utils_memcpy(dest, memptr, size);
		return (Ptr)((char*)memptr + size);
	}

	template <class T>
	void utils_test_me(T func, long times =1) {
		double tot_time = 0;
		for(long i=0; i<times; i++){
			const auto time_start = std::chrono::system_clock::now();
			func();
			const auto time_end = std::chrono::system_clock::now();
			tot_time += std::chrono::duration<double>(time_end - time_start).count();
		}

		if(tot_time > 1) std::cout << "\ntime : " << tot_time << "s" << std::endl;
		else std::cout << "\ntime : " << tot_time * 1000.0 << "ms" << std::endl;
	}

	template <class type>
	type utils_get_random(const type min_, const type max_) {
		const auto diff = (max_ - min_);
		return diff != 0? min_ + (std::rand()%diff) : min_;
	}

	template <class ...S>
	void utils_testline(S... args) {
		const auto line = ikex::format(args...) + '\n';
		std::printf(line.c_str());
		static bool is_open = false;
		static std::ofstream file;
		if (!is_open) { file.open("test.txt"); 
						is_open = true; }
		if (file.is_open()) file.write(line.c_str(), line.length());
	}
}