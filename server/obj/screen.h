/*
 *  window.h  --  declare server class s_window
 *
 *  Copyright (C) 1993-2019 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#ifndef TWIN_SCREEN_H
#define TWIN_SCREEN_H

#include "obj/widget.h"

struct s_sB { /* for SCREENFL_USEBG screens */
  dat BgWidth, BgHeight;
  tcell *Bg;
};

struct s_fn_screen {
  uldat Magic;
  void (*Insert)(screen, all, screen Prev, screen Next);
  void (*Remove)(screen);
  void (*Delete)(screen);
  void (*ChangeField)(screen, udat field, uldat CLEARMask, uldat XORMask);
  /* widget */
  fn_obj Fn_Obj;
  void (*DrawSelf)(draw_ctx *D);
  widget (*FindWidgetAt)(screen Parent, dat X, dat Y);
  gadget (*FindGadgetByCode)(screen Parent, udat Code);
  void (*SetXY)(screen, dat X, dat Y);
  void (*SetFill)(screen, tcell Fill);
  widget (*Focus)(screen);
  widget (*KbdFocus)(screen);
  void (*Map)(screen, widget Parent);
  void (*UnMap)(screen);
  void (*MapTopReal)(screen, screen);
  void (*Raise)(screen);
  void (*Lower)(screen);
  void (*Own)(screen, msgport);
  void (*DisOwn)(screen);
  void (*RecursiveDelete)(screen, msgport);
  void (*Expose)(screen, dat XWidth, dat YWidth, dat Left, dat Up, const char *, const trune *,
                 const tcell *);
  byte (*InstallHook)(screen, fn_hook, fn_hook *Where);
  void (*RemoveHook)(screen, fn_hook, fn_hook *Where);
  /* screen */
  fn_widget Fn_Widget;
  menu (*FindMenu)(screen);
  screen (*Find)(dat j);
  screen (*CreateSimple)(dat NameLen, const char *Name, tcell Bg);
  void (*BgImage)(screen, dat BgWidth, dat BgHeight, const tcell *Bg);
  void (*DrawMenu)(screen, dat Xstart, dat Xend);
  void (*ActivateMenu)(screen, menuitem, byte ByMouse);
  void (*DeActivateMenu)(screen);
};

struct s_screen : public s_obj {
  fn_screen Fn;
  screen Prev, Next;  /* list in the same All */
  widget dummyParent; /* NULL */
  /* widget */
  widget FirstW, LastW; /* list of children */
  widget FocusW;        /* same as SelectW : focused child */
  dat dummyLeft, YLimit, dummyXWidth, dummyYWidth;
  uldat Attr;
  uldat Flags;
  ldat XLogic, YLogic;
  widget O_Prev, O_Next; /* list with the same msgport (owner) */
  msgport Owner;
  fn_hook ShutDownHook; /* hooks for this widget */
  fn_hook Hook, *WhereHook;
  fn_hook MapUnMapHook;
  msg MapQueueMsg;
  tcell USE_Fill;
  union {
    struct s_sB B;
    struct s_wE E;
  } USE;
  /* screen */
  dat NameLen;
  char *Name;
  window MenuWindow, ClickWindow;
  all All;
  fn_hook FnHookW; /* allow hooks on children Map()/UnMap() inside this widget */
  widget HookW;

  static screen Create(dat NameLen, const char *Name, dat BgWidth, dat BgHeight, const tcell *Bg);
  screen Init(dat NameLen, const char *Name, dat BgWidth, dat BgHeight, const tcell *Bg);

  /* obj */
  uldat Magic() const {
    return Fn->Magic;
  }
  void Insert(all a, screen prev, screen next) {
    Fn->Insert(this, a, prev, next);
  }
  void Remove() {
    Fn->Remove(this);
  }
  void Delete() {
    Fn->Delete(this);
  }
  /* widget */
  void DrawSelf(draw_ctx *D) {
    Fn->DrawSelf(D);
  }
  widget FindWidgetAt(dat x, dat y) {
    return Fn->FindWidgetAt(this, x, y);
  }
  gadget FindGadgetByCode(udat code) {
    return Fn->FindGadgetByCode(this, code);
  }
  void SetXY(dat x, dat y) {
    Fn->SetXY(this, x, y);
  }
  void SetFill(tcell fill) {
    Fn->SetFill(this, fill);
  }
  widget Focus() {
    return Fn->Focus(this);
  }
  widget KbdFocus() {
    return Fn->KbdFocus(this);
  }
  void Map(widget parent) {
    Fn->Map(this, parent);
  }
  void UnMap() {
    Fn->UnMap(this);
  }
  void MapTopReal(screen scr) {
    Fn->MapTopReal(this, scr);
  }
  void Raise() {
    Fn->Raise(this);
  }
  void Lower() {
    Fn->Lower(this);
  }
  void Own(msgport port) {
    Fn->Own(this, port);
  }
  void DisOwn() {
    Fn->DisOwn(this);
  }
  void RecursiveDelete(msgport port) {
    Fn->RecursiveDelete(this, port);
  }
  void Expose(dat xwidth, dat ywidth, dat left, dat up, const char *ascii, const trune *runes,
              const tcell *cells) {
    Fn->Expose(this, xwidth, ywidth, left, up, ascii, runes, cells);
  }
  byte InstallHook(fn_hook hook, fn_hook *where) {
    return Fn->InstallHook(this, hook, where);
  }
  void RemoveHook(fn_hook hook, fn_hook *where) {
    Fn->RemoveHook(this, hook, where);
  }
  /* screen */
  menu FindMenu() {
    return Fn->FindMenu(this);
  }
  screen Find(dat j) {
    return Fn->Find(j);
  }
  screen CreateSimple(dat namelen, const char *name, tcell bg) {
    return Fn->CreateSimple(namelen, name, bg);
  }
  void BgImage(dat bgwidth, dat bgheight, const tcell *bg) {
    Fn->BgImage(this, bgwidth, bgheight, bg);
  }
  void DrawMenu(dat xstart, dat xend) {
    Fn->DrawMenu(this, xstart, xend);
  }
  void ActivateMenu(menuitem item, byte bymouse) {
    Fn->ActivateMenu(this, item, bymouse);
  }
  void DeActivateMenu() {
    Fn->DeActivateMenu(this);
  }
};

/* Screen->Attr */
/* not used */

/* Screen->Flags */
#define SCREENFL_USEBG 0x00                   /* it's the default */
#define SCREENFL_USEEXPOSE WIDGETFL_USEEXPOSE /* 0x02 */
#define SCREENFL_USEFILL WIDGETFL_USEFILL     /* 0x03 */
#define SCREENFL_USEANY WIDGETFL_USEANY       /* 0x07 */

#define S_USE(s, USExxx) (((s)->Flags & SCREENFL_USEANY) == CAT(SCREENFL_, USExxx))

#define SCREENFL_BACK_SELECT 0x0010
#define SCREENFL_BACK_PRESSED 0x0020
#define SCREENFL_NOTVISIBLE 0x8000

#endif /* TWIN_SCREEN_H */
