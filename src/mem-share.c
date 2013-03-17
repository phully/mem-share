 #include "mem-share.h"

#define MAX_NODE_NUM 10
#define BASE_KEY 75

/* variant to identify channels */
static int g_chn_indx= 0;
/* work model. 0:push modle,1:pull modle. */
static int g_work_model= 0;
/* share memory address */
static void *g_shm_addr[MAX_NODE_NUM]= {0};

static key_t g_shmkey[MAX_NODE_NUM];

static int g_shm_id[MAX_NODE_NUM];

int shm_init(void)
{
    int i=0;

    for(i= 0; i< MAX_NODE_NUM; i++){
        g_shmkey[i]= BASE_KEY+ i;
    }

err:
    return -1;
}
/* add one transfer channel. */
IPC_FD shm_chn_add(int max_buf_size)
{
    void *ret;

    /* check variant */
    if(max_buf_size<= 0){
        printf("max buffer size is not support!\n");
        goto err;
    }

    if(0== g_work_model)
    {
        g_shm_id[g_chn_indx]= shmget(g_shmkey[g_chn_indx], max_buf_size, 0777| IPC_CREATE);
        if(-1== g_shm_id[g_chn_indx]){
            printf("share memory get error!\n");
            goto err;
        }

        ret= shmat(g_shm_id[g_chn_indx], 0, 0);
        if((void *)-1== ret){
            printf("share memory attach address error\n");
            goto err;
        }
        g_shm_addr[g_chn_indx]= ret;
        ((SHARE_BUF_NODE *)g_shm_addr[g_chn_indx])->share_pt= ret+ sizeof(SHARE_BUF_NODE);
    }

    if(1== g_work_model){
        printf("operation is not suppored by this model!\n");
        printf("may try shm_chn_attach please!\n");
        goto err;
    }

    return g_chn_indx++;
err:
    return -1;
}

IPC_FD shm_chn_attach(void)
{
err:
    return -1;
}

int shm_push(IPC_FD mfd, SHARE_BUF_NODE node)
{
    if((mfd< 0)&& (mfd> MAX_NODE_NUM){
        printf("IPC_FD may be not right!\n");
        goto err;
    }
    if((node.share_size<0)|| (node.share_size> ((SHARE_BUF_NODE *)g_shm_addr[mfd])->max_buf_size)){
        printf("node buffer size is too big!\n");
        goto err;
    }

    ((SHARE_BUF_NODE *)g_shm_addr[mfd])->share_size= node.share_size;
    memcpy(((SHARE_BUF_NODE *)g_shm_addr[mfd])->share_pt, node.share_pt, node.share_size);

    return 0;

err:
    return -1;
}

int shm_pull(IPC_FD mfd, SHARE_BUF_NODE node)
{
err:
    return -1;
}
