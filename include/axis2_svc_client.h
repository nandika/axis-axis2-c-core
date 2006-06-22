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

#ifndef AXIS2_SVC_CLIENT_H
#define AXIS2_SVC_CLIENT_H


/**
  * @file axis2_svc_client.h
  * The service client interface serves as the primary client
  * interface for consuming services. You can set the options to be
  * used by the service client and then invoke an operation on a given
  * service. There are several ways of invoking a service operation,
  * which are based on the concept of a message exchange pattern
  * (MEP). The two basic MEPs supported by service client are out-only
  * and out-in. Each MEP can be used in either blocking or
  * non-blocking mode. The operation invocations using the service
  * client API are based on the XML-in/XML-out principle: both the
  * payload to be sent to the service and the result from the service
  * are in XML, represented in AXIOM.
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_svc.h>
#include <axis2_options.h>
#include <axis2_qname.h>
#include <axiom_element.h>
#include <axis2_callback.h>
#include <axis2_endpoint_ref.h>
#include <axis2_svc_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_op_client.h>

/** Name of anonymous service */
#define AXIS2_ANON_SERVICE  "__ANONYMOUS_SERVICE__"

/** out-only MEP operation name */
#define AXIS2_ANON_OUT_ONLY_OP "__OPERATION_OUT_ONLY__"

/** out-only robust MEP operation name */
#define AXIS2_ANON_ROBUST_OUT_ONLY_OP "__OPERATION_ROBUST_OUT_ONLY__"

/** out-in MEP operation name */
#define AXIS2_ANON_OUT_IN_OP "__OPERATION_OUT_IN__"


#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axis2_svc_client service client
     * @ingroup axis2_core_clientapi
     * @{
     */

    /** Type name for struct axis2_svc_client_ops */
    typedef struct axis2_svc_client_ops axis2_svc_client_ops_t;
    /** Type name for struct axis2_svc_client */
    typedef struct axis2_svc_client axis2_svc_client_t;


    /**
     * service client ops struct
     * Encapsulator struct for operations of axis2_svc_client
     */
    struct axis2_svc_client_ops
    {
        /**
          * Returns the axis2_svc_t this is a client for. This is primarily 
          * useful when the service is created anonymously or from WSDL.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @return a pointer to axis service struct, or NULL if no service 
          * is associated. Returns a reference, not a cloned copy. 
          */
        const axis2_svc_t *(AXIS2_CALL *
                get_svc)(
                    const axis2_svc_client_t *svc_client,
                    const axis2_env_t *env);

        /**
          * Sets the options to be used by service client.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param options pointer to options struct to be set
          * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
          */
        axis2_status_t (AXIS2_CALL *
                set_options)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    const axis2_options_t *options);

        /**
          * Gets options used by service client. 
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @return a pointer to the options struct if options set, else NULL.
          * Returns a reference, not a cloned copy.
          */
        const axis2_options_t *(AXIS2_CALL *
                get_options)(
                    const axis2_svc_client_t *svc_client,
                    const axis2_env_t *env);

        /**
          * Sets the overriding options. The overriding client options related 
          * to this service interaction override any options that the 
          * underlying operation client may have.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param options pointer to options struct to be set
          * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
          */
        axis2_status_t (AXIS2_CALL *
                set_override_options)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    const axis2_options_t *override_options);

        /**
          * Gets the overriding options.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @return pointer to overriding options struct if options set, else NULL.
          * Returns a reference, not a cloned copy.
          */
        const axis2_options_t *(AXIS2_CALL *
                get_override_options)(
                    const axis2_svc_client_t *svc_client,
                    const axis2_env_t *env);

        /**
          * Engages the named module. The engaged modules extend the message
          * processing when consuming services. Modules help to apply QoS
          * norms in messaging. Once a module is engaged to a service client,
          * the axis2_engine makes sure to invoke the module for all the 
          * interactions between the client and the service.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param module_name name of the module to be engaged
          * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
          */
        axis2_status_t (AXIS2_CALL *
                engage_module)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    const axis2_char_t *module_name);

        /**
          * Dis-engages the named module. Dis-engaging a module on a service 
          * client ensures that the axis2_engine would not invoke the named 
          * module when sending and receiving messages.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param module_name name of the module to be dis-engaged
          * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
          */
        axis2_status_t (AXIS2_CALL *
                disengage_module)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    const axis2_char_t *module_name);

        /**
          * Adds an XML element as a header to be sent to the server side.
          * This allows users to go beyond the usual XML-in/XML-out pattern,
          * and send custom SOAP headers. Once added, service client owns
          * the header and will clean up when the service client is freed.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param header om node representing the SOAP header in XML
          * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
          */
        axis2_status_t (AXIS2_CALL *
                add_header)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    axiom_node_t *header);

        /**
          * Removes all the headers added to service client.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
          */
        axis2_status_t (AXIS2_CALL *
                remove_all_headers)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env);

        /**
          * This method can be used to send an XML message.
          * This is a simple method to invoke a service operation whose MEP is
          * Robust Out-Only. If a fault triggers on server side, this method 
          * would report an error back to the caller.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param op_qname operation qname. NULL is equivalent to an
          * operation name of "__OPERATION_ROBUST_OUT_ONLY__"
          * @param payload pointer to OM node representing the XML payload to be sent
          * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
          */
        axis2_status_t (AXIS2_CALL *
                send_robust)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    const axis2_qname_t *op_qname,
                    const axiom_node_t *payload);

        /**
          * Sends a message and forget about it. This method is used to interact with
          * a service operation whose MEP is In-Only. That is, there is no
          * opportunity to get an error from the service via this method; one may still
          * get client-side errors, such as host unknown etc.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param op_qname operation qname. NULL is equivalent to an
          * operation name of "__OPERATION_OUT_ONLY__"
          * @param payload pointer to OM node representing the XML payload to be sent
          */
        void (AXIS2_CALL *
                fire_and_forget)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    const axis2_qname_t *op_qname,
                    const axiom_node_t *payload);

        /**
          * Sends XML request and receives XML response.
          * This method is used to interact with a service operation whose MEP is In-Out.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param op_qname operation qname. NULL is equivalent to an
          * operation name of "__OPERATION_OUT_IN__"
          * @param payload pointer to OM node representing the XML payload to be sent
          * @return pointer to OM node representing the XML response. The 
          * caller owns the returned node.
          */
        axiom_node_t *(AXIS2_CALL *
                send_receive)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    const axis2_qname_t *op_qname,
                    const axiom_node_t *payload);

        /**
          * Sends XML request and receives XML response, but does not block for response.
          * This method is used to interact in non-blocking mode with a service
          * operation whose MEP is In-Out.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param op_qname operation qname. NULL is equivalent to an
          * operation name of "__OPERATION_OUT_IN__"
          * @param payload pointer to OM node representing the XML payload to be sent
          * @callback pointer to callback struct used to capture response
          */
        void (AXIS2_CALL *
                send_receive_non_blocking)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    const axis2_qname_t *op_qname,
                    const axiom_node_t *payload,
                    axis2_callback_t *callback);

        /**
          * Creates an op_client for a specific operation. This is the way to  
          * create a full functional MEP client which can be used to exchange 
          * messages for this specific operation.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param op_qname axis2_qname_t of the operation
          * @return pointer to newly created op_client configured for the given operation
          */
        axis2_op_client_t *(AXIS2_CALL *
                create_op_client)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    const axis2_qname_t *op_qname);

        /**
          * Cleans up service client invocation. This will close the output 
          * stream and/or remove entry from waiting queue of the transport 
          * listener queue.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
          */
        axis2_status_t (AXIS2_CALL *
                finalize_invoke)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env);

        /**
          * Gets the service client's own endpoint_ref, that is the
          * endpoint the client will be sending from.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param transport name of the transport, e.g "http"
          * @return pointer to the endpoint_ref struct. Returns a reference, 
          * not a cloned copy.
          */
        const axis2_endpoint_ref_t *(AXIS2_CALL *
                get_own_endpoint_ref)(
                    const axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    const axis2_char_t *transport);

        /**
          * Gets the target endpoint ref.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @return pointer to the endpoint_ref struct. Returns a reference, 
          * not a cloned copy.
          */
        const axis2_endpoint_ref_t *(AXIS2_CALL *
                get_target_endpoint_ref)(
                    const axis2_svc_client_t *svc_client,
                    const axis2_env_t *env);

        /**
          * Sets the target endpoint ref.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @param target_epr pointer to the endpoint_ref struct to be set as 
          * target. service client takes over the ownership of the struct.
          * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
          */
        axis2_status_t (AXIS2_CALL *
                set_target_endpoint_ref)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *target_epr);

        /**
          * Gets the service context.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @return pointer to service context struct. service client owns
          * the returned pointer.
          */
        const axis2_svc_ctx_t *(AXIS2_CALL *
                get_svc_ctx)(
                    const axis2_svc_client_t *svc_client,
                    const axis2_env_t *env);

        /**
          * Frees the service client.
          * @param svc_client pointer to service client struct
          * @param env pointer to environment struct
          * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
          */

        axis2_status_t (AXIS2_CALL *
                free_fn)(
                    axis2_svc_client_t *svc_client,
                    const axis2_env_t *env);
    };

    /**
     * Service client struct
     */
    struct axis2_svc_client
    {
        axis2_svc_client_ops_t *ops;
    };

    /**
     * Creates a service client struct.
     * @param env pointer to environment struct
     * @param client_home name of the directory that contains the Axis2/C repository
     * @return a pointer to newly created service client struct,
     *         or NULL on error with error code set in environment's error
     */
    AXIS2_EXTERN axis2_svc_client_t * AXIS2_CALL
    axis2_svc_client_create(
        const axis2_env_t *env,
        const axis2_char_t *client_home);

    /**
     * Creates a service client struct for a specified service and configuration
     * context.
     * @param env pointer to environment struct
     * @param conf_ctx pointer to configuration context. Newly created client 
     * assumes ownership of the conf_ctx
     * @param svc pointer to service struct representing the service to be consumed.
     * Newly created client assumes ownership of the service 
     * @param client_home name of the directory that contains the Axis2/C repository
     * @return a pointer to newly created service client struct,
     *         or NULL on error with error code set in environment's error
     */
    AXIS2_EXTERN axis2_svc_client_t * AXIS2_CALL
    axis2_svc_client_create_with_conf_ctx_and_svc(
        const axis2_env_t *env,
        const axis2_char_t *client_home,
        axis2_conf_ctx_t *conf_ctx,
        axis2_svc_t *svc);

/** Gets the service consumed by service client. 
    @sa axis2_svc_client_ops#get_svc */
#define AXIS2_SVC_CLIENT_GET_AXIS_SERVICE(svc_client, env) \
        ((svc_client)->ops->get_svc(svc_client, env))

/** Sets the options to be used by service client. 
    @sa axis2_svc_client_ops#set_options */
#define AXIS2_SVC_CLIENT_SET_OPTIONS(svc_client, env, options) \
        ((svc_client)->ops->set_options(svc_client, env, options))

/** Gets the options being used by service client. 
    @sa axis2_svc_client_ops#get_options */
#define AXIS2_SVC_CLIENT_GET_OPTIONS(svc_client, env) \
        ((svc_client)->ops->get_options(svc_client, env))

/** Sets the overriding options to be used by service client. 
    @sa axis2_svc_client_ops#set_override_options */
#define AXIS2_SVC_CLIENT_SET_OVERRIDE_OPTIONS(svc_client, env, override_options) \
        ((svc_client)->ops->set_override_options(svc_client, env, override_options))

/** Gets the overriding options being used by service client 
    @sa axis2_svc_client_ops#get_override_options */
#define AXIS2_SVC_CLIENT_GET_OVERRIDE_OPTIONS(svc_client, env) \
        ((svc_client)->ops->get_override_options(svc_client, env))

/** Engages the given module. 
    @sa axis2_svc_client_ops#engage_module */
#define AXIS2_SVC_CLIENT_ENGAGE_MODULE(svc_client, env, module_name) \
        ((svc_client)->ops->engage_module(svc_client, env, module_name))

/** Dis-engages the given module. 
    @sa axis2_svc_client_ops#disengage_module */
#define AXIS2_SVC_CLIENT_DISENGAGE_MODULE(svc_client, env, module_name) \
        ((svc_client)->ops->disengage_module(svc_client, env, module_name))

/** Adds a SOAP header to be sent by the service client. 
    @sa axis2_svc_client_ops#add_header */
#define AXIS2_SVC_CLIENT_ADD_HEADER(svc_client, env, header) \
        ((svc_client)->ops->add_header(svc_client, env, header))

/** Removes all the SOAP headers currently set on service client. 
    @sa axis2_svc_client_ops#remove_all_headers */
#define AXIS2_SVC_CLIENT_REMOVE_ALL_HEADERS(svc_client, env) \
        ((svc_client)->ops->remove_all_headers(svc_client, env))

/** Sends the given payload in a robust manner. 
    Robust means that SOAP faults are captured and reported. 
    @sa axis2_svc_client_ops#send_robust */
#define AXIS2_SVC_CLIENT_SEND_ROBUST(svc_client, env, payload) \
        ((svc_client)->ops->send_robust(svc_client, env, NULL, payload))

/** Sends the given payload in a robust manner targeted to named operation. 
    @sa axis2_svc_client_ops#send_robust */
#define AXIS2_SVC_CLIENT_SEND_ROBUST_WITH_OP_QNAME(svc_client, env, op_qname, payload) \
        ((svc_client)->ops->send_robust(svc_client, env, op_qname, payload))

/** Sends the given payload and forgets about it. 
    No SOAP faults are reported.
    @sa axis2_svc_client_ops#fire_and_forget */
#define AXIS2_SVC_CLIENT_FIRE_AND_FORGET(svc_client, env, payload) \
        ((svc_client)->ops->fire_and_forget(svc_client, env, NULL, payload))

/** Sends the given payload targeted to named operation and forgets about it
    @sa axis2_svc_client_ops#fire_and_forget */
#define AXIS2_SVC_CLIENT_FIRE_AND_FORGET_WITH_OP_QNAME(svc_client, env, op_qname, payload) \
        ((svc_client)->ops->fire_and_forget(svc_client, env, op_qname, payload))

/** Sends the given payload and receives the response. 
    @sa axis2_svc_client_ops#send_receive */
#define AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, env, payload) \
        ((svc_client)->ops->send_receive(svc_client, env, NULL, payload))

/** Sends the given payload targeted to named operation and receives the response. 
    @sa axis2_svc_client_ops#send_receive */
#define AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME(svc_client, env, op_qname, payload) \
        ((svc_client)->ops->send_receive(svc_client, env, op_qname, payload))

/** Sends the given payload and receives the response.
    Does not block till response arrives. Uses the callback to capture response. 
    @sa axis2_svc_client_ops#send_receive_non_blocking */
#define AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback) \
        ((svc_client)->ops->send_receive_non_blocking(svc_client, env, NULL, payload, callback))

/** Sends the given payload targeted to named operation and receives the response.
    Does not block till response arrives. Uses the callback to capture response 
    @sa axis2_svc_client_ops#send_receive_non_blocking */
#define AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING_WITH_OP_QNAME(svc_client, env, op_qname, payload, callback) \
        ((svc_client)->ops->send_receive_non_blocking(svc_client, env, op_qname, payload, callback))

/** Creates an operation client for a specific operation. 
    @sa axis2_svc_client_ops#create_op_client */
#define AXIS2_SVC_CLIENT_CREATE_OP_CLIENT(svc_client, env, op_qname) \
        ((svc_client)->ops->create_op_client(svc_client, env, op_qname))

/** Cleans up after the invocations. 
    @sa axis2_svc_client_ops#finalize_invoke */
#define AXIS2_SVC_CLIENT_FINALIZE_INVOKE(svc_client, env) \
        ((svc_client)->ops->finalize_invoke(svc_client, env))

/** Gets the service client's own endpoint ref. 
    @sa axis2_svc_client_ops#get_own_endpoint_ref */
#define AXIS2_SVC_CLIENT_GET_OWN_ENDPOINT_REF(svc_client, env, transport) \
        ((svc_client)->ops->get_own_endpoint_ref(svc_client, env, transport))

/** Gets the target endpoint ref. 
    @sa axis2_svc_client_ops#get_target_endpoint_ref */
#define AXIS2_SVC_CLIENT_GET_TARGET_ENDPOINT_REF(svc_client, env) \
        ((svc_client)->ops->get_target_endpoint_ref(svc_client, env))

/** Sets the target endpoint ref. 
    @sa axis2_svc_client_ops#set_target_endpoint_ref */
#define AXIS2_SVC_CLIENT_SET_TARGET_ENDPOINT_REF(svc_client, env, target_epr) \
        ((svc_client)->ops->set_target_endpoint_ref(svc_client, env, target_epr))

/** Gets the service context being used by the service client. 
    @sa axis2_svc_client_ops#get_svc_ctx */
#define AXIS2_SVC_CLIENT_GET_SVC_CTX(svc_client, env) \
        ((svc_client)->ops->get_svc_ctx(svc_client, env))

/** Frees given service client. 
    @sa axis2_svc_client_ops#free */
#define AXIS2_SVC_CLIENT_FREE(svc_client, env) \
        ((svc_client)->ops->free_fn(svc_client, env))

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_SVC_CLIENT_H */
