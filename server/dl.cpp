/*
 *  dl.c  --  load/unload modules
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
#include "methods.h"
#include "data.h"
#include "dl.h"
#include "util.h"
#include "printk.h"
#include "version.h"

#include "dl_helper.h"

byte DlOpen(module Module) {
  dlhandle Handle = NULL;
  uldat len, len0 = 1 + strlen(pkg_libdir) + strlen(DL_PREFIX) + strlen(DL_SUFFIX);
  char *name = NULL;
  byte (*init_func)(module);

  if (!dlinit_once()) {
    return tfalse;
  }

  if (Module && !Module->Handle && (!Module->NameLen || Module->Name)) {
    /* dlopen(NULL, ...) returns a handle for the main program */
    if (Module->NameLen) {
      len = len0 + Module->NameLen;
      if ((name = (char *)AllocMem(len + 1)))
        sprintf(name, "%s/%s%.*s%s", pkg_libdir, DL_PREFIX, (int)Module->NameLen, Module->Name,
                DL_SUFFIX);
      else {
        return tfalse;
      }
    }
    Handle = dlopen(name);
    if (name)
      FreeMem(name);
  }
  if (!Handle) {
    Error(DLERROR);
    Errstr = dlerror();
    return tfalse;
  }

  if (name) {
    init_func = (byte(*)(module))dlsym(Handle, "InitModule");
    if (init_func && init_func(Module)) {
      Module->Handle = (void *)Handle;
      return ttrue;
    }
    dlclose(Handle);

    if (init_func) {
      if (Errstr == NULL || *Errstr == '\0') {
        Error(DLERROR);
        Errstr = "InitModule() failed";
      }
    } else {
      Error(DLERROR);
      Errstr = "InitModule() not found in module";
    }
    return tfalse;
  }
  return ttrue;
}

void DlClose(module Module) {
  if (Module && Module->Handle) {
    if (Module->NameLen != 0) {
      void (*quit_func)(module) = (void (*)(module))dlsym((dlhandle)Module->Handle, "QuitModule");
      if (quit_func)
        quit_func(Module);
    }
    dlclose((dlhandle)Module->Handle);
    Module->Handle = NULL;
  }
}

module DlLoadAny(uldat len, const char *name) {
  module Module;

  for (Module = All->FirstModule; Module; Module = Module->Next) {
    if (len == Module->NameLen && !memcmp(name, Module->Name, len))
      /* already loaded! */
      return Module;
  }

  if ((Module = New(module)(len, name))) {
    if (Module->DlOpen())
      return Module;
    Module->Delete();
  }
  return NULL;
}

static module So[MAX_So];

udat DlName2Code(const char *name) {
  if (!strcmp(name, "wm"))
    return WMSo;
  if (!strcmp(name, "term"))
    return TermSo;
  if (!strcmp(name, "socket"))
    return SocketSo;
  if (!strcmp(name, "rcparse"))
    return RCParseSo;
  return MainSo;
}

static const char *DlCode2Name(uldat code) {
  switch (code) {
  case WMSo:
    return "wm";
  case TermSo:
    return "term";
  case SocketSo:
    return "socket";
  case RCParseSo:
    return "rcparse";
  default:
  case MainSo:
    return NULL;
  }
}

module DlLoad(uldat code) {
  module M = (module)0;
  if (code < MAX_So && !(M = So[code])) {
    const char *name = DlCode2Name(code);
    M = DlLoadAny(name ? strlen(name) : 0, name);
    if ((So[code] = M)) {
      if (All->FnHookModule)
        All->FnHookModule(All->HookModule);
    } else {
      printk("failed to load module %s: %s\n", name ? name : "(NULL)",
             Errstr ? Errstr : "unknown error");
    }
  }
  return M;
}

void DlUnload(uldat code) {
  if (code < MAX_So) {
    if (So[code]) {
      So[code]->Delete();
      So[code] = NULL;
      if (All->FnHookModule)
        All->FnHookModule(All->HookModule);
    }
  }
}

module DlIsLoaded(uldat code) {
  if (code < MAX_So)
    return So[code];
  return (module)0;
}

void *DlSym(module Module, const char *name) {
  if (Module && name)
    return (void *)dlsym((dlhandle)Module->Handle, name);

  return NULL;
}
