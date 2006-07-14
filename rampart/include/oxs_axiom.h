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

#ifndef OXS_AXIOM_H
#define OXS_AXIOM_H


/**
  * @file oxs_axiom.h
  * @brief 
  */

#include <axis2_defines.h>
#include <oxs_strings.h>
#include <axis2_env.h>
#include <axis2_util.h>
#include <axiom_node.h>
#include <axiom_document.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*** Forward Declarations ****/

typedef struct axiom_document_t oxs_axiom_document, *oxs_axiom_document_ptr;
typedef struct axiom_node_t oxs_axiom_node, *oxs_axiom_node_ptr;

/**
* returns 1 sucess 0 otherwise
*/
AXIS2_EXTERN int AXIS2_CALL
oxs_axiom_check_node_name(const axis2_env_t *env, axiom_node_t* node, axis2_char_t* name, axis2_char_t* ns);

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_AXIOM_H */
