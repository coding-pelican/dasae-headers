#include "dh/main.h"

typedef struct Foo {
    i32 a;
    i32 b;
} Foo;

union Model {
    struct {
        Foo*  n0;
        usize n1;
    } _field;
    struct {
        Foo*  foo;
        usize index;
    };
};

fn_((getModel(void))(union Model)) {
    static usize index = 0;
    static Foo   foo   = { .a = 1, .b = 2 };
    return (union Model){
        .foo   = &foo,
        .index = index
    };
}

#define tuple(field0, field1...) blk({ \
    (struct { \
        TypeOf(field0) _n0; \
        TypeOf(field1) _n1; \
    }){ \
        ._n0 = field0, \
        ._n1 = field1 \
    }; \
})
#define tuple_at(tuple_addr, index) blk({ \
    let        __$tuple = tuple_addr; \
    blk_return __$tuple->_n##index; \
})
#define tuple_atMut(tuple_addr, index) blk({ \
    let __$tuple = tuple_addr; \
    blk_return & __$tuple->_n##index; \
})
#define untuple(tuple, name0, name1...) \
    let __$tuple = tuple; \
    name0        = __$tuple._n0; \
    name1        = __$tuple._n1

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    {
        let __$ret = getModel();
        let a      = __$ret._field.n0;
        let b      = __$ret._field.n1;
        let_ignore = a;
        let_ignore = b;
    }

    {
        var a = 1;
        var b = 2;
        let c = tuple(a, b);
        untuple(c, var n0, let n1);
        let_ignore = n0;
        let_ignore = n1;

        var d      = tuple_at(&c, 0);
        let e      = tuple_atMut(&c, 0);
        let_ignore = d;
        let_ignore = e;
    }

    return_ok({});
} $unscoped_(fn);
