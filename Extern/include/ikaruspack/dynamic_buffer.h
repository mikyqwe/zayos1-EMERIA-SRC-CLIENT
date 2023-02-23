#pragma once

namespace ikaruspack {
	class dynamic_buffer {
	public:
		enum eConst {
			DynamicInitSize = 1024,
			DynamicSize = 1024 * 5,
		};

		enum class MoveDirection {
			MovePrevious,
			MoveNext,
		};

		typedef char*		buffer_ptr;
		typedef uint64_t	buffer_size;

		typedef struct {
			buffer_size alloc_size;
			buffer_size write_cursor;
			buffer_size read_cursor;
			buffer_ptr  ptr;
		} buffer_info;


	public:
		dynamic_buffer(buffer_size init_size=DynamicInitSize);
		dynamic_buffer(buffer_info& init_info);
		~dynamic_buffer();



		//* templates methods
		template <class T>
		bool write(const T& obj) {
			return write(&obj, sizeof(T));
		}

		template <class T>
		bool write(const T* objptr) {
			return write(objptr, sizeof(T));
		}


		template <class T>
		bool read(T* obj_ptr) {
			return read(obj_ptr, sizeof(T));
		}

		template <class T>
		bool read(T& r_obj) {
			return read(&r_obj, sizeof(T));
		}


		template <class T>
		bool write_from_stream(T& stream, buffer_size size) {
			if (!can_write(size)) 
				if (!__realloc(size + m_write_cursor)) 
					return false;


			stream.read((char*) (m_memoryptr + m_write_cursor) , size);
			m_write_cursor += size;
			return true;
		}


		






		//* non-inline methods
		void			clear();

		bool			write(const void* mem, buffer_size size);
		bool			read(void* mem, buffer_size size);

		bool			can_write(buffer_size size);
		bool			can_read(buffer_size size);

		bool			write_from_buffer(dynamic_buffer& from, buffer_size size);

		buffer_size		get_size() const;
		buffer_size		get_capacity() const;
		buffer_size		get_space() const;
		buffer_ptr		get_data() const;

		bool			reserve(buffer_size size);
		bool			is_alloc();

		void			reset_reading();
		bool			move_reading(buffer_size size, MoveDirection direction = MoveDirection::MoveNext );

		buffer_info		release();
		void			assign(buffer_info& info);

		//* reserved methods
	private:
		void			__init(buffer_size init_size);
		bool			__realloc(buffer_size min_);
		

		//* reserved members
	private:
		buffer_ptr		m_memoryptr;
		buffer_size		m_alloc_size;
		buffer_size		m_write_cursor;
		buffer_size		m_read_cursor;




	public:
		typedef dynamic_buffer* pointer;


	};
}

