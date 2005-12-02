#include <axis2_svc.h>

typedef struct axis2_svc_impl axis2_svc_impl_t;
	
/** 
 * @brief Service struct impl
 *	Axis2 Service impl  
 */ 
struct axis2_svc_impl
{
	axis2_svc_t svc;
	struct axis2_svc_grp *parent;
	axis2_hash_t *wasaction_opeartionmap;  
    axis2_char_t *axis2_svc_name; 
    /** to keep the time that last update time of the service */
    long last_update;
    axis2_char_t * filename;
    struct axis2_wsdl_svc *service_impl;
    /** to store module ref at deploy time parsing */
    axis2_array_list_t *module_list;    

};

#define AXIS2_INTF_TO_IMPL(svc) ((axis2_svc_impl_t *)svc)

/*************************** Function headers ********************************/

axis2_status_t AXIS2_CALL 
axis2_svc_free (axis2_svc_t *svc, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_add_operation (axis2_svc_t *svc, axis2_env_t **env
		,                   struct axis2_operation *operation);

struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_with_qname (axis2_svc_t *svc, axis2_env_t **env,
		                            axis2_qname_t *operation_name);
		
struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_with_name (axis2_svc_t *svc, axis2_env_t **env,
		                            const axis2_char_t* operation_name);

axis2_hash_t * AXIS2_CALL
axis2_svc_get_operations (axis2_svc_t *svc, axis2_env_t **env);
		
axis2_status_t AXIS2_CALL
axis2_svc_set_parent (axis2_svc_t *svc, axis2_env_t **env, 
                        axis2_svc_grp_t *svc_grp);

axis2_svc_grp_t * AXIS2_CALL
axis2_svc_get_parent (axis2_svc_t *svc, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_name (axis2_svc_t *svc, 
                    axis2_env_t **env,
                    axis2_qname_t *qname);

axis2_qname_t * AXIS2_CALL
axis2_svc_get_name (const axis2_svc_t *svc, 
                    axis2_env_t **env);	

axis2_status_t AXIS2_CALL
axis2_svc_add_param (axis2_svc_t *svc, 
                        axis2_env_t **env, 
                        axis2_param_t *param);

axis2_param_t * AXIS2_CALL
axis2_svc_get_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                const axis2_char_t *name);

axis2_array_list_t * AXIS2_CALL
axis2_svc_get_params (axis2_svc_t *svc, 
                        axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked (axis2_svc_t *svc, 
                            axis2_env_t **env,
		                    const axis2_char_t *param_name);

axis2_status_t AXIS2_CALL
axis2_svc_set_svc_interface(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_wsdl_interface *svc_interface);

struct axis2_wsdl_interface * AXIS2_CALL
axis2_svc_get_svc_interface(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_engage_module(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_module_desc * moduleref,
                            struct axis2_engine_config * axis2_config);

axis2_status_t AXIS2_CALL
axis2_svc_add_module_operations(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_module_desc * module,
                            struct axis2_engine_config * axis2_config);
                                
axis2_status_t AXIS2_CALL
axis2_svc_add_to_engaged_module_list(axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        struct axis2_module_desc *module_name);
                                    
axis2_array_list_t * AXIS2_CALL
axis2_svc_get_engaged_modules(axis2_svc_t *svc,
                                axis2_env_t **env);


void *AXIS2_CALL
axis2_svc_get_wsdl_operation(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_qname_t *operation_name);

axis2_status_t AXIS2_CALL
axis2_svc_set_context_path(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *context_path);

axis2_char_t * AXIS2_CALL
axis2_svc_get_context_path(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_style(axis2_svc_t *svc,
                        axis2_env_t **env,
                        axis2_char_t * style);

axis2_char_t * AXIS2_CALL
axis2_svc_get_style(axis2_svc_t *svc,
                    axis2_env_t **env);

struct axis2_flow * AXIS2_CALL
axis2_svc_get_inflow(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_inflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *inflow);

struct axis2_flow * AXIS2_CALL
axis2_svc_get_outflow(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_outflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *outflow);

struct axis2_flow *AXIS2_CALL
axis2_svc_get_fault_inflow(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_fault_inflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *fault_flow);

struct axis2_flow * AXIS2_CALL
axis2_svc_get_fault_outflow(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_fault_outflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *fault_flow);

struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_by_soap_action(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *soap_action);


struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_by_soap_action_and_endpoint(axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        axis2_char_t *soap_action,
                                        axis2_qname_t * endpoint);       

axis2_char_t * AXIS2_CALL
axis2_svc_get_axis2_svc_name(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_axis2_svc_name(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *axis2_svc_name);

axis2_status_t AXIS2_CALL
axis2_svc_set_last_update(axis2_svc_t *svc,
                            axis2_env_t **env);

long AXIS2_CALL
axis2_svc_get_last_update(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_char_t * AXIS2_CALL
axis2_svc_get_filename(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_filename(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *filename);

axis2_hash_t * AXIS2_CALL
axis2_svc_get_endpoints(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_endpoints(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_hash_t * endpoints);

axis2_status_t AXIS2_CALL
axis2_svc_set_endpoint(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_wsdl_endpoint * endpoint);

struct axis2_wsdl_endpoint * AXIS2_CALL
axis2_svc_get_endpoint(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_qname_t * qname);

axis2_char_t * AXIS2_CALL
axis2_svc_get_namespace(axis2_svc_t *svc,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_add_mapping(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t * mapping_key , 
                            struct axis2_operation * axis2_opt);
                            
/************************* End of function headers ***************************/

axis2_svc_t * AXIS2_CALL
axis2_svc_create (axis2_env_t **env)
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_array_list_t *array_list_l = NULL;
    struct axis2_param_container *param_container_l = NULL;
    struct axis2_wsdl_interface *wsdl_interface_l = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
	svc_impl = (axis2_svc_impl_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_svc_impl_t));
    if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    svc_impl->parent = NULL;
    svc_impl->axis2_svc_name = NULL;
    svc_impl->last_update = 0;
    svc_impl->svc.param_container = NULL;
    svc_impl->svc.flow_container = NULL;
    svc_impl->svc.wsdl_svc = NULL;
    svc_impl->wasaction_opeartionmap = NULL;
    svc_impl->module_list = NULL;
    
    svc_impl->svc.param_container = (struct axis2_param_container *)
		axis2_param_container_create(env);		
	if(NULL == svc_impl->svc.param_container)
	{
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}
    
    svc_impl->svc.flow_container = (struct axis2_flow_container *)
		axis2_flow_container_create(env);		
	if(NULL == svc_impl->svc.flow_container)
	{
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}
    
    
    svc_impl->svc.wsdl_svc = (axis2_wsdl_svc_t *)axis2_wsdl_svc_create(env);		
	if(NULL == svc_impl->svc.wsdl_svc)
	{
        axis2_svc_free(&(svc_impl->svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}
    
    svc_impl->wasaction_opeartionmap = axis2_hash_make (env);				
	if(NULL == svc_impl->wasaction_opeartionmap)
	{
        axis2_svc_free(&(svc_impl->svc), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}
    
    /** Create modle list of default size */
    svc_impl->module_list = axis2_array_list_create(env, 0);
    if(NULL == svc_impl->module_list)
    {
        axis2_svc_free(&(svc_impl->svc), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    svc_impl->svc.ops = AXIS2_MALLOC((*env)->allocator, sizeof(axis2_svc_ops_t));
	if(NULL == svc_impl->svc.ops)
	{
        axis2_svc_free(&(svc_impl->svc), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
	svc_impl->svc.ops->free = axis2_svc_free;
	svc_impl->svc.ops->add_operation = axis2_svc_add_operation;
	svc_impl->svc.ops->get_operation_with_qname = 
		axis2_svc_get_operation_with_qname;
	svc_impl->svc.ops->get_operation_with_name = 
		axis2_svc_get_operation_with_name;
	svc_impl->svc.ops->get_operations = axis2_svc_get_operations;
	svc_impl->svc.ops->set_parent = axis2_svc_set_parent;
	svc_impl->svc.ops->get_parent = axis2_svc_get_parent;
    svc_impl->svc.ops->set_name = axis2_svc_set_name;
	svc_impl->svc.ops->get_name = axis2_svc_get_name;
	svc_impl->svc.ops->add_param = axis2_svc_add_param;
	svc_impl->svc.ops->get_param = axis2_svc_get_param;
	svc_impl->svc.ops->get_params = axis2_svc_get_params;
    svc_impl->svc.ops->is_param_locked = axis2_svc_is_param_locked;
    svc_impl->svc.ops->set_svc_interface = axis2_svc_set_svc_interface;
    svc_impl->svc.ops->get_svc_interface = axis2_svc_get_svc_interface;
    
    svc_impl->svc.ops->engage_module = axis2_svc_engage_module;
    
    svc_impl->svc.ops->add_module_operations = axis2_svc_add_module_operations;
    
    svc_impl->svc.ops->add_to_engaged_module_list = axis2_svc_add_to_engaged_module_list;
    
    svc_impl->svc.ops->get_engaged_modules = axis2_svc_get_engaged_modules;
    
    svc_impl->svc.ops->get_wsdl_operation = axis2_svc_get_wsdl_operation;
    
    svc_impl->svc.ops->set_context_path = axis2_svc_set_context_path;
    
    svc_impl->svc.ops->get_context_path = axis2_svc_get_context_path;
    
    svc_impl->svc.ops->set_style = axis2_svc_set_style;
    
    svc_impl->svc.ops->get_style = axis2_svc_get_style;
    
    svc_impl->svc.ops->get_inflow = axis2_svc_get_inflow;
    
    svc_impl->svc.ops->set_inflow = axis2_svc_set_inflow;
    
    svc_impl->svc.ops->get_outflow = axis2_svc_get_outflow;
    
    svc_impl->svc.ops->set_outflow = axis2_svc_set_outflow;
    
    svc_impl->svc.ops->get_fault_inflow = axis2_svc_get_fault_inflow;
    
    svc_impl->svc.ops->set_fault_inflow = axis2_svc_set_fault_inflow;
    
    svc_impl->svc.ops->get_fault_outflow = axis2_svc_get_fault_outflow;
    
    svc_impl->svc.ops->set_fault_outflow = axis2_svc_set_fault_outflow;
    
    svc_impl->svc.ops->get_operation_by_soap_action = axis2_svc_get_operation_by_soap_action;
    
    svc_impl->svc.ops->get_operation_by_soap_action_and_endpoint = axis2_svc_get_operation_by_soap_action_and_endpoint;
    
    svc_impl->svc.ops->get_axis2_svc_name = axis2_svc_get_axis2_svc_name;
    
    svc_impl->svc.ops->set_axis2_svc_name = axis2_svc_set_axis2_svc_name;
    
    svc_impl->svc.ops->set_last_update = axis2_svc_set_last_update;
    
    svc_impl->svc.ops->get_last_update = axis2_svc_get_last_update;
    
    svc_impl->svc.ops->get_filename = axis2_svc_get_filename;
    
    svc_impl->svc.ops->set_filename = axis2_svc_set_filename;
    
    svc_impl->svc.ops->get_endpoints = axis2_svc_get_endpoints;
    
    svc_impl->svc.ops->set_endpoints = axis2_svc_set_endpoints;
    
    svc_impl->svc.ops->set_endpoint = axis2_svc_set_endpoint;
    
    svc_impl->svc.ops->get_endpoint = axis2_svc_get_endpoint;
    
    svc_impl->svc.ops->get_namespace = axis2_svc_get_namespace;
    
    svc_impl->svc.ops->add_mapping = axis2_svc_add_mapping;

       
    wsdl_interface_l = axis2_wsdl_interface_create(env);
    if(NULL == wsdl_interface_l)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    axis2_svc_set_svc_interface(&(svc_impl->svc), env, wsdl_interface_l); 
    
    param_container_l = axis2_param_container_create(env);
    if(NULL == param_container_l)
       AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc_impl->svc.wsdl_svc->
        wsdl_component, env, PARAMETER_KEY, param_container_l);
    
    array_list_l = axis2_array_list_create(env, 0);
    if(NULL == array_list_l)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc_impl->svc.wsdl_svc->
        wsdl_component, env, MODULEREF_KEY, array_list_l);
	return &(svc_impl->svc);	
}

axis2_svc_t * AXIS2_CALL
axis2_svc_create_with_qname (axis2_env_t **env, 
                                axis2_qname_t *qname)
{
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
	axis2_svc_impl_t *svc_impl = AXIS2_INTF_TO_IMPL(axis2_svc_create(env));
	if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
	axis2_svc_set_name(&(svc_impl->svc), env, qname);
	
	return &(svc_impl->svc);
}

axis2_svc_t * AXIS2_CALL
axis2_svc_create_with_wsdl_svc (axis2_env_t **env, 
                                axis2_wsdl_svc_t *wsdl_svc)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_svc, NULL);
    
	axis2_svc_impl_t *svc_impl = 
        AXIS2_INTF_TO_IMPL(axis2_svc_create(env));
    
	if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
	if(NULL != svc_impl->svc.wsdl_svc)
        AXIS2_WSDL_SVC_FREE(svc_impl->svc.wsdl_svc, env);
    svc_impl->svc.wsdl_svc = wsdl_svc;
       
	return &(svc_impl->svc);	
}

/********************** Start of function implementations ********************/

axis2_status_t AXIS2_CALL
axis2_svc_free (axis2_svc_t *svc, 
                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
	if(NULL != svc->ops)
		AXIS2_FREE((*env)->allocator, svc->ops);
    
    if(NULL != svc->param_container)
	    AXIS2_PARAM_CONTAINER_FREE(svc->param_container, env);
    
    if(NULL != svc->flow_container)
	    AXIS2_FLOW_CONTAINER_FREE(svc->flow_container, env);
    
    if(NULL != svc->wsdl_svc)
	    AXIS2_PARAM_CONTAINER_FREE(svc->wsdl_svc, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->parent)
	    AXIS2_SVC_GRP_FREE(AXIS2_INTF_TO_IMPL(svc)->parent, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap)
		axis2_hash_free(AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->module_list)
	    AXIS2_ARRAY_LIST_FREE(AXIS2_INTF_TO_IMPL(svc)->module_list, env);

    if(NULL != AXIS2_INTF_TO_IMPL(svc)->axis2_svc_name)
        AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(svc)->axis2_svc_name);        
    
	AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(svc));
    
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_svc_add_operation (axis2_svc_t *svc,
                            axis2_env_t **env,
		                    struct axis2_operation *axis2_opt)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, axis2_opt, AXIS2_FALSE);
    
    AXIS2_OPERATION_SET_PARENT(axis2_opt, env, svc);
    return AXIS2_WSDL_INTERFACE_SET_OPERATION(axis2_svc_get_svc_interface(svc, env),
        env, axis2_opt);
}

struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_with_qname (axis2_svc_t *svc,
                                        axis2_env_t **env,
		                                axis2_qname_t *operation_name)
{
    struct axis2_operation *operation_l = NULL;
    axis2_char_t *op_str = NULL;
    /*axis2_hash_t *all_operations = NULL; */
    
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, operation_name, NULL);
    
    op_str = AXIS2_QNAME_GET_LOCALPART(operation_name, env);
    /* commented until AXIS2_WSDL_INTERFACE_GET_ALL_OPERATIONS is implemented
    all_operations = AXIS2_WSDL_INTERFACE_GET_ALL_OPERATIONS(
        axis2_svc_get_svc_interface(svc, env), env);
    opeartion_l = (struct axis2_operation) (axis2_hash_get (all_operations, 
        op_str, AXIS2_HASH_KEY_STRING));
    */
    if(NULL == operation_l )
    {
        operation_l = (struct axis2_operation *) (axis2_hash_get (
                AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap, op_str,
                AXIS2_HASH_KEY_STRING));
    }
    
    return operation_l;	
}	

struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_with_name (axis2_svc_t *svc, 
                                    axis2_env_t **env,
		                            const axis2_char_t* nc_name)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, nc_name, NULL);
    
    return (axis2_operation_t *) axis2_hash_get(AXIS2_WSDL_INTERFACE_GET_OPERATIONS(
        axis2_svc_get_svc_interface(svc, env), env), nc_name, AXIS2_HASH_KEY_STRING);
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_operations (axis2_svc_t *svc, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    
    return AXIS2_WSDL_INTERFACE_GET_OPERATIONS(axis2_svc_get_svc_interface(svc,
        env), env);
}
	
axis2_status_t AXIS2_CALL
axis2_svc_set_parent (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                axis2_svc_grp_t *svc_grp)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_grp, AXIS2_FAILURE);
    axis2_svc_impl_t *svc_impl = AXIS2_INTF_TO_IMPL(svc);
	if(svc_impl->parent)
        AXIS2_SVC_GRP_FREE(svc_impl->parent, env);
	svc_impl->parent = svc_grp;
    
	return AXIS2_SUCCESS;
}

axis2_svc_grp_t * AXIS2_CALL
axis2_svc_get_parent (axis2_svc_t *svc, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    if(NULL == AXIS2_INTF_TO_IMPL(svc)->parent)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_PARENT_NULL, NULL);
	
	return AXIS2_INTF_TO_IMPL(svc)->parent;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_name (axis2_svc_t *svc, 
                    axis2_env_t **env,
                    axis2_qname_t *qname)
{
	AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    
    return AXIS2_WSDL_SVC_SET_NAME(svc->wsdl_svc, env, qname);
}

axis2_qname_t * AXIS2_CALL
axis2_svc_get_name (const axis2_svc_t *svc, 
                    axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    return AXIS2_WSDL_SVC_GET_NAME(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_add_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                axis2_param_t *param)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
    struct axis2_param_container *param_container_l = NULL;
   
    if(axis2_svc_is_param_locked(svc, env, AXIS2_PARAM_GET_NAME(param, env)))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE,
            AXIS2_FAILURE);
    } else
    {
        param_container_l = (struct axis2_param_container *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->
                wsdl_component, env, PARAMETER_KEY);
        return AXIS2_PARAM_CONTAINER_ADD_PARAM(param_container_l, env, param);
    }
    return AXIS2_FAILURE;
}

axis2_param_t * AXIS2_CALL
axis2_svc_get_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                const axis2_char_t *name)
{
    struct axis2_param_container *param_container_l = NULL;
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    
    param_container_l = (struct axis2_param_container *)
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->
            wsdl_component, env, PARAMETER_KEY);
    return AXIS2_PARAM_CONTAINER_GET_PARAM(param_container_l, env, name);
}

axis2_array_list_t * AXIS2_CALL
axis2_svc_get_params (axis2_svc_t *svc, 
                        axis2_env_t **env)
{
    struct axis2_param_container *param_container_l = NULL;
	AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
	
    param_container_l = (struct axis2_param_container *)
    AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->
        wsdl_component, env, PARAMETER_KEY);
    
    return AXIS2_PARAM_CONTAINER_GET_PARAMS(param_container_l, env);
}

axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked (axis2_svc_t *svc, 
                            axis2_env_t **env,
		                    const axis2_char_t *param_name)
{
    axis2_bool_t locked = AXIS2_FALSE;
    axis2_param_t *param_l = NULL;
    struct axis2_engine_config *engine_config_l = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);
    
   /*checking the locked value of parent */

    if (NULL != axis2_svc_get_parent(svc, env)) 
    {
        /*commented until AXIS2_SVC_GRP_GET_ENGINE_CONFIG is implemented
        engine_config_l = AXIS2_SVC_GRP_GET_ENGINE_CONFIG(
            AXIS2_SVC_GET_PARENT(svc, env), env);
        locked =  AXIS2_ENGINE_CONFIG_IS_PARAM_LOCKED(engine_config_l, env, param_name);
        */
    }
    if(AXIS2_TRUE == locked)
    {
        return AXIS2_TRUE;
    } else 
    {
        param_l = axis2_svc_get_param(svc, env, param_name);
        return (NULL != param_l && AXIS2_PARAM_IS_LOCKED(param_l, env));
    }
}

axis2_status_t AXIS2_CALL
axis2_svc_set_svc_interface(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_wsdl_interface *svc_interface) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_interface, AXIS2_FAILURE);
    return AXIS2_WSDL_SVC_SET_SVC_INTERFACE(svc->wsdl_svc, env, svc_interface);
}

struct axis2_wsdl_interface *AXIS2_CALL
axis2_svc_get_svc_interface(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    return AXIS2_WSDL_SVC_GET_SVC_INTERFACE(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_engage_module(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_module_desc * moduleref,
                            struct axis2_engine_config * axis2_config)
{
    struct axis2_module_desc * modu = NULL;
    axis2_array_list_t *collection_module = NULL;
    struct axis2_phase_resolver *phase_resolver = NULL;
    int i = 0;
    axis2_status_t status = AXIS2_FAILURE;
        
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, moduleref, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, axis2_config, AXIS2_FAILURE);
    
    
    collection_module = (axis2_array_list_t *) 
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->wsdl_component,
            env, MODULEREF_KEY);
    
    for(i = 0; i < AXIS2_ARRAY_LIST_SIZE(collection_module, env); i++)
    {
        modu = (struct axis2_module_desc *) AXIS2_ARRAY_LIST_GET(collection_module,
            env, i);
        if(AXIS2_QNAME_EQUALS(AXIS2_MODULE_DESC_GET_NAME(modu, env), env,  
            AXIS2_MODULE_DESC_GET_NAME(moduleref, env)))
        {
            /* module has alredy been engaged on the service. Operation terminated !!! */
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC,
                AXIS2_FAILURE);
            return AXIS2_FAILURE;            
        }
        
    }
   
    phase_resolver = axis2_phase_resolver_create_with_config(env, axis2_config);
    status = AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_SVC(phase_resolver, env, svc, moduleref);
    status = AXIS2_ARRAY_LIST_ADD(collection_module, env, moduleref);
    
    return status;
}

axis2_status_t AXIS2_CALL
axis2_svc_add_module_operations(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_module_desc * module_desc,
                            struct axis2_engine_config * axis2_config) 
{
    axis2_hash_t * map = NULL;
    axis2_hash_index_t *index = NULL;
    void *v = NULL;
    struct axis2_phase_resolver * pr = NULL;
    struct axis2_operation * axis2_opt = NULL;
    axis2_array_list_t *params = NULL;
    struct axis2_param *param = NULL;
    int j = 0;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_desc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, axis2_config, AXIS2_FAILURE);
    
    map = AXIS2_MODULE_DESC_GET_OPERATIONS(module_desc, env);
    pr = axis2_phase_resolver_create_with_config_and_svc(env, axis2_config, svc);
    
    for (index = axis2_hash_first (map, env); index; index = 
        axis2_hash_next (env, index))
    {

        axis2_hash_this (index, NULL, NULL, &v);
        axis2_opt = (struct axis2_operation *) v;
        params = AXIS2_OPERATION_GET_PARAMS(axis2_opt, env);
        /* Adding wsa-maping into service */
        for(j = 0; j < AXIS2_ARRAY_LIST_SIZE(params, env); j++)
        {
            param = (struct axis2_param *) AXIS2_ARRAY_LIST_GET(params, env, j);
            if(0 == AXIS2_STRCMP( AXIS2_PARAM_GET_NAME(param, env), WSA_ACTION))
            {
                status = axis2_svc_add_mapping(svc, env,
                    (axis2_char_t *) AXIS2_PARAM_GET_VALUE(param, env), axis2_opt);
                if(AXIS2_FAILURE == status)
                {
                    if(pr)
                        AXIS2_PHASE_RESOLVER_FREE(pr, env); 
                    return status;
                }
            }
                
        }
        status = AXIS2_PHASE_RESOLVER_BUILD_MODULE_OPERATION(pr, env, axis2_opt);
        if(AXIS2_FAILURE == status)
        {
            if(pr)
                AXIS2_PHASE_RESOLVER_FREE(pr, env); 
            return status;
        }
        
        status = axis2_svc_add_operation(svc, env, axis2_opt);
 
    }
    if(pr)
        AXIS2_PHASE_RESOLVER_FREE(pr, env); 
    return status;
}

axis2_status_t AXIS2_CALL
axis2_svc_add_to_engaged_module_list(axis2_svc_t *svc,
                                axis2_env_t **env,
                                struct axis2_module_desc *module_name) 
{
    axis2_array_list_t *collection_module = NULL;
    struct axis2_module_desc * module_desc = NULL;
    int i = 0;
     
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_name, AXIS2_FAILURE);
    
    collection_module = (axis2_array_list_t *) 
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(svc->wsdl_svc->wsdl_component, 
            env, MODULEREF_KEY);
    for(i = 0; i < AXIS2_ARRAY_LIST_SIZE(collection_module, env); i++)
    {
        module_desc = (struct axis2_module_desc *) AXIS2_ARRAY_LIST_GET(
            collection_module, env, i);
        if(AXIS2_QNAME_EQUALS(AXIS2_MODULE_DESC_GET_NAME(module_desc, env), env,
                AXIS2_MODULE_DESC_GET_NAME(module_name, env)))
        {
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_ARRAY_LIST_ADD(collection_module, env, module_name);
}

axis2_array_list_t * AXIS2_CALL
axis2_svc_get_engaged_modules(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    
    return (axis2_array_list_t *) AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, MODULEREF_KEY);
}

void * AXIS2_CALL
axis2_svc_get_wsdl_operation(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_qname_t *operation_name)
{
    struct axis2_wsdl_interface *svc_interface = NULL;
        
    axis2_char_t * op_str = NULL;
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, operation_name, NULL);
    
    op_str = AXIS2_QNAME_GET_LOCALPART(operation_name, env);
    svc_interface = axis2_svc_get_svc_interface(svc, env);
    
    return AXIS2_WSDL_INTERFACE_GET_OPERATION(svc_interface, env, op_str) ;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_context_path(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *context_path) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, context_path, AXIS2_FAILURE);
    
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->wsdl_component,
        env, CONTEXTPATH_KEY, context_path);
}

axis2_char_t * AXIS2_CALL
axis2_svc_get_context_path(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    return (axis2_char_t *) AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, CONTEXTPATH_KEY);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_style(axis2_svc_t *svc,
                        axis2_env_t **env,
                        axis2_char_t * style) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, style, AXIS2_FAILURE);
    
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->wsdl_component,
        env, STYLE_KEY, style);
}

axis2_char_t * AXIS2_CALL
axis2_svc_get_style(axis2_svc_t *svc,
                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    return (axis2_char_t *) AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, STYLE_KEY);
}

struct axis2_flow * AXIS2_CALL
axis2_svc_get_inflow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    return (struct axis2_flow *) AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, INFLOW_KEY);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_inflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *inflow) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, inflow, AXIS2_FAILURE);
    
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->wsdl_component,
        env, INFLOW_KEY, inflow);
}

struct axis2_flow * AXIS2_CALL
axis2_svc_get_outflow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    return (struct axis2_flow *) AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, OUTFLOW_KEY);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_outflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *outflow) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, outflow, AXIS2_FAILURE);
    
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->
        wsdl_component, env, OUTFLOW_KEY, outflow);
}

struct axis2_flow *AXIS2_CALL
axis2_svc_get_fault_inflow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    
    return (struct axis2_flow *) AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, IN_FAULTFLOW_KEY);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_fault_inflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *fault_flow) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, fault_flow, AXIS2_FAILURE);
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->wsdl_component,
        env, IN_FAULTFLOW_KEY, fault_flow);
}

struct axis2_flow * AXIS2_CALL
axis2_svc_get_fault_outflow(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    
    return (struct axis2_flow *) AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        svc->wsdl_svc->wsdl_component, env, OUT_FAULTFLOW_KEY);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_fault_outflow(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_flow *fault_flow) 
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, fault_flow, AXIS2_FAILURE);
    
    return AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(svc->wsdl_svc->wsdl_component, env,
        OUT_FAULTFLOW_KEY, fault_flow);
}

struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_by_soap_action(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *soap_action) 
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *endpoints = NULL;
    axis2_hash_index_t *index = NULL;
    void *value = NULL;
    struct axis2_wsdl_endpoint * endpoint = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, soap_action, NULL);
    if(0 == AXIS2_STRCMP(soap_action, ""))
        return NULL;
    
    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    
    endpoints = axis2_svc_get_endpoints(svc, env);
    index = axis2_hash_first (endpoints, env);
    if(NULL != index)
    {
        axis2_hash_this(index, NULL, NULL, &value);
        endpoint = (struct axis2_wsdl_endpoint *) value;
        return axis2_svc_get_operation_by_soap_action_and_endpoint(svc, env, soap_action,
            AXIS2_WSDL_ENDPOINT_GET_NAME(endpoint, env));
    }

    return NULL;
}

struct axis2_operation * AXIS2_CALL
axis2_svc_get_operation_by_soap_action_and_endpoint(axis2_svc_t *svc,
                                        axis2_env_t **env,
                                        axis2_char_t *soap_action,
                                        axis2_qname_t * endpoint) 
{
    axis2_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *binding_operations = NULL;
    struct axis2_operation * axis2_opt = NULL;
    int count = 0;
    int j = 0;
    axis2_hash_index_t *index_i = NULL;
    void *k = NULL;
    struct axis2_wsdl_binding_operation *binding_operation = NULL;
    struct axis2_wsdl_soap_operation *element = NULL;
    struct axis2_operation *op = NULL;
    axis2_linked_list_t *extensiblity_elements = NULL;
    axis2_qname_t *type_1 = NULL;
    axis2_qname_t *type_2 = NULL;
        
    svc_impl = AXIS2_INTF_TO_IMPL(svc);
    
    type_1 = axis2_qname_create(env, "operation", SOAP_11_OPERATION, NULL);
    binding_operations = AXIS2_WSDL_BINDING_GET_BINDING_OPERATIONS(
        AXIS2_WSDL_ENDPOINT_GET_BINDING(axis2_svc_get_endpoint(svc, env, endpoint),
            env), env);
    
    index_i = axis2_hash_first (binding_operations, env);
    do
    {
        axis2_hash_this (index_i, &k, NULL, NULL);
        binding_operation = (struct axis2_wsdl_binding_operation *) k;
        extensiblity_elements = AXIS2_WSDL_COMPONENT_GET_EXTENSIBILITY_ELEMENTS(
            binding_operation->extensible_component->wsdl_component, env);
        do
        {
            element = (struct axis2_wsdl_soap_operation *) AXIS2_LINKED_LIST_GET(
                extensiblity_elements, env, j); 
                        
            type_2 = AXIS2_WSDL_EXTENSIBLE_ELEMENT_GET_TYPE(element->extensible_element, env);
            if(AXIS2_QNAME_EQUALS(type_2, env, type_1))
            {
                if(0 == AXIS2_STRCMP(AXIS2_WSDL_SOAP_OPERATION_GET_SOAP_ACTION(
                    element, env), soap_action))
                {
                    op = (struct axis2_operation *) 
                        AXIS2_WSDL_BINDING_OPERATION_GET_OPERATION(binding_operation,
                            env);
                    count++;    
                }
                    
            }
            j++;
        }while(j < AXIS2_LINKED_LIST_SIZE(extensiblity_elements, env));
        index_i = axis2_hash_next (env, index_i);
    } while(NULL != index_i);
    if (1 == count) 
    {
        return op;
    }
    
    return NULL;
}
       
axis2_char_t * AXIS2_CALL
axis2_svc_get_axis2_svc_name(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(svc)->axis2_svc_name;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_axis2_svc_name(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *axis2_svc_name) 
{
    axis2_svc_impl_t *svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if(svc_impl->axis2_svc_name)
        AXIS2_FREE((*env)->allocator, svc_impl->axis2_svc_name);
    svc_impl->axis2_svc_name = axis2_svc_name;
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_svc_set_last_update(axis2_svc_t *svc,
                            axis2_env_t **env)
{
    /*last_update = new Date().getTime(); */
    return AXIS2_SUCCESS;
}

long AXIS2_CALL
axis2_svc_get_last_update(axis2_svc_t *svc,
                            axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(svc)->last_update;
}

axis2_char_t * AXIS2_CALL
axis2_svc_get_filename(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(svc)->filename;
}

axis2_status_t AXIS2_CALL
axis2_svc_set_filename(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t *filename) 
{
    axis2_svc_impl_t *svc_impl = AXIS2_INTF_TO_IMPL(svc);
    if(svc_impl->filename)
        AXIS2_FREE((*env)->allocator, svc_impl->filename);
    AXIS2_INTF_TO_IMPL(svc)->filename = filename;
    return AXIS2_SUCCESS;
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_endpoints(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return AXIS2_WSDL_SVC_GET_ENDPOINTS(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_endpoints(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_hash_t * endpoints) 
{
    return AXIS2_WSDL_SVC_SET_ENDPOINTS(svc->wsdl_svc, env, endpoints);
}

axis2_status_t AXIS2_CALL
axis2_svc_set_endpoint(axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_wsdl_endpoint * endpoint)
{
    return AXIS2_WSDL_SVC_SET_ENDPOINT(svc->wsdl_svc, env, endpoint);
}

struct axis2_wsdl_endpoint * AXIS2_CALL
axis2_svc_get_endpoint(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_qname_t * qname) 
{
    return AXIS2_WSDL_SVC_GET_ENDPOINT(svc->wsdl_svc, env, qname);
}

axis2_char_t * AXIS2_CALL
axis2_svc_get_namespace(axis2_svc_t *svc,
                            axis2_env_t **env) 
{
    return AXIS2_WSDL_SVC_GET_NAMESPACE(svc->wsdl_svc, env);
}

axis2_status_t AXIS2_CALL
axis2_svc_add_mapping(axis2_svc_t *svc,
                            axis2_env_t **env,
                            axis2_char_t * mapping_key , 
                            struct axis2_operation * axis2_opt)
{
    axis2_svc_impl_t *svc_impl = AXIS2_INTF_TO_IMPL(svc);
    
    axis2_hash_set(svc_impl->wasaction_opeartionmap, mapping_key, 
        AXIS2_HASH_KEY_STRING, axis2_opt);
    return AXIS2_SUCCESS;
}
