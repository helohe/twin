/* Copyright (C) 2000 by Massimiliano Ghilardi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 *  parts of code taken from:
 *
 *  rxvt-2.4.7 sources (various authors)
 *  available at ftp://ftp.math.fu-berlin.de/pub/rxvt/
 *  and usual Linux mirrors
 *
 *
 * other parts from Twin autor:
 *
 *  Copyright (C) 1993-1999  Massimiliano Ghilardi
 *
 */

/* tell <stdlib.h> to declare grantpt(), unlockpt(), ptsname() */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#include "twautoconf.h" /* for TW_HAVE* macros */
#include "twconfig.h"   /* for CONF_* macros */

#ifdef TW_HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef TW_HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef TW_HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef TW_HAVE_GRP_H
#include <grp.h>
#endif
#ifdef TW_HAVE_TERMIOS_H
#include <termios.h>
#else
#ifdef TW_HAVE_TERMIO_H
#include <termio.h>
#endif
#endif
#ifdef TW_HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ifdef TW_HAVE_SIGNAL_H
#include <signal.h>
#endif

#include "tty_ioctl.h"

#include "twin.h"
#include "data.h"
#include "main.h"
#include "printk.h"
#include "util.h"

/* pseudo-teletype connections handling functions */

/* 0. variables */
static char *ptydev, *ttydev;
static int ptyfd, ttyfd;

static void pty_error(const char *d, const char *f, const char *arg) {
  printk("twin: " SS ": " SS "(\"" SS "\") failed: " SS "\n", d ? d : "<NULL>", f ? f : "<NULL>",
         arg ? arg : "<NULL>", strerror(errno));
}

static void get_pty_error(const char *f, const char *arg) {
  pty_error("opening pseudo-tty", f, arg);
}

/* 1. Acquire a pseudo-teletype from the system. */
/*
 * On failure, returns tfalse.
 * On success, fills ttydev and ptydev with the names of the
 * master and slave parts and sets ptyfd to the pty file descriptor
 */
static byte get_pty(void) {
  int fd = -1, sfd = -1;
#ifdef CONF_TERM_DEVPTS

  /* open master pty */
  if (
#ifdef TW_HAVE_GETPT
      (fd = getpt()) >= 0
#else
      (fd = open("/dev/ptmx", O_RDWR | O_NOCTTY)) >= 0
#endif
  ) {

    if (grantpt(fd) == 0) {
      if (unlockpt(fd) == 0) {
        char *devname = ptsname(fd);
        ptydev = ttydev = devname;
        if ((sfd = open(devname, O_RDWR | O_NOCTTY)) >= 0)
          goto Found;
        else
          get_pty_error("slave open", devname);
      } else
        get_pty_error("unlockpt", "");
    } else
      get_pty_error("grantpt", "");

    close(fd);
  } else
    get_pty_error(
#ifdef TW_HAVE_GETPT
        "getpt", ""
#else
        "open", "/dev/ptmx"
#endif
    );

#else
  static char pty_name[] = "/dev/pty??";
  static char tty_name[] = "/dev/tty??";
  int len = strlen(tty_name);
  char *c1, *c2;

  ptydev = pty_name;
  ttydev = tty_name;

#define PTYCHAR1 "pqrstuvwxyzabcde"
#define PTYCHAR2 "0123456789abcdef"
  for (c1 = PTYCHAR1; *c1; c1++) {
    ptydev[len - 2] = ttydev[len - 2] = *c1;
    for (c2 = PTYCHAR2; *c2; c2++) {
      ptydev[len - 1] = ttydev[len - 1] = *c2;
      if ((fd = open(ptydev, O_RDWR | O_NOCTTY)) >= 0) {
        if ((sfd = open(ttydev, O_RDWR | O_NOCTTY)) >= 0)
          /* access(ttydev, R_OK|W_OK) won't do as it checks against REAL uid */
          goto Found;
        close(fd);
      }
    }
  }
  printk("twin: failed to get a pty/tty pseudo-tty pair\n");

#endif
  return tfalse;

Found:
  fcntl(fd, F_SETFL, O_NDELAY);
  fcntl(fd, F_SETFD, FD_CLOEXEC);
  ttyfd = sfd;
  ptyfd = fd;
  return ttrue;
}

/* 2. Fixup permission for pty master/slave pairs */
static byte fixup_pty(void) {
  /* from util.c */
  extern gid_t get_tty_grgid(void);

  uid_t id = getuid();
  gid_t tty_gid = get_tty_grgid();

  if (tty_gid != (gid_t)-1 &&
#ifndef CONF_TERM_DEVPTS
      chown(ptydev, id, 0) == 0 && chmod(ptydev, 0600) == 0 &&
#endif
      chown(ttydev, id, tty_gid) == 0 && chmod(ttydev, 0620) == 0)
    return ttrue;
  return tfalse;
}

static void setup_pty_error(const char *f, const char *arg) {
  pty_error("setting up slave tty", f, arg);
}

/* 3. Setup tty size and termios settings */
/*
 * do it before the fork() and NOT in the child to avoid
 * races with future tty resizes performed by the parent!
 */
static byte setup_tty(ttydata *Data) {
  struct winsize wsiz;
  /* from hw.c, ttysave is the console original state */
  extern struct termios ttysave;

  if (All->SetUp->Flags & setup_terminals_utf8)
    Data->utf8 = 1;

  wsiz.ws_col = Data->SizeX;
  wsiz.ws_row = Data->SizeY;
  wsiz.ws_xpixel = 0;
  wsiz.ws_ypixel = 0;

  if (ioctl(ptyfd, TIOCSWINSZ, &wsiz) >= 0) {
    if (tty_setioctl(ttyfd, &ttysave) >= 0)
      return ttrue;
    else
      setup_pty_error("tty_setioctl", "");
  } else
    setup_pty_error("ioctl", "TIOCSWINSZ");
  return tfalse;
}

/* 4. Establish ttyfd as controlling teletype for new session and switch to it */
static byte switchto_tty(void) {
  int i;
  pid_t pid;

  pid = setsid();
  if (pid < 0)
    return tfalse;

  /*
   * Hope all other file descriptors are set to fcntl(fd, F_SETFD, FD_CLOEXEC)
   */
  for (i = 0; i <= 2; i++) {
    if (i != ttyfd) {
      close(i);
      dup2(ttyfd, i);
    }
  }
  if (ttyfd > 2)
    close(ttyfd);

#ifdef TIOCSCTTY
  ioctl(0, TIOCSCTTY, 0);
#endif

/* set process group */
#if defined(_POSIX_VERSION) || defined(__svr4__)
  tcsetpgrp(0, pid);
#elif defined(TIOCSPGRP)
  ioctl(0, TIOCSPGRP, &pid);
#endif

  return ttrue;
}

/* exported API: fork() a program in a pseudo-teletype */
byte SpawnInWindow(window Window, const char *arg0, const char *const *argv) {
  pid_t childpid;
  remotedata *data;

  /* 0 */
  if (flag_secure) {
    printk(flag_secure_msg);
    return tfalse;
  }
  GainPrivileges();

  /* 1 */
  if (!get_pty()) {
    DropPrivileges();
    return tfalse;
  }
  /* 2 */
  (void)fixup_pty();
  DropPrivileges();

  /* 3 */
  if (setup_tty(Window->USE.C.TtyData)) {
    switch ((childpid = fork())) {
    case -1:
      /* failed */
      close(ptyfd);
      ptyfd = -1;
      break;
    case 0:
      /* child */
      /* 4 */
      if (switchto_tty())
        execvp(arg0, (char *const *)RemoveConst(argv));
      exit(1);
      break;
    default:
      /* father */
      data = &Window->RemoteData;
      data->Fd = ptyfd;
      data->ChildPid = childpid;
      break;
    }
  } else {
    close(ptyfd);
    ptyfd = -1;
  }
  close(ttyfd);
  return ptyfd != -1;
}
