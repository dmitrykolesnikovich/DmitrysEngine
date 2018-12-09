/* Copyright (c) 2017-2018 Dmitry Stepanov a.k.a mr.DIMAS
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

/**
* @brief
*/
typedef struct de_gui_window_t
{
	de_gui_node_t* border;
	de_gui_node_t* grid;
	de_gui_node_t* header;
	de_gui_node_t* header_grid;
	de_gui_node_t* title;
	de_gui_node_t* scroll_viewer;
	de_gui_node_t* close_button;
	de_gui_node_t* collapse_button;
	de_bool_t is_dragging;
	de_vec2_t mouse_click_pos;
	de_vec2_t init_pos;
} de_gui_window_t;

de_gui_node_t* de_gui_window_create(void);

de_gui_node_t* de_gui_window_get_title(de_gui_node_t* window);

void de_gui_window_set_content(de_gui_node_t* window, de_gui_node_t* content);

de_gui_node_t* de_gui_window_get_content(de_gui_node_t* window);