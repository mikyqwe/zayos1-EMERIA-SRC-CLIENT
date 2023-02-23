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
#ifndef __INCLUDE_HEADER_TEXTLINE_IKARUSGL__
#define __INCLUDE_HEADER_TEXTLINE_IKARUSGL__

namespace ikgl
{
	/**
	* Function used to make a default font information.
	* @param rFont is the reference to the font information to be set.
	*/
	void_t make_default_font(font_t& rFont);
	font_t make_font(wstring_t family, uint_t size, int_t style = Gdiplus::FontStyleRegular, gdiplus_unit_t unit = Gdiplus::UnitPixel);
	font_t make_font_easy(wstring_t family, uint_t size);


	class text : public window
	{
	public:
		text(gui* guiptr);
		~text();

		/**
		* Function used to run the callback event 'on_render' set on the text and to receive the rendering requesting answer from the widget and its children.
		* \return true if at least one of the children or the widget needed the rendering.
		*/
		bool_t on_render();

		/**
		* Function used to draw the widget. It's automatically used by #ikgl::gui objects. That allow you to draw the text in a customized rendering function
		* @param hdc is a HANDLE to a DC object. You can find more about DC object in Microsoft Documention
		* @param graphicsptr is a pointer to a Gdiplus::Graphics object. You can find more about Gdiplus::Graphics object in Microsoft Documention
		*/
		virtual void_t draw(hdc_t& hdc,  gdiplus_graphics_t* graphicsptr);

		/**
		* Function used to reset the text object.
		*/
		void_t clear();

		/**
		* Function used to set the text to be shown by the widget.
		* @param text is the text to be shown
		* @param resize if true will resize the widget to fit the text 
		*/
		void_t set_text(wstring_t text, bool resize=false);

		/**
		* Function used to set the text to be shown by the widget.
		* @param text is the text to be shown
		* @param resize if true will resize the widget to fit the text
		*/
		void_t set_text(string_t  text, bool resize = false);

		/**
		* Function used to draw the text. It's automatically used by #ikgl::gui objects. That allow you to draw the text in a customized rendering function
		* @param hdc is a HANDLE to a DC object. You can find more about DC object in Microsoft Documention
		* @param graphicsptr is a pointer to a Gdiplus::Graphics object. You can find more about Gdiplus::Graphics object in Microsoft Documention
		*/
		void_t draw_text(hdc_t& hdc,  gdiplus_graphics_t* graphicsptr);

		/**
		* Function used to obtain the text set to the text widget. This overload will return a #ikgl::wstring_t . It's a bit more efficient than the overload that it return a string_t because it doesn't need to convert it.
		* \return the text set to the text widget. 
		*/
		wstring_t get_textw();

		/**
		* Function used to obtain the text set to the text widget. This overload will return a #ikgl::string_t . It's a bit less efficient than the overload that it return a wstring_t because it doesn't need to convert it.
		* \return the text set to the text widget.
		*/
		string_t get_text();

		/**
		* Function used to obtain the text set to the text widget. This overload will copy the bytes into am array of char. It's a bit less efficient than the overload that it return a wstring_t because it doesn't need to convert it.
		* @param szBuff is a pointer to the first element of the char array.
		* @param dwMaxLen is the length of the char array.
		*/
		void_t get_text(char_ptr_t szBuff, dword_t dwMaxLen);

		/**
		* Function used to set a font to the text widget.
		* @param fontInfo is a pointer to the #ikgl::font_t object that it contains the information about the font.
		*/
		void_t set_font(const font_t* fontInfo);

		/**
		* Function used to set a font to the text widget.
		* @param fontInfo is a reference to the #ikgl::font_t object that it contains the information about the font.
		*/
		void_t set_font(const font_t& fontInfo);

		/**
		* Function used to obtain the font set on the text widget.
		* @param r_out is the output of the function that will receive the font information.
		*/
		void_t get_font(font_t& r_out);

		/**
		* Function used to obtain the font set on the text widget.
		* \return a constant reference to the member of the widget.
		*/
		const font_t& get_font();

		/**
		* Function used to obtain the color set to the text widget.
		* \return a constant reference to the member of the widget.
		*/
		const color_rgba_t& get_color() const;

		/**
		* Function used to set a color to the text widget.
		* @param rColor is the color to set to the widget.
		*/
		void_t set_color(const color_rgba_t& rColor);

		/**
		* Function used to set the desired horizontal alignment to the text.
		* @param halign is a #ikgl::text_halign_t value to set as horizontal alignment.
		*/
		void_t set_horizontal_alignment(text_halign_t halign);

		/**
		* Function used to set the desired vertical alignment to the text.
		* @param valign is a #ikgl::text_valign_t value to set as horizontal alignment.
		*/
		void_t set_vertical_alignment(text_valign_t valign);

		/**
		* Function used to set a callback to run when the text will change.
		* @param event is a callback to set as 'on_change_text' event.
		*/
		void_t set_on_change_text(widget_event_t event);

		/**
		* Function used to set the size of the text.
		* @param size is the size to set to the text
		*/
		void_t set_text_size(const coord_t size);

		/**
		* Function used to run the callback set as 'on_change_text' event.
		*/
		void_t on_change_text();

		/**
		* Function used to obtain the text size. The value obtained from this function is calculated during the run of the function, so it's recommended to use it to store the result rather than use it many times.
		*/
		widget_size_t get_text_size();
		
		/**
		* Function used to obtain statically the widget type of the class.
		* \return statically the widget type of the class.
		*/
	    static widget_type_t type() { return widget_type_t::textline; }

		/**
		* Function used to obtain statically the empty name of the class.
		* \return statically the empty name of the class.
		*/
		static wstring_t empty_name() { return L"NONAME_TEXT"; }


	protected:
		void_t __create_default_font();
		void_t __clear_text();
		void_t __normalize_by_alignment(gdiplus_rect_t& rect, const wstring_t& txt = L"" );
		void_t __normalize_by_alignment(widget_pos_t& pos, const wstring_t& txt = L"");

	protected:
		wstring_t _ws_text;
		font_t _font_info;
		color_rgba_t _color;
		widget_event_t* _on_change_text_event;
		text_halign_t	_string_horizontal_align;
		text_valign_t	_string_vertical_align;
		gdiplus_font_t* _font;
	};
}

#endif __INCLUDE_HEADER_TEXTLINE_IKARUSGL__
