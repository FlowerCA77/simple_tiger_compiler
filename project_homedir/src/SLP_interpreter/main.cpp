/**
 * @section SLP `__main__` file
 * @ingroup SLP_interpreter
 */
#include <cstring>
#include <iostream>

template <typename T = int>
T max(T a, T b) {
    return a > b ? a : b;
}

template <typename T = int>
T min(T a, T b) {
    return a < b ? a : b;
}

#include "main.hpp"

/** @subsection `maxargs` work. */
/** @brief update `count` counter and `history_max` record in Exp */
void update_e(A_exp e, int &count, int &history_max, bool &is_in_print, bool &is_out_print) {
    if (e->kind == A_exp_::A_opExp) {
        is_out_print = false;
        A_exp e_left = e->u.op.left;
        A_exp e_right = e->u.op.right;
        update_e(e_left, count, history_max, is_in_print, is_out_print);
        update_e(e_right, count, history_max, is_in_print, is_out_print);
    }
    if (e->kind == A_exp_::A_eseqExp) {
        is_out_print = false;
        A_stm e_s = e->u.eseq.stm;
        A_exp e_e = e->u.eseq.exp;
        update_s(e_s, count, history_max, is_in_print, is_out_print);
        update_e(e_e, count, history_max, is_in_print, is_out_print);
    }
}

/** @brief update `count` counter and `history_max` record in ExpList */
void update_el(A_expList el, int &count, int &history_max, bool &is_in_print, bool &is_out_print) {
    if (el->kind == A_expList_::A_pairExpList) {
        if (is_in_print)
            count++;
        A_exp e_head = el->u.pair.head;
        A_expList el_tail = el->u.pair.tail;
        update_e(e_head, count, history_max, is_in_print, is_out_print);
        update_el(el_tail, count, history_max, is_in_print, is_out_print);
        history_max = max(history_max, count);
    }
    if (el->kind == A_expList_::A_lastExpList) {
        if (is_in_print)
            count++;
        A_exp e_last = el->u.last;
        update_e(e_last, count, history_max, is_in_print, is_out_print);
        history_max = max(history_max, count);
        is_in_print = false;
    }
}

/** @brief update `count` counter and `history_max` record in Stm */
void update_s(A_stm s, int &count, int &history_max, bool &is_in_print, bool &is_out_print) {
    auto kind = s->kind;
    if (kind == A_stm_::A_compoundStm) {
        A_stm s1 = s->u.compound.stm1;
        A_stm s2 = s->u.compound.stm2;
        update_s(s1, count, history_max, is_in_print, is_out_print);
        update_s(s2, count, history_max, is_in_print, is_out_print);
    }
    if (kind == A_stm_::A_assignStm) {
        A_exp e = s->u.assign.exp;
        update_e(e, count, history_max, is_in_print, is_out_print);
    }
    if (kind == A_stm_::A_printStm) {
        is_in_print = true;
        count = 0;
        A_expList el = s->u.print.exps;
        update_el(el, count, history_max, is_in_print, is_out_print);
        history_max = max(history_max, count);
    }
}

/** @brief Count `print` args number max */
int maxargs(A_stm s) {
    bool is_in_print = false;
    int count = 0;
    int history_max = 0;
    bool is_out_print = false;
    update_s(s, count, history_max, is_in_print, is_out_print);
    return history_max;
}

/** @subsection `interp` work. */

/**
 * @brief update table t1 with new identifier id2 and value t2
 */
Table_ update(Table_ t1, string id2, int v2) {
    Table_ t2 = (Table_)checked_malloc(sizeof(*t2));
    *t2 = {.id = id2, .value = v2, .tail = t1};
    return t2;
}

/**
 * @brief find the identifier key's value in table t
 */
int lookup(Table_ t, string key) {
    Table_ cur = t;
    while (cur) {
        if (std::strcmp(cur->id, key) == 0)
            return cur->value;
        cur = cur->tail;
    }
    return -1;
}

/**
 * @brief interprete a stm s, and table t is the current state.
 */
Table_ interpStm(A_stm s, Table_ t) {
    if (s == nullptr)
        return nullptr;
    Table_ t2 = (Table_)checked_malloc(sizeof(*t2));
    IntAndTable_ it2 = (IntAndTable_)checked_malloc(sizeof(*it2));
    switch (s->kind) {
    case (A_stm_::A_compoundStm): {
        t2 = interpStm(s->u.compound.stm1, t);
        t2 = interpStm(s->u.compound.stm2, t2);
        return t2;
    }
    case (A_stm_::A_assignStm): {
        it2 = interpExp(s->u.assign.exp, t);
        t2 = update(it2->tail, s->u.assign.id, it2->i);
        return t2;
    }
    case (A_stm_::A_printStm): {
        A_expList exps_list = s->u.print.exps;
        while (true) {
            if (exps_list->kind == A_expList_::A_lastExpList) {
                it2 = interpExp(exps_list->u.last, t);
                std::cout << it2->i;
                break;
            } else {
                it2 = interpExp(exps_list->u.pair.head, t);
                std::cout << it2->i << " ";
                exps_list = exps_list->u.pair.tail;
            }
        }
        std::cout << std::endl;
        return it2->tail;
    }
    }
    return nullptr;
}

/**
 * @brief interprete a exp e, and table t is the current state.
 */
IntAndTable_ interpExp(A_exp e, Table_ t) {
    if (e == nullptr)
        return nullptr;
    IntAndTable_ it2 = (IntAndTable_)checked_malloc(sizeof(*it2));
    switch (e->kind) {
    case (A_exp_::A_idExp): {
        it2->i = lookup(t, e->u.id);
        it2->tail = t;
        return it2;
    }
    case (A_exp_::A_numExp): {
        it2->i = e->u.num;
        it2->tail = t;
        return it2;
    }
    case (A_exp_::A_opExp): {
        it2 = interpExp(e->u.op.left, t);
        int cur = it2->i;
        it2 = interpExp(e->u.op.right, it2->tail);
        switch (e->u.op.oper) {
        case (A_binop::A_plus): {
            cur += it2->i;
            break;
        }
        case (A_binop::A_minus): {
            cur -= it2->i;
            break;
        }
        case (A_binop::A_times): {
            cur *= it2->i;
            break;
        }
        case (A_binop::A_div): {
            cur /= it2->i;
            break;
        }
        }
        it2->i = cur;
        return it2;
    }
    case (A_exp_::A_eseqExp): {
        t = interpStm(e->u.eseq.stm, t);
        it2 = interpExp(e->u.eseq.exp, t);
        return it2;
    }
    }
    return nullptr;
}

/**
 * @brief interprete a explist el, and table t is the current state.
 */
IntAndTable_ interpExpList(A_expList el, Table_ t) {
    if (el == nullptr)
        return nullptr;
    IntAndTable_ it2 = (IntAndTable_)checked_malloc(sizeof(*it2));
    switch (el->kind) {
    case (A_expList_::A_pairExpList): {
        it2 = interpExp(el->u.pair.head, t);
        it2 = interpExpList(el->u.pair.tail, it2->tail);
        return it2;
    }
    case (A_expList_::A_lastExpList): {
        it2 = interpExp(el->u.last, t);
        return it2;
    }
    }
    return nullptr;
}

/**
 * @brief the final SLP interpreter.
 */
void interp(A_stm s) {
    interpStm(s, nullptr);
}

/** @subsection examples of SLP interpreter */
/**
 * @brief Example Tiger Code 1
 * @details
 * ```
 * a := 5+3; b := (print(a, a-1), 10*a);
 * print(b); print(c1);
 * ```
 */
A_stm prog_generator(void) {
    return A_CompoundStm(
        A_AssignStm("a", A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
        A_CompoundStm(
            A_AssignStm(
                "b",
                A_EseqExp(
                    A_PrintStm(
                        A_PairExpList(
                            A_IdExp("a"),
                            A_LastExpList(
                                A_OpExp(A_IdExp("a"), A_minus, A_NumExp(1))))),
                    A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
            A_CompoundStm(
                A_PrintStm(A_LastExpList(A_IdExp("b"))),
                A_PrintStm(A_LastExpList(A_IdExp("c1"))))));
}

/**
 * @brief Example Tiger Code 2
 * @details
 * ```
 * a := 5+3; b := (print(a, a-1), 10*a); print(b);
 * print(c1, c2, c3, c4, c5, c6, c7, c8, c9, c10);
 * ```
 */
A_stm prog_generator2(void) {
    return A_CompoundStm(
        A_AssignStm("a", A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
        A_CompoundStm(
            A_AssignStm(
                "b",
                A_EseqExp(
                    A_PrintStm(
                        A_PairExpList(
                            A_IdExp("a"),
                            A_LastExpList(
                                A_OpExp(A_IdExp("a"), A_minus, A_NumExp(1))))),
                    A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
            A_CompoundStm(
                A_PrintStm(A_LastExpList(A_IdExp("b"))),
                A_PrintStm(A_PairExpList(
                    A_IdExp("c1"),
                    A_PairExpList(
                        A_IdExp("c2"),
                        A_PairExpList(
                            A_IdExp("c3"),
                            A_PairExpList(
                                A_IdExp("c4"),
                                A_PairExpList(
                                    A_IdExp("c5"),
                                    A_PairExpList(
                                        A_IdExp("c6"),
                                        A_PairExpList(
                                            A_IdExp("c7"),
                                            A_PairExpList(
                                                A_IdExp("c8"),
                                                A_PairExpList(
                                                    A_IdExp("c9"),
                                                    A_LastExpList(
                                                        A_IdExp("c10")))))))))))))));
}

int main() {
    A_stm prog = prog_generator();
    std::cout << "Program 1 print maxargs:\n"
              << maxargs(prog) << std::endl;
    A_stm prog2 = prog_generator2();
    std::cout << "Program 2 print maxargs:\n"
              << maxargs(prog2) << std::endl;
    std::cout << "Program 1:\n";
    interp(prog);
    std::cout << "Program 2:\n";
    interp(prog2);
    std::cout << std::endl;
}