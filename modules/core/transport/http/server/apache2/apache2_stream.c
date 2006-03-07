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

#include <string.h>
#include <stdlib.h>
#include <apache2_stream.h>
#include <http_protocol.h>

/** 
 * @brief Stream struct impl
 *	Streaming mechanisms for Apache2 web server 
 */
typedef struct apache2_stream_impl apache2_stream_impl_t;
  
struct apache2_stream_impl
{
	axis2_stream_t stream; 
    request_rec *request;   
};

#define AXIS2_INTF_TO_IMPL(stream) ((apache2_stream_impl_t *)(stream))

/********************************Function headers******************************/
axis2_status_t AXIS2_CALL 
apache2_stream_free (axis2_stream_t *stream, axis2_env_t **env);

int AXIS2_CALL
apache2_stream_write(axis2_stream_t *stream, axis2_env_t **env, 
						const void *buffer, size_t count);
int AXIS2_CALL 
apache2_stream_read(axis2_stream_t *stream, axis2_env_t **env, 
						void *buffer, size_t count);
int AXIS2_CALL 
apache2_stream_skip(axis2_stream_t *stream, axis2_env_t **env, int count);

int AXIS2_CALL 
apache2_stream_get_char(axis2_stream_t *stream, axis2_env_t **env);

/************************* End of function headers ****************************/
/*
 * Internal function. Not exposed to outside
 */
AXIS2_DECLARE(axis2_stream_t *)
axis2_stream_create_apache2(axis2_env_t **env, request_rec *request)
{
    apache2_stream_impl_t *stream_impl = NULL;
	AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, request, NULL);
    	
    stream_impl = (apache2_stream_impl_t *)AXIS2_MALLOC(
                        (*env)->allocator, sizeof(apache2_stream_impl_t));
	
	if(NULL == stream_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    stream_impl->request = request;
	stream_impl->stream.ops = (axis2_stream_ops_t *) AXIS2_MALLOC (
                        (*env)->allocator, sizeof (axis2_stream_ops_t));
	if (NULL == stream_impl->stream.ops)
	{
		apache2_stream_free(&(stream_impl->stream), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return NULL;
	}
    	
	stream_impl->stream.ops->free = apache2_stream_free;
    stream_impl->stream.ops->read = apache2_stream_read;
    stream_impl->stream.ops->write = apache2_stream_write;
    stream_impl->stream.ops->skip = apache2_stream_skip;
    stream_impl->stream.ops->get_char = apache2_stream_get_char;
    
	return &(stream_impl->stream);
}


axis2_status_t AXIS2_CALL
apache2_stream_free (axis2_stream_t *stream, axis2_env_t **env)
{
    apache2_stream_impl_t *stream_impl = NULL;
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	
	stream_impl = AXIS2_INTF_TO_IMPL(stream);
	if (NULL != stream_impl->stream.ops)
    {
        AXIS2_FREE ((*env)->allocator, stream_impl->stream.ops);
    }
   	AXIS2_FREE((*env)->allocator, stream_impl);
	
    return AXIS2_SUCCESS;
}

int AXIS2_CALL 
apache2_stream_read(axis2_stream_t *stream, axis2_env_t **env, 
						void *buffer, size_t count)
{
    apache2_stream_impl_t *stream_impl = NULL;
	AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
    
    stream_impl = AXIS2_INTF_TO_IMPL(stream);
    return ap_get_client_block(stream_impl->request, buffer, count);
}

int AXIS2_CALL
apache2_stream_write(axis2_stream_t *stream, axis2_env_t **env, 
						const void *buf, size_t count)
{
	apache2_stream_impl_t *stream_impl = NULL;
    char *write_buf = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, buf, AXIS2_FAILURE);
    stream_impl = AXIS2_INTF_TO_IMPL(stream);
    axis2_char_t *buffer = (axis2_char_t*)buf;
    if(count <= 0)
    {
        return count;
    }
    /* we need to NULL terminate the buffer if it is not already done so */
    if(buffer[count-1] != '\0')
    {
        int len = -1;
        write_buf = AXIS2_MALLOC((*env)->allocator, count + 1);
        if(NULL == write_buf)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, 
                        AXIS2_FAILURE);
            return -1;
        }
        memcpy(write_buf, buffer, count);
        write_buf[count] = '\0';
        len = ap_rputs(write_buf, stream_impl->request);
        AXIS2_FREE((*env)->allocator, write_buf);
        return len;
    }
    else
    {
        write_buf = buffer;
        return ap_rputs(write_buf, stream_impl->request);
    }
    /* we shoudn't come here unless a serious problem*/
    return -1;
}


int AXIS2_CALL 
apache2_stream_skip(axis2_stream_t *stream, axis2_env_t **env, int count)
{
	apache2_stream_impl_t *stream_impl = NULL;
    axis2_char_t *tmp_buffer = NULL;
    int len = -1;
	AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
	stream_impl = AXIS2_INTF_TO_IMPL(stream);
    
    tmp_buffer = AXIS2_MALLOC((*env)->allocator, count * sizeof(axis2_char_t));
    if(tmp_buffer == NULL)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, 
                          AXIS2_FAILURE);
        return -1;
    }
    len = ap_get_client_block(stream_impl->request, tmp_buffer, count);
    AXIS2_FREE((*env)->allocator, tmp_buffer);
    return len;
    
}

int AXIS2_CALL 
apache2_stream_get_char(axis2_stream_t *stream, axis2_env_t **env)
{
	int ret = -1;
	AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
    
    /* TODO implement this */
    return ret;
}