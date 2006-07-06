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

#include <stdio.h>
#include <axiom.h>
#include <axiom_xml_reader.h>
#include <axis2_env.h>
#include <axis2_utils.h>
#include <platforms/axis2_platform_auto_sense.h>
#include <woden_wsdl10_reader.h>
#include <woden_wsdl10_desc.h>
#include <woden_interface.h>
#include <woden_binding.h>
#include <woden_element_decl.h>
#include <woden_svc.h>

#include <axiom.h>

static axiom_document_t* 
get_root_element_from_filename(
        const axis2_env_t *env, 
        axis2_char_t *filename);

int main(int argc, char *argv[])
{
    axis2_env_t *env = NULL;
    axiom_document_t *om_doc = NULL;
    axis2_char_t *doc_base_uri = NULL;
    axis2_char_t *axis2c_home = NULL;
    woden_wsdl10_reader_t *reader = NULL;
    void *desc = NULL;
    void *intface = NULL;
    axis2_array_list_t *intfaces = NULL;
    axis2_qname_t *intface_qname = NULL;
    axis2_char_t *filename = NULL;
    axis2_array_list_t *svc_list = NULL;
    axis2_array_list_t *binding_list = NULL;
    axis2_array_list_t *ed_list = NULL;
    void *ed = NULL;
    axis2_qname_t *ed_qname = NULL;
    
    if(argc > 1)
    {
        filename = argv[1];
        printf("filename:%s\n", filename);
    }
    else
    {
        printf("Give a WSDL 1.1 file name and continue\n");
        return 0;        
    }
    
    env = axis2_env_create_all("test.log", 1);
    
    om_doc = get_root_element_from_filename(env, filename);
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    doc_base_uri = AXIS2_STRACAT (axis2c_home, "/woden", env);
    reader = woden_wsdl10_reader_create(env);
    
    desc = (void *)WODEN_WSDL10_READER_READ_WSDL(reader, env, om_doc, doc_base_uri);
    intfaces = WODEN_WSDL10_DESC_GET_INTERFACES(desc, env);
    intface = AXIS2_ARRAY_LIST_GET(intfaces, env, 0);
    if (intface)
    {
        intface_qname = WODEN_INTERFACE_GET_QNAME(intface, env);
    }
    if (intface_qname) 
        printf("Interface qname is %s\n", AXIS2_QNAME_TO_STRING(intface_qname, env));
    svc_list = WODEN_WSDL10_DESC_ELEMENT_GET_SVC_ELEMENTS(desc, env);
    if (svc_list)
    {
        void *svc = NULL;
        svc = AXIS2_ARRAY_LIST_GET(svc_list, env, 0);
        if (svc)
        {
            axis2_qname_t *svc_qname = WODEN_SVC_GET_QNAME(svc, env);
            if (svc_qname)
            {
                printf("First service qname is %s\n", AXIS2_QNAME_TO_STRING(svc_qname, env));
            }
        }
    }
    binding_list = WODEN_WSDL10_DESC_ELEMENT_GET_BINDING_ELEMENTS(desc, env);
    if (binding_list)
    {
        void *binding = NULL;
        binding = AXIS2_ARRAY_LIST_GET(binding_list, env, 0);
        if (binding)
        {
            axis2_qname_t *binding_qname = WODEN_BINDING_GET_QNAME(binding, env);
            if (binding_qname)
            {
                printf("First binding qname is %s\n", AXIS2_QNAME_TO_STRING(binding_qname, env));
            }
        }
    }

    ed_list = WODEN_WSDL10_DESC_GET_ELEMENT_DECLS(desc, env);
    if (ed_list)
    {
        
        ed = AXIS2_ARRAY_LIST_GET(ed_list, env, 0);
        if (ed)
            ed_qname = WODEN_ELEMENT_DECL_GET_QNAME(ed, env);
    }
    ed = WODEN_WSDL10_DESC_GET_ELEMENT_DECL(desc, env, ed_qname);
    if (ed)
    {
        axis2_char_t *content_model = NULL;
        axis2_generic_obj_t *obj = NULL;
        
        axis2_qname_t *ed_qname = WODEN_ELEMENT_DECL_GET_QNAME(ed, env);
        if (ed_qname)
        {
            printf("Element declaration qname is %s\n", AXIS2_QNAME_TO_STRING(ed_qname, env));
        }
        content_model = WODEN_ELEMENT_DECL_GET_CONTENT_MODEL(ed, env);
        if (content_model)
        {
            printf("Content model is %s\n", content_model);
        }
        obj = WODEN_ELEMENT_DECL_GET_CONTENT(ed, env);
        if (obj)
        {
            void *value = NULL;

            value = AXIS2_GENERIC_OBJ_GET_VALUE(obj, env);
            printf("Content is:\n");
        }
    }
    return 0;
}

static axiom_document_t* 
get_root_element_from_filename(
        const axis2_env_t *env, 
        axis2_char_t *filename)
{
    axiom_xml_reader_t *reader = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axiom_document_t *doc   = NULL;

    reader = axiom_xml_reader_create_for_file(env, filename, NULL);
    om_builder = axiom_stax_builder_create(env, reader);
    doc = axiom_document_create(env, NULL, om_builder); 
    AXIOM_DOCUMENT_BUILD_ALL(doc, env);

    return doc;    
}

