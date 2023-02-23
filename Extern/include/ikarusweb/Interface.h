#pragma once

namespace ikarusweb {

	class Interface {
	public:
		typedef std::vector<Download> DownloadVector;

	public:
		Interface();
		~Interface();




	private:
		std::mutex				m_mutex;
		DownloadVector			m_downloads;


	};

}