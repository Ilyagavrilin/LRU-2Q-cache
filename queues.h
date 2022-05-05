#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define POSITION __FILE__, __LINE__, __PRETTY_FUNCTION__
#define SECURITY_LVL 1
#define SECURE_PTR

typedef enum {
  EXIT,
  NOT_EXIT,
} RESULT;

typedef enum {
  ROOT,
  NOT_ROOT,
} IS_ROOT;

typedef enum {
  FIFO_IN,
  FIFO_OUT,
  LRU,
} CHAIN_NAME;

typedef enum {
  HEAD,
  TAIL,
} INS_PLACE;

typedef struct chain_t_ {
  CHAIN_NAME name;
  IS_ROOT type;
  union {
    long value; //use long instead of int because struct uses two 4 bytes (integer) fields
    struct {
      int max_cnt;
      int cur_cnt;
    };
  };

  struct chain_t_* next;
  struct chain_t_* prev;
} chain_t;
//!
//! \param name name of selected queue (CHAIN_NAME - enum, defined in queues.h)
//! \param max_cnt  size of selected queue
//! \return pointer to root of created queue
chain_t* ChainCtr(CHAIN_NAME name, int max_cnt);
//!
//! \param root pointer to root of queue for free
void ChainDtr(chain_t* root);
//!
//! \param value value which will be placed in node
//! \param name name of selected queue (CHAIN_NAME - enum, defined in queues.h)
//! \return pointer to allocated node
chain_t* ChainMakeNode(long value, CHAIN_NAME name);
//!
//! \warning you can add the same node many times (if you not sure define SECURE_PTR 1 - enable pointer checking, loose speed)
//! \param root pointer too root of selected queue
//! \param to_insert pointer to allocated cell
//! \param place HEAD or TAIL, respectively root`s next and prev pointer
//! \return pointer to inserted cell, or NULL in cause of error
chain_t* ChainInsert (chain_t* root, chain_t* to_insert, INS_PLACE place);
//!
//! \brief Safe version of ChainInsert, generate node in function
chain_t* ChainSafeInsert(chain_t* root, long ins_value, INS_PLACE place);
//!
//! \param root pointer too root of selected queue
//! \param to_remove pointer to removing node
//! \return pointer to root or NULL in cause of error or queue`s mosmatch
chain_t* ChainRemove(chain_t* root, chain_t* to_remove);
static int ShowErr(int expr,const char* msg, RESULT res, const char* file, int line, const char* func);
int ChainCheckOccurence(chain_t* root, chain_t* to_check);
void DumpPoints(chain_t *root, FILE *dump_ptr);
FILE* ChainDump(chain_t* root, FILE* dump_ptr);