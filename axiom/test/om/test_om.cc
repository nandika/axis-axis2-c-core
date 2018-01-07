
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

#include <gtest/gtest.h>

#include <axiom_stax_builder.h>
#include <axiom_document.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_text.h>
#include <axiom_data_source.h>
#include <axutil_stream.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axiom_xml_reader.h>
#include <stdio.h>
#include <axiom_xml_writer.h>
#include <axutil_env.h>


int AXIS2_CALL
read_input(
    char *buffer,
    int size,
    void *ctx)
{
    int len = 0;
    char *pos = NULL;
    len = fread(buffer, sizeof(char), size, (FILE*)ctx);
    if (buffer)
        pos = strstr(buffer, "---");
    if (pos)
    {
        len = pos - buffer;
        *pos = '\0';
    }
    return len;
}

/* FIXME 
 * These tests exercise code, but don't actually check that the output is
 * correct.  They didn't when they were in the old test format, either.
 */


class TestOM: public ::testing::Test
{

    protected:
        void SetUp()
        {

            m_allocator = axutil_allocator_init(NULL);
            m_axis_log = axutil_log_create(m_allocator, NULL, NULL);
            m_error = axutil_error_create(m_allocator);

            m_environment = axutil_env_create_with_error_log(m_allocator, m_error, m_axis_log);

        }

        void TearDown()
        {
            axutil_env_free(m_environment);
        }


        axutil_allocator_t *m_allocator = NULL;
        axutil_env_t *m_environment = NULL;
        axutil_error_t *m_error = NULL;
        axutil_log_t *m_axis_log = NULL;

};


TEST_F(TestOM, test_om_build) {

    const char *filename = "../resources/xml/om/test.xml";


    FILE *f = NULL;
    axiom_element_t *ele1 = NULL;
    axiom_element_t *ele2 = NULL;
    axiom_stax_builder_t *builder = NULL;
    axiom_text_t *text = NULL;
    axiom_document_t *document = NULL;
    axiom_node_t *node1 = NULL;
    axiom_node_t *node2 = NULL;
    axiom_node_t *node3 = NULL;
    axiom_output_t *om_output = NULL;
    axiom_namespace_t *ns = NULL;
    axiom_xml_reader_t *reader = NULL;
    axiom_xml_writer_t *writer = NULL;

    char *buffer = NULL;
    axutil_hash_t* hash = NULL;

    f = fopen(filename, "r");
    ASSERT_NE(f, nullptr);

    /** create pull parser */
    reader =
        axiom_xml_reader_create_for_io(m_environment, read_input, NULL, f,
                                       NULL);
    ASSERT_NE(reader, nullptr);

     /** create axiom_stax_builder by parsing pull_parser struct */
    builder = axiom_stax_builder_create(m_environment, reader);
    ASSERT_NE(builder, nullptr);

    /**
        create an om document
        document is the container of om model created using builder
    */
    document = axiom_stax_builder_get_document(builder, m_environment);
    ASSERT_NE(document, nullptr);

    node1 = axiom_document_get_root_element(document, m_environment);
    ASSERT_NE(node1, nullptr);
    if (node1)
    {
        /** print root node information */
        ele1 = (axiom_element_t*)axiom_node_get_data_element(node1, m_environment);
        if (ele1)
        {
            printf("root localname %s\n",
                   axiom_element_get_localname(ele1, m_environment));

            hash = axiom_element_get_all_attributes(ele1,m_environment);
            if(hash)
            {
                axutil_hash_index_t *hi;
                const void *key= NULL;
                void *val = NULL;
                for (hi = axutil_hash_first(hash,m_environment); hi; hi = axutil_hash_next(m_environment, hi))
                {
                    axutil_hash_this(hi, &key, NULL,&val);
                    if(val)
                    {
                        printf(" Attribute name: %s",
                            axiom_attribute_get_localname((axiom_attribute_t *)val,m_environment));
                        printf("   value: %s\n",
                            axiom_attribute_get_value((axiom_attribute_t *)val,m_environment));
                    }
                }
            }
        }

        ns = axiom_element_get_namespace(ele1, m_environment, node1);

        if (ns)
        {
            printf("root ns prefix %s\n",
                   axiom_namespace_get_prefix(ns, m_environment));
            printf("root ns uri %s\n",
                   axiom_namespace_get_uri(ns, m_environment));
                        printf("=============================================");

        }
        else
            printf("=============================================");

    }

    /** build the document continuously untill all the xml file is built in to a om model */

    node2 = axiom_document_build_next(document, m_environment);
    do
    {

        if (!node2)
            break;

        switch (axiom_node_get_node_type(node2, m_environment))
        {
        case AXIOM_ELEMENT:
            ele2 =
                (axiom_element_t *) axiom_node_get_data_element(node2,
                                                                m_environment);
                    printf("=============================================");
            if (ele2 && axiom_element_get_localname(ele2, m_environment))
            {
                printf("\n localname %s\n",
                           axiom_element_get_localname(ele2, m_environment));
                hash = axiom_element_get_all_attributes(ele2,m_environment);
                if(hash)
                {
                    axutil_hash_index_t *hi;
                    const void *key= NULL;
                    void *val = NULL;
                    for (hi = axutil_hash_first(hash,m_environment); hi; hi = axutil_hash_next(m_environment, hi))
                    {
                        axutil_hash_this(hi, &key, NULL,&val);
                        if(val)
                        {
                            printf(" Attribute name: %s",
                                axiom_attribute_get_localname((axiom_attribute_t *)val,m_environment));
                            printf("   value: %s\n",
                                axiom_attribute_get_value((axiom_attribute_t *)val,m_environment));
                        }
                    }
                }

            }



            if (!node3)
                node3 = node2;

            break;
        case AXIOM_TEXT:

            text =
                (axiom_text_t *) axiom_node_get_data_element(node2,
                                                             m_environment);
            if (text && axiom_text_get_value(text, m_environment))
                printf("\n text value  %s \n",
                       axiom_text_get_value(text, m_environment));
            break;

        default:
            break;
        }

        node2 = axiom_document_build_next(document, m_environment);
    }
    while (node2);
    printf("END: pull document\n");

    printf("Serialize pulled document\n");

    writer =
        axiom_xml_writer_create_for_memory(m_environment, NULL, AXIS2_TRUE, 0,
                                           AXIS2_XML_PARSER_TYPE_BUFFER);
    om_output = axiom_output_create(m_environment, writer);

    axiom_node_serialize_sub_tree(node3, m_environment, om_output);

    buffer = (axis2_char_t *) axiom_xml_writer_get_xml(writer, m_environment);

    if (buffer)
        printf("Sub Tree = %s\n", buffer);

    axiom_output_free(om_output, m_environment);

    //axiom_stax_builder_free(builder, m_environment);

/*     if (buffer) */

/*         AXIS2_FREE(m_environment->allocator, buffer); */
    printf("\nend test_om_build\n");
    fclose(f);
    return;
}


TEST_F(TestOM, test_om_serialize) {

    /*
       <book xmlns:axiomc="http://ws.apache.org/axis2/c/om" xmlns:isbn="urn:ISBN:0-395-74341-6">
       <title>Axis2/C OM HOWTO</title>
       <isbn:number>1748491379</isbn:number>
       <author title="Mr" name="Axitoc Oman" />
       <notes>
       <p xmlns="urn:w3-org-ns:HTML">
       This is <i>vey good</i> book on OM!
       </p>
       </notes>
       </book>
     */
    int status;
    axiom_element_t *ele1 = NULL,
        *ele2 = NULL,
        *ele3 = NULL,
        *ele4 = NULL;
    axiom_node_t *node1 = NULL,
        *node2 = NULL,
        *node3 = NULL,
        *node4 = NULL,
        *node5 = NULL,
        *node6 = NULL;
    axiom_data_source_t *data_source = NULL;
    axutil_stream_t *stream = NULL;
    axiom_attribute_t *attr1 = NULL,
        *attr2 = NULL;
    axiom_namespace_t *ns1 = NULL,
        *ns2 = NULL;
    axiom_text_t *text1 = NULL;
    axiom_output_t *om_output = NULL;
    axiom_xml_writer_t *writer = NULL;
    axis2_char_t *output_buffer = NULL;

    printf("\nstart test_om_serialize\n");

    ns1 =
        axiom_namespace_create(m_environment,
                               "http://ws.apache.org/axis2/c/om", "axiom");
    ns2 = axiom_namespace_create(m_environment, "urn:ISBN:0-395-74341-6", "isbn");
    ele1 = axiom_element_create(m_environment, NULL, "book", ns1, &node1);
    axiom_element_declare_namespace(ele1, m_environment, node1, ns2);

    ele2 = axiom_element_create(m_environment, node1, "title", ns1, &node2);
    attr1 = axiom_attribute_create(m_environment, "title22", NULL, NULL);

    axiom_element_add_attribute(ele2, m_environment, attr1, node2);

    text1 = axiom_text_create(m_environment, node2, "Axis2/C OM HOWTO", &node3);

    ele3 = axiom_element_create(m_environment, node1, "number", ns2, &node4);

    text1 = axiom_text_create(m_environment, node4, "1748491379", &node5);

    ele4 = axiom_element_create(m_environment, node1, "author", ns1, &node6);

    attr1 = axiom_attribute_create(m_environment, "title", "Mr", ns1);

    axiom_element_add_attribute(ele4, m_environment, attr1, node6);

    attr2 = axiom_attribute_create(m_environment, "name", "Axitoc Oman", ns1);

    axiom_element_add_attribute(ele4, m_environment, attr2, node6);

    data_source = axiom_data_source_create(m_environment, node1, &node6);
    stream = axiom_data_source_get_stream(data_source, m_environment);
    if (stream)
    {
        axutil_stream_write(stream, m_environment,
                            "<this xmlns:axiom=\"http://ws.apache.org/axis2/c/om\">is a test</this>",
                            axutil_strlen
                            ("<this xmlns:axiom=\"http://ws.apache.org/axis2/c/om\">is a test</this>"));
    }

    /* serializing stuff */
    writer =
        axiom_xml_writer_create_for_memory(m_environment, NULL, AXIS2_TRUE, 0,
                                           AXIS2_XML_PARSER_TYPE_BUFFER);
    om_output = axiom_output_create(m_environment, writer);

    printf("Serialize built document\n");
    status = axiom_node_serialize(node1, m_environment, om_output);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    printf("\naxiom_node_serialize success\n");
    /* end serializing stuff */

    axiom_node_free_tree(node1, m_environment);
    output_buffer =
        (axis2_char_t *) axiom_xml_writer_get_xml(writer, m_environment);

    axiom_output_free(om_output, m_environment);
    /*    if (output_buffer) */
    /*     { */
    /*         printf("%s", output_buffer); */
    /*         AXIS2_FREE(m_environment->allocator, output_buffer); */
    /*     } */

    printf("\nend test_om_serialize\n");

    return;
}


TEST_F(TestOM, test_om_buffer) {

    /*AXIS2C-1628 buffer modified by axiom_node_create_from_buffer */
    axis2_char_t * output;

    char * xml = strdup("<foo>T1 &amp; T2</foo>");
    char * xml_unaltered= strdup("<foo>T1 &amp; T2</foo>");

    printf("\nstart test_om_bufer\n");

    axiom_node_t * om_node = axiom_node_create_from_buffer(m_environment, xml);

    output = axiom_node_to_string(om_node,m_environment);

    ASSERT_EQ(0, axutil_strcmp(xml,xml_unaltered));

    ASSERT_EQ(0, axutil_strcmp(output,xml_unaltered));

    axiom_node_free_tree(om_node,m_environment);
    AXIS2_FREE(m_environment->allocator, output);
    AXIS2_FREE(m_environment->allocator, xml);
    AXIS2_FREE(m_environment->allocator, xml_unaltered);

    printf("\nend test_om_bufer\n");

    return;
}
