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
#ifndef __INCLUDE_GUI_CONSTANTS_HEADER_IKARUSGL__
#define __INCLUDE_GUI_CONSTANTS_HEADER_IKARUSGL__


namespace ikgl
{
	/**
	* Enumeration used to describe the two kind of return value for a #ikgl::widget_event_t function
	*/
	enum class widget_event_return_t {
		UNSET_CALLBACK = false, /**< Value false will automatically unset the callback */
		KEEP_CALLBACK = true, /**< Value true keep the callback set to the widget */
	};

	/**
	* Enumeration used to describe the two kind of return value for a #ikgl::message_event_t function
	*/
	enum class message_return_t {
		NO_PREVENT_DEFAULT = false, /**< Value false will not interrupt the message procedure */
		PREVENT_DEFAULT = true, /**< Value true will automatically return from the message procedure and prevet the default message procedure */
	};

	/**
	* Enumeration used to declare the gui object constants
	*/
	enum eGuiConstants { 
		DEFAULT_FPS = 15, /**< default value of MAX fps (frame per second) to set to a gui object when it's created (15 ) */
		DEFAULT_UPS = 30, /**< default value of ups (update per second) to set to a gui object when it's created (30 ) */
		DEFAULT_ALPHA_COLOR = RGB(255,0,255), /**< default value of transparent color to be used when using a layered window ( RGB(255,0,255) ) */
	};

	/**
	* Enumeration used to declare the default 'z' axis. They are not used at the moment.
	*/
	enum eConstLayer{
		LAYER_TYPE_TOP_MOST		= -2,
		LAYER_TYPE_UNDER_TOP	= -1,
		LAYER_TYPE_INVALID		=  0,
		LAYER_TYPE_START		=  1,
	};

	/**
	* Enumeration used to declare varius widget type indexes.
	*/
	enum class widget_type_t {
		window			= 0, /**< type index for window objects */
		textline		= 1, /**< type index for textline objects */
		button			= 2, /**< type index for button objects */
		image			= 3, /**< type index for image objects */
		scrollbar		= 4, /**< type index for scrollbar objects */
		animation		= 5, /**< type index for animation objects */
		input			= 7, /**< type index for input objects */
		box				= 8, /**< type index for box objects */
		selector		= 9, /**< type index for __template_selector objects */
	};

	/**
	* Enumeration used to declare the type of horizontal alignment
	*/
	enum class text_halign_t {
		TEXT_HALIGN_CENTER = 0, /**< value for horizontal alignment centered */
		TEXT_HALIGN_LEFT   = 1, /**< value for horizontal alignment to the left */
		TEXT_HALIGN_RIGHT  = 2, /**< value for horizontal alignment to the right */
	};

	/**
	* Enumeration used to declare the type of horizontal alignment
	*/
	enum class text_valign_t {
		TEXT_VALIGN_CENTER  = 0, /**< value for vertical alignment to the center */
		TEXT_VALIGN_TOP     = 1, /**< value for vertical alignment to the top */
		TEXT_VALIGN_BOTTOM  = 2, /**< value for vertical alignment to the bottom */
	};



	
	/**
	* Enumeration used to declare the type of button status
	*/
	enum class button_status_t{
		BUTTON_STATUS_DEFAULT	= 0,  /**< Value for status default (status up)  */
		BUTTON_STATUS_OVER		= 1,  /**< Value for status over  */
		BUTTON_STATUS_DOWN		= 2,  /**< Value for status down  */
	};
	
	
	/**
	* Enumeration used to declare invalid id
	*/
	enum eConstWidget{
		IKGL_WIDGET_INVALID_ID	= -1, /** Value to be set as starting value for the id of the widgets */
	};

	/**
	* Enumeration used to declare the default values for the scrollbar objects
	*/
	enum eConstScrollBar{
		SCROLLBAR_DEFAULT_HEIGHT				= 100,/** The default height to be set to the scrollbars when initialized */
		SCROLLBAR_DEFAULT_WIDTH					= 20,/** The default width to be set to the scrollbars when initialized */
		SCROLLBAR_DEFAULT_MIDDLEBUTTON_HEIGHT	= 30,/** The default middle button height to be set to the scrollbars when initialized */
		SCROLLBAR_DEFAULT_STEP_BUTTON_HEIGHT	= 20,/** The defaultstep button height to be set to the scrollbars when initialized */

		SCROLLBAR_STATUS_DEFAULT				= 0,/** Value for status default */
		SCROLLBAR_STATUS_PICKED					= 1,/** Value for status picked */

		SCROLLBAR_DEFAULT_BACKGROUND_COLOR		= RGB(110,110,110),/** The default background color to be set to the scrollbars when initialized */
		SCROLLBAR_DEFAULT_BUTTON_DEFAULT_COLOR	= RGB(180,180,180),/** The default button default color to be set to the scrollbars when initialized */
		SCROLLBAR_DEFAULT_BUTTON_OVER_COLOR		= RGB(230,230,230),/** The default button over color to be set to the scrollbars when initialized */
		SCROLLBAR_DEFAULT_BUTTON_DOWN_COLOR		= RGB(160,160,160),/** The default button down color to be set to the scrollbars when initialized */

	};

	/** The default value for step movement for scrollbar */
	constexpr float SCROLLBAR_DEFAULT_STEP = 0.0333f;


	/** The scroll types for the mouse wheel event information stored in the widget */
	enum class scroll_type_t {
		SCROLL_TYPE_UP, /**< Scroll type Up */
		SCROLL_TYPE_DOWN,/**< Scroll type Down */
	};

	/** Enumeration used for the scroll button pressed flags */
	enum class scroll_down_button_t {
		SCROLL_CTRL_DOWN  = 1<<0, /** Value for ctrl down */
		SCROLL_ALT_DOWN   = 1<<1, /** Value for alt down */
		SCROLL_SHIFT_DOWN = 1<<2, /** Value for shift down */
	}; 


	//* animated image (animation)
	constexpr float ANIMATION_DEFAULT_DELAY = 0.1f;
	
	//* input constants
	constexpr DWORD INPUT_LIMIT_DEFAULT = (DWORD)-1;

}



#endif //__INCLUDE_GUI_CONSTANTS_HEADER_IKARUSGL__
