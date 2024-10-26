/**
 * @defgroup SLP_interpreter
 * @brief an interpreter to Straight-Line Program(SLP) language
 */

#include "grammar_interpreter.hpp"

void update_e(A_exp e, int &count, int &history_max, bool &is_in_print, bool &is_out_print);
void update_el(A_expList el, int &count, int &history_max, bool &is_in_print, bool &is_out_print);
void update_s(A_stm s, int &count, int &history_max, bool &is_in_print, bool &is_out_print);
int maxargs(A_stm);

void interp(A_stm);

/**
 * @details
 * `table` and `int_and_table` are used to store identifiers.<br>
 * A `table` is used to store a stm and it's value.<br>
 * A `int_and_table` is combines a table and a value, which is the expression's
 * return value.<br>
 * In fact, these tables are linked note list.
 */

using Table_ = struct table *;
using IntAndTable_ = struct int_and_table *;

struct table {
    string id;
    int value;
    Table_ tail;
};

Table_ Table(string id, int value, struct table *tail) {
    Table_ t = (Table_)checked_malloc(sizeof(*t));
    *t = {.id = id, .value = value, .tail = tail};
    return t;
}

struct int_and_table {
    int i;
    Table_ tail;
};

Table_ interpStm(A_stm s, Table_ t);
IntAndTable_ interpExp(A_exp e, Table_ t);
IntAndTable_ interpExpList(A_expList e, Table_ t);
void interp(A_stm);