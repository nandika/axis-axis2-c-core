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

#ifndef AXIS2_GENERIC_OBJ_H
#define AXIS2_GENERIC_OBJ_H

#include <axis2_defines.h>
#include <axutil_error.h>
#include <axutil_env.h>
#include <axutil_utils.h>


#ifdef __cplusplus
extern "C"
{
#endif
    
typedef struct axutil_generic_obj axutil_generic_obj_t;
typedef struct axutil_generic_obj_ops axutil_generic_obj_ops_t;
    
/**
 * @defgroup axutil_generic_obj Generic Obj
 * @ingroup woden_util 
 * @{
 */

/** 
 * @brief Generic Obj ops struct
 * Encapsulator struct for ops of woden_generic_obj
 */
 struct axutil_generic_obj_ops
{

    axis2_status_t (AXIS2_CALL *
    free) (
            axutil_generic_obj_t *generic_obj, 
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_scope) (
            axutil_generic_obj_t *generic_obj,
            const axutil_env_t *env,
            axis2_scope_t scope);

    axis2_status_t (AXIS2_CALL *
    set_free_func) (
            axutil_generic_obj_t *generic_obj,
            const axutil_env_t *env,
            AXIS2_FREE_VOID_ARG free_func);

    axis2_status_t (AXIS2_CALL *
    set_value) (
            axutil_generic_obj_t *generic_obj,
            const axutil_env_t *env,
            void *value);
    
    void *(AXIS2_CALL *
    get_value) (
            axutil_generic_obj_t *generic_obj,
            const axutil_env_t *env);

};
    
     
/** 
     * @brief
     */ 
 struct axutil_generic_obj
{
   axutil_generic_obj_ops_t *ops;
};

/**
 * create new generic_obj
 * @return generic_obj newly created generic_obj
 */
AXIS2_EXTERN axutil_generic_obj_t * AXIS2_CALL
axutil_generic_obj_create(const axutil_env_t *env);

/*************************** Function macros **********************************/

#define  axutil_generic_obj_free(generic_obj, env) \
      ((generic_obj)->ops->free (generic_obj, env))

#define axutil_generic_obj_set_free_func(generic_obj, env, free_func) \
      ((generic_obj)->ops->set_free_func (generic_obj, env, free_func))  

#define AXIS2_GENERIC_OBJ_SET_SCOPE(generic_obj, env, scope) \
      ((generic_obj)->ops->set_scope (generic_obj, env, scope)) 

#define  axutil_generic_obj_set_value(generic_obj, env, value) \
      ((generic_obj)->ops->set_value (generic_obj, env, value))

#define  axutil_generic_obj_get_value(generic_obj, env) \
        ((generic_obj)->ops->get_value(generic_obj, env))
                                        
/*************************** End of function macros ***************************/



/** @} */
    
#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_GENERIC_OBJ_H */
