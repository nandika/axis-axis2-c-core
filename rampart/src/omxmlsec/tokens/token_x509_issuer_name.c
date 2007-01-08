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
#include <oxs_constants.h>
#include <oxs_error.h>
#include <oxs_token_x509_issuer_name.h>
#include <axiom_element.h>
#include <oxs_axiom.h>


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_token_get_issuer_name(const axis2_env_t *env,
        axiom_node_t *issuer_name_node)
{
    axis2_char_t *val = NULL;
    /*TODO Verification*/
    val = (axis2_char_t*)oxs_axiom_get_node_content(env, issuer_name_node);
    return val;

}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_token_build_issuer_name_element(const axis2_env_t *env,
        axiom_node_t *parent,
        axis2_char_t* value
                                    )
{
    axiom_node_t *issuer_name_node = NULL;
    axiom_element_t *issuer_name_ele = NULL;
    axis2_status_t ret;
    axiom_namespace_t *ns_obj = NULL;

    ns_obj = axiom_namespace_create(env, OXS_DSIG_NS,
            OXS_DS);

    issuer_name_ele = axiom_element_create(env, parent, OXS_NODE_X509_ISSUER_NAME, ns_obj, &issuer_name_node);
    if (!issuer_name_ele)
    {
        oxs_error(env, ERROR_LOCATION,
                OXS_ERROR_ELEMENT_FAILED, "Error creating  element");
        return NULL;
    }

    if (value)
    {
        ret  = AXIOM_ELEMENT_SET_TEXT(issuer_name_ele, env, value, issuer_name_node);
    }

    return issuer_name_node;

}

