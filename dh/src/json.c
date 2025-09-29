#include "dh/json.h"
#include "dh/debug.h"
#include "dh/mem.h"
#include "dh/Str.h"
#include "dh/fmt.h"
#include "dh/variant.h"

// =============================================================================
// Value Creation and Destruction
// =============================================================================

fn_(json_Value_initNull(void), json_Value $scope) {
    return_(variant_of(json_Value_null, {}));
} $unscoped;

fn_(json_Value_initBool(bool value), json_Value $scope) {
    return_(variant_of(json_Value_bool, value));
} $unscoped;

fn_(json_Value_initNumber(f64 value), json_Value $scope) {
    return_(variant_of(json_Value_number, value));
} $unscoped;

fn_(json_Value_initString(mem_Allocator allocator, Sli_const$u8 str), Err$json_Value $scope) {
    json_Value_Str new_str = {};
    try_(json_Str_initData(&new_str, str, allocator));
    return_ok(variant_of(json_Value_string, new_str));
} $unscoped

fn_(json_Value_finiString(json_Value value), void) {
    debug_assert(json_Value_isString(value));
    let str = variant_as(&value, json_Value_string);
    json_Str_fini(str, str->allocator);
}

fn_(json_Value_initArray(mem_Allocator allocator), Err$json_Value $scope) {
    json_Value_Arr arr = {};
    try_(json_Arr_reserve(&arr, 0, allocator));
    return_ok(variant_of(json_Value_array, arr));
} $unscoped;

fn_(json_Value_finiArray(json_Value value), void) { /* NOLINT */
    debug_assert(json_Value_isArray(value));
    let arr = variant_as(&value, json_Value_array);
    for_slice (arr->items, item) {
        json_Value_freeAll(*item);
    }
    if (arr->items.ptr) {
        mem_Allocator_free(arr->allocator, anySli(arr->items));
    }
}

fn_(json_Value_initObject(mem_Allocator allocator), Err$json_Value $scope) {
    json_Value_Obj obj = {};
    try_(json_Obj_reserve(&obj, 0, allocator));
    return_ok(variant_of(json_Value_object, obj));
} $unscoped;

fn_(json_Value_finiObject(json_Value value), void) { /* NOLINT */
    debug_assert(json_Value_isObject(value));
    let obj = variant_as(&value, json_Value_object);
    for_slice (obj->entries, entry) {
        json_Str_fini(&entry->key, entry->key.allocator);
        json_Value_freeAll(entry->value);
    }
    if (obj->entries.ptr) {
        mem_Allocator_free(obj->allocator, anySli(obj->entries));
    }
}

// =============================================================================
// Value Inspection
// =============================================================================

fn_(json_Value_isNull(json_Value self), bool) { return self.tag == json_Value_null; }
fn_(json_Value_isBool(json_Value self), bool) { return self.tag == json_Value_bool; }
fn_(json_Value_isNumber(json_Value self), bool) { return self.tag == json_Value_number; }
fn_(json_Value_isString(json_Value self), bool) { return self.tag == json_Value_string; }
fn_(json_Value_isArray(json_Value self), bool) { return self.tag == json_Value_array; }
fn_(json_Value_isObject(json_Value self), bool) { return self.tag == json_Value_object; }

// =============================================================================
// Value Conversion
// =============================================================================

fn_(json_Value_asBool(json_Value self), Opt$bool $scope) {
    if (json_Value_isBool(self)) {
        return_some(variant_extract(self, json_Value_bool));
    }
    return_none();
} $unscoped;

fn_(json_Value_asNumber(json_Value self), Opt$f64 $scope) {
    if (json_Value_isNumber(self)) {
        return_some(variant_extract(self, json_Value_number));
    }
    return_none();
} $unscoped;

fn_(json_Value_asString(json_Value self), Opt$Ptr$json_Str $scope) {
    if (json_Value_isString(self)) {
        return_some(variant_as(&self, json_Value_string));
    }
    return_none();
} $unscoped;

fn_(json_Value_asArray(json_Value self), Opt$Ptr$json_Arr $scope) {
    if (json_Value_isArray(self)) {
        return_some(variant_as(&self, json_Value_array));
    }
    return_none();
} $unscoped;

fn_(json_Value_asObject(json_Value self), Opt$Ptr$json_Obj $scope) {
    if (json_Value_isObject(self)) {
        return_some(variant_as(&self, json_Value_object));
    }
    return_none();
} $unscoped;

// =============================================================================
// Value Utilities
// =============================================================================

fn_(json_Value_validate(json_Value self), Err$void $scope) {
    // TODO: Implement validation logic
    $ignore = self;
    return_err(Err_NotImplemented());
} $unscoped;

fn_(json_Value_getPath(json_Value self, Sli_const$u8 path), Opt$Ptr$json_Value $scope) {
    // TODO: Implement path parsing logic
    $ignore = self;
    $ignore = path;
    return_none();
} $unscoped;

fn_(json_Value_getTypeName(json_Value self), Sli_const$u8) {
    match_(self, {
        pattern_(json_Value_null, return u8_l("null"));
        pattern_(json_Value_bool, return u8_l("bool"));
        pattern_(json_Value_number, return u8_l("number"));
        pattern_(json_Value_string, return u8_l("string"));
        pattern_(json_Value_array, return u8_l("array"));
        pattern_(json_Value_object, return u8_l("object"));
        fallback_(claim_unreachable);
    });
};

fn_(json_Value_eql(json_Value self, json_Value other), bool) {
    if (self.tag != other.tag) { return false; }
    match_(self, {
        pattern_(json_Value_null, return true);
        pattern_(json_Value_bool, (value), return *value == variant_extract(other, json_Value_bool));
        pattern_(json_Value_number, (value), return *value == variant_extract(other, json_Value_number));
        pattern_(json_Value_string, (value), return json_Str_eql(value, variant_as(&other, json_Value_string)));
        pattern_(json_Value_array, (value), return json_Arr_eql(value, variant_as(&other, json_Value_array)));
        pattern_(json_Value_object, (value), return json_Obj_eql(value, variant_as(&other, json_Value_object)));
        fallback_(claim_unreachable);
    });
}

fn_(json_Value_clone(json_Value self, mem_Allocator allocator), Err$json_Value $scope) { /* NOLINT */
    match_(self, {
        pattern_(json_Value_null, return_ok(self));
        pattern_(json_Value_bool, return_ok(variant_of(json_Value_bool, variant_extract(self, json_Value_bool))));
        pattern_(json_Value_number, return_ok(variant_of(json_Value_number, variant_extract(self, json_Value_number))));
        pattern_(json_Value_string, (value), {
            json_Value_Str new_str = {};
            try_(json_Str_initData(&new_str, value->data.as_const, allocator));
            return_ok(variant_of(json_Value_string, new_str));
        });
        pattern_(json_Value_array, (value), {
            json_Value_Arr new_arr = {};
            try_(json_Arr_reserve(&new_arr, value->items.len, allocator));
            for_slice (value->items, (item)) {
                let new_item = try_(json_Value_clone(*item, allocator));
                try_(json_Arr_push(&new_arr, new_item, allocator));
            }
            return_ok(variant_of(json_Value_array, new_arr));
        });
        pattern_(json_Value_object, (value), {
            json_Value_Obj new_obj = {};
            try_(json_Obj_reserve(&new_obj, value->entries.len, allocator));
            for_slice (value->entries, (entry)) {
                let new_entry = ((json_Value_ObjEntry){
                    .key   = entry->key,
                    .value = try_(json_Value_clone(entry->value, allocator)),
                });
                try_(json_Obj_add(&new_obj, new_entry.key.data.as_const, new_entry.value, allocator));
            }
            return_ok(variant_of(json_Value_object, new_obj));
        });
        fallback_(claim_unreachable);
    });
} $unscoped;

fn_(json_Value_freeAll(json_Value self), void) { /* NOLINT */
    match_(self, {
        pattern_(json_Value_null, $fallthrough);
        pattern_(json_Value_bool, $fallthrough);
        pattern_(json_Value_number, return);
        pattern_(json_Value_string, return json_Value_finiString(self));
        pattern_(json_Value_array, return json_Value_finiArray(self));
        pattern_(json_Value_object, return json_Value_finiObject(self));
        fallback_(claim_unreachable);
    });
}

// =============================================================================
// Parser
// =============================================================================

fn_(json_Parser_init(mem_Allocator allocator), json_Parser) {
    return (json_Parser){
        .allocator    = allocator,
        .current_char = none(),
        .line         = 1,
        .col          = 0
    };
}

fn_(json_Parser_parse(json_Parser parser), Err$json_Value) {
    return json_Parser_parseWithOpts(parser, json_parse_default_options);
}

fn_(json_Parser_parseWithOpts(json_Parser parser, json_ParseOptions options), Err$json_Value) {
    $ignore = (parser);
    $ignore = (options);
    // TODO: Implement parser
    return err$(Err$json_Value, Err_NotImplemented());
}

// =============================================================================
// Stringifier
// =============================================================================

fn_(json_Strfier_init(io_Writer writer), json_Strfier) {
    return (json_Strfier){
        .writer        = writer,
        .indent_level  = 0,
        .pretty_print  = false,
        .indent_string = Str_fromZ((u8*)"  ").as_const
    };
}

fn_(json_Strfier_strfy(json_Strfier strfier, json_Value value), Err$usize) {
    return json_Strfier_strfyWithOpts(strfier, value, json_strfy_default_options);
}

fn_(json_Strfier_strfyWithOpts(json_Strfier strfier, json_Value value, json_StrfyOptions options), Err$usize) {
    $ignore = (strfier);
    $ignore = (value);
    $ignore = (options);
    // TODO: Implement stringifier
    return err$(Err$usize, Err_NotImplemented());
}

// =============================================================================
// String
// =============================================================================

fn_(json_Str_init(json_Str* self, mem_Allocator allocator), Err$void $scope) {
    self->allocator = allocator;
    self->data      = (Sli$u8){ .ptr = null, .len = 0 };
    self->cap       = 0;
    return_ok({});
} $unscoped;

fn_(json_Str_initCap(json_Str* self, usize capacity, mem_Allocator allocator), Err$void $scope) {
    self->allocator = allocator;
    if (capacity == 0) {
        self->data = (Sli$u8){ .ptr = null, .len = 0 };
        self->cap  = 0;
    } else {
        self->data     = meta_cast$(Sli$u8, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), capacity)));
        self->data.len = 0; // It's empty for now
        self->cap      = capacity;
    }
    return_ok({});
} $unscoped

fn_(json_Str_initData(json_Str* self, Sli_const$u8 str, mem_Allocator allocator), Err$void $scope) {
    try_(json_Str_initCap(self, str.len, allocator));
    mem_copy(self->data.ptr, str.ptr, str.len);
    self->data.len = str.len;
    return_ok({});
} $unscoped

fn_(json_Str_fini(json_Str* self, mem_Allocator allocator), void) {
    if (self->data.ptr) {
        mem_Allocator_free(allocator, anySli(self->data));
    }
}

fn_(json_Str_append(json_Str* self, json_Str* other, mem_Allocator allocator), Err$void) {
    return json_Str_appendStr(self, other->data.as_const, allocator);
}

fn_(json_Str_appendStr(json_Str* self, Sli_const$u8 str, mem_Allocator allocator), Err$void $scope) {
    // TODO: implement growth logic
    $ignore = (self);
    $ignore = (str);
    $ignore = (allocator);
    return_err(Err_NotImplemented());
} $unscoped

fn_(json_Str_eql(const json_Str* self, const json_Str* other), bool) {
    $ignore = (self);
    $ignore = (other);
    return false;
    // return Sli_eql(u8, self->data.as_const, other->data.as_const);
}

fn_(json_Str_eqlStr(const json_Str* self, Sli_const$u8 str), bool) {
    $ignore = (self);
    $ignore = (str);
    return false;
    // return Sli_eql(u8, self->data.as_const, str);
}

// =============================================================================
// Array
// =============================================================================

fn_(json_Arr_reserve(json_Arr* self, usize capacity, mem_Allocator allocator), Err$void) {
    $ignore = (self);
    $ignore = (capacity);
    $ignore = (allocator);
    return err$(Err$void, Err_NotImplemented());
}

fn_(json_Arr_push(json_Arr* self, json_Value value, mem_Allocator allocator), Err$void) {
    $ignore = (self);
    $ignore = (value);
    $ignore = (allocator);
    return err$(Err$void, Err_NotImplemented());
}

fn_(json_Arr_pop(json_Arr* self), Opt$Ptr$json_Value $scope) {
    $ignore = (self);
    return_none();
} $unscoped;

fn_(json_Arr_clear(json_Arr* self), void) {
    for_slice (self->items, item) {
        json_Value_freeAll(*item);
    }
    self->items.len = 0;
}

fn_(json_Arr_at(const json_Arr* self, usize index), Opt$Ptr$json_Value $scope) {
    if (index < self->items.len) {
        return_some(&self->items.ptr[index]);
    }
    return_none();
} $unscoped;

fn_(json_Arr_len(const json_Arr* self), usize) {
    return self->items.len;
}

fn_(json_Arr_eql(const json_Arr* self, const json_Arr* other), bool) {
    $ignore = (self);
    $ignore = (other);
    return false;
}

// =============================================================================
// Object
// =============================================================================

fn_(json_Obj_reserve(json_Obj* self, usize capacity, mem_Allocator allocator), Err$void) {
    $ignore = (self);
    $ignore = (capacity);
    $ignore = (allocator);
    return err$(Err$void, Err_NotImplemented());
}

fn_(json_Obj_add(json_Obj* self, Sli_const$u8 key, json_Value value, mem_Allocator allocator), Err$void) {
    $ignore = (self);
    $ignore = (key);
    $ignore = (value);
    $ignore = (allocator);
    return err$(Err$void, Err_NotImplemented());
}

fn_(json_Obj_remove(json_Obj* self, Sli_const$u8 key), bool) {
    $ignore = (self);
    $ignore = (key);
    return false;
}

fn_(json_Obj_clear(json_Obj* self, mem_Allocator allocator), void) {
    $ignore = (allocator);
    for_slice (self->entries, entry) {
        json_Str_fini(&entry->key, entry->key.allocator);
        json_Value_freeAll(entry->value);
    }
    self->entries.len = 0;
}

fn_(json_Obj_has(const json_Obj* self, Sli_const$u8 key), bool) {
    return isSome(json_Obj_get(self, key));
}

fn_(json_Obj_set(json_Obj* self, Sli_const$u8 key, json_Value value), Err$void) {
    $ignore = (self);
    $ignore = (key);
    $ignore = (value);
    return err$(Err$void, Err_NotImplemented());
}

fn_(json_Obj_get(const json_Obj* self, Sli_const$u8 key), Opt$Ptr$json_Value $scope) {
    for_slice (self->entries, entry) {
        if (json_Str_eqlStr(&entry->key, key)) {
            return_some(&entry->value);
        }
    }
    return_none();
} $unscoped;

fn_(json_Obj_len(const json_Obj* self), usize) {
    return self->entries.len;
}

fn_(json_Obj_eql(const json_Obj* self, const json_Obj* other), bool) {
    $ignore = (self);
    $ignore = (other);
    return false;
}

// =============================================================================
// Iterators
// =============================================================================

fn_(json_Arr_iter(const json_Arr* self), json_ArrIter) {
    return (json_ArrIter){ .ctx = self, .index = 0 };
}

fn_(json_ArrIter_next(json_ArrIter* self), Opt$Ptr$json_Value $scope) {
    if (self->index < self->ctx->items.len) {
        return_some(&self->ctx->items.ptr[self->index++]);
    }
    return_none();
} $unscoped;

fn_(json_Obj_iter(const json_Obj* self), json_ObjIter) {
    return (json_ObjIter){ .ctx = self, .index = 0 };
}

fn_(json_ObjIter_next(json_ObjIter* self), Opt$Ptr$json_ObjEntry $scope) {
    if (self->index < self->ctx->entries.len) {
        return_some(&self->ctx->entries.ptr[self->index++]);
    }
    return_none();
} $unscoped;

// =============================================================================
// Default Options
// =============================================================================

const json_ParseOptions json_parse_default_options = {
    .allow_comments        = false,
    .allow_trailing_commas = false,
    .allow_unquoted_keys   = false,
    .max_depth             = 0
};

const json_StrfyOptions json_strfy_default_options = {
    .pretty_print   = false,
    .indent_string  = { .ptr = (const u8*)"  ", .len = 2 },
    .escape_unicode = false,
    .escape_slash   = false
};

const json_StrfyOptions json_strfy_pretty_options = {
    .pretty_print   = true,
    .indent_string  = { .ptr = (const u8*)"  ", .len = 2 },
    .escape_unicode = false,
    .escape_slash   = false
};
