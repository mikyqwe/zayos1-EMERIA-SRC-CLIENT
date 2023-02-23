#pragma once

namespace ikgl {
	class input : public text {
	public:
		input(gui* guiptr);
		~input();

		/**
		* Function used to draw the widget. It's automatically used by #ikgl::gui objects. That allow you to draw the input in a customized rendering function
		* @param hdc is a HANDLE to a DC object. You can find more about DC object in Microsoft Documention
		* @param graphicsptr is a pointer to a Gdiplus::Graphics object. You can find more about Gdiplus::Graphics object in Microsoft Documention
		*/
		void_t draw(hdc_t& hdc,  gdiplus_graphics_t* graphicsptr);

		/**
		* Function used to disable the secret flag. Secret flag transform the text in '*' usefull for the passwords/pins input.
		*/
		void_t disable_secret();

		/**
		* Function used to receive statically the empty name of this class
		* \return statically the empty name of this class
		*/
		static wstring_t empty_name() { return L"NONAME_INPUT"; }

		/**
		* Function used to enable the secret flag. Secret flag transform the text in '*' usefull for the passwords/pins input.
		*/
		void_t enable_secret();

		/**
		* Function used to obtain the input limit set to the input object. You can set the input limit by using #ikgl::input::set_input_limit .
		* \return the input limit set to the input object
		*/
		dword_t get_input_limit();

		/**
		* Function used to know the value of the secret flag. Secret flag transform the text in '*' usefull for the passwords/pins input.
		* \return the value of the secret flag
		*/
		bool_t is_secret();

		/**
		* Function used to run the 'on_key_down' event to the widget.
		* @param message_info is the information needed by the event to be run (key code/message code and parameters) 
		*/
		void_t on_key_down(const key_message_info_t& message_info);

		/**
		* Function used to run the 'on_mouseleft_down' event to the widget. It's an override of the window class #ikgl::window::on_mouseleft_down .
		*/
		void_t on_mouseleft_down();

		/**
		* Function used to set the cursor position. The cursor position is the actual place to write into the input.
		* @param pos is the position to set as cursor position.
		*/
		void_t set_cursor_pos(const int_t pos);

		/**
		* Function used to set the input limit to the input object. You can obtain the actual value by using #ikgl::input::get_input_limit .
		* @param limit is the limit to set as maximum characters to write into the input box.
		*/
		void_t set_input_limit(dword_t limit);

		/**
		* Function used to set default text to be shown when the input box is empty and is not focused.
		* @param default_text is the text to set as default.
		*/
		void_t set_default_text(const wstring_t& default_text);

		/**
		* Function used to set default text to be shown when the input box is empty and is not focused.
		* @param default_text is the text to set as default.
		*/
		void_t set_default_text(const string_t& default_text);

		/**
		* Function used to set as focused the widget. The focus on the input make the used to be able to write into it. You can use #ikgl::window::kill_focus to kill the focus on the widget.
		*/
		void_t set_focus();

		/**
		* Function used to return statically the widget type of this class.
		* \return statically the widget type of this class.
		*/
		static widget_type_t type() { return widget_type_t::input; }

	private:
		void_t __adjust_cursor_position();

		void_t __draw_cursor(hdc_t& hdc,  gdiplus_graphics_t* graphicsptr);
		void_t __draw_secret_text(hdc_t& hdc,  gdiplus_graphics_t* graphicsptr);
		void_t __draw_default_text(hdc_t& hdc,  gdiplus_graphics_t* graphicsptr);

		void_t __on_char_down(wchar_t key);
		void_t __on_key_down(wparam_t wparam);

		void_t __on_move_cursor(int_t pos);
		void_t __on_delete();
		void_t __on_backspace();
		void_t __on_cancel();
		void_t __on_escape();

		void_t __toogle_cursor();
		
	private:
		int_t _cursor_position;
		bool_t _is_show_cursor;
		bool_t _is_secret_enabled;
		dword_t _input_limit;
		wstring_t _default_text;
	};
}

