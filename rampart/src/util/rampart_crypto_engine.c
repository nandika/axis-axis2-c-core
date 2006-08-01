/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

/* Rampart_crypto_util will contain crypto functionalities of the rampart model
 * 
 */
#include <stdio.h>
#include <axis2_utils.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_enc_engine.h>
#include <rampart_crypto_engine.h>
#include <oxs_token_encrypted_type.h>
#include <oxs_token_encryption_method.h>
#include <oxs_token_cipher_data.h>
#include <oxs_token_cipher_value.h>
#include <oxs_token_key_info.h>
#include <oxs_token_key_name.h>

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_crypto_decrypt_message(const axis2_env_t *env,
                    /*  struct axis2_msg_ctx *msg_ctx,*/
                      axiom_soap_envelope_t *soap_envelope )
{
    axis2_char_t *key = NULL, *key_name = NULL;
    axis2_status_t ret = AXIS2_FAILURE;
    axiom_node_t *enc_data_node = NULL;
    axiom_node_t *body_node = NULL, *header_node = NULL;
    axiom_soap_body_t *body = NULL;
    axiom_soap_header_t *header = NULL;
    axis2_char_t *decrypted_data = NULL;
    enc_ctx_ptr enc_ctx = NULL;

    ret = AXIS2_SUCCESS;/*TODO Remove*/
    /*TODO get the key using callbacks*/
    key = "0123456701234567";
    key_name = "KauKey";

    body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    header = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);

    body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(body, env);
    header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(header, env);

    /*TODO We assume that the very first element of bpody is encrypted data.
    This might be different if a sub element is encrypted*/
    enc_data_node = AXIOM_NODE_GET_FIRST_CHILD(body_node, env);
/*    ret = oxs_axiom_check_node_name(env, enc_data_node, OXS_NodeEncryptedData, NULL);   
    if(ret){
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                     "EncryptedData node searching failed." );
        return ret;
        
    }
*/    
    /*Build the encryption ctx*/
    enc_ctx = oxs_ctx_create_ctx(env);

    /*Set the key*/
    enc_ctx->key = oxs_key_create_key(env, key_name, (unsigned char*)key, AXIS2_STRLEN(key), OXS_KEY_USAGE_DECRYPT);

    ret = oxs_enc_decrypt_template(env, enc_data_node, &decrypted_data, enc_ctx);
    if(ret == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                     "oxs_enc_decrypt_template failed");
        return ret;
    }else{
        printf("Decrypted data is \n %s", decrypted_data);
    }

    /*Create a stream reader and then build the node using decrypted text*/

    return ret;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_crypto_encrypt_message(const axis2_env_t *env,
                    /*  struct axis2_msg_ctx *msg_ctx,*/
                      axiom_soap_envelope_t *soap_envelope )
{
    axis2_char_t *key = NULL, *key_name = NULL;
    axis2_status_t ret = AXIS2_FAILURE;
    axiom_node_t *node_to_enc = NULL, *body_node = NULL, *header_node = NULL;
    axiom_node_t *enc_type_node = NULL, *enc_mtd_node = NULL, *key_info_node = NULL, *key_name_node = NULL;
    axiom_node_t *cv_node = NULL, *cd_node = NULL, *temp = NULL;
    axiom_soap_body_t *body = NULL;
    axiom_soap_header_t *header = NULL;
    axis2_char_t *str_to_enc = NULL;
    enc_ctx_ptr enc_ctx = NULL;
       

    ret = AXIS2_SUCCESS;/*TODO Remove*/
   
    /*TODO get the key using callbacks*/ 
    key = "0123456701234567";
    key_name = "KauKey";

    body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    header = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
       
    body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(body, env);
    header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(header, env);

    /*TODO Get the node to be encrypted*/   
    /*If non is specified we encrypt the first element of the Body element*/
    if(!node_to_enc){
        node_to_enc = AXIOM_NODE_GET_FIRST_CHILD(body_node, env);
    }

    str_to_enc = AXIOM_NODE_TO_STRING(node_to_enc, env);
    
    /*Build the template*/
    /*TODO Support enc key later*/
    /*NOTE : Here I pass body_node as the parent. Might be a prob :(*/
    enc_type_node =  oxs_token_build_encrypted_type_element(env, 
                        AXIOM_NODE_GET_PARENT(node_to_enc, env),
                        OXS_NodeEncryptedData, 
                        OXS_TypeEncElement,
                        "EncDataId-12345" );
    enc_mtd_node = oxs_token_build_encryption_method_element(env, enc_type_node, OXS_HrefDes3Cbc);
    key_info_node = oxs_token_build_key_info_element(env, enc_type_node);
    key_name_node = oxs_token_build_key_name_element(env, key_info_node, key_name );
    cd_node = oxs_token_build_cipher_data_element(env, enc_type_node);
    cv_node = oxs_token_build_cipher_value_element(env,  cd_node, NULL); /*We pass NULL here OMXMLSEC will populate this*/

    /*Build the encryption ctx*/
    enc_ctx = oxs_ctx_create_ctx(env); 
    
    /*Set the key*/
    enc_ctx->key = oxs_key_create_key(env, key_name, (unsigned char*)key, AXIS2_STRLEN(key), OXS_KEY_USAGE_ENCRYPT);

    /*Hand the template over to OMXMLSEC*/
    ret = oxs_enc_encrypt_template(env, enc_type_node, str_to_enc, enc_ctx);
    if(ret == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                     "oxs_enc_encrypt_template failed");
        return ret;
    }else{
        printf("Encryption template is \n %s", AXIOM_NODE_TO_STRING(enc_type_node, env));
    }

    /*Remove the encrypted node*/
    temp = AXIOM_NODE_DETACH(node_to_enc, env);
    if(!temp){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "Detaching encrypyted node failed");
        return AXIS2_FAILURE;
    }

    /*Now arrange this encrypted nodes in a suitable manner to the envelope*/ 
    
    return ret;
}

