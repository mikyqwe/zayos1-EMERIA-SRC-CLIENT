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
#ifndef __INCLUDE_GUI_INCLUDE_HEADER__IKARUSGL__
#define __INCLUDE_GUI_INCLUDE_HEADER__IKARUSGL__
#include "stdafx.h"

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif //_CRT_SECURE_NO_WARNINGS


#define WIN32_LEAN_AND_MEAN             // Escludere gli elementi usati raramente dalle intestazioni di Windows


#define __ikgl_set_bit(flag, bitn)		ikgl::utilities::set_bit(flag, bitn) /**< Basically used to set the binary flag*/
#define __ikgl_unset_bit(flag, bitn)	ikgl::utilities::unset_bit(flag,bitn)  /**< Basically used to unset the binary flag*/
#define __ikgl_isset_bit(flag, bitn)	ikgl::utilities::isset_bit(flag,bitn)  /**< Basically used to check the binary flag*/
#define __ikgl_toogle_bit(flag, bitn)	ikgl::utilities::toogle_bit(flag,bitn)  /**< Basically used to toogle the binary flag*/
//#define __TEST_PERFORMANCE
//#define __ALLOC_TEST_CONSOLE

namespace ikgl{
	class window;
	class gui;
}

/*
GLOBAL FUNC POINTER USED TO DEBUG/DEBUGW THE LIBRARY
YOU CAN CHOICE TO USE OR NOT THIS WAY TO DEBUG
*/


#ifdef _DEBUG
#	define __ikgl_debug(...) ikgl::utilities::debug(__VA_ARGS__)  /**< Basically used to print information into the console or to run the customized method of the debug settable by using #ikgl::utilities::set_debug_func */
#	define __ikgl_debugW(...) ikgl::utilities::debugW(__VA_ARGS__)  /**< Basically used to print information into the console or to run the customized method of the debug settable by using #ikgl::utilities::set_debug_func */
#else
#	define __ikgl_debug(...) 
#	define __ikgl_debugW(...) 
#endif

#define __ikgl_error(...) ikgl::utilities::error(__VA_ARGS__)  /**< Basically used to print errors into the console or to run the customized method of the errors settable by using #ikgl::utilities::set_error_func */
#define __ikgl_errorW(...) ikgl::utilities::errorW(__VA_ARGS__) /**< Basically used to print errors into the console or to run the customized method of the errors settable by using #ikgl::utilities::set_error_func */

//widget event
#define __ikgl_wevent_func_type				ikgl::widget_event_return_t(kgl::window* widget)  /**< A Short tricks to declare the functype  */
#define __ikgl_wevent_lambda_type			[](ikgl::window* widget) -> ikgl::widget_event_return_t  /**< A Short tricks to declare the lambda type  */
#define __ikgl_wevent_lambda_type_func		(ikgl::window* widget) -> ikgl::widget_event_return_t  /**< A Short tricks to declare the functype without [] */
#define __ikgl_wevent_declare(name)			ikgl::widget_event_return_t name(ikgl::window* widget) /**< A Short tricks to declare the functions to be used as widget_event_t */
#define __ikgl_wevent_lambda(expression)	__ikgl_wevent_lambda_type expression /**< A Short tricks to declare the lambdas to be used as widget_event_t */


#define __ikgl_custom_render_declare(name)	ikgl::widget_event_return_t name(ikgl::hdc_t& hdc, ikgl::gdiplus_graphics_t* graphicsptr, ikgl::window* widgetptr) /**< A Short tricks to declare the functions to be used as widget_event_t */

#define __ikgl_to_window_pointer(pObj) (static_cast<ikgl::window*>(pObj)) /**< A Short tricks to cast the widget to the window instance */


//message dispatch
#define __ikgl_message_event(funcname)			ikgl::message_return_t funcname(ikgl::hwnd_t hwnd, ikgl::uint_t message, ikgl::wparam_t wparam, ikgl::lparam_t lparam)  /**< A Short tricks to declare message events */
#define __ikgl_message_lambda(exp)				(ikgl::hwnd_t hwnd, ikgl::uint_t message, ikgl::wparam_t wparam, ikgl::lparam_t lparam)->ikgl::message_return_t exp /**< A Short tricks to declare message events */

#define __ikgl_loop_message_event(funcname)		ikgl::double_t funcname(ikgl::double_t uiDeltaTime) /**< A Short tricks to declare message events */
#define __ikgl_loop_message_lambda(exp)			(ikgl::double_t uiDeltaTime)->double_t exp /**< A Short tricks to declare message events */


//general define
#include "constants.h"
#include "types.h"



namespace ikgl{
	extern mutex_t _g_global_mutex_debug;
	extern std::function<void_t(const_char_t*, valist_t)> IKGL_global_debug;
	extern std::function<void_t(const wchar_t*, valist_t)> IKGL_global_debugW;

	extern std::function<void_t(const_char_t*, valist_t)> IKGL_global_error;
	extern std::function<void_t(const wchar_t*, valist_t)> IKGL_global_errorW;
}

#include "external.h"
#include "utilities.h"
#include "bench_mark.h"
#include "draw.h"

//widget
#include "window.h"
#include "box.h"
#include "text.h"
#include "input.h"
#include "button.h"
#include "image.h"
#include "scrollbar.h"
#include "animation.h"
#include "selector.h"
#include "layer.h"
#include "gui.h"


#endif //__INCLUDE_GUI_INCLUDE_HEADER__IKARUSGL__
