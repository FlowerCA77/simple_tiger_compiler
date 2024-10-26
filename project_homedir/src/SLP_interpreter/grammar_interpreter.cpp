#include "grammar_interpreter.hpp"
#include "../utils.hpp"

A_stm A_CompoundStm(A_stm stm1, A_stm stm2)
{
    A_stm s = (A_stm)checked_malloc(sizeof(*s));
    s->kind = A_stm_::A_compoundStm;
    s->u.compound = {.stm1 = stm1, .stm2 = stm2};
    return s;
}

A_stm A_AssignStm(string id, A_exp exp)
{
    A_stm s = (A_stm)checked_malloc(sizeof(*s));
    s->kind = A_stm_::A_assignStm;
    s->u.assign = {.id = id, .exp = exp};
    return s;
}

A_stm A_PrintStm(A_expList exps)
{
    A_stm s = (A_stm)checked_malloc(sizeof(*s));
    s->kind = A_stm_::A_printStm;
    s->u.print.exps = exps;
    return s;
}

A_exp A_IdExp(string id)
{
    A_exp e = (A_exp)checked_malloc(sizeof(*e));
    e->kind = A_exp_::A_idExp;
    e->u.id = id;
    return e;
}

A_exp A_NumExp(int num)
{
    A_exp e = (A_exp)checked_malloc(sizeof(*e));
    e->kind = A_exp_::A_numExp;
    e->u.num = num;
    return e;
}

A_exp A_OpExp(A_exp left, A_binop oper, A_exp right)
{
    A_exp e = (A_exp)checked_malloc(sizeof(*e));
    e->kind = A_exp_::A_opExp;
    e->u.op = {.left = left, .oper = oper, .right = right};
    return e;
}

A_exp A_EseqExp(A_stm stm, A_exp exp)
{
    A_exp e = (A_exp)checked_malloc(sizeof(*e));
    e->kind = A_exp_::A_eseqExp;
    e->u.eseq = {.stm = stm, .exp = exp};
    return e;
}

A_expList A_PairExpList(A_exp head, A_expList tail)
{
    A_expList el = (A_expList)checked_malloc(sizeof(*el));
    el->kind = A_expList_::A_pairExpList;
    el->u.pair = {.head = head, .tail = tail};
    return el;
}

A_expList A_LastExpList(A_exp last)
{
    A_expList el = (A_expList)checked_malloc(sizeof(*el));
    el->kind = A_expList_::A_lastExpList;
    el->u.last = last;
    return el;
}