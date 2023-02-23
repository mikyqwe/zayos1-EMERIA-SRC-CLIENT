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
#ifndef __INCLUDE_HEADER_LAYER_IKARUSGL__
#define __INCLUDE_HEADER_LAYER_IKARUSGL__


namespace ikgl
{
	class layer
	{
	public:
		/**
		* Function used to create new elements and automatically set the basic information to the widget.
		* \return the pointer to the newone widget of type T created.
		*/
		template<class T>
		T* create_element() {
			T* obj = new T(this->_gui);
			obj->set_widget_type(T::type());
			obj->set_id(alloc_id());
			add_child(obj);
			return obj;
		}

		/**
		* Function used to create new elements and automatically set the basic information to the widget.
		* @param t will contain the output address of the newone widget of type T created.
		*/
		template <class T>
		void_t create_element(T*& t) {
			t = new T(this->_gui);
			t->set_widget_type(T::type());
			t->set_id(alloc_id());
			add_child(t);
		}

		
		layer(gui* guiptr);
		~layer();

		/**
		* Function used to add a child to the layer. It's recommended to create the widget by using the #ikgl::layer::create_element if you need to create a newone widget.
		* @param child is the pointer to the widget to add into childrenlist.
		*/
		void_t add_child(window* child);

		/**
		* Function used to reset the widget.
		*/
		void_t clear();

		/**
		* Function used to obtain a reference to the children list.
		* \return a reference to the children list.
		*/
		window::children_deque_t& get_children_ref();

		/**
		* Function used to check what widget is picked in the input position.
		* @param pos is the input position to check.
		* \return a pointer to the picked widget. The function may return nullptr if the input position doesn't pick any widget.
		*/
		window* get_picked(const widget_pos_t& pos);

		/**
		* Function used to obtain a pointer to the widget that it owns that id.
		* @param id is the identificator to find.
		* \return a pointer to the widget that owns the id. The function may return nullptr if there aren't widgets that own the input id.
		*/
		window* get_child(const int id) const;

		/**
		* Function used to obtain as output the address to the widget that it owns that input id.
		* @param id is the id to find.
		* @param winptr is the output pointer to assign.
		* \return true if the widget is found.
		*/
		bool_t get_child(const int id, window** winptr) const;

		/**
		* Function used to run the callback event 'on_render' set on the children and to receive the rendering requesting answer from the children.
		* \return true if at least one of the children of the layer needed the rendering.
		*/
		bool_t on_render();

		/**
		* Function used to apply the mouse movement to the children.
		* @param oldpos is the oldone mouse position
		* @param newpos is the newone mouse position
		*/
		void_t on_mousemove(const widget_pos_t& oldpos, const widget_pos_t& newpos);

		/**
		* Function used to run the event 'on_resize_gui' on all the children.
		*/
		void_t on_resize_gui();
		
		/**
		* Function used to remove a root widget from the children list. The removing from the children list will destroy the widget.
		* @param child is a pointer to the child to remove.
		*/
		void_t remove_child(window* child);

		/**
		* Function used to render the layer. It's automatically used by #ikgl::gui objects. It allow you to use it in a customized rendering loop.
		* @param hdc is a HANDLE to a DC object. You can find more about DC object in Microsoft Documention
		* @param graphicsptr is a pointer to a Gdiplus::Graphics object. You can find more about Gdiplus::Graphics object in Microsoft Documention
		*/
		void_t render(hdc_t& hdc, gdiplus_graphics_t* graphicsptr);

		/**
		* Function used to update the children of the layer. It's automatically used by the #ikgl::gui object. It allow you to use it in a customized update loop.
		* @param time is the actual time used to run the timer events that are to be run. (You can use #ikgl::utilities::get_time_double to obtain the actual time in double_t seconds)
		*/
		void_t update(const double_t time);
		

	private:
		window::children_deque_t	_children_list;
		gui* _gui;
	};
}

#endif//__INCLUDE_HEADER_LAYER_IKARUSGL__
