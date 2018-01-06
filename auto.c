/* Local variables and the run time stack. */
/* #includes */ /*{{{C}}}*//*{{{*/
#undef  _POSIX_SOURCE
#define _POSIX_SOURCE   1
#undef  _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 2

#include "config.h"

#include <assert.h>
#include <ctype.h>
#ifdef HAVE_GETTEXT
#include <libintl.h>
#define _(String) gettext(String)
#else
#define _(String) String
#endif
#include <stdlib.h>
#include <string.h>

#include "auto.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif
/*}}}*/
/* #defines */ /*{{{*/
#define INCREASE_STACK 16
/*}}}*/

/* interpretation methods */
struct Auto *Auto_new(struct Auto *this) /*{{{*/
{
  this->stackPointer=0;
  this->stackCapacity=0;
  this->framePointer=0;
  this->frameSize=0;
  this->onerror.line=-1;
  this->erl=0;
  Value_new_NIL(&this->err);
  Value_new_NIL(&this->lastdet);
  this->begindata.line=-1;
  this->slot=(union AutoSlot*)0;
  this->cur=this->all=(struct Symbol*)0;
  return this;
}
/*}}}*/
void Auto_destroy(struct Auto *this) /*{{{*/
{
  struct Symbol *l;

  Value_destroy(&this->err);
  Value_destroy(&this->lastdet);
  if (this->stackCapacity) free(this->slot);
  for (l=this->all; l!=(struct Symbol*)0; )
  {
    struct Symbol *f;

    f=l;
    l=l->next;
    free(f->name);
    free(f);
  }
}
/*}}}*/
struct Var *Auto_pushArg(struct Auto *this) /*{{{*/
{
  if ((this->stackPointer+1)>=this->stackCapacity)
  {
    this->slot=realloc(this->slot,sizeof(this->slot[0])*(this->stackCapacity?(this->stackCapacity=this->stackPointer+INCREASE_STACK):(this->stackCapacity=INCREASE_STACK)));
  }
  return &this->slot[this->stackPointer++].var;
}
/*}}}*/
void Auto_pushFuncRet(struct Auto *this, int firstarg, struct Pc *pc) /*{{{*/
{
  if (this->stackPointer+2>=this->stackCapacity)
  {
    this->slot=realloc(this->slot,sizeof(this->slot[0])*(this->stackCapacity?(this->stackCapacity=this->stackCapacity+INCREASE_STACK):(this->stackCapacity=INCREASE_STACK)));
  }
  this->slot[this->stackPointer].retException.onerror=this->onerror;
  this->slot[this->stackPointer].retException.resumeable=this->resumeable;
  ++this->stackPointer;
  this->slot[this->stackPointer].retFrame.pc=*pc;
  this->slot[this->stackPointer].retFrame.framePointer=this->framePointer;
  this->slot[this->stackPointer].retFrame.frameSize=this->frameSize;
  ++this->stackPointer;
  this->framePointer=firstarg;
  this->frameSize=this->stackPointer-firstarg;
  this->onerror.line=-1;
}
/*}}}*/
void Auto_pushGosubRet(struct Auto *this, struct Pc *pc) /*{{{*/
{
  if ((this->stackPointer+1)>=this->stackCapacity)
  {
    this->slot=realloc(this->slot,sizeof(this->slot[0])*(this->stackCapacity?(this->stackCapacity=this->stackPointer+INCREASE_STACK):(this->stackCapacity=INCREASE_STACK)));
  }
  this->slot[this->stackPointer].retFrame.pc=*pc;
  ++this->stackPointer;
}
/*}}}*/
struct Var *Auto_local(struct Auto *this, int l) /*{{{*/
{
  assert(this->frameSize>(l+2));
  return &(this->slot[this->framePointer+l].var);
}
/*}}}*/
int Auto_funcReturn(struct Auto *this, struct Pc *pc) /*{{{*/
{
  int i,retFrame,retException;

  if (this->stackPointer==0) return 0;
  assert(this->frameSize);
  retFrame=this->framePointer+this->frameSize-1;
  retException=this->framePointer+this->frameSize-2;
  assert(retException>=0 && retFrame<this->stackPointer);
  for (i=0; i<this->frameSize-2; ++i) Var_destroy(&this->slot[this->framePointer+i].var);
  this->stackPointer=this->framePointer;
  if (pc!=(struct Pc*)0) *pc=this->slot[retFrame].retFrame.pc;
  this->frameSize=this->slot[retFrame].retFrame.frameSize;
  this->framePointer=this->slot[retFrame].retFrame.framePointer;
  this->onerror=this->slot[retException].retException.onerror;
  return 1;
}
/*}}}*/
int Auto_gosubReturn(struct Auto *this, struct Pc *pc) /*{{{*/
{
  if (this->stackPointer<=this->framePointer+this->frameSize) return 0;
  --this->stackPointer;
  if (pc) *pc=this->slot[this->stackPointer].retFrame.pc;
  return 1;
}
/*}}}*/
void Auto_frameToError(struct Auto *this, struct Program *program, struct Value *v) /*{{{*/
{
  int i=this->stackPointer,framePointer,frameSize,retFrame;
  struct Pc p;

  framePointer=this->framePointer;
  frameSize=this->frameSize;
  while (i>framePointer+frameSize)
  {
    p=this->slot[--i].retFrame.pc;
    Value_errorSuffix(v,_("Called"));
    Program_PCtoError(program,&p,v);
  }
  if (i)
  {
    retFrame=framePointer+frameSize-1;
    i=framePointer;
    p=this->slot[retFrame].retFrame.pc;
    frameSize=this->slot[retFrame].retFrame.frameSize;
    framePointer=this->slot[retFrame].retFrame.framePointer;
    Value_errorSuffix(v,_("Proc Called"));
    Program_PCtoError(program,&p,v);
  }
}
/*}}}*/
void Auto_setError(struct Auto *this, long int line, struct Pc *pc, struct Value *v) /*{{{*/
{
  this->erpc=*pc;
  this->erl=line;
  Value_destroy(&this->err);
  Value_clone(&this->err,v);
}
/*}}}*/

/* compilation methods */
int Auto_find(struct Auto *this, struct Identifier *ident) /*{{{*/
{
  struct Symbol *find;

  for (find=this->cur; find!=(struct Symbol*)0; find=find->next)
  {
    const char *s=ident->name;
    const char *r=find->name;

    while (*s && tolower(*s)==tolower(*r)) { ++s; ++r; };
    if (tolower(*s)==tolower(*r))
    {
      ident->sym=find;
      return 1;
    }
  }
  return 0;
}
/*}}}*/
int Auto_variable(struct Auto *this, const struct Identifier *ident) /*{{{*/
{
  struct Symbol **tail;
  int offset;

  for (offset=0,tail=&this->cur; *tail!=(struct Symbol*)0; tail=&(*tail)->next,++offset)
  {
    const char *s=ident->name;
    const char *r=(*tail)->name;

    while (*s && tolower(*s)==tolower(*r)) { ++s; ++r; };
    if (tolower(*s)==tolower(*r)) return 0;
  }
  (*tail)=malloc(sizeof(struct Symbol));
  (*tail)->next=(struct Symbol*)0;
  (*tail)->name=strcpy(malloc(strlen(ident->name)+1),ident->name);
  (*tail)->type=LOCALVAR;
  (*tail)->u.local.type=ident->defaultType;
  /* the offset -1 of the V_VOID procedure return symbol is ok, it is not used */
  (*tail)->u.local.offset=offset-(this->cur->u.local.type==V_VOID?1:0);
  return 1;
}
/*}}}*/
enum ValueType Auto_argType(const struct Auto *this, int l) /*{{{*/
{
  struct Symbol *find;
  int offset;

  if (this->cur->u.local.type==V_VOID) ++l;
  for (offset=0,find=this->cur; l!=offset; find=find->next,++offset) assert(find!=(struct Symbol*)0);
  assert(find!=(struct Symbol*)0);
  return find->u.local.type;
}
/*}}}*/
enum ValueType Auto_varType(const struct Auto *this, struct Symbol *sym) /*{{{*/
{
  struct Symbol *find;

  for (find=this->cur; find->u.local.offset!=sym->u.local.offset; find=find->next) assert(find!=(struct Symbol*)0);
  assert(find!=(struct Symbol*)0);
  return find->u.local.type;
}
/*}}}*/
void Auto_funcEnd(struct Auto *this) /*{{{*/
{
  struct Symbol **tail;

  for (tail=&this->all; *tail!=(struct Symbol*)0; tail=&(*tail)->next);
  *tail=this->cur;
  this->cur=(struct Symbol*)0;
}
/*}}}*/
