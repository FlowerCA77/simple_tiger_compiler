/**
 * @file grammar_interpreter.hpp
 * @ingroup SLP_interpreter
 * @brief realize of grammar
 * @details
 * Grammar atoms: stm, exp, expList, id, num, ...<br>
 * Grammar rules: A_compoundStm, A_assignStm, A_printStm, ...<br>
 * @verbatim
 * For example, grammar atoms ga have rules gr1, gr2, ...
 *  using A_ga = ptr to struct A_ga_
 *  struct A_ga_:
 *      field enum   ->  rules tokens
 *      field union  ->  rules components
 * constructo of rule:
 *  auto A_gr(A_ga components ...) -> pointer to struct A_ga;
 * @endverbatim
 */

/**
 * @details
 * grammars was defined to type<br>
 * for with data grammars, we use pointer to struct<br>
 * for without data grammars, we use enum<br>
 */

#include "../utils.hpp"

using A_stm = struct A_stm_ *;
using A_exp = struct A_exp_ *;
using A_expList = struct A_expList_ *;
enum A_binop
{
    A_plus,  // +
    A_minus, // -
    A_times, // *
    A_div    // /
};

/**
 * @struct A_stm_
 * @details
 * @code
 * Stm -> Stm; Stm
 * Stm -> id := Stm
 * Stm -> print(ExpList)
 * @endcode
 */
struct A_stm_
{
    enum
    {
        A_compoundStm, // Stm -> Stm; Stm
        A_assignStm,   // Stm -> id := Exp
        A_printStm     // Stm -> print(ExpList)
    } kind;
    union
    {
        struct
        {
            A_stm stm1, stm2;
        } compound;
        struct
        {
            string id;
            A_exp exp;
        } assign;
        struct
        {
            A_expList exps;
        } print;
    } u;
};

A_stm A_CompoundStm(A_stm stm1, A_stm stm2);
A_stm A_AssignStm(string id, A_exp exp);
A_stm A_PrintStm(A_expList exps);

/**
 * @struct A_exp_
 * @details
 * @code
 * Exp -> id
 * Exp -> num
 * Exp -> Exp Binop Exp
 * Exp -> (Stm, Exp)
 * @endcode
 */
struct A_exp_
{
    enum
    {
        A_idExp,  // Exp -> id
        A_numExp, // Exp -> num
        A_opExp,  // Exp -> Exp Binop Exp
        A_eseqExp // Exp -> (Stm, Exp)
    } kind;
    union
    {
        string id;
        int num;
        struct
        {
            A_exp left;
            A_binop oper;
            A_exp right;
        } op;
        struct
        {
            A_stm stm;
            A_exp exp;
        } eseq;
    } u;
};

A_exp A_IdExp(string id);
A_exp A_NumExp(int num);
A_exp A_OpExp(A_exp left, A_binop oper, A_exp right);
A_exp A_EseqExp(A_stm stm, A_exp exp);

/**
 * @struct A_expList_
 * @details
 * @code
 * ExpList -> Exp, ExpList
 * ExpList -> Exp
 * @endcode
 */
struct A_expList_
{
    enum
    {
        A_pairExpList, // ExpList -> Exp, ExpList
        A_lastExpList  // ExpList -> Exp
    } kind;
    union
    {
        struct
        {
            A_exp head;
            A_expList tail;
        } pair;
        A_exp last;
    } u;
};

A_expList A_PairExpList(A_exp head, A_expList tail);
A_expList A_LastExpList(A_exp last);