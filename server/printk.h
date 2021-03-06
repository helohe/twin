/* Copyright (C) 2000 by Massimiliano Ghilardi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef TWIN_PRINTK_H
#define TWIN_PRINTK_H

#ifndef SS
#define SS "%." STR(TW_SMALLBUFF) "s"
#endif

int printk(const char *format, ...);
void printk_str(int len, const char *s);
int printk_receive_fd(int fd);
int flushk(void);

byte RegisterPrintk(int fd);
void UnRegisterPrintk(void);

#endif /* TWIN_PRINTK_H */
