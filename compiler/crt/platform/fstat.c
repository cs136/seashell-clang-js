/**
 * Seashell's C Runtime Library (C bindings)
 * Copyright (C) 2013-2015 The Seashell Maintainers.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * See also 'ADDITIONAL TERMS' at the end of the included LICENSE file.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "seashell-runtime.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#undef errno
extern int errno;

int _fstat(int fd, struct stat* st) {
  int32_t mode; uint64_t size; int64_t mtime, atime, ctime;
  int syscall_result = _seashell_RT_fstat(fd, &mode, &size, &mtime, &atime, &ctime);
  memset(st, 0, sizeof(struct stat));
  st->st_mode = mode;
  st->st_size = size;
  st->st_mtime = mtime;
  st->st_atime = atime;
  st->st_ctime = ctime;
  RT_RESULT(syscall_result);
}
