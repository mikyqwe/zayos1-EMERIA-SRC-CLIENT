#pragma once

namespace ikgl {
	/**
	* This widget is used to draw children just in the rectangle of interest, discarding the rest of the drawed graphic. The use of frame windows is less efficient than window, it is not recommended if it is not really usefull for your needs.
	*/
    class frame_window : public window
    {

    public:
        frame_window(gui* ptr);

		/**
		* Function used to draw the widget (used internally by the gui object but also usable if you are using a custom rendering method). It is modified to discard all drawing out of the rectangle of the window
		* @param hdc is and handle to a DC object (Microsoft docs contains the information about the DC/HDC)
		* @param graphicsptr is a pointer to a gdiplus graphic object used to draw the widget (Micosoft docs contains the information about Gdiplus::Graphics)
		*/
		virtual void_t draw(hdc_t& hdc, gdiplus_graphics_t* graphicsptr);
    };
}
