#pragma once


namespace ikarusweb2 {

	class Download {

	public:
		enum class DownloadOption {
			Block		= 1UL << 0,
			NonBlock	= 1UL << 1,
			TraceData	= 1UL << 2,
			UseCallback	= 1UL << 3,
			SProtocol   = 1UL << 4,
		};

		enum class SetUrlError {
			UnrecognizedUrl = 0,
			NoInternetConnection,
			ConnectionServerFail,
			SProtocolFail,
			ConnectionSuccess,
		};

		enum class DownloadError {
			IoCtlError,
			RecvError,
			NoInternetConnection,
			DownloadTimeout,
			BufferWritingError,
			RequestError,
			UnsetUrl,
			DownloadSuccess,
			NoError,
		};

		enum class DownloadTimeout {
			DownloadTimoutVeryShort	= 1,
			DownloadTimoutShort		= 3,
			DownloadTimoutRegular	= 5,
			DownloadTimoutLarge		= 10,
			DownloadTimoutVeryLarge = 20,
		};

		typedef std::function<void(Download*)> Callback;
		typedef Callback* CallbackPtr;
		typedef DWORD DownloadFlag;


		static bool				isDownloadOption(DownloadFlag flag, DownloadOption opt);
		static void				setDownloadOption(DownloadFlag& flag, DownloadOption opt);
		static void				unsetDownloadOption(DownloadFlag& flag, DownloadOption opt);
		static void				toogleDownloadOption(DownloadFlag& flag, DownloadOption opt);


		Download();
		~Download();

		void					clear();
		void					shut();
		
		bool					join();
		bool					joinable();

		SetUrlError				set_url(const DownloadFlag flag, const std::string url);
		SetUrlError				set_url(const DownloadFlag flag, const std::wstring url);

		bool					start_download();

		DynamicBuffer&			lock_buffer();
		void					unlock_buffer();

		void					set_success_callback(Callback callback);
		void					set_fail_callback(Callback callback);
		void					set_update_callback(Callback callback);

		void					set_timeout(double seconds);
		void					set_timeout(DownloadTimeout timeout);
		void					set_timeout_lock(double seconds);


		const std::wstring&		get_servername() const;
		const std::wstring&     get_pagename() const;
		DWORD					get_port() const;

		double					get_timeout();
		double					get_timeout_lock();

		double					get_remain_seconds();
		double					get_remain_seconds_lock();

		long long				get_download_speed();
		long long				get_download_speed_lock();

		DownloadError			get_error();
		DownloadError			get_error_lock();

		int						get_error_code();
		int						get_error_code_lock();

		bool					is_working();
		bool					is_working_lock();

		long long				get_total_bytes();
		long long				get_total_bytes_lock();

		DWORD					get_header_size();
		DWORD					get_header_size_lock();

		DynamicBuffer&			get_buffer();

	private:
		bool					__send_request();
		bool					__blocking_download();
		void					_clean_download();
		void					_exame_page_post_request();

	private:
		std::mutex				m_mutex;
		DynamicBuffer			m_buffer;
		std::thread*			m_threadPtr;

		std::wstring			m_stPage;
		std::wstring			m_stServer;
		std::wstring			m_stPostData;
		DWORD					m_dwPort;

		HINTERNET				m_hInternet = NULL;
		HINTERNET				m_hConnection = NULL;
		HINTERNET				m_hRequest = NULL;

		CallbackPtr				m_successCallback;
		CallbackPtr				m_failCallback;
		CallbackPtr				m_updateCallback;

		bool					m_bIsShut;
		bool					m_bIsUsingNoEndSocket;
		bool					m_bIsUsingSecureProtocol;

		//* trace data informations
		long long				m_llDownSpeed;
		double					m_remainSeconds;

		long long				m_llTotalBytes;
		DWORD					m_dwHeaderSize;

		int						m_errorCode;
		DownloadError			m_downloadError;

		double					m_downloadTimeout;
		DownloadFlag			m_reservedFlag;
	};

}
