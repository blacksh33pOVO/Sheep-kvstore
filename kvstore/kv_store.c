
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kv_store.h"
//#include "kvs_rbtree.c"
#include <assert.h>
#include <stddef.h>
#if ENABLE_ARRAY
extern kvs_array_t global_array;
#endif

#if ENABLE_RBTREE
extern kvs_rbtree_t global_rbtree;
#endif

#if ENABLE_HASH
extern kvs_hash_t global_hash;
#endif
void *kvs_malloc(size_t size)
{
    return malloc(size);
}

void kvs_free(void *ptr)
{
    return free(ptr);
}


const char *command[]=
{
    "SET", "GET", "DEL", "MOD", "EXIST",
    "RSET", "RGET", "RDEL", "RMOD", "REXIST",
    "HSET", "HGET", "HDEL", "HMOD", "HEXIST"
};

enum 
{
    //array
    KVS_CMD_START = 0,
    KVS_CMD_SET = KVS_CMD_START,
    KVS_CMD_GET,
    KVS_CMD_DEL,
    KVS_CMD_MOD,
    KVS_CMD_EXIST,

    //rbtree
    KVS_CMD_RSET,
    KVS_CMD_RGET,
    KVS_CMD_RDEL,
    KVS_CMD_RMOD,
    KVS_CMD_REXIST,

    //hash
    KVS_CMD_HSET,
    KVS_CMD_HGET,
    KVS_CMD_HDEL,
    KVS_CMD_HMOD,
    KVS_CMD_HEXIST,

    KVS_CMD_COUNT,
};
const char *response[] = 
{

};


int kvs_split_token(char *msg, char *tokens[])
{
    if (msg == NULL || tokens == NULL)
    {
        return -1;
    }
    
    int idx = 0;
    char *token = strtok(msg, " ");
    while (token != NULL)
    {
        tokens[idx++] = token;
        token = strtok(NULL, " ");
        
    }
    return idx;
}



int kvs_filter_procotol(char** tokens, int count, char *response)
{
    if (tokens[0] == NULL || count == 0 || response == NULL)
    {
        return -1;
    }
    int cmd = KVS_CMD_START;
    for (cmd = KVS_CMD_START; cmd < KVS_CMD_COUNT; cmd++)
    {
        if (strcmp(tokens[0], command[cmd]) == 0)
        {
            break;
        }
    }

    int length = 0;
    int ret = 0;
    switch (cmd)
    {
#if ENABLE_ARRAY
    //array
    case KVS_CMD_SET:
        ret = kvs_array_set(&global_array, tokens[1], tokens[2]);
        if (ret < 0)
        {
            length= sprintf(response, "ERROR\r\n");
        }else if (ret == 0)
        {
            length= sprintf(response, "OK\r\n");
        }else
        {
            length= sprintf(response, "EXIST\r\n");
        }
        break;


    case KVS_CMD_GET:
    {
        char *result = kvs_array_get(&global_array, tokens[1]);
        if (result == NULL)
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }else 
        {
            length= sprintf(response, "%s\r\n",result);
        }
        break;
    }

    case KVS_CMD_DEL:
        ret = kvs_array_del(&global_array, tokens[1]);
        if (ret < 0)
        {
            length= sprintf(response, "ERROR\r\n");
        }else if (ret == 0)
        {
            length= sprintf(response, "OK\r\n");
        }else
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }
        break;


    case KVS_CMD_MOD:
        ret = kvs_array_mod(&global_array, tokens[1], tokens[2]);
        if (ret < 0)
        {
            length= sprintf(response, "ERROR\r\n");
        }else if (ret == 0)
        {
            length= sprintf(response, "OK\r\n");
        }else
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }
        
        break;

    case KVS_CMD_EXIST:
        ret = kvs_array_exist(&global_array, tokens[1]);
        if (ret == 0)
        {
            length= sprintf(response, "EXIST\r\n");
        }else
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }
        break;
#endif

#if ENABLE_RBTREE
        //rbtree
        case KVS_CMD_RSET:
        ret = kvs_rbtree_set(&global_rbtree, tokens[1], tokens[2]);
        //ret = kvs_array_set(&global_array, tokens[1], tokens[2]);
        if (ret < 0)
        {
            length= sprintf(response, "ERROR\r\n");
        }else if (ret == 0)
        {
            length= sprintf(response, "OK\r\n");
        }else
        {
            length= sprintf(response, "EXIST\r\n");
        }
        break;


    case KVS_CMD_RGET:
    {
        //char *result = kvs_array_get(&global_array, tokens[1]);
        char *result = kvs_rbtree_get(&global_rbtree, tokens[1]);
        if (result == NULL)
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }else 
        {
            length= sprintf(response, "%s\r\n",result);
        }
    }
        break;


    case KVS_CMD_RDEL:
        //ret = kvs_array_del(&global_array, tokens[1]);
        ret = kvs_rbtree_del(&global_rbtree, tokens[1]);
        if (ret < 0)
        {
            length= sprintf(response, "ERROR\r\n");
        }else if (ret == 0)
        {
            length= sprintf(response, "OK\r\n");
        }else
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }
        break;


    case KVS_CMD_RMOD:
        //ret = kvs_array_mod(&global_array, tokens[1], tokens[2]);
        ret = kvs_rbtree_mod(&global_rbtree, tokens[1], tokens[2]);
        if (ret < 0)
        {
            length= sprintf(response, "ERROR\r\n");
        }else if (ret == 0)
        {
            length= sprintf(response, "OK\r\n");
        }else
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }
        
        break;

    case KVS_CMD_REXIST:
        //ret = kvs_array_exist(&global_array, tokens[1]);
        ret = kvs_rbtree_exist(&global_rbtree, tokens[1]);
        if (ret == 0)
        {
            length= sprintf(response, "EXIST\r\n");
        }else
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }
        
        break;
#endif


//hash
#if ENABLE_HASH
    case KVS_CMD_HSET:
        ret = kvs_hash_set(&global_hash, tokens[1], tokens[2]);

        if (ret < 0)
        {
            length= sprintf(response, "ERROR\r\n");
        }else if (ret == 0)
        {
            length= sprintf(response, "OK\r\n");
        }else
        {
            length= sprintf(response, "EXIST\r\n");
        }
        break;


    case KVS_CMD_HGET:
    {
        char *result = kvs_hash_get(&global_hash, tokens[1]);
        if (result == NULL)
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }else 
        {
            length= sprintf(response, "%s\r\n",result);
        }
    }
        break;


    case KVS_CMD_HDEL:
        //ret = kvs_array_del(&global_array, tokens[1]);
        ret = kvs_hash_del(&global_hash, tokens[1]);
        if (ret < 0)
        {
            length= sprintf(response, "ERROR\r\n");
        }else if (ret == 0)
        {
            length= sprintf(response, "OK\r\n");
        }else
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }
        break;


    case KVS_CMD_HMOD:
        //ret = kvs_array_mod(&global_array, tokens[1], tokens[2]);
        ret = kvs_hash_mod(&global_hash, tokens[1], tokens[2]);
        if (ret < 0)
        {
            length= sprintf(response, "ERROR\r\n");
        }else if (ret == 0)
        {
            length= sprintf(response, "OK\r\n");
        }else
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }
        
        break;

    case KVS_CMD_HEXIST:
        //ret = kvs_array_exist(&global_array, tokens[1]);
        ret = kvs_hash_exist(&global_hash, tokens[1]);
        if (ret == 0)
        {
            length= sprintf(response, "EXIST\r\n");
        }else
        {
            length= sprintf(response, "NON-EXIST\r\n");
        }
        
        break;

#endif
    default:
        assert(0);
    }
    return length;
}
    

    


int kvs_protocol(char *msg, int length, char *response)
{
    //SET Key Value
    //GET KEY
    //DEL KEY
    if (msg == NULL || length <= 0 || response == NULL)
    {
        return -1;
    }
    
    //printf("recv %d:%s\n", length, msg);
    char *tokens[KVS_MAX_TOKENS] = {0};
    int count = kvs_split_token(msg, tokens);
    if (count == -1)
    {
        return -1;
    }
    

    //memcpy(response, msg, length);
    return kvs_filter_procotol(tokens, count, response);
}

extern int reactor_start(unsigned short port, msg_handler handler);
extern int ntyco_start(unsigned short port, msg_handler handler);
extern int proactor_start(unsigned short port, msg_handler handler);

int init_kvengine(void)
{
#if ENABLE_ARRAY

    memset(&global_array, 0, sizeof(kvs_array_t));
    kvs_array_create(&global_array);


#endif

#if ENABLE_RBTREE 

    memset(&global_rbtree, 0, sizeof(kvs_rbtree_t));
    kvs_rbtree_create(&global_rbtree);
#endif

#if ENABLE_HASH
    memset(&global_hash, 0, sizeof(kvs_hash_t));
    kvs_hash_create(&global_hash);
#endif
    return 0;

}

void dest_kvengine(void)
{
#if ENABLE_ARRAY
    kvs_array_destroy(&global_array);
#endif

#if ENABLE_RBTREE 
    kvs_rbtree_destroy(&global_rbtree);
#endif

#if ENABLE_HASH
    kvs_hash_destroy(&global_hash);
#endif

}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return -1;

    init_kvengine();

    int port = atoi(argv[1]);
#if (NETWORK_SELECT == NETWORK_REACTOR)
    reactor_start(port, kvs_protocol);
    
#elif (NETWORK_SELECT == NETWORK_NTYCO)
    ntyco_start(port, kvs_protocol);

#elif (NETWORK_SELECT == NETWORK_PROACTOR)
    proactor_start(port, kvs_protocol);

#endif

    dest_kvengine();

    return 0;
}

