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

#include <stdio.h>
#include <axis2_util.h>
#include <oxs_constants.h>
#include <oxs_error.h>
#include <oxs_buffer.h>
#include <oxs_cipher.h>
#include <oxs_c14n.h>
#include <oxs_axiom.h>
#include <oxs_utility.h>
#include <openssl_rsa.h>
#include <openssl_digest.h>
#include <oxs_sign_ctx.h>
#include <oxs_sign_part.h>
#include <oxs_xml_signature.h>
#include <oxs_signature.h>
#include <oxs_transform.h>
#include <oxs_token_ds_reference.h>
#include <oxs_token_digest_method.h>
#include <oxs_token_digest_value.h>
#include <oxs_token_transforms.h>
#include <oxs_token_transform.h>
#include <oxs_token_c14n_method.h>
#include <oxs_token_signature.h>
#include <oxs_token_signature_method.h>
#include <oxs_token_signature_value.h>
#include <oxs_token_signed_info.h>
#include <oxs_token_x509_certificate.h>
#include <oxs_token_x509_data.h>
#include <oxs_token_key_info.h>
/*Private functions*/

/*parent is ds:Signature*/
static axis2_status_t
oxs_xml_sig_build_key_info(const axis2_env_t *env,
    axiom_node_t *parent,
    oxs_sign_ctx_t *sign_ctx)
{
    axiom_node_t *key_info_node = NULL;
    axiom_node_t *x509_data_node = NULL;
    axiom_node_t *x509_certificate_node = NULL;
    axis2_char_t *cert_data = NULL;
    oxs_x509_cert_t *cert = NULL;
    
    /*Build the KeyInfo node*/
    key_info_node = oxs_token_build_key_info_element(env, parent);

    /*TODO: Right now we support only X509Data. But should support other patterns as well*/
    cert = oxs_sign_ctx_get_certificate(sign_ctx, env);
    if(!cert){
        return AXIS2_FAILURE;
    }

    /*Get certificate data*/
    cert_data = oxs_x509_cert_get_data(cert, env);

    /*Build the X509Data node*/
    x509_data_node = oxs_token_build_x509_data_element(env, key_info_node);

    /*Now build the X509Certificate node*/
    x509_certificate_node = oxs_token_build_x509_certificate_element(env, x509_data_node, cert_data); 

    return AXIS2_SUCCESS;
}

/*parent is ds:SignedInfo*/
static axis2_status_t
oxs_xml_sig_build_reference(const axis2_env_t *env,
    axiom_node_t *parent,
    oxs_sign_part_t *sign_part)
{
    axis2_char_t *serialized_node = NULL; 
    axis2_char_t *digest = NULL; 
    axis2_char_t *digest_mtd = NULL; 
    axis2_char_t *ref_id = NULL; 
    axis2_char_t *id = NULL; 
    axis2_array_list_t *transforms = NULL;
    axiom_node_t *node = NULL;
    axiom_node_t *reference_node = NULL;
    axiom_node_t *digest_value_node = NULL;
    axiom_node_t *digest_mtd_node = NULL;
    int i=0;

    /*Get the node to digest*/
    node = oxs_sign_part_get_node(sign_part, env);
    
    /*Get the reference ID from the node and hence to the ds:Reference node*/
    id = oxs_axiom_get_attribute_value_of_node_by_name(env, node, "wsu:Id"); 
    
    ref_id = AXIS2_STRACAT("#", id, env);/* <ds:Reference URI="#id">*/
    reference_node = oxs_token_build_ds_reference_element(env, parent ,NULL, ref_id, NULL);

    /*Get transforms if any*/
    transforms = oxs_sign_part_get_transforms(sign_part, env);
    
    if((transforms) && (0 < AXIS2_ARRAY_LIST_SIZE(transforms, env))){
        axiom_node_t *transforms_node = NULL;
        oxs_tr_dtype_t output_dtype = OXS_TRANSFORM_TYPE_UNKNOWN;/*This will always be the current dtype*/
        void *tr_output = NULL;
        output_dtype = OXS_TRANSFORM_TYPE_NODE; /*We always begin with a node*/
        
        tr_output = node; /*The first transformation is applied to the node*/

        /*Add ds:Transforms element*/
        transforms_node = oxs_token_build_transforms_element(env, reference_node);
        /*LOOP: Apply transforms. For example exclusive C14N*/
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(transforms, env); i++){
            oxs_transform_t *tr = NULL;
            oxs_transform_tr_func tr_func = NULL;
            oxs_tr_dtype_t input_dtype = OXS_TRANSFORM_TYPE_UNKNOWN;
            void *tr_input = NULL;
            axis2_char_t *tr_id = NULL;

            /*Get the ith transform*/
            tr = (oxs_transform_t*)AXIS2_ARRAY_LIST_GET(transforms, env, i);
            tr_id = oxs_transform_get_id(tr, env);
            tr_func = oxs_transform_get_transform_function(tr, env);
            input_dtype = oxs_transform_get_input_data_type(tr, env);
            
            printf("Transform required = %s", tr_id);
            /*Prepare the input*/
            /*If the required input type is CHAR and what we have is a NODE*/
            if((input_dtype == OXS_TRANSFORM_TYPE_CHAR) && (output_dtype == OXS_TRANSFORM_TYPE_NODE)){
                /*Serialize*/
                tr_input = axiom_node_to_string((axiom_node_t*)tr_output, env);
            /*If the required input type is NODE and what we have is a CHAR*/
            }else if((input_dtype == OXS_TRANSFORM_TYPE_NODE) && (output_dtype == OXS_TRANSFORM_TYPE_CHAR)){
                /*TODO De-serialize*/
            }else{
                /*Let it go as it is. */
                tr_input = tr_output;
            }

            /*Apply transform*/
            if(tr_func){
                output_dtype = (*tr_func)(env, tr_input, input_dtype, &tr_output);
            }else{
                oxs_error(env, ERROR_LOCATION, OXS_ERROR_TRANSFORM_FAILED,"Cannot get the transform implementation for %s", tr_id);
            }
            /*If the output data type is unknown OR the output is NULL its an error*/
            if((output_dtype == OXS_TRANSFORM_TYPE_UNKNOWN) || (!tr_output)){
                oxs_error(env, ERROR_LOCATION, OXS_ERROR_TRANSFORM_FAILED,"Transform failed for %s", tr_id);
                /*return AXIS2_FAILURE*/
            }
            /*Add to ds:Transforms element*/
            oxs_token_build_transform_element(env, transforms_node, tr_id);
        }
    }
    /*Serialize node*/
    serialized_node = AXIOM_NODE_TO_STRING(node, env);
    
    /*Make digest.*/
    digest_mtd = oxs_sign_part_get_digest_mtd(sign_part, env);
    digest = openssl_sha1(env, serialized_node, axis2_strlen(serialized_node)); 

    /*Construct nodes*/
    digest_mtd_node = oxs_token_build_digest_method_element(env, reference_node, digest_mtd);
    digest_value_node = oxs_token_build_digest_value_element(env, reference_node, digest);
    
    return AXIS2_SUCCESS; 
}
/**
 *  C14N -> Serialize -> Sign the <SignedInfo> element
 */
static axis2_status_t
oxs_xml_sig_sign_signed_info(const axis2_env_t *env,
    axiom_node_t *signature_node,
    axiom_node_t *signed_info_node,
    oxs_sign_ctx_t *sign_ctx)
{
    axis2_char_t *signature_val = "FAKE_SIG_VAL(734dwe93721fd8y2==";
    axis2_char_t *serialized_signed_info = NULL;
    axis2_char_t *c14n_algo = NULL;
    axis2_char_t *c14nized = NULL;
    axiom_node_t *signature_val_node = NULL;
    axiom_document_t *doc = NULL;
    oxs_buffer_t *input_buf = NULL;
    oxs_buffer_t *output_buf = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    /*TODO : Cannonicalize <SignedInfo>*/
    c14n_algo = oxs_sign_ctx_get_c14n_mtd(sign_ctx, env);
    doc = axiom_node_get_document(signed_info_node, env);
    oxs_c14n_apply(env, doc, AXIS2_FALSE, &c14nized, AXIS2_TRUE, NULL, signed_info_node); 
    
    /*Then serialize <SignedInfo>*/
    serialized_signed_info = c14nized; /*AXIOM_NODE_TO_STRING(signed_info_node, env);*/

    /*Make the input and out put buffers*/
    input_buf = oxs_buffer_create(env);
    output_buf = oxs_buffer_create(env);

    OXS_BUFFER_POPULATE(input_buf, env, (unsigned char *)serialized_signed_info, axis2_strlen(serialized_signed_info));
    /*Then sign... NOTE: The signature process includes making the digest. e.g. rsa-sha1 => RSA(SHA-1(contents))*/ 
    status = oxs_sig_sign(env, sign_ctx, input_buf, output_buf);

    signature_val = (axis2_char_t*)OXS_BUFFER_GET_DATA(output_buf, env);
    
    /*Construct <SignatureValue>*/
    signature_val_node = oxs_token_build_signature_value_element(env, signature_node, signature_val);

    return AXIS2_SUCCESS;
}

/*Public functions*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL 
oxs_xml_sig_sign(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    axiom_node_t *parent)
{
    axiom_node_t *signed_info_node = NULL;
    axiom_node_t *signature_node = NULL;
    axiom_node_t *signature_mtd_node = NULL;
    axiom_node_t *c14n_mtd_node = NULL;
    axis2_char_t *sign_algo = NULL;
    axis2_char_t *c14n_algo = NULL;
    axis2_array_list_t *sign_parts = NULL;
    int i=0;

    /*Construct the <Signature> element*/
    signature_node = oxs_token_build_signature_element(env, parent, "Sign-ID");

    /*Construct the <SignedInfo>  */
    signed_info_node = oxs_token_build_signed_info_element(env, signature_node);

    /*Construct the <CanonicalizationMethod> */
    c14n_algo = oxs_sign_ctx_get_c14n_mtd(sign_ctx, env);
    c14n_mtd_node = oxs_token_build_c14n_method_element(env, signed_info_node, c14n_algo);

    /*Construct the <SignatureMethod>  */
    sign_algo = oxs_sign_ctx_get_sign_mtd_algo(sign_ctx, env);
    signature_mtd_node = oxs_token_build_signature_method_element(env, signed_info_node, sign_algo);

    /*Look for signature parts*/
    sign_parts = oxs_sign_ctx_get_sign_parts(sign_ctx , env);

    /*For each and every signature part in sig ctx,*/
    for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(sign_parts, env); i++){
        oxs_sign_part_t *sign_part =  NULL;

        /*TODO Get ith sign_part*/
        sign_part = (oxs_sign_part_t*)axis2_array_list_get(sign_parts, env, i);
        /*Create <ds:Reference> elements */
        oxs_xml_sig_build_reference(env, signed_info_node, sign_part);

    }
    /*At this point we have a complete <SignedInfo> node. Now we need to sign it*/
    oxs_xml_sig_sign_signed_info(env, signature_node, signed_info_node, sign_ctx); 

    /*Now we need to build the KeyInfo node*/    
    oxs_xml_sig_build_key_info(env, signature_node, sign_ctx);     

    return AXIS2_SUCCESS;
}

/*******************************Verification specific*****************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_xml_sig_process_ref_node(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    axiom_node_t *ref_node,
    axiom_node_t *scope_node)
{
    axis2_char_t *ref_id = NULL;
    axis2_char_t *child_node_name = NULL;
    oxs_sign_part_t *sign_part = NULL;
    axiom_node_t *reffed_node = NULL;
    axiom_node_t *child_node = NULL;

    /*Create a sign part and populate it*/
    sign_part = oxs_sign_part_create(env);

    ref_id =  oxs_token_get_ds_reference(env, ref_node);
    oxs_sign_part_set_id(sign_part, env, ref_id);

    /*Find the node refered by this ref_id and set to the sign part*/
    reffed_node = oxs_axiom_get_node_by_id(env, scope_node, "wsu:Id", ref_id );
    if(reffed_node){
       oxs_sign_part_set_node(sign_part, env, reffed_node); 
    }else{
        return AXIS2_FAILURE; /*No such node. Its an error*/
    }

    /*First child is optional Transforms element*/
    child_node = AXIOM_NODE_GET_FIRST_CHILD(ref_node, env);
    child_node_name = axiom_util_get_localname(child_node, env);
    if(0 == axis2_strcmp(child_node_name, OXS_NODE_TRANSFORMS)){
        /*Transforms found*/
        /*TODO*/

        /*At the end, set the next node as the child node*/
        child_node = AXIOM_NODE_GET_NEXT_SIBLING(child_node, env);
    }else{
        /*There are no transforms for this sign part*/
    }
    /* mandatory ds:DigestMethod*/
    child_node_name = axiom_util_get_localname(child_node, env);
    if(0 == axis2_strcmp(child_node_name, OXS_NODE_DIGEST_METHOD)){
        axis2_char_t *digest_mtd = NULL;
        /*ds:DigestMethod found*/
        digest_mtd = oxs_token_get_digest_method(env, child_node);
        oxs_sign_part_set_digest_mtd(sign_part, env, digest_mtd);

        /*At the end, set the next node as the child node*/
        child_node = AXIOM_NODE_GET_NEXT_SIBLING(child_node, env);
    }else{
        return AXIS2_FAILURE;
    }

    /* mandatory ds:DigestValue*/
    child_node_name = axiom_util_get_localname(child_node, env);
    if(0 == axis2_strcmp(child_node_name, OXS_NODE_DIGEST_VALUE)){
        /*ds:DigestValue found*/
        axis2_char_t *digest_val = NULL;
        digest_val = oxs_token_get_digest_value(env, child_node);  
        oxs_sign_part_set_digest_val(sign_part, env, digest_val);
    }else{
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;

}

    /*Process Signature Node. We need to populate 
     * 1. Sig_mtd
     * 2. C14N Mtd
     * 3. Sign parts
     *      3.1. Id
     *      3.2  Digest mtd
     *      3.3. Transforms*/
   
AXIS2_EXTERN axis2_status_t AXIS2_CALL 
oxs_xml_sig_process_signature_node(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    axiom_node_t *signature_node,
    axiom_node_t *scope_node)
{
    axiom_node_t *cur_node = NULL;
    axiom_node_t *signed_info_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    signed_info_node = oxs_axiom_get_first_child_node_by_name(env, signature_node, 
                            OXS_NODE_SIGNEDINFO, OXS_DSIG_NS, OXS_DS );
    if(!signed_info_node){
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_SIG_VERIFICATION_FAILED,"Cannot find <ds:SignedInfo> " );        
        return AXIS2_FAILURE;
    }

    /*Process signed info element*/
    cur_node = AXIOM_NODE_GET_FIRST_CHILD(signed_info_node, env);
    /*Iterate thru children of <SignedInfo>*/
    while(cur_node){
        axis2_char_t *localname =  NULL;
        
        localname  = axiom_util_get_localname(cur_node, env);
         
        if(0 == axis2_strcmp(localname, OXS_NODE_CANONICALIZATION_METHOD)){
            axis2_char_t *c14n_mtd = NULL;
            c14n_mtd = oxs_token_get_c14n_method(env, cur_node);
            oxs_sign_ctx_set_c14n_mtd(sign_ctx, env, c14n_mtd);

        }else if(0 == axis2_strcmp(localname, OXS_NODE_SIGNATURE_METHOD)){
            axis2_char_t *sig_mtd = NULL;
            sig_mtd = oxs_token_get_signature_method(env, cur_node);
            oxs_sign_ctx_set_sign_mtd_algo(sign_ctx, env, sig_mtd);

        }else if(0 == axis2_strcmp(localname, OXS_NODE_REFERENCE)){
            /*There might be multiple references*/
            status = oxs_xml_sig_process_ref_node(env, sign_ctx, cur_node, scope_node);        
            if(status == AXIS2_FAILURE){
                return AXIS2_FAILURE;
            }

        }else{
            printf("oxs_sig : We do not process cur node name %s", localname);
        }
        
        cur_node = AXIOM_NODE_GET_NEXT_SIBLING(cur_node, env);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
oxs_xml_sig_verify(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    axiom_node_t *signature_node,
    axiom_node_t *scope_node)
{
    axis2_status_t status = AXIS2_FAILURE;

    /*Set operation to verify*/
    oxs_sign_ctx_set_operation(sign_ctx, env, OXS_SIGN_OPERATION_VERIFY);
    /*Populate the sign_ctx by inspecting the ds:Signature node*/
    status = oxs_xml_sig_process_signature_node(env, sign_ctx, signature_node, scope_node);
    if(status != AXIS2_SUCCESS){
        /*Something went wrong in the Signature node!!! :(*/
        return AXIS2_FAILURE;
    }
    
    /*Verify*/
    return AXIS2_SUCCESS;
}
