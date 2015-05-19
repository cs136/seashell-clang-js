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

int32_t _seashell_RT_exit(int32_t result) __attribute__((noreturn));
int32_t _seashell_RT_close(int32_t fd);
int32_t _seashell_RT_open(const char* name, int32_t flags, int32_t mode);
int32_t _seashell_RT_read(int32_t fd, void* ptr, uint32_t len);
int32_t _seashell_RT_write(int32_t fd, const void* ptr, uint32_t len);
int32_t _seashell_RT_isatty(int32_t fd);
int32_t _seashell_RT_link(const char *old, const char *newl);
int32_t _seashell_RT_lseek(int32_t file, int64_t ptr, int32_t dir);
void* _seashell_RT_brk_base(void);
int32_t _seashell_RT_brk(void* target);
int32_t _seashell_RT_unlink(const char *name);
int32_t _seashell_RT_stat(const char *name, int32_t *mode, uint64_t *size, int64_t *mtime, int64_t *atime, int64_t *ctime);
int32_t _seashell_RT_fstat(int32_t fd, int32_t *mode, uint64_t *size, int64_t* mtime, int64_t *atime, int64_t *ctime);
int32_t _seashell_RT_gettimeofday(int64_t *seconds, int64_t *microseconds, int32_t *correction, int32_t *dst);

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

