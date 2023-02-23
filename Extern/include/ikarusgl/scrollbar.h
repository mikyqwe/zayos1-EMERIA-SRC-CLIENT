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
#pragma once
namespace ikgl
{
	class scrollbar : public window
	{

	public:
		typedef byte_t	scrollbar_status_t;
		typedef float_t scrollbar_value_t;
		
		//scrollbar
		typedef struct {
			button::button_cover_t button1;
			button::button_cover_t middle_button;
			button::button_cover_t button2;
		} TScrollbarCover;

	public:
		scrollbar(gui* guiptr);
		~scrollbar();

		/**
		* Function used to reset the scrollbar object.
		*/
		void_t clear();

		/**
		* Function used to run the callback event 'on_render' set on the scrollbar and to receive the rendering requesting answer from the widget and its children.
		* \return true if at least one of the children or the widget needed the rendering.
		*/
		bool_t on_render();

		/**
		* Function used to draw the widget. It's automatically used by #ikgl::gui objects. That allow you to draw the scrollbar in a customized rendering function
		* @param hdc is a HANDLE to a DC object. You can find more about DC object in Microsoft Documention
		* @param graphicsptr is a pointer to a Gdiplus::Graphics object. You can find more about Gdiplus::Graphics object in Microsoft Documention
		*/
		void_t draw( hdc_t& hdc,  gdiplus_graphics_t* graphicsptr);

		/**
		* Function used to set a 'on_scroll' callback to the scrollbar to be run when the position of the middle button will change.
		* @param event is the callback to be run.
		*/
		void_t set_on_scroll(const widget_event_t event);

		/**
		* Function used to run the 'on_scroll' callback of the scrollbar.
		*/
		void_t on_scroll();

		/**
		* Function used to get the value of the scrollbar. The value of the scrollbar is relative to the position of the middlebutton and it is always in the range 0.0 ~ 1.0
		* \return the value of the scrollbar
		*/
		scrollbar_value_t get_value() const ;

		/**
		* Function used to set the actual value of the scrollbar and to move the middle button. The value of the scrollbar is relative to the position of the middlebutton and it is always in the range 0.0 ~ 1.0
		* @param fValue is the value to set to the scrollbar.
		*/
		void_t set_value(const scrollbar_value_t fValue);

		/**
		* Function used to set the step value of the scrollbar used to calculate the newone value when the user press up/down buttons.
		* @param fStep is the value to set as step value.
		*/
		void_t set_step(const scrollbar_value_t fStep);

		/**
		* Function used to obtain the step value set on the scrollbar.
		* \return the step value set on the scrollbar.
		*/
		scrollbar_value_t get_step() const ;

		/**
		* Function used to obtain the status of the scrollbar. The status maybe one of these values : #ikgl::SCROLLBAR_STATUS_DEFAULT or #ikgl::SCROLLBAR_STATUS_PICKED
		* \return the status of the scrollbar
		*/
		scrollbar_status_t get_status() const ;

		/**
		* Function used to set the status of the scrollbar. The status maybe one of these values : #ikgl::SCROLLBAR_STATUS_DEFAULT or #ikgl::SCROLLBAR_STATUS_PICKED
		* @param status to set to the scrollbar
		*/
		void_t set_status(const scrollbar_status_t status);

		/**
		* Function used to set the size of the scrollbar. The function override the #ikgl::window::set_size function and will automatically positioning the buttons of the scrollbar.
		* @param size is the destination size to set to the scrollbar.
		*/
		void_t set_size(const widget_size_t& size);

		/**
		* Function used to set the size of the scrollbar. The function override the #ikgl::window::set_size function and will automatically positioning the buttons of the scrollbar.
		* @param size is the pointer to the existing object and destination size to set to the scrollbar.
		*/
		void_t set_size(const widget_size_t* size);

		/**
		* Function used to set the size of the scrollbar. The function override the #ikgl::window::set_size function and will automatically positioning the buttons of the scrollbar.
		* @param w is the destination width of the scrollbar.
		* @param h is the destination height of the scrollbar.
		*/
		void_t set_size(const coord_t w, const coord_t h);

		/**
		* Function used to set only the height of the scrollbar.
		* @param h is the destination height of the scrollbar.
		*/
		void_t set_height(const coord_t h);

		/**
		* Function used to set the middle button height of the scrollbar.
		* @param h is the destination height to set to the middle button of the scrollbar.
		*/
		void_t set_middle_button_height(const coord_t h);

		/**
		* Function that simulate the click on the up button.
		*/
		void_t on_click_button1();

		/**
		* Function that simulate the click on the down button.
		*/
		void_t on_click_button2();

		/**
		* Function used to set the offset between the middle button position and the mouse position to follow correctly the mouse when it pick the middle button.
		*/
		void_t set_picked_position(const widget_pos_t& p);

		/**
		* Function used to follow the mouse. It's automatically used by #ikgl::gui object
		* @param p is the position to follow
		*/
		void_t follow_mouse(const widget_pos_t& p);

		/**
		* Function used to get the instance of the up buttton of the scrollbar.
		* \return the instance of the up button of the scrollbar.
		*/
		ikgl::button* get_button1_ptr() const ;

		/**
		* Function used to get the instance of the down button of the scrollbar.
		* \return the instance of the down button of the scrollbar.
		*/
		ikgl::button* get_button2_ptr() const ;

		/**
		* Function used to get the instance of the middle button of the scrollbar.
		* \return the instance of the middle button of the scrollbar.
		*/
		ikgl::button* get_middle_button_ptr() const ;

		/**
		* Function used to return the space which the middle button may scroll.
		* \return the space which the middle button may scroll.
		*/
		scrollbar_value_t get_scrollable_length();

		/**
		* Function used to return statically the widget type.
		* \return return statically the widget type.
		*/
		static widget_type_t type() { return widget_type_t::scrollbar; }

		/**
		* Function used to return statically the empty name of the widget.
		* \return statically the empty name of the widget.
		*/
		static wstring_t		empty_name() { return L"NONAME_SCROLLBAR"; }
		

		/**
		* Override the function #ikgl::window::hide_children function to avoid to hide the buttons of the scrollbars (up, down, middle)
		*/
		void_t hide_children();

		/**
		* Override the function #ikgl::window::hide_children function to avoid to remove the buttons of the scrollbars (up, down, middle)
		*/
		void_t remove_children();	

		

	private:
		void_t __draw_background( hdc_t& hdc,  gdiplus_graphics_t* graphicsptr);
		void_t __refresh_middle_button_position();
		void_t __assign_button_events();
		void_t __create_default_button();
		void_t __clear_scrollbar();

		
	private:

		widget_event_t*			_on_scroll_event;
		scrollbar_status_t		_status;
		scrollbar_value_t		_value;
		scrollbar_value_t		_step;
		widget_pos_t			_picked_position;

		button* _button1;
		button* _button2;
		button* _middle_button;

		
	};
}
