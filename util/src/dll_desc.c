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

#include <axis2_dll_desc.h>
#include <axis2_class_loader.h>

struct axis2_dll_desc
{
    axis2_char_t *dll_name;
    axis2_char_t *path_qualified_dll_name;
    axis2_dll_type_t dll_type;
    int load_options;
    AXIS2_DLHANDLER dl_handler;
    CREATE_FUNCT create_funct;
    DELETE_FUNCT delete_funct;
    AXIS2_TIME_T timestamp;
    axis2_error_codes_t error_code;


};

AXIS2_EXTERN axis2_dll_desc_t * AXIS2_CALL
axis2_dll_desc_create(const axis2_env_t *env)
{
    axis2_dll_desc_t *dll_desc = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    dll_desc = (axis2_dll_desc_t *) AXIS2_MALLOC(env->allocator
            , sizeof(axis2_dll_desc_t));

    if (NULL == dll_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    dll_desc->dll_name = NULL;
    dll_desc->path_qualified_dll_name = NULL;
    dll_desc->dll_type = 0;
    dll_desc->load_options = 0;
    dll_desc->dl_handler = NULL;
    dll_desc->create_funct = NULL;
    dll_desc->delete_funct = NULL;
    dll_desc->timestamp = 0;
    dll_desc->error_code = AXIS2_ERROR_NONE;

    return dll_desc;
}

/******************************************************************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_dll_desc_free(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (dll_desc->dl_handler)
    {
        axis2_class_loader_delete_dll(env, dll_desc);
    }

    if (dll_desc->dll_name)
    {
        AXIS2_FREE(env->allocator, dll_desc->dll_name);
        dll_desc->dll_name = NULL;
    }

    if (dll_desc->path_qualified_dll_name)
    {
        AXIS2_FREE(env->allocator, dll_desc->path_qualified_dll_name);
        dll_desc->path_qualified_dll_name = NULL;
    }

    dll_desc->create_funct = NULL;
    dll_desc->delete_funct = NULL;

    if (dll_desc)
    {
        AXIS2_FREE(env->allocator, dll_desc);
        dll_desc = NULL;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_dll_desc_free_void_arg(void *dll_desc,
        const axis2_env_t *env)
{
    axis2_dll_desc_t *dll_desc_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    dll_desc_l = (axis2_dll_desc_t *) dll_desc;
    return axis2_dll_desc_free(dll_desc_l, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_dll_desc_set_name(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env,
        axis2_char_t *name)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);

    if (dll_desc->path_qualified_dll_name)
    {
        AXIS2_FREE(env->allocator, dll_desc->path_qualified_dll_name);
        dll_desc->path_qualified_dll_name = NULL;
    }
    dll_desc->path_qualified_dll_name = AXIS2_STRDUP(name, env);
    if (!dll_desc->path_qualified_dll_name)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_dll_desc_get_name(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return dll_desc->path_qualified_dll_name;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_dll_desc_set_load_options(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env,
        int options)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    dll_desc->load_options = options;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_dll_desc_set_type(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env,
        axis2_dll_type_t type)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    dll_desc->dll_type = type;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_dll_type_t AXIS2_CALL
axis2_dll_desc_get_type(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return dll_desc->dll_type;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_dll_desc_get_load_options(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return dll_desc->load_options;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_dll_desc_set_dl_handler(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env,
        AXIS2_DLHANDLER dl_handler)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dl_handler, AXIS2_FAILURE);

    if (dll_desc->dl_handler)
    {
        AXIS2_FREE(env->allocator, dll_desc->dl_handler);
        dll_desc->dl_handler = NULL;
    }
    dll_desc->dl_handler = dl_handler;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN AXIS2_DLHANDLER AXIS2_CALL
axis2_dll_desc_get_dl_handler(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return dll_desc->dl_handler;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_dll_desc_set_create_funct(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env,
        CREATE_FUNCT funct)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    dll_desc->create_funct = funct;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN CREATE_FUNCT AXIS2_CALL
axis2_dll_desc_get_create_funct(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return dll_desc->create_funct;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_dll_desc_set_delete_funct(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env,
        DELETE_FUNCT funct)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    dll_desc->delete_funct = funct;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN DELETE_FUNCT AXIS2_CALL
axis2_dll_desc_get_delete_funct(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return dll_desc->delete_funct;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_dll_desc_set_timestamp(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env,
        AXIS2_TIME_T timestamp)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    dll_desc->timestamp = timestamp;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN AXIS2_TIME_T AXIS2_CALL
axis2_dll_desc_get_timestamp(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return dll_desc->timestamp;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_dll_desc_set_error_code(
    axis2_dll_desc_t *dll_desc,
    const axis2_env_t *env,
    axis2_error_codes_t error_code)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    dll_desc->error_code = error_code;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_error_codes_t AXIS2_CALL
axis2_dll_desc_get_error_code(
    axis2_dll_desc_t *dll_desc,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return dll_desc->error_code;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_dll_desc_create_platform_specific_dll_name(axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env,
        const axis2_char_t *class_name)
{
    axis2_char_t *temp_name = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    temp_name = AXIS2_STRACAT(AXIS2_LIB_PREFIX, class_name, env);
    dll_desc->dll_name = AXIS2_STRACAT(temp_name, AXIS2_LIB_SUFFIX, env);
    AXIS2_FREE(env->allocator, temp_name);
    return dll_desc->dll_name;
}


