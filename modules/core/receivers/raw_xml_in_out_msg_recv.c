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
 
#include <axis2_raw_xml_in_out_msg_recv.h>
#include <string.h>
#include <axis2_om_element.h>
#include <axis2_soap_envelope.h>
#include <axis2_soap_body.h>
#include <axis2_core_utils.h>
#include <axis2_engine.h>

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_raw_xml_in_out_msg_recv_invoke_business_logic(axis2_msg_recv_t *msg_recv,
                                                    axis2_env_t **env,
                                                    axis2_msg_ctx_t *msg_ctx,
                                                    axis2_msg_ctx_t *new_msg_ctx);

axis2_status_t AXIS2_CALL
axis2_raw_xml_in_out_msg_recv_receive(axis2_msg_recv_t *msg_recv,
                                    axis2_env_t **env,
                                    axis2_msg_ctx_t *msg_ctx);
                                
/************************** End of function prototypes ************************/

axis2_msg_recv_t *AXIS2_CALL 
axis2_raw_xml_in_out_msg_recv_create(axis2_env_t **env)
{
    axis2_msg_recv_t *msg_recv = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
	AXIS2_ENV_CHECK(env, NULL);
	msg_recv = axis2_msg_recv_create(env);
	if(NULL == msg_recv)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    status = AXIS2_MSG_RECV_SET_SCOPE(msg_recv, env, AXIS2_APPLICATION_SCOPE);
    if(AXIS2_TRUE != status)
    {
        AXIS2_MSG_RECV_FREE(msg_recv, env);
        return NULL;
    }
    msg_recv->ops->invoke_in_out_business_logic = 
        axis2_raw_xml_in_out_msg_recv_invoke_business_logic;
    msg_recv->ops->receive = axis2_raw_xml_in_out_msg_recv_receive;
	return msg_recv;
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_raw_xml_in_out_msg_recv_invoke_business_logic(axis2_msg_recv_t *msg_recv,
                                                    axis2_env_t **env,
                                                    axis2_msg_ctx_t *msg_ctx,
                                                    axis2_msg_ctx_t *new_msg_ctx)
{
    axis2_svc_skeleton_t *svc_obj = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_op_t *op_desc = NULL;
    axis2_char_t *style = NULL;
    axis2_om_node_t *om_node = NULL;
    axis2_om_element_t *om_element = NULL;
    axis2_char_t *local_name = NULL;
    axis2_om_node_t *result_node = NULL;
    axis2_om_node_t *body_content_node = NULL;
    axis2_om_element_t *body_content_element = NULL;
    axis2_soap_envelope_t *default_envelope = NULL;
    axis2_soap_body_t *out_body = NULL;
    axis2_om_node_t *out_node = NULL;
    
    /* get the implementation class for the Web Service */
    svc_obj = AXIS2_MSG_RECV_GET_IMPL_OBJ(msg_recv, env, msg_ctx);
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    op_desc = AXIS2_OP_CTX_GET_OP(op_ctx, env);
    
    svc_obj = AXIS2_MSG_RECV_GET_IMPL_OBJ(msg_recv, env, msg_ctx);
    
    if (!svc_obj)
    {
        axis2_char_t message[1024];
        sprintf(message, "%s:%d - Impl object for service not set in message receiver", __FILE__, __LINE__);
        AXIS2_LOG(env, message, AXIS2_LOG_DEBUG);
        return AXIS2_FAILURE;
    }
    /* find the WebService method */
    /*Object obj = getTheImplementationObject(msgContext);

    // find the WebService method
    Class ImplClass = obj.getClass();

    //Inject the Message Context if it is asked for
    DependencyManager.configureBusinessLogicProvider(obj, msgContext, newmsgContext);

    AxisOperation opDesc = msgContext.getOperationContext()
            .getAxisOperation();
    Method method = findOperation(opDesc, ImplClass);
    if (method != null) {
        String style = msgContext.getOperationContext()
                .getAxisOperation()
                .getStyle();

        Class[] parameters = method.getParameterTypes();
        Object[] args = null;

        if (parameters == null || parameters.length == 0) {
            args = new Object[0];
        } else if (parameters.length == 1) 
        {
        */
    
    style = AXIS2_OP_GET_STYLE(op_desc, env);
    if(0 == AXIS2_STRCMP(AXIS2_STYLE_DOC, style))
    {
        axis2_soap_envelope_t *envelope = NULL;
        axis2_soap_body_t *body = NULL;
        
        envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
        body = AXIS2_SOAP_ENVELOPE_GET_BODY(envelope, env);
        om_node = AXIS2_SOAP_BODY_GET_BASE_NODE(body, env);
        om_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(om_node, env);
        
    }
    else if(0 == AXIS2_STRCMP(AXIS2_STYLE_RPC, style))
    {
        axis2_soap_envelope_t *envelope = NULL;
        axis2_soap_body_t *body = NULL;
        axis2_om_node_t *op_node = NULL;
        axis2_om_element_t *op_element = NULL;
        
        envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
        body = AXIS2_SOAP_ENVELOPE_GET_BODY(envelope, env);
        op_node = AXIS2_SOAP_BODY_GET_BASE_NODE(body, env);
        op_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(op_node, env);
        if(NULL != op_element)
        {
            local_name = AXIS2_OM_ELEMENT_GET_LOCALNAME(op_element, env);
            if(NULL != local_name)
            {
                axis2_array_list_t *function_arr = NULL;
                int i = 0;
                int size = 0;
                axis2_bool_t matches = AXIS2_FALSE;
                
                function_arr = svc_obj->func_array;
                size = AXIS2_ARRAY_LIST_SIZE(function_arr, env);
                if(AXIS2_TRUE != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
                {
                    return AXIS2_FAILURE;
                }
                
                for (i = 0; i < size; i++) 
                {
                    axis2_char_t *function_name = NULL;
                    
                    function_name = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(
                        function_arr, env, i);
                    if(0 == AXIS2_STRCMP(function_name, local_name))
                    {
                        matches = AXIS2_TRUE;
                        
                    }
                }
                if(AXIS2_TRUE == matches)
                {
                    om_node = AXIS2_OM_NODE_GET_FIRST_CHILD(op_node, env);
                    om_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(om_node, env);   
                }
                else
                {
                    AXIS2_ERROR_SET((*env)->error, 
                        AXIS2_ERROR_OM_ELEMENT_MISMATCH, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
            }
            else
            {
                AXIS2_ERROR_SET((*env)->error, 
                    AXIS2_ERROR_OM_ELEMENT_INVALID_STATE, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
        }
        else
        {
            AXIS2_ERROR_SET((*env)->error, 
                    AXIS2_ERROR_RPC_NEED_MATCHING_CHILD, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    else
    {
        AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_UNKNOWN_STYLE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    result_node = AXIS2_SVC_SKELETON_INVOKE(svc_obj, env, om_node);
    if(0 == AXIS2_STRCMP(style, AXIS2_STYLE_RPC))
    {
        axis2_om_namespace_t *ns = NULL;
        axis2_char_t *res_name = NULL;
        
        default_envelope = axis2_soap_envelope_create_null(env);
        out_body = AXIS2_SOAP_ENVELOPE_GET_BODY(default_envelope, env);
        if(!default_envelope)
        {
            return AXIS2_FAILURE;
        }
        res_name = AXIS2_STRACAT(local_name, "Response", env);
        ns = axis2_om_namespace_create(env, "http://soapenc/", "res");
        if(!ns)
        {
            return AXIS2_FAILURE;
        }
        out_node = AXIS2_SOAP_BODY_GET_BASE_NODE(out_body, env);
        body_content_element = axis2_om_element_create(env, out_node, res_name, 
            ns, &body_content_node);
        
        AXIS2_OM_NODE_ADD_CHILD(body_content_node, env, result_node);
        
    }
    else
    {
        body_content_node = result_node;
    }
    return AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(new_msg_ctx, env, default_envelope);
}

axis2_status_t AXIS2_CALL
axis2_raw_xml_in_out_msg_recv_receive(axis2_msg_recv_t *msg_recv,
                                    axis2_env_t **env,
                                    axis2_msg_ctx_t *msg_ctx)
{
    axis2_msg_ctx_t *out_msg_ctx = NULL;
    axis2_engine_t *engine = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_svc_ctx_t *svc_ctx = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_recv, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    
    out_msg_ctx = axis2_core_utils_create_out_msg_ctx(env, msg_ctx);
    axis2_raw_xml_in_out_msg_recv_invoke_business_logic(msg_recv, env, msg_ctx,
        out_msg_ctx);
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    svc_ctx = AXIS2_OP_CTX_GET_PARENT(op_ctx, env);
    conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(svc_ctx, env);
    engine = axis2_engine_create(env, conf_ctx);
    if(!engine)
    {
        return AXIS2_FAILURE;
    }        
    return AXIS2_ENGINE_SEND(engine, env, out_msg_ctx);
    
}

/*
public Method findOperation(AxisOperation op, Class ImplClass) {
    Method method = null;
    String methodName = op.getName().getLocalPart();
    Method[] methods = ImplClass.getMethods();
    for (int i = 0; i < methods.length; i++) {
        if (methods[i].getName().equals(methodName)) {
            method = methods[i];
            break;
        }
    }
    return method;
}
*/

/**
 * Following block distinguish the exposed part of the dll.
 */


int axis2_get_instance(struct axis2_msg_recv **inst,
                        axis2_env_t **env)
{
    *inst = axis2_raw_xml_in_out_msg_recv_create(env);
    if(!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

int axis2_remove_instance(axis2_svc_skeleton_t *inst,
                            axis2_env_t **env)
{
    axis2_status_t status = AXIS2_FAILURE;
    if (inst)
    {
        status = AXIS2_MSG_RECV_FREE(inst, env);
    }
    return status;
}
