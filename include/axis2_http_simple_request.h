/*
* Licensed to the Apache Software Foundation (ASF) under one or more
* contributor license agreements.  See the NOTICE file distributed with
* this work for additional information regarding copyright ownership.
* The ASF licenses this file to You under the Apache License, Version 2.0
* (the "License"); you may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef AXIS2_HTTP_SIMPLE_REQUEST_H
#define AXIS2_HTTP_SIMPLE_REQUEST_H

/**
 * @defgroup axis2_http_simple_request http simple request
 * @ingroup axis2_core_trans_http
 * @{
 */

/**
 * @file axis2_http_simple_request.h
 * @brief axis2 HTTP Simple Request
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axutil_env.h>
#include <axis2_http_request_line.h>
#include <axis2_http_header.h>
#include <axutil_stream.h>
#include <axutil_array_list.h>


#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_http_simple_request */
    typedef struct axis2_http_simple_request axis2_http_simple_request_t;

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axis2_http_request_line_t *AXIS2_CALL
    axis2_http_simple_request_get_request_line(const axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     * @param request_line pointer to request line
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_simple_request_set_request_line(axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env,
        axis2_http_request_line_t *request_line);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     * @param name pointer to name
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_http_simple_request_contains_header(axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env,
        const axis2_char_t *name);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
    axis2_http_simple_request_get_headers(const axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     * @param str pointer to str
     */
    AXIS2_EXTERN axis2_http_header_t *AXIS2_CALL
    axis2_http_simple_request_get_first_header(const axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env,
        const axis2_char_t *str);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     * @param str pointer to str
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_simple_request_remove_headers(axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env,
        const axis2_char_t *str);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     * @param header pointer to header
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_simple_request_add_header(axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env,
        axis2_http_header_t *header);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
    axis2_http_simple_request_get_content_type(const axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
    axis2_http_simple_request_get_charset(const axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axis2_ssize_t AXIS2_CALL
    axis2_http_simple_request_get_content_length(const axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axutil_stream_t *AXIS2_CALL
    axis2_http_simple_request_get_body(const axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     * @param buf double pointer to buf
     */
    AXIS2_EXTERN axis2_ssize_t AXIS2_CALL
    axis2_http_simple_request_get_body_bytes(const axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env,
        char **buf);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     * @param str pointer to str
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_http_simple_request_set_body_string(axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env,
        void *str,
        unsigned int str_len);

    /**
     * @param simple_request pointer to simple request
     * @param env pointer to environment struct
     * @return void
     */
    AXIS2_EXTERN void AXIS2_CALL
    axis2_http_simple_request_free(axis2_http_simple_request_t *simple_request,
        const axutil_env_t *env);

    /**
     * @param env pointer to environment struct
     * @param request_line pointer to request line
     * @param http_headers double pointer to http headers
     * @param http_hdr_count
     * @param content pointer to content
     */
    AXIS2_EXTERN axis2_http_simple_request_t *AXIS2_CALL
    axis2_http_simple_request_create (const axutil_env_t *env,
        axis2_http_request_line_t *request_line,
        axis2_http_header_t **http_headers,
        axis2_ssize_t http_hdr_count,
        axutil_stream_t *content);

/** Gets the request line. */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(simple_request, env) \
                            axis2_http_simple_request_get_request_line\
                            (simple_request, env)

/** Set request line. */
#define AXIS2_HTTP_SIMPLE_REQUEST_SET_REQUEST_LINE\
                            (simple_request, env, request_line) \
                            axis2_http_simple_request_set_request_line\
                            (simple_request, env, request_line)

/** Contains header. */
#define AXIS2_HTTP_SIMPLE_REQUEST_CONTAINS_HEADER(simple_request, env, name)\
                            axis2_http_simple_request_contains_header\
                            (simple_request, env, name)

/** Get headers. */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_HEADERS(simple_request, env)\
                            axis2_http_simple_request_get_headers(simple_request, env)

/** Gets the first header. */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_FIRST_HEADER(simple_request, env, str) \
                            axis2_http_simple_request_get_first_header\
                            (simple_request, env, str)

/** Remove headers. */
#define AXIS2_HTTP_SIMPLE_REQUEST_REMOVE_HEADERS(simple_request, env, str) \
                            axis2_http_simple_request_remove_headers\
                            (simple_request, env, str)

/** Add header. */
#define AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(simple_request, env, header) \
                            axis2_http_simple_request_add_header\
                            (simple_request, env, header)

/** Gets the content type. */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_TYPE(simple_request, env)\
                            axis2_http_simple_request_get_content_type\
                            (simple_request, env)

/** Gets the charset. */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_CHARSET(simple_request, env) \
                            axis2_http_simple_request_get_charset\
                            (simple_request, env)

/** Gets the content length. */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_LENGTH(simple_request, env)\
                            axis2_http_simple_request_get_content_length\
                            (simple_request, env)

/** Gets the body. */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_BODY(simple_request, env)\
                            axis2_http_simple_request_get_body\
                            (simple_request, env)

/** Get body bytes. */
#define AXIS2_HTTP_SIMPLE_REQUEST_GET_BODY_BYTES(simple_request, env, buf)\
                            axis2_http_simple_request_get_body_bytes\
                            (simple_request, env, buf)

/** Sets the body string. */
#define AXIS2_HTTP_SIMPLE_REQUEST_SET_BODY_STRING(simple_request, env, str, str_len)\
                            axis2_http_simple_request_set_body_string\
                            (simple_request, env, str, str_len)

/** Frees the http simple request. */
#define AXIS2_HTTP_SIMPLE_REQUEST_FREE(simple_request, env) \
                            axis2_http_simple_request_free(simple_request, env)

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_SIMPLE_REQUEST_H */
