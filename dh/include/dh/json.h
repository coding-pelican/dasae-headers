/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    json.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-20 (date of creation)
 * @updated 2025-06-20 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  json
 */

#ifndef JSON_INCLUDED
#define JSON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/mem/Allocator.h"
#include "dh/io/Writer.h"

/*========== Macros and Declarations ========================================*/

// Error codes constants
config_ErrSet(json_Err,
    InvalidSyntax,
    InvalidNumber,
    InvalidString,
    InvalidUtf8,
    UnexpectedEof,
    TooDeep
);

// Forward declarations
typedef struct json_Value          json_Value;
typedef struct json_Value_Str      json_Value_Str;
typedef struct json_Value_Arr      json_Value_Arr;
typedef struct json_Value_Obj      json_Value_Obj;
typedef struct json_Value_ObjEntry json_Value_ObjEntry;

// Type aliases
typedef json_Value_Str      json_Str;
typedef json_Value_Arr      json_Arr;
typedef json_Value_Obj      json_Obj;
typedef json_Value_ObjEntry json_ObjEntry;

// JSON string representation
struct json_Value_Str {
    mem_Allocator allocator;
    Sli$u8        data;
    usize         cap;
};
use_Opt$(json_Str);
// JSON array
struct json_Value_Arr {
    mem_Allocator allocator;
    Sli$$(json_Value) items;
    usize cap;
};
use_Opt$(json_Arr);
// JSON object
struct json_Value_Obj {
    mem_Allocator allocator;
    Sli$$(json_Value_ObjEntry) entries;
    usize cap;
};
use_Opt$(json_Obj);
// JSON value (using variant)
variant_(
    (json_Value),
    (json_Value_null, Void),
    (json_Value_bool, bool),
    (json_Value_number, f64),
    (json_Value_string, json_Value_Str),
    (json_Value_array, json_Value_Arr),
    (json_Value_object, json_Value_Obj)
);
use_Opt$(json_Value);
use_Err$(json_Value);

// Value creation helpers
extern fn_((json_Value_initNull(void))(json_Value));
extern fn_((json_Value_initBool(bool value))(json_Value));
extern fn_((json_Value_initNumber(f64 value))(json_Value));
extern fn_((json_Value_initString(mem_Allocator allocator, Sli_const$u8 str))(Err$json_Value)) $must_check;
extern fn_((json_Value_finiString(json_Value value))(void));
extern fn_((json_Value_initArray(mem_Allocator allocator))(Err$json_Value)) $must_check;
extern fn_((json_Value_finiArray(json_Value value))(void));
extern fn_((json_Value_initObject(mem_Allocator allocator))(Err$json_Value)) $must_check;
extern fn_((json_Value_finiObject(json_Value value))(void));

// Type checking methods
extern fn_((json_Value_isNull(json_Value self))(bool));
extern fn_((json_Value_isBool(json_Value self))(bool));
extern fn_((json_Value_isNumber(json_Value self))(bool));
extern fn_((json_Value_isString(json_Value self))(bool));
extern fn_((json_Value_isArray(json_Value self))(bool));
extern fn_((json_Value_isObject(json_Value self))(bool));

// Type conversion helpers
extern fn_((json_Value_asBool(json_Value self))(Opt$bool));
extern fn_((json_Value_asNumber(json_Value self))(Opt$f64));
extern fn_((json_Value_asString(json_Value self))(Opt$Ptr$json_Str));
extern fn_((json_Value_asArray(json_Value self))(Opt$Ptr$json_Arr));
extern fn_((json_Value_asObject(json_Value self))(Opt$Ptr$json_Obj));

// Validate JSON value structure
extern fn_((json_Value_validate(json_Value self))(Err$void)) $must_check;
// Path-based access (e.g., "user.name" or "items[0].id")
extern fn_((json_Value_getPath(json_Value self, Sli_const$u8 path))(Opt$Ptr$json_Value));
// Get string representation of JSON type
extern fn_((json_Value_getTypeName(json_Value self))(Sli_const$u8));
// Check if JSON value equals another
extern fn_((json_Value_eql(json_Value self, json_Value other))(bool));
// Deep copy a JSON value
extern fn_((json_Value_clone(json_Value self, mem_Allocator allocator))(Err$json_Value)) $must_check;
// Free a JSON value and all its children
extern fn_((json_Value_freeAll(json_Value self))(void));

// JSON object key-value pair
struct json_Value_ObjEntry {
    json_Value_Str key;
    json_Value     value;
};
use_Opt$(json_ObjEntry);

// JSON parser state (without io_Reader for now since it's commented out)
typedef struct json_Parser {
    mem_Allocator allocator;
    Opt$u8        current_char;
    usize         line;
    usize         col;
} json_Parser;
// Parser options
typedef struct json_ParseOptions {
    bool  allow_comments;        // Allow // and /* */ comments
    bool  allow_trailing_commas; // Allow trailing commas in arrays/objects
    bool  allow_unquoted_keys;   // Allow unquoted object keys
    usize max_depth;             // Maximum nesting depth (0 = unlimited)
} json_ParseOptions;
// Initialize parser with allocator (removed io_Reader until it's available)
extern fn_((json_Parser_init(mem_Allocator allocator))(json_Parser));
// Parse JSON with default options
extern fn_((json_Parser_parse(json_Parser parser))(Err$json_Value)) $must_check;
// Parse JSON from the reader
extern fn_((json_Parser_parseWithOpts(json_Parser parser, json_ParseOptions options))(Err$json_Value)) $must_check;

// JSON strfier state
typedef struct json_Strfier {
    io_Writer    writer;
    i32          indent_level;
    bool         pretty_print;
    Sli_const$u8 indent_string;
} json_Strfier;
// Strfier options
typedef struct json_StrfyOptions {
    bool         pretty_print;   // Enable pretty printing with indentation
    Sli_const$u8 indent_string;  // String to use for indentation (default: "  ")
    bool         escape_unicode; // Escape non-ASCII characters as \uXXXX
    bool         escape_slash;   // Escape forward slashes as \/
} json_StrfyOptions;
// Initialize strfier with writer (interface passed by value)
extern fn_((json_Strfier_init(io_Writer writer))(json_Strfier));
// strfy JSON with default options
extern fn_((json_Strfier_strfy(json_Strfier strfier, json_Value value))(Err$usize)) $must_check;
// strfy JSON value to writer
extern fn_((json_Strfier_strfyWithOpts(json_Strfier strfier, json_Value value, json_StrfyOptions options))(Err$usize)) $must_check;

// String functions
extern fn_((json_Str_init(json_Str* self, mem_Allocator allocator))(Err$void)) $must_check;
extern fn_((json_Str_initCap(json_Str* self, usize capacity, mem_Allocator allocator))(Err$void)) $must_check;
extern fn_((json_Str_initData(json_Str* self, Sli_const$u8 str, mem_Allocator allocator))(Err$void)) $must_check;
extern fn_((json_Str_fini(json_Str* self, mem_Allocator allocator))(void));
extern fn_((json_Str_append(json_Str* self, json_Str* other, mem_Allocator allocator))(Err$void)) $must_check;
extern fn_((json_Str_appendStr(json_Str* self, Sli_const$u8 str, mem_Allocator allocator))(Err$void)) $must_check;
extern fn_((json_Str_eql(const json_Str* self, const json_Str* other))(bool));
extern fn_((json_Str_eqlStr(const json_Str* self, Sli_const$u8 str))(bool));

// Array operations
// Reserve capacity in array
extern fn_((json_Arr_reserve(json_Arr* self, usize capacity, mem_Allocator allocator))(Err$void)) $must_check;
// Push value to array
extern fn_((json_Arr_push(json_Arr* self, json_Value value, mem_Allocator allocator))(Err$void)) $must_check;
// Pop value from array
extern fn_((json_Arr_pop(json_Arr* self))(Opt$Ptr$json_Value));
// Clear array
extern fn_((json_Arr_clear(json_Arr* self))(void));
// Get value from array by index
extern fn_((json_Arr_at(const json_Arr* self, usize index))(Opt$Ptr$json_Value));
// Get array length
extern fn_((json_Arr_len(const json_Arr* self))(usize));
// Check if array equals another
extern fn_((json_Arr_eql(const json_Arr* self, const json_Arr* other))(bool));

// Object operations
// Reserve capacity in object
extern fn_((json_Obj_reserve(json_Obj* self, usize capacity, mem_Allocator allocator))(Err$void)) $must_check;
// Add key-value pair to object
extern fn_((json_Obj_add(json_Obj* self, Sli_const$u8 key, json_Value value, mem_Allocator allocator))(Err$void)) $must_check;
// Remove key-value pair from object
extern fn_((json_Obj_remove(json_Obj* self, Sli_const$u8 key))(bool));
// Remove all key-value pairs from object
extern fn_((json_Obj_clear(json_Obj* self, mem_Allocator allocator))(void));
// Check if object has key
extern fn_((json_Obj_has(const json_Obj* self, Sli_const$u8 key))(bool));
// Set key-value pair in object
extern fn_((json_Obj_set(json_Obj* self, Sli_const$u8 key, json_Value value))(Err$void)) $must_check;
// Get value from object by key
extern fn_((json_Obj_get(const json_Obj* self, Sli_const$u8 key))(Opt$Ptr$json_Value));
// Get object size
extern fn_((json_Obj_len(const json_Obj* self))(usize));
// Check if object equals another
extern fn_((json_Obj_eql(const json_Obj* self, const json_Obj* other))(bool));

// Array iterator
typedef struct json_ArrIter {
    const json_Arr* ctx;
    usize           index;
} json_ArrIter;
extern fn_((json_Arr_iter(const json_Arr* self))(json_ArrIter));
extern fn_((json_ArrIter_next(json_ArrIter* self))(Opt$Ptr$json_Value));

// Object iterator
typedef struct json_ObjIter {
    const json_Obj* ctx;
    usize           index;
} json_ObjIter;
extern fn_((json_Obj_iter(const json_Obj* self))(json_ObjIter));
extern fn_((json_ObjIter_next(json_ObjIter* self))(Opt$Ptr$json_ObjEntry));

// Default options constants
extern const json_ParseOptions json_parse_default_options;
extern const json_StrfyOptions json_strfy_default_options;
extern const json_StrfyOptions json_strfy_pretty_options;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* JSON_INCLUDED */
