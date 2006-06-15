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

#include <axiom_stax_builder.h>
#include <axiom_document.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_text.h>
#include <axis2_stream.h>
#include <axis2_log_default.h>
#include <axis2_error_default.h>
#include <axiom_reader.h>
#include <stdio.h>
#include <axiom_writer.h>
#include <axiom_soap_builder.h>
#include <axiom_soap_const.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_body.h>
#include <axiom_soap_header.h>
#include <axiom_soap_message.h>
#include <axiom_soap_header_block.h>
#include <axiom_soap_fault.h>
#include <axiom_soap_fault_code.h>
#include <axiom_soap_fault_role.h>
#include <axiom_soap_fault_value.h>
#include <axiom_soap_fault_detail.h>
#include <axiom_soap_fault_role.h>

FILE *f = NULL;
int read_soap(char *buffer,int size,void* ctx)
{
   int len = 0;
   char* pos = NULL;
   len = fread(buffer, sizeof(char),size,f);
   if (buffer)
       pos = strstr(buffer, "---");
   if (pos)
   {
        len = pos - buffer;
        *pos = '\0';
   }
   printf("buffer = %s\n", buffer);
   return len;
}

/*int read_soap(char *buffer, int size, void *ctx)
{
    return fread(buffer, sizeof(char), size, f); 
}*/

int close_soap(void *ctx)
{
    fclose(f);
    return AXIS2_TRUE;
}

int printnode(axiom_node_t *om_node, const axis2_env_t *env)
{
    axiom_element_t *om_ele = NULL;
    axis2_char_t *localname = NULL;
    axiom_namespace_t *om_ns =  NULL;
    axis2_char_t *uri = NULL;
    axis2_char_t *prefix = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!om_node)
        return AXIS2_FAILURE;
        
    if(AXIOM_NODE_GET_NODE_TYPE(om_node, env) == AXIOM_ELEMENT)
    {
        om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(om_node, env);
        if(!om_ele)
            return AXIS2_FAILURE;
            
        localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
        if(NULL != localname)
            printf("\n %s \n", localname);
        om_ns = AXIOM_ELEMENT_GET_NAMESPACE(om_ele, env, om_node);
    
        if(om_ns)
        {
            prefix = AXIOM_NAMESPACE_GET_PREFIX(om_ns, env);
            uri    = AXIOM_NAMESPACE_GET_URI(om_ns, env);
            
            printf("\n uri %s \n prefix %s \n",uri, prefix);
        }
    }
    return 0;
}

int build_soap(const axis2_env_t *env, const char *filename, const axis2_char_t *uri)
{
    axiom_stax_builder_t *om_builder = NULL;
    
    axiom_reader_t *xml_reader = NULL;
    
    axiom_soap_builder_t *soap_builder = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;
    
    axiom_node_t *om_node = NULL;
    axis2_char_t *buffer = NULL;
    
    axiom_writer_t *xml_writer = NULL;
    axiom_output_t *om_output = NULL;
    
    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axiom_children_qname_iterator_t *children_iter = NULL;
    
    int status = AXIS2_SUCCESS;    
    
    if(!filename)
        return -1;
    f = fopen(filename, "r");
    if(!f)
        return -1;
    printf(" \n\n _________ Test SOAP BUILD ___________ \n\n ");
    
    xml_reader = axiom_reader_create_for_io(env, read_soap,close_soap ,NULL, NULL);
    if(!xml_reader)
    {
        printf("%s \n", AXIS2_ERROR_GET_MESSAGE(env->error));
        return AXIS2_FAILURE;
    }

   
    om_builder = axiom_stax_builder_create(env, xml_reader);
    if(!om_builder)
    {
        AXIOM_READER_FREE(xml_reader, env);
        printf("%s \n", AXIS2_ERROR_GET_MESSAGE(env->error));
        return AXIS2_FAILURE;
    }
    
    soap_builder = axiom_soap_builder_create(env, om_builder, uri);
    if(!soap_builder)
    {
        printf("%s \n", AXIS2_ERROR_GET_MESSAGE(env->error));
        return AXIS2_FAILURE;
    }
    soap_envelope = AXIOM_SOAP_BUILDER_GET_SOAP_ENVELOPE(soap_builder, env);
    if(!soap_envelope)
    {
        AXIOM_SOAP_BUILDER_FREE(soap_builder, env);
        printf("%s \n", AXIS2_ERROR_GET_MESSAGE(env->error));
        return AXIS2_FAILURE;
    }

    
    om_node = AXIOM_SOAP_ENVELOPE_GET_BASE_NODE(soap_envelope, env);
    
    if(NULL != om_node)
        printnode(om_node, env);
    
    soap_header = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
    if(NULL != soap_header)
    {
        om_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(soap_header, env);
        if(NULL != om_node)
            printnode(om_node, env);
    
        children_iter = AXIOM_SOAP_HEADER_EXAMINE_ALL_HEADER_BLOCKS(soap_header, env);
        if(NULL != children_iter)
        {
            while(AXIOM_CHILDREN_ITERATOR_HAS_NEXT(children_iter, env))
            {   
                om_node = AXIOM_CHILDREN_ITERATOR_NEXT(children_iter, env);
                if(NULL != om_node)
                    printnode(om_node, env);
            }
        }
    }
    
    soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    if (NULL != soap_body)
    {
        om_node = AXIOM_SOAP_BODY_GET_BASE_NODE(soap_body, env);
        if(NULL != om_node)
            printnode(om_node, env);
        else
            printf("\n\n soap body base node null \n\n");
    }
    else
    {
        printf("%s \n", AXIS2_ERROR_GET_MESSAGE(env->error));
        printf ("\n\n ERROR soap_body NULL.\n\n");
        return AXIS2_FAILURE;
    }

   
    if(AXIOM_SOAP_BODY_HAS_FAULT(soap_body, env))
    {
            printf("\n\nsoap body  has a fault element\n\n\n ");
    }
    
    om_node = AXIOM_SOAP_BODY_GET_BASE_NODE(soap_body, env);
    if(om_node)
    {
        while(!(AXIOM_NODE_IS_COMPLETE(om_node, env)))
        {
            status = AXIOM_SOAP_BUILDER_NEXT(soap_builder, env);
            if(status == AXIS2_FAILURE)
            {      
                printf("failure %s" ,AXIS2_ERROR_GET_MESSAGE(env->error));
                return AXIS2_FAILURE;
            }               
        }
    }
    
    xml_writer = axiom_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE, 
                                          AXIS2_XML_PARSER_TYPE_BUFFER);
    if(!xml_writer)
    {
        AXIOM_SOAP_BUILDER_FREE(soap_builder, env);
        return AXIS2_FAILURE;
    }
    
    om_output = axiom_output_create( env, xml_writer);  
    if(!om_output)
    {
        AXIOM_SOAP_BUILDER_FREE(soap_builder, env);
        AXIOM_WRITER_FREE(xml_writer, env);
        return AXIS2_FAILURE;
    }

    
    AXIOM_SOAP_ENVELOPE_SERIALIZE(soap_envelope, env, om_output, AXIS2_FALSE);
    
    buffer = (axis2_char_t*)AXIOM_WRITER_GET_XML(xml_writer, env);         
    
    printf("\n\nThe serialized xml is >>>>>>>>>>>>>\n\n\n%s \n\n\n", buffer);
    
    if(NULL != buffer)
         AXIS2_FREE(env->allocator, buffer);
    
    AXIOM_SOAP_ENVELOPE_FREE(soap_envelope, env);
    
    AXIOM_OUTPUT_FREE(om_output, env);
    
    printf(" \n __________ END TEST SOAP BUILD ____________ \n");
    
    return AXIS2_SUCCESS;
}


int build_soap_programatically(const axis2_env_t *env)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axiom_soap_fault_t *soap_fault = NULL;
    axiom_soap_fault_code_t *fault_code = NULL;
    axiom_soap_header_block_t *hb1 = NULL;
    
    axiom_namespace_t *env_ns = NULL;
    axiom_namespace_t *test_ns = NULL;
    axiom_namespace_t *role_ns = NULL;
    
    axiom_writer_t *xml_writer = NULL;
    axiom_output_t *om_output = NULL;
    
    axis2_char_t *buffer = NULL;
    axiom_node_t *hb_node = NULL;
    axiom_element_t *hb_ele =  NULL;
    
    printf(" \n ____________ BUILD SOAP PROGRAMATICALLY _______________ \n");
    
    
    env_ns = axiom_namespace_create(env, "http://www.w3.org/2003/05/soap-envelope", "env");
    if(!env_ns)
        return AXIS2_FAILURE;
        
    soap_envelope = axiom_soap_envelope_create(env, env_ns);
    if(!soap_envelope)
        return AXIS2_FAILURE;
        
    soap_header = axiom_soap_header_create_with_parent(env, soap_envelope);
    if(!soap_header)
        return AXIS2_FAILURE;
    
    test_ns = axiom_namespace_create(env, "http://example.org/ts-tests", "test");
    
    role_ns = axiom_namespace_create(env, "http://www.w3.org/2003/05/soap-envelope/role/next","role");

   
    
    hb1 = axiom_soap_header_block_create_with_parent(env, "echoOk", role_ns , soap_header);
    
    hb_node = AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE(hb1, env);
    
    hb_ele = AXIOM_NODE_GET_DATA_ELEMENT(hb_node, env);
    
    AXIOM_ELEMENT_SET_NAMESPACE(hb_ele, env, test_ns, hb_node);
    
    soap_body = axiom_soap_body_create_with_parent(env, soap_envelope);
    

    soap_fault = axiom_soap_fault_create_with_parent(env, soap_body);
    
    fault_code = axiom_soap_fault_code_create_with_parent(env, soap_fault);
    
    xml_writer = axiom_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE, 
                                          AXIS2_XML_PARSER_TYPE_BUFFER);
    
    om_output = axiom_output_create( env, xml_writer);
    
    AXIOM_SOAP_ENVELOPE_SERIALIZE(soap_envelope, env, om_output, AXIS2_FALSE);
    
    buffer = (axis2_char_t*)AXIOM_WRITER_GET_XML(xml_writer, env);         
    
    printf("%s \n",  buffer); 

    AXIOM_SOAP_ENVELOPE_FREE(soap_envelope, env);
    
    AXIS2_FREE(env->allocator, buffer);
    
    buffer = NULL;
    
    AXIOM_OUTPUT_FREE(om_output, env);
    
    printf("\n __________ END BUILD SOAP PROGRAMATICALLY ____________\n");
    
    return AXIS2_SUCCESS;
}

int create_soap_fault(const axis2_env_t *env)
{
    axiom_soap_envelope_t *soap_envelope= NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_fault_t *soap_fault = NULL;
    axiom_writer_t *xml_writer = NULL;
    axiom_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;

    soap_envelope = 
            axiom_soap_envelope_create_default_soap_fault_envelope(env, 
                "Fault Code", "Fault Reason", AXIOM_SOAP12,
                NULL, NULL);
    soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    soap_fault = AXIOM_SOAP_BODY_GET_FAULT(soap_body, env);
    
    axiom_soap_fault_detail_create_with_parent(env, soap_fault);
    axiom_soap_fault_role_create_with_parent(env, soap_fault);
    xml_writer = axiom_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE, 
                                          AXIS2_XML_PARSER_TYPE_BUFFER);
    om_output = axiom_output_create( env, xml_writer);
    AXIOM_SOAP_ENVELOPE_SERIALIZE(soap_envelope, env, om_output, AXIS2_FALSE);
    buffer = (axis2_char_t*)AXIOM_WRITER_GET_XML(xml_writer, env);
        printf("%s \n",  buffer);
    AXIS2_FREE(env->allocator, buffer);
    AXIOM_SOAP_ENVELOPE_FREE(soap_envelope, env);
    AXIOM_OUTPUT_FREE(om_output, env);
return 0;
}

int test_soap_fault_value(const axis2_env_t *env)
{
   axiom_soap_envelope_t *soap_envelope = NULL;
   axiom_soap_body_t *soap_body = NULL;
   axiom_soap_fault_t *soap_fault = NULL;
   axiom_soap_fault_code_t *soap_code = NULL;
   axiom_soap_fault_value_t *value = NULL;
   axis2_char_t *value_text = NULL;

   printf("TEST SOAP FAULT VALUE\n");
   soap_envelope = axiom_soap_envelope_create_default_soap_fault_envelope(
         env, "env:Receiver", "Something went wrong!", AXIOM_SOAP12,
            NULL, NULL);
   soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
   soap_fault = AXIOM_SOAP_BODY_GET_FAULT(soap_body, env);
   soap_code = AXIOM_SOAP_FAULT_GET_CODE(soap_fault, env);
   value = AXIOM_SOAP_FAULT_CODE_GET_VALUE(soap_code, env);
   value_text = AXIOM_SOAP_FAULT_VALUE_GET_TEXT(value, env);   
   
   printf ("Actual = %s Expected = %s |", value_text, "env:Receiver");
   if (0 == strcmp(value_text, "env:Receiver"))
      printf("SUCCESS\n");
   else
      printf("FAILURE\n");

   AXIOM_SOAP_ENVELOPE_FREE(soap_envelope, env);
   return 0;
}   
int main(int argc, char *argv[])
{
    axis2_env_t *env = NULL;
    axis2_allocator_t *allocator = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log = NULL;
    const axis2_char_t *uri = AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    const char *filename = "./../../resources/xml/soap/test.xml";
    if(argc > 1)
        filename = argv[1];
    if(argc > 2)
    {
        if(AXIS2_STRCMP(argv[2],"0") == 0)
            uri = AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI;
        else if(AXIS2_STRCMP(argv[2],"1") == 0)
            uri = AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    allocator = axis2_allocator_init (NULL);
    log = axis2_log_create(allocator, NULL, "test_soap.log");
    log->level = AXIS2_LOG_LEVEL_DEBUG;
    error = axis2_error_create(allocator);
    env = axis2_env_create_with_error_log(allocator, error,  log);
    
    axis2_error_init(); 
    /*build_soap_programatically(env);   */
    build_soap(env, filename,uri); 
    /*create_soap_fault(env); 
   test_soap_fault_value(env); */
    axis2_env_free(env); 
    axis2_allocator_free(allocator);
    return 0;        
}
