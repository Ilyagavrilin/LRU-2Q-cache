#include "queues.h"

static int ShowErr(int expr, const char *msg, RESULT res, const char *file, int line, const char *func) {
#if SECURITY_LVL > 0
  if (expr > 0) { return 1; }
  fprintf(stderr, "Error in %s in line (%d), function %s: %s\n", file, line, func, msg);
  if (res == EXIT) {
    exit(1);
  }
  return 0;
#else
  return 1;
#endif
}

chain_t *ChainCtr(CHAIN_NAME name, int max_cnt) {
  ShowErr(max_cnt >= 0, "invalid chain size", EXIT, POSITION);
  chain_t *res = NULL;

  res = (chain_t *) malloc(sizeof(chain_t));
  ShowErr(res != NULL, "malloc doesn`t return a buffer", EXIT, POSITION);

  res->name = name;
  res->type = ROOT;
  res->cur_cnt = 0;
  res->max_cnt = max_cnt;
  res->next = res;
  res->prev = res;

  return res;
}

void ChainDtr(chain_t *root) {
  int cnt = 0;
  ShowErr(root != NULL, "invalid root pointer", EXIT, POSITION);
  ShowErr(root->type == ROOT, "invalid cell type", EXIT, POSITION);
#if SECURITY_LVL > 1
  ChainISDump(root);
#endif
  chain_t *to_delete = root->next;
  chain_t *tmp;
  while (to_delete != NULL && to_delete->type != ROOT) {
    tmp = to_delete;
    to_delete = to_delete->next;
    free(tmp);
    cnt++;
  }

  ShowErr(cnt == root->cur_cnt, "invalid current count value", NOT_EXIT, POSITION);
  free(root);

}

chain_t *ChainInsert(chain_t *root, chain_t *to_insert, INS_PLACE place) {
  ShowErr(root != NULL && root->type == ROOT, "invalid type or pointer to \'root\' cell", EXIT, POSITION);
  ShowErr(to_insert != NULL & to_insert->type != ROOT, "invalid type or pointer of inserting cell", EXIT, POSITION);
  ShowErr(root->name == to_insert->name, "elements belong different chains", EXIT, POSITION);
#ifdef SECURE_PTR
  if (!ShowErr(ChainCheckOccurence(root, to_insert) == 0, "this node already in queue", EXIT,
               POSITION)) {
    return NULL;
  }
#endif
#if SECURITY_LVL > 1
  ChainISDump(root);
#endif
  if (root->cur_cnt + 1 > root->max_cnt) {
    return NULL;
  }

  chain_t *tmp;
  switch (place) {
    case HEAD:
      tmp = root->next;
      root->next = to_insert;
      tmp->prev = to_insert;
      to_insert->next = tmp;
      to_insert->prev = root;
      break;
    case TAIL:
      tmp = root->prev;
      root->prev = to_insert;
      tmp->next = to_insert;
      to_insert->next = root;
      to_insert->prev = tmp;
      break;
    default:
      ShowErr(0, "Undefined place value", EXIT, POSITION);
  }
  root->cur_cnt++;
  return root;
}

chain_t *ChainSafeInsert(chain_t *root, long ins_value, INS_PLACE place) {
  chain_t *to_insert = ChainMakeNode(ins_value, root->name);
  return ChainInsert(root, to_insert, place);
}

chain_t *ChainMakeNode(long value, CHAIN_NAME name) {
  chain_t *res;
  res = malloc(sizeof(chain_t));
  ShowErr(res != NULL, "memory allocation error", EXIT, POSITION);
  res->next = NULL;
  res->prev = NULL;
  res->name = name;
  res->type = NOT_ROOT;
  res->value = value;

  return res;
}

chain_t *ChainRemove(chain_t *root, chain_t *to_remove) {
  ShowErr(root != NULL && to_remove != NULL, "one of inserted values is null pointed", EXIT, POSITION);
  ShowErr(root->type == ROOT && to_remove->type == NOT_ROOT, "incorrect type of inserted values", EXIT, POSITION);
#ifdef SECURE_PTR
  if (!ShowErr(ChainCheckOccurence(root, to_remove), "this node didn`t found in queue", EXIT,
               POSITION)) {
    return NULL;
  }
#endif
#if SECURITY_LVL > 1
  ChainISDump(root);
#endif
  if (root->cur_cnt == 0) {
    return NULL; // we have nothing to remove
  }
  chain_t *prev = to_remove->prev;
  chain_t *next = to_remove->next;
  prev->next = next;
  next->prev = prev;
  root->cur_cnt--;
  return to_remove;
}

chain_t* ChainRemoveFP(chain_t* root, INS_PLACE remove_place){
  ShowErr(root->type == ROOT, "incorrect type of root", NOT_EXIT, POSITION);
  if (root->cur_cnt < 1) {
    ShowErr(root->cur_cnt >= 1, "nothing to remove", NOT_EXIT, POSITION);
    return NULL;
  }

  switch (remove_place) {
    case HEAD:
      return ChainRemove(root, root->next);
    case TAIL:
      return ChainRemove(root, root->prev);
    default:
      return NULL;
  }
}

int ChainCheckOccurence(chain_t *root, chain_t *to_check) {
  chain_t *curr = root->next;
  while (curr != root) {
    if (curr == to_check) {
      return 1;
    }
    curr = curr->next;
  }
  return 0;
}

void DumpPoints(chain_t *root, FILE *dump_ptr) {
  fprintf(dump_ptr, "P%p[shape=record, label=\"prev=0x%p |pointer=0x%p\\n cur_count=%d \\n max_count=%d |next=0x%p \"];\n", root, root->prev, root,
          root->cur_cnt, root->max_cnt, root->next);
  chain_t* curr = root->next;
  while (curr != root) {
    fprintf(dump_ptr, "P%p[shape=record, label=\"prev=0x%p |pointer=0x%p\\n value=%ld|next=0x%p \"];\n", curr, curr->prev, curr,
            curr->value, curr->next);
    curr = curr->next;
  }
}

void DumpEdges(chain_t* root, FILE* dump_ptr) {
  fprintf(dump_ptr, "P%p -> P%p;\n", root, root->next);
  fprintf(dump_ptr, "P%p -> P%p;\n", root, root->prev);
  chain_t* curr = root->next;
  while (curr != root) {
    fprintf(dump_ptr, "P%p -> P%p;\n", curr, curr->next);
    fprintf(dump_ptr, "P%p -> P%p;\n", curr, curr->prev);
    curr = curr->next;
  }
}

FILE *ChainDump(chain_t *root, FILE *dump_ptr) {
  if (dump_ptr == NULL) {
    dump_ptr = fopen("output.gv", "w");
  }
  ShowErr(dump_ptr != NULL, "can`t create a file to dump", NOT_EXIT, POSITION);
  if (dump_ptr == NULL) {
    return NULL;
  }

  fprintf(dump_ptr, "digraph G{\ncomment=\"Hello world\"\n");
  DumpPoints(root, dump_ptr);
  DumpEdges(root, dump_ptr);
  fprintf(dump_ptr, "}");
  return dump_ptr;
}

void ChainISDump(chain_t* root) {
  if (root == NULL) {
    return;
  }

  char* fname;
  clock_t t = time(NULL);
  struct tm* aTm = localtime(&t);
  char filename[30];
  sprintf(filename, "dump_%02dh_%02dm_%02ds.gv", aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
  FILE* file = fopen(filename, "w");
  if (file == NULL) {
    ShowErr(file != NULL, "no dump avaliable", NOT_EXIT, POSITION);
    return;
  }

  ChainDump(root, file);
  fclose(file);
#ifdef LNX
  char command[50];
  sprintf(command, "dot -Tsvg -O %s", filename);
  system(command);
#endif
}