#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define MAX_MSG_LENGTH  1024
#define TIME_SUB_MS(tv1, tv2)  ((tv1.tv_sec - tv2.tv_sec) * 1000 + (tv1.tv_usec - tv2.tv_usec) / 1000)

int connect_tcpserver(char *ip, int port)
{
    int connfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connfd == -1)
    {
        perror("socket");
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    int ret = connect(connfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    if (ret == -1)
    {
        perror("connect");
    }
    return connfd;
}

int recv_msg(int fd, char *msg, int length)
{
    int ret = recv(fd, msg, length, 0);
    if (ret == -1)
    {
        perror("recv");
        exit(1);
    }
    return ret;
}

int send_msg(int fd, char *msg, int length)
{
    int ret = send(fd, msg, length, 0);
    if (ret == -1)
    {
        perror("send");
        exit(1);
    }
    return ret;
}


void testcase(int fd, char *msg, char *pattern, char *casename)
{
    if (!msg || !pattern || !casename)
    {
        return;
    }
    send_msg(fd, msg, strlen(msg));
    
    char result[MAX_MSG_LENGTH] = {0};
    recv_msg(fd, result, MAX_MSG_LENGTH);

    if (strcmp(result, pattern) == 0)
    {
        //printf("==> PASS -> %s\n", casename);
    }else
    {
        printf("==> FAILED -> %s,'%s' != '%s'\n", casename, result, pattern);
        exit(1);
    }    
}


void array_testcase(int connfd)
{
	testcase(connfd, "SET Teacher King", "OK\r\n", "SET-Teacher");
	testcase(connfd, "GET Teacher", "King\r\n", "GET-Teacher");
	testcase(connfd, "MOD Teacher Darren", "OK\r\n", "MOD-Teacher");
	testcase(connfd, "GET Teacher", "Darren\r\n", "GET-Teacher");
	testcase(connfd, "EXIST Teacher", "EXIST\r\n", "EXIST-Teacher");
	testcase(connfd, "DEL Teacher", "OK\r\n", "DEL-Teacher");
	testcase(connfd, "GET Teacher", "NON-EXIST\r\n", "GET-Teacher");
	testcase(connfd, "MOD Teacher KING", "NON-EXIST\r\n", "MOD-Teacher");
	testcase(connfd, "EXIST Teacher", "NON-EXIST\r\n", "GET-Teacher");
}

void rbtree_testcase(int connfd)
{
	testcase(connfd, "RSET Teacher King", "OK\r\n", "RSET-Teacher");
	testcase(connfd, "RGET Teacher", "King\r\n", "RGET-Teacher");
	testcase(connfd, "RMOD Teacher Darren", "OK\r\n", "RMOD-Teacher");
	testcase(connfd, "RGET Teacher", "Darren\r\n", "RGET-Teacher");
	testcase(connfd, "REXIST Teacher", "EXIST\r\n", "REXIST-Teacher");
	testcase(connfd, "RDEL Teacher", "OK\r\n", "RDEL-Teacher");
	testcase(connfd, "RGET Teacher", "NON-EXIST\r\n", "RGET-Teacher");
	testcase(connfd, "RMOD Teacher KING", "NON-EXIST\r\n", "RMOD-Teacher");
	testcase(connfd, "REXIST Teacher", "NON-EXIST\r\n", "RGET-Teacher");
}

void hash_testcase(int connfd)
{
	testcase(connfd, "HSET Teacher King", "OK\r\n", "HSET-Teacher");
	testcase(connfd, "HGET Teacher", "King\r\n", "HGET-Teacher");
	testcase(connfd, "HMOD Teacher Darren", "OK\r\n", "HMOD-Teacher");
	testcase(connfd, "HGET Teacher", "Darren\r\n", "HGET-Teacher");
	testcase(connfd, "HEXIST Teacher", "EXIST\r\n", "HEXIST-Teacher");
	testcase(connfd, "HDEL Teacher", "OK\r\n", "HDEL-Teacher");
	testcase(connfd, "HGET Teacher", "NON-EXIST\r\n", "HGET-Teacher");
	testcase(connfd, "HMOD Teacher KING", "NON-EXIST\r\n", "HMOD-Teacher");
	testcase(connfd, "HEXIST Teacher", "NON-EXIST\r\n", "HGET-Teacher");
}
void array_testcase_1w(int connfd)
{
    int i = 0;
    int count = 10000;

    struct timeval tv_begin;
    gettimeofday(&tv_begin, NULL);

    for ( i = 0; i < count; i++)
    {
        array_testcase(connfd);
    }
    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    int time_used = TIME_SUB_MS(tv_end, tv_begin);
    printf("array case--> time used:%d, qps:%d\n", time_used, 90000 * 1000/ time_used);
}

void rbtree_testcase_1w(int connfd)
{
    int i = 0;
    int count = 10000;

    struct timeval tv_begin;
    gettimeofday(&tv_begin, NULL);

    for ( i = 0; i < count; i++)
    {
        rbtree_testcase(connfd);
    }
    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    int time_used = TIME_SUB_MS(tv_end, tv_begin);
    printf("rbtree case--> time used:%d, qps:%d\n", time_used, 90000 * 1000/ time_used);
}

void hash_testcase_1w(int connfd)
{
    int i = 0;
    int count = 10000;

    struct timeval tv_begin;
    gettimeofday(&tv_begin, NULL);

    for ( i = 0; i < count; i++)
    {
        hash_testcase(connfd);
    }
    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    int time_used = TIME_SUB_MS(tv_end, tv_begin);
    printf("hash case--> time used:%d, qps:%d\n", time_used, 90000 * 1000/ time_used);
}

void rbtree_testcase_3w_0(int connfd)
{
    int i = 0;
    int count = 10000;

    struct timeval tv_begin;
    gettimeofday(&tv_begin, NULL);

    for ( i = 0; i < count; i++)
    {
        char cmd[128] = {0};
        snprintf(cmd, 128, "RSET Teacher%d King%d", i, i);
        testcase(connfd, cmd, "OK\r\n", "RSET-Teacher");
    }

    for ( i = 0; i < count; i++)
    {
        char cmd[128] = {0};
        snprintf(cmd, 128, "RGET Teacher%d", i);
        char result[128] = {0};
        snprintf(result, 128,"King%d\r\n", i);
        testcase(connfd, cmd, result, "RSET-Teacher");
    }
    for ( i = 0; i < count; i++)
    {
        char cmd[128] = {0};
        snprintf(cmd, 128, "RMOD Teacher%d Darren%d", i, i);
        testcase(connfd, cmd, "OK\r\n", "RSET-Teacher");
    }
    


    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    int time_used = TIME_SUB_MS(tv_end, tv_begin);
    printf("rbtree case--> time used:%d, qps:%d\n", time_used, 30000 * 1000/ time_used);
}


int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("argc must be 4\n");
        exit(EXIT_FAILURE);
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);
    int mode = atoi(argv[3]);

    int connfd = connect_tcpserver(ip, port);
    if (mode == 0)
    {
        //rbtree_testcase(connfd);
        rbtree_testcase_1w(connfd);
    }else if (mode == 1)
    {
        rbtree_testcase_3w_0(connfd);
    }else if (mode == 2)
    {
        //array_testcase(connfd);
        array_testcase_1w(connfd);
    }else if (mode == 3)
    {
        //hash_testcase(connfd);
        hash_testcase_1w(connfd);
    }
    


    return 0;
}

