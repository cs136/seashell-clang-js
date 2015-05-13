/**
 * Seashell's LLVM interface.
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

/** Note: Taken from PDCLib 0.5+master.  Make sure this stays in sync
 *  with whatever C runtime library we choose to implement.
 */

#ifndef __RUNTIME_H__
#define __RUNTIME_H__

/* C errno values */
#define _SS_ERANGE 1
#define _SS_EDOM   2
#define _SS_EILSEQ 3

/* C++11/POSIX errno values */
#define _SS_E2BIG 4
#define _SS_ECONNRESET 5
#define _SS_EISCONN 6
#define _SS_ENOENT 7
#define _SS_ENOTRECOVERABLE 8
#define _SS_EROFS 9
#define _SS_EACCES 10
#define _SS_EDEADLK 11
#define _SS_EISDIR 12
#define _SS_ENOEXEC 13
#define _SS_ENOTSOCK 14
#define _SS_ESPIPE 15
#define _SS_EADDRINUSE 16
#define _SS_EDESTADDRREQ 17
#define _SS_ELOOP 18
#define _SS_ENOLCK 19
#define _SS_ENOTSUPP 20
#define _SS_ESRCH 21
#define _SS_EADDRNOTAVAIL 22
#define _SS_EMFILE 23
#define _SS_ENOLINK 24
#define _SS_ENOTTY 25
#define _SS_ETIME 26
#define _SS_EAFNOSUPPORT 27
#define _SS_EEXIST 28
#define _SS_EMLINK 29
#define _SS_ENOMEM 30
#define _SS_ENXIO 31
#define _SS_ETIMEDOUT 32
#define _SS_EAGAIN 33
#define _SS_EFAULT 34
#define _SS_EMSGSIZE 35
#define _SS_ENOMSG 36
#define _SS_EOPNOTSUPP 37
#define _SS_ETXTBSY 38
#define _SS_EALREADY 39
#define _SS_EFBIG 40
#define _SS_ENAMETOOLONG 41
#define _SS_ENOPROTOOPT 42
#define _SS_EOVERFLOW 43
#define _SS_EWOULDBLOCK _SS_EAGAIN
#define _SS_EBADF 44
#define _SS_EHOSTUNREACH 45
#define _SS_ENETDOWN 46
#define _SS_ENOSPC 47
#define _SS_EOWNERDEAD 48
#define _SS_EXDEV 49
#define _SS_EBADMSG 50
#define _SS_EIDRM 51
#define _SS_ENETRESET 52
#define _SS_ENOSR 53
#define _SS_EPERM 54
#define _SS_EBUSY 55
#define _SS_ENETUNREACH 56
#define _SS_ENOSTR 57
#define _SS_EPIPE 58
#define _SS_ECANCELED 59
#define _SS_EINPROGRESS 60
#define _SS_ENFILE 61
#define _SS_ENOSYS 62
#define _SS_EPROTO 63
#define _SS_ECHILD 64
#define _SS_EINTR 65
#define _SS_ENOBUFS 66
#define _SS_ENOTCONN 67
#define _SS_EPROTONOSUPPORT 68
#define _SS_ECONNABORTED 69
#define _SS_EINVAL 70
#define _SS_ENODATA 71
#define _SS_ENOTDIR 72
#define _SS_EPROTOTYPE 73

#endif
