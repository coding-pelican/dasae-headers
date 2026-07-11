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
#pragma once
#ifndef core_chain__included
#define core_chain__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pipe.h"
#include "scope.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((chain__State $fits($packed))(
    chain__State_undefined = 0,
    chain__State_init = 1,
    chain__State_eval = 2,
)) chain__State;

#define chain$(/*(_$T)(_$s)(_$fnOps...)*/...) __step__chain$(__step__chain$__parse __VA_ARGS__)

#define each_(/*(_$p_e)_$body*/...) __step__each_(__step__each___capt __VA_ARGS__)
#define filter_(/*(_$p_e)_$pred*/...) __step__filter_(__step__filter___capt __VA_ARGS__)
#define map$(/*(_$T)(_$p_e)_$xform*/...) __step__map$(__step__map$__parse __VA_ARGS__)
#define fold_(/*(_$default), (_$acc, _$p_e)_$combine*/...) __step__fold_(__VA_ARGS__)
#define reduce_(/*(_$acc, _$p_e)_$combine*/...) __step__reduce_(__VA_ARGS__)
#define all_(/*(_$p_e)_$pred*/...) __step__all_(__step__all___capt __VA_ARGS__)
#define any_(/*(_$p_e)_$pred*/...) __step__any_(__step__any___capt __VA_ARGS__)
#if UNUSED_CODE
#define collect_(/*(_$gpa)*/...) __step__collect_(__step__collect___gpa __VA_ARGS__)
#define collectFixed_(/*(_$out)*/...) __step__collectFixed_(__step__collectFixed___out __VA_ARGS__)
#define collectWithin_(/*(_$out)*/...) __step__collectWithin_(__step__collectWithin___out __VA_ARGS__)
#endif /* UNUSED_CODE */

/*========== Macros and Definitions =========================================*/

#define __step__chain$(...) __step__chain$__emit(__VA_ARGS__)
#define __step__chain$__parse(_$T...) _$T, __step__chain$__parseS
#define __step__chain$__parseS(_$s...) pp_uniqTok(s), _$s, __step__chain$__parseFnOps
#define __step__chain$__parseFnOps(_$fnOps...) _$fnOps
#define __step__chain$__emit(...) __chain$(__VA_ARGS__)
#define __chain$(_$T, __s, _$s, ...) $suppress_compound_token_split_by_macro( \
    pp_overload(__chain$, __VA_ARGS__)(_$T, __s, _$s, __VA_ARGS__) \
)
#define __chain$_1(_$T, __s, _$s, _$fnOp1...) ({ \
    let __s = _$s; \
    var_(__reserved_acc, _$T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_$T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_undefined : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        let __curr = _$fnOp1; \
        asg_l((&__reserved_acc)(__curr)); \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_$s))(__curr)) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        let __curr = _$fnOp1; \
        asg_l((&__reserved_acc)(__curr)); \
    } $end(for); \
    __reserved_acc; \
})
#define __chain$_2(_$T, __s, _$s, _$fnOp1, _$fnOp2...) ({ \
    let __s = _$s; \
    var_(__reserved_acc, _$T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_$T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_undefined : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            let __curr = _$fnOp2; \
            asg_l((&__reserved_acc)(__curr)); \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_$s))(__curr)) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            let __curr = _$fnOp2; \
            asg_l((&__reserved_acc)(__curr)); \
        }; \
    } $end(for); \
    __reserved_acc; \
})
#define __chain$_3(_$T, __s, _$s, _$fnOp1, _$fnOp2, _$fnOp3...) ({ \
    let __s = _$s; \
    var_(__reserved_acc, _$T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_$T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_undefined : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                let __curr = _$fnOp3; \
                asg_l((&__reserved_acc)(__curr)); \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_$s))(__curr)) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                let __curr = _$fnOp3; \
                asg_l((&__reserved_acc)(__curr)); \
            }; \
        }; \
    } $end(for); \
    __reserved_acc; \
})
#define __chain$_4(_$T, __s, _$s, _$fnOp1, _$fnOp2, _$fnOp3, _$fnOp4...) ({ \
    let __s = _$s; \
    var_(__reserved_acc, _$T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_$T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_undefined : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _$fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    let __curr = _$fnOp4; \
                    asg_l((&__reserved_acc)(__curr)); \
                }; \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_$s))(__curr)) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _$fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    let __curr = _$fnOp4; \
                    asg_l((&__reserved_acc)(__curr)); \
                }; \
            }; \
        }; \
    } $end(for); \
    __reserved_acc; \
})
#define __chain$_5(_$T, __s, _$s, _$fnOp1, _$fnOp2, _$fnOp3, _$fnOp4, _$fnOp5...) ({ \
    let __s = _$s; \
    var_(__reserved_acc, _$T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_$T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_undefined : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _$fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _$fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        let __curr = _$fnOp5; \
                        asg_l((&__reserved_acc)(__curr)); \
                    }; \
                }; \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_$s))(__curr)) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _$fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _$fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        let __curr = _$fnOp5; \
                        asg_l((&__reserved_acc)(__curr)); \
                    }; \
                }; \
            }; \
        }; \
    } $end(for); \
    __reserved_acc; \
})
#define __chain$_6(_$T, __s, _$s, _$fnOp1, _$fnOp2, _$fnOp3, _$fnOp4, _$fnOp5, _$fnOp6...) ({ \
    let __s = _$s; \
    var_(__reserved_acc, _$T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_$T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_undefined : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _$fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _$fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _$fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            let __curr = _$fnOp6; \
                            asg_l((&__reserved_acc)(__curr)); \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_$s))(__curr)) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _$fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _$fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _$fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            let __curr = _$fnOp6; \
                            asg_l((&__reserved_acc)(__curr)); \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    } $end(for); \
    __reserved_acc; \
})
#define __chain$_7(_$T, __s, _$s, _$fnOp1, _$fnOp2, _$fnOp3, _$fnOp4, _$fnOp5, _$fnOp6, _$fnOp7...) ({ \
    let __s = _$s; \
    var_(__reserved_acc, _$T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_$T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_undefined : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _$fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _$fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _$fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            $maybe_unused let __curr = _$fnOp6; \
                            { \
                                let_(__reserved, TypeOf(__curr)) = __curr; \
                                let __curr = _$fnOp7; \
                                asg_l((&__reserved_acc)(__curr)); \
                            }; \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_$s))(__curr)) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _$fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _$fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _$fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            $maybe_unused let __curr = _$fnOp6; \
                            { \
                                let_(__reserved, TypeOf(__curr)) = __curr; \
                                let __curr = _$fnOp7; \
                                asg_l((&__reserved_acc)(__curr)); \
                            }; \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    } $end(for); \
    __reserved_acc; \
})
#define __chain$_8(_$T, __s, _$s, _$fnOp1, _$fnOp2, _$fnOp3, _$fnOp4, _$fnOp5, _$fnOp6, _$fnOp7, _$fnOp8...) ({ \
    let __s = _$s; \
    var_(__reserved_acc, _$T) = {}; \
    $maybe_unused var_(__reserved_buf, E$$(S$$(_$T))) = {}; \
    $maybe_unused var_(__reserved_buf_cap, usize) = 0; \
    var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_undefined : chain__State_init; \
    do { \
        let_(__reserved, TypeOf(__s.ptr)) = __s.ptr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _$fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _$fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _$fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            $maybe_unused let __curr = _$fnOp6; \
                            { \
                                let_(__reserved, TypeOf(__curr)) = __curr; \
                                $maybe_unused let __curr = _$fnOp7; \
                                { \
                                    let_(__reserved, TypeOf(__curr)) = __curr; \
                                    let __curr = _$fnOp8; \
                                    asg_l((&__reserved_acc)(__curr)); \
                                }; \
                            }; \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    } while (false); \
    __chain_state = chain__State_eval; \
    for_(($s(_$s))(__curr)) { \
        let_(__reserved, TypeOf(__curr)) = __curr; \
        $maybe_unused let __curr = _$fnOp1; \
        { \
            let_(__reserved, TypeOf(__curr)) = __curr; \
            $maybe_unused let __curr = _$fnOp2; \
            { \
                let_(__reserved, TypeOf(__curr)) = __curr; \
                $maybe_unused let __curr = _$fnOp3; \
                { \
                    let_(__reserved, TypeOf(__curr)) = __curr; \
                    $maybe_unused let __curr = _$fnOp4; \
                    { \
                        let_(__reserved, TypeOf(__curr)) = __curr; \
                        $maybe_unused let __curr = _$fnOp5; \
                        { \
                            let_(__reserved, TypeOf(__curr)) = __curr; \
                            $maybe_unused let __curr = _$fnOp6; \
                            { \
                                let_(__reserved, TypeOf(__curr)) = __curr; \
                                $maybe_unused let __curr = _$fnOp7; \
                                { \
                                    let_(__reserved, TypeOf(__curr)) = __curr; \
                                    let __curr = _$fnOp8; \
                                    asg_l((&__reserved_acc)(__curr)); \
                                }; \
                            }; \
                        }; \
                    }; \
                }; \
            }; \
        }; \
    } $end(for); \
    __reserved_acc; \
})

#define __step__each_(...) __step__each___emit(__VA_ARGS__)
#define __step__each___capt(_$p_e...) _$p_e,
#define __step__each___emit(...) __each_(__VA_ARGS__)
#define __each_(_$p_e, _$body...) ({ \
    if (__chain_state != chain__State_eval) { \
    } else { \
        let _$p_e = __reserved; \
        _$body; \
    }; \
    __reserved; \
})

#define __step__filter_(...) __step__filter___emit(__VA_ARGS__)
#define __step__filter___capt(_$p_e...) _$p_e,
#define __step__filter___emit(...) __filter_(__VA_ARGS__)
#define __filter_(_$p_e, _$pred...) ({ \
    if (__chain_state != chain__State_eval) { \
    } else { \
        let _$p_e = __reserved; \
        if (!(_$pred)) { continue; }; \
    }; \
    __reserved; \
})

#define __step__map$(...) __step__map$__emit(__VA_ARGS__)
#define __step__map$__parse(_$T...) _$T, __step__map$__parseP
#define __step__map$__parseP(_$p_e...) _$p_e,
#define __step__map$__emit(...) __map$(__VA_ARGS__)
#define __map$(_$T, _$p_e, _$xform...) expr_(_$T* $scope)({ \
    if (__chain_state != chain__State_eval) { \
        $break_(&l0$((_$T))); \
    } else { \
        let _$p_e = __reserved; \
        $break_(&copy(_$xform)); \
    }; \
}) $unscoped(expr)

#define __step__fold_(_$default, ...) __step__fold___emit( \
    _$default, __step__fold___capt __VA_ARGS__ \
)
#define __step__fold___default(_$default...) _$default
#define __step__fold___capt(_$acc, _$p_e...) _$acc, _$p_e,
#define __step__fold___emit(...) __fold_(__VA_ARGS__)
#define __fold_(_$default, _$acc, _$p_e, _$combine...) expr_(TypeOf(__reserved_acc) $scope)({ \
    if (__chain_state != chain__State_eval) { \
        $break_(__step__fold___default _$default); \
    } else { \
        let _$p_e = __reserved; \
        var _$acc = __reserved_acc; \
        $break_(_$combine); \
    }; \
}) $unscoped(expr)
/* TODO: support tryFold_ */

#define __step__reduce_(...) __step__reduce___emit(__step__reduce___capt __VA_ARGS__)
#define __step__reduce___capt(_$acc, _$p_e...) _$acc, _$p_e,
#define __step__reduce___emit(...) __reduce_(__VA_ARGS__)
#define __reduce_(_$acc, _$p_e, _$combine...) expr_(TypeOf(__reserved_acc) $scope)({ \
    if (__chain_state != chain__State_eval) { \
        $break_(none()); \
    } else { \
        if (isNone(__reserved_acc)) { \
            $break_(some(*__reserved)); \
            continue; \
        }; \
        let _$p_e = __reserved; \
        var _$acc = orelse_((__reserved_acc)(*__reserved)); \
        $break_(some(_$combine)); \
    }; \
}) $unscoped(expr)
/* TODO: support tryReduce_ */

#define __step__all_(...) __step__all___emit(__VA_ARGS__)
#define __step__all___capt(_$p_e...) _$p_e,
#define __step__all___emit(...) __all_(__VA_ARGS__)
#define __all_(_$p_e, _$pred...) fold_((true), (__acc, _$p_e)(__acc && _$pred))

#define __step__any_(...) __step__any___emit(__VA_ARGS__)
#define __step__any___capt(_$p_e...) _$p_e,
#define __step__any___emit(...) __any_(__VA_ARGS__)
#define __any_(_$p_e, _$pred...) fold_((false), (__acc, _$p_e)(__acc || _$pred))

#if UNUSED_CODE
#define collect_(/*(_$gpa)*/) __step__collect_(__step__collect___gpa __VA_ARGS__)
#define __step__collect_(...) __step__collect$__emit(__VA_ARGS__)
#define __step__collect$__emit(...) __collect$(__VA_ARGS__)
#define __collect$(_$gpa...) ({ \
    if (__chain_state == chain__State_undefined) { \
        asg_l((&__reserved_buf)(ok({}))); \
    } else if (__chain_state == chain__State_init) { \
        __reserved_buf = u_castE$((TypeOf(__reserved_buf))(mem_Alctr_alloc(_$gpa, typeInfo$(InnerT), ))); \
    } else { \
    }; \
})

#define collectFixed_(/*_$out*/...) __step__collectFixed_(__step__collectFixed___out __VA_ARGS__)

#define collectWithin_(/*_$out*/...) __step__collectWithin___emit(__VA_ARGS__)
#define __step__collectWithin___emit(...) __collectWithin_(__VA_ARGS__)
#define __collectWithin_(_$out...) expr_(TypeOf(__reserved_acc) $scope)({ \
    if (__chain_state != chain__State_eval) { \
        let_(__out, TypeOf(__reserved_acc)) = _$out; \
        __reserved_buf_cap = __out.len; \
        asg_l((&__reserved_buf)(ok(.as_raw = S_slice((__out)($r(0, 0))).as_raw))); \
        $break_(S_slice((__out)($r(0, 0)))); \
    } else { \
        let __buf = catch_((E_asP(&__reserved_buf))($ignore, claim_unreachable)); \
        claim_assert(__buf->len <= __reserved_buf_cap); \
        __buf->len++; \
        *S_at((*__buf)[__buf->len - 1]) = *__reserved; \
        $break_(S_slice((*__buf)($r(0, __buf->len)))); \
    }; \
}) $unscoped(expr)
#endif /* UNUSED_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_chain__included */
