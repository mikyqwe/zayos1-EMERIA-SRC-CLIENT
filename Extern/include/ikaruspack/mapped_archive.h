#pragma once

namespace ikaruspack {
	class mapped_archive {

	public:
		static const uint64_t	filename_max_len = 256;

	public:


		typedef __Key			key;
		typedef uint32_t		hash32;
		typedef uint64_t		offset;
		typedef uint64_t		size;
		typedef uint8_t			magic;

		typedef struct {
			wchar_t			fname[filename_max_len];
			offset			foffset;
			size			fsize;
			hash32			fhash;
			magic			fmagic;
			size			funcompressed;
		}file_info;

		typedef struct {
			std::wstring filename;
			std::wstring realfilename;
		}insert_file_info_tw;

		typedef struct {
			std::string filename;
			std::string realfilename;
		}insert_file_info_t;

#ifdef __USE_SHORTER_DICTFILE_SIZE__
#pragma pack(push)
#pragma pack(1)
		typedef struct {
			offset			foffset;
			size			fsize;
			hash32			fhash;
			magic			fmagic;
			size			funcompressed;
			size			flen;
		} file_header;
#pragma pack(pop)
#endif

		typedef std::map<hash32, file_info> file_info_map;
		typedef std::ifstream istream;
		typedef std::ofstream ostream;


	public:
		mapped_archive();
		~mapped_archive();

		//* template
		//* function 
		//* usefull for tricks
		template <class T>
		T on_file(std::string filename, std::function<T(void*, size)> func) {
			dynamic_buffer buffer;
			if (!get_file(filename, &buffer))
				return func(nullptr, 0);

			return func(buffer.get_data(), buffer.get_size());
		}

		template <class T>
		T on_file(std::wstring filename, std::function<T(void*, size)> func) {
			dynamic_buffer buffer;
			if (!get_file(filename, &buffer))
				return func(nullptr, 0);
			return func(buffer.get_data(),buffer.get_size());

		}

		template <class T>
		T on_file(std::string filename, std::function<T(dynamic_buffer::pointer)> func) {
			dynamic_buffer buffer;
			if (!get_file(filename, &buffer))
				return func(nullptr);
			return func(&buffer);

		}

		template <class T>
		T on_file(std::wstring filename, std::function<T(dynamic_buffer::pointer)> func) {
			dynamic_buffer buffer;
			if (!get_file(filename, &buffer))
				return func(nullptr);
			return func(&buffer);

		}


		void on_file(std::wstring filename, std::function<void(dynamic_buffer::pointer)> func) {
			dynamic_buffer buffer;
			if (!get_file(filename, &buffer))
				func(nullptr);

			else
				func(&buffer);
		}


		void on_file(std::string filename, std::function<void(dynamic_buffer::pointer)> func) {
			dynamic_buffer buffer;
			if (!get_file(filename, &buffer))
				func(nullptr);

			else
				func(&buffer);
		}

		void on_file(std::wstring filename, std::function<void(void*, size)> func) {
			dynamic_buffer buffer;
			if (!get_file(filename, &buffer))
				func(nullptr, 0);

			else
				func(buffer.get_data(), buffer.get_size());
		}


		void on_file(std::string filename, std::function<void(void*, size)> func) {
			dynamic_buffer buffer;
			if (!get_file(filename, &buffer))
				func(nullptr, 0);

			else
				func(buffer.get_data(), buffer.get_size());
		}


		bool on_each_file_mt(std::function<void(const std::wstring&, dynamic_buffer::pointer)> func);
		bool on_each_file_mt(std::function<void(const std::string&, dynamic_buffer::pointer)> func);

		void on_each_file(std::function<void(const std::wstring&, dynamic_buffer::pointer)> func) {
			for (auto iter = this->m_info_map.begin(); iter != this->m_info_map.end(); iter++) {
				dynamic_buffer buffer;
				if (!_get_file_from_iter(iter, &buffer))
					func(iter->second.fname, nullptr);
				else func(iter->second.fname, &buffer);
			}
		}

		
		void on_each_file(std::function<void(const std::string&, dynamic_buffer::pointer)> func) {
			for (auto iter = this->m_info_map.begin(); iter != this->m_info_map.end(); iter++) {
				dynamic_buffer buffer;
				std::string name = utils_to_string(iter->second.fname);
				if (!_get_file_from_iter(iter, &buffer))
					func(name, nullptr);
				else func(name, &buffer);
			}
		}


		//* enabling / disabling options
		void			enable_encryption();
		void			enable_compression();
		void			enable_cryptopp();
		
		void			disable_encryption();
		void			disable_compression();
		void			disable_cryptopp();

		bool			is_encryption_enabled();
		bool			is_compression_enabled();
		bool			is_cryptopp_enabled();

		bool			is_in_archive(const std::string& filename);
		bool			is_in_archive(const std::wstring& filename);
		bool			is_in_archive(const hash32 hash);
#ifdef __IMPROVE_IMPORT_ARCHIVE__
		bool			set_outfile(const std::wstring& wpath, const std::wstring& wdict);
		bool			set_outfile(const std::string& path, const std::string& dict);
#else
		bool			set_outfile(const std::wstring& wpath);
		bool			set_outfile(const std::string& path);
#endif

		//* inputs methods
#ifdef __IMPROVE_IMPORT_ARCHIVE__
		bool			import_archive(const std::wstring& path, dynamic_buffer& dict);
		bool			import_archive(const std::string& path, dynamic_buffer& dict);

		bool			import_archive(const std::wstring& path, const std::wstring& dict);
		bool			import_archive(const std::string& path, const std::string& dict);
#else
		bool			import_archive(const std::wstring& path);
		bool			import_archive(const std::string& path);
#endif

		bool			get_file(const std::wstring& path, dynamic_buffer::pointer buffer);
		bool			get_file(const std::string& path, dynamic_buffer::pointer buffer);
		bool			get_file(const hash32 hash, dynamic_buffer::pointer buffer);
		bool			get_file(const std::wstring& path, void* mem, const size memsize);
		bool			get_file(const std::string& path, void* mem, const size memsize);

		bool			get_file_mt_safe(const hash32 hash, dynamic_buffer::pointer buffer);

		bool			insert_file(const std::wstring& path, const std::wstring& realname= L"");
		bool			insert_file(const std::string& path, const std::string& realname = "");
		bool			insert_file(const std::wstring& path, const void* mem, const  size fsize, const std::wstring& realname = L"");
		bool			insert_file(const std::string& path, const void* mem, const size fsize, const std::string& realname = "");

		bool			insert_files_mt(const uint8_t thread_count, std::vector<std::string> files, std::function<bool(const std::wstring&, dynamic_buffer::pointer)> func = {});
		bool			insert_files_mt(const uint8_t thread_count, std::vector<insert_file_info_t> files, std::function<bool(const std::wstring&, dynamic_buffer::pointer)> func = {});
		bool			insert_files_mt(const uint8_t thread_count, std::vector<insert_file_info_tw> files, std::function<bool(const std::wstring&, dynamic_buffer::pointer)> func = {});

		bool			insert_folder(const std::wstring& folder);
		bool			insert_folder(const std::string& folder);

		bool			export_files(std::function<bool(const std::wstring&, dynamic_buffer::pointer)> func = {});
		bool			export_files(const std::string& into, std::function<bool(const std::wstring&, dynamic_buffer::pointer)> func = {});
		bool			export_files(const std::wstring& into, std::function<bool(const std::wstring&, dynamic_buffer::pointer)> func = {});

		bool			export_files_mt(const uint8_t thread_count, std::function<bool(const std::wstring&, dynamic_buffer::pointer)> func = {});
		bool			export_files_mt(const uint8_t thread_count, const std::string& into, std::function<bool(const std::wstring&, dynamic_buffer::pointer)> func = {});
		bool			export_files_mt(const uint8_t thread_count, const std::wstring& into, std::function<bool(const std::wstring&, dynamic_buffer::pointer)> func = {});

		bool			export_file(const std::wstring& file, const std::wstring& into = L"");
		bool			export_file(const std::string& file, const std::string& into="");



		bool			close_output();

		void			set_key(const key& key_);
		const key&		get_key() const;

		size			get_file_count() const;

	private:
#ifdef __IMPROVE_IMPORT_ARCHIVE__
		bool			__import_archive(dynamic_buffer& buff);
#else
		bool			__import_archive();
#endif
		bool			_get_file_from_iter(const file_info_map::iterator it, dynamic_buffer::pointer buffer);

	private:
		file_info_map	m_info_map;
		key				m_key;
		istream*		m_input_stream;
		ostream*		m_output_stream;
#ifdef __IMPROVE_IMPORT_ARCHIVE__
		ostream*		m_output_dict_stream;
#endif
		bool			m_use_encryption;
		bool			m_use_compression;
		bool			m_use_cryptopp;
	};
}


