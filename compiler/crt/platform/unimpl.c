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
#include <sys/times.h>
#include <errno.h>
#undef errno
extern int errno;

int _execve(char* name, char** argv, char** env) {
  RT_RESULT(-ENOMEM);
}

int _fork(void) {
  RT_RESULT(-EAGAIN);
}

int _getpid(void) {
  return 1;
}

int _wait(int pid) {
  RT_RESULT(-ECHILD);
}

int _times(struct tms* buf) {
  RT_RESULT(-ENOSYS);
}

int _kill(int pid, int sig) {
  if (pid == _getpid()) {
    _seashell_RT_exit(128 + sig);
    RT_RESULT(-EINVAL);
  } else {
    RT_RESULT(-EINVAL);
  }
}

int _fcntl(int fd, int cmd, ... ) {
  RT_RESULT(-ENOSYS);
}
