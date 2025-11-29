/**
 * @copyright Copyright (c) 2025. Gyeongtae Kim.
 * @license   MIT License - see LICENSE file for details
 *
 * @file    chain.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-26 (date of creation)
 * @updated 2025-11-26 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core
 * @prefix  (none)
 *
 * @brief   Functional chaining utilities
 * @details Provides utilities for functional chaining operations.
 */
#ifndef core_chain__included
#define core_chain__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim.h"
#include "scope.h"

/*========== Macros and Declarations ========================================*/

typedef enum chain__State : u8 {
    chain__State_none = 0,
    chain__State_init = 1,
    chain__State_eval = 2
} chain__State;

#define chain$(/*(_T)(_s)(_fnOps...)*/...) __step__chain$(__step__chain$__parse __VA_ARGS__)

#define each_(/*(_p_e)_body*/...) __step__each_(__step__each___capt __VA_ARGS__)
#define filter_(/*(_p_e)_pred*/...) __step__filter_(__step__filter___capt __VA_ARGS__)
#define map$(/*(_T)(_p_e)_xform*/...) __step__map$(__step__map$__parse __VA_ARGS__)
#define fold_(/*(_default), (_acc, _p_e)_combine*/...) __step__fold_(__VA_ARGS__)
#define reduce_(/*(_acc, _p_e)_combine*/...) __step__reduce_(__VA_ARGS__)
#if UNUSED_CODE
#define collect_(/*(_gpa)*/...) __step__collect_(__step__collect___gpa __VA_ARGS__)
#define collectFixed_(/*(_out)*/...) __step__collectFixed_(__step__collectFixed___out __VA_ARGS__)
#define collectWithin_(/*(_out)*/...) __step__collectWithin_(__step__collectWithin___out __VA_ARGS__)
#endif /* UNUSED_CODE */

/*========== Macros and Definitions =========================================*/

#define __step__chain$(...) __step__chain$__emit(__VA_ARGS__)
#define __step__chain$__parse(_T...) _T, __step__chain$__parseS
#define __step__chain$__parseS(_s...) pp_uniqTok(s), _s, __step__chain$__parseFnOps
#define __step__chain$__parseFnOps(_fnOps...) _fnOps
#define __step__chain$__emit(...) __chain$(__VA_ARGS__)
#define __chain$(_T, __s, _s, ...) pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcompound-token-split-by-macro\"", \
    "clang diagnostic pop", \
    pp_overload(__chain$, __VA_ARGS__)(_T, __s, _s, __VA_ARGS__) \
)
#define __chain$_1(_T, __s, _s, _fnOp1...) ({ \
    let __s = _s; \
    var_(__reserved_acc, _T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_none : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        let __curr = _fnOp1; \
        asg_lit((&__reserved_acc)(__curr)); \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_s))(__curr) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        let __curr = _fnOp1; \
        asg_lit((&__reserved_acc)(__curr)); \
    }); \
    __reserved_acc; \
})
#define __chain$_2(_T, __s, _s, _fnOp1, _fnOp2...) ({ \
    let __s = _s; \
    var_(__reserved_acc, _T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_none : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            let __curr = _fnOp2; \
            asg_lit((&__reserved_acc)(__curr)); \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_s))(__curr) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            let __curr = _fnOp2; \
            asg_lit((&__reserved_acc)(__curr)); \
        }; \
    }); \
    __reserved_acc; \
})
#define __chain$_3(_T, __s, _s, _fnOp1, _fnOp2, _fnOp3...) ({ \
    let __s = _s; \
    var_(__reserved_acc, _T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_none : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                let __curr = _fnOp3; \
                asg_lit((&__reserved_acc)(__curr)); \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_s))(__curr) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                let __curr = _fnOp3; \
                asg_lit((&__reserved_acc)(__curr)); \
            }; \
        }; \
    }); \
    __reserved_acc; \
})
#define __chain$_4(_T, __s, _s, _fnOp1, _fnOp2, _fnOp3, _fnOp4...) ({ \
    let __s = _s; \
    var_(__reserved_acc, _T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_none : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    let __curr = _fnOp4; \
                    asg_lit((&__reserved_acc)(__curr)); \
                }; \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_s))(__curr) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    let __curr = _fnOp4; \
                    asg_lit((&__reserved_acc)(__curr)); \
                }; \
            }; \
        }; \
    }); \
    __reserved_acc; \
})
#define __chain$_5(_T, __s, _s, _fnOp1, _fnOp2, _fnOp3, _fnOp4, _fnOp5...) ({ \
    let __s = _s; \
    var_(__reserved_acc, _T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_none : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        let __curr = _fnOp5; \
                        asg_lit((&__reserved_acc)(__curr)); \
                    }; \
                }; \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_s))(__curr) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        let __curr = _fnOp5; \
                        asg_lit((&__reserved_acc)(__curr)); \
                    }; \
                }; \
            }; \
        }; \
    }); \
    __reserved_acc; \
})
#define __chain$_6(_T, __s, _s, _fnOp1, _fnOp2, _fnOp3, _fnOp4, _fnOp5, _fnOp6...) ({ \
    let __s = _s; \
    var_(__reserved_acc, _T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_none : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            let __curr = _fnOp6; \
                            asg_lit((&__reserved_acc)(__curr)); \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_s))(__curr) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            let __curr = _fnOp6; \
                            asg_lit((&__reserved_acc)(__curr)); \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    }); \
    __reserved_acc; \
})
#define __chain$_7(_T, __s, _s, _fnOp1, _fnOp2, _fnOp3, _fnOp4, _fnOp5, _fnOp6, _fnOp7...) ({ \
    let __s = _s; \
    var_(__reserved_acc, _T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_none : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            $maybe_unused let __curr = _fnOp6; \
                            { \
                                let_(__reserved, TypeOf(__curr)) = __curr; \
                                let __curr = _fnOp7; \
                                asg_lit((&__reserved_acc)(__curr)); \
                            }; \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_s))(__curr) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            $maybe_unused let __curr = _fnOp6; \
                            { \
                                let_(__reserved, TypeOf(__curr)) = __curr; \
                                let __curr = _fnOp7; \
                                asg_lit((&__reserved_acc)(__curr)); \
                            }; \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    }); \
    __reserved_acc; \
})
#define __chain$_8(_T, __s, _s, _fnOp1, _fnOp2, _fnOp3, _fnOp4, _fnOp5, _fnOp6, _fnOp7, _fnOp8...) ({ \
    let __s = _s; \
    var_(__reserved_acc, _T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_none : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            $maybe_unused let __curr = _fnOp6; \
                            { \
                                let_(__reserved, TypeOf(__curr)) = __curr; \
                                $maybe_unused let __curr = _fnOp7; \
                                { \
                                    let_(__reserved, TypeOf(__curr)) = __curr; \
                                    let __curr = _fnOp8; \
                                    asg_lit((&__reserved_acc)(__curr)); \
                                }; \
                            }; \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_s))(__curr) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            $maybe_unused let __curr = _fnOp6; \
                            { \
                                let_(__reserved, TypeOf(__curr)) = __curr; \
                                $maybe_unused let __curr = _fnOp7; \
                                { \
                                    let_(__reserved, TypeOf(__curr)) = __curr; \
                                    let __curr = _fnOp8; \
                                    asg_lit((&__reserved_acc)(__curr)); \
                                }; \
                            }; \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    }); \
    __reserved_acc; \
})

#define __step__each_(...) __step__each___emit(__VA_ARGS__)
#define __step__each___capt(_p_e...) _p_e,
#define __step__each___emit(...) __each_(__VA_ARGS__)
#define __each_(_p_e, _body...) ({ \
    if (__chain_state != chain__State_eval) { \
    } else { \
        let _p_e = __reserved; \
        _body; \
    }; \
    __reserved; \
})

#define __step__filter_(...) __step__filter___emit(__VA_ARGS__)
#define __step__filter___capt(_p_e...) _p_e, __step__filter___pred
#define __step__filter___pred(_pred...) _pred
#define __step__filter___emit(...) __filter_(__VA_ARGS__)
#define __filter_(_p_e, _pred...) ({ \
    if (__chain_state != chain__State_eval) { \
    } else { \
        let _p_e = __reserved; \
        if (!(_pred)) { continue; }; \
    }; \
    __reserved; \
})

#define __step__map$(...) __step__map$__emit(__VA_ARGS__)
#define __step__map$__parse(_T...) _T, __step__map$__parseP
#define __step__map$__parseP(_p_e...) _p_e,
#define __step__map$__emit(...) __map$(__VA_ARGS__)
#define __map$(_T, _p_e, _xform...) expr_(_T* $scope)({ \
    if (__chain_state != chain__State_eval) { \
        $break_(&(_T){}); \
    } else { \
        let _p_e = __reserved; \
        $break_(&copy(_xform)); \
    }; \
}) $unscoped_(expr)

#define __step__fold_(_default, ...) __step__fold___emit(_default, __step__fold___capt __VA_ARGS__)
#define __step__fold___capt(_acc, _p_e...) _acc, _p_e,
#define __step__fold___emit(...) __fold_(__VA_ARGS__)
#define __fold_(_default, _acc, _p_e, _combine...) expr_(TypeOf(__reserved_acc) $scope)({ \
    if (__chain_state != chain__State_eval) { \
        $break_(_default); \
    } else { \
        let _p_e = __reserved; \
        var _acc = __reserved_acc; \
        $break_(_combine); \
    }; \
}) $unscoped_(expr)

#define __step__reduce_(...) __step__reduce___emit(__step__reduce___capt __VA_ARGS__)
#define __step__reduce___capt(_acc, _p_e...) _acc, _p_e,
#define __step__reduce___emit(...) __reduce_(__VA_ARGS__)
#define __reduce_(_acc, _p_e, _combine...) expr_(TypeOf(__reserved_acc) $scope)({ \
    if (__chain_state != chain__State_eval) { \
        $break_(none()); \
    } else { \
        if (isNone(__reserved_acc)) { \
            $break_(some(*__reserved)); \
            continue; \
        }; \
        let _p_e = __reserved; \
        var _acc = orelse_((__reserved_acc)(*__reserved)); \
        $break_(some(_combine)); \
    }; \
}) $unscoped_(expr)

#if UNUSED_CODE
#define collect_(/*(_gpa)*/) __step__collect_(__step__collect___gpa __VA_ARGS__)
#define __step__collect_(...) __step__collect$__emit(__VA_ARGS__)
#define __step__collect$__emit(...) __collect$(__VA_ARGS__)
#define __collect$(_gpa...) ({ \
    if (__chain_state == chain__State_none) { \
        asg_lit((&__reserved_buf)(ok({}))); \
    } else if (__chain_state == chain__State_init) { \
        __reserved_buf = u_castE$((TypeOf(__reserved_buf))(mem_Allocator_alloc(_gpa, typeInfo$(InnerT), ))); \
    } else { \
    }; \
})

#define collectFixed_(/*_out*/...) __step__collectFixed_(__step__collectFixed___out __VA_ARGS__)

#define collectWithin_(/*_out*/...) __step__collectWithin___emit(__VA_ARGS__)
#define __step__collectWithin___emit(...) __collectWithin_(__VA_ARGS__)
#define __collectWithin_(_out...) expr_(TypeOf(__reserved_acc) $scope)({ \
    if (__chain_state != chain__State_eval) { \
        let_(__out, TypeOf(__reserved_acc)) = _out; \
        __reserved_buf_cap = __out.len; \
        asg_lit((&__reserved_buf)(ok(.as_raw = sliceS(__out, $r(0, 0)).as_raw))); \
        $break_(sliceS(__out, $r(0, 0))); \
    } else { \
        let __buf = catch_((E_asP(&__reserved_buf))($ignore, claim_unreachable)); \
        claim_assert(__buf->len <= __reserved_buf_cap); \
        __buf->len++; \
        *S_at((*__buf)[__buf->len - 1]) = *__reserved; \
        $break_(sliceS(*__buf, $r(0, __buf->len))); \
    }; \
}) $unscoped_(expr)
#endif /* UNUSED_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_chain__included */
