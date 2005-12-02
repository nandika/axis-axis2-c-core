#include <axis2_om_stax_builder.h>
#include <axis2_om_document.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>
#include <axis2_stream_default.h>
#include <axis2_log_default.h>
#include <axis2_error_default.h>

#include <stdio.h>
#include <axis2_xml_writer.h>

/** 
   Define the environment related variables globaly so that they are available 
   for both functions 
*/

axis2_allocator_t *allocator = NULL;
axis2_env_t *environment = NULL;
axis2_stream_t *stream = NULL;
axis2_error_t *error = NULL;
axis2_log_t *axis_log     = NULL;
FILE *f = NULL;
/** a method that demonstrate creating a om model using an xml file */


int read_input(char *buffer,int size)
{
   return fread(buffer, sizeof(char),size,f);
}

int
test_om_build (char *filename)
{
    axis2_om_element_t *ele1 = NULL, *ele2 = NULL;
    axis2_om_stax_builder_t *builder = NULL;
    axis2_om_text_t *text = NULL;
    axis2_om_document_t *document = NULL;
    axis2_om_node_t *node1 = NULL, *node2 = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_om_namespace_t* ns = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_xml_writer_t *writer = NULL;
    axis2_char_t *buf = NULL;
    
    
    f =fopen(filename, "r");
    if(!f)
        return -1;
      
    /** create pull parser */
    reader = axis2_xml_reader_create_for_memory(&environment, read_input, NULL);
    
    if(!reader)
    {
        printf("ERROR CREATING PULLPARSER");
        return -1;
    }
    /** create axis2_om_stax_builder by parsing pull_parser struct */
    
    builder = axis2_om_stax_builder_create (&environment,reader);
    
    if(!builder)
    {
        printf("ERROR CREATING PULL PARSER");
        return -1;
    }
    /** 
        create an om document
        document is the container of om model created using builder 
    */
           
    document = axis2_om_document_create (&environment, NULL, builder);
    /**
        get root element , building starts hear 
     */
    
    node1 = AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT (document,&environment);
    if(node1)
    {
        /** print root node information */
        
       if(AXIS2_OM_NODE_GET_NODE_TYPE(node1, &environment) == AXIS2_OM_ELEMENT)
       {
        ele1 =AXIS2_OM_NODE_GET_DATA_ELEMENT(node1,&environment);
        if(ele1)
             
        printf ("root localname %s\n",AXIS2_OM_ELEMENT_GET_LOCALNAME(ele1,&environment));
         
        ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(ele1,&environment);
    
        if (ns)
        {
            printf ("root ns prefix %s\n", AXIS2_OM_NAMESPACE_GET_PREFIX(ns,&environment));
            printf ("root ns uri %s\n", AXIS2_OM_NAMESPACE_GET_URI(ns,&environment));
            ns = NULL;
        }
       }
    }
    /** build the document continuously untill all the xml file is built in to a om model */
    
    
    node2 = AXIS2_OM_DOCUMENT_BUILD_NEXT( document , &environment);
    do
    {

        if (!node2)
            break;

        switch (AXIS2_OM_NODE_GET_NODE_TYPE(node2,&environment))
        {
        case AXIS2_OM_ELEMENT:
            ele2 =(axis2_om_element_t*) AXIS2_OM_NODE_GET_DATA_ELEMENT(node2, &environment);
        if(ele2 && AXIS2_OM_ELEMENT_GET_LOCALNAME(ele2,&environment))
        { 
               printf("\n localname %s\n" , AXIS2_OM_ELEMENT_GET_LOCALNAME(ele2,&environment)); 
            
               ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(ele2,&environment);
    
            if (ns)
            {
                printf ("prefix %s\n", AXIS2_OM_NAMESPACE_GET_PREFIX(ns,&environment));
                printf ("ns uri %s\n", AXIS2_OM_NAMESPACE_GET_URI(ns,&environment));
            }
            
                ns = NULL;
            }            
            break;
        case AXIS2_OM_TEXT:
            
            text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node2,&environment);    
            if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&environment))
                printf("\n text value  %s \n", AXIS2_OM_TEXT_GET_VALUE(text,&environment));
            break;

        default:
            break;
        }

        node2 = AXIS2_OM_DOCUMENT_BUILD_NEXT (document , &environment);
    }
    while (node2);
    printf ("END: pull document\n");

    printf ("Serialize pulled document\n\n\n\n");
    
    writer = axis2_xml_writer_create_for_memory(&environment, &buf, NULL, AXIS2_TRUE, 0);
    /*
    writer = axis2_xml_writer_create(&environment, "finaltest.xml", NULL, AXIS2_TRUE, 0);
    */
    om_output = axis2_om_output_create (&environment, writer);
    AXIS2_OM_NODE_SERIALIZE (AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(document, &environment), &environment , om_output);
    AXIS2_OM_DOCUMENT_FREE(document, &environment); 
    axis2_om_output_free(om_output, &environment);  
    AXIS2_OM_STAX_BUILDER_FREE(builder, &environment);
  
    printf("%s",buf);    
    AXIS2_FREE(environment->allocator, buf);
    printf ("\ndone\n");
    return 0;
}



int
test_om_serialize ()
{

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
    axis2_xml_writer_t *writer = NULL;
    axis2_om_element_t *ele1 = NULL, *ele2 = NULL, *ele3 = NULL, *ele4 =
        NULL;
    axis2_om_node_t *node1 = NULL, *node2 = NULL, *node3 = NULL, *node4 =
        NULL, *node5 = NULL, *node6 = NULL;
    axis2_om_attribute_t *attr1 = NULL, *attr2 = NULL;
    axis2_om_namespace_t *ns1 = NULL, *ns2 = NULL, *ns3 = NULL;
    axis2_om_text_t *text1 = NULL;
    axis2_om_output_t *om_output = NULL;
    char *buffer = NULL;
    ns1 =
        axis2_om_namespace_create (&environment,
                                   "http://ws.apache.org/axis2/c/om",
                                   "axiom");
    ns2 =
        axis2_om_namespace_create (&environment, "urn:ISBN:0-395-74341-6",
                                   "isbn");

    ele1 = axis2_om_element_create (&environment, NULL, "book", ns1, &node1);
    AXIS2_OM_ELEMENT_DECLARE_NAMESPACE(ele1,&environment,node1,ns2);
    
    ele2 = axis2_om_element_create (&environment, node1, "title", ns1, &node2);
    
    text1 = axis2_om_text_create (&environment, node2, "Axis2/C OM HOWTO", &node3);
    
    ele3 = axis2_om_element_create (&environment, node1, "number", ns2, &node4);
    
    text1 = axis2_om_text_create (&environment, node4, "1748491379", &node5);
    
    ele4 = axis2_om_element_create (&environment, node1, "author", ns1, &node6);
    
    attr1 = axis2_om_attribute_create (&environment, "title", "Mr", ns1);
    
    AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(ele4, &environment,attr1);
    
    attr2 = axis2_om_attribute_create (&environment, "name", "Axitoc Oman", ns1);
    
    AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(ele4,&environment, attr2);
    
    writer = axis2_xml_writer_create_for_memory(&environment, &buffer, NULL, AXIS2_TRUE, 0);
    /* for guththila use following */
    /*
    writer = axis2_xml_writer_create(&environment,NULL, NULL, AXIS2_TRUE, 0);
    */
   
    /* serializing stuff */
    om_output = axis2_om_output_create (&environment,writer);

    printf ("Serialize built document\n");
    AXIS2_XML_WRITER_WRITE_START_DOCUMENT(om_output->xml_writer , &environment);
    status = AXIS2_OM_NODE_SERIALIZE (node1,&environment ,om_output);
    AXIS2_XML_WRITER_WRITE_END_DOCUMENT(om_output->xml_writer, &environment);
    if (status != AXIS2_SUCCESS)
    {
        printf ("\naxis2_om_node_serialize failed\n");
        return status;
    }
    else
        printf ("\naxis2_om_node_serialize success\n");
    /* end serializing stuff */

     AXIS2_OM_NODE_FREE_TREE(node1,&environment);
     axis2_om_output_free(om_output, &environment);
             
     printf("%s", buffer); 
     AXIS2_FREE(environment->allocator, buffer); 
     printf ("\nDONE\n");

    return 0;
}

int
main (int argc, char *argv[])
{
    char *file_name = "contents.xml";
    if (argc > 1)
        file_name = argv[1];
    allocator = axis2_allocator_init (NULL);
    axis_log = axis2_log_create(allocator, NULL);
    error = axis2_error_create(allocator);

    stream = axis2_stream_create(allocator, NULL);

    environment = axis2_env_create_with_error_stream_log(allocator, error, stream, axis_log);
    test_om_build (file_name); 
    test_om_serialize();
    axis2_env_free(environment); 
    
    return 0;
 }
