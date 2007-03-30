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

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axutil_string.h>
#include <axis2_utils.h>
#include <axutil_hash.h>
#include <axis2_uri.h>
#include <axutil_qname.h>
#include <woden.h>
#include <woden_schema_constants.h>

axis2_bool_t AXIS2_CALL
woden_schema_constants_compare_schema(
    axutil_qname_t *schema,
    const axutil_env_t *env)
{
    axutil_qname_t *q1999 = NULL;
    axutil_qname_t *q2000 = NULL;
    axutil_qname_t *q2001 = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    q1999 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_XSD_1999);
    q2000 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_XSD_2000);
    q2001 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_XSD_2001);

    if (AXIS2_TRUE == axutil_qname_equals(schema, env, q1999) ||
            AXIS2_TRUE == axutil_qname_equals(schema, env, q2000) ||
            AXIS2_TRUE == axutil_qname_equals(schema, env, q2001))
    {
        ret = AXIS2_TRUE;
    }
    if (q1999)
        axutil_qname_free(q1999, env);
    if (q2000)
        axutil_qname_free(q2000, env);
    if (q2001)
        axutil_qname_free(q2001, env);

    return ret;
}

axis2_bool_t AXIS2_CALL
woden_schema_constants_compare_include(
    axutil_qname_t *include,
    const axutil_env_t *env)
{
    axutil_qname_t *q1999 = NULL;
    axutil_qname_t *q2000 = NULL;
    axutil_qname_t *q2001 = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    q1999 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_INCLUDE_XSD_1999);
    q2000 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_INCLUDE_XSD_2000);
    q2001 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_INCLUDE_XSD_2001);

    if (AXIS2_TRUE == axutil_qname_equals(include, env, q1999) ||
            AXIS2_TRUE == axutil_qname_equals(include, env, q2000) ||
            AXIS2_TRUE == axutil_qname_equals(include, env, q2001))
    {
        ret = AXIS2_TRUE;
    }
    if (q1999)
        axutil_qname_free(q1999, env);
    if (q2000)
        axutil_qname_free(q2000, env);
    if (q2001)
        axutil_qname_free(q2001, env);

    return ret;
}

axis2_bool_t AXIS2_CALL
woden_schema_constants_compare_import(
    axutil_qname_t *import,
    const axutil_env_t *env)
{
    axutil_qname_t *q1999 = NULL;
    axutil_qname_t *q2000 = NULL;
    axutil_qname_t *q2001 = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    q1999 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_IMPORT_XSD_1999);
    q2000 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_IMPORT_XSD_2000);
    q2001 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_IMPORT_XSD_2001);

    if (AXIS2_TRUE == axutil_qname_equals(import, env, q1999) ||
            AXIS2_TRUE == axutil_qname_equals(import, env, q2000) ||
            AXIS2_TRUE == axutil_qname_equals(import, env, q2001))
    {
        ret = AXIS2_TRUE;
    }
    if (q1999)
        axutil_qname_free(q1999, env);
    if (q2000)
        axutil_qname_free(q2000, env);
    if (q2001)
        axutil_qname_free(q2001, env);

    return ret;
}

axis2_bool_t AXIS2_CALL
woden_schema_constants_compare_redefine(
    axutil_qname_t *redefine,
    const axutil_env_t *env)
{
    axutil_qname_t *q1999 = NULL;
    axutil_qname_t *q2000 = NULL;
    axutil_qname_t *q2001 = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    q1999 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_REDEFINE_XSD_1999);
    q2000 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_REDEFINE_XSD_2000);
    q2001 = axutil_qname_create_from_string(env, WODEN_Q_ELEM_REDEFINE_XSD_2001);

    if (AXIS2_TRUE == axutil_qname_equals(redefine, env, q1999) ||
            AXIS2_TRUE == axutil_qname_equals(redefine, env, q2000) ||
            AXIS2_TRUE == axutil_qname_equals(redefine, env, q2001))
    {
        ret = AXIS2_TRUE;
    }
    if (q1999)
        axutil_qname_free(q1999, env);
    if (q2000)
        axutil_qname_free(q2000, env);
    if (q2001)
        axutil_qname_free(q2001, env);

    return ret;
}

