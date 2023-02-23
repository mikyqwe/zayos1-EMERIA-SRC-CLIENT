/*
Copyright (c) 2019 IkarusDeveloper

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef __INCLUDE_HEADER_UTILS_IKARUSGL__
#define __INCLUDE_HEADER_UTILS_IKARUSGL__
#define __use_debug_mutex() std::lock_guard<mutex_t> lock(_g_global_mutex_debug);

namespace ikgl
{
	namespace utilities {

		inline void_t default_debug(const_char_t* fmt, valist_t args) {
			static char szBuff[512];
			vsnprintf(szBuff, sizeof(szBuff), fmt, args);
			printf("%s\n", szBuff);
		}

		inline void_t default_debugW(const wchar_t* fmt, valist_t args) {
			static wchar_t buff[512];
			_vsnwprintf_s(buff, sizeof(buff) / sizeof(buff[0]), fmt, args);
			wprintf(L"%s\n", buff);
		}

		inline void_t set_debug_func(std::function<void_t(const_char_t*, valist_t)> func) {
			IKGL_global_debug = func;
		}

		inline void_t set_debug_funcW(std::function<void_t(const wchar_t* fmt, valist_t)> func) {
			IKGL_global_debugW = func;
		}

		inline void_t set_error_func(std::function<void_t(const_char_t*, valist_t)> func) {
			IKGL_global_error = func;
		}

		inline void_t set_error_funcW(std::function<void_t(const wchar_t* fmt, valist_t)> func) {
			IKGL_global_errorW = func;
		}

		inline void_t debug(const_char_t* fmt, ...) {
			valist_t args;
			va_start(args, fmt);
			{
				__use_debug_mutex();
				IKGL_global_debug(fmt, args);
			}
			va_end(args);
		}

		inline void_t debugW(const wchar_t* fmt, ...) {
			valist_t args;
			va_start(args, fmt);
			{
				__use_debug_mutex();
				IKGL_global_debugW(fmt, args);
			}
			va_end(args);
		}

		inline void_t error(const_char_t* fmt, ...) {
			valist_t args;
			va_start(args, fmt);
			{
				__use_debug_mutex();
				IKGL_global_error(fmt, args);
			}
			va_end(args);
		}

		inline void_t errorW(const wchar_t* fmt, ...) {
			valist_t args;
			va_start(args, fmt);
			{
				__use_debug_mutex();
				IKGL_global_errorW(fmt, args);
			}
			va_end(args);
		}

		inline const double_t get_time_double() {
			return std::chrono::duration<double_t>(std::chrono::system_clock::now().time_since_epoch()).count();
		}

		
		inline std::string to_string(const std::wstring& wstr)
		{
			using convert_t = std::codecvt_utf8<wchar_t>;
			static std::wstring_convert<convert_t, wchar_t> strconverter;
			return strconverter.to_bytes(wstr);
		}

		inline std::wstring to_wstring(const std::string& str)
		{
			using convert_t = std::codecvt_utf8<wchar_t>;
			static std::wstring_convert<convert_t, wchar_t> strconverter;
			return strconverter.from_bytes(str);
		}

		inline const_char_t* get_message_name(uint_t msg)
		{
#define __case_of_message(s) case s: return #s
			switch (msg) {
				__case_of_message(WM_QUIT);
				__case_of_message(WM_SIZE);
				__case_of_message(WM_LBUTTONUP);
				__case_of_message(WM_RBUTTONUP);
				__case_of_message(WM_LBUTTONDOWN);
				__case_of_message(WM_RBUTTONDOWN);
				__case_of_message(WM_DESTROY);
				__case_of_message(WM_PAINT);
				__case_of_message(WM_CREATE);
				__case_of_message(WM_MOUSEMOVE);
				__case_of_message(WM_NULL);
				__case_of_message(WM_MOVE);
				__case_of_message(WM_ACTIVATE);
				__case_of_message(WM_SETFOCUS);
				__case_of_message(WM_KILLFOCUS);
				__case_of_message(WM_ENABLE);
				__case_of_message(WM_SETREDRAW);
				__case_of_message(WM_SETTEXT);
				__case_of_message(WM_GETTEXT);
				__case_of_message(WM_GETTEXTLENGTH);
				__case_of_message(WM_CLOSE);
				__case_of_message(WM_QUERYENDSESSION);
				__case_of_message(WM_QUERYOPEN);
				__case_of_message(WM_ERASEBKGND);
				__case_of_message(WM_SYSCOLORCHANGE);
				__case_of_message(WM_ENDSESSION);
				__case_of_message(WM_SHOWWINDOW);
				__case_of_message(WM_DEVMODECHANGE);
				__case_of_message(WM_ACTIVATEAPP);
				__case_of_message(WM_FONTCHANGE);
				__case_of_message(WM_TIMECHANGE);
				__case_of_message(WM_CANCELMODE);
				__case_of_message(WM_SETCURSOR);
				__case_of_message(WM_MOUSEACTIVATE);
				__case_of_message(WM_CHILDACTIVATE);
				__case_of_message(WM_QUEUESYNC);
				__case_of_message(WM_GETMINMAXINFO);
				__case_of_message(WM_PAINTICON);
				__case_of_message(WM_ICONERASEBKGND);
				__case_of_message(WM_NEXTDLGCTL);
				__case_of_message(WM_SPOOLERSTATUS);
				__case_of_message(WM_DRAWITEM);
				__case_of_message(WM_MEASUREITEM);
				__case_of_message(WM_DELETEITEM);
				__case_of_message(WM_VKEYTOITEM);
				__case_of_message(WM_CHARTOITEM);
				__case_of_message(WM_SETFONT);
				__case_of_message(WM_GETFONT);
				__case_of_message(WM_SETHOTKEY);
				__case_of_message(WM_GETHOTKEY);
				__case_of_message(WM_QUERYDRAGICON);
				__case_of_message(WM_COMPAREITEM);
				__case_of_message(WM_GETOBJECT);
				__case_of_message(WM_COMPACTING);
				__case_of_message(WM_COMMNOTIFY);
				__case_of_message(WM_WINDOWPOSCHANGING);
				__case_of_message(WM_WINDOWPOSCHANGED);
				__case_of_message(WM_POWER);
				__case_of_message(WM_COPYDATA);
				__case_of_message(WM_CANCELJOURNAL);
				__case_of_message(WM_INPUTLANGCHANGEREQUEST);
				__case_of_message(WM_INPUTLANGCHANGE);
				__case_of_message(WM_TCARD);
				__case_of_message(WM_HELP);
				__case_of_message(WM_USERCHANGED);
				__case_of_message(WM_NOTIFYFORMAT);
				__case_of_message(WM_CONTEXTMENU);
				__case_of_message(WM_STYLECHANGING);
				__case_of_message(WM_STYLECHANGED);
				__case_of_message(WM_DISPLAYCHANGE);
				__case_of_message(WM_GETICON);
				__case_of_message(WM_SETICON);
				__case_of_message(WM_NCCREATE);
				__case_of_message(WM_NCDESTROY);
				__case_of_message(WM_NCCALCSIZE);
				__case_of_message(WM_NCHITTEST);
				__case_of_message(WM_NCPAINT);
				__case_of_message(WM_NCACTIVATE);
				__case_of_message(WM_GETDLGCODE);
				__case_of_message(WM_SYNCPAINT);
				__case_of_message(WM_NCMOUSEMOVE);
				__case_of_message(WM_NCLBUTTONDOWN);
				__case_of_message(WM_NCLBUTTONUP);
				__case_of_message(WM_NCLBUTTONDBLCLK);
				__case_of_message(WM_NCRBUTTONDOWN);
				__case_of_message(WM_NCRBUTTONUP);
				__case_of_message(WM_NCRBUTTONDBLCLK);
				__case_of_message(WM_NCMBUTTONDOWN);
				__case_of_message(WM_NCMBUTTONUP);
				__case_of_message(WM_NCMBUTTONDBLCLK);
				__case_of_message(WM_NCXBUTTONDOWN);
				__case_of_message(WM_NCXBUTTONUP);
				__case_of_message(WM_NCXBUTTONDBLCLK);
				__case_of_message(WM_INPUT);
				__case_of_message(WM_KEYDOWN);
				__case_of_message(WM_KEYUP);
				__case_of_message(WM_CHAR);
				__case_of_message(WM_DEADCHAR);
				__case_of_message(WM_SYSKEYDOWN);
				__case_of_message(WM_SYSKEYUP);
				__case_of_message(WM_SYSCHAR);
				__case_of_message(WM_SYSDEADCHAR);
				__case_of_message(WM_KEYLAST);
				__case_of_message(WM_IME_COMPOSITION);
				__case_of_message(WM_COMMAND);
				__case_of_message(WM_SYSCOMMAND);
				__case_of_message(WM_TIMER);
				__case_of_message(WM_HSCROLL);
				__case_of_message(WM_VSCROLL);
				__case_of_message(WM_INITMENU);
				__case_of_message(WM_INITMENUPOPUP);
				__case_of_message(WM_MENUSELECT);
				__case_of_message(WM_MENUCHAR);
				__case_of_message(WM_MENURBUTTONUP);
				__case_of_message(WM_MENUDRAG);
				__case_of_message(WM_MENUGETOBJECT);
				__case_of_message(WM_UNINITMENUPOPUP);
				__case_of_message(WM_MENUCOMMAND);
				__case_of_message(WM_CHANGEUISTATE);
				__case_of_message(WM_UPDATEUISTATE);
				__case_of_message(WM_QUERYUISTATE);
				__case_of_message(WM_CTLCOLORMSGBOX);
				__case_of_message(WM_CTLCOLOREDIT);
				__case_of_message(WM_CTLCOLORLISTBOX);
				__case_of_message(WM_CTLCOLORBTN);
				__case_of_message(WM_CTLCOLORDLG);
				__case_of_message(WM_CTLCOLORSCROLLBAR);
				__case_of_message(WM_CTLCOLORSTATIC);
				__case_of_message(WM_LBUTTONDBLCLK);
				__case_of_message(WM_RBUTTONDBLCLK);
				__case_of_message(WM_MBUTTONDOWN);
				__case_of_message(WM_MBUTTONUP);
				__case_of_message(WM_MBUTTONDBLCLK);
				__case_of_message(WM_MOUSELAST);
				__case_of_message(WM_MOUSEWHEEL);
				__case_of_message(WM_XBUTTONDOWN);
				__case_of_message(WM_XBUTTONUP);
				__case_of_message(WM_XBUTTONDBLCLK);
				__case_of_message(WM_PARENTNOTIFY);
				__case_of_message(WM_ENTERMENULOOP);
				__case_of_message(WM_EXITMENULOOP);
				__case_of_message(WM_NEXTMENU);
				__case_of_message(WM_SIZING);
				__case_of_message(WM_CAPTURECHANGED);
				__case_of_message(WM_MOVING);
				__case_of_message(WM_POWERBROADCAST);
				__case_of_message(WM_DEVICECHANGE);
				__case_of_message(WM_MDICREATE);
				__case_of_message(WM_MDIDESTROY);
				__case_of_message(WM_MDIACTIVATE);
				__case_of_message(WM_MDIRESTORE);
				__case_of_message(WM_MDINEXT);
				__case_of_message(WM_MDIMAXIMIZE);
				__case_of_message(WM_MDITILE);
				__case_of_message(WM_MDICASCADE);
				__case_of_message(WM_MDIICONARRANGE);
				__case_of_message(WM_MDIGETACTIVE);
				__case_of_message(WM_MDISETMENU);
				__case_of_message(WM_ENTERSIZEMOVE);
				__case_of_message(WM_EXITSIZEMOVE);
				__case_of_message(WM_DROPFILES);
				__case_of_message(WM_MDIREFRESHMENU);
				__case_of_message(WM_IME_SETCONTEXT);
				__case_of_message(WM_IME_NOTIFY);
				__case_of_message(WM_IME_CONTROL);
				__case_of_message(WM_IME_COMPOSITIONFULL);
				__case_of_message(WM_IME_CHAR);
				__case_of_message(WM_IME_REQUEST);
				__case_of_message(WM_IME_KEYDOWN);
				__case_of_message(WM_IME_KEYUP);
				__case_of_message(WM_NCMOUSEHOVER);
				__case_of_message(WM_MOUSEHOVER);
				__case_of_message(WM_NCMOUSELEAVE);
				__case_of_message(WM_MOUSELEAVE);
				__case_of_message(WM_CUT);
				__case_of_message(WM_COPY);
				__case_of_message(WM_PASTE);
				__case_of_message(WM_CLEAR);
				__case_of_message(WM_UNDO);
				__case_of_message(WM_RENDERFORMAT);
				__case_of_message(WM_RENDERALLFORMATS);
				__case_of_message(WM_DESTROYCLIPBOARD);
				__case_of_message(WM_DRAWCLIPBOARD);
				__case_of_message(WM_PAINTCLIPBOARD);
				__case_of_message(WM_VSCROLLCLIPBOARD);
				__case_of_message(WM_SIZECLIPBOARD);
				__case_of_message(WM_ASKCBFORMATNAME);
				__case_of_message(WM_CHANGECBCHAIN);
				__case_of_message(WM_HSCROLLCLIPBOARD);
				__case_of_message(WM_QUERYNEWPALETTE);
				__case_of_message(WM_PALETTEISCHANGING);
				__case_of_message(WM_PALETTECHANGED);
				__case_of_message(WM_HOTKEY);
				__case_of_message(WM_PRINT);
				__case_of_message(WM_PRINTCLIENT);
				__case_of_message(WM_APPCOMMAND);
				__case_of_message(WM_HANDHELDFIRST);
				__case_of_message(WM_HANDHELDLAST);
				__case_of_message(WM_AFXFIRST);
				__case_of_message(WM_AFXLAST);
				__case_of_message(WM_PENWINFIRST);
			default: return "Unknow";
			}

#undef __case_of_message
		}

		inline const_char_t* get_gdiplus_status_str_name(const gdiplus_status_t& status) {
#define __case_of_gdiplus_status(s) case s: return #s
			switch (status) {
				__case_of_gdiplus_status(Gdiplus::Ok);
				__case_of_gdiplus_status(Gdiplus::GenericError);
				__case_of_gdiplus_status(Gdiplus::InvalidParameter);
				__case_of_gdiplus_status(Gdiplus::OutOfMemory);
				__case_of_gdiplus_status(Gdiplus::ObjectBusy);
				__case_of_gdiplus_status(Gdiplus::InsufficientBuffer);
				__case_of_gdiplus_status(Gdiplus::NotImplemented);
				__case_of_gdiplus_status(Gdiplus::Win32Error);
				__case_of_gdiplus_status(Gdiplus::WrongState);
				__case_of_gdiplus_status(Gdiplus::Aborted);
				__case_of_gdiplus_status(Gdiplus::FileNotFound);
				__case_of_gdiplus_status(Gdiplus::ValueOverflow);
				__case_of_gdiplus_status(Gdiplus::AccessDenied);
				__case_of_gdiplus_status(Gdiplus::UnknownImageFormat);
				__case_of_gdiplus_status(Gdiplus::FontFamilyNotFound);
				__case_of_gdiplus_status(Gdiplus::FontStyleNotFound);
				__case_of_gdiplus_status(Gdiplus::NotTrueTypeFont);
				__case_of_gdiplus_status(Gdiplus::UnsupportedGdiplusVersion);
				__case_of_gdiplus_status(Gdiplus::GdiplusNotInitialized);
				__case_of_gdiplus_status(Gdiplus::PropertyNotFound);
				__case_of_gdiplus_status(Gdiplus::PropertyNotSupported);
			default: return "Unknow";
			}
#undef __case_of_gdiplus_status
		}

		template <class T>
		void_t copy_obj(T& objDest, const T& objSrc) {
			memcpy(&objDest, &objSrc, sizeof(T));
		}

		inline gdiplus_image_t* get_image_from_buffer(const void_t* data, dword_t len) {
			hglobal_t buffer = ::GlobalAlloc(GMEM_MOVEABLE, len);
			void_t* bufferptr = nullptr;	istream_t* pStream = nullptr;

			//checking if works the global allocation
			if (!buffer) {
				__ikgl_error("loadImage :: cannot allocate global (len %u) \n", len);
				return nullptr;
			}

			//check if success the Lock
			if (!(bufferptr = ::GlobalLock(buffer))) {
				::GlobalFree(buffer);
				__ikgl_error("loadImage :: cannot Lock global \n");
				return nullptr;
			}

			//copying memory into global mem locked
			memcpy(bufferptr, data, len);

			//creating stream
			if (::CreateStreamOnHGlobal(buffer, FALSE, &pStream) == S_OK) {
				//making image from stream 
				auto* image = gdiplus_image_t::FromStream(pStream, false);
				pStream->Release();

				//unlock & free global
				::GlobalUnlock(buffer);
				::GlobalFree(buffer);

				//return true if pointer is != nullptr and if load operation sucess
				return image;
			}

			//else unlocking and free the global and return false
			else {
				::GlobalUnlock(buffer);
				::GlobalFree(buffer);
				__ikgl_error("loadImage :: cannot create stream \n");
				return nullptr;
			}
		}




		template <class T>
		uint_t get_element_count(T& vec)
		{
			if (sizeof(vec) == 0)
				return 0;

			return sizeof(vec) / sizeof(vec[0]);
		}

		inline void_t normalize_rect(gdiplus_rectf_t& rect, float_t degree) {
			auto toRad = [](const float_t degree) -> float_t {
				return degree * 3.141592653589793f / 180.0f;
			};

			const float_t alpha = toRad(degree);

			//* calculating to avoid the multiple use of cos
			const float_t cosalpha = cos(alpha);
			const float_t sinalpha = sin(alpha);

			//* define constant to make readable the formula
			const float_t y = rect.Y;
			const float_t x = rect.X;

			const float_t x_ = x * cosalpha + y * sinalpha;
			const float_t y_ = y * cosalpha - x * sinalpha;

			rect.X = x_;
			rect.Y = y_;
		}

		inline void_t normalize_rect(gdiplus_rect_t& rect, float_t degree) {

			auto toRad = [](const float_t degree) -> float_t {
				return degree * 3.141592653589793f / 180.0f;
			};

			const float_t alpha = toRad(degree);

			//* calculating to avoid the multiple use of cos
			const float_t cosalpha = cos(alpha);
			const float_t sinalpha = sin(alpha);

			//* define constant to make readable the formula
			const float_t y = (float_t)rect.Y;
			const float_t x = (float_t)rect.X;

			const float_t x_ = x * cosalpha + y * sinalpha;
			const float_t y_ = y * cosalpha - x * sinalpha;

			rect.X = (int_t)x_;
			rect.Y = (int_t)y_;
		}

		inline void_t normalize_rect(widget_rect_t& rect, float_t degree) {

			auto to_radiant = [](const float_t degree) -> float_t {
				return degree * 3.141592653589793f / 180.0f;
			};

			const float_t alpha = to_radiant(degree);

			//* calculating to avoid the multiple use of cos
			const float_t cosalpha = cos(alpha);
			const float_t sinalpha = sin(alpha);

			//* define constant to make readable the formula
			const float_t y = (float_t)rect.top;
			const float_t x = (float_t)rect.left;

			const auto w = rect.right - rect.left;
			const auto h = rect.bottom - rect.top;

			const float_t x_ = x * cosalpha + y * sinalpha;
			const float_t y_ = y * cosalpha - x * sinalpha;

			rect.left = (coord_t)x_;
			rect.top = (coord_t)y_;

			rect.right = rect.left + w;
			rect.bottom = rect.top + h;
		}

		inline void_t normalize_position(widget_pos_t& pos, float_t degree) {

			auto to_radiant = [](const float_t degree) -> float_t {
				return degree * 3.141592653589793f / 180.0f;
			};

			const float_t alpha = to_radiant(degree);

			//* calculating to avoid the multiple use of cos
			const float_t cosalpha = cos(alpha);
			const float_t sinalpha = sin(alpha);

			//* define constant to make readable the formula
			const float_t y = (float_t)pos.y;
			const float_t x = (float_t)pos.x;

			const float_t x_ = x * cosalpha + y * sinalpha;
			const float_t y_ = y * cosalpha - x * sinalpha;

			pos.x = (coord_t)x_;
			pos.y = (coord_t)y_;
		}

		inline widget_size_t measure_string_strict(uint_t emsize, gdiplus_graphics_t* graphicsptr, LPCWSTR textptr, int_t textlen, const gdiplus_font_t* font, widget_pos_t pos) {
			gdiplus_string_format_t format = gdiplus_string_format_t::GenericTypographic();
			format.SetTrimming(gdiplus_string_trimming_t::StringTrimmingNone);
			format.SetFormatFlags(format.GetFormatFlags() | gdiplus_string_format_flag_t::StringFormatFlagsMeasureTrailingSpaces);

			//* setting tab stops
			const auto tabcount = std::count(textptr, textptr + textlen, (wchar_t)'\t');

			if (tabcount != 0) {
				std::vector<gdiplus_real_t> vectab;
				vectab.resize(tabcount, 15.f);
				format.SetTabStops(0, (INT)tabcount, vectab.data());
			}

			gdiplus_rectf_t rect;
			graphicsptr->MeasureString(textptr, textlen, font, pos.to_gdiplus_pointf(), &format, &rect);

			widget_size_t size = { coord_t(rect.Width), coord_t(rect.Height) };
			return size;
		}

		template <class T, typename S>
		std::vector<T> split_string_by(const T& str, const S delimiter) {
			std::vector<T> result;
			result.reserve(std::count(str.begin(), str.end(), delimiter) + 1);

			size_t last_pos = str.length() - 1;
			size_t old_pos = 0;
			size_t cur_pos = 0;

			if (result.size() > 1) {
				while ((cur_pos = str.find(delimiter, old_pos)) != T::npos) {
					result.emplace_back(str.substr(old_pos, cur_pos - old_pos));
					old_pos = cur_pos + 1;

					if (cur_pos == last_pos) {
						break;
					}
				}

				if (old_pos < last_pos) {
					result.emplace_back(str.substr(old_pos));
				}
			}

			else {
				result.emplace_back(str);
			}

			return result;
		}

		inline widget_size_t get_desktop_size() {
			RECT rect = external::get_window_rect(external::get_desktop_window());
			return { rect.right - rect.left, rect.bottom - rect.top };
		}

		inline widget_size_t get_app_bar_size() {
			appbardata_t abd = {};
			abd.cbSize = sizeof(appbardata_t);

			if (::SHAppBarMessage(ABM_GETTASKBARPOS, &abd)) {
				return { abd.rc.right - abd.rc.left, abd.rc.bottom - abd.rc.top };
			}

			else {
				return { 0,0 };
			}
		}

		inline widget_rect_t get_app_bar_rect() {
			appbardata_t abd = {};
			abd.cbSize = sizeof(appbardata_t);

			if (::SHAppBarMessage(ABM_GETTASKBARPOS, &abd)) {
				return { abd.rc };
			}

			else {
				return { 0,0,0,0 };
			}
		}

		inline void_t zero_memory(void_t* mem, int_t len) {
			memset(mem, 0, len);
		}

		template <class T>
		void_t zero_obj(T& obj) {
			memset(&obj, 0, sizeof(T));
		}

		template <class T>
		void_t zero_obj_ptr(T* obj) {
			memset(obj, 0, sizeof(T));
		}

		template <class T>
		void_t destroy_obj_ptr(T*& ptr) {
			if (ptr) {
				delete(ptr);
				ptr = nullptr;
			}
		}

		template <class T>
		hash_name_t get_hash(const T& obj) {
			return std::hash<T>{}(obj);
		}

		template <class T>
		wstring_t get_empty_name_by_type(T* win) {
			return T::empty_name();
		}

		template <class t1, class t2>
		void_t set_bit(t1& flag, const t2& bit) {
			flag |= static_cast<t1>(bit);
		}

		template <class t1, class t2>
		void_t unset_bit(t1& flag, const t2& bit) {
			flag &= ~static_cast<t1>(bit);
		}

		template <class t1, class t2>
		bool_t isset_bit(t1 flag, const t2& bit) {
			return flag & static_cast<t1>(bit);
		}

		template <class t1, class t2>
		void_t toogle_bit(t1& flag, const t2& bit) {
			flag ^= static_cast<t1>(bit);
		}

		template <class t1>
		float_t float_cast(const t1& v) {
			return static_cast<float_t>(v);
		}

		class safe_runner {
		public:

			safe_runner(mutex_t& mutex) {
				this->_mutex = &mutex;
			}

			void set_use_mutex(bool_t v) {
				_use_mutex = v;
			}

			bool_t get_use_mutex() {
				return this->_use_mutex;
			}

			template <class F>
			void run_function(F func) {
				if (this->_use_mutex) {
					std::lock_guard<std::mutex> _lock(*this->_mutex);
					func();
				} else func();
			}

		private:
			bool_t _use_mutex;
			mutex_t* _mutex;
		};

		template <class type>
		type maxof(const type v1, const type v2) {
			return  (v1 > v2 ? v1 : v2);
		}

		template <class type>
		type minof(const type v1, const type v2) {
			return (v1 > v2 ? v2 : v1);
		}

		template <class type>
		type minmaxof(const type minval, const type v, const type maxval) {
			return (maxof<type>(minval, minof<type>(maxval, v)));
		}

		template <class type>
		type moduleof(const type v) {
			return maxof<type>(v, -v);
		}

		template <class type, class origin>
		type to(const origin val) {
			return static_cast<type>(val);
		}

		template<class type>
		class safe_dividend {
		public:
			safe_dividend(const type v) {
				this->_val = v;
			}
			
			type operator / (const type divisor) {
				if (divisor == 0)
					return 0;
				return this->_val / divisor;
			}

			type operator /= (const type divisor) {
				this->_val = *this / divisor;
				return this->_val;
			}

		private:
			type _val;
		};
	}
}


#endif //__INCLUDE_HEADER_UTILS_IKARUSGL__
