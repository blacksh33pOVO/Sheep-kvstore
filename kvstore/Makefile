CC:=gcc
TARGET = kvstore
SRCS = proactor.c reactor.c ntyco.c kv_store.c kvs_array.c kvs_rbtree.c kvs_hash.c
INC = -I ../NtyCo-master/core/
LIBS = -L ../NtyCo-master -lntyco -luring
#kvs_reactor: kvs_reactor.c kv_store.c
#	-$(CC) -o $@ $^
#ntyco 的KV
#ntyco: ntyco.c kv_store.c
#	-$(CC) -o $@ $^  -I ../NtyCo-master/core/ -L ../NtyCo-master -lntyco
#reactor的KV
#kvs_store: reactor.c kv_store.c
#	-$(CC) -o $@ $^
#proactor的KV
#proactor: proactor.c kv_store.c
#	-$(CC) -o $@ $^ -luring
#三个一起编译
all:
	$(CC) -o $(TARGET) $(SRCS) $(INC) $(LIBS)

#kvs_store: proactor.c reactor.c ntyco.c kv_store.c kvs_array.c
#	-$(CC) -o $@ $^ -I ../NtyCo-master/core/ -L ../NtyCo-master -lntyco -luring

clean:
	rm -rf kvstore
