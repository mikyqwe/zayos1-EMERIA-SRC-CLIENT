#pragma once

namespace ikaruspack {
	class __Key {
	public:
		__Key();
		~__Key();

		void					set_bytes(const std::string& inp);
		const std::string&		get_bytes() const;
		const char*				c_str() const;

		void					confuse_bytes1();
		void					confuse_bytes2();
		void					confuse_bytes3();
		void					confuse_bytes4();



	private:
		std::string bytes;

	};
}