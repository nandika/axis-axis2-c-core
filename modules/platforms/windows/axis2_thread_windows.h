/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_THREAD_WINDOWS_H
#define AXIS2_THREAD_WINDOWS_H

#include <windows.h>
#include <process.h>
#include <axis2_thread.h>

#define SHELL_PATH "cmd.exe"

typedef HANDLE axis2_os_thread_t; /* Native thread */
/* Chosen for us by apr_initialize */
DWORD tls_axis2_thread = 0;

struct axis2_thread_t 
{
	HANDLE *td;
    void *data;
    axis2_thread_start_t func;
};

struct axis2_threadattr_t 
{
    int detach;
    size_t stacksize;
};

struct axis2_threadkey_t 
{
	DWORD key;
};

struct axis2_thread_once_t 
{
	long value;
};

#endif  /* AXIS2_THREAD_WINDOWS_H */
