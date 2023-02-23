#pragma once

namespace ikaruspack {
	class archive {

	public:
		static const uint64_t	filename_max_len = 256;

	public:
		

		typedef __Key			key;
		typedef uint32_t		hash32;
		typedef uint64_t		offset;
		typedef uint64_t		size;
		typedef uint8_t			magic;
		
		typedef struct  {
			wchar_t			fname[filename_max_len];
			offset			foffset;
			size			fsize;
			hash32			fhash;
			magic			fmagic;
		}file_info;

		

		typedef std::map<hash32, file_info> file_info_map;
		typedef std::map<hash32, dynamic_buffer*> file_buffer_map;



	public:
		archive();
		~archive();




		//* template
		//* function 
		//* usefull for tricks
		template <class T>
		T on_file(std::string filename, std::function<T(void*,size)> func) {
			dynamic_buffer::pointer buffptr;
			if(!get_file(filename, &buffptr))
				return func(nullptr,0);

			return func(buffptr->get_data(), buffptr->get_size());
		}

		template <class T>
		T on_file(std::wstring filename, std::function<T(void*,size)> func) {
			dynamic_buffer::pointer buffptr;
			if(!get_file(filename, &buffptr))
				return func(nullptr,0);
			return func(buffptr->get_data(), buffptr->get_size());

		}

		template <class T>
		T on_file(std::string filename, std::function<T(dynamic_buffer::pointer)> func) {
			dynamic_buffer::pointer buffptr;
			if(!get_file(filename, &buffptr))
				return func(nullptr);
			return func(buffptr);

		}

		template <class T>
		T on_file(std::wstring filename, std::function<T(dynamic_buffer::pointer)> func) {
			dynamic_buffer::pointer buffptr;
			if(!get_file(filename, &buffptr))
				return func(nullptr);
			return func(buffptr);

		}


		void on_file(std::wstring filename, std::function<void(dynamic_buffer::pointer)> func) {
			dynamic_buffer::pointer buffptr;
			if(!get_file(filename, &buffptr))
				func(nullptr);

			else
				func(buffptr);
		}


		void on_file(std::string filename, std::function<void(dynamic_buffer::pointer)> func) {
			dynamic_buffer::pointer buffptr;
			if(!get_file(filename, &buffptr))
				func(nullptr);

			else
				func(buffptr);
		}

		void on_file(std::wstring filename, std::function<void(void*, size)> func) {
			dynamic_buffer::pointer buffptr;
			if(!get_file(filename, &buffptr))
				func(nullptr,0);

			else
				func(buffptr->get_data(), buffptr->get_size());
		}


		void on_file(std::string filename, std::function<void(void*, size)> func) {
			dynamic_buffer::pointer buffptr;
			if(!get_file(filename, &buffptr))
				func(nullptr, 0);

			else
				func(buffptr->get_data(), buffptr->get_size());
		}




		//* inputs methods
		bool			import_archive(std::wstring path);
		bool			import_archive(std::string path);

		bool			get_file(std::wstring path, dynamic_buffer::pointer* buffer) ;
		bool			get_file(std::string path,  dynamic_buffer::pointer* buffer) ;
		bool			get_file(std::wstring path, void* mem, size memsize) ;
		bool			get_file(std::string path,  void* mem, size memsize) ;

		//* outputs methods
		bool			export_archive(std::wstring path) const;
		bool			export_archive(std::string path) const;

		bool			insert_file(std::wstring path);
		bool			insert_file(std::string path);
		bool			insert_file(std::wstring path, const void* mem, size fsize);
		bool			insert_file(std::string path, const void* mem, size fsize);

		bool			insert_folder(std::wstring folder);
		bool			insert_folder(std::string folder);

		bool			export_files();
		bool			export_files(std::string into);
		bool			export_files(std::wstring into);


		//* common methods
		void			set_key(const key& key_);
		const key&		get_key() const;

		size			get_file_count() const;
		size			get_memory_size() const;

		
	private:
		bool			__import_archive(dynamic_buffer& buff);



	private:
		file_info_map					m_info_map;
		file_buffer_map					m_buff_map;
		key								m_key;
	};
}


