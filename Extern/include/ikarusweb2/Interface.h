#pragma once

namespace ikarusweb2 {

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