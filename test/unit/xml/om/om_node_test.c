#include "om_node_test.h"
#include <CuTest.h>

void Testaxis2_om_node_add_child(CuTest *tc)
{
    axis2_om_node_t *parent;
    axis2_om_node_t *child;
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_om_node_t *actual;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    
    parent = axis2_om_node_create(&env);   
    child  = axis2_om_node_create(&env); 
    
    AXIS2_OM_NODE_ADD_CHILD(parent, &env, child);
    actual = AXIS2_OM_NODE_GET_FIRST_CHILD(parent, &env);
    CuAssertPtrEquals(tc, child, actual);  
    /*axis2_env_free(env); */
}

void Testaxis2_om_node_set_parent(CuTest *tc)
{
    
    axis2_om_node_t *parent;
    axis2_om_node_t *child;
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_om_node_t *actual;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    
    parent = axis2_om_node_create(&env);   
    child  = axis2_om_node_create(&env); 
    
    AXIS2_OM_NODE_SET_PARENT(child, &env, parent);
    CuAssertPtrEquals(tc, parent, AXIS2_OM_NODE_GET_PARENT(child, &env));
    /*axis2_env_free(env);*/
}

    
void Testaxis2_om_node_insert_sibling_before(CuTest *tc)
{
    axis2_om_node_t *parent;
    axis2_om_node_t *child;
    axis2_om_node_t *sibling;
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    
    parent = axis2_om_node_create(&env);   
    child  = axis2_om_node_create(&env);
    sibling = axis2_om_node_create(&env);

    AXIS2_OM_NODE_ADD_CHILD(parent,  &env, child);
    AXIS2_OM_NODE_INSERT_SIBLING_BEFORE(child, &env, sibling);
    
    CuAssertPtrEquals(tc, sibling,AXIS2_OM_NODE_GET_FIRST_CHILD(parent, &env));    
    /*axis2_env_free(env);*/
}
void Testaxis2_om_node_insert_sibling_after(CuTest *tc)
{
    axis2_om_node_t *parent;
    axis2_om_node_t *child;
    axis2_om_node_t *sibling;
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    
    parent = axis2_om_node_create(&env);   
    child  = axis2_om_node_create(&env);
    sibling = axis2_om_node_create(&env);

    AXIS2_OM_NODE_ADD_CHILD(parent, &env, child);
    AXIS2_OM_NODE_INSERT_SIBLING_AFTER(child, &env, sibling);
    
    CuAssertPtrEquals(tc, sibling,AXIS2_OM_NODE_GET_NEXT_SIBLING(child, &env));
    /*axis2_env_free(env);*/
}
void Testaxis2_om_node_detach(CuTest *tc)
{
    axis2_om_node_t *parent;
    axis2_om_node_t *child;
    axis2_om_node_t *sibling1;
    axis2_om_node_t *sibling2;
    
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    
    parent = axis2_om_node_create(&env);   
    child  = axis2_om_node_create(&env);
    sibling1 = axis2_om_node_create(&env);
    sibling2 = axis2_om_node_create(&env);
    
    
    AXIS2_OM_NODE_ADD_CHILD(parent, &env, child);
    AXIS2_OM_NODE_ADD_CHILD(parent, &env, sibling1);
    AXIS2_OM_NODE_ADD_CHILD(parent, &env, sibling2);

    CuAssertPtrEquals(tc, sibling1, AXIS2_OM_NODE_GET_NEXT_SIBLING(child, &env));

    AXIS2_OM_NODE_DETACH(sibling1, &env);
    
    CuAssertPtrEquals(tc, sibling2, AXIS2_OM_NODE_GET_NEXT_SIBLING(child, &env));
    /*axis2_env_free(env);*/
}
