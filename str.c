/* Dyanamic strings. */
/* #includes */ /*{{{C}}}*//*{{{*/
#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif
/*}}}*/

/* from: http://www.strudel.org.uk/itoa/ */
int strreverse(char* begin, char* end) {
        int count=0;
	char aux;
	while(end>begin){
		aux=*end, *end--=*begin, *begin++=aux;
		count++;
	}
	return count;
}
	
int BAS_itoa(int value, char *string, int base) {
#ifdef HAVE_ITOA
        return itoa(value, string, base);
#else
	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char *wstr=string;
	int sign;
	div_t res;

	// Validate base
	if (base<2 || base>35){ *wstr='\0'; return -1; }

	// Take care of sign
	if ((sign=value) < 0) value = -value;

	// Conversion. Number is reversed.
	do {
		res = div(value,base);
		*wstr++ = num[res.rem];
	}while(value=res.quot);
	if(sign<0) *wstr++='-';
	*wstr='\0';

	// Reverse string
	return strreverse(string,wstr-1);
#endif
}

int BAS_ultostr(unsigned long value, char *string, int base) {
#ifdef HAVE_ULTOSTR
        return ultostr(value, string, base);
#else
	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char *wstr=string;
	unsigned long sign;
	div_t res;

	// Validate base
	if (base<2 || base>35){ *wstr='\0'; return -1; }

	// Take care of sign
	if ((sign=value) < 0) value = -value;

	// Conversion. Number is reversed.
	do {
		res = div(value,base);
		*wstr++ = num[res.rem];
	}while(value=res.quot);
	if(sign<0) *wstr++='-';
	*wstr='\0';

	// Reverse string
	return strreverse(string,wstr-1);
#endif
}

int cistrcmp(const char *s, const char *r) /*{{{*/
{
  assert(s!=(char*)0);
  assert(r!=(char*)0);
  while (*s && tolower(*s)==tolower(*r)) { ++s; ++r; };
  return ((tolower(*s)-tolower(*r)));
}
/*}}}*/

struct String *String_new(struct String *this) /*{{{*/
{
  assert(this!=(struct String*)0);
  this->length=0;
  this->character=(char*)0;
  this->field=(struct StringField*)0;
  return this;
}
/*}}}*/
void String_destroy(struct String *this) /*{{{*/
{
  assert(this!=(struct String*)0);
  if (this->field) String_leaveField(this);
  if (this->length) free(this->character);
}
/*}}}*/
int String_joinField(struct String *this, struct StringField *field, char *character, size_t length) /*{{{*/
{
  struct String **n;

  assert(this!=(struct String*)0);
  if (this->field) String_leaveField(this);
  this->field=field;
  if ((n=(struct String**)realloc(field->refStrings,sizeof(struct String*)*(field->refCount+1)))==(struct String**)0) return -1;
  field->refStrings=n;
  field->refStrings[field->refCount]=this;
  ++field->refCount;
  if (this->length) free(this->character);
  this->character=character;
  this->length=length;
  return 0;
}
/*}}}*/
void String_leaveField(struct String *this) /*{{{*/
{
  struct StringField *field;
  int i;
  struct String **ref;

  assert(this!=(struct String*)0);
  field=this->field;
  assert(field!=(struct StringField*)0);
  for (i=0,ref=field->refStrings; i<field->refCount; ++i,++ref)
  {
    if (*ref==this)
    {
      int further=--field->refCount-i;

      if (further) memmove(ref,ref+1,further*sizeof(struct String**));
      this->character=(char*)0;
      this->length=0;
      this->field=(struct StringField*)0;
      return;
    }
  }
  assert(0);
}
/*}}}*/
struct String *String_clone(struct String *this, const struct String *original) /*{{{*/
{
  assert(this!=(struct String*)0);
  String_new(this);
  String_appendString(this,original);
  return this;
}
/*}}}*/
int String_size(struct String *this, size_t length) /*{{{*/
{
  char *n;

  assert(this!=(struct String*)0);
  if (this->field) String_leaveField(this);
  if (length)
  {
    if (length>this->length)
    {
      if ((n=realloc(this->character,length+1))==(char*)0) return -1;
      this->character=n;
    }
    this->character[length]='\0';
  }
  else
  {
    if (this->length) free(this->character);
    this->character=(char*)0;
  }
  this->length=length;
  return 0;
}
/*}}}*/
int String_appendString(struct String *this, const struct String *app) /*{{{*/
{
  size_t oldlength=this->length;

  if (this->field) String_leaveField(this);
  if (app->length==0) return 0;
  if (String_size(this,this->length+app->length)==-1) return -1;
  memcpy(this->character+oldlength,app->character,app->length);
  return 0;
}
/*}}}*/
int String_appendChar(struct String *this, char ch) /*{{{*/
{
  size_t oldlength=this->length;

  if (this->field) String_leaveField(this);
  if (String_size(this,this->length+1)==-1) return -1;
  this->character[oldlength]=ch;
  return 0;
}
/*}}}*/
int String_appendChars(struct String *this, const char *ch) /*{{{*/
{
  size_t oldlength=this->length;
  size_t chlen=strlen(ch);

  if (this->field) String_leaveField(this);
  if (String_size(this,this->length+chlen)==-1) return -1;
  memcpy(this->character+oldlength,ch,chlen);
  return 0;
}
/*}}}*/
int String_appendPrintf(struct String *this, const char *fmt, ...) /*{{{*/
{
  char buf[1024];
  size_t l,j;
  va_list ap;

  if (this->field) String_leaveField(this);
  va_start(ap, fmt);
  l=vsprintf(buf,fmt,ap);
  va_end(ap);
  j=this->length;
  if (String_size(this,j+l)==-1) return -1;
  memcpy(this->character+j,buf,l);
  return 0;
}
/*}}}*/
int String_appendBinary(struct String *this, const char *fmt, long int arg_li) /*{{{*/
{
  char string[1024];
  char buf[1024];
  size_t l,ll,j;

  if (this->field) String_leaveField(this);
  ll=BAS_ultostr((unsigned long)arg_li,(char *)(string),2);
  l=sprintf(buf,fmt,(char *)(string));
  j=this->length;
  if (String_size(this,j+l)==-1) return -1;
  memcpy(this->character+j,buf,l);
  return 0;
}
/*}}}*/
int String_insertChar(struct String *this, size_t where, char ch) /*{{{*/
{
  size_t oldlength=this->length;

  if (this->field) String_leaveField(this);
  assert(where<oldlength);
  if (String_size(this,this->length+1)==-1) return -1;
  memmove(this->character+where+1,this->character+where,oldlength-where);
  this->character[where]=ch;
  return 0;
}
/*}}}*/
int String_delete(struct String *this, size_t where, size_t len) /*{{{*/
{
  size_t oldlength=this->length;

  if (this->field) String_leaveField(this);
  assert(where<oldlength);
  assert(len>0);
  if ((where+len)<oldlength) memmove(this->character+where,this->character+where+len,oldlength-where-len);
  this->character[this->length-=len]='\0';
  return 0;
}
/*}}}*/
void String_ucase(struct String *this) /*{{{*/
{
  size_t i;

  for (i=0; i<this->length; ++i) this->character[i]=toupper(this->character[i]);
}
/*}}}*/
void String_lcase(struct String *this) /*{{{*/
{
  size_t i;

  for (i=0; i<this->length; ++i) this->character[i]=tolower(this->character[i]);
}
/*}}}*/
int String_cmp(const struct String *this, const struct String *s) /*{{{*/
{
  size_t pos;
  int res;
  const char *thisch,*sch;

  for (pos=0,thisch=this->character,sch=s->character; pos<this->length && pos<s->length; ++pos,++thisch,++sch)
  {
    if ((res=(*thisch-*sch))) return res;
  }
  return (this->length-s->length);
}
/*}}}*/
void String_lset(struct String *this, const struct String *s) /*{{{*/
{
  size_t copy;

  copy=(this->length<s->length ? this->length : s->length);
  if (copy) memcpy(this->character,s->character,copy);
  if (copy<this->length) memset(this->character+copy,' ',this->length-copy);
}
/*}}}*/
void String_rset(struct String *this, const struct String *s) /*{{{*/
{
  size_t copy;

  copy=(this->length<s->length ? this->length : s->length);
  if (copy) memcpy(this->character+this->length-copy,s->character,copy);
  if (copy<this->length) memset(this->character,' ',this->length-copy);
}
/*}}}*/
void String_set(struct String *this, size_t pos, const struct String *s, size_t length) /*{{{*/
{
  if (this->length>=pos)
  {
    if (this->length<(pos+length)) length=this->length-pos;
    if (length) memcpy(this->character+pos,s->character,length);
  }
}
/*}}}*/

struct StringField *StringField_new(struct StringField *this) /*{{{*/
{
  this->refStrings=(struct String**)0;
  this->refCount=0;
  return this;
}
/*}}}*/
void StringField_destroy(struct StringField *this) /*{{{*/
{
  int i;

  for (i=this->refCount; i>0; ) String_leaveField(this->refStrings[--i]);
  this->refCount=-1;
  free(this->refStrings);
}
/*}}}*/
