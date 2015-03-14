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
// Module: stdext/posix/popen/windows.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2010-2-2 1:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_POSIX_POPEN_WINDOWS_H
#define STDEXT_POSIX_POPEN_WINDOWS_H

#ifndef STDEXT_BASIC_H
#include "../../Basic.h"
#endif

#ifndef _INC_FCNTL
#include <fcntl.h>
#endif

#ifndef _INC_IO
#include <io.h>
#endif

#if (0)
#define STDEXT_POPEN_NO_WINDOW
#endif

// -------------------------------------------------------------------------
// function popen_ex

struct PFILE
{
	FILE* stream;
	HANDLE prochnd;
	
	operator FILE* () const {
		return stream;
	}
};

namespace detail {

inline HANDLE dupHandle(HANDLE fh, BOOL inherit)
{
	HANDLE copy, self = GetCurrentProcess();
	if (!DuplicateHandle(self, fh, self, &copy, 0, inherit, DUPLICATE_SAME_ACCESS|DUPLICATE_CLOSE_SOURCE)) {
		return NULL;
	}
	return copy;
}

inline PFILE popen_ex(const char *command, const char *type, const char *cwd, char *env)
{
	using namespace detail;

	static const char* const comspec = (GetVersion() < 0x80000000 ? "cmd.exe" : "command.com");

	FILE *stream = NULL;
	int fno, str_len = strlen(type), read, mode;
	STARTUPINFO startup;
	PROCESS_INFORMATION process;
	SECURITY_ATTRIBUTES security;
	HANDLE in, out;
	DWORD dwCreateFlags = 0;
	char *cmd;

	security.nLength				= sizeof(SECURITY_ATTRIBUTES);
	security.bInheritHandle			= TRUE;
	security.lpSecurityDescriptor	= NULL;

	if (!str_len || !CreatePipe(&in, &out, &security, 2048L)) {
		PFILE fp = { 0, 0 };
		return fp;
	}

	memset(&startup, 0, sizeof(STARTUPINFO));
	memset(&process, 0, sizeof(PROCESS_INFORMATION));

	startup.cb			= sizeof(STARTUPINFO);
	startup.dwFlags		= STARTF_USESTDHANDLES;
	startup.hStdError	= GetStdHandle(STD_ERROR_HANDLE);

	read = (type[0] == 'r') ? TRUE : FALSE;
	mode = ((str_len == 2) && (type[1] == 'b')) ? O_BINARY : O_TEXT;

	if (read) {
		in = dupHandle(in, FALSE);
		startup.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
		startup.hStdOutput = out;
	} else {
		out = dupHandle(out, FALSE);
		startup.hStdInput  = in;
		startup.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	dwCreateFlags = NORMAL_PRIORITY_CLASS;

#if defined(STDEXT_POPEN_NO_WINDOW)
	dwCreateFlags |= CREATE_NO_WINDOW;
#endif

	cmd = (char*)malloc(strlen(command)+strlen(comspec)+sizeof(" /c ")+2);
	sprintf(cmd, "%s /c \"%s\"", comspec, command);
	if (!CreateProcess(NULL, cmd, &security, &security, security.bInheritHandle, dwCreateFlags, env, cwd, &startup, &process)) {
		free(cmd);
		PFILE fp = { 0, 0 };
		return fp;
	}
	free(cmd);

	CloseHandle(process.hThread);

	if (read) {
		fno = _open_osfhandle((intptr_t)in, _O_RDONLY | mode);
		CloseHandle(out);
	} else {
		fno = _open_osfhandle((intptr_t)out, _O_WRONLY | mode);
		CloseHandle(in);
	}

	stream = _fdopen(fno, type);

	PFILE proc = { stream, process.hProcess };
	return proc;
}

} // namespace detail

// -------------------------------------------------------------------------
// function popen

inline PFILE popen(const char *command, const char *type)
{
	return detail::popen_ex(command, type, NULL, NULL);
}

// -------------------------------------------------------------------------
// function pclose

inline int pclose(PFILE proc)
{
	DWORD termstat = 0;

	fflush(proc.stream);
	fclose(proc.stream);

	WaitForSingleObject(proc.prochnd, INFINITE);
	GetExitCodeProcess(proc.prochnd, &termstat);
	CloseHandle(proc.prochnd);

	return termstat;
}

// -------------------------------------------------------------------------

#endif /* STDEXT_POSIX_POPEN_WINDOWS_H */
