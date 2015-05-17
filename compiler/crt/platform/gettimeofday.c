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
#include <sys/time.h>
#include <errno.h>
#undef errno
extern int errno;

int _gettimeofday(struct timeval *p, struct timezone *z) {
  int64_t seconds, microseconds;
  int correction, dst;
  int syscall_result = _seashell_RT_gettimeofday(&seconds, &microseconds, &correction, &dst);
  if (p) {
    p->tv_sec = seconds;
    p->tv_usec = microseconds;
  }
  if (z) {
    z->tz_minuteswest = correction;
    z->tz_dsttime = dst;
  }
  RT_RESULT(syscall_result);
}
