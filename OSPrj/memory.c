/*
 * @Description: 
 * @Author: Cuibb
 * @Date: 2021-12-08 21:39:33
 * @LastEditTime: 2021-12-09 02:08:14
 * @LastEditors: Cuibb
 */

#include "memory.h"
#include "utility.h"
#include "list.h"

#ifdef MEM_DEBUG
#include <stdio.h>
#include <time.h>
#endif

#define FM_ALLOC_SIZE          32
#define FM_NODE_SIZE           sizeof(FMemNode)

#define VM_HEAD_SIZE           sizeof(VMemHead)

typedef byte(FMemUnit)[FM_ALLOC_SIZE];
typedef union _FMemNode FMemNode;

union _FMemNode
{
    FMemNode* next;
    FMemUnit* ptr;
};

typedef struct
{
    FMemNode* node;
    FMemNode* nbase;
    FMemUnit* ubase;
    uint max; 
}FMemList;

typedef struct 
{
    ListNode head;
    uint used;
    uint free;
    byte* ptr;
}VMemHead;

static FMemList gFMList = {0};
static List gVMList = {0};

static void FMemInit(byte* mem, uint size)
{
    FMemNode* pn = NULL;
    uint max = 0;
    int i = 0;

    CHECK_PTR(mem);

    max = size / (FM_ALLOC_SIZE + FM_NODE_SIZE);
    gFMList.max = max;
    gFMList.node = (FMemNode*)mem;
    gFMList.nbase = (FMemNode*)mem;
    gFMList.ubase = (FMemUnit*)AddrOff(mem, FM_NODE_SIZE * max);

    pn = gFMList.node;
    for(i = 0; i < (max -1); i++) {
        FMemNode* current = (FMemNode*)AddrOff(pn, i);
        FMemNode* next = (FMemNode*)AddrOff(pn, i+1);
        current->next = next;
    }
    ((FMemNode*)AddrOff(pn, i))->next = NULL;;
}

static void* FMemAlloc()
{
    void* ret = NULL;

    if ( gFMList.node ) {
        FMemNode* alloc = gFMList.node;
        int index = (int)AddrIndex(alloc, gFMList.nbase);

        ret = AddrOff(gFMList.ubase, index);

        gFMList.node = alloc->next;
        alloc->ptr = ret;
    }

    return ret;
}

static int FMemFree(void* ptr)
{
    int ret = 0;

    if ( ptr ) {
        int index = AddrIndex((FMemUnit*)ptr, gFMList.ubase);
        FMemNode* pn = AddrOff(gFMList.nbase, index);

        if ( (index < gFMList.max) && IsEqual(ptr, pn->ptr) ) {
            pn->next = gFMList.node;
            gFMList.node = pn;
            ret = 1;
        }
    }

    return ret;
}

static void VMemInit(byte* mem, uint size)
{
    VMemHead* head = (VMemHead*)mem;
    int i = 0;

    CHECK_PTR(head);

    List_Init(&gVMList);
    head->used = 0;
    head->free = size - VM_HEAD_SIZE;
    head->ptr = AddrOff(head, 1);

    List_AddTail(&gVMList, (ListNode*)head);
}

static void* VMemAlloc(uint size)
{
    ListNode* pn = NULL;
    VMemHead* ret = NULL;
    uint alloc = size + VM_HEAD_SIZE;

    List_ForEach(&gVMList, pn) {
        VMemHead* current = (VMemHead*)pn;

        if ( current->free >= alloc ) {
            ret = (VMemHead*)((uint)current->ptr + (current->used + current->free) - alloc);
            ret->used = size;
            ret->free = 0;
            ret->ptr = AddrOff(ret, 1);

            current->free -= alloc;

            List_AddAfter((ListNode*)current, (ListNode*)ret);

            break;
        }
    }

    return ret ? ret->ptr : NULL;
}

static int VMemFree(void* ptr)
{
    int ret = 0;

    if ( ptr ) {
        ListNode* pn = NULL;

        List_ForEach(&gVMList, pn) {
            VMemHead* current = (VMemHead*)pn;

            if ( IsEqual(current->ptr, ptr) ) {
                VMemHead* prev = (VMemHead*)(current->head.prev);
                prev->free += current->used + current->free + VM_HEAD_SIZE;

                List_DelNode((ListNode*)current);
                
                ret = 1;
                break;
            }
        }
    }

    return ret;
}

void MemModInit(byte* mem, uint size)
{
    byte* fmem = NULL;
    byte* vmem = NULL;
    uint fsize = size / 2;
    uint vsize = size - fsize;

    CHECK_PTR(mem);

    fmem = mem;
    vmem = AddrOff(mem, fsize);

    FMemInit(fmem, fsize);
    VMemInit(vmem, vsize);
}

void* Malloc(uint size)
{
    void* ret = NULL;

    if ( size <= FM_ALLOC_SIZE ) {
        ret = FMemAlloc(size);
    }

    if ( !ret ) {
        ret = VMemAlloc(size);
    }

    return ret;
}
void Free(void* ptr)
{
    if ( !FMemFree(ptr) ) {
        VMemFree(ptr);
    }
}

#ifdef MEM_DEBUG
void FMemTest()
{
    static byte fmem[0x10000] = {0};
    static void* array[2000] = {0};
    int i = 0;

    FMemNode* pn = NULL;
    
    FMemInit(fmem, sizeof(fmem));

    pn = gFMList.node;

    while ( pn ) {
        i++;
        pn = pn->next;
    }
    printf("i = %d\n", i);

    for (i = 0; i < 100000; i++) {
        int ii = i % 2000;
        byte* p = FMemAlloc();

        if ( array[ii] ) {
            FMemFree(array[ii]);
            array[ii] = NULL;
        }
        array[ii] = p;

        if ( (i % 3) == 0 ) {
            int index = rand() % 2000;
            FMemFree(array[index]);
            array[index] = NULL;
        }
    }

    for (i = 0; i < 2000; i++) {
        FMemFree(array[i]);
        array[i] = NULL;
    }

    i = 0;
    pn = gFMList.node;
    while ( pn ) {
        i++;
        pn = pn->next;
    }
    printf("i = %d\n", i);
}

void VMemTest()
{
    static byte vmem[0x10000] = {0};
    static void* array[2000] = {0};
    ListNode* pn = NULL;
    int i = 0;

    srand(time(NULL));

    VMemInit(vmem, sizeof(vmem));

    List_ForEach(&gVMList, pn) {
        VMemHead* current = (VMemHead*)pn;

        i++;
        printf("i = %d\n", i);
        printf("used: %d\n", current->used);
        printf("free: %d\n", current->free);
        printf("\n");
    }

    for (i = 0; i < 100000; i++) {
        int ii = i % 2000;
        byte* p = VMemAlloc(1 + rand() % 400);

        if ( array[ii] ) {
            VMemFree(array[ii]);
            array[ii] = NULL;
        }
        array[ii] = p;

        if ( (i % 3) == 0 ) {
            int index = rand() % 2000;
            VMemFree(array[index]);
            array[index] = NULL;
        }
    }

    for (i = 0; i < 2000; i++) {
        VMemFree(array[i]);
        array[i] = NULL;
    }

    i = 0;
    List_ForEach(&gVMList, pn) {
        VMemHead* current = (VMemHead*)pn;

        i++;
        printf("i = %d\n", i);
        printf("used: %d\n", current->used);
        printf("free: %d\n", current->free);
        printf("\n");
    }
}
#endif

