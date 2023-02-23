#pragma once

namespace ikarusweb2 {
	class DynamicBuffer {
	public:
		enum eConst {
			DynamicInitSize = 1024,
			DynamicSize = 1024 * 5,
		};

		typedef BYTE* BufferPtr;
		typedef DWORD BufferSize;

	public:
		DynamicBuffer(BufferSize initSize=DynamicInitSize);
		~DynamicBuffer();


		void		clear();
		bool		write(void* mem, BufferSize size);
		BufferPtr   read();
		BufferSize  get_size();
		BufferSize  get_capacity();
		BufferSize  get_space();
		bool		reserve(BufferSize size);
		bool		is_alloc();

		//* template methods
		template <class T>
		bool        write_obj(const T& obj) {
			return write(&obj, sizeof(T));
		}

		template <class T>
		bool		read_obj(T* objPtr) {
			if (m_usedSize >= sizeof(T)) {
				memcpy(objPtr, m_memoryptr, sizeof(T));
				return true;
			}

			else {
				return false;
			}
		}



	private:
		void		__init(BufferSize init_size);
		bool        __realloc(BufferSize min_);
		bool		__can_write(BufferSize size);


	private:
		BufferPtr	m_memoryptr;
		BufferSize  m_allocatedSize;
		BufferSize  m_usedSize;

	};
}

