/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/posix/unistd.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2010-2-2 1:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_POSIX_UNISTD_H
#define STDEXT_POSIX_UNISTD_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

// -------------------------------------------------------------------------

#if defined(X_OS_WINDOWS)

#ifndef _INC_IO
#include <io.h>
#endif

#ifndef _INC_FCNTL
#include <fcntl.h>
#endif

inline int winx_call ftruncate(int fd, off_t length)
{
#if defined(X_CC_VC6)
	return _chsize(fd, length);
#else
	const errno_t e = _chsize_s(fd, length);
	return e == 0 ? 0 : -1;
#endif
}

inline int winx_call truncate(const char* path, off_t length) /* Truncate PATH to LENGTH bytes. */
{
	int fd, ret, save;

	fd = _open(path, O_WRONLY);
	if (fd < 0)
		return -1;

	ret = ftruncate(fd, length);
	save = errno;
	_close(fd);
	if (ret < 0)
		errno = save;
	
	return ret;
}

#else

#ifndef	_UNISTD_H
#include <unistd.h>
#endif

#ifndef	_SYS_TYPES_H
#include <sys/types.h>
#endif

#endif

// -------------------------------------------------------------------------

#endif /* STDEXT_POSIX_UNISTD_H */
