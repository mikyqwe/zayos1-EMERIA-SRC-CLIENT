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
#ifndef __INCLUDE_GUI_TYPES_HEADER_IKARUSGL__
#define __INCLUDE_GUI_TYPES_HEADER_IKARUSGL__

//* decalring prototypes of 
//* utilities function
//* used into types methods

namespace ikgl {
	namespace utilities {
		template <class type>
		type maxof(const type v1, const type v2);

		template <class type>
		type minof(const type v1, const type v2);

		template <class type>
		type minmaxof(const type minval, const type v, const type maxval);

		template <class type>
		type moduleof(const type v);

		template <class type, class origin>
		type to(const origin val);
	}
}



namespace ikgl{
	typedef size_t 				 hash_name_t;
	typedef DWORD				 dword_t;
	typedef WORD				 word_t;
	typedef BYTE				 byte_t;
	typedef INT  int_t;
	typedef BOOL				 bool8_t;
	typedef bool				 bool_t;
	typedef short				 short_t;
	typedef unsigned short		 ushort_t;
	typedef unsigned long		 ulong_t;
	typedef long				 long_t;
	typedef char				 char_t;
	typedef unsigned char		 uchar_t;
	typedef char*				 char_ptr_t;
	typedef unsigned char*		 uchar_ptr_t;
	typedef double				 double_t;
	typedef long double			 ldouble_t;
	typedef long long			 longlong_t;
	typedef unsigned long long	 ulonglong_t;
	typedef float				 float_t;
	typedef const char			 const_char_t;
	typedef va_list				 valist_t;
	

	typedef long_t				 layer_level_t;
	typedef int_t				 widget_dict_key_t;
	typedef long_t				 coord_t;
	 
	typedef COLORREF			 mc_colorref_t;
	typedef RECT				 mc_rect_t;
	typedef SIZE				 mc_size_t;
	typedef HWND				 hwnd_t;
	typedef HINSTANCE			 hinstance_t;
	typedef HDC  hdc_t;
	typedef HMENU				 hmenu_t;
	typedef HBITMAP				 hbitmap_t;
	typedef HCURSOR				 hcursor_t;
	typedef WPARAM				 wparam_t;
	typedef LPARAM				 lparam_t;
	typedef MSG  msg_t;
	typedef UINT				 uint_t;
	typedef void				 void_t;
	typedef ULONG_PTR			 ulong_ptr_t;
	typedef LONG_PTR			 long_ptr_t;

	//* standard types
	typedef std::wstring		 wstring_t;
	typedef std::string			 string_t;
	typedef std::mutex			 mutex_t;
	typedef std::thread			 thread_t;
	typedef std::atomic<bool_t>  atomic_bool_t;
	typedef std::atomic<float_t> atomic_float_t;
	typedef std::atomic<word_t>  atomic_word_t;
	typedef std::atomic<int_t>   atomic_int_t;

	//* gdiplus typedefs to conform with non-maiusc words
	using namespace Gdiplus;
	typedef Graphics			 gdiplus_graphics_t;
	typedef GraphicsPath		 gdiplus_graphics_path_t;
	typedef RectF				 gdiplus_rectf_t;
	typedef Rect				 gdiplus_rect_t;
	typedef PointF 				 gdiplus_pointf_t;
	typedef Point				 gdiplus_point_t;
	typedef Image				 gdiplus_image_t;
	typedef Bitmap				 gdiplus_bitmap_t;
	typedef REAL				 gdiplus_real_t;
	typedef Color				 gdiplus_color_t;
	typedef PixelFormat			 gdiplus_pixel_format_t;
	typedef Font				 gdiplus_font_t;
	typedef Status				 gdiplus_status_t;
	typedef ImageAttributes		 gdiplus_image_attributes_t;
	typedef ColorMatrix			 gdiplus_color_matrix_t;
	typedef SolidBrush			 gdiplus_solid_brush_t;
	typedef Pen  gdiplus_pen_t;
	typedef StringFormat		 gdiplus_string_format_t;
	typedef ARGB				 gdiplus_argb_t;
	typedef INT  gdiplus_int_t;
	typedef Unit				 gdiplus_unit_t;
	typedef StringAlignment		 gdiplus_string_alignment_t;
	typedef FontFamily			 gdiplus_font_family_t;
	typedef StringTrimming		 gdiplus_string_trimming_t;
	typedef StringFormatFlags	 gdiplus_string_format_flag_t;
	typedef BitmapData			 gdiplus_bitmap_data_t;
	typedef GdiplusStartupInput	 gdiplus_startup_input_t;
	typedef GdiplusStartupOutput gdiplus_startup_output_t;

	typedef HANDLE				 handle_t;
	typedef HGLOBAL				 hglobal_t;
	typedef DEVMODEW			 devmode_t;
	typedef LONG_PTR			 long_ptr_t;
	typedef LRESULT				 lresult_t;
	typedef HRESULT				 hresult_t;
	typedef WNDCLASSEXW			 wndclassexw_t;
	typedef WNDCLASSEXA			 wndclassexa_t;
	typedef TRACKMOUSEEVENT		 trackmousevent_t;
	typedef POINT				 point_t;
	typedef BLENDFUNCTION		 blendfunction_t;
	typedef IStream				 istream_t;
	typedef APPBARDATA			 appbardata_t;
	typedef BITMAP				 bitmap_t;
	typedef LPMINMAXINFO		 lpminmaxinfo_t;
#ifdef __WIN32__
	typedef hresult_t			 wnd_procedure_result_t;
#else
	typedef lresult_t			 wnd_procedure_result_t;
#endif

	typedef std::unordered_map<wstring_t, ikgl::window*> 		window_name_map_t;
	typedef std::function<widget_event_return_t(window*)> 		widget_event_t;
	typedef std::function<void_t(window*)>  	render_event_t;
	typedef std::function<message_return_t(hwnd_t, uint_t, wparam_t, lparam_t)>		message_event_t;
	typedef std::function<double_t(double_t)>  loop_message_event_t;

	typedef std::function<widget_event_return_t(hdc_t&, gdiplus_graphics_t*, window*)> 	custom_render_t;
	
	/** Used to store the necessaries information to register a timer event into a widget. */
	typedef struct timer_event_s {
	private:
		static int_t new_timer_id() { static int_t id = 0; return id++; }

	public:
		widget_event_t event; /**< the event to be run when the timer is end.*/
		double_t ex_time; /**< the next timestamp to be reached up to run the event.*/
		double_t seconds; /**< the loop time to set the next timestamp after the run .*/
		int_t identifier; /**< the unique number of the timer .*/

		timer_event_s() : 
			ex_time(0.0), 
			seconds(0.0),
			identifier(new_timer_id())
		{}

	} timer_event_t;


	/** The structure used to register the messages handler to the #ikgl::gui object */
	typedef struct 
	{ 
		uint_t			message; /**< The message type to be handled.*/
		message_event_t	event; /**< The event to run to handle the message .*/
	} message_event_info_t;

	/** The structure used to store the information about a 'key message' to be handled */
	typedef struct {
		uint_t	 message; /**< The message code received.*/
		wparam_t wparam; /**< The wparam sent with the message. */
		lparam_t lparam; /**< The lparam sent with the message. */
	} key_message_info_t;

	/** A template structure used to manipulate differents types of position value.*/
	template <class Type>
	struct __template_pos
	{
		Type x;
		Type y;

		template <class Ot > 
		__template_pos<Type>( const __template_pos<Ot>& r) {
			this->x = static_cast<Type>(r.x);
			this->y = static_cast<Type>(r.y);
		}

		__template_pos<Type>() {
			this->x = static_cast<Type>(0);
			this->y = static_cast<Type>(0);
		}

		__template_pos<Type>( const __template_pos<Type>& r) {
			this->x = r.x;
			this->y = r.y;
		}

		template<class Ot>
		__template_pos<Type>(const Ot rx, const Ot ry) {
			this->x = static_cast<Type>(rx);
			this->y = static_cast<Type>(ry);
		}

		__template_pos<Type>(const Type rx, const Type ry) {
			this->x = rx;
			this->y = ry;
		}


		void_t operator +=(const __template_pos< Type >& p)	{
			this->x += p.x;
			this->y += p.y;
		}

		void_t operator -=(const __template_pos< Type >& p) {
			this->x -= p.x;
			this->y -= p.y;
		}

		__template_pos<Type> operator -(const __template_pos< Type >& p) const {
			return { this->x-p.x, this->y-p.y};
		}

		__template_pos<Type> operator +(const __template_pos< Type >& p) const {
			return { this->x + p.x, this->y + p.y };
		}

		bool_t operator == (const __template_pos< Type >& val) const {
			return this->x == val.x && this->y==val.y;
		}

		bool_t operator != (const __template_pos< Type >& val) const {
			return this->x != val.x || this->y != val.y;
		}

		gdiplus_point_t to_gdiplus_point() const {
			gdiplus_point_t t;
			t.X = (gdiplus_int_t)this->x;
			t.Y = (gdiplus_int_t)this->y;

			return t;
		}

		gdiplus_pointf_t to_gdiplus_pointf() const {
			gdiplus_pointf_t t;
			t.X = (gdiplus_real_t)this->x;
			t.Y = (gdiplus_real_t)this->y;

			return t;
		}

		template <class Ot>
		void move_x(const Ot dx) {
			this->x += static_cast<Type>(dx);
		}

		void move_x(const Type dx) {
			this->x += dx;
		}


		template <class Ot>
		void move_y(const Ot dy) {
			this->y += static_cast<Type>(dy);
		}

		void move_y(const Type dy) {
			this->y += dy;
		}
	};

	typedef __template_pos< coord_t > widget_pos_t; /**< The structure used to store the position information as coord_t */
	typedef __template_pos< float_t > widget_posf_t;/**< The structure used to store the position information as floa_t */


	/** A template structure used to manipulate different types of size */
	template <class Type>
	struct __template_size
	{
		Type width;
		Type height;

		__template_size<Type>(const __template_size< Type >& r){
			this->width	 =r.width;
			this->height =r.height;
		}

		__template_size<Type>() {
			this->width  = static_cast<Type>(0);
			this->height = static_cast<Type>(0);
		}

		__template_size<Type>(const Type w, const Type h){
			this->width	= w;
			this->height= h;
		}

		template <class Ot>
		__template_size<Type>(const __template_size< Ot >& r) {
			this->width  = static_cast<Type>(r.width);
			this->height = static_cast<Type>(r.height);
		}

		template <class Ot>
		__template_size<Type>(const Ot w, const Ot h) {
			this->width  = static_cast<Type>(w);
			this->height = static_cast<Type>(h);
		}

		bool_t operator == (const __template_size<Type>& val) {
			return width == val.width && height == val.height;
		}

		bool_t operator != (const __template_size<Type>& val) {
			return width != val.width || height != val.height;
		}

		void_t operator += (const __template_size<Type>& val) {
			this->width += val.width;
			this->height += val.height;
		}

		void_t operator -= (const __template_size<Type>& val) {
			this->width -= val.width;
			this->height -= val.height;
		}

		void change_width(const Type dw) {
			this->width += dw;
		}

		void change_height(const Type dh) {
			this->height += dh;
		}

		__template_size<Type> operator -(const __template_size<Type>& p) const {
			return { this->width - p.width, this->height - p.height };
		}

		__template_size<Type> operator +(const __template_size<Type>& p) const {
			return { this->width + p.width, this->height + p.height };
		}

		template <class Ot>
		void change_width(const Ot dw) {
			this->width += static_cast<Type>(dw);
		}

		template <class Ot>
		void change_height(const Ot dh) {
			this->height += static_cast<Type>(dh);
		}
	};


	using widget_size_t = __template_size<coord_t>; /**< The structure used to store the size information as coord_t */
	using widget_sizef_t = __template_size<float_t>; /**< The structure used to store the size information as float_t */

	typedef struct srect
	{
		coord_t left;
		coord_t top;
		coord_t right;
		coord_t bottom;


		srect( coord_t l , coord_t t, coord_t r, coord_t b) {
			this->left =l; top =t; right =r; bottom =b;
		}

		srect(const widget_pos_t& pos, const widget_size_t& size) {
			this->left   = pos.x;
			this->right  = pos.x + size.width;
			this->top    = pos.y;
			this->bottom = pos.y + size.height;
		}

		srect(const mc_rect_t& r) {
			this->left = r.left;
			this->right = r.right;
			this->top = r.top;
			this->bottom = r.bottom;
		}

		void_t to_mc_rect(mc_rect_t & rect) const {
			rect.top	= this->top;
			rect.left	= this->left;
			rect.right	= this->right;
			rect.bottom	= this->bottom;
		}

		gdiplus_rect_t to_gdiplus_rect() const {

			gdiplus_rect_t rect;
			rect.X = (gdiplus_int_t) this->left;
			rect.Y = (gdiplus_int_t) this->top;
			rect.Width = (gdiplus_int_t) (this->right - this->left);
			rect.Height= (gdiplus_int_t) (this->bottom- this->top);

			return rect;
		}

		gdiplus_rectf_t to_gdiplus_rectf() const {

			gdiplus_rectf_t rect;
			rect.X = (gdiplus_real_t)this->left;
			rect.Y = (gdiplus_real_t)this->top;
			rect.Width = (gdiplus_real_t)(this->right   - this->left);
			rect.Height = (gdiplus_real_t)(this->bottom - this->top);

			return rect;
		}

		bool_t intersect(const srect& r) {
			auto in_range = [](	const coord_t _m, 
								const coord_t _v, 
								const coord_t _M) {
				return _m <= _v && _v <= _M;
			};

			//* when at least one of the sides left or right is in the left/right range of this
			//* and at least one of the sides top or bottom is in the top/bottom range of this 
			return ((in_range(this->left, r.left, this->right)  || in_range(this->left, r.right,  this->right)) &&
				    (in_range(this->top,  r.top,  this->bottom) || in_range(this->top,  r.bottom, this->bottom)));
		}

	} widget_rect_t; /**< The structure used to store the rectangle information as coord_t */


	


	typedef struct {
		scroll_type_t type; /**< The scroll type */
		int_t times; /**< The scroll times  */
		ulong_t flag; /**< The flag to known the pressure on the keys (alt, shift, ctrl) */
	} mousewheel_data_t; /**< The structure used to store mousewheel message information */

	//textline
	typedef struct sfontinfo{
		wstring_t		font_family; /**< A wstring_t that contains the family name : e.g. L"Arial" */
		uint_t			size; /**< Font size */
		int_t			style; /**< Font style  */
		gdiplus_unit_t	unit; /**< unit */
	} font_t; /**< The structure used to store the font information */

	//for image and buttons
	typedef struct scolor_rgba {
		float_t r;
		float_t g;
		float_t b;
		float_t a;

		gdiplus_argb_t to_gdiplus_argb() const{
			return gdiplus_color_t::MakeARGB(
				utilities::minmaxof<byte_t>(0, utilities::to<byte_t>(a * MAXUINT8), MAXUINT8),
				utilities::minmaxof<byte_t>(0, utilities::to<byte_t>(r * MAXUINT8), MAXUINT8),
				utilities::minmaxof<byte_t>(0, utilities::to<byte_t>(g * MAXUINT8), MAXUINT8),
				utilities::minmaxof<byte_t>(0, utilities::to<byte_t>(b * MAXUINT8), MAXUINT8)
			);
		}

		mc_colorref_t to_colorref() const{
			return RGB(
				utilities::minmaxof<byte_t>(0, utilities::to<byte_t>(r * MAXUINT8), MAXUINT8),
				utilities::minmaxof<byte_t>(0, utilities::to<byte_t>(g * MAXUINT8), MAXUINT8),
				utilities::minmaxof<byte_t>(0, utilities::to<byte_t>(b * MAXUINT8), MAXUINT8)
			);
		}

		scolor_rgba(mc_colorref_t ref) {
			r = float_t(GetRValue(ref)) / float_t(MAXUINT8);
			g = float_t(GetGValue(ref)) / float_t(MAXUINT8);
			b = float_t(GetBValue(ref)) / float_t(MAXUINT8);
			a = 1.0f;
		}
		
		scolor_rgba(mc_colorref_t ref, float_t alpha) {
			r = float_t(GetRValue(ref)) / float_t(MAXUINT8);
			g = float_t(GetGValue(ref)) / float_t(MAXUINT8);
			b = float_t(GetBValue(ref)) / float_t(MAXUINT8);
			a = alpha;
		}

		scolor_rgba(float_t r_, float_t g_, float_t b_, float_t a_) {
			r = r_; g = g_, b = b_, a = a_;
		}

		scolor_rgba(const scolor_rgba& c) {
			a=c.a;	r=c.r;	g=c.g;	b=c.b;
		}

		scolor_rgba(const scolor_rgba& c, float_t alpha) {
			a=utilities::minmaxof(0.0f,c.a*alpha,1.0f);
			r=c.r;	g=c.g;	b=c.b;
		}


		scolor_rgba() {
			a= r= g= b= 0.f;
		}

	} color_rgba_t; /**< The structure used to store the color information (r,g,b,a) */

	const color_rgba_t TEXTLINE_DEFAULT_COLOR = {1.0f,1.0f,1.0f,1.0f};
	const color_rgba_t BUTTON_DEFAULT_DEFAULT_COLOR = {0.7f, 0.7f, 0.7f , 1.0f};
	const color_rgba_t BUTTON_DEFAULT_DOWN_COLOR = {0.9f, 0.9f, 0.9f , 1.0f};
	const color_rgba_t BUTTON_DEFAULT_OVER_COLOR = {0.3f, 0.3f, 0.3f , 1.0f};



	//for buttons
	typedef struct button_color_info_s
	{
		color_rgba_t	default_color; /**< default color to be drawed when the button is in state 'up' */
		color_rgba_t	over_color; /**< over color to be drawed when the button is in state 'over' */
		color_rgba_t	down_color; /**< down color to be drawed when the button is in state 'down' */

		color_rgba_t	border_color; /**< border color to be drawed if enabled */
		uint_t			border_width; /**< border width */


		void setup( const color_rgba_t& def, 
					const color_rgba_t& over, 
					const color_rgba_t& down) {
			this->default_color = def;
			this->over_color = over;
			this->down_color = down;

			this->border_color = {};
			this->border_width = 0;
		}

		void setup( const color_rgba_t& def,
					const color_rgba_t& over,
					const color_rgba_t& down,
					const color_rgba_t& border) {
			this->default_color = def;
			this->over_color = over;
			this->down_color = down;
			this->border_color = border;
			this->border_width = 1;
		}

		void setup( const color_rgba_t& def,
					const color_rgba_t& over,
					const color_rgba_t& down,
					const color_rgba_t& border,
					const uint_t border_width) {

			this->default_color = def;
			this->over_color = over;
			this->down_color = down;
			this->border_color = border;
			this->border_width = border_width;
		}

	} button_color_info_t; /**< The structure used to store the cover colors information */


	//for image
	typedef struct srect_scale
	{
		float_t top_scale, left_scale, bottom_scale, right_scale;

		widget_size_t get_size(const widget_size_t& size){

			coord_t horizontal_left	 = (coord_t) -(size.width * (left_scale-1.0f));
			coord_t horizontal_right	 = (coord_t)  (size.width * right_scale);

			coord_t horizontal_begin = utilities::minof(horizontal_left, horizontal_right);
			coord_t horizontal_end	 = utilities::maxof(horizontal_left, horizontal_right);

			coord_t vertical_top	 = (coord_t) -(size.height * (top_scale - 1.0f));
			coord_t vertical_bottom	 = (coord_t)  (size.height * (bottom_scale));

			coord_t vertical_begin	 = utilities::minof(vertical_top, vertical_bottom);
			coord_t vertical_end	 = utilities::maxof(vertical_top, vertical_bottom);

			return { horizontal_end - horizontal_begin , vertical_end - vertical_begin };
		}

	} rect_scale_t; /**< Structure used to store the recting scale information to use to truncate or to repeat an image when rendered. #ikgl::image::set_rect_scale */


	


	typedef struct sbackground_info
	{
		color_rgba_t bg_color; /**< The color to be rendered when the #ikgl::window::draw_background is called */
		color_rgba_t border_color;  /**< The border color to be rendered when the #ikgl::window::draw_background is called */
		
		sbackground_info(const sbackground_info& r) {
			bg_color = r.bg_color;
			border_color = r.border_color;
		}

		sbackground_info(const color_rgba_t& bg, const color_rgba_t& border) : bg_color(bg), border_color(border) {
		}
		
		sbackground_info(const color_rgba_t& bg) : bg_color(bg), border_color(bg) {
		}
			
		sbackground_info() {
		}

	} background_info_t; /**< Structure used to store the background information of a widget */



	//gui types
	typedef struct
	{
		wndclassexw_t		wc;
		dword_t				ex_style;
		const wchar_t*		window_name;
		dword_t				style;
		int_t				x;
		int_t				y;
		int_t				width;
		int_t				height;
		hwnd_t				parent;
		hmenu_t				menu;
		hinstance_t			instance;
		void_t*				lp_param;
	} create_gui_infow_t;  /**< Structure used to store the necessaries information to create an application window  */


	typedef  struct
	{
		wndclassexa_t		wc;
		dword_t				ex_style;
		const_char_t*		window_name;
		dword_t				style;
		int_t				x;
		int_t				y;
		int_t				width;
		int_t				height;
		hwnd_t				parent;
		hmenu_t				menu;
		hinstance_t			instance;
		void_t*				lp_param;
	} create_gui_infoa_t; /**< Structure used to store the necessaries information to create an application window  */
}



#endif //__INCLUDE_GUI_TYPES_HEADER_IKARUSGL__
