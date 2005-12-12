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

#ifndef AXIS2_WSDL_INTERFACE_H
#define AXIS2_WSDL_INTERFACE_H

/**
 * @file axis2_wsdl_interface.h
 * @brief Axis2 Wsdl interface interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_hash.h>
#include <axis2_wsdl_operation.h>
#include <axis2_operation.h>
#include <axis2_linked_list.h>
#include <axis2_wsdl_extensible_component.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
struct axis2_wsdl_operation;
struct axis2_operation;    
struct axis2_wsdl_extensible_component;    
typedef struct axis2_wsdl_interface axis2_wsdl_interface_t;
typedef struct axis2_wsdl_interface_ops axis2_wsdl_interface_ops_t;

/** @defgroup axis2_wsdl_interface Wsdl Interface
 * @ingroup axis2_wsdl
 * @{
 */


/** 
 * @brief Wsdl Interface operations struct
 * Encapsulator struct for operations of axis2_wsdl_interface
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_interface_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_interface_t *wsdl_interface,
		    axis2_env_t **env);

    /**
     * Method getDefinedOperations
     *
     * @return
     */
    axis2_hash_t *(AXIS2_CALL *
    get_defined_operations) (axis2_wsdl_interface_t *wsdl_interface,
                                                axis2_env_t **env);
    
    /**
     * @return
     */
    axis2_linked_list_t *(AXIS2_CALL *
    get_faults) (axis2_wsdl_interface_t *wsdl_interface,
                                        axis2_env_t **env);
    
    /**
     * @return
     */
    axis2_qname_t *(AXIS2_CALL *
    get_name) (axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env);
                                    
    /**
     * @return
     */
    axis2_hash_t *(AXIS2_CALL *
    get_operations) (axis2_wsdl_interface_t *wsdl_interface,
                                        axis2_env_t **env);
    
    /**
     * Retruns the <code>WSDLOperation</code>
     *
     * @param nCName
     * @return
     */
    void *(AXIS2_CALL *
    get_operation) (axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    axis2_char_t *nc_name);
                                    
    /**
     * @return
     */
    axis2_hash_t *(AXIS2_CALL *
    get_super_interfaces) (axis2_wsdl_interface_t *wsdl_interface,
                                                axis2_env_t **env);
    
    /**
     * Method getSuperInterface
     *
     * @param qName
     * @return
     */
    axis2_wsdl_interface_t *(AXIS2_CALL *
    get_super_interface) (axis2_wsdl_interface_t *wsdl_interface,
                                                axis2_env_t **env,
                                                axis2_qname_t *qname);
                                                
    /**
     * The Targetnamespace is that of the namespace URI of the QName of
     * this component.
     *
     * @return URI as a String if the name is set otherwise will return null.
     */
    axis2_char_t *(AXIS2_CALL *
    get_target_namespace) (axis2_wsdl_interface_t *wsdl_interface,
                                                axis2_env_t **env);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_faults) (axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    axis2_linked_list_t *list);
                                    
    /**
     * @param qName
     */
    axis2_status_t (AXIS2_CALL *
    set_name) (axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    axis2_qname_t *qName);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_operations) (axis2_wsdl_interface_t *wsdl_interface,
                                        axis2_env_t **env,
                                        axis2_hash_t *list);
                                        
    /**
     * The operation is added by its ncname. If operation is null
     * it will not be added. If the Operation name is null a
     * <code>WSDLProcessingException</code> will be thrown.
     *
     * @param operation
     */
    axis2_status_t (AXIS2_CALL *
    set_operation) (axis2_wsdl_interface_t *wsdl_interface,
                                        axis2_env_t **env,
                                        void *operation,
                                        axis2_operation_type_t opt_type);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_super_interfaces) (axis2_wsdl_interface_t *wsdl_interface,
                                                axis2_env_t **env,
                                                axis2_hash_t *list);
                                                
    /**
     * The Inteface will be added to the list of super interfaces keyed with
     * the QName.
     *
     * @param interfaceComponent WSDLInterface Object
     */
    axis2_status_t (AXIS2_CALL *
    add_super_interface)(axis2_wsdl_interface_t *wsdl_interface,
                                                axis2_env_t **env,
                                                axis2_wsdl_interface_t *interface_component);
    
    /**
     * Will return the StyleDefault if exist , otherwise will return null
     *
     * @return
     */
    axis2_char_t *(AXIS2_CALL *
    get_style_default)(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env);
                                            
    
    /**
     * Method setStyleDefault
     *
     * @param styleDefault
     */
    axis2_status_t (AXIS2_CALL *
    set_style_default) (axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env,
                                            axis2_char_t *style_default);

};

/** 
 * @brief Wsdl container struct
 *	Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_interface
{
	axis2_wsdl_interface_ops_t *ops;
    struct axis2_wsdl_extensible_component *extensible_component;
    axis2_operation_type_t optr_type; /*0-wsdl_operation, 1-axis2_operation */
};

/**
 * Creates wsdl property struct
 * @return pointer to newly created wsdl property
 */
AXIS2_DECLARE(axis2_wsdl_interface_t *) axis2_wsdl_interface_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_INTERFACE_FREE(wsdl_interface, env) \
		((wsdl_interface->ops)->free (wsdl_interface, env))

#define AXIS2_WSDL_INTERFACE_GET_DEFINED_OPERATIONS(wsdl_interface, env) \
		((wsdl_interface->ops)->get_defined_operations (wsdl_interface, env))

#define AXIS2_WSDL_INTERFACE_GET_FAULTS(wsdl_interface, env) \
		((wsdl_interface->ops)->get_faults (wsdl_interface, env))

#define AXIS2_WSDL_INTERFACE_GET_NAME(wsdl_interface, env) \
		((wsdl_interface->ops)->get_name (wsdl_interface, env))

#define AXIS2_WSDL_INTERFACE_GET_OPERATIONS(wsdl_interface, env) \
		((wsdl_interface->ops)->get_operations (wsdl_interface, env))

#define AXIS2_WSDL_INTERFACE_GET_OPERATION(wsdl_interface, env, nc_name) \
		((wsdl_interface->ops)->get_operation (wsdl_interface, env, nc_name))
        
#define AXIS2_WSDL_INTERFACE_GET_SUPER_INTERFACES(wsdl_interface, env) \
		((wsdl_interface->ops)->get_super_interfaces (wsdl_interface, env))

#define AXIS2_WSDL_INTERFACE_GET_SUPER_INTERFACE(wsdl_interface, env) \
		((wsdl_interface->ops)->get_super_interface (wsdl_interface, env))        

#define AXIS2_WSDL_INTERFACE_GET_TARGET_NAMESPACE(wsdl_interface, env) \
		((wsdl_interface->ops)->get_target_namespace (wsdl_interface, env))
        
#define AXIS2_WSDL_INTERFACE_SET_FAULTS(wsdl_interface, env, list) \
		((wsdl_interface->ops)->set_faults (wsdl_interface, env, list))
        
#define AXIS2_WSDL_INTERFACE_SET_NAME(wsdl_interface, env, name) \
		((wsdl_interface->ops)->set_name (wsdl_interface, env, name))
        
#define AXIS2_WSDL_INTERFACE_SET_OPERATIONS(wsdl_interface, env, list) \
		((wsdl_interface->ops)->set_operations (wsdl_interface, env, list))

#define AXIS2_WSDL_INTERFACE_SET_OPERATION(wsdl_interface, env, operation, optr_type) \
		((wsdl_interface->ops)->set_operation (wsdl_interface, env, operation, optr_type))

#define AXIS2_WSDL_INTERFACE_SET_SUPER_INTERFACES(wsdl_interface, env, list) \
		((wsdl_interface->ops)->set_super_interfaces (wsdl_interface, env, list))
        
#define AXIS2_WSDL_INTERFACE_SET_SUPER_INTERFACE(wsdl_interface, env, interface) \
		((wsdl_interface->ops)->set_super_interface (wsdl_interface, env, interface))
        
#define AXIS2_WSDL_INTERFACE_GET_STYLE_DEFAULT(wsdl_interface, env) \
		((wsdl_interface->ops)->get_style_default (wsdl_interface, env))
        
#define AXIS2_WSDL_INTERFACE_SET_STYLE_DEFAULT(wsdl_interface, env, style_default) \
		((wsdl_interface->ops)->set_style_default (wsdl_interface, env, style_default))
	
/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_INTERFACE_H */
