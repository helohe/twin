/*
 *  tterm.c  --  create and manage multiple terminal emulator windows on twin
 *
 *  Copyright (C) 1999-2001 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#include "twin.h"
#include "alloc.h"
#include "extreg.h"
#include "data.h"
#include "methods.h"
#include "obj/id.h" // Id2Obj()

#include "remote.h"
#include "pty.h"
#include "util.h"
#include "common.h"

#define COD_QUIT (udat)1
#define COD_SPAWN (udat)3

static menu Term_Menu;

static char *default_args[3];

static msgport Term_MsgPort;

static void TwinTermH(msgport MsgPort);
static void TwinTermIO(int Fd, window Window);

static void termShutDown(widget W) {
  window Window;
  if (IS_WINDOW(W)) {
    Window = (window)W;
    if (Window->RemoteData.Fd != NOFD)
      close(Window->RemoteData.Fd);
    UnRegisterWindowFdIO(Window);
  }
}

static window newTermWindow(const char *title) {
  window Window;

  Window = New(window)(
      Fn_window, Term_MsgPort, strlen(title), title, NULL, Term_Menu, TCOL(twhite, tblack),
      LINECURSOR, WINDOW_WANT_KEYS | WINDOW_DRAG | WINDOW_RESIZE | WINDOW_Y_BAR | WINDOW_CLOSE,
      WINDOWFL_CURSOR_ON | WINDOWFL_USECONTENTS,
      /*width*/ 80, /*height*/ 25, /*scrollbacklines*/ 1000);

  if (Window) {
    Act(SetColors, Window)(Window, 0x1FF, TCOL(thigh | tyellow, tcyan),
                           TCOL(thigh | tgreen, thigh | tblue), TCOL(twhite, thigh | tblue),
                           TCOL(thigh | twhite, thigh | tblue), TCOL(thigh | twhite, thigh | tblue),
                           TCOL(twhite, tblack), TCOL(thigh | tblack, thigh | twhite),
                           TCOL(thigh | tblack, tblack), TCOL(tblack, thigh | tblack));

    Act(Configure, Window)(Window, (1 << 2) | (1 << 3), 0, 0, 7, 3, 0, 0);
  }
  return Window;
}

static window OpenTerm(const char *arg0, const char *const *argv) {
  window Window;
  const char *title;

  /* if {arg0, argv} is {NULL, ...} or {"", ... } then start user's shell */
  if (arg0 && *arg0 && argv && argv[0]) {
    if ((title = strrchr(argv[0], '/')))
      title++;
    else
      title = argv[0];
  } else {
    arg0 = default_args[0];
    argv = (const char *const *)default_args + 1;

    title = "Twin Term";
  }

  if ((Window = newTermWindow(title))) {
    if (SpawnInWindow(Window, arg0, argv)) {
      if (RegisterWindowFdIO(Window, TwinTermIO)) {
        Window->ShutDownHook = termShutDown;
        Window->Map((widget)All->FirstScreen);
        return Window;
      }
      close(Window->RemoteData.Fd);
    }
    Window->Delete();
  }
  return NULL;
}

static void TermWriteTRuneWindow(window W, uldat len, const trune *hwData) {
  trune (*inv_charset)(trune) = W->USE.C.TtyData->InvCharset;
  byte *Data, *sData;
  uldat n;

  if ((Data = sData = (byte *)AllocMem(n = len))) {
    while (n--)
      *Data++ = (byte)inv_charset(*hwData++);

    RemoteWindowWriteQueue(W, len, sData);
    FreeMem(sData);
  }
}

static void TwinTermH(msgport MsgPort) {
  msg Msg;
  event_any *Event;
  udat Code /*, Repeat*/;
  window Win;

  while ((Msg = Term_MsgPort->FirstMsg)) {
    Msg->Remove();

    Event = &Msg->Event;
    Win = (window)Event->EventSelection.W;
    if (Win && !IS_WINDOW(Win))
      Win = NULL;

    if (Msg->Type == msg_widget_key) {
      Code = Event->EventKeyboard.Code;
      /* send keypresses */
      if (Win)
        (void)RemoteWindowWriteQueue(Win, Event->EventKeyboard.SeqLen,
                                     Event->EventKeyboard.AsciiSeq);
    } else if (Msg->Type == msg_selection) {

      TwinSelectionRequest((obj)Term_MsgPort, Win->Id, TwinSelectionGetOwner());

    } else if (Msg->Type == msg_selection_notify) {

      if ((Win = (window)Id2Obj(window_magic_id, Event->EventSelectionNotify.ReqPrivate))) {
        if (Event->EventSelectionNotify.Magic == SEL_TRUNEMAGIC)
          TermWriteTRuneWindow(Win, Event->EventSelectionNotify.Len / sizeof(trune),
                               (trune *)Event->EventSelectionNotify.Data);
        else
          (void)RemoteWindowWriteQueue(Win, Event->EventSelectionNotify.Len,
                                       Event->EventSelectionNotify.Data);
      }
    } else if (Msg->Type == msg_widget_mouse) {
      if (Win) {
        char buf[10];
        byte len = CreateXTermMouseEvent(&Event->EventMouse, 10, buf);

        /* send mouse movements using xterm mouse protocol */
        if (len)
          (void)RemoteWindowWriteQueue(Win, len, buf);
      }
    } else if (Msg->Type == msg_widget_gadget) {
      if (Win && Event->EventGadget.Code == 0 /* Close Code */) {
        Win->Delete();
      }
    } else if (Msg->Type == msg_menu_row) {
      if (Event->EventMenu.Menu == Term_Menu) {
        Code = Event->EventMenu.Code;
        switch (Code) {
        case COD_SPAWN:
          OpenTerm(NULL, NULL);
          break;
        default:
          break;
        }
      }
    } else if (Msg->Type == msg_user_control) {
      /* this duplicates the same functionality of builtin.c */
      if (Event->EventControl.Code == MSG_CONTROL_OPEN) {
        char **cmd = TokenizeStringVec(Event->EventControl.Len, Event->EventControl.Data);
        if (cmd) {
          OpenTerm(cmd[0], (const char *const *)cmd);
          FreeStringVec(cmd);
        } else
          OpenTerm(NULL, NULL);
      }
    }
    Msg->Delete();
  }
}

static void TwinTermIO(int Fd, window Window) {
  static char buf[TW_BIGBUFF];
  uldat got = 0, chunk = 0;

  do {
    /*
     * TW_BIGBUFF - 1 to avoid silly windows...
     * linux ttys buffer up to 4095 bytes.
     */
    chunk = read(Fd, buf + got, TW_BIGBUFF - 1 - got);
  } while (chunk && chunk != (uldat)-1 && (got += chunk) < TW_BIGBUFF - 1);

  if (got) {
    Window->TtyWriteAscii(got, buf);
  } else if (chunk == (uldat)-1 && errno != EINTR && errno != EWOULDBLOCK) {
    /* something bad happened to our child :( */
    Window->Delete();
  }
}

#include "tty.h"

static void OverrideMethods(byte enter) {
  if (enter) {
    OverrideMethod(widget, KbdFocus, FakeKbdFocus, TtyKbdFocus);
    OverrideMethod(gadget, KbdFocus, FakeKbdFocus, TtyKbdFocus);
    OverrideMethod(window, KbdFocus, FakeKbdFocus, TtyKbdFocus);
    OverrideMethod(window, TtyWriteAscii, FakeWriteAscii, TtyWriteAscii);
    OverrideMethod(window, TtyWriteString, FakeWriteString, TtyWriteString);
    OverrideMethod(window, TtyWriteTRune, FakeWriteTRune, TtyWriteTRune);
    OverrideMethod(window, TtyWriteTCell, FakeWriteTCell, TtyWriteTCell);
    ForceKbdFocus();
  } else {
    OverrideMethod(window, TtyWriteTCell, TtyWriteTCell, FakeWriteTCell);
    OverrideMethod(window, TtyWriteTRune, TtyWriteTRune, FakeWriteTRune);
    OverrideMethod(window, TtyWriteString, TtyWriteString, FakeWriteString);
    OverrideMethod(window, TtyWriteAscii, TtyWriteAscii, FakeWriteAscii);
    OverrideMethod(window, KbdFocus, TtyKbdFocus, FakeKbdFocus);
    OverrideMethod(gadget, KbdFocus, TtyKbdFocus, FakeKbdFocus);
    OverrideMethod(widget, KbdFocus, TtyKbdFocus, FakeKbdFocus);
  }
}

EXTERN_C byte InitModule(module Module) {
  window Window;
  const char *shellpath, *shell;

  if (((shellpath = getenv("SHELL")) || (shellpath = "/bin/sh")) &&
      (default_args[0] = CloneStr(shellpath)) &&
      (default_args[1] =
           (shell = strrchr(shellpath, '/')) ? CloneStr(shell) : CloneStr(shellpath)) &&

      (Term_MsgPort = New(msgport)(Fn_msgport, 14, "builtin twterm", (uldat)0, (udat)0,
                                          (byte)0, TwinTermH)) &&
      (Term_Menu =
           New(menu)(Fn_menu, Term_MsgPort, TCOL(tblack, twhite), TCOL(tblack, tgreen),
                            TCOL(thigh | tblack, twhite), TCOL(thigh | tblack, tblack),
                            TCOL(tred, twhite), TCOL(tred, tgreen), (byte)0)) &&
      Info4Menu(Term_Menu, ROW_ACTIVE, (uldat)19, " Builtin Twin Term ",
                (const tcolor *)"ptppppppptpppptpppp") &&

      (Window = Win4Menu(Term_Menu)) && Row4Menu(Window, COD_SPAWN, ROW_ACTIVE, 10, " New Term ") &&
      Row4Menu(Window, COD_QUIT, tfalse, 6, " Exit ") &&
      Item4Menu(Term_Menu, Window, ttrue, 6, " File ") &&

      Item4MenuCommon(Term_Menu)) {

    RegisterExt(Term, Open, OpenTerm);
    OverrideMethods(ttrue);

    if (default_args[1][0] == '/')
      default_args[1][0] = '-';
    return ttrue;
  }
  if (shellpath)
    printk("twin: InitTerm(): " SS "\n", Errstr);
  else
    printk("twin: environment variable $SHELL not set!\n");
  return tfalse;
}

EXTERN_C void QuitModule(module Module) {
  UnRegisterExt(Term, Open, OpenTerm);
  OverrideMethods(tfalse);
  if (Term_MsgPort) {
    Term_MsgPort->Delete();
  }
}