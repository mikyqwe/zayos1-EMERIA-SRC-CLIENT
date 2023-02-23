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
#ifndef __HEADER_INCLUDE_windowS_IKARUSGL__
#define __HEADER_INCLUDE_windowS_IKARUSGL__


namespace ikgl
{
	/**
	* Function used to obtain the next value of id (by using a static int_t value)
	* \return return the next value of id (by using a static int_t value)
	*/
	int_t alloc_id();


	/**
	* Class used to inherit all types of widgets and which contains the common information that any widget needs to have (position, size, callbacks, parent, ...) .
	* Can also be used as a container of widgets or a Rectangle by setting the background color
	*/
	class window
	{
	public:
		typedef std::deque<window*> children_deque_t;
		

	public:

		/**
		* Function used to allocate the new type T widget and set basic information (guiptr, parent pointer, id, widget type)
		* \return the pointer to the T widget created.
		*/
		template<class T>
		T* create_element() {
			auto obj = new T(this->_gui);
			obj->set_widget_type(T::type());
			obj->set_id(alloc_id());
			add_child(obj);
			return obj;
		}


		/**
		* Function used to allocate the new type T widget and set basic information (guiptr, parent pointer, id, widget type)
		* @param t used to contain the result pointer of the created widget 
		*/
		template <class T>
		void_t create_element(T*& t) {
			t = new T(this->_gui);
			t->set_widget_type(T::type());
			t->set_id(alloc_id());
			add_child(t);
		}


		
		window(gui* guiptr);
		virtual ~window();

		/**
		* Function used to add the child to the childrenlist and to set the parent to the child.
		* @param pchild is a pointer to the child object
		*/
		virtual void_t add_child(window* pchild);

		/**
		* Function used to add a timer callback
		* @param seconds is the amount of seconds to delay the call of the callback 
		* @param eventTimer is the event to call every <seconds> seconds. The return of this function is of type #widget_event_return_t and it is used to know if the timer is to be removed or not after the call of the callback.
		*/
		virtual void_t add_timer(const double_t seconds, widget_event_t eventTimer);

		/**
		* Function used to call the rendering of this widget at the next application rendering cycle
		*/
		virtual void_t call_render();

		/**
		* Function used to reset the object (for example if you want to reuse it as a new object but without deleting it from memory)
		*/
		virtual void_t clear();

		/**
		* Function used to make the object unclickable and ignored by mouse in and mouse out events. Opposite method to #enable_pick
		*/
		virtual void_t disable_pick();

		/**
		* Function used to draw the widget (used internally by the gui object but also usable if you are using a custom rendering method)
		* @param hdc is and handle to a DC object (Microsoft docs contains the information about the DC/HDC)
		* @param graphicsptr is a pointer to a gdiplus graphic object used to draw the widget (Micosoft docs contains the information about Gdiplus::Graphics)
		*/
		virtual void_t draw(hdc_t& hdc, gdiplus_graphics_t* graphicsptr);

		/**
		* Function used to draw the widget background (used internallyby by draw function but usefull for use it in a custom rendering method)
		* @param hdc is and handle to a DC object (Microsoft docs contains the information about the DC/HDC)
		* @param graphicsptr is a pointer to a gdiplus graphic object used to draw the widget background (Micosoft docs contains the information about Gdiplus::Graphics)
		*/
		virtual void_t draw_background(hdc_t& hdc, gdiplus_graphics_t* graphicsptr);

		/**
		* Function used to draw the widget children (used internallyby by draw function but usefull for use it in a custom rendering method)
		* @param hdc is and handle to a DC object (Microsoft docs contains the information about the DC/HDC)
		* @param graphicsptr is a pointer to a gdiplus graphic object used to draw the widget children (Micosoft docs contains the information about Gdiplus::Graphics)
		*/
		virtual void_t draw_children(hdc_t& hdc, gdiplus_graphics_t* graphicsptr);


		/**
		* Function used to obtain the default name (of an unnamed widget)
		* \return the function return a wstring_t object contains the default name of the widget
		*/
		static wstring_t empty_name() { return L"NONAME_WINDOW"; }

		/**
		* Function used to make the object clickable again and not ignored by mouse events. Opposite method to #disable_pick
		*/
		virtual void_t enable_pick();

		/**
		* Function used to remove a child from the childrenlist without deleting it from the heap.
		* @param child the child pointer to be erased from the childrenlist
		*/
		virtual void_t erase_child(window* child);


		/**
		* Function used to initiate a gradual disappearance or reappearance of the widget which occurs through a gradual increase or decrease in the alpha value of the object.
		* @param alpha is the value of alpha to obtain at the end of the translation
		* @param seconds is the duration of the translation in seconds (e.g. 0.12) the precision of this value depend on the FPS set on gui object (default value #DEFAULT_FPS ) 
		* @param eventFade is the callback to set as "end transition event" as we do in jquery transitions. ( Optional argument )
		*/
		virtual void_t fade_to(const float_t alpha, const float_t seconds, widget_event_t* eventFade = nullptr);


		/**
		* Function used obtain the alpha factor set on the widget.
		* \return the alpha factor set on the widget.
		*/
		virtual float_t get_alpha() const;

		/**
		* Function used to obtain the pointer fo the child that it has the desidered id. The return may be nullptr.
		* @param iID is used to known the unique id of the desired child to get.
		* \return this function returns pointer to the window widget (that it is convertible using #widget_cast) that it got the iID argument as widget_id. The function may return nullptr if it is not found.
		*/
		virtual window* get_child(const int_t iID) const;

		/**
		* Function used to obtain the pointer fo the child that it has the desidered id. The return may be nullptr.
		* @param iID [in] is used to known the unique id of the desired child to get.
		* @param pWin  will contain pointer to the window widget (that it is convertible using #widget_cast) that it got the iID argument as widget_id. The pointer may contains nullptr if it is not found.
		*/
		virtual bool_t get_child(const int_t iID, window** pWin) const;

		/**
		* Function used to obtain a reference to the childrenlist container.
		* \return this function returns a reference to the childrenlist container.
		*/
		virtual children_deque_t& get_children_ref();

		/**
		* Function used to obtain the absolute value of the alpha factor set on this widget (and its parents three).
		* \return the absolute alpha factor set to this widget and its parents ( return this->alpha * this->parent->get_global_alpha() )
		*/
		virtual float_t get_global_alpha() const;

		/**
		* Function used to obtain the global position of the widget in the application.
		* \return this function returns the global position of the widget in the application.
		*/
		virtual widget_pos_t get_global_position() const;

		/**
		* Function used to obtain the global position of the widget in the application.
		* @param r_pos  will contain the global position of the widget in the application.
		*/
		virtual void_t get_global_position(widget_pos_t& r_pos) const;

		/**
		* Function used to obtain the global position of the widget in the application.
		* @param x  will contain the global x position of the widget in the application.
		* @param y  will contain the global y position of the widget in the application.
		*/
		virtual void_t get_global_position(coord_t& x, coord_t& y) const;

		/**
		* Function used to get a widget_rect_t object that contains the rectangle position of the widget in the application.
		* \return this function returns a widget_rect_t object that contains the rectangle position of the widget in the application.
		*/
		virtual widget_rect_t get_global_rect() const;

		/**
		* Function used to get a widget_rect_t object that contains the rectangle position of the widget in the application.
		* @param t_rect  will contain the rectangle position of the widget in the application.
		*/
		virtual void_t get_global_rect(widget_rect_t& t_rect) const;

		/**
		* Function used to obtain the rectangle position of the widget in the application.
		* @param left  will contain the left coordinate of the rectangle
		* @param top  will contain the top coordinate of the rectangle
		* @param right  will contain the right coordinate of the rectangle
		* @param bottom  will contain the out coordinate of the rectangle
		*/
		virtual void_t get_global_rect(coord_t& left, coord_t& top, coord_t& right, coord_t& bottom) const;

		/**
		* Function used to obtain the absolute angle of rotation of the widget relative to the angle considered 0 where the top and left sides of the position rectangle of the widget are parallel to the x and y axis of the coordinates respectively.
		* \return this function returns the absolute angle of rotation of the widget relative to the angle considered 0 where the top and left sides of the position rectangle of the widget are parallel to the x and y axis of the coordinates respectively.
		*/
		virtual float_t get_global_rotation() const;

		/**
		* Function used to obtain the pointer to the gui object owner of the widget
		* \return this function returns  the pointer to the gui object owner of the widget
		*/
		virtual gui* get_gui() const;

		/**
		* Function used to obtain the pointer to the gui object owner of the widget
		* @param guiptr  will contain the address to the object gui that own the widget
		*/
		virtual bool_t get_gui(gui* guiptr) const;

		/**
		* Function used to get the unique id of the widget
		* \return this function returns the unique id of the widget
		*/
		virtual int_t get_id() const;

		/**
		* Function used to obtain information about the mousewhell event used to the widget. The information are correct only if you are sure that the event has just occurred on the object. (e.g. if you record an on_mousewheel callback or if you record an event to the WM_MOUSEWHEEL message )
		* \return this function returns information about the mousewhell event used to the widget. The information are correct only if you are sure that the event has just occurred on the object. (e.g. if you record an on_mousewheel callback or if you record an event to the WM_MOUSEWHEEL message )
		*/
		virtual const mousewheel_data_t& get_mousewheel_data() const;

		/**
		* Function used to obtain information about the last key event applied to the widget. The information are correct only if you are sure that the event has just occurred on the object. (e.g. if you record an on_key_down/on_key_up callback or if you record an event to the WM_KEYDOWN message or similar messages )
		* \return this function returns information about the last key event applied to the widget. The information are correct only if you are sure that the event has just occurred on the object. (e.g. if you record an on_key_down/on_key_up callback or if you record an event to the WM_KEYDOWN message or similar messages )
		*/
		virtual const key_message_info_t& get_key_message_info() const;

		/**
		* Function actually unused (in future it will be used to know the Z axis of the widget) 
		* \return in future it will be used to know the Z axis of the widget, actually it is unused.
		*/
		virtual layer_level_t get_layer_level() const;

		/**
		* Function used to obtain the parent pointer of the widget. it may return nullptr if the widget is a root widget inserted to a layer directly.
		* \return this function returns the parent pointer of the widget. it may return nullptr if the widget is a root widget inserted to a layer directly.
		*/
		virtual window* get_parent() const;

		/**
		* Function used to obtain the picked widget pointer. By using a position as input this function will return the child picked in that position.
		* @param pos contain the position to be used for checking the "picked" object
		* \return this function returns the picked widget pointer. It may return this pointer if the position is in the rectangle of the widget and there arent any child that surrounds the position.
		*/
		virtual window* get_picked(const widget_pos_t& pos);

		/**
		* Function used to obtain the local position of the child. Local position and global position may be the same value if the widget is a root (it doesnt have a parent)
		* @param r_pos will contain the local position of the child.
		*/
		virtual void_t get_position(widget_pos_t& r_pos) const;

		/**
		* Function used to obtain the local position of the child. Local position and global position may be the same value if the widget is a root (it doesnt have a parent)
		* @param x will contain the x value of the local widget position.
		* @param y will contain the y value of the local widget position.
		*/
		virtual void_t get_position(coord_t& x, coord_t& y) const;

		/**
		* Function used to obtain the local position of the child. Local position and global position may be the same value if the widget is a root (it doesnt have a parent)
		* \return an object of type #widget_pos_t that contain the local position of the widget.
		*/
		virtual widget_pos_t get_position() const;

		/**
		* Function used to obtain the local x value of the position of the widget.
		* \return this function returns the local x value of the position of the widget.
		*/
		virtual coord_t get_x() const;

		/**
		* Function used to obtain the local y value of the position of the widget.
		* \return this function returns the local y value of the position of the widget.
		*/
		virtual coord_t get_y() const;

		/**
		* Function used to obtain the width value of thesize of the widget.
		* \return this function returns the width value of thesize of the widget.
		*/
		virtual coord_t get_width() const;

		/**
		* Function used to obtain the height value of thesize of the widget.
		* \return this function returns the height value of thesize of the widget.
		*/
		virtual coord_t get_height() const;

		/**
		* Function used to obain the size of the widget (the size of the rectangle)
		* @param r_size will contain the size of the widget
		*/
		virtual void_t get_size(widget_size_t& r_size) const;

		/**
		* Function used to obain the size of the widget (the size of the rectangle)
		* @param w will contain the width of size of the widget
		* @param h will contain the height of the size size of the widget
		*/
		virtual void_t get_size(coord_t& w, coord_t& h) const;

		/**
		* Function used to obain the size of the widget (the size of the rectangle)
		* \return this function returns a #widget_size_t object that contain the size of the widget.
		*/
		virtual const widget_size_t& get_size() const;

		/**
		* Function used to obtain the rectangle position of the widget (left, top, right, bottom)
		* @param rect will contain the rectangle position of the widget
		*/
		virtual void_t get_rect(widget_rect_t& rect) const;

		/**
		* Function used to obtain the rectangle position of the widget (left, top, right, bottom)
		* @param rect will contain the rectangle position of the widget into a RECT object (Micosoft Docs explain what is it)
		*/
		virtual void_t get_rect(mc_rect_t& rect) const;

		/**
		* Function used to obtain the rectangle position of the widget (left, top, right, bottom)
		* \return this function returns a #widget_rect_t object that contains the rectangle position of the object
		*/
		virtual widget_rect_t get_rect() const;

		/**
		* Function used to obtain the local rotation (relative to the parent) set to the widget in degrees.
		* \return this function returns the local rotation (relative to the parent) set to the widget in degrees.
		*/
		virtual float_t get_rotation() const;

		/**
		* Function used to obtain the widget type (you can known the widget type using a pointer to a window widget to cast the pointer to the inherited object). It is possible to obtain the widget type statically from the type using type::type() (e.g. button::type())
		* \return this function returns the type of the object ( one of the values in the enumeration #widget_type_t
		*/
		virtual widget_type_t get_widget_type() const;

		/**
		* Function used to obtain the name set to the window as wstring_t (it could return an empty string if the name is not set) The name in the widget object is saved as a wstring, for this reason is more efficient to use #set_window_name and #get_window_name by passing/receiving wstring_t
		* \return this function returns the name of the window set using #set_window_name as wstring_t.
		*/
		virtual wstring_t get_window_name() const;

		/**
		* Function used to obtain the name set to the window as string_t (it could return an empty string if the name is not set). The name in the widget object is saved as a wstring, for this reason is more efficient to use #set_window_name and #get_window_name by passing/receiving wstring_t
		* \return this function returns the name of the window set using #set_window_name as string_t.
		*/
		virtual string_t get_window_name_str() const;

		/**
		* Function used to know if the input position is contained in the rectangle of the widget.
		* @param pos is the input position to exams.
		* \return this function returns true if the input position is contained in the rectangle of the widget.
		*/
		virtual bool_t is_in(const widget_pos_t& pos) const;

		/**
		* Function used to know if the input position is contained in the rectangle of the widget.
		* @param x is the input x coordinate of the position to exams.
		* @param y is the input y coordinate of the position to exams.
		* \return this function returns true if the input position is contained in the rectangle of the widget.
		*/
		virtual bool_t is_in(const coord_t x, const coord_t y) const;

		/**
		* Function used to know if the widget type of this object is equal to the argument passed.
		* @param type is the input type to compare.
		* \return this function returns true if the input type is equal to this object's type.
		*/
		virtual bool_t is_widget_type(const widget_type_t type) const;

		/**
		* Function used to know if the widget is focused. (The widget is focused when it is the last clicked by default, and It's possible to manage the state by using #kill_focus and #set_focus )
		* \return this function returns true if the window is focused.
		*/
		virtual bool_t is_focused() const;

		/**
		* Function used to know if the widget will be redraw at the next cycle of the rendering thread.
		* \return this function returns true if the widget will be redraw at the next cycle of the rendering thread.
		*/
		virtual bool_t is_to_render() const;

		/**
		* Function used to know if the widget is shown. (It's possible to hide or show the widget by using #hide or #show )
		* \return this function returns true if the widget is shown.
		*/
		virtual bool_t is_shown() const;

		/**
		* Function used to know if the pick action is available on this widget. (it is possible to enable and disable it by using #enable_pick and #disable_pick )
		* \return this function returns true if the widget is pickable.
		*/
		virtual bool_t is_pickable() const;

		/**
		* Function used to know if the widget owns the child as argument.
		* @param pchild is a pointer to the object that is to exams.
		* \return this function returns true if the pchild is found in the childrenlist
		*/
		virtual bool_t has_child(const window* pchild) const;

		/**
		* Function used to hide the widget. (It's possible to know the state (hidden, shown) of the widget by using #is_shown )
		*/
		virtual void_t hide();

		/**
		* Function used to apply the #hide to every child own by the widget. (The function doesn't hide this widget)
		*/
		virtual void_t hide_children();

		/**
		* Function used to kill the focus on the widget. (It's possible to set the focus on the widget by using #set_focus, and It's possible to know the state by using #is_focused )
		*/
		virtual void_t kill_focus();
		
		/**
		* Function used to move the widget on the horizontal axis. You can pass as argument a negative or a positive value.
		* @param x contain the desired offset to move the widget.
		*/
		virtual void_t move_x(const coord_t x);

		/**
		* Function used to move the widget on the vertical axis. You can pass as argument a negative or a positive value.
		* @param y contain the desired offset to move the widget.
		*/
		virtual void_t move_y(const coord_t y);

		/**
		* Function used to move the widget on the horizontal axis on the left. You can pass as argument a negative value to move on the right.
		* @param val contains the value to be subtract to the actual x coordinates.
		*/
		virtual void_t move_left(const coord_t val);

		/**
		* Function used to move the widget on the horizontal axis on the right. You can pass as argument a negative value to move on the left.
		* @param val contains the value to be sum to the actual x coordinates.
		*/
		virtual void_t move_right(const coord_t val);

		/**
		* Function used to move the widget on the vertical axis on the top. You can pass as argument a negative value to move on the bottom.
		* @param val contains the value to be subtract to the actual y coordinates.
		*/
		virtual void_t move_top(const coord_t val);

		/**
		* Function used to move the widget on the vertical axis on the bottom. You can pass as argument a negative value to move on the top.
		* @param val contains the value to be sum to the actual y coordinates.
		*/
		virtual void_t move_bottom(const coord_t val);

		/**
		* Function used to align left side of the widget to the left side of the parent rectangle (It set 0 as local x coordinate)
		*/
		virtual void_t move_to_left();

		/**
		* Function used to align right side of the widget to the right side of the parent rectangle (It set <parent_width> - <child_width> as local x coordinate)
		*/
		virtual void_t move_to_right();

		/**
		* Function used to align top side of the widget to the top side of the parent rectangle (It set 0 as local y coordinate)
		*/
		virtual void_t move_to_top();

		/**
		* Function used to align bottom side of the widget to the bottom side of the parent rectangle (It set <parent_height> - <child_height> as local y coordinate)
		*/
		virtual void_t move_to_bottom();

		/**
		* vertically align the widget with its parent by matching the two centers.
		*/
		virtual void_t move_vertical_center();

		/**
		* horizontally align the widget with its parent by matching the two centers.
		*/
		virtual void_t move_horizontal_center();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mouseleft_up to set  the callback)
		*/
		virtual void_t on_mouseleft_up();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mouseleft_down to set  the callback)
		*/
		virtual void_t on_mouseleft_down();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mouseright_up to set  the callback)
		*/
		virtual void_t on_mouseright_up();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mouseright_down to set  the callback)
		*/
		virtual void_t on_mouseright_down();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mousemove to set  the callback)
		*/
		virtual void_t on_mousemove();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_resize_gui to set  the callback)
		*/
		virtual void_t on_resize_gui();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_redraw to set  the callback)
		*/
		virtual void_t on_redraw();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mousewheel to set  the callback)
		*/
		virtual void_t on_mousewheel(const mousewheel_data_t& data);

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_key_down to set  the callback)
		*/
		virtual void_t on_key_down(const key_message_info_t& message_info);

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_key_up to set  the callback)
		*/
		virtual void_t on_key_up(const key_message_info_t& message_info);

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_hide to set  the callback)
		*/
		virtual void_t on_hide();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_show to set  the callback)
		*/
		virtual void_t on_show();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_resize to set  the callback)
		*/
		virtual void_t on_resize();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_move to set  the callback)
		*/
		virtual void_t on_move();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_set_focus to set  the callback)
		*/
		virtual void_t on_set_focus();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_kill_focus to set  the callback)
		*/
		virtual void_t on_kill_focus();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mouseover_in to set  the callback)
		*/
		virtual void_t on_mouseover_in();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_mouseover_out to set  the callback)
		*/
		virtual void_t on_mouseover_out();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_move_parent to set  the callback)
		*/
		virtual void_t on_move_parent();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_resize_parent to set  the callback)
		*/
		virtual void_t on_resize_parent();

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_rotate_parent to set  the callback)
		*/
		virtual void_t on_rotate_parent();
		
		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_rotate_parent to set  the callback)
		*/
		virtual void_t on_custom_render(hdc_t& hdc, gdiplus_graphics_t* graphicsptr);

		/**
		* Function used to start the callback set on the specific event. (It's possible to use #set_on_render to set  the callback)
		*/
		virtual bool_t on_render();

		/**
		* Function used to destroy all children and remove them from the list.
		*/
		virtual void_t remove_children();

		/**
		* Function used to destroy a child and remove it from the list
		* @param pChild is the pointer to the child
		*/
		virtual void_t remove_child(const window* pChild);

		/**
		* Function used to rotate the widget. The rotation will be applied to all the children. This function doesn't set the rotation value, it applies the value to the actual rotation value. (e.g. if the rotation is 30degrees and rotate function is used by passing 30 degreees the result rotation value is 60 degrees)
		* @param degree is the value of degrees to rotate the widget.
		*/
		virtual void_t rotate(const float_t degree);

		/**
		* Function used to set the gui pointer that owns the widget.
		* @param guiptr is the pointer to an existing gui object that own the widget.
		*/
		virtual void_t set_gui(gui* guiptr);

		/**
		* Function used to set the name of the window. The name of the window have to be unique to use it in #ikgl::gui::get_window_by_name function. You can obtain the name of a window by using #get_window_name. Because of the name is saved internally of the widget as a wstring_t It is more efficient to use the overload of this function by passing a wstring_t object if possible.
		* @param name to set to the window.
		*/
		virtual void_t set_window_name(const wstring_t name);

		/**
		* Function used to set the name of the window. The name of the window have to be unique to use it in #ikgl::gui::get_window_by_name function. You can obtain the name of a window by using #get_window_name. Because of the name is saved internally of the widget as a wstring_t It is more efficient to use the overload of this function by passing a wstring_t object if possible.
		* @param name to set to the window.
		*/
		virtual void_t set_window_name(const string_t name);

		/**
		* Function used to set the background information of the widget. Every kind of widgets can use background that fit the widget rectangle positions when drawn down.
		* @param info is a #background_info_t that contain the information to set as background to this widget. (Color, border color, border width)
		*/
		virtual void_t set_background(const background_info_t* info);

		/**
		* Function used to set the background information of the widget. Every kind of widgets can use background that fit the widget rectangle positions when drawn down.
		* @param info is a #background_info_t that contain the information to set as background to this widget. (Color, border color, border width)
		*/
		virtual void_t set_background(const background_info_t& info);

		/**
		* Function used to set the value of degrees you want to see the widget rotated. You can use #rotate to apply a supplementary rotation of <val> degrees if you want directly 'set' the value.
		* @param degree is the value of degrees to set as rotation to the widget.
		*/
		virtual void_t set_rotation(const float_t degree);

		/**
		* Function used to set the id to the widget. The ID is a unique item to use for find the widget when u lose the reference by using #ikgl::gui::get_widget_by_id .
		* @param id is the unique value to set as id to the widget. The #create_element function set to the child the unique id by itself, but if you want to set an unique custom id you can use negative values for that.
		*/
		virtual void_t set_id(const int_t id);

		/**
		* Function unused at the moment. It will set the Z axis to the widget.
		* @param layer is the Z coordinate to set to the widget. (actually it is not used) 
		*/
		virtual void_t set_layer_level(const layer_level_t layer);

		/**
		* Function used to set the widget type to the widget. Consider to don't use it if you really don't know what this change could set off.
		* @param type is the type to set.
		*/
		virtual void_t set_widget_type(const widget_type_t type);

		/**
		* Function used to set the LOCAL position of the widget. You can obtain the actual position of the widget by using #get_position .
		* @param pos is the position to set to the widget.
		*/
		virtual void_t set_position(const widget_pos_t& pos);

		/**
		* Function used to set the LOCAL position of the widget. You can obtain the actual position of the widget by using #get_position .
		* @param pos is a pointer to the position to set to the widget.
		*/
		virtual void_t set_position(const widget_pos_t* pos);

		/**
		* Function used to set the LOCAL position of the widget. You can obtain the actual position of the widget by using #get_position .
		* @param x contain the x coordinate to set to the widget
		* @param y contain the y coordinate to set to the widget
		*/
		virtual void_t set_position(const coord_t x, const coord_t y);

		/**
		* Function used to set the size of the widget. There are some types of widget that override this mathod. You can obtain the actual widget size by using #get_size
		* @param size is the pointer to an object of type widget_size_t that exists and will contain the value of the size to set to the widget.
		*/
		virtual void_t set_size(const widget_size_t* size);

		/**
		* Function used to set the size of the widget. There are some types of widget that override this mathod. You can obtain the actual widget size by using #get_size
		* @param size is an object of type widget_size_t that it will contain the value of the size to set to the widget.
		*/
		virtual void_t set_size(const widget_size_t& size);

		/**
		* Function used to set the size of the widget. There are some types of widget that override this mathod. You can obtain the actual widget size by using #get_size
		* @param w will contain the width to set to the widget.
		* @param h will contain the height to set to the widget.
		*/
		virtual void_t set_size(const coord_t w, const coord_t h);

		/**
		* Function used to set the alpha factor to apply to the widget and the children when are them rendered
		* @param alpha is the value to set.
		*/
		virtual void_t set_alpha(const float_t alpha);

		/**
		* Function used to set the parent of the widget. It's recommended to use #create_element to set automatically parent and other options to the widget.
		* @param parent is the pointer to the widget parent to set to the widget.
		*/
		virtual void_t set_parent(window* parent);

		/**
		* Function used to move at the begin the child into the list. This mean it will be rendered as last child (so the overlayed widget)
		*/
		virtual void_t set_top();

		/**
		* Function used to set the child as focused. The focused child will receive the message from the keyboard. You can use #kill_focus to make the widget unfocused or #is_focused to known the state.
		*/
		virtual void_t set_focus();

		/**
		* Function used to set the child (passed as argument) to the top in the childrenlist. This make the child to overlay the other children.
		* @param child is a pointer to the child to set as top.
		*/
		virtual void_t set_child_top(window* child);

		/**
		* Function used to set a callback to the event 'mouse left button up'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more. The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_mouseleft_up(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'mouse left button down'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more. The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_mouseleft_down(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'mouse right button up'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more. The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_mouseright_up(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'mouse move'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_mousemove(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'resize gui'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_resize_gui(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'mouse right button down'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_mouseright_down(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'redraw' that it is different to 'on render'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_redraw(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on render' that it is different to 'redraw'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_render(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on show'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_show(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on hide'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_hide(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on resize'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_resize(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on move'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_move(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on mouse wheel'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_mousewheel(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on update'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_update(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on key down'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_key_down(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on key up'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_key_up(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on set focus'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_set_focus(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on kill focus'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_kill_focus(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on mouse over in'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_mouseover_in(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on mouse over out'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_mouseover_out(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on move parent'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_move_parent(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on resize parent'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_resize_parent(const widget_event_t event);

		/**
		* Function used to set a callback to the event 'on rotate parent'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_rotate_parent(const widget_event_t event);
		
		/**
		* Function used to set a callback to the event 'on rotate parent'. The gui object will call it automatically.
		* @param event is a #widget_event_t (lambda or function) that is to be set as callback for the event.  The return of the event could remove the callback after the first use. see #widget_event_return_t to know more.
		*/
		virtual void_t set_on_custom_render(const custom_render_t event);

		/**
		* Function used to set a rounded border to the background.
		* @param rounding is the radius used to round the borders.
		*/
		virtual void_t set_rounding(const int_t rounding);

		/**
		* Function used to set as shown the widget. You can use #hide to hide the widget and #is_shown to know the state.
		*/
		virtual void_t show();

		/**
		* Function used to set as shown the children. You can use #hide_children to hide the children.
		*/
		virtual void_t show_children();

		/**
		* Function used to start an animation that will change the size of the widget gradually. The transition will end in $seconds seconds. The precision of the animation's duration depend on the FPS set to the gui object (by default 30)
		* @param size is the desired size at the end of the animation.
		* @param seconds is the duration of the animation in seconds. The real precision of the animation's duration depend on the value of FPS set on the gui object.
		* @param event_end is a callback used at the end of the animation. You can avoid to pass this argument.
		*/
		virtual void_t stretch_to(const widget_size_t& size, const double_t seconds, widget_event_t* event_end = nullptr);

		/**
		* Function used to start an animation that will change the position of the widget gradually. The transition will end in <seconds> seconds. The precision of the animation's duration depend on the FPS set to the gui object (by default 30)
		*/
		virtual void_t translate_to(const widget_pos_t& pos, const double_t seconds, widget_event_t* event_end = nullptr);

		/**
		* Function used to return statically the type of the widget.
		* \return this function returns statically the type of the widget.
		*/
		static widget_type_t type() { return widget_type_t::window; }
		
		/**
		* Function used to update the widget. The update function run the timer events if necessary, and run the update callback event set to the widget.
		* @param time is the actual time to pass to the widget. It will compare the time passed to the 'timer event' time to know if it is necessary or not to run them.
		*/
		virtual void_t update(const double_t time);
		
	protected:
		/**
		* Function used to update the fading of the alpha factor.
		*/
		virtual void_t __update_alpha_fading();

		/**
		* Function used to update the translation of the position.
		*/
		virtual void_t __update_pos_translating();

		/**
		* Function used to update the translation of the size.
		*/
		virtual void_t __update_size_translating();

		/**
		* Function used to set the parent pointer without run the #add_child to the parent. 
		*/
		virtual void_t __set_parent_pointer( window* parent);

	protected:
		//* positioning dynamically
		widget_pos_t _pos;
		widget_pos_t _dest_pos;
		widget_posf_t _translating_pos_step;
		widget_posf_t _pos_on_translation;

		//* resizing dynamically
		widget_size_t _size;
		widget_size_t _dest_size;
		widget_sizef_t _translating_size_step;
		widget_sizef_t _size_on_translation;

		window* _parent;
		children_deque_t _children_deque;

		layer_level_t _layer_level;
		widget_type_t _widget_type;
		
		byte_t _rendering_calls;
		bool_t _is_focused;

		widget_event_t* _on_mouseleft_up_event;
		widget_event_t* _on_mouseleft_down_event;
		widget_event_t* _on_mouseright_up_event;
		widget_event_t* _on_mouseright_down_event;

		widget_event_t* _on_mousemove_event;
		widget_event_t* _on_resize_gui_event;

		widget_event_t* _on_redraw_event;
		widget_event_t* _on_set_focus_event;
		widget_event_t* _on_kill_focus_event;

		widget_event_t* _on_mouseover_in_event;
		widget_event_t* _on_mouseover_out_event;

		widget_event_t* _on_resize_event;
		widget_event_t* _on_move_event;
		widget_event_t* _on_show_event;
		widget_event_t* _on_hide_event;
 
		widget_event_t* _on_mousewheel_event;
		widget_event_t* _on_key_down_event;
		widget_event_t* _on_key_up_event;
		widget_event_t* _on_update_event;
		widget_event_t* _on_fade_end;
		widget_event_t* _on_translating_pos_end;
		widget_event_t* _on_translating_size_end;
		widget_event_t* _on_render_event;

		widget_event_t* _on_resize_parent_event;
		widget_event_t* _on_move_parent_event;
		widget_event_t* _on_rotate_parent_event;

		custom_render_t* _on_custom_render;

		int_t _rounding;
		int_t _id;
		wstring_t _ws_name;
		bool_t _is_show;
		background_info_t* _bg_info;
		bool_t _is_pickable;

		gui* _gui;
		mousewheel_data_t _mousewheel_data;
		key_message_info_t _key_message_info;

		float_t _alpha;
		float_t _dest_alpha;
		float_t _step_alpha;
		std::vector<timer_event_t> _timer_events_vec;
		float_t _degree;
	};
}

#endif //__HEADER_INCLUDE_windowS_IKARUSGL__

