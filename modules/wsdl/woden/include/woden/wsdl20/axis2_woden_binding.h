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

#ifndef AXIS2_WODEN_BINDING_H
#define AXIS2_WODEN_BINDING_H

/**
 * @file axis2_woden_binding.h
 * @brief Axis2 Binding Interface
 * Represents the Binding component from the WSDL 2.0 Component model.
 * This component provides a read-only, abstract view of the WSDL 
 * interface, including any interface information defined within
 * imported or included WSDL documents. 
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_binding_element.h>
#include <woden/wsdl20/axis2_woden_configurable.h>

/** @defgroup axis2_woden_binding Binding
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_binding_base axis2_woden_binding_base_t;
typedef struct axis2_woden_binding axis2_woden_binding_t;
typedef struct axis2_woden_binding_ops axis2_woden_binding_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_binding_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *binding,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    to_binding_free) (void *binding,
            axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *binding,
            axis2_env_t **env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *binding,
            axis2_env_t **env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *binding,
            axis2_env_t **env);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *binding,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    get_interface) (
            void *binding,
            axis2_env_t **env);

    axis2_url_t *(AXIS2_CALL *
    get_type) (
            void *binding,
            axis2_env_t **env);

    axis2_array_list_t *(AXIS2_CALL *
    get_binding_faults) (
            void *binding,
            axis2_env_t **env);

    axis2_array_list_t *(AXIS2_CALL *
    get_binding_ops) (
            void *binding,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    to_element)  (
            void *binding,
            axis2_env_t **env);
    /**************************************************************************
     *                      Non-API implementation methods
     **************************************************************************/
    /* 
     * Get the binding fault with the specified 'ref' attribute qname.
     * 
     * TODO decide if this type of qname-based accessor is needed, either internally or on API.
     *  
     * Note that for this type of key-based access, the choice of List rather than Map makes
     * the implementation more complicated. However, the advantage of List is that it avoids the
     * null key problem that arises when the binding fault is missing its 'ref' attribute qname.
     */
    void *(AXIS2_CALL *
    get_binding_fault_element) (
            void *binding,
            axis2_env_t **env,
            axis2_qname_t *qname);

    /*
     * Get the binding operation with the specified 'ref' attribute qname.
     * 
     * TODO decide if this type of qname-based accessor is needed, either internally or on API.
     * 
     * Note that for this type of key-based access, the choice of List rather than Map makes
     * the implementation more complicated. However, the advantage of List is that it avoids the
     * null key problem that arises when the binding operation is missing its 'ref' attribute qname.
     */
    void *(AXIS2_CALL *
    get_binding_op_element) (
            void *binding,
            axis2_env_t **env,
            axis2_qname_t *qname);

    axis2_status_t (AXIS2_CALL *
    set_interface_element) (
            void *binding,
            axis2_env_t **env,
            void *interface);
};

union axis2_woden_binding_base
{
    axis2_woden_configurable_t configurable;
    axis2_woden_binding_element_t binding_element;
};

struct axis2_woden_binding
{
    axis2_woden_binding_base_t base;
    axis2_woden_binding_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_binding_t *)
axis2_woden_binding_create(
        axis2_env_t **env);


/***************************Woden C Internal Methods***************************/
AXIS2_DECLARE(axis2_woden_binding_t *)
axis2_woden_binding_to_binding_element(
        void *binding,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_t *)
axis2_woden_binding_to_configurable_element(
        void *binding,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_t *)
axis2_woden_binding_to_documentable_element(
        void *binding,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_t *)
axis2_woden_binding_to_configurable(
        void *binding,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_binding_resolve_methods(
        axis2_woden_binding_t *binding,
        axis2_env_t **env,
        axis2_woden_binding_t *binding_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_BINDING_FREE(binding, env) \
		(((axis2_woden_binding_t *) binding)->ops->free(binding, env))

#define AXIS2_WODEN_BINDING_TO_BINDING_FREE(binding, env) \
		(((axis2_woden_binding_t *) binding)->ops->to_binding_free(binding, env))

#define AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env) \
		(((axis2_woden_binding_t *) binding)->ops->super_objs(binding, env))

#define AXIS2_WODEN_BINDING_TYPE(binding, env) \
		(((axis2_woden_binding_t *) binding)->ops->type(binding, env))

#define AXIS2_WODEN_BINDING_GET_BASE_IMPL(binding, env) \
		(((axis2_woden_binding_t *) binding)->ops->get_base_impl(binding, env))

#define AXIS2_WODEN_BINDING_GET_QNAME(binding, env) \
		(((axis2_woden_binding_t *) binding)->\
         get_qname(binding, env))

#define AXIS2_WODEN_BINDING_GET_INTERFACE(binding, env) \
		(((axis2_woden_binding_t *) binding)->\
         get_interface(binding, env))

#define AXIS2_WODEN_BINDING_GET_TYPE(binding, env) \
		(((axis2_woden_binding_t *) binding)->ops->\
         get_type(binding, env))

#define AXIS2_WODEN_BINDING_GET_BINDING_FAULTS(binding, env, qname) \
		(((axis2_woden_binding_t *) binding)->ops->\
         get_binding_faults(binding, env, qname))

#define AXIS2_WODEN_BINDING_GET_BINDING_OPS(binding, env) \
		(((axis2_woden_binding_t *) binding)->ops->\
         get_binding_ops(binding, env))

#define AXIS2_WODEN_BINDING_TO_ELEMENT(binding, env) \
		(((axis2_woden_binding_t *) binding)->ops->\
         to_element(binding, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_BINDING_H */
