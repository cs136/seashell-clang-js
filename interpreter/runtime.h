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

/** Note: Taken from newlib-1.8.2.  Make sure this stays in sync
 *  with whatever C runtime library we choose to implement.
 */

#ifndef __RUNTIME_H__
#define __RUNTIME_H__

#define _SS_EPERM 1		/* Not super-user */
#define _SS_ENOENT 2	/* No such file or directory */
#define _SS_ESRCH 3		/* No such process */
#define _SS_EINTR 4		/* Interrupted system call */
#define _SS_EIO 5		/* I/O error */
#define _SS_ENXIO 6		/* No such device or address */
#define _SS_E2BIG 7		/* Arg list too long */
#define _SS_ENOEXEC 8	/* Exec format error */
#define _SS_EBADF 9		/* Bad file number */
#define _SS_ECHILD 10	/* No children */
#define _SS_EAGAIN 11	/* No more processes */
#define _SS_ENOMEM 12	/* Not enough core */
#define _SS_EACCES 13	/* Permission denied */
#define _SS_EFAULT 14	/* Bad address */
#define _SS_ENOTBLK 15	/* Block device required */
#define _SS_EBUSY 16	/* Mount device busy */
#define _SS_EEXIST 17	/* File exists */
#define _SS_EXDEV 18	/* Cross-device link */
#define _SS_ENODEV 19	/* No such device */
#define _SS_ENOTDIR 20	/* Not a directory */
#define _SS_EISDIR 21	/* Is a directory */
#define _SS_EINVAL 22	/* Invalid argument */
#define _SS_ENFILE 23	/* Too many open files in system */
#define _SS_EMFILE 24	/* Too many open files */
#define _SS_ENOTTY 25	/* Not a typewriter */
#define _SS_ETXTBSY 26	/* Text file busy */
#define _SS_EFBIG 27	/* File too large */
#define _SS_ENOSPC 28	/* No space left on device */
#define _SS_ESPIPE 29	/* Illegal seek */
#define _SS_EROFS 30	/* Read only file system */
#define _SS_EMLINK 31	/* Too many links */
#define _SS_EPIPE 32	/* Broken pipe */
#define _SS_EDOM 33		/* Math arg out of domain of func */
#define _SS_ERANGE 34	/* Math result not representable */
#define _SS_ENOMSG 35	/* No message of desired type */
#define _SS_EIDRM 36	/* Identifier removed */
#define _SS_ECHRNG 37	/* Channel number out of range */
#define _SS_EL2NSYNC 38	/* Level 2 not synchronized */
#define _SS_EL3HLT 39	/* Level 3 halted */
#define _SS_EL3RST 40	/* Level 3 reset */
#define _SS_ELNRNG 41	/* Link number out of range */
#define _SS_EUNATCH 42	/* Protocol driver not attached */
#define _SS_ENOCSI 43	/* No CSI structure available */
#define _SS_EL2HLT 44	/* Level 2 halted */
#define _SS_EDEADLK 45	/* Deadlock condition */
#define _SS_ENOLCK 46	/* No record locks available */
#define _SS_EBADE 50	/* Invalid exchange */
#define _SS_EBADR 51	/* Invalid request descriptor */
#define _SS_EXFULL 52	/* Exchange full */
#define _SS_ENOANO 53	/* No anode */
#define _SS_EBADRQC 54	/* Invalid request code */
#define _SS_EBADSLT 55	/* Invalid slot */
#define _SS_EDEADLOCK 56	/* File locking deadlock error */
#define _SS_EBFONT 57	/* Bad font file fmt */
#define _SS_ENOSTR 60	/* Device not a stream */
#define _SS_ENODATA 61	/* No data (for no delay io) */
#define _SS_ETIME 62	/* Timer expired */
#define _SS_ENOSR 63	/* Out of streams resources */
#define _SS_ENONET 64	/* Machine is not on the network */
#define _SS_ENOPKG 65	/* Package not installed */
#define _SS_EREMOTE 66	/* The object is remote */
#define _SS_ENOLINK 67	/* The link has been severed */
#define _SS_EADV 68		/* Advertise error */
#define _SS_ESRMNT 69	/* Srmount error */
#define _SS_ECOMM 70	/* Communication error on send */
#define _SS_EPROTO 71	/* Protocol error */
#define _SS_EMULTIHOP 74	/* Multihop attempted */
#define _SS_ELBIN 75	/* Inode is remote (not really error) */
#define _SS_EDOTDOT 76	/* Cross mount point (not really error) */
#define _SS_EBADMSG 77	/* Trying to read unreadable message */
#define _SS_ENOTUNIQ 80	/* Given log. name not unique */
#define _SS_EBADFD 81	/* f.d. invalid for this operation */
#define _SS_EREMCHG 82	/* Remote address changed */
#define _SS_ELIBACC 83	/* Can't access a needed shared lib */
#define _SS_ELIBBAD 84	/* Accessing a corrupted shared lib */
#define _SS_ELIBSCN 85	/* .lib section in a.out corrupted */
#define _SS_ELIBMAX 86	/* Attempting to link in too many libs */
#define _SS_ELIBEXEC 87	/* Attempting to exec a shared library */
#define _SS_ENOSYS 88	/* Function not implemented */
#define _SS_ENMFILE 89      /* No more files */
#define _SS_ENOTEMPTY 90	/* Directory not empty */
#define _SS_ENAMETOOLONG 91	/* File or path name too long */
#define _SS_ELOOP 92	/* Too many symbolic links */
#define _SS_EOPNOTSUPP 95	/* Operation not supported on transport endpoint */
#define _SS_EPFNOSUPPORT 96 /* Protocol family not supported */
#define _SS_ECONNRESET 104  /* Connection reset by peer */
#define _SS_ENOBUFS 105	/* No buffer space available */
#define _SS_EAFNOSUPPORT 106
#define _SS_EPROTOTYPE 107
#define _SS_ENOTSOCK 108
#define _SS_ENOPROTOOPT 109
#define _SS_ESHUTDOWN 110
#define _SS_ECONNREFUSED 111	/* Connection refused */
#define _SS_EADDRINUSE 112		/* Address already in use */
#define _SS_ECONNABORTED 113	/* Connection aborted */
#define _SS_ENETUNREACH 114
#define _SS_ENETDOWN 115
#define _SS_ETIMEDOUT 116
#define _SS_EHOSTDOWN 117
#define _SS_EHOSTUNREACH 118
#define _SS_EINPROGRESS 119
#define _SS_EALREADY 120
#define _SS_EDESTADDRREQ 121
#define _SS_EMSGSIZE 122
#define _SS_EPROTONOSUPPORT 123
#define _SS_ESOCKTNOSUPPORT 124
#define _SS_EADDRNOTAVAIL 125
#define _SS_ENETRESET 126
#define _SS_EISCONN 127
#define _SS_ENOTCONN 128
#define _SS_ETOOMANYREFS 129
#define _SS_EPROCLIM 130
#define _SS_EUSERS 131
#define _SS_EDQUOT 132
#define _SS_ESTALE 133
#define _SS_ENOTSUP 134
#define _SS_ENOMEDIUM 135

/* From cygwin32.  */
#define _SS_EWOULDBLOCK EAGAIN	/* Operation would block */

#endif
