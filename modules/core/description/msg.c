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

#include <axis2_msg.h>
#include <axis2_property.h>

struct axis2_msg
{
    /** parent operation */
    axis2_op_t *parent;
    /** list of phases that represent the flow  */
    axis2_array_list_t *flow;
    /** name of the message */
    axis2_char_t *name;
    /** XML schema element qname */
    axis2_qname_t *element_qname;
    /** direction of message */
    axis2_char_t *direction;
    /** parameter container to hold message parameters */
    struct axis2_param_container *param_container;
};

AXIS2_EXTERN axis2_msg_t *AXIS2_CALL
axis2_msg_create(const axis2_env_t *env)
{
    axis2_msg_t *msg = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    msg = (axis2_msg_t *) AXIS2_MALLOC(env->allocator, sizeof(axis2_msg_t));

    if (!msg)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    msg->param_container = NULL;
    msg->parent = NULL;
    msg->flow = NULL;
    msg->name = NULL;
    msg->element_qname = NULL;
    msg->direction = NULL;

    msg->param_container = 
        (axis2_param_container_t *)axis2_param_container_create(env);
    if (!msg->param_container)
    {
        axis2_msg_free(msg, env);
        return NULL;
    }

    msg->flow = axis2_array_list_create(env, 0);
    if (!msg->flow)
    {
        axis2_msg_free(msg, env);
        return NULL;
    }

    return msg;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_msg_free(axis2_msg_t *msg,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (msg->flow)
    {
        axis2_array_list_free(msg->flow, env);
    }

    if (msg->name)
    {
        AXIS2_FREE(env->allocator, msg->name);
    }

    if (msg->element_qname)
    {
        axis2_qname_free(msg->element_qname, env);
    }

    if (msg->direction)
    {
        AXIS2_FREE(env->allocator, msg->direction);
    }

    if (msg->param_container)
    {
        axis2_param_container_free(msg->param_container, env);
    }

    msg->parent = NULL;

    if (msg)
    {
        AXIS2_FREE(env->allocator, msg);
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_msg_add_param(axis2_msg_t *msg,
    const axis2_env_t *env,
    axis2_param_t *param)
{
    axis2_char_t *param_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param, AXIS2_FALSE);

    param_name = axis2_param_get_name(param, env);
    if (AXIS2_TRUE == axis2_msg_is_param_locked(msg, env, param_name))
    {
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        return axis2_param_container_add_param(msg->param_container, env,
            param);
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_param_t *AXIS2_CALL
axis2_msg_get_param(const axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *param_name)
{
    AXIS2_PARAM_CHECK(env->error, param_name, NULL);

    return axis2_param_container_get_param(msg->param_container, env, param_name);
}

AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
axis2_msg_get_all_params(const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    AXIS2_PARAM_CHECK(env->error, msg->param_container, AXIS2_FALSE);

    return axis2_param_container_get_params(msg->param_container, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_msg_set_parent(axis2_msg_t *msg,
    const axis2_env_t *env,
    axis2_op_t *op)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg->parent = op;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_op_t *AXIS2_CALL
axis2_msg_get_parent(const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    return msg->parent;
}

AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
axis2_msg_get_flow(const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    return msg->flow;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_msg_is_param_locked(axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *param_name)
{
    axis2_op_t *parent_l = NULL;
    axis2_param_t *param_l = NULL;
    axis2_bool_t locked = AXIS2_FALSE;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, param_name, AXIS2_FALSE);

    /* checking the locked status in parent */
    parent_l = axis2_msg_get_parent(msg, env);
    if (parent_l)
    {
        locked = axis2_op_is_param_locked(parent_l, env, param_name);
    }
    if (AXIS2_TRUE == locked)
    {
        return AXIS2_TRUE;
    }
    else
    {
        param_l = axis2_msg_get_param(msg, env, param_name);
    }
    return (param_l  && axis2_param_is_locked(param_l, env));
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_msg_set_flow(axis2_msg_t *msg,
    const axis2_env_t *env,
    axis2_array_list_t *flow)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (msg->flow)
    {
        axis2_array_list_free(msg->flow, env);
        msg->flow = NULL;
    }
    if (flow)
    {
        msg->flow = flow;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_msg_get_direction(
    const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    return msg->direction;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_msg_set_direction(axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *direction)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (msg->direction)
    {
        AXIS2_FREE(env->allocator, msg->direction);
        msg->direction = NULL;
    }

    if (direction)
    {
        msg->direction = axis2_strdup(direction, env);
        if (!(msg->direction))
        {
            return AXIS2_FAILURE;
        }
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_qname_t *AXIS2_CALL
axis2_msg_get_element_qname(const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    return msg->element_qname;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_msg_set_element_qname(axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_qname_t *element_qname)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (msg->element_qname)
    {
        axis2_qname_free(msg->element_qname, env);
        msg->element_qname = NULL;
    }

    if (element_qname)
    {
        msg->element_qname = axis2_qname_clone((axis2_qname_t *)element_qname, env);
        if (!(msg->element_qname))
        {
            return AXIS2_FAILURE;
        }
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_msg_get_name(const axis2_msg_t *msg,
    const axis2_env_t *env)
{
    return msg->name;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_msg_set_name(axis2_msg_t *msg,
    const axis2_env_t *env,
    const axis2_char_t *name)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (msg->name)
    {
        AXIS2_FREE(env->allocator, msg->name);
        msg->name = NULL;
    }

    if (name)
    {
        msg->name = axis2_strdup(name, env);
        if (!(msg->name))
        {
            return AXIS2_FAILURE;
        }
    }

    return AXIS2_SUCCESS;
}


