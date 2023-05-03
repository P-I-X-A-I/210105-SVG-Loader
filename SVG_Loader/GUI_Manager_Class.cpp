#include "framework.h"
#include "GUI_Manager_Class.h"


GUI_Manager_Class::GUI_Manager_Class()
{
	hInst = GetModuleHandle(NULL);

	// to use trackbar, init common controls
	InitCommonControls();

	// small font
	h_smallFont = CreateFont(
		12, // size
		0, //width
		0, // escapement
		0, // orientation
		FW_NORMAL, //thickness
		FALSE, // itaric
		FALSE, // underline
		0, // strike out
		SHIFTJIS_CHARSET, // charset
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		NULL // font name
	);

	// normal
	h_normalFont = CreateFont(
		13, // size
		0, //width
		0, // escapement
		0, // orientation
		FW_NORMAL, //thickness
		FALSE, // itaric
		FALSE, // underline
		0, // strike out
		SHIFTJIS_CHARSET, // charset
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		NULL // font name
	);

	// large font
	h_largeFont = CreateFont(
		16, // size
		0, //width
		0, // escapement
		0, // orientation
		FW_NORMAL, //thickness
		FALSE, // itaric
		FALSE, // underline
		0, // strike out
		SHIFTJIS_CHARSET, // charset
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		NULL // font name
	);
}


GUI_Manager_Class::~GUI_Manager_Class()
{
}


//******************** text field ******************************

HWND GUI_Manager_Class::create_textfield(HWND parentWindow, LPCWSTR string, int x, int y, int width, int height, int ID)
{
	HWND tempTexField = CreateWindow(
		TEXT("STATIC"),
		string,
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		x, y,
		width, height,
		parentWindow,
		(HMENU)ID,
		hInst,
		NULL
	);


	int error = SetBkMode(GetDC(tempTexField), TRANSPARENT);
	SendMessage(tempTexField, WM_SETFONT, (WPARAM)h_normalFont, MAKELPARAM(FALSE, 0));

	return tempTexField;
}

void GUI_Manager_Class::set_textfield_string(HWND targetWnd, LPCWSTR string, ENUM_FONT_SIZE size)
{
	HFONT fontsize = h_normalFont;
	switch (size)
	{
	case FONT_SMALL:
		fontsize = h_smallFont;
		break;
	case FONT_NORMAL:
		fontsize = h_normalFont;
		break;
	case FONT_LARGE:
		fontsize = h_largeFont;
		break;
	default:
		break;
	}

	SendMessage(targetWnd, WM_SETFONT, (WPARAM)fontsize, MAKELPARAM(FALSE, 0));
	SendMessage(targetWnd, WM_SETTEXT, 0, (LPARAM)string);
}




//**************** button *************************

HWND GUI_Manager_Class::create_button(HWND parentWindow, LPCWSTR string, int x, int y, int width, int height, int ID)
{
	HWND tempButton = CreateWindowW(
		TEXT("BUTTON"),
		string,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, y,
		width, height,
		parentWindow,
		(HMENU)ID,
		hInst,
		NULL
	);

	SendMessage(tempButton, WM_SETFONT, (WPARAM)h_normalFont, MAKELPARAM(FALSE, 0));

	return tempButton;
}


void GUI_Manager_Class::set_button_string(HWND targetWnd, LPCWSTR string, ENUM_FONT_SIZE size)
{
	HFONT fontsize = h_normalFont;

	switch (size)
	{
	case FONT_SMALL:
		fontsize = h_smallFont;
		break;
	case FONT_NORMAL:
		fontsize = h_normalFont;
		break;
	case FONT_LARGE:
		fontsize = h_largeFont;
		break;
	default:
		break;
	}

	SendMessage(targetWnd, WM_SETFONT, (WPARAM)fontsize, MAKELPARAM(FALSE, 0));
	SendMessage(targetWnd, WM_SETTEXT, 0, (LPARAM)string);
}





// check box *******************************
HWND GUI_Manager_Class::create_checkbox(HWND parentWindow, LPCWSTR string, int x, int y, int width, int height, int ID)
{
	HWND tempButton = CreateWindowW(
		TEXT("BUTTON"),
		string,
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		x, y,
		width, height,
		parentWindow,
		(HMENU)ID,
		hInst,
		NULL
	);

	SendMessage(tempButton, WM_SETFONT, (WPARAM)h_normalFont, MAKELPARAM(FALSE, 0));

	return tempButton;
}
void GUI_Manager_Class::set_checkbox_string(HWND targetWnd, LPCWSTR string, ENUM_FONT_SIZE size)
{
	HFONT fontsize = h_normalFont;

	switch (size)
	{
	case FONT_SMALL:
		fontsize = h_smallFont;
		break;
	case FONT_NORMAL:
		fontsize = h_normalFont;
		break;
	case FONT_LARGE:
		fontsize = h_largeFont;
		break;
	default:
		break;
	}

	SendMessage(targetWnd, WM_SETFONT, (WPARAM)fontsize, MAKELPARAM(FALSE, 0));
	SendMessage(targetWnd, WM_SETTEXT, 0, (LPARAM)string);
}



void GUI_Manager_Class::set_checkbox_state(HWND targetWnd, bool state)
{
	if (state)
	{
		SendMessage(targetWnd, BM_SETCHECK, BST_CHECKED, 0);
	}
	else
	{
		SendMessage(targetWnd, BM_SETCHECK, BST_UNCHECKED, 0);
	}
}




bool GUI_Manager_Class::get_checkbox_state(HWND targetWnd)
{
	if (BST_CHECKED == SendMessage(targetWnd, BM_GETCHECK, 0, 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//******************* SLIDER ***************************

HWND GUI_Manager_Class::create_slider(HWND parentWindow, int range, int x, int y, int width, int height, int ID)
{
	HWND tempSlider = CreateWindowEx(
		NULL, // window style
		TRACKBAR_CLASS, // class
		NULL, // window name
		WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
		x, y,
		width, height,
		parentWindow,
		(HMENU)ID,
		hInst,
		NULL
	);

	SendMessage(tempSlider, TBM_SETRANGE, true, MAKELPARAM(0, range));
	SendMessage(tempSlider, TBM_SETPOS, true, 0);
	SendMessage(tempSlider, TBM_SETPAGESIZE, 0, 1);
	return tempSlider;
}

HWND GUI_Manager_Class::create_vertical_slider(HWND parentWindow, int range, int x, int y, int width, int height, int ID)
{
	HWND tempSlider = CreateWindowEx(
		NULL,
		TRACKBAR_CLASS,
		NULL,
		WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_VERT,
		x, y,
		width, height,
		parentWindow,
		(HMENU)ID,
		hInst,
		NULL
	);

	SendMessage(tempSlider, TBM_SETRANGE, true, MAKELPARAM(0, range));
	SendMessage(tempSlider, TBM_SETPOS, true, 0);
	SendMessage(tempSlider, TBM_SETPAGESIZE, 0, 1);
	return tempSlider;
}

int GUI_Manager_Class::get_slider_position(HWND targetWnd)
{
	int pos = SendMessage(targetWnd, TBM_GETPOS, 0, 0);

	return pos;
}

void GUI_Manager_Class::set_slider_position(HWND targetWnd, int pos)
{
	SendMessage(targetWnd, TBM_SETPOS, true, pos);
}

void GUI_Manager_Class::set_slider_range(HWND targetWnd, int min, int max)
{
	SendMessage(targetWnd, TBM_SETRANGE, true, MAKELPARAM(min, max));
}




//******************** COMBOBOX *********************

HWND GUI_Manager_Class::create_combobox(HWND parentWindow, int x, int y, int width, int height, int ID)
{
	HWND tempCombo = CreateWindow(
		TEXT("COMBOBOX"),
		NULL,
		WS_CHILD | WS_VISIBLE | ES_RIGHT | CBS_DROPDOWNLIST,
		x, y,
		width, height,
		parentWindow,
		(HMENU)ID,
		hInst,
		NULL
	);

	SendMessage(tempCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("0"));
	SendMessage(tempCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("1"));
	SendMessage(tempCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("2"));

	SendMessage(tempCombo, WM_SETFONT, (WPARAM)h_largeFont, MAKELPARAM(FALSE, 0));

	SendMessage(tempCombo, CB_SETCURSEL, 0, 0);

	return tempCombo;
}


void GUI_Manager_Class::add_combobox_item(HWND targetWnd, LPCWSTR item)
{
	SendMessage(targetWnd, CB_ADDSTRING, 0, (LPARAM)item);
}

void GUI_Manager_Class::delete_combobox_item_at_index(HWND targetWnd, int index)
{
	SendMessage(targetWnd, CB_DELETESTRING, index, 0);
}

void GUI_Manager_Class::delete_combobox_all(HWND targetWnd)
{
	while (SendMessage(targetWnd, CB_GETCOUNT, 0, 0) != 0)
	{
		SendMessage(targetWnd, CB_DELETESTRING, 0, 0);
	}
}

int GUI_Manager_Class::get_combobox_itemcount(HWND targetWnd)
{
	int count = SendMessage(targetWnd, CB_GETCOUNT, 0, 0);

	return count;
}

int GUI_Manager_Class::get_combobox_selected_index(HWND targetWnd)
{
	int selIndex = SendMessage(targetWnd, CB_GETCURSEL, 0, 0);

	return selIndex;
}

void GUI_Manager_Class::set_combobox_selected_item_at_index(HWND targetWnd, int index)
{
	SendMessage(targetWnd, CB_SETCURSEL, index, 0);
}




// **************** LIST BOX *************************

HWND GUI_Manager_Class::create_listbox(HWND parentWindow, int x, int y, int width, int height, int ID)
{
	HWND tempList = CreateWindow(
		TEXT("LISTBOX"),
		NULL,
		WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_BORDER | LBS_NOINTEGRALHEIGHT | WS_VSCROLL,
		x, y,
		width, height,
		parentWindow,
		(HMENU)8000,
		hInst,
		NULL
	);

	SendMessage(tempList, WM_SETFONT, (WPARAM)h_largeFont, MAKELPARAM(FALSE, 0));
	SendMessage(tempList, LB_SETHORIZONTALEXTENT, 300, 0); // has h scroll bar
	return tempList;
}


void GUI_Manager_Class::add_listbox_item(HWND targetWnd, LPCWSTR item)
{
	SendMessage(targetWnd, LB_ADDSTRING, 0, (LPARAM)item);
}
void GUI_Manager_Class::delete_listbox_item_at_index(HWND targetWnd, int index)
{
	SendMessage(targetWnd, LB_DELETESTRING, index, 0);
}
void GUI_Manager_Class::delete_listbox_all(HWND targetWnd)
{
	while (SendMessage(targetWnd, LB_GETCOUNT, 0, 0) != 0)
	{
		SendMessage(targetWnd, LB_DELETESTRING, 0, 0);
	}
}
int GUI_Manager_Class::get_listbox_itemcount(HWND targetWnd)
{
	int count = SendMessage(targetWnd, LB_GETCOUNT, 0, 0);

	return count;
}
int GUI_Manager_Class::get_listbox_selected_index(HWND targetWnd)
{
	int selIndex = SendMessage(targetWnd, LB_GETCURSEL, 0, 0);

	return selIndex;
}
void GUI_Manager_Class::set_listbox_selected_item_at_index(HWND targetWnd, int index)
{
	SendMessage(targetWnd, LB_SETCURSEL, index, 0);
}


// ***************** progress bar ********************

HWND GUI_Manager_Class::create_progressbar(HWND parentWindow, int x, int y, int width, int height, int ID)
{
	HWND tempProgress = CreateWindowEx(
		0,
		PROGRESS_CLASS,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | PBS_SMOOTH,
		x, y,
		width, height,
		parentWindow,
		(HMENU)ID,
		hInst,
		NULL
	);

	SendMessage(tempProgress, PBM_SETRANGE32, 0, 100);
	SendMessage(tempProgress, PBM_SETSTEP, 1, 0); // for SendMessage( hWnd, PBM_STEPIT, 0, 0 );
	SendMessage(tempProgress, PBM_SETPOS, 0, 0);

	return tempProgress;
}

void GUI_Manager_Class::set_progressbar_position(HWND targetWnd, int pos)
{
	SendMessage(targetWnd, PBM_SETPOS, (WPARAM)pos, 0);
}


void GUI_Manager_Class::increse_progressbar(HWND targetWnd)
{
	SendMessage(targetWnd, PBM_STEPIT, 0, 0);
}

//*************** common management ***********************

void GUI_Manager_Class::set_enabled(HWND targetWnd, bool yn)
{
	EnableWindow(targetWnd, yn);
}

void GUI_Manager_Class::set_frame(HWND targetWnd, int x, int y, int width, int height)
{
	SetWindowPos(
		targetWnd,
		NULL,
		x, y,
		width, height,
		SWP_NOZORDER
	);
}

void GUI_Manager_Class::set_hidden(HWND targetWnd, bool yn)
{
	if (yn)
	{
		ShowWindow(targetWnd, SW_HIDE);
	}
	else
	{
		ShowWindow(targetWnd, SW_SHOW);
	}
}