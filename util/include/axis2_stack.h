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

#ifndef AXIS2_STACK_H
#define AXIS2_STACK_H


/**
* @file axis2_stack.h
* @brief represents a stack
*/

#include <axis2_utils_defines.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axis2_util_stack stack
     * @ingroup axis2_util
     * @{
     */

    typedef struct axis2_stack axis2_stack_t;

    AXIS2_EXTERN axis2_stack_t * AXIS2_CALL
    axis2_stack_create(const axis2_env_t *env);

    /**
     * Free function of the stack
     * @param stack pointer to stack
     * @param env environemnt
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_stack_free(axis2_stack_t *stack,
        const axis2_env_t *env);

    AXIS2_EXTERN void* AXIS2_CALL
    axis2_stack_pop(axis2_stack_t *stack,
        const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_stack_push(axis2_stack_t *stack,
        const axis2_env_t *env,
        void* value);

    AXIS2_EXTERN int AXIS2_CALL
    axis2_stack_size(axis2_stack_t *stack,
        const axis2_env_t *env);
    /**
     * returns the last put value from the stack
     * without removing it from stack
     */
    AXIS2_EXTERN void* AXIS2_CALL
    axis2_stack_get(axis2_stack_t *stack,
        const axis2_env_t *env);

    AXIS2_EXTERN void* AXIS2_CALL
    axis2_stack_get_at(axis2_stack_t *stack,
        const axis2_env_t *env,
        int i);

    /** @} */

#ifdef __cplusplus
}
#endif
#endif /* AXIS2_STACK_H */

