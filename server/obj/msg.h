/*
 *  msg.h  --  declare server class s_msg
 *
 *  Copyright (C) 1993-2019 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#ifndef _TWIN_MSG_H
#define _TWIN_MSG_H

#include "obj/event.h"
#include "obj/fwd.h"
#include "printk.h"
#include <Tw/datatypes.h>

struct s_fn_msg {
  uldat Magic, Size;
  void (*Insert)(msg, msgport, msg Prev, msg Next);
  void (*Remove)(msg);
  void (*Delete)(msg);
  void (*ChangeField)(msg, udat field, uldat CLEARMask, uldat XORMask);
  /* msg */
  fn_obj Fn_Obj;
};

struct s_msg {
  uldat Id;
  fn_msg Fn;
  msg Prev, Next;
  msgport MsgPort;
  /* msg */
  udat Type; /* See note above */
  udat Len;  /* length of Event */
  event_any Event;

  /* obj */
  uldat Magic() const {
    return Fn->Magic;
  }
  uldat Size() const {
    return Fn->Size;
  }
  static msg Create(fn_msg Fn, udat type, udat eventlen);
  void Insert(msgport port, msg prev, msg next) {
    Fn->Insert(this, port, prev, next);
  }
  void Remove() {
    Fn->Remove(this);
  }
  void Delete() {
    Fn->Delete(this);
  }
};

#endif /* _TWIN_MSG_H */