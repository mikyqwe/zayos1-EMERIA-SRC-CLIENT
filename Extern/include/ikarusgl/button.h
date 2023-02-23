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
#ifndef __INCLUDE_HEADER_BUTTON_IKARUSGL__
#define __INCLUDE_HEADER_BUTTON_IKARUSGL__

namespace ikgl
{
	class button : public window
	{
	public:
		typedef struct {
			gdiplus_image_t* default_image;
			gdiplus_image_t* over_image;
			gdiplus_image_t* down_image;
		} button_cover_t;

	public:
		button(gui* guiptr);
		~button();

		/**
		* Function used to reset the object before to use again it as newone. It's recommended to avoid the new use of old widget if not really needed. It's recommended to remove it from its parent and to create a newone.
		*/
		void_t clear();
		
		/**
		* Function used to draw the widget. It's automatically used from gui object. The use of this function is allowed to be used just in "custom" drawing methods.
		* @param hdc is an HANDLE to a DC object. (You can find information about DC in Microsoft Documentation )
		* @param graphicsptr is a pointer to a Gdiplus::Graphics object. (You can find information about Gdiplus::Graphics in Microsoft Documentation )
		*/
		void_t draw(hdc_t& hdc,  gdiplus_graphics_t* graphicsptr);

		/**
		* Function used to remove the roundered border. The roundered button are only available by using the colors drawing by using #set_colors and not by using #set_default_cover and similar. You can enable again it by using #enable_round .
		*/
		void_t disable_round();

		/**
		* Function used to draw the cover of the button. Drawing the cover include the draw of the set colors.
		* @param hdc is an HANDLE to a DC object. (You can find information about DC in Microsoft Documentation )
		* @param graphicsptr is a pointer to a Gdiplus::Graphics object. (You can find information about Gdiplus::Graphics in Microsoft Documentation )
		*/
		void_t draw_cover(hdc_t& hdc, gdiplus_graphics_t* graphicsptr);

		/**
		* Function used to disable the rendering of the borders of the button. You can use #enable_border to enable it again.
		*/
		void_t disable_border();

		/**
		* Function used to enable the rendering of the borders of the button. You can use #disable_border to disable it again.
		*/
		void_t enable_border();

		/**
		* Function used to enable the roundered borders of the button. You can use the roundered border just if the button doesnt have any default/over/down cover. You can use the #disable_round to disable.
		*/
		void_t enable_round();

		/**
		* Function used to return statically the empty name of the object.
		* \return this function returns statically the empty name of the object.
		*/
		static wstring_t empty_name() { return L"NONAME_BUTTON"; }

		/**
		* Function used to hide all the children of the widget. It's an override of the window class method. It's necessary to override the window class method to avoid to hide the text of the button implemented as a child of the widget.
		*/
		void_t hide_children();

		/**
		* Function used to obtain the text of the button as a wstring. It's more efficient than #get_text because it doesn't need to convert the text from wstring to string.
		* \return this function returns the text of the button as a wstring.
		*/
		wstring_t get_textw();

		/**
		* Function used to obtain the text of the button as a string. It's less efficient than #get_textw because it needs to convert the text from wstring to string.
		* \return this function returns the text of the button as a string.
		*/
		string_t get_text();

		/**
		* Function used to obtain the text of the button into the buffer. It's less efficient than #get_textw because it needs to convert the text from wstring to string.
		* @param szBuff a pointer to a char array.
		* @param dwLen the length of the array.
		*/
		void_t get_text(char_ptr_t szBuff, dword_t dwLen);

		/**
		* Function used to obtain the colors set to the button.
		* @param pinfo is a pointer to an existing #button_color_info_t object that will contain the output colors set.
		*/
		void_t get_colors(button_color_info_t* pinfo);

		/**
		* Function used to obtain the text instance of the button. This function may return nullptr when is used on a button that it doesn't have set any text.
		* \return this function returns the text instance of the button. This function may return nullptr when is used on a button that it doesn't have set any text.
		*/
		ikgl::text* get_text_instance();
		
		/**
		* Function used to know if the button is set on state "over". The button states are up, down, over.
		* \return this function returns true if the state of the button is "over".
		*/
		bool_t is_over();

		/**
		* Function used to know if the button is set on state "up". The button states are up, down, over.
		* \return this function returns true if the state of the button is "up".
		*/
		bool_t is_up();

		/**
		* Function used to know if the button is set on state "down". The button states are up, down, over.
		* \return this function returns true if the state of the button is "down".
		*/
		bool_t is_down();

		/**
		* Function used to know if the button will use the cover images or the colors to be rendered.
		* \return this function returns true if the buttons owns cover images.
		*/
		bool_t is_covered();
		
		/**
		* Function used to call the 'on render' call back and to make the checks about rendering. This function is automatically used from gui object. It's a public method to allow the use of it in a 'custom' draw function.
		* \return this function returns true when the widget or one or more of its children need to be rendered.
		*/
		bool_t on_render();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mouseleft_up to set  the callback). It's an override of the window class method.
		*/
		void_t on_mouseleft_up();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mouseleft_down to set  the callback). It's an override of the window class method.
		*/
		void_t on_mouseleft_down();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mouseover_in to set  the callback). It's an override of the window class method.
		*/
		void_t on_mouseover_in();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mouseover_out to set  the callback). It's an override of the window class method.
		*/
		void_t on_mouseover_out();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mousemove to set  the callback). It's an override of the window class method.
		*/
		void_t on_mousemove();

		/**
		* Function used to destroy all children and remove them from the list. It's an override of the window class method necessary to don't remove the text instance implemented as a child of the button.
		*/
		void_t remove_children();

		/**
		* Function used to set the size of the widget. You can obtain the actual widget size by using #get_size . It's an override of the window class method necessary to resize the text instance implemented as a child of the button.
		* @param size is the pointer to an object of type widget_size_t that exists and will contain the value of the size to set to the widget.
		*/
		void_t set_size(widget_size_t* size);

		/**
		* Function used to set the size of the widget. You can obtain the actual widget size by using #get_size . It's an override of the window class method necessary to resize the text instance implemented as a child of the button.
		* @param size a reference to an object of type widget_size_t that exists and it will contain the value of the size to set to the widget.
		*/
		void_t set_size(widget_size_t& size);

		/**
		* Function used to set the size of the widget. There are some types of widget that override this mathod. You can obtain the actual widget size by using #get_size . It's an override of the window class method necessary to resize the text instance implemented as a child of the button.
		* @param w is the value of width to set to the widget.
		* @param h is the value of height to set to the widget.
		*/
		void_t set_size(coord_t w, coord_t h);

		/**
		* Function used to set a text to the button. This method allow a text instance to show the text into the button. This text instance is not pickable and it is implemented as a child of the button.
		* @param text is the wstring that contains the text to set.
		*/
		void_t set_text(wstring_t text);

		/**
		* Function used to set a text to the button. This method allow a text instance to show the text into the button. This text instance is not pickable and it is implemented as a child of the button.
		* @param text is the string that contains the text to set.
		*/
		void_t set_text(string_t text);

		/**
		* Function used to set a text to the button. This method allow a text instance to show the text into the button. This text instance is not pickable and it is implemented as a child of the button.
		* @param text a pointer to an array of chars.
		* @param dwLen is the string length.
		*/
		void_t set_text(const_char_t* text, dword_t dwLen);

		/**
		* Function used to set an image as 'default' cover of the button. The default cover is the image to be rendered while the button is in the status 'up'.
		* @param path is the path to the file to be open.
		* \return this function returns true if the image is loaded successfully.
		*/
		bool_t set_default_cover(const wstring_t path);

		/**
		* Function used to set an image as 'default' cover of the button. The default cover is the image to be rendered while the button is in the status 'up'.
		* @param path is the path to the file to be open.
		* \return this function returns true if the image is loaded successfully.
		*/
		bool_t set_default_cover(const string_t path);

		/**
		* Function used to set an image as 'default' cover of the button. The default cover is the image to be rendered while the button is in the status 'up'.
		* @param pData is a pointer to the buffer that contains the image bytes.
		* @param len is the len of the buffer.
		* \return this function returns true if the image is loaded successfully.
		*/
		bool_t set_default_cover(const void_t* pData, dword_t len);

		/**
		* Function used to set the colors to be shown when the button is rendered. The colors are used just when the button doesn't have a cover.
		* @param pinfo a pointer to the button_color_info_t that exists and it contains the colors to be set.
		*/
		void_t set_colors(button_color_info_t* pinfo);

		/**
		* Function used to set a text color to the text instance. This method could be used before or after #ikgl::button::set_text method .
		* @param color is a COLORREF object that contain the color to be set. (You can find more about COLORREF in Microsoft's Documentation )
		*/
		void_t set_text_color(mc_colorref_t color);

		/**
		* Function used to set a callback to be run when the user click on the button.
		* @param event is the event to be used. To automatically unset the callback after the use you can return ikgl::widget_event_return_t::UNSET_CALLBACK by the event.
		*/
		void_t set_on_click_event(widget_event_t event);

		/**
		* Function used to set an image as 'down' cover of the button. The down cover is the image to be rendered while the button is in the status 'down'.
		* @param path is the path to the file to be open.
		* \return this function returns true if the image is loaded successfully.
		*/
		bool_t set_down_cover(const wstring_t path);

		/**
		* Function used to set an image as 'down' cover of the button. The down cover is the image to be rendered while the button is in the status 'down'.
		* @param path is the path to the file to be open.
		* \return this function returns true if the image is loaded successfully.
		*/
		bool_t set_down_cover(const string_t path);

		/**
		* Function used to set an image as 'down' cover of the button. The down cover is the image to be rendered while the button is in the status 'down'.
		* @param pData is a pointer to the buffer that contains the image bytes.
		* @param len is the len of the buffer.
		* \return this function returns true if the image is loaded successfully.
		*/
		bool_t set_down_cover(const void_t* pData, dword_t len);

		/**
		* Function used to set an image as 'over' cover of the button. The over cover is the image to be rendered while the button is in the status 'over'.
		* @param path is the path to the file to be open.
		* \return this function returns true if the image is loaded successfully.
		*/
		bool_t set_over_cover(const wstring_t path);

		/**
		* Function used to set an image as 'over' cover of the button. The over cover is the image to be rendered while the button is in the status 'over'.
		* @param path is the path to the file to be open.
		* \return this function returns true if the image is loaded successfully.
		*/
		bool_t set_over_cover(const string_t path);

		/**
		* Function used to set an image as 'over' cover of the button. The over cover is the image to be rendered while the button is in the status 'over'.
		* @param pData is a pointer to the buffer that contains the image bytes.
		* @param len is the len of the buffer.
		* \return this function returns true if the image is loaded successfully.
		*/
		bool_t set_over_cover(const void_t* pData, dword_t len);

		/**
		* Function used to set a tooltip to the button to be shown then the mouse is over the button.
		* @param text is the text to be shown in the tooltip.
		* @param font is the font of the text to be shown in the tooltip.
		* @param colorText is the color of the text to be shown in the tooltip.
		* @param colorBackground is the background color of the rectangle of the tooltip.
		*/
		void_t set_tooltip(const string_t& text, const font_t& font, const color_rgba_t& colorText, const color_rgba_t& colorBackground);
		
		/**
		* Function used to set a tooltip to the button to be shown then the mouse is over the button.
		* @param text is the text to be shown in the tooltip.
		* @param font is the font of the text to be shown in the tooltip.
		* @param colorText is the color of the text to be shown in the tooltip.
		* @param colorBackground is the background color of the rectangle of the tooltip.
		*/
		void_t set_tooltip(const wstring_t& text, const font_t& font, const color_rgba_t& colorText, const color_rgba_t& colorBackground);
		
		/**
		* Function used to set a tooltip to the button to be shown then the mouse is over the button.
		* @param text is the text to be shown in the tooltip.
		* @param font is the font of the text to be shown in the tooltip.
		* @param colorText is the color of the text to be shown in the tooltip.
		* @param colorBackground is the background color of the rectangle of the tooltip.
		* @param pos is the relative position to set to the tooltip.
		*/
		void_t set_tooltip(const wstring_t& text, const font_t& font, const color_rgba_t& colorText, const color_rgba_t& colorBackground, const widget_pos_t& pos);

		/**
		* Function used to return statically the type of the widget.
		* \return this function returns statically the type of the widget.
		*/
		static widget_type_t type() { return widget_type_t::button; }

	private:
		/**
		* Function used to reset the button class members
		*/
		void_t __clear_button();

	private:
		button_color_info_t _colors;
		color_rgba_t _text_color;
		button_cover_t _cover;
		widget_event_t* _on_click_event;
		button_status_t _button_status;
		text* _textline;
		bool_t _is_use_border;
		bool_t _is_use_round;
		bool_t _is_run_click_event;
		window* _tooltip;
	};
}


#endif //__INCLUDE_HEADER_BUTTON_IKARUSGL__

