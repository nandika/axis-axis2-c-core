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

#ifndef AXIS2_RAND_H
#define AXIS2_RAND_H

#include <axis2_error.h>
#include <axis2_env.h>
#include <axis2_date_time.h>
#include <axis2_base64_binary.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_rand rand
 * @ingroup axis2_util
 * @{
 */

/**
 * @file axis2_rand.h
 * @brief A simple thread safe and reentrant random number generator
 */

/**
 * This is reentrant and thread safe simple random number generator
 * function. it is passed an pointer to an unsigned int state value 
 * which is used inside the function and changed in each call.
 * @param seedp pointer to an unsigned int used as the internal state
 * @return int <code>int<code>
 */
AXIS2_EXTERN int AXIS2_CALL
axis2_rand(unsigned int *seedp);

/**
 * This is reentrant and thread safe simple random number generator
 * function. it is passed an pointer to an unsigned int state value 
 * which is used inside the function and changed in each call. Also 
 * it is passed a range in which the random number is selected
 * @param seedp pointer to an unsigned int used as the internal state
 * @param start start of the range
 * @param end end of the range
 * @return int If invalid range is entered -1 is returned <code>int<code>
 */
AXIS2_EXTERN int AXIS2_CALL
axis2_rand_with_range(unsigned int *seedp,
    int start,
    int end);

/**
 * A random seed value generated based on the time
 */
AXIS2_EXTERN unsigned int AXIS2_CALL
axis2_rand_get_seed_value_based_on_time(
    const axis2_env_t *env);

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_RAND_H */
