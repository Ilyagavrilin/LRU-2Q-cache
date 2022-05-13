#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
//this define help to allocate place where error was raised
#define POSITION __FILE__, __LINE__, __PRETTY_FUNCTION__
//define SECURITY_LVL for enabling in_function warning
//if you want graphic dump set SECURITY_LVL>1
#define SECURITY_LVL 1
//SECURE_PTR includes check of elements of chain to avoid situation, when one element inserted in chain two times
//#define SECURE_PTR
// define if you want to compile program for linux
//#define LNX
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
//!\brief analog of ChainRemove to remove first(Head) or last(Tail) element of chain
chain_t* ChainRemoveFP(chain_t* root, INS_PLACE remove_place);
//------------------------------------------------------------------------
static int ShowErr(int expr,const char* msg, RESULT res, const char* file, int line, const char* func);
int ChainCheckOccurence(chain_t* root, chain_t* to_check);
void DumpPoints(chain_t *root, FILE *dump_ptr);
FILE* ChainDump(chain_t* root, FILE* dump_ptr);
void ChainISDump(chain_t* root);