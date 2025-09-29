#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

// Forward declarations
typedef struct MyData         MyData;
typedef struct MyStorage      MyStorage;
typedef struct Field          Field;
typedef struct BinaryOperator BinaryOperator;
typedef struct StorageField   StorageField;
typedef struct RunningSum     RunningSum;
typedef struct StorageDict    StorageDict;
typedef struct IndexDict      IndexDict;

// MyData structure
struct MyData {
    double value;
};

// MyStorage structure
struct MyStorage {
    char*   type_name;
    MyData* data;
    int     size;
    int     capacity;
};

// Field structure
struct Field {
    char* storage_type;
    char* access_path;
    int   access_offset;
    char* sub_field;
};

// BinaryOperator structure
struct BinaryOperator {
    char op_type;
};

// StorageField structure
struct StorageField {
    char*  storage_name;
    Field* field;
};

// Dictionary structures
struct StorageDict {
    char**      keys;
    MyStorage** values;
    int         size;
};

struct IndexDict {
    char** keys;
    int*   values;
    int    size;
};

// RunningSum structure
struct RunningSum {
    StorageField*   storage_fields;
    int             storage_fields_count;
    BinaryOperator* bi_operators;
    int             bi_operators_count;
    IndexDict       last_indices;
    int             begin_offset;
    int             end_offset;
    int             reserved_size_for_nested;
    double*         calc_buffer;
    double*         result_buffer;
};

// MyData functions
MyData* MyData_create(double value) {
    MyData* data = (MyData*)malloc(sizeof(MyData));
    data->value  = value;
    return data;
}

// MyStorage functions
MyStorage* MyStorage_create(const char* type_name) {
    MyStorage* storage = (MyStorage*)malloc(sizeof(MyStorage));
    storage->type_name = strdup(type_name);
    storage->capacity  = 10;
    storage->data      = (MyData*)malloc(sizeof(MyData) * storage->capacity);
    storage->size      = 0;
    return storage;
}

void MyStorage_append(MyStorage* storage, MyData* values, int count) {
    while (storage->size + count > storage->capacity) {
        storage->capacity *= 2;
        storage->data = (MyData*)realloc(storage->data, sizeof(MyData) * storage->capacity);
    }
    for (int i = 0; i < count; i++) {
        storage->data[storage->size++] = values[i];
    }
}

void MyStorage_update(MyStorage* storage, MyData* value) {
    if (storage->size > 0) {
        storage->data[storage->size - 1] = *value;
    }
}

void MyStorage_destroy(MyStorage* storage) {
    free(storage->type_name);
    free(storage->data);
    free(storage);
}

// Field functions
Field* Field_create(const char* storage_type, const char* access_path, int access_offset, const char* sub_field) {
    Field* field         = (Field*)malloc(sizeof(Field));
    field->storage_type  = strdup(storage_type);
    field->access_path   = strdup(access_path);
    field->access_offset = access_offset;
    field->sub_field     = strdup(sub_field);
    return field;
}

double* Field_accessAbsolute(Field* field, MyStorage* storage, int index) {
    assert(strcmp(storage->type_name, field->storage_type) == 0);
    if (index < 0 || index >= storage->size) {
        return NULL;
    }

    // For simplicity, we assume access_path is "data" and sub_field is "value"
    double* value = &(storage->data[index].value);
    if (isnan(*value)) {
        return NULL;
    }
    return value;
}

double* Field_access(Field* field, MyStorage* storage, int index) {
    return Field_accessAbsolute(field, storage, index + field->access_offset);
}

void Field_destroy(Field* field) {
    free(field->storage_type);
    free(field->access_path);
    free(field->sub_field);
    free(field);
}

// BinaryOperator functions
BinaryOperator* BinaryOperator_create(char op_type) {
    BinaryOperator* op = (BinaryOperator*)malloc(sizeof(BinaryOperator));
    op->op_type        = op_type;
    return op;
}

double BinaryOperator_evaluate(BinaryOperator* op, double lhs, double rhs) {
    switch (op->op_type) {
    case '+':
        return lhs + rhs;
    case '-':
        return lhs - rhs;
    default:
        assert(0);
    }
}

// Dictionary functions
StorageDict StorageDict_create(int initial_capacity) {
    StorageDict dict;
    dict.keys   = (char**)malloc(sizeof(char*) * initial_capacity);
    dict.values = (MyStorage**)malloc(sizeof(MyStorage*) * initial_capacity);
    dict.size   = 0;
    return dict;
}

void StorageDict_set(StorageDict* dict, const char* key, MyStorage* value) {
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->keys[i], key) == 0) {
            dict->values[i] = value;
            return;
        }
    }
    dict->keys[dict->size]   = strdup(key);
    dict->values[dict->size] = value;
    dict->size++;
}

MyStorage* StorageDict_get(StorageDict* dict, const char* key) {
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->keys[i], key) == 0) {
            return dict->values[i];
        }
    }
    return NULL;
}

IndexDict IndexDict_create(int initial_capacity) {
    IndexDict dict;
    dict.keys   = (char**)malloc(sizeof(char*) * initial_capacity);
    dict.values = (int*)malloc(sizeof(int) * initial_capacity);
    dict.size   = 0;
    return dict;
}

void IndexDict_set(IndexDict* dict, const char* key, int value) {
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->keys[i], key) == 0) {
            dict->values[i] = value;
            return;
        }
    }
    dict->keys[dict->size]   = strdup(key);
    dict->values[dict->size] = value;
    dict->size++;
}

int IndexDict_get(IndexDict* dict, const char* key) {
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->keys[i], key) == 0) {
            return dict->values[i];
        }
    }
    return -1;
}

// RunningSum functions
int RunningSum_windowSize(RunningSum* rs) {
    return rs->begin_offset - rs->end_offset + 1;
}

RunningSum* RunningSum_create(StorageField* storage_fields, int sf_count, BinaryOperator* bi_operators, int bo_count, int begin_offset, int end_offset, int reserved_size_for_nested) {
    RunningSum* rs = (RunningSum*)malloc(sizeof(RunningSum));

    rs->storage_fields           = storage_fields;
    rs->storage_fields_count     = sf_count;
    rs->bi_operators             = bi_operators;
    rs->bi_operators_count       = bo_count;
    rs->begin_offset             = begin_offset;
    rs->end_offset               = end_offset;
    rs->reserved_size_for_nested = reserved_size_for_nested;

    rs->last_indices = IndexDict_create(10);

    int window_size   = RunningSum_windowSize(rs);
    rs->calc_buffer   = (double*)malloc(sizeof(double) * window_size);
    rs->result_buffer = (double*)malloc(sizeof(double) * reserved_size_for_nested);

    for (int i = 0; i < window_size; i++) {
        rs->calc_buffer[i] = NAN;
    }
    for (int i = 0; i < reserved_size_for_nested; i++) {
        rs->result_buffer[i] = NAN;
    }

    return rs;
}

double RunningSum_evaluateExpr(RunningSum* rs, StorageDict* storages, IndexDict* indices) {
    __auto_type getValue = ^double(int term) {
        char*      storage_name = rs->storage_fields[term].storage_name;
        Field*     field        = rs->storage_fields[term].field;
        MyStorage* storage      = StorageDict_get(storages, storage_name);
        int        index        = IndexDict_get(indices, storage_name);
        double*    value        = Field_access(field, storage, index);
        if (value == NULL) {
            return NAN;
        }
        return *value;
    };

    if (rs->bi_operators_count == 0) {
        return getValue(0);
    }

    double evaluated = getValue(0);
    for (int term = 0; term < rs->bi_operators_count; term++) {
        double rhs = getValue(term + 1);
        evaluated  = BinaryOperator_evaluate(&rs->bi_operators[term], evaluated, rhs);
    }
    return evaluated;
}

void RunningSum_getCurrIndices(StorageDict* storages, IndexDict* result) {
    for (int i = 0; i < storages->size; i++) {
        IndexDict_set(result, storages->keys[i], storages->values[i]->size);
    }
}

int RunningSum_getCurrDiff(RunningSum* rs, StorageDict* storages) {
    char*      name       = rs->storage_fields[0].storage_name;
    MyStorage* storage    = StorageDict_get(storages, name);
    int        last_index = IndexDict_get(&rs->last_indices, name);
    return storage->size - last_index;
}

void printBuffer(const char* name, double* buffer, int size) {
    printf("   %s: [", name);
    for (int i = 0; i < size; i++) {
        if (isnan(buffer[i])) {
            printf("nan");
        } else {
            printf("%.1f", buffer[i]);
        }
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

double RunningSum_evaluate(RunningSum* rs, StorageDict* storages) {
    IndexDict curr_indices = IndexDict_create(10);
    RunningSum_getCurrIndices(storages, &curr_indices);

    int window_size = RunningSum_windowSize(rs);

    if (rs->last_indices.size == 0) {
        // First evaluation
        int min_index = curr_indices.values[0];
        for (int i = 1; i < curr_indices.size; i++) {
            if (curr_indices.values[i] < min_index) {
                min_index = curr_indices.values[i];
            }
        }

        int traverse_len = min_index;
        if (traverse_len > rs->reserved_size_for_nested + window_size - 1) {
            traverse_len = rs->reserved_size_for_nested + window_size - 1;
        }

        if (traverse_len < window_size) {
            return NAN;
        }

        rs->last_indices = curr_indices;

        double sum = 0.0;
        for (int idx = 0; idx < window_size; idx++) {
            IndexDict indices = IndexDict_create(10);
            for (int i = 0; i < rs->last_indices.size; i++) {
                int index_value = (rs->last_indices.values[i] - traverse_len + 1) + idx;
                IndexDict_set(&indices, rs->last_indices.keys[i], index_value);
            }

            double evaled_calc = RunningSum_evaluateExpr(rs, storages, &indices);

            // Shift calc_buffer left and append new value
            for (int i = 0; i < window_size - 1; i++) {
                rs->calc_buffer[i] = rs->calc_buffer[i + 1];
            }
            rs->calc_buffer[window_size - 1] = evaled_calc;
            sum += evaled_calc;

            printf("curr state:\n");
            printBuffer("calc_buffer", rs->calc_buffer, window_size);
            printBuffer("result_buffer", rs->result_buffer, rs->reserved_size_for_nested);
        }

        // Shift result_buffer left and append sum
        for (int i = 0; i < rs->reserved_size_for_nested - 1; i++) {
            rs->result_buffer[i] = rs->result_buffer[i + 1];
        }
        rs->result_buffer[rs->reserved_size_for_nested - 1] = sum;

        printf("curr state:\n");
        printBuffer("calc_buffer", rs->calc_buffer, window_size);
        printBuffer("result_buffer", rs->result_buffer, rs->reserved_size_for_nested);

        for (int idx = window_size; idx < traverse_len; idx++) {
            IndexDict indices = IndexDict_create(10);
            for (int i = 0; i < rs->last_indices.size; i++) {
                int index_value = (rs->last_indices.values[i] - traverse_len + 1) + idx;
                IndexDict_set(&indices, rs->last_indices.keys[i], index_value);
            }

            double evaled_calc = RunningSum_evaluateExpr(rs, storages, &indices);
            double popped_calc = rs->calc_buffer[0];

            // Shift calc_buffer left and append new value
            for (int i = 0; i < window_size - 1; i++) {
                rs->calc_buffer[i] = rs->calc_buffer[i + 1];
            }
            rs->calc_buffer[window_size - 1] = evaled_calc;

            double recent_result = rs->result_buffer[rs->reserved_size_for_nested - 1];
            double new_result    = recent_result + evaled_calc - popped_calc;

            // Shift result_buffer left and append new result
            for (int i = 0; i < rs->reserved_size_for_nested - 1; i++) {
                rs->result_buffer[i] = rs->result_buffer[i + 1];
            }
            rs->result_buffer[rs->reserved_size_for_nested - 1] = new_result;

            printf("curr state:\n");
            printBuffer("calc_buffer", rs->calc_buffer, window_size);
            printBuffer("result_buffer", rs->result_buffer, rs->reserved_size_for_nested);
        }

        return rs->result_buffer[rs->reserved_size_for_nested - 1];
    }

    // Subsequent evaluations
    double reeval_calc                                  = RunningSum_evaluateExpr(rs, storages, &rs->last_indices);
    double recent_calc                                  = rs->calc_buffer[window_size - 1];
    rs->calc_buffer[window_size - 1]                    = reeval_calc;
    double recent_result                                = rs->result_buffer[rs->reserved_size_for_nested - 1];
    rs->result_buffer[rs->reserved_size_for_nested - 1] = recent_result + reeval_calc - recent_calc;

    int jumped_interval = RunningSum_getCurrDiff(rs, storages);
    int traverse_len    = window_size + rs->reserved_size_for_nested - 1;

    if (traverse_len <= jumped_interval) {
        int offset = jumped_interval - traverse_len;
        for (int i = 0; i < rs->last_indices.size; i++) {
            rs->last_indices.values[i] += offset;
        }
        jumped_interval = traverse_len;
    }

    for (int j = 0; j < jumped_interval; j++) {
        for (int i = 0; i < rs->last_indices.size; i++) {
            rs->last_indices.values[i] += 1;
        }

        double evaled_calc = RunningSum_evaluateExpr(rs, storages, &rs->last_indices);
        double popped_calc = rs->calc_buffer[0];

        // Shift calc_buffer left and append new value
        for (int i = 0; i < window_size - 1; i++) {
            rs->calc_buffer[i] = rs->calc_buffer[i + 1];
        }
        rs->calc_buffer[window_size - 1] = evaled_calc;

        recent_result     = rs->result_buffer[rs->reserved_size_for_nested - 1];
        double new_result = recent_result + evaled_calc - popped_calc;

        // Shift result_buffer left and append new result
        for (int i = 0; i < rs->reserved_size_for_nested - 1; i++) {
            rs->result_buffer[i] = rs->result_buffer[i + 1];
        }
        rs->result_buffer[rs->reserved_size_for_nested - 1] = new_result;
    }

    return rs->result_buffer[rs->reserved_size_for_nested - 1];
}

void printStorage(const char* name, MyStorage* storage) {
    printf("- %s: (data=[", name);
    for (int i = 0; i < storage->size; i++) {
        printf("(value=%.1f)", storage->data[i].value);
        if (i < storage->size - 1) {
            printf(", ");
        }
    }
    printf("])\n");
}

void printRunningSumState(RunningSum* rs, StorageDict* storages) {
    printf("storage_fields: [");
    for (int i = 0; i < rs->storage_fields_count; i++) {
        printf("(%s, Field(%s, %s, %d, %s))", rs->storage_fields[i].storage_name, rs->storage_fields[i].field->storage_type, rs->storage_fields[i].field->access_path, rs->storage_fields[i].field->access_offset, rs->storage_fields[i].field->sub_field);
        if (i < rs->storage_fields_count - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printStorage("a", StorageDict_get(storages, "a"));
    printStorage("b", StorageDict_get(storages, "b"));
    printStorage("c", StorageDict_get(storages, "c"));

    printf("bi_operators: [");
    for (int i = 0; i < rs->bi_operators_count; i++) {
        printf("(op_type=%c)", rs->bi_operators[i].op_type);
        if (i < rs->bi_operators_count - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("last_indices: {");
    for (int i = 0; i < rs->last_indices.size; i++) {
        printf("'%s': %d", rs->last_indices.keys[i], rs->last_indices.values[i]);
        if (i < rs->last_indices.size - 1) {
            printf(", ");
        }
    }
    printf("}\n");

    printf("begin_offset: %d\n", rs->begin_offset);
    printf("end_offset: %d\n", rs->end_offset);
    printf("window_range: range(%d, %d)\n", rs->end_offset, rs->begin_offset + 1);
    printf("window_size: %d\n", RunningSum_windowSize(rs));
    printf("reserved_size_for_nested: %d\n", rs->reserved_size_for_nested);

    IndexDict curr_indices = IndexDict_create(10);
    RunningSum_getCurrIndices(storages, &curr_indices);
    printf("curr_indices: {");
    for (int i = 0; i < curr_indices.size; i++) {
        printf("'%s': %d", curr_indices.keys[i], curr_indices.values[i]);
        if (i < curr_indices.size - 1) {
            printf(", ");
        }
    }
    printf("}\n");

    printf("curr_diff: %d\n", RunningSum_getCurrDiff(rs, storages));
    printBuffer("calc_buffer", rs->calc_buffer, RunningSum_windowSize(rs));
    printBuffer("result_buffer", rs->result_buffer, rs->reserved_size_for_nested);
}

int main() {
    // Create storages
    MyStorage* a        = MyStorage_create("MyStorageA");
    MyData     a_data[] = { { 1 }, { 2 }, { 3 } };
    MyStorage_append(a, a_data, 3);

    MyStorage* b        = MyStorage_create("MyStorageB");
    MyData     b_data[] = { { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 } };
    MyStorage_append(b, b_data, 6);

    MyStorage* c        = MyStorage_create("MyStorageC");
    MyData     c_data[] = { { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 }, { 7 }, { 8 }, { 9 } };
    MyStorage_append(c, c_data, 9);

    StorageDict storages = StorageDict_create(10);
    StorageDict_set(&storages, "a", a);
    StorageDict_set(&storages, "b", b);
    StorageDict_set(&storages, "c", c);

    // Create storage fields
    StorageField storage_fields[] = {
        { "a", Field_create("MyStorageA", "data", -1, "value") },
        { "b", Field_create("MyStorageB", "data", -4, "value") },
        { "c", Field_create("MyStorageC", "data", -7, "value") }
    };

    // Create binary operators
    BinaryOperator bi_operators[] = {
        { '+' },
        { '+' }
    };

    // Create RunningSum
    RunningSum* running_sum = RunningSum_create(
        storage_fields, 3,
        bi_operators, 2,
        -1, -3, 3
    );

    // First evaluation
    printf("--------------------------------\n");
    double result = RunningSum_evaluate(running_sum, &storages);
    printf("%.1f\n", result);
    printRunningSumState(running_sum, &storages);

    // Append data
    MyData a_append[] = { { 4 }, { 5 } };
    MyStorage_append(a, a_append, 2);
    MyData b_append[] = { { 7 }, { 8 } };
    MyStorage_append(b, b_append, 2);
    MyData c_append[] = { { 10 }, { 11 } };
    MyStorage_append(c, c_append, 2);

    printf("--------------------------------\n");
    result = RunningSum_evaluate(running_sum, &storages);
    printf("%.1f\n", result);
    printRunningSumState(running_sum, &storages);

    // Update data
    MyData a_update = { 6 };
    MyStorage_update(a, &a_update);
    MyData b_update = { 9 };
    MyStorage_update(b, &b_update);
    MyData c_update = { 12 };
    MyStorage_update(c, &c_update);

    printf("--------------------------------\n");
    result = RunningSum_evaluate(running_sum, &storages);
    printf("%.1f\n", result);
    printRunningSumState(running_sum, &storages);

    // Large append
    MyData a_large[] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 1 }, { 2 }, { 3 } };
    MyStorage_append(a, a_large, 9);
    MyData b_large[] = { { 0 }, { 0 }, { 0 }, { 1 }, { 2 }, { 3 }, { 0 }, { 0 }, { 0 } };
    MyStorage_append(b, b_large, 9);
    MyData c_large[] = { { 1 }, { 2 }, { 3 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
    MyStorage_append(c, c_large, 9);

    printf("--------------------------------\n");
    result = RunningSum_evaluate(running_sum, &storages);
    printf("%.1f\n", result);
    printRunningSumState(running_sum, &storages);

    // Cleanup would go here in production code
    // (freeing all allocated memory)

    return 0;
}
