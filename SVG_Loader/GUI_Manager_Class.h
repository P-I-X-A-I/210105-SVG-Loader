#pragma once

#include "commctrl.h"
#pragma comment( lib, "ComCtl32.lib" )


enum ENUM_FONT_SIZE
{
	FONT_SMALL,
	FONT_NORMAL,
	FONT_LARGE
};

class GUI_Manager_Class
{
public:
	GUI_Manager_Class();
	~GUI_Manager_Class();

	// text field
	HWND create_textfield(HWND parentWindow, LPCWSTR string, int x, int y, int width, int height, int ID);
	void set_textfield_string(HWND targetWnd, LPCWSTR string, ENUM_FONT_SIZE size);


	// button
	HWND create_button(HWND parentWindow, LPCWSTR string, int x, int y, int width, int height, int ID);
	void set_button_string(HWND targetWnd, LPCWSTR string, ENUM_FONT_SIZE size);

	// check box
	HWND create_checkbox(HWND parentWindow, LPCWSTR string, int x, int y, int width, int height, int ID);
	void set_checkbox_string(HWND targetWnd, LPCWSTR string, ENUM_FONT_SIZE size);
	void set_checkbox_state(HWND targetWnd, bool state);
	bool get_checkbox_state(HWND targetWnd);

	// slider
	HWND create_slider(HWND parentWindow, int range, int x, int y, int width, int heigh, int ID);
	HWND create_vertical_slider(HWND parentWindow, int range, int x, int y, int width, int height, int ID);
	int get_slider_position(HWND targetWnd);
	void set_slider_position(HWND targetWnd, int pos);
	void set_slider_range(HWND targetWnd, int min, int max);


	// combobox
	HWND create_combobox(HWND parentWindow, int x, int y, int width, int height, int ID);
	void add_combobox_item(HWND targetWnd, LPCWSTR item);
	void delete_combobox_item_at_index(HWND targetWnd, int index);
	void delete_combobox_all(HWND targetWnd);
	int get_combobox_itemcount(HWND targetWnd);
	int get_combobox_selected_index(HWND targetWnd);
	void set_combobox_selected_item_at_index(HWND targetWnd, int index);


	// list box
	HWND create_listbox(HWND parentWindow, int x, int y, int width, int height, int ID);
	void add_listbox_item(HWND targetWnd, LPCWSTR item);
	void delete_listbox_item_at_index(HWND targetWnd, int index);
	void delete_listbox_all(HWND targetWnd);
	int get_listbox_itemcount(HWND targetWnd);
	int get_listbox_selected_index(HWND targetWnd);
	void set_listbox_selected_item_at_index(HWND targetWnd, int index);

	// progress bar
	HWND create_progressbar(HWND parentWindow, int x, int y, int width, int height, int ID);
	void set_progressbar_position(HWND targetWnd, int pos);
	void increse_progressbar(HWND targetWnd);


	// tooltip controll (sub)


	// list view


	// tab control

	// common management
	void set_enabled(HWND targetWnd, bool yn);
	void set_frame(HWND targetWnd, int x, int y, int width, int height);
	void set_hidden(HWND targetWnd, bool yn);

private:
	HFONT h_smallFont;
	HFONT h_normalFont;
	HFONT h_largeFont;

	HINSTANCE hInst;

};

