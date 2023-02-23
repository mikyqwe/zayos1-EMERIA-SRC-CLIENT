
namespace ikgl{
	namespace external {

		template <class T>
		T invalid_handle_cast() {
			return (T)INVALID_HANDLE_VALUE;
		}

		template <class T>
		T null_cast() {
			return (T)NULL;
		}

		template <class T>
		bool_t is_invalid_handle_value(const T& handle) {
			return handle == invalid_handle_cast<T>();
		}

		template <class T>
		T select_object(hdc_t hdc, T obj) {
			return (T) ::SelectObject(hdc,obj);
		}

		template <class T>
		bool_t delete_object(T obj) {
			return ::DeleteObject(obj) != FALSE;
		}

		template <class T>
		bool_t get_object(T val, size_t size, void_t* ptr) {
			return ::GetObjectW(val, (int)size, ptr);
		}

		inline word_t loword(lparam_t param) {
			return LOWORD(param);
		}

		inline word_t hiword(lparam_t param) {
			return HIWORD(param);
		}

		inline bool_t destroy_window(hwnd_t hwn) {
			return ::DestroyWindow(hwn) != FALSE;
		}

		inline dword_t get_last_error() {
			return ::GetLastError();
		}

		inline hdc_t get_dc(hwnd_t hwd) {
			return ::GetDC(hwd);
		}

		inline void_t release_dc(hwnd_t hwn, hdc_t hdc) {
			::ReleaseDC(hwn, hdc);
		}

		inline bool_t delete_dc(hdc_t hdc) {
			return ::DeleteDC(hdc) != FALSE;
		}

		inline hdc_t create_compatible_dc(hdc_t hdc) {
			return ::CreateCompatibleDC(hdc);
		}

		inline hbitmap_t create_compatible_bitmap(hdc_t hdc, const long_t w, const long_t h) {
			return ::CreateCompatibleBitmap(hdc, w, h);
		}

		inline bool_t bit_swap(hdc_t hdc_dest, hdc_t hdc_src, const long_t w, const long_t h) {
			return ::BitBlt(hdc_dest, 0, 0, w, h, hdc_src, 0, 0, SRCCOPY) != FALSE;
		}

		inline bool_t bit_blt(hdc_t hdc_dest, const coord_t xdest, const coord_t ydest, const long_t w, const long_t h ,hdc_t hdc_src, const coord_t xsrc, const coord_t ysrc ) {
			return ::BitBlt(hdc_dest, xdest, ydest, w, h, hdc_src, xsrc, ysrc, SRCCOPY) != FALSE;
		}

		inline bool_t update_layered_window(hwnd_t hwn,  hdc_t hdc_dest, hdc_t hdc_src, const long_t w, const long_t h, mc_colorref_t color, bool_t use_colorkey, const float_t alpha) {
			mc_size_t size = {w,h};  point_t pt = {};  blendfunction_t bl = {};   bl.SourceConstantAlpha = utilities::minmaxof<const byte_t>(0, byte_t(alpha * float_t(MAXUINT8)),MAXUINT8);
			return ::UpdateLayeredWindow(hwn, hdc_dest, NULL, &size, hdc_src, &pt, color, &bl, use_colorkey ? ULW_ALPHA | ULW_COLORKEY : ULW_ALPHA) != FALSE;
		}

		inline long_ptr_t get_window_long_ptr(hwnd_t hwn, int_t index) {
			return ::GetWindowLongPtrW(hwn, index);
		}

		inline lresult_t def_win_procedure(hwnd_t hwn, uint_t msg, wparam_t wparam, lparam_t lparam) {
			return ::DefWindowProcW(hwn, msg, wparam, lparam);
		}

		inline word_t register_class(wndclassexa_t* p) {
			return ::RegisterClassExA(p);
		}

		inline word_t register_class(wndclassexw_t* p) {
			return ::RegisterClassExW(p);
		}

		inline hwnd_t create_window_ex(dword_t arg1, const wchar_t* arg2, const wchar_t* arg3, dword_t arg4, int_t arg5, int_t arg6, int_t arg7, int_t arg8, hwnd_t arg9, hmenu_t arg10, hinstance_t arg11, void_t* arg12) {
			return ::CreateWindowExW(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12);
		}

		inline hwnd_t create_window_ex(dword_t arg1, const_char_t* arg2, const_char_t* arg3, dword_t arg4, int_t arg5, int_t arg6, int_t arg7, int_t arg8, hwnd_t arg9, hmenu_t arg10, hinstance_t arg11, void_t* arg12) {
			return ::CreateWindowExA(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12);
		}

		inline bool_t show_window(hwnd_t hwn, int_t cmd) {
			return ::ShowWindow(hwn , cmd);
		}

		inline bool_t update_window(hwnd_t hwn) {
			return ::UpdateWindow(hwn);
		}

		inline bool_t move_window(hwnd_t hwn, const coord_t x, const coord_t y, const coord_t w, const coord_t h) {
			return ::MoveWindow(hwn, (int_t)x, (int_t)y, (int_t)w, (int_t)h, TRUE) != FALSE;
		}

		inline bool_t set_window_pos(hwnd_t hwnd , hwnd_t insert_after, const coord_t x, const coord_t y, coord_t w, coord_t h, uint_t flags) {
			return ::SetWindowPos(hwnd, insert_after, x, y, w, h, flags) != FALSE;
		}

		inline void_t update_framemode(hwnd_t hwnd) {
			::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		}

		inline long_ptr_t set_window_long_ptr(hwnd_t hwn, const int_t nindex, const long_ptr_t lval) {
			return ::SetWindowLongPtrW(hwn, nindex, lval);
		}

		inline bool_t get_window_rect(hwnd_t hwn, mc_rect_t* rectptr) {
			return ::GetWindowRect(hwn, rectptr);
		}

		inline bool_t peek_message(msg_t* pm, hwnd_t hwn, uint_t command) {
			return ::PeekMessageW(pm, hwn, 0,0, command) > 0;
		}

		inline bool_t is_quit_message(msg_t ms) {
			return ms.message == WM_QUIT;
		}

		inline bool_t get_message(msg_t* pm, hwnd_t hwn) {
			return ::GetMessageW(pm, hwn, 0, 0) > 0;
		}

		inline bool_t translate_message(const msg_t* p) {
			return ::TranslateMessage(p);
		}

		inline lresult_t dispatch_message(const msg_t* p) {
			return ::DispatchMessageW(p);
		}

		inline mc_rect_t get_window_rect(hwnd_t hwn) {
			mc_rect_t rect;
			::GetWindowRect(hwn, &rect);
			return rect;
		}

		inline hwnd_t get_desktop_window() {
			return ::GetDesktopWindow();
		}

		inline hcursor_t load_cursor(hinstance_t hinst, const wchar_t* pw) {
			return ::LoadCursorW(hinst, pw);
		}

		inline hcursor_t set_cursor(hcursor_t cursor) {
			return ::SetCursor(cursor);
		}

		inline void_t post_quit_message(int_t exit_code) {
			::PostQuitMessage(exit_code);
		}

		inline point_t get_cursor_pos() {
			point_t p{};
			::GetCursorPos(&p);
			return p;
		}

		inline point_t screen_to_client(hwnd_t hwn, point_t p) {
			::ScreenToClient(hwn, &p);
			return p;
		}

		inline word_t get_key_state_wparam(wparam_t wparam) {
			return GET_KEYSTATE_WPARAM(wparam);
		}

		inline short_t get_wheel_delta_wparam(wparam_t wparam) {
			return GET_WHEEL_DELTA_WPARAM(wparam);
		}

		inline bool_t track_mouse_event(trackmousevent_t* p) {
			return TrackMouseEvent(p);
		}

		inline void_t post_messagew(hwnd_t hwn, uint_t msg, wparam_t wparam, lparam_t lparam) {
			::PostMessageW(hwn, msg, wparam, lparam);
		}

		inline dword_t get_display_frequency() {
			devmode_t devmode = {};
			devmode.dmSize = sizeof(devmode_t);
			devmode.dmDriverExtra = 0;

			if (EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &devmode) == 0) {
				return 0;
			}

			else {
				return devmode.dmDisplayFrequency;
			}
		}

		inline void_t sleep(dword_t ms) {
			::Sleep(ms);
		}

		//* gdiplus external function
		inline void_t set_text_rendering_hint_antialias(gdiplus_graphics_t* ptr) {
			ptr->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		}

		inline gdiplus_status_t rotate_trasform(gdiplus_graphics_t* ptr, const float_t alpha) {
			return (gdiplus_status_t) ptr->RotateTransform(alpha);
		}

		inline gdiplus_status_t draw_image(gdiplus_graphics_t* ptr, gdiplus_image_t* image, const gdiplus_real_t x, const gdiplus_real_t y, const gdiplus_real_t w, const gdiplus_real_t h) {
			return ptr->DrawImage(image,x,y,w,h);
		}

		inline gdiplus_status_t draw_image(gdiplus_graphics_t* ptr, gdiplus_image_t* image, const gdiplus_rectf_t& rect, const gdiplus_real_t x, const gdiplus_real_t y, const gdiplus_real_t w, const gdiplus_real_t h, const gdiplus_unit_t unit, gdiplus_image_attributes_t* pattr) {
			return ptr->DrawImage(image, rect, x, y, w, h, unit, pattr);
		}

		inline gdiplus_status_t draw_image(gdiplus_graphics_t* ptr, gdiplus_image_t* image, gdiplus_rect_t rect) {
			return ptr->DrawImage(image, rect);
		}

		inline gdiplus_status_t draw_image(gdiplus_graphics_t* ptr, gdiplus_image_t* image, const gdiplus_rect_t& rect, const int_t x, const int_t y, const int_t w, const int_t h, const gdiplus_unit_t unit, gdiplus_image_attributes_t* pattr) {
			return ptr->DrawImage(image, rect, x, y, w, h, unit, pattr);
		}

		inline gdiplus_status_t scale_trasform(gdiplus_graphics_t* ptr, const gdiplus_real_t sx, const gdiplus_real_t sy) {
			return ptr->ScaleTransform(sx, sy);
		}

		inline gdiplus_status_t reset_trasform(gdiplus_graphics_t* ptr) {
			return ptr->ResetTransform();
		}

		inline gdiplus_status_t gdiplus_startup(ulong_ptr_t* ptr, gdiplus_startup_input_t* pinput, gdiplus_startup_output_t* out) {
			return Gdiplus::GdiplusStartup(ptr, pinput, out);
		}
		
		inline void gdiplus_shutdown(ulong_ptr_t ptr) {
			Gdiplus::GdiplusShutdown(ptr);
		}
	}
}