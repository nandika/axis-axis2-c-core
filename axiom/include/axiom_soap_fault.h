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

#ifndef AXIOM_SOAP_FAULT_H
#define AXIOM_SOAP_FAULT_H


/**
 * @file axiom_soap_fault.h
 * @brief axiom_soap_fault struct
 */
#include <axiom_soap_const.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <axiom_element.h>


#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_fault axiom_soap_fault_t;
    typedef struct axiom_soap_fault_ops axiom_soap_fault_ops_t;

    struct axiom_soap_fault_reason;
    struct axiom_soap_fault_detail;
    struct axiom_soap_fault_sub_code;
    struct axiom_soap_fault_code;
    struct axiom_soap_fault_node;
    struct axiom_soap_fault_role;
    struct axiom_soap_fault_text;
    struct axiom_soap_fault_value;
    struct axiom_soap_body;
    struct axiom_soap_builder;

    /**
     * @defgroup axiom_soap_fault soap fault
     * @ingroup axiom_soap
     * @{
     */

    /**
     *   \brief soap_fault operations struct
     *   ops Encapsulator struct of axiom_soap fault
     */
    struct axiom_soap_fault_ops
    {
        /**
          * Free an axiom_soap_fault
          * @param  fault pointer to soap_fault struct
          * @param  env Environment. MUST NOT be NULL
          * @return status of the op. AXIS2_SUCCESS on success 
          *         else AXIS2_FAILURE
          */

        axis2_status_t(AXIS2_CALL *
                free_fn)(axiom_soap_fault_t *fault,
                        const axis2_env_t *env);
        /**
          * this function returns a axiom_soap_fault_code struct
          * if a fault code is associated with this soap fault
          * only valid when called after building the soap fault
          * @param fault soap fault struct
          * @param env environment must not be NULL
          * @returns pointer to  soap_fault_code struct if one is associated 
          *  with this soap_fault struct , NULL is returned otherwise
          */
        struct axiom_soap_fault_code *(AXIS2_CALL *
                            get_code)(axiom_soap_fault_t *fault,
                                    const axis2_env_t *env);
        /**
         * @param fault soap fault struct
         * @param env environment must not be NULL
         * @returns pointer to soap_fault_reason struct if one is associated 
         * with this soap_fault struct , NULL is returned otherwise
         */

        struct axiom_soap_fault_reason *(AXIS2_CALL *
                            get_reason)(axiom_soap_fault_t *fault,
                                    const axis2_env_t *env);

        /**
         * @param fault soap fault struct
         * @param env environment must not be NULL
         * @returns pointer to soap_fault_node struct if one is associated 
         *  with this soap_fault struct , NULL is returned otherwise
         */
        struct axiom_soap_fault_node*(AXIS2_CALL *
                            get_node)(axiom_soap_fault_t *fault,
                                    const axis2_env_t *env);

        /**
         * @param fault soap fault struct
         * @param env environment must not be NULL
         * @returns pointer to soap_fault_code struct if one is associated 
         * with this soap_fault struct , NULL is returned otherwise
         */
        struct axiom_soap_fault_role*(AXIS2_CALL *
                            get_role)(axiom_soap_fault_t *fault,
                                    const axis2_env_t *env);
        /**
          * @param fault soap fault struct
          * @param env environment must not be NULL
          * @returns a pointer to  soap_fault_code struct if one is 
          * associated with this soap_fault struct , NULL is returned otherwise
          */

        struct axiom_soap_fault_detail*(AXIS2_CALL *
                            get_detail)(axiom_soap_fault_t *fault,
                                    const axis2_env_t *env);
        /**
          * @param fault soap fault struct 
          * @param env enviroment must not be NULL
          * @returns a pointer to  soap_fault_code struct if one is 
          * associated with this soap_fault struct , NULL is returned otherwise
          */
        axis2_char_t *(AXIS2_CALL *
                get_exception)(axiom_soap_fault_t *fault,
                        const axis2_env_t *env);
        /**
          * set an error string 
          * @param fualt soap fault struct
          * @param env enviroment must not be NULL
          * @param exception error message to be stored on soap fault
          */
        axis2_status_t(AXIS2_CALL *
                set_exception)(axiom_soap_fault_t *fault,
                        const axis2_env_t *env,
                        axis2_char_t *exception);
        /**
          * returns the axiom_node_t struct which is wrapped by
          * this soap fault struct
          * @param fault soap fault struct
          * @param env environment must not be NULL
          * @returns a pointer to  axiom_node_t struct if an om node is associated 
          * with this soap fault struct, otherwise return NULL
          */
        axiom_node_t*(AXIS2_CALL *
                get_base_node)(axiom_soap_fault_t *fault,
                        const axis2_env_t *env);


    };

    /**
      * \brief soap_fault struct
      * represent a soap_fault
      */
    struct axiom_soap_fault
    {
        /** operation of axiom_soap_fault struct */
        axiom_soap_fault_ops_t *ops;

    };

    /**
     * creates a soap fault struct
     * @param env environment must not be NULL
     * @param parent soap body struct to which this soap
     * fault is the child
     * @param env Environment. MUST NOT be NULL
     * @returns pointer to axiom_soap_fault_t struct on success
     *  otherwise return NULL with error code set in environments error
     */


    AXIS2_EXTERN axiom_soap_fault_t * AXIS2_CALL
    axiom_soap_fault_create_with_parent(const axis2_env_t *env,
            struct axiom_soap_body *parent);

    /** create an returns a axiom_soap_fault_t struct with a soap fault detail
     * element and have this exceptio string as a text of a child of soap fault
     * detail
     * @param env environment must not be NULL
     * @param parent soap body struct must not be NULL
     * @param exceptio an error string must not be NULL
     * @returns pointer to axiom_soap_fault_t on success ,
     * otherwise return NULL
     */

    AXIS2_EXTERN axiom_soap_fault_t * AXIS2_CALL
    axiom_soap_fault_create_with_exception(const axis2_env_t *env,
            struct axiom_soap_body *parent,
            axis2_char_t* exception);

    AXIS2_EXTERN axiom_soap_fault_t * AXIS2_CALL
    axiom_soap_fault_create_default_fault(const axis2_env_t *env,
            struct axiom_soap_body *parent,
            const axis2_char_t *code_value,
            const axis2_char_t *reason_text,
            const int soap_version);

    /******************** Macros **************************************************/


    /** free soap_fault */
#define AXIOM_SOAP_FAULT_FREE(fault , env) \
        ((fault)->ops->free_fn(fault, env))

#define AXIOM_SOAP_FAULT_GET_CODE(fault , env) \
        ((fault)->ops->get_code(fault, env))

#define AXIOM_SOAP_FAULT_GET_REASON(fault , env) \
        ((fault)->ops->get_reason(fault, env))

#define AXIOM_SOAP_FAULT_GET_DETAIL(fault , env) \
        ((fault)->ops->get_detail(fault, env))

#define AXIOM_SOAP_FAULT_GET_ROLE(fault , env) \
        ((fault)->ops->get_role(fault, env))

#define AXIOM_SOAP_FAULT_GET_NODE(fault , env) \
        ((fault)->ops->get_node(fault, env))

#define AXIOM_SOAP_FAULT_GET_BASE_NODE(fault, env) \
        ((fault)->ops->get_base_node(fault, env))


#define AXIOM_SOAP_FAULT_GET_EXCEPTION(fault, env) \
        ((fault)->ops->get_exception(fault, env))

#define AXIOM_SOAP_FAULT_SET_EXCEPTION(fault, env, exception) \
        ((fault)->ops->set_exception(fault, env, exception))

    /** @} */
#ifdef __cplusplus
}
#endif


#endif /* AXIOM_SOAP_FAULT_H */
