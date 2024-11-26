/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    gen_expt.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-20 (date of creation)
 * @updated 2024-11-20 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  gen
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef GEN_INCLUDED
#define GEN_INCLUDED (1)

/*========== Core Macro Utilities =========================================*/

#define meta_concat(a, b...)      IMPL_meta_concat(a, b)
#define IMPL_meta_concat(a, b...) a##b

#define meta_concat3(a, b, c...)      IMPL_meta_concat3(a, b, c)
#define IMPL_meta_concat3(a, b, c...) a##b##c

#define meta_stringify(x)      IMPL_meta_stringify(x)
#define IMPL_meta_stringify(x) #x

#define meta_wrapParens(x...)      IMPL_meta_wrapParens(x)
#define IMPL_meta_wrapParens(x...) (x)

#define meta_unwrapParens(x...)      IMPL_meta_unwrapParens(x)
#define IMPL_meta_unwrapParens(x...) x

/*========== Type Definition Helpers ======================================*/

// Define wrapped type with added members
#define gen_Type(BaseType, T, Alias) \
    typedef union Alias {            \
        BaseType Base[1];            \
        struct {                     \
            gen_fields(BaseType)(T)  \
        };                           \
    } Alias

// Define a method Alias for the wrapper
#define gen_fn(T, fn) \
    meta_concat3(T, _, fn)

#define gen_fields(T) \
    meta_concat3(T, _, fields)

#define gen_methods(T) \
    meta_concat3(T, _, methods)

/*========== Function Wrapper Generators ==================================*/

// Generate wrapper for a function that returns the same type
#define gen_wrapFn(BaseType, T, Alias, fn, params, args, RetType)       \
    static_inline RetType gen_fn(Alias, fn)(meta_unwrapParens params) { \
        return gen_fn(BaseType, fn)(meta_unwrapParens args);            \
    }

// Generate wrapper for a function that converts return type
#define gen_wrapFnConvert(BaseType, T, Alias, fn, params, args, RetType) \
    static_inline RetType gen_fn(Alias, fn)(meta_unwrapParens params) {  \
        return (RetType)gen_fn(BaseType, fn)(meta_unwrapParens args);    \
    }

// Generate multiple wrappers from a list of function signatures
#define gen_wrapFnAll(BaseType, T, Alias) \
    gen_methods(BaseType)(BaseType, T, Alias)

/*========== Generic Collection Generator ================================*/

// Define a collection generator that works for any collection type
#define gen_COLLECTION(Base, T, Alias, BaseType_fields, method_gen) \
    gen_Type(Base, T, Alias, BaseType_fields);                      \
    gen_wrapFnAll(Alias, Base, method_gen)

// Use it to generate any collection type
#define gen_ARR_FOR(T, Alias) \
    gen_COLLECTION(ds_Arr, T, Alias, ARR_EXTRA_MEMBERS(T), gen_ARR_METHODS)

#define gen_LIST_FOR(T, Alias) \
    gen_COLLECTION(ds_List, T, Alias, LIST_EXTRA_MEMBERS(T), gen_LIST_METHODS)

/*========== Extended Utilities =========================================*/

// Generate comparison operators
#define gen_OPERATORS(Alias, T)                                                           \
    static inline bool Alias##_eq(const Alias* a, const Alias* b) {                       \
        return memcmp(Alias##_data(a), Alias##_data(b), Alias##_len(a) * sizeof(T)) == 0; \
    }                                                                                     \
    /* Add more operators as needed */

// Generate common iterators
#define gen_ITERATORS(Alias, T)                          \
    static inline T* Alias##_begin(Alias* vec) {         \
        return Alias##_mut_data(vec);                    \
    }                                                    \
    static inline T* Alias##_end(Alias* vec) {           \
        return Alias##_mut_data(vec) + Alias##_len(vec); \
    }                                                    \
    /* Add more iterator utilities */

// Full feature generation
#define gen_VEC_FULL(T, Alias) \
    gen_type_for(T, Alias);    \
    gen_OPERATORS(Alias, T);   \
    gen_ITERATORS(Alias, T)

#endif // GEN_INCLUDED
