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
#ifndef __SEASHELL_RUNTIME__
#define __SEASHELL_RUNTIME__

#ifdef __cplusplus
extern "C" {
#endif

#define __need_ptrdiff_t
#define __need_size_t
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

int _seashell_RT_exit(int result) __attribute__((noreturn));
int _seashell_RT_close(int fd);
int _seashell_RT_open(const char* name, int flags, int mode);
int _seashell_RT_read(int fd, void* ptr, size_t len);
int _seashell_RT_write(int fd, const void* ptr, size_t len);
int _seashell_RT_isatty(int fd);
int _seashell_RT_link(const char *old, const char *newl);
int _seashell_RT_lseek(int file, ssize_t ptr, int dir);
void* _seashell_RT_sbrk(ptrdiff_t incr);
int _seashell_RT_unlink(const char *name);
int _seashell_RT_stat(const char *name, int32_t *mode, uint64_t *size, int64_t *mtime, int64_t *atime, int64_t *ctime);
int _seashell_RT_fstat(int fd, int32_t *mode, uint64_t *size, int64_t* mtime, int64_t *atime, int64_t *ctime);
int _seashell_RT_gettimeofday(int64_t *seconds, int64_t *microseconds, int *correction, int *dst);

#ifdef _COMPILING_NEWLIB
#define RT_RESULT(x) \
do { \
  int result = (x); \
  if (result < 0) { \
    errno = -result; \
    return -1; \
  } else { \
    return result; \
  }\
} while(0)
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif

