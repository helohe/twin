/*
 *  menu.h  --  declare server class s_menu
 *
 *  Copyright (C) 1993-2019 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#ifndef TWIN_EVENT_H
#define TWIN_EVENT_H

#include "obj/fwd.h"
#include <Tw/datatypes.h>

enum e_msg {
  msg_key = 0,
  msg_mouse = 1,
  msg_map = 2,
  msg_control = 3,

  msg_display = 0x0FFF,

  msg_system_first = 0x1000,
  msg_widget_key = 0x1000,
  msg_widget_mouse = 0x1001,
  msg_widget_change = 0x1002,
  msg_widget_gadget = 0x1003,
  msg_menu_row = 0x1004,
  msg_selection = 0x1005,
  msg_selection_notify = 0x1006,
  msg_selection_request = 0x1007,
  msg_selection_clear = 0x1008,

  msg_user_first = 0x2000,
  msg_user_control = 0x2000,
  msg_user_control_reply = 0x2001,
  msg_user_clientmsg = 0x2100,
};

/*
 * Notes about MsgType :
 *
 * 0x0000 ... 0x0FFF : Messages from Twin to the WM or another special task;
 * currently defined are:
 * msg_key                use Msg->Event.EventKeyboard to get the event
 * msg_mouse                use ...Event.EventMouse
 * msg_control                use ...Event.EventControl
 *
 * msg_display                use ...Event.EventDisplay
 *
 * 0x1000 ... 0x1FFF : Messages from the WM to a generick task
 * currently defined are:
 * msg_widget_key        use ...EventKeyboard
 * msg_widget_mouse        use ...EventMouse
 * msg_widget_change        use ...EventWindow
 * msg_widget_gadget        use ...EventGadget
 * msg_menu_row                use ...EventMenu
 * msg_selection        use ...EventSelection
 * msg_selection_notify        use ...EventSelectionNotify
 * msg_selection_request        use ...EventSelectionRequest
 * msg_selection_clear        use ...EventCommon
 * If you don't want to get messages from gadgets or menuitem rows,
 * just set to 0 (zero) their Code.
 *
 * 0x2000 ... 0xFFFF : Messages from a generic task to another.
 * they are completely user-defined. As guideline, the following
 * are defined:
 *
 * msg_user_control        use ...Event.EventControl
 */

typedef struct s_event_common {
  widget W;
  udat Code, pad;
} event_common;

typedef struct s_event_map {
  widget W;
  udat Code, pad; /* unused */
  screen Screen;
} event_map;

typedef struct s_event_keyboard {
  widget W;
  udat Code, ShiftFlags, SeqLen;
  byte pad;
  char AsciiSeq[1]; /* AsciiSeq[SeqLen] == '\0' */
} event_keyboard;

typedef struct s_event_mouse {
  widget W;
  udat Code, ShiftFlags;
  dat X, Y;
} event_mouse;

typedef struct s_event_control {
  widget W;
  udat Code, Len;
  dat X, Y;
  char Data[sizeof(uldat)]; /* [Len] bytes actually */
} event_control;

/* some msg_control codes */
#define MSG_CONTROL_QUIT ((udat)0)
#define MSG_CONTROL_RESTART ((udat)1)
#define MSG_CONTROL_OPEN ((udat)2)
#define MSG_CONTROL_DRAGNDROP ((udat)3)

/* use for free-format messages between clients */
typedef struct s_event_clientmsg {
  widget W;
  udat Code, Format;
  uldat Len;
  union {
    byte b[sizeof(uldat)];
    udat d[sizeof(uldat) / sizeof(udat)];
    uldat l[1];
  } Data; /* [Len] bytes actually */
} event_clientmsg;

typedef struct s_event_display {
  widget W; /* not used here */
  udat Code, Len;
  dat X, Y;
  void *Data; /* [Len] bytes actually */
} event_display;

enum e_event_display_code {
  ev_dpy_DrawTCell = 0,
  ev_dpy_FlushHW = 1,
  ev_dpy_KeyboardEvent = 2,
  ev_dpy_MouseEvent = 3,
  ev_dpy_SetCursorType = 4,
  ev_dpy_MoveToXY = 5,
  ev_dpy_Resize = 6,

  ev_dpy_SelectionExport = 8,
  ev_dpy_DragArea = 9,
  ev_dpy_Beep = 10,
  ev_dpy_Configure = 11,
  ev_dpy_SetPalette = 12,
  ev_dpy_ResetPalette = 13,
  ev_dpy_Helper = 14,
  ev_dpy_RedrawVideo = 15,
  ev_dpy_Quit = 16,
};

typedef struct s_event_widget event_widget;
struct s_event_widget {
  widget W;
  udat Code, Flags;
  dat XWidth, YWidth;
  dat X, Y;
};

/* some msg_widget_change codes */
#define MSG_WIDGET_RESIZE 0
#define MSG_WIDGET_EXPOSE 1

/* some msg_widget_change flags */
#define MSG_WIDGETFL_SHADED 1

typedef struct s_event_gadget event_gadget;
struct s_event_gadget {
  widget W;
  udat Code, Flags; /* the Flags of the gadget */
};

typedef struct s_event_menu event_menu;
struct s_event_menu {
  window W;
  udat Code, pad;
  menu Menu;
  row Row;
};

typedef struct s_event_selection event_selection;
struct s_event_selection {
  widget W;
  udat Code, pad; /* unused */
  dat X, Y;
};

#define MAX_MIMELEN 64

typedef struct s_event_selectionnotify event_selectionnotify;
struct s_event_selectionnotify {
  widget W;
  udat Code, pad; /* unused */
  uldat ReqPrivate;
  uldat Magic;
  char MIME[MAX_MIMELEN];
  uldat Len;
  char Data[sizeof(uldat)]; /* Data[] is Len bytes actually */
};
/*SelectionNotify Magic*/
#define SEL_APPEND 0x00000000
#define SEL_UTF8MAGIC 0x38465455 /* UTF-8 */
#define SEL_FILEMAGIC 0x656C6946
#define SEL_URLMAGIC 0x2F4C5255
#define SEL_DATAMAGIC 0x41544144 /* check MIME if you get this */
#define SEL_IDMAGIC 0x64496449

typedef struct s_event_selectionrequest event_selectionrequest;
struct s_event_selectionrequest {
  widget W;
  udat Code, pad; /* unused */
  obj Requestor;
  uldat ReqPrivate;
};

typedef union event_any event_any;
union event_any {
  event_common EventCommon;
  event_keyboard EventKeyboard;
  event_mouse EventMouse;
  event_control EventControl;
  event_clientmsg EventClientMsg;
  event_display EventDisplay;
  event_map EventMap;
  event_widget EventWidget;
  event_gadget EventGadget;
  event_menu EventMenu;
  event_selection EventSelection;
  event_selectionnotify EventSelectionNotify;
  event_selectionrequest EventSelectionRequest;
};

#endif /* TWIN_EVENT_H */
