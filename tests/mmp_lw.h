/*********************************
 * Macro Meta-Programming Lib
 * - Lightweight Header
 *********************************/

// ARG -> {SUB_}MACRO_ARG or {SUB_}MACRO_FUNC_ARG or UNKNOWN
// Arg -> Identifier of TypeArg or FuncArg
// arg -> Identifier of valueArg

// param   -> Ignored parameters
// _param  -> Used parameters
// __value -> Declaration of variables included in the macro definition
// __ID    -> Declaration internal or implementation overloading of macro functions or keywords

// AaBb                 -> AaBb
// EnclosedAaBb         -> (Aa Bb)
// UnclosedAaBb         -> Aa Bb
// AaEnclosedBb         -> (Aa)Bb
// AaBbEnclosed         -> Aa(Bb)
// EnclosedAaBbEach     -> (Aa)(Bb)
// PairAaBb             -> (Aa, Bb)
// PairEnclosedAaBbEach -> ((Aa), (Bb))

#ifndef MMP_LW_H
#  define MMP_LW_H

#  define MMP_BLOCK
#  define MMP_EMPTY()
#  define MMP_IGNORE(...)
#  define MMP_EXPAND(...) __VA_ARGS__

#  define MMP_PRIM_CONCAT(_X, _Y) _X##_Y
#  define MMP_PRIM_JOIN(_X, _Y)   _X##_##_Y

#  define MMP_CONCAT(_X, _Y) \
        MMP_CONCAT_RETURN(_X, _Y)
#  define MMP_CONCAT_RETURN(_X, _Y) _X##_Y
#  define MMP_CONCAT3(_X, _Y, _Z) \
        MMP_CONCAT3_RETURN(_X, _Y, _Z)
#  define MMP_CONCAT3_RETURN(_X, _Y, _Z) _X##_Y##_Z

#  define MMP_JOIN(_X, _Y) \
        MMP_JOIN_RETURN(_X, _Y)
#  define MMP_JOIN_RETURN(_X, _Y) _X##_##_Y
#  define MMP_JOIN3(_X, _Y, _Z) \
        MMP_JOIN3_RETURN(_X, _Y, _Z)
#  define MMP_JOIN3_RETURN(_X, _Y, _Z) _X##_##_Y##_##_Z

#  define MMP_EXTN(_MCR, _EXTS) \
        MMP_EXTN_RETURN(_MCR, _EXTS)
#  define MMP_EXTN_RETURN(_MCR, _EXTS) _MCR##_##_EXTS

#  define MMP_IMPL(_MCR) \
        MMP_IMPL_RETURN(_MCR)
#  define MMP_IMPL_RETURN(_MCR) __##_MCR

#  define MMP_IMPL_OVLD(_MCRSuffixed_) \
        MMP_IMPL_OVLD_RETURN(_MCRSuffixed_)
#  define MMP_IMPL_OVLD_RETURN(_MCRSuffixed_) __##_MCRSuffixed_

#  define MMP_IMPL_OVLD_UNARY(_MCRSuffixed_, _OVLS) \
        MMP_IMPL_OVLD_UNARY_RETURN(_MCRSuffixed_, _OVLS)
#  define MMP_IMPL_OVLD_UNARY_RETURN(_MCRSuffixed_, _OVLS) __##_MCRSuffixed_##_OVLS

#  define MMP_IMPL_OVLD_BINARY(_MCRSuffixed_, _OVLSSuffixed_, _OVLS) \
        MMP_IMPL_OVLD_BINARY_UNEVAL(_MCRSuffixed_, _OVLSSuffixed_, _OVLS)
#  define MMP_IMPL_OVLD_BINARY_RETURN(_MCRSuffixed_, _OVLSSuffixed_, _OVLS) __##_MCRSuffixed_##_OVLSSuffixed_##_OVLS

#  define MMP_DEFER(...)  __VA_ARGS__ MMP_EMPTY()
#  define MMP_DEFER1(...) __VA_ARGS__ MMP_EMPTY()
#  define MMP_DEFER2(...) __VA_ARGS__ MMP_DEFER(MMP_EMPTY)()
#  define MMP_DEFER3(...) __VA_ARGS__ MMP_DEFER2(MMP_EMPTY)()
#  define MMP_DEFER4(...) __VA_ARGS__ MMP_DEFER3(MMP_EMPTY)()
#  define MMP_DEFER5(...) __VA_ARGS__ MMP_DEFER4(MMP_EMPTY)()
#  define MMP_DEFER6(...) __VA_ARGS__ MMP_DEFER5(MMP_EMPTY)()
#  define MMP_DEFER7(...) __VA_ARGS__ MMP_DEFER6(MMP_EMPTY)()
#  define MMP_DEFER8(...) __VA_ARGS__ MMP_DEFER7(MMP_EMPTY)()

#  define MMP_EVAL(...)    MMP_EVAL255(__VA_ARGS__)
#  define MMP_EVAL1(...)   __VA_ARGS__
#  define MMP_EVAL3(...)   MMP_EVAL1(MMP_EVAL1(__VA_ARGS__))
#  define MMP_EVAL7(...)   MMP_EVAL3(MMP_EVAL3(__VA_ARGS__))
#  define MMP_EVAL15(...)  MMP_EVAL7(MMP_EVAL7(__VA_ARGS__))
#  define MMP_EVAL31(...)  MMP_EVAL15(MMP_EVAL15(__VA_ARGS__))
#  define MMP_EVAL63(...)  MMP_EVAL31(MMP_EVAL31(__VA_ARGS__))
#  define MMP_EVAL127(...) MMP_EVAL63(MMP_EVAL63(__VA_ARGS__))
#  define MMP_EVAL255(...) MMP_EVAL127(MMP_EVAL127(__VA_ARGS__))

#  define MMP_1ST(_A1, ...)          _A1
#  define MMP_2ND(A1, _A2, ...)      _A2
#  define MMP_3RD(A1, A2, _A3...)    _A3
#  define MMP_REST(A1, ...)          __VA_ARGS__
#  define MMP_REST2(A1, A2, ...)     __VA_ARGS__
#  define MMP_REST3(A1, A2, A3, ...) __VA_ARGS__

#  define MMP_ENCLOSE(...)          (__VA_ARGS__)
#  define MMP_EXECUTER_UNCLOSE(...) __VA_ARGS__
#  define MMP_UNCLOSE(...)          MMP_EXECUTER_UNCLOSE __VA_ARGS__

#  define MMP_TRUE(...)  1
#  define MMP_FALSE(...) 0

#  define MMP_DETECT()     ~, 1
#  define MMP_DETECTS(...) MMP_2ND(__VA_ARGS__, 0)

#  define MMP_CMPL(_bit) MMP_IMPL_OVLD_UNARY_RETURN(MMP_CMPL_, _bit)
#  define __MMP_CMPL_0   1
#  define __MMP_CMPL_1   0

#  define MMP_NOT(_value) MMP_DETECTS(MMP_IMPL_OVLD_UNARY_RETURN(MMP_NOT_, _value))
#  define __MMP_NOT_0     MMP_DETECT()

#  define MMP_BOOL(_value) MMP_CMPL(MMP_NOT(_value))

#  define MMP_IF(_cond)                MMP_IMPL_OVLD_UNARY(MMP_IF_, MMP_BOOL(_cond))
#  define __MMP_IF_0(trueClause, ...)  __VA_ARGS__
#  define __MMP_IF_1(_trueClause, ...) _trueClause
#  define MMP_THEN
#  define MMP_ELSE
#  define MMP_WHEN(_pred) MMP_IF(_pred)( \
      MMP_UNCLOSE, MMP_IGNORE            \
  )
#  define MMP_UNLESS(_pred) MMP_IF(MMP_NOT(_pred))( \
      MMP_UNCLOSE, MMP_IGNORE                       \
  )
#  define MMP_SELECT(_CondClause, _ThenClause, ...) MMP_IMPL_OVLD_UNARY(MMP_IF_, _CondClause)( \
      MMP_DEFER(_ThenClause), MMP_DEFER(__VA_ARGS__)                                           \
  )

/* test if the value is an existing macro */
#  define MMP_GET_PATTERN_EXISTS_RESULT(_EXISTS) (MMP_IMPL_OVLD_UNARY_RETURN(MMP_PATTERN_MATCH_, _EXISTS), DOESNT_EXIST)
#  define __MMP_PATTERN_MATCH_EXISTS(...)  EXPANDED, EXISTS(__VA_ARGS__)) MMP_IGNORE(

/* extract the 2nd field in the tuple */
#  define MMP_EXISTS_PATTERN(_VALUE)                                  MMP_GET_PATTERN_EXIST_VALUE(MMP_GET_PATTERN_EXISTS_RESULT(_VALUE))
#  define MMP_EXECUTER_GET_PATTERN_EXIST_VALUE(EXPANDED, _existValue) _existValue
#  define MMP_GET_PATTERN_EXIST_VALUE(_PairResult)                    MMP_EXECUTER_GET_PATTERN_EXIST_VALUE _PairResult

/* return 1 if exists, otherwise 0 */
#  define MMP_DOES_VALUE_EXIST(_EXISTS)       MMP_IMPL_OVLD_UNARY_RETURN(MMP_DOES_VALUE_EXIST_, _EXISTS)
#  define __MMP_DOES_VALUE_EXIST_DOESNT_EXIST 0
#  define __MMP_DOES_VALUE_EXIST_EXISTS(...)  1

/* extract x from EXIST(x) */
#  define MMP_EXTRACT_VALUE(_EXISTS)      MMP_IMPL_OVLD_UNARY_RETURN(MMP_EXTRACT_VALUE_, _EXISTS)
#  define __MMP_EXTRACT_VALUE_EXISTS(...) __VA_ARGS__

/* extract value if exists, otherwise return given default value */
#  define MMP_TRY_EXTRACT_EXISTS(_VALUE, ...)                    \
        MMP_IF(MMP_DOES_VALUE_EXIST(MMP_EXISTS_PATTERN(_VALUE))) \
        (                                                        \
            MMP_THEN MMP_EXTRACT_VALUE(_VALUE),                  \
            MMP_ELSE __VA_ARGS__                                 \
        )

#  define MMP_BOOL_OPER_OVLD_UNARY(_OPSuffixed_, _p, _default) MMP_TRY_EXTRACT_EXISTS( \
      MMP_IMPL_OVLD_UNARY_RETURN(_OPSuffixed_, _p), _default                           \
  )
#  define MMP_BOOL_OPER_OVLD_BINARY(_OPSuffixed_, _p, _q, _default) MMP_TRY_EXTRACT_EXISTS( \
      MMP_IMPL_OVLD_BINARY_RETURN(_OPSuffixed_, _p, _q), _default                           \
  )

#  define MMP_AND(_p, _q) MMP_BOOL_OPER_OVLD_BINARY(MMP_AND_, _p, _q, 0)
#  define __MMP_AND_11    EXISTS(1)

#  define MMP_OR(_p, _q) MMP_BOOL_OPER_OVLD_BINARY(MMP_OR_, _p, _q, 1)
#  define __MMP_OR_00    EXISTS(0)

#  define MMP_NAND(_p, _q) MMP_BOOL_OPER_OVLD_BINARY(MMP_NAND_, _p, _q, 1)
#  define __MMP_NAND_11    EXISTS(0)
// #define MMP_AND(_p, _q) MMP_NAND(MMP_NAND(_p, _q), MMP_NAND(_p, _q))
// #define MMP_OR(_p, _q)  MMP_NAND(MMP_NAND(_p, _p), (MMP_NAND(_q, _q))

#  define MMP_NOR(_p, _q) MMP_BOOL_OPER_OVLD_BINARY(MMP_NOR_, _p, _q, 0)
#  define __MMP_NOR_11    EXISTS(1)
// #define MMP_AND(_p, _q) MMP_NOR(MMP_NOR(_p, _p), MMP_NOR(_q, _q))
// #define MMP_OR(_p, _q)  MMP_NOR(MMP_NOR(_p, _q), (MMP_NOR(_p, _q))

#  define MMP_XOR(_p, _q) MMP_BOOL_OPER_OVLD_BINARY(MMP_XOR_, _p, _q, 0)
#  define __MMP_XOR_01    EXISTS(1)
#  define __MMP_XOR_10    EXISTS(1)

#  define MMP_EQV(_p, _q) MMP_BOOL_OPER_OVLD_BINARY(MMP_EQV_, _p, _q, 0)
#  define __MMP_EQV_00    EXISTS(1)
#  define __MMP_EQV_11    EXISTS(1)

#  define MMP_IS_ZERO(_x)    MMP_BOOL_OPER_OVLD_UNARY(MMP_IS_ZERO_, _x, 0)
#  define __MMP_IS_ZERO_0    EXISTS(1)
#  define MMP_IS_NOT_ZERO(x) MMP_NOT(MMP_IS_ZERO(x))

#  define MMP_IS_ONE(_x)     MMP_BOOL_OPER_OVLD_UNARY(MMP_IS_ONE_, _x, 0)
#  define __MMP_IS_ONE_1     EXISTS(1)
#  define MMP_IS_NOT_ONE(_x) MMP_NOT(MMP_IS_ONE(_x))

#  define MMP_HEAD(_ARG, ...) _ARG
#  define MMP_TAIL(ARG, ...)  __VA_ARGS__
#  define MMP_IS_EMPTY(...)   MMP_TRY_EXTRACT_EXISTS( \
      MMP_DEFER(MMP_HEAD)(__VA_ARGS__ EXISTS(1)), 0   \
  )

#  define MMP_EXECUTER_IS_ENCLOSED(...) EXISTS(1)
#  define MMP_IS_ENCLOSED(_X, ...)      MMP_TRY_EXTRACT_EXISTS( \
      MMP_EXECUTER_IS_ENCLOSED _X, 0                            \
  )
#  define MMP_IS_NOT_ENCLOSED(_X, ...) MMP_NOT(MMP_IS_ENCLOSED(_X, __VA_ARGS__))

#  define MMP_UNCLOSE_OPT(...)                 \
        MMP_IF(MMP_IS_ENCLOSED(__VA_ARGS__))   \
        (                                      \
            MMP_THEN MMP_UNCLOSE(__VA_ARGS__), \
            MMP_ELSE __VA_ARGS__               \
        )

#  define MMP_NOT_COMPARABLE(_X, _Y) MMP_IS_ENCLOSED(  \
      MMP_JOIN(COMPARE, _X)(MMP_JOIN(COMPARE, _Y))(()) \
  )

#  define MMP_IS_COMPARABLE(_X) MMP_IS_ENCLOSED( \
      MMP_JOIN(COMPARE, _X)(())                  \
  )

#  define MMP_NOT_EQUALS(_X, _Y)                                      \
        MMP_IF(MMP_AND(MMP_IS_COMPARABLE(_X), MMP_IS_COMPARABLE(_Y))) \
        (                                                             \
            MMP_THEN MMP_NOT_COMPARABLE,                              \
            MMP_ELSE 1 MMP_IGNORE                                     \
        )(_X, _Y)

#  define MMP_EQUALS(_X, _Y) MMP_CMPL(MMP_NOT_EQUALS(_X, _Y))

#endif // !MMP_LW_H



// /* Helper macros to count the number of arguments */
// #define ASSERT__TYPE_BY_NARGS(expression, ...) MMP_EXPAND(ASSERT__TYPE_BY_NARGS_RETURN(__VA_ARGS__))
// #define ASSERT__TYPE_BY_NARGS_RETURN(...)       MMP_IF(MMP_IS_EMPTY(__VA_ARGS__))(1, 2)

// /* Select the correct assert macro based on the number of arguments */
// #define assert(...) MMP_EXPAND(MMP_CONCAT MMP_BLOCK(assert, ASSERT__TYPE_BY_NARGS(__VA_ARGS__))(__VA_ARGS__))

// /* assert without additional message */
// #define assert1(expression)                                                                                                                 \
//       do {                                                                                                                                   \
//           if (!(expression)) {                                                                                                              \
//               (void)fprintf(stderr, "assertion failed: %s\nFile: %s\nLine: %d\nFunction: %s\n", #expression, __FILE__, __LINE__, __func__); \
//               DEBUG_BREAK();                                                                                                                 \
//               abort();                                                                                                                       \
//           }                                                                                                                                  \
//       } while (0)

// /* assert with additional formatted message */
// #define assert2(expression, ...)                                                                          \
//       do {                                                                                                 \
//           if (!(expression)) {                                                                            \
//               (void)fprintf(stderr, "assertion failed: %s\n", #expression);                               \
//               (void)fprintf(stderr, __VA_ARGS__);                                                          \
//               (void)fprintf(stderr, "\nFile: %s\nLine: %d\nFunction: %s\n", __FILE__, __LINE__, __func__); \
//               DEBUG_BREAK();                                                                               \
//               abort();                                                                                     \
//           }                                                                                                \
//       } while (0)

// void test() {
//     assert(1 == 1);
// }
