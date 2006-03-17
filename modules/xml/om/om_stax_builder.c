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

#include <axis2_om_stax_builder.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>
#include <axis2_om_processing_instruction.h>
#include <axis2_om_comment.h>
#include <axis2_string.h>
#include <axis2_xml_writer.h>
#include <axis2_om_doctype.h>
#include <axis2_om_node_internal.h>
#include <axis2_om_stax_builder_internal.h>
/**************************** function prototypes *****************************/

axis2_om_node_t * AXIS2_CALL 
axis2_om_stax_builder_next (axis2_om_stax_builder_t *builder,
                            axis2_env_t **env);
                                 
axis2_status_t AXIS2_CALL 
axis2_om_stax_builder_discard_current_element (axis2_om_stax_builder_t *builder,
                                               axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_om_stax_builder_free(axis2_om_stax_builder_t *builder,
                           axis2_env_t **env);

                                            
axis2_om_document_t* AXIS2_CALL
axis2_om_stax_builder_get_document (axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env);
                                            
                                            
axis2_status_t  AXIS2_CALL
axis2_om_stax_builder_set_document(axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env,
                                    axis2_om_document_t *document);                           
   
axis2_bool_t  AXIS2_CALL
axis2_om_stax_builder_is_complete(axis2_om_stax_builder_t *builder,
                                   axis2_env_t **env);
                                                              											
int AXIS2_CALL 
axis2_om_stax_builder_next_with_token(axis2_om_stax_builder_t *builder,
                                          axis2_env_t **env);                                                                                                                                            											
													
/********************************* axis2_om_stax_builder_impl_t struct ********/
typedef struct axis2_om_stax_builder_impl_t
{
    axis2_om_stax_builder_t om_stax_builder;
    /** pull parser instance used by the builder */
    axis2_xml_reader_t *parser;
    /** last node the builder found */
    axis2_om_node_t *lastnode;
    
    axis2_om_node_t *root_node;
    /** document associated with the builder */
    axis2_om_document_t *document;
    /** done building the document? */
    axis2_bool_t done;
    /** parser was accessed? */
    axis2_bool_t parser_accessed;
    /** caching enabled? */
    axis2_bool_t cache;
    /** current event */
    int current_event;
    
    int element_level;
        
}axis2_om_stax_builder_impl_t;

/************************************** Macro *********************************/				

#define AXIS2_INTF_TO_IMPL(builder) ((axis2_om_stax_builder_impl_t*)builder)

/******************************************************************************/

													
AXIS2_DECLARE(axis2_om_stax_builder_t *)
axis2_om_stax_builder_create (axis2_env_t **env,
                              axis2_xml_reader_t *parser)
{
    axis2_om_stax_builder_impl_t *builder = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, parser, NULL);
    
    builder = (axis2_om_stax_builder_impl_t *)AXIS2_MALLOC (
                (*env)->allocator, sizeof(axis2_om_stax_builder_impl_t));

    if (!builder)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    builder->cache = AXIS2_TRUE;
    builder->parser_accessed = AXIS2_FALSE;
    builder->done = AXIS2_FALSE;
    builder->lastnode = NULL;
    builder->document = NULL;
    builder->parser = parser;
    builder->current_event = -1;
    builder->root_node = NULL;
    builder->element_level = 0;
   
    /* ops */
    builder->om_stax_builder.ops = NULL;
    builder->om_stax_builder.ops = (axis2_om_stax_builder_ops_t *) AXIS2_MALLOC(
                                        (*env)->allocator, 
                                        sizeof(axis2_om_stax_builder_ops_t));

    if (!builder->om_stax_builder.ops)
    {
        AXIS2_XML_READER_FREE (builder->parser, env);
        AXIS2_FREE ((*env)->allocator, builder);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    builder->om_stax_builder.ops->next = 
            axis2_om_stax_builder_next;
    builder->om_stax_builder.ops->discard_current_element = 
            axis2_om_stax_builder_discard_current_element;
        
	builder->om_stax_builder.ops->free = 
	        axis2_om_stax_builder_free;
	builder->om_stax_builder.ops->set_document = 
	        axis2_om_stax_builder_set_document;
	        
    builder->om_stax_builder.ops->next_with_token =
            axis2_om_stax_builder_next_with_token;                       
            
    builder->om_stax_builder.ops->is_complete =
            axis2_om_stax_builder_is_complete;
                                            
	
    return &(builder->om_stax_builder);
}


axis2_status_t
axis2_om_stax_builder_process_attributes (axis2_om_stax_builder_t *om_builder,
                                          axis2_env_t **env,
                                          axis2_om_node_t *element_node)
{
    int i = 0;
    int attribute_count;
    axis2_om_attribute_t *attribute = NULL;
    axis2_om_namespace_t *ns = NULL;
    axis2_char_t *uri = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *attr_name = NULL;
    axis2_char_t *attr_value = NULL;
    
    axis2_om_stax_builder_impl_t *builder_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, element_node, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(om_builder);
    

    attribute_count = AXIS2_XML_READER_GET_ATTRIBUTE_COUNT(
                               builder_impl->parser,env);
    for (i=1; i <= attribute_count ; i++)
    {
        axis2_om_element_t *temp_ele =  NULL;
        
        uri =  AXIS2_XML_READER_GET_ATTRIBUTE_NAMESPACE_BY_NUMBER(
                       builder_impl->parser, env, i);
              
        prefix = AXIS2_XML_READER_GET_ATTRIBUTE_PREFIX_BY_NUMBER(
                        builder_impl->parser, env, i);
        if (uri)
        {
            if (axis2_strcmp (uri, "") != 0)
            {
                axis2_om_element_t *om_ele = NULL;
                om_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(element_node,env);
                if(om_ele)
                {
                    ns = AXIS2_OM_ELEMENT_FIND_NAMESPACE ( om_ele ,
                        env , element_node, uri, prefix);
        
        /* newly added to handle "xml:*" attributes" (AXIS2_STRCMP(prefix, "xml") == 0) && */
                    if(!ns)
                    {
                        ns = axis2_om_namespace_create(env, uri, prefix);
                    }
                }                        
            }
        }
        attr_name = AXIS2_XML_READER_GET_ATTRIBUTE_NAME_BY_NUMBER(
                                 builder_impl->parser, env, i);
        
        attr_value = AXIS2_XML_READER_GET_ATTRIBUTE_VALUE_BY_NUMBER(
                                 builder_impl->parser, env, i);
        if(NULL != attr_name)
        {   
            attribute = axis2_om_attribute_create (env, attr_name, attr_value, ns);
            if(!attribute)
                return AXIS2_FAILURE;
            
            temp_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(element_node,env);
            if(NULL != temp_ele)
            {
                status = AXIS2_OM_ELEMENT_ADD_ATTRIBUTE (temp_ele, env, attribute, element_node);
            }
        }                        
        if(uri)
            AXIS2_XML_READER_XML_FREE(builder_impl->parser, env, uri);
        if(prefix)
            AXIS2_XML_READER_XML_FREE(builder_impl->parser, env, prefix);
        if(attr_name)
            AXIS2_XML_READER_XML_FREE(builder_impl->parser, env, attr_name);
        if(attr_value)
            AXIS2_XML_READER_XML_FREE(builder_impl->parser, env, attr_value);
    }
    return status;
}


axis2_om_node_t *
axis2_om_stax_builder_create_om_text (axis2_om_stax_builder_t * om_stax_builder,
                                      axis2_env_t **env)
{
    axis2_char_t *temp_value = NULL;
    axis2_om_node_t *node = NULL;
    axis2_om_stax_builder_impl_t *builder = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    if (!builder->lastnode)
    {
        AXIS2_ERROR_SET((*env)->error, 
                         AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL, AXIS2_FAILURE);
        return NULL;
    }
    temp_value = AXIS2_XML_READER_GET_VALUE (builder->parser, env);

    if (!temp_value)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_XML_READER_VALUE_NULL, AXIS2_FAILURE);
        return NULL;
    }
  
    if (AXIS2_OM_NODE_GET_BUILD_STATUS(builder->lastnode, env))
    {
        axis2_om_text_create (env, 
                              AXIS2_OM_NODE_GET_PARENT(builder->lastnode, env),
                              temp_value, &node);
  
    }
    else
    {
        axis2_om_text_create (env, builder->lastnode, temp_value, &node);
    }

    axis2_om_node_set_build_status(node , env, AXIS2_TRUE);
    builder->lastnode = node;
    
    AXIS2_XML_READER_XML_FREE(builder->parser , env, temp_value);
    return node;
}


axis2_status_t AXIS2_CALL
axis2_om_stax_builder_discard_current_element (axis2_om_stax_builder_t *om_stax_builder,
                                               axis2_env_t **env)
{
    axis2_om_node_t *element = NULL;
    axis2_om_node_t *prev_node = NULL;
    axis2_om_node_t *parent = NULL;
    axis2_om_stax_builder_impl_t *builder = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    element = builder->lastnode;

    if (AXIS2_OM_NODE_GET_BUILD_STATUS(element, env) || !(builder->cache))
    {
        AXIS2_ERROR_SET((*env)->error,
                AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD, AXIS2_FAILURE);
              
        return AXIS2_FAILURE;
    }

    builder->cache = AXIS2_FALSE;
    do
    {
        while (AXIS2_XML_READER_NEXT (builder->parser, env)
                != AXIS2_XML_READER_END_ELEMENT);
    }
    while (!(AXIS2_OM_NODE_GET_BUILD_STATUS(element, env)));

    /*All children of this element is pulled now */

    prev_node = AXIS2_OM_NODE_GET_PREVIOUS_SIBLING(element, env);
    if (prev_node)
    {
        AXIS2_OM_NODE_FREE_TREE(AXIS2_OM_NODE_GET_NEXT_SIBLING(prev_node, env), env);
        axis2_om_node_set_next_sibling(prev_node, env, NULL);        
    }
    else
    {
        parent = AXIS2_OM_NODE_GET_PARENT(element, env);
        AXIS2_OM_NODE_FREE_TREE(AXIS2_OM_NODE_GET_FIRST_CHILD(parent, env), env);
        axis2_om_node_set_first_child(parent, env, NULL);
        builder->lastnode = parent;
    }
    builder->cache = AXIS2_TRUE;

    return AXIS2_SUCCESS;
}

axis2_status_t
axis2_om_stax_builder_process_namespaces (axis2_om_stax_builder_t *om_stax_builder,
                                          axis2_env_t **env,
                                          axis2_om_node_t *node,
                                          int is_soap_element)
{
    axis2_status_t status = AXIS2_SUCCESS;
    int namespace_count = 0;
    axis2_om_namespace_t *om_ns = NULL;
    axis2_om_namespace_t *temp_ns = NULL;
    /* temp values */
    axis2_char_t *temp_prefix = NULL;
    axis2_char_t *temp_ns_prefix = NULL;
    axis2_char_t *temp_ns_uri    = NULL;
    
    axis2_om_stax_builder_impl_t *builder = NULL;
    int i=0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    namespace_count = AXIS2_XML_READER_GET_NAMESPACE_COUNT (builder->parser, env);
    for (i = 1; i <= namespace_count; i++)
    {
        temp_ns_prefix = AXIS2_XML_READER_GET_NAMESPACE_PREFIX_BY_NUMBER(
                                builder->parser, env, i);
                                     
        temp_ns_uri = AXIS2_XML_READER_GET_NAMESPACE_URI_BY_NUMBER(
                            builder->parser, env , i);
                            
        if(AXIS2_STRCMP(temp_ns_prefix,"xmlns") == 0 || !temp_ns_prefix)
        {
            /** default namespace case */
            /** !temp_ns_prefix is for guththila */         
             axis2_om_element_t *om_ele = NULL;
             om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
            
             om_ns = axis2_om_namespace_create ( env, temp_ns_uri, NULL );
             if(!om_ns || !om_ele)
                 return AXIS2_FAILURE;
             
             status = AXIS2_OM_ELEMENT_DECLARE_NAMESPACE( om_ele, env, node, om_ns);
            
             temp_ns = AXIS2_OM_ELEMENT_FIND_DECLARED_NAMESPACE(om_ele, env, temp_ns_uri, NULL);
             if(temp_ns)
             {
                AXIS2_OM_ELEMENT_SET_NAMESPACE (om_ele, env, om_ns, node); 
             }                 
             else
             {
                AXIS2_OM_NAMESPACE_FREE(om_ns, env);
                om_ns = NULL;
             }
        }
        else
        {       
            axis2_om_element_t *om_ele = NULL;
            om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
            
            om_ns = axis2_om_namespace_create ( env, temp_ns_uri, temp_ns_prefix );
            if(!om_ns || !om_ele)
                return AXIS2_FAILURE;
            
            status = AXIS2_OM_ELEMENT_DECLARE_NAMESPACE(om_ele, env, node, om_ns);
           /*        
            temp_ns = AXIS2_OM_ELEMENT_FIND_DECLARED_NAMESPACE(om_ele, 
                            env, temp_ns_uri,temp_ns_prefix);
           */
        }        
        AXIS2_XML_READER_XML_FREE(builder->parser, env, temp_ns_prefix);
        AXIS2_XML_READER_XML_FREE(builder->parser, env, temp_ns_uri);
        if (!om_ns)
        {
            /* something went wrong */
            return AXIS2_FAILURE;
        }                   
    }
    /* set own namespace */
    temp_prefix = AXIS2_XML_READER_GET_PREFIX ( builder->parser, env);
    if (temp_prefix)
    {
        om_ns = AXIS2_OM_ELEMENT_FIND_NAMESPACE (
                    (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env),
                    env, node, NULL, temp_prefix);
       
        if (om_ns)
        {
            axis2_om_element_t *om_ele = NULL;
            om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
            if(om_ele)
                AXIS2_OM_ELEMENT_SET_NAMESPACE (om_ele, env, om_ns, node);
        }
        else
        {
            AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    if(temp_prefix)
        AXIS2_XML_READER_XML_FREE(builder->parser, env, temp_prefix);
    
    return status;
}

axis2_om_node_t *
axis2_om_stax_builder_create_om_element (axis2_om_stax_builder_t *om_stax_builder,
                                         axis2_env_t **env)
{
    axis2_om_node_t *element_node = NULL;
    axis2_om_element_t *om_ele = NULL;
    axis2_om_stax_builder_impl_t *builder_impl = NULL;
    axis2_char_t *temp_localname = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, om_stax_builder, NULL);
    
    builder_impl = AXIS2_INTF_TO_IMPL(om_stax_builder);
    
    temp_localname  = AXIS2_XML_READER_GET_NAME (builder_impl->parser, env);

    if (!temp_localname)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL, AXIS2_FAILURE); 
        return NULL;
    }
    
    builder_impl->element_level++;
    
    if (!(builder_impl->lastnode))
    {
        om_ele = axis2_om_element_create (env , NULL, temp_localname, NULL, &element_node);
        if(!om_ele)
            return NULL;
            
        builder_impl->root_node = element_node;
                
        axis2_om_node_set_builder(element_node, env, om_stax_builder);        
   
        if(builder_impl->document)        
        {
            axis2_om_node_set_document(element_node, env, builder_impl->document);
            AXIS2_OM_DOCUMENT_SET_ROOT_ELEMENT(builder_impl->document, env, element_node);
        }
    }
    else if(AXIS2_OM_NODE_GET_BUILD_STATUS(builder_impl->lastnode, env))
    {   
        axis2_om_element_t *om_ele = NULL;
        
        om_ele = axis2_om_element_create (env, 
                    AXIS2_OM_NODE_GET_PARENT(builder_impl->lastnode, env),
                    temp_localname, NULL, &element_node);
        if(!om_ele)
            return NULL;
            
        if(element_node)
        {
            axis2_om_node_set_next_sibling(builder_impl->lastnode, env, element_node);                                 
            axis2_om_node_set_previous_sibling(element_node , env, builder_impl->lastnode);
            axis2_om_node_set_document(element_node, env, builder_impl->document); 
            axis2_om_node_set_builder(element_node, env, om_stax_builder);
        }            
    }
    else
    {
        axis2_om_element_t *om_ele = NULL;
        om_ele = axis2_om_element_create ( env, builder_impl->lastnode,
                                 temp_localname, NULL, &element_node);
        if(element_node)
        {
            axis2_om_node_set_first_child(builder_impl->lastnode , env, element_node);                     
            axis2_om_node_set_parent(element_node , env, builder_impl->lastnode);
            axis2_om_node_set_document(element_node, env, builder_impl->document); 
            axis2_om_node_set_builder(element_node, env, om_stax_builder);
        }            
    }
    if(temp_localname)
        AXIS2_XML_READER_XML_FREE(builder_impl->parser , env, temp_localname);

    /** order of processing namespaces first is important */
    axis2_om_stax_builder_process_namespaces ( om_stax_builder, env, element_node, 0);
    axis2_om_stax_builder_process_attributes ( om_stax_builder, env, element_node);

    builder_impl->lastnode = element_node;
	
    return element_node;
}


axis2_om_node_t *
axis2_om_stax_builder_create_om_comment (axis2_om_stax_builder_t *builder,
                                         axis2_env_t **env)
{
    axis2_om_node_t *comment_node = NULL;
    axis2_om_stax_builder_impl_t *builder_impl = NULL;
    axis2_char_t *comment_value = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    comment_value  = AXIS2_XML_READER_GET_VALUE(builder_impl->parser, env);
    if (!comment_value)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL, AXIS2_FAILURE);
        return NULL;        
    }
    
    if (!(builder_impl->lastnode))
    {
        /* do nothing */ 
        AXIS2_XML_READER_XML_FREE(builder_impl->parser , env, comment_value);
        return NULL;   
    }
    else if (AXIS2_OM_NODE_GET_BUILD_STATUS(builder_impl->lastnode, env))
    {
        axis2_om_comment_create (env, 
                AXIS2_OM_NODE_GET_PARENT(builder_impl->lastnode, env),
                comment_value , &comment_node);
       
       axis2_om_node_set_next_sibling(builder_impl->lastnode, env, comment_node);                                 
        axis2_om_node_set_previous_sibling(comment_node , env, builder_impl->lastnode);
    }
    else
    {
        axis2_om_comment_create( env, builder_impl->lastnode,
                                 comment_value ,&comment_node);
        
        axis2_om_node_set_first_child(builder_impl->lastnode , env, comment_node);                     
        axis2_om_node_set_parent(comment_node , env, builder_impl->lastnode);
    }
    
    AXIS2_XML_READER_XML_FREE(builder_impl->parser , env, comment_value);

    builder_impl->lastnode = comment_node;

    return comment_node;
    
}


axis2_om_node_t * 
axis2_om_stax_builder_create_om_doctype (axis2_om_stax_builder_t * builder,
                                         axis2_env_t **env)
{
    axis2_om_stax_builder_impl_t *builder_impl;
    axis2_om_node_t *doctype_node = NULL;
    axis2_char_t *doc_value = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    doc_value = AXIS2_XML_READER_GET_DTD(builder_impl->parser, env);
    if(!doc_value)
        return NULL;
    if(!(builder_impl->lastnode))
    {
        axis2_om_doctype_create(env, NULL, doc_value, &doctype_node);
        if(builder_impl->document)
        {            
            AXIS2_OM_DOCUMENT_SET_ROOT_ELEMENT(builder_impl->document, 
                                        env, doctype_node);
        }            
    }
    builder_impl->lastnode = doctype_node;
    AXIS2_XML_READER_XML_FREE(builder_impl->parser, env, doc_value);
    return doctype_node;
}

 
axis2_om_node_t * 
axis2_om_stax_builder_create_om_processing_instruction (axis2_om_stax_builder_t *builder,
                                                        axis2_env_t **env)
{
    axis2_om_node_t *pi_node = NULL;
    axis2_om_stax_builder_impl_t *builder_impl = NULL;
    axis2_char_t *target = NULL;
    axis2_char_t *value  = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    target = AXIS2_XML_READER_GET_PI_TARGET (builder_impl->parser, env);
    value  = AXIS2_XML_READER_GET_PI_DATA(builder_impl->parser, env);
    if (!target)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_XML_READER_ELEMENT_NULL, AXIS2_FAILURE);
        return NULL;        
    }
    if (!(builder_impl->lastnode))
    {
        /* do nothing */
        AXIS2_XML_READER_XML_FREE(builder_impl->parser , env, target);
        AXIS2_XML_READER_XML_FREE(builder_impl->parser , env, value);
        return NULL;
    }
    else if (AXIS2_OM_NODE_GET_BUILD_STATUS(builder_impl->lastnode, env) ||
     (AXIS2_OM_NODE_GET_NODE_TYPE(builder_impl->lastnode, env) == AXIS2_OM_TEXT))
    {
        axis2_om_processing_instruction_create(env,
                        AXIS2_OM_NODE_GET_PARENT(builder_impl->lastnode, env),
                        target, value, &pi_node);
                 
        axis2_om_node_set_next_sibling(builder_impl->lastnode, env, pi_node);                                 
        axis2_om_node_set_previous_sibling(pi_node , env, builder_impl->lastnode);
    }
    else
    {
        axis2_om_processing_instruction_create( env, builder_impl->lastnode,
                                 target, value, &pi_node);
        axis2_om_node_set_first_child(builder_impl->lastnode , env, pi_node);                     
        axis2_om_node_set_parent(pi_node , env, builder_impl->lastnode);
    }
    if(target)
        AXIS2_XML_READER_XML_FREE(builder_impl->parser , env, target);
    if(value)
        AXIS2_XML_READER_XML_FREE(builder_impl->parser , env, value);
    
    builder_impl->lastnode = pi_node;
    return pi_node;
}

axis2_status_t AXIS2_CALL
axis2_om_stax_builder_end_element (axis2_om_stax_builder_t *om_stax_builder,
                                   axis2_env_t **env)
{
    axis2_om_node_t *parent = NULL;
    axis2_om_stax_builder_impl_t *builder = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE );
    builder = AXIS2_INTF_TO_IMPL(om_stax_builder);

    builder->element_level--;

    if (builder->lastnode)
    {
        if (AXIS2_OM_NODE_GET_BUILD_STATUS((builder->lastnode), env))
        {
            parent = AXIS2_OM_NODE_GET_PARENT((builder->lastnode), env);
            if (parent)
            {
                axis2_om_node_set_build_status(parent, env, AXIS2_TRUE);
                builder->lastnode= parent;
            }
        }
        else
        {
            axis2_om_node_set_build_status((builder->lastnode), env, AXIS2_TRUE);
        }
    }
    if(builder->root_node)
    {
        if(AXIS2_OM_NODE_GET_BUILD_STATUS(builder->root_node , env))
        {
            builder->done = AXIS2_TRUE;
        }   
    }        
    return AXIS2_SUCCESS;
}


axis2_om_node_t * AXIS2_CALL
axis2_om_stax_builder_next (axis2_om_stax_builder_t *om_stax_builder,
                            axis2_env_t **env)
{
    int token = 0;
    axis2_om_stax_builder_impl_t *builder_impl = NULL;
    axis2_om_node_t *node = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    builder_impl = AXIS2_INTF_TO_IMPL(om_stax_builder);
    if(!builder_impl->parser)
        return NULL;
    do
    {
        if (builder_impl->done)
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL, AXIS2_FAILURE);
            return NULL;
        }

        token = AXIS2_XML_READER_NEXT (builder_impl->parser, env);
        
        if(token == -1)
            return NULL;

        builder_impl->current_event = token;
                
        if (!(builder_impl->cache))
        {
            return NULL;
        }

        switch (token)
        {
        case AXIS2_XML_READER_START_DOCUMENT:
            /*Do nothing */
            break;
        
        case AXIS2_XML_READER_START_ELEMENT:
           node = axis2_om_stax_builder_create_om_element (
                        om_stax_builder, env); 
            break;
        
        case AXIS2_XML_READER_EMPTY_ELEMENT:
            node = axis2_om_stax_builder_create_om_element (
                        om_stax_builder, env);
        
        case AXIS2_XML_READER_END_ELEMENT:
            axis2_om_stax_builder_end_element (om_stax_builder, env);
            break;
        
        
        case AXIS2_XML_READER_SPACE:
            /* Do nothing */
            break;
        
        case AXIS2_XML_READER_CHARACTER:
            node = axis2_om_stax_builder_create_om_text(om_stax_builder, env);
            break;
                
        case AXIS2_XML_READER_ENTITY_REFERENCE:
            break;
        
        case AXIS2_XML_READER_COMMENT:
            
             node = axis2_om_stax_builder_create_om_comment(om_stax_builder, env);
                axis2_om_stax_builder_end_element (om_stax_builder, env);
        
            break;
        
        case AXIS2_XML_READER_PROCESSING_INSTRUCTION:
            
            node = axis2_om_stax_builder_create_om_processing_instruction(
                                        om_stax_builder , env );
            axis2_om_stax_builder_end_element (om_stax_builder, env);
          
            break;
        
        case AXIS2_XML_READER_CDATA:
            break;
        
        case AXIS2_XML_READER_DOCUMENT_TYPE:
        /* node = axis2_om_stax_builder_create_om_doctype(om_stax_builder, env);
        */          
        break;
        
        default:
            break;
        }
    }
    while (!node);
    return node;
}


axis2_status_t 
AXIS2_CALL axis2_om_stax_builder_free(axis2_om_stax_builder_t *builder,
                                      axis2_env_t **env)
{
    axis2_om_stax_builder_impl_t *builder_impl =  NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!builder)
        return AXIS2_FAILURE;
    
    builder_impl = AXIS2_INTF_TO_IMPL(builder);

    if(builder_impl->parser)
    {
        AXIS2_XML_READER_FREE(builder_impl->parser, env);
        builder_impl->parser = NULL;
    }
    if(builder_impl->document)
    {
        AXIS2_OM_DOCUMENT_FREE(builder_impl->document, env);
        builder_impl->document = NULL;
    }
    else
    {
        if(NULL != builder_impl->root_node )
        {
            AXIS2_OM_NODE_FREE_TREE(builder_impl->root_node, env);
            builder_impl->root_node = NULL;
        }
    }
	if(builder->ops)
	{
		AXIS2_FREE ((*env)->allocator, builder->ops);
        builder->ops = NULL;
	}	
	AXIS2_FREE ((*env)->allocator, AXIS2_INTF_TO_IMPL(builder));
	return AXIS2_SUCCESS;	
}


axis2_om_document_t* AXIS2_CALL
axis2_om_stax_builder_get_document (axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env,NULL);
    return AXIS2_INTF_TO_IMPL(builder)->document;
}
/**
    This is an internal function
*/
axis2_status_t  AXIS2_CALL
axis2_om_stax_builder_set_document(axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env,
                                    axis2_om_document_t *document)
{
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, builder, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, document , AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(builder)->document = document ;
    return AXIS2_SUCCESS;
}
/**
    This is an internal function 
*/

AXIS2_DECLARE(int)
axis2_om_stax_builder_get_current_event(axis2_om_stax_builder_t *builder,
                                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, -1);
    AXIS2_PARAM_CHECK((*env)->error, builder, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(builder)->current_event;
} 
/**
 This is an internal function 
*/
AXIS2_DECLARE(axis2_om_node_t*)
axis2_om_stax_builder_get_lastnode(axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, builder, NULL);
    return AXIS2_INTF_TO_IMPL(builder)->lastnode;   
}
/**
This is an internal function 
*/
axis2_bool_t AXIS2_CALL
axis2_om_stax_builder_is_complete(axis2_om_stax_builder_t *builder,
                                   axis2_env_t **env)
{
    axis2_om_stax_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return builder_impl->done;    
}                                                                                                              
/**
This is an internal function to be used by soap builder only
*/
AXIS2_DECLARE(axis2_status_t)
axis2_om_stax_builder_set_lastnode(axis2_om_stax_builder_t *builder,
                                    axis2_env_t **env,
                                    axis2_om_node_t *om_node)
{
    axis2_om_stax_builder_impl_t *builder_impl = NULL;
    AXIS2_PARAM_CHECK((*env)->error, om_node, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, builder, AXIS2_FAILURE);
    
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->lastnode = om_node;
    return AXIS2_SUCCESS;    
}                                    
/**
internal function for soap builder only
*/
AXIS2_DECLARE(int)
axis2_om_stax_builder_get_element_level(axis2_om_stax_builder_t* builder,
                                        axis2_env_t **env)
{
    axis2_om_stax_builder_impl_t *builder_impl = NULL;
    AXIS2_PARAM_CHECK((*env)->error, builder, -1);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return builder_impl->element_level;    
}                                         
/**
internal function for soap builder only
*/
AXIS2_DECLARE(axis2_status_t)
axis2_om_stax_builder_set_element_level(axis2_om_stax_builder_t* builder,
                                        axis2_env_t **env,
                                        int element_level)
{
    axis2_om_stax_builder_impl_t *builder_impl = NULL;
    AXIS2_PARAM_CHECK((*env)->error, builder, AXIS2_FAILURE);
    
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->element_level = element_level;    
    return AXIS2_SUCCESS;
} 

int AXIS2_CALL 
axis2_om_stax_builder_next_with_token(axis2_om_stax_builder_t *builder,
                                          axis2_env_t **env)
{

    int token = 0;
    void *val = NULL;
    
    axis2_om_stax_builder_impl_t *builder_impl = NULL;
    
    if(!builder) return -1;
    
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    if (builder_impl->done)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL, AXIS2_FAILURE);
        return -1;
    }
    
    if(!builder_impl->parser) return -1;
    
    token = AXIS2_XML_READER_NEXT (builder_impl->parser, env);
    
    if(token == -1)
    {
        builder_impl->done = AXIS2_TRUE;
        return -1;
    }
     
    if (!(builder_impl->cache))
    {
        return -1;
    }
    switch (token)
    {
        case AXIS2_XML_READER_START_DOCUMENT:
        /*Do nothing */
        break;
        
        case AXIS2_XML_READER_START_ELEMENT:
            val = axis2_om_stax_builder_create_om_element (
                        builder, env); 
            if(!val)    
                return -1;
                
            break;
        
        case AXIS2_XML_READER_EMPTY_ELEMENT:
             val = axis2_om_stax_builder_create_om_element (
                        builder, env);
             if(!val)
                return -1;
                
        case AXIS2_XML_READER_END_ELEMENT:
            axis2_om_stax_builder_end_element (builder, env);
            break;
        
        
        case AXIS2_XML_READER_SPACE:
            /* Do nothing */
            break;
        
        case AXIS2_XML_READER_CHARACTER:
            val = axis2_om_stax_builder_create_om_text(builder, env);
            if(!val)
                return -1;
            break;
                
        case AXIS2_XML_READER_ENTITY_REFERENCE:
            break;
        
        case AXIS2_XML_READER_COMMENT:
            val = axis2_om_stax_builder_create_om_comment(builder, env);
            if(!val)
                return -1;
            axis2_om_stax_builder_end_element (builder, env);
            break;
        
        case AXIS2_XML_READER_PROCESSING_INSTRUCTION:
            val = axis2_om_stax_builder_create_om_processing_instruction(
                                        builder , env );
            if(!val)
                return -1;
            axis2_om_stax_builder_end_element (builder, env);
            break;
        
        case AXIS2_XML_READER_CDATA:
            break;
        
        case AXIS2_XML_READER_DOCUMENT_TYPE:
        break;
        
        default:
            break;
    }
   return token;
}
