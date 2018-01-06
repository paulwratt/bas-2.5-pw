#ifndef STATEMENT_H
#define STATEMENT_H

extern struct Value *stmt_CALL(struct Value *value);
extern struct Value *stmt_CASE(struct Value *value);
extern struct Value *stmt_CHDIR_MKDIR(struct Value *value);
extern struct Value *stmt_CLEAR(struct Value *value);
extern struct Value *stmt_CLOSE(struct Value *value);
extern struct Value *stmt_CLS(struct Value *value);
extern struct Value *stmt_COLOR(struct Value *value);
extern struct Value *stmt_DATA(struct Value *value);
extern struct Value *stmt_DEFFN_DEFPROC_FUNCTION_SUB(struct Value *value);
extern struct Value *stmt_DEC_INC(struct Value *value);
extern struct Value *stmt_DEFINT_DEFDBL_DEFSTR(struct Value *value);
extern struct Value *stmt_DELETE(struct Value *value);
extern struct Value *stmt_DIM(struct Value *value);
extern struct Value *stmt_DISPLAY(struct Value *value);
extern struct Value *stmt_DO(struct Value *value);
extern struct Value *stmt_DOcondition(struct Value *value);
extern struct Value *stmt_EDIT(struct Value *value);
extern struct Value *stmt_ELSE_ELSEIFELSE(struct Value *value);
extern struct Value *stmt_END(struct Value *value);
extern struct Value *stmt_ENDIF(struct Value *value);
extern struct Value *stmt_ENDFN(struct Value *value);
extern struct Value *stmt_ENDPROC_SUBEND(struct Value *value);
extern struct Value *stmt_ENDSELECT(struct Value *value);
extern struct Value *stmt_ENVIRON(struct Value *value);
extern struct Value *stmt_FNEXIT(struct Value *value);
extern struct Value *stmt_COLON_EOL(struct Value *value);
extern struct Value *stmt_QUOTE_REM(struct Value *value);
extern struct Value *stmt_EQ_FNRETURN_FNEND(struct Value *value);
extern struct Value *stmt_ERASE(struct Value *value);
extern struct Value *stmt_EXITDO(struct Value *value);
extern struct Value *stmt_EXITFOR(struct Value *value);
extern struct Value *stmt_FIELD(struct Value *value);
extern struct Value *stmt_FOR(struct Value *value);
extern struct Value *stmt_GET_PUT(struct Value *value);
extern struct Value *stmt_GOSUB(struct Value *value);
extern struct Value *stmt_RESUME_GOTO(struct Value *value);
extern struct Value *stmt_KILL(struct Value *value);
extern struct Value *stmt_LET(struct Value *value);
extern struct Value *stmt_LINEINPUT(struct Value *value);
extern struct Value *stmt_LIST_LLIST(struct Value *value);
extern struct Value *stmt_LOAD(struct Value *value);
extern struct Value *stmt_LOCAL(struct Value *value);
extern struct Value *stmt_LOCATE(struct Value *value);
extern struct Value *stmt_LOCK_UNLOCK(struct Value *value);
extern struct Value *stmt_LOOP(struct Value *value);
extern struct Value *stmt_LOOPUNTIL(struct Value *value);
extern struct Value *stmt_LSET_RSET(struct Value *value);
extern struct Value *stmt_IDENTIFIER(struct Value *value);
extern struct Value *stmt_IF_ELSEIFIF(struct Value *value);
extern struct Value *stmt_IMAGE(struct Value *value);
extern struct Value *stmt_INPUT(struct Value *value);
extern struct Value *stmt_MAT(struct Value *value);
extern struct Value *stmt_MATINPUT(struct Value *value);
extern struct Value *stmt_MATPRINT(struct Value *value);
extern struct Value *stmt_MATREAD(struct Value *value);
extern struct Value *stmt_MATREDIM(struct Value *value);
extern struct Value *stmt_MATWRITE(struct Value *value);
extern struct Value *stmt_NAME(struct Value *value);
extern struct Value *stmt_NEW(struct Value *value);
extern struct Value *stmt_NEXT(struct Value *value);
extern struct Value *stmt_ON(struct Value *value);
extern struct Value *stmt_ONERROR(struct Value *value);
extern struct Value *stmt_ONERRORGOTO0(struct Value *value);
extern struct Value *stmt_ONERROROFF(struct Value *value);
extern struct Value *stmt_OPEN(struct Value *value);
extern struct Value *stmt_OPTIONBASE(struct Value *value);
extern struct Value *stmt_OPTIONRUN(struct Value *value);
extern struct Value *stmt_OPTIONSTOP(struct Value *value);
extern struct Value *stmt_OUT_POKE(struct Value *value);
extern struct Value *stmt_PRINT_LPRINT(struct Value *value);
extern struct Value *stmt_RANDOMIZE(struct Value *value);
extern struct Value *stmt_READ(struct Value *value);
extern struct Value *stmt_COPY_RENAME(struct Value *value);
extern struct Value *stmt_RENUM(struct Value *value);
extern struct Value *stmt_REPEAT(struct Value *value);
extern struct Value *stmt_RESTORE(struct Value *value);
extern struct Value *stmt_RETURN(struct Value *value);
extern struct Value *stmt_RUN(struct Value *value);
extern struct Value *stmt_SAVE(struct Value *value);
extern struct Value *stmt_SELECTCASE(struct Value *value);
extern struct Value *stmt_SHELL(struct Value *value);
extern struct Value *stmt_SLEEP(struct Value *value);
extern struct Value *stmt_STOP(struct Value *value);
extern struct Value *stmt_SUBEXIT(struct Value *value);
extern struct Value *stmt_SWAP(struct Value *value);
extern struct Value *stmt_SYSTEM(struct Value *value);

extern struct Value *stmt_TROFF(struct Value *value);
extern struct Value *stmt_TRON(struct Value *value);
extern struct Value *stmt_TRUNCATE(struct Value *value);
extern struct Value *stmt_UNNUM(struct Value *value);
extern struct Value *stmt_UNTIL(struct Value *value);
extern struct Value *stmt_WAIT(struct Value *value);
extern struct Value *stmt_WHILE(struct Value *value);
extern struct Value *stmt_WEND(struct Value *value);
extern struct Value *stmt_WIDTH(struct Value *value);
extern struct Value *stmt_WRITE(struct Value *value);
extern struct Value *stmt_XREF(struct Value *value);
extern struct Value *stmt_ZONE(struct Value *value);

#endif
