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
 
 #ifndef AXIS2_WSDLPARSER_WRAPPER_H
 #define AXIS2_WSDLPARSER_WRAPPER_H
 
 /**
 * @file axis2_wsdl4c_parser.h
 * @brief defines WSDLPARSER_WRAPPER constants
 */
#include <axis2_array_list.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2 wsdl4c_parser constants
 * @ingroup axis2_wsdl4c_parser 
 * @{
 */

   /**
   * The default constructor for WsdlParser
   * @param input stream for the wsdl file
   * @param output stream for any error outputs
   * @param schemaPath path to directory containing XML schema files
   *
   */
    void *
    axis2_wsdl4c_parser_create_wsdl_parser(char* wsdl_file);

    void *
    axis2_wsdl4c_parser_destroy_parser(void *parser);

   /**
    * set_schema_path
    * Set path to location of XML schema files, used by parser.
    */
    void
    axis2_wsdl4c_parser_set_schema_path(void *parser, char *schema_path);
   
    /**
     * get_next_element processes the next Wsdl ELement in the stream and
     * returns the type of the root Wsdl element parsed.
     * @return  one of  START,DEFINITION,DOCUMENTATION,ANNOTATION,
     *            TYPES,MESSAGE,PORT_TYPE,BINDING,SERVICE,IMPORT
     */ 
    int
    axis2_wsdl4c_parser_get_next_element(void *parser); 

    /**
    * get_event_type
    * returns the type Wsdl element parsed.
    * @return  one of the Wsdl event types
    */
    int
    axis2_wsdl4c_parser_get_event_type(void *parser); 
  
    /**
    * get_target_namespace
    * @return target namespace of the wsdl document
    */
    const char *
    axis2_wsdl4c_parser_get_namespace(void *parser);

    /**
    * get_name
    * @return name of the wsdl document
    */
    const char *
    axis2_wsdl4c_parser_get_name(void *parser);

    /**
    * getDocumentation Returns the documentation string for the WSDL file
    *                  This method must be called after <code>getEventType</code> returns DOCUMENT
    * @return documentation std::string .
    */
    const void *
    axis2_wsdl4c_parser_get_documentation(void *parser);

    /**
    * get_binding .This method must be called after
    *  <code>get_event_type</code> returns BINDING
    * @return reference to the Binding element just parsed.
    */
    const void *
    axis2_wsdl4c_parser_get_binding(void *parser);

    /**
    * get_binding_q .
    * @param Qname refering to the binding element
    * @return reference to the Binding element
    */
    const void *
    axis2_wsdl4c_parser_get_binding_q(void *parser, void *q); 

    /**
    * get_bindings
    * @return iterator to a list of binding elements
    */
    axis2_array_list_t *
    axis2_wsdl4c_parser_get_bindings(void *parser);

    /**
    * get_service .This method must be called after
    *  <code>getEventType</code> returns SERVICE
    * @return reference to the Binding element just parsed.
    */
    /*const void *
    axis2_wsdl4c_parser_get_svc(void *parser) 
    {
        WsdlParser *wp = (WsdlParser *) parser;
        return wp->getService();
    }*/

    /**
    * get_port_type .This method must be called after
    *  <code>get_event_type</code> returns PORT_TYPE
    * @return reference to the port type element just parsed.
    */
    const void *
    axis2_wsdl4c_parser_get_port_type(void *parser);

    /**
    * get_port_type_q
    * @param Qname refering to the port type element
    * @return reference to the port type element
    */
    const void *
    axis2_wsdl4c_parser_get_port_type_q(void *parser, void *q);

    /**
    * get_port_types
    * @return iterators to a container of port types
    */
    axis2_array_list_t *
    axis2_wsdl4c_parser_get_port_types(void *parser);

    /**
    * get_operations
    * @param Qname refering to the port type element
    * @return Returns the array list of operations
    */
    axis2_array_list_t *
    axis2_wsdl4c_parser_get_operations(void *parser, void *port_type_qname);

    /**
    * get_operation
    * @param Qname refering to the port type element
    * @param Qname refering to the operation element
    * @return reference to the operation element
    */
    const void *
    axis2_wsdl4c_parser_get_operation(void *parser, void *port_type_qname, void *q);

    /**
    * get_message .
    * This method must be called after
    *  <code>get_event_type</code> returns MESSAGE
    * @return reference to the message element just parsed.
    */
    const void *
    axis2_wsdl4c_parser_get_message(void *parser);

    /**
    * get_message_q_q .
    * @param Qname of the message element
    * @return reference to the message element
    */
    const void *
    axis2_wsdl4c_parser_get_message_a_q(void *parser, void *q);

    /**
    * get_messages
    * @return list of all messages  parsed.
    */
    void *
    axis2_wsdl4c_parser_get_messages(void *parser);

    /**
    * get_num_schemas
    * @return number of schemas in the wsdl document
    */
    int
    axis2_wsdl4c_parser_get_num_schemas(void *parser);

    /**
    * get_schema_parser
    * @param target namespace of the schema
    * @return pointer to the schema parser for the namespace
    */
    const void *
    axis2_wsdl4c_parser_get_schema_parser(void *parser, char *target_namespace);

    /**
    * to query the status of wsdl parsing
    */
    bool
    axis2_wsdl4c_parser_status(void *parser);

    const char *
    axis2_wsdl4c_parser_wsdl_path(void *parser);

    /**
    * add_extensibility_handler 
    * Registers an extensibility handler
    * for Wsdl extensibility namespaces
    * @param pointer to the instance of WsdlExtension
    */
    void
    axis2_wsdl4c_parser_add_extensibility_handler(void *parser, void *ext);

    /**
    * get_extensibility_handler_q_ns returns theextensibility handler
    *                         for the namespaces
    * @param namespace
    */
    void *
    axis2_wsdl4c_parser_get_extensibility_handler_a_ns(void *parser, char *ns);

    /* 
    *return the extensibility handler for the given id.
    * WsdlParser only stores ids for the extensibilit elements.
    */
    void *
    axis2_wsdl4c_parser_get_extensibility_handler_a_extId(void *parser, int extId);

    /**
    * set_feature enables one of the features of the parser
    * @return   true or false 
    */
    /*bool
    axis2_wsdl4c_parser_set_feature(void *parser, int feature_id) 
    {
        WsdlParser *wp = (WsdlParser *) parser;
        return wp->setFeature(feature_id);
    }*/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDLPARSER_WRAPPER_H */
