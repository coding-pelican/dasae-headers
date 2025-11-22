/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    sample-decision_tree.c
 * @author  Gyeongtae Kim (@dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-07 (date of creation)
 * @updated 2025-03-27 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/samples
 * @prefix  (none)
 *
 * @brief   Decision tree implementation for classification
 * @details This file implements a basic decision tree classifier that can:
 *          - Build a tree from training data
 *          - Make predictions on new data
 *          - Save/load trees to/from files
 *          - Handle CSV data input
 */

#include "dh/main.h"
#include "dh/log.h"

#include "dh/mem.h"
#include "dh/heap/Page.h"
#include "dh/ArrList.h"

// Decision tree structures
typedef struct TreeNode_Decision TreeNode_Decision;
typedef struct TreeNode_Leaf TreeNode_Leaf;
typedef variant_((TreeNode)(
    (TreeNode_decision, struct TreeNode_Decision {
        variant_(TreeNode $T)* left;
        variant_(TreeNode $T)* right;
        u32 feature_index;
        f32 threshold;
    }),
    (TreeNode_leaf, struct TreeNode_Leaf { i32 class_label; })
)) TreeNode;
T_use_P$(TreeNode);
T_use_E$(P$TreeNode);
// Forward declarations
$static fn_((TreeNode_createLeaf(mem_Allocator gpa, i32 class_label))(E$P$TreeNode)) $must_check;
$static fn_((TreeNode_createDecision(mem_Allocator gpa, u32 feature_index, f32 threshold, TreeNode* left, TreeNode* right))(E$P$TreeNode)) $must_check;
$static fn_((TreeNode_destroyRecur(mem_Allocator gpa, TreeNode* node))(void));
$static fn_((TreeNode_predict(const TreeNode* node, const f32* features, u32 n_features))(i32));
$static fn_((TreeNode_printRecur(const TreeNode* node, u32 depth))(void));
$static fn_((TreeNode_saveToFileRecur(const TreeNode* node, FILE* file))(E$void)) $must_check;
$static fn_((TreeNode_loadFromFileRecur(mem_Allocator gpa, FILE* file))(E$P$TreeNode)) $must_check;

// Dataset structure
typedef struct Dataset {
    mem_Allocator gpa;
    ArrList$$(f32) features;
    ArrList$$(i32) labels;
    u32 n_samples;
    u32 n_features;
} Dataset;
T_use_E$(Dataset);
// Forward declarations
$static fn_((Dataset_loadFromCSV(mem_Allocator gpa, S_const$u8 filename, bool has_header))(E$Dataset)) $must_check;
$static fn_((Dataset_destroy(Dataset* dataset))(void));

// Main function
fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    // Initialize logging to a file
    try_(log_init("log/debug.log"));
    defer_(log_fini());
    {
        // Configure logging behavior
        log_setLevel(log_Level_debug);
        log_showTimestamp(true);
        log_showLevel(true);
        log_showLocation(false);
        log_showFunction(true);
    }
    log_info("Starting decision tree application");

    // Create gpa
    var gpa = heap_Page_allocator(&(heap_Page){});

    // Load dataset if a filename was provided, otherwise create a demo tree
    if (1 < args.len) {
        log_info("Loading dataset from %s", atS(args, 1)->ptr);
        var dataset = try_(Dataset_loadFromCSV(gpa, *atS(args, 1), true));
        defer_(Dataset_destroy(&dataset));

        // Here normally build the tree from the dataset
        // For simplicity, we'll just create a demo tree
        log_info("Loaded %u samples with %u features", dataset.n_samples, dataset.n_features);

        // Demo tree creation would go here
    }

    // Create a simple decision tree (manually for demo purposes)
    // In a real implementation, this would be learned from data
    log_info("Creating demo decision tree");
    let leaf_setosa = try_(TreeNode_createLeaf(gpa, 0));
    let leaf_versicolor = try_(TreeNode_createLeaf(gpa, 1));
    let leaf_virginica = try_(TreeNode_createLeaf(gpa, 2));

    let versicolor_virginica = try_(TreeNode_createDecision(
        gpa,
        3,     // feature index (petal width)
        1.75f, // threshold
        leaf_versicolor,
        leaf_virginica
    ));

    let root = try_(TreeNode_createDecision(
        gpa,
        2,     // feature index (petal length)
        2.45f, // threshold
        leaf_setosa,
        versicolor_virginica
    ));

    defer_(TreeNode_destroyRecur(gpa, root));

    // Print the tree
    log_info("Decision tree structure:");
    TreeNode_printRecur(root, 0);

    // Save the tree to a file
    with_(var save_file = fopen("decision_tree.bin", "wb")) {
        if (!save_file) {
            log_error("Failed to open file for writing: decision_tree.bin");
            return_err(fs_File_Err_OpenFailed());
        }

        try_(TreeNode_saveToFileRecur(root, save_file));
        let_ignore = fclose(save_file);
        log_info("Saved decision tree to decision_tree.bin");
    }

    // Load the tree from the file
    let loaded_tree = blk({
        var load_file = fopen("decision_tree.bin", "rb");
        if (!load_file) {
            log_error("Failed to open file for reading: decision_tree.bin");
            return_err(fs_File_Err_OpenFailed());
        }

        let loaded = try_(TreeNode_loadFromFileRecur(gpa, load_file));
        let_ignore = fclose(load_file);
        log_info("Loaded decision tree from decision_tree.bin");

        blk_return loaded;
    });
    defer_(TreeNode_destroyRecur(gpa, loaded_tree));

    // Verify the loaded tree by printing it
    log_info("Loaded tree structure:");
    TreeNode_printRecur(loaded_tree, 0);

    // Make some predictions
    const f32 samples[3][4] = {
        { 5.1f, 3.5f, 1.4f, 0.2f }, // Setosa
        { 6.3f, 3.3f, 4.7f, 1.6f }, // Versicolor
        { 6.5f, 3.0f, 5.2f, 2.0f }  // Virginica
    };

    for (u32 i = 0; i < 3; ++i) {
        let prediction = TreeNode_predict(root, samples[i], 4);
        log_info("Sample %u: Class %d", i + 1, prediction);
    }

    log_info("Decision tree application completed successfully");
    return_ok({});
} $unguarded_(fn);

// Implementation of TreeNode functions
fn_((TreeNode_createLeaf(mem_Allocator gpa, i32 class_label))(E$P$TreeNode) $scope) {
    let node = try_(u_castE$((E$P$TreeNode)(mem_Allocator_create(gpa, typeInfo$(TreeNode)))));
    variant_asg(
        node,
        union_of((TreeNode_leaf){
            .class_label = class_label,
        })
    );
    return_ok(node);
} $unscoped_(fn);

fn_((TreeNode_createDecision(mem_Allocator gpa, u32 feature_index, f32 threshold, TreeNode* left, TreeNode* right))(E$P$TreeNode) $scope) {
    let node = try_(u_castE$((E$P$TreeNode)(mem_Allocator_create(gpa, typeInfo$(TreeNode)))));
    variant_asg(
        node,
        union_of((TreeNode_decision){
            .feature_index = feature_index,
            .threshold = threshold,
            .left = left,
            .right = right,
        })
    );
    return_ok(node);
} $unscoped_(fn);

fn_((TreeNode_destroyRecur(mem_Allocator gpa, TreeNode* node))(void)) /* NOLINT(misc-no-recursion) */ {
    if (node == null) { return; }
    match_(deref(node)) {
        pattern_((TreeNode_leaf)($ignore)) break $end(pattern);
        pattern_((TreeNode_decision)(decision)) {
            TreeNode_destroyRecur(gpa, decision->left);
            TreeNode_destroyRecur(gpa, decision->right);
            break;
        } $end(pattern);
        default_() {
            log_error("Invalid node type encountered during prediction");
            claim_unreachable;
        } $end(default);
    } $end(match);
    mem_Allocator_destroy(gpa, u_anyP(node));
}

fn_((TreeNode_predict(const TreeNode* node, const f32* features, u32 n_features))(i32)) {
    var current = node;
    while (true) {
        match_(deref(current)) {
            pattern_((TreeNode_leaf)(leaf)) {
                return leaf->class_label;
            } $end(pattern);
            pattern_((TreeNode_decision)(decision)) {
                if (decision->feature_index >= n_features) {
                    log_error("Feature index out of bounds: %u >= %u", decision->feature_index, n_features);
                    claim_unreachable;
                }

                if (features[decision->feature_index] <= decision->threshold) {
                    current = decision->left;
                } else {
                    current = decision->right;
                }
                continue;
            } $end(pattern);
            default_() {
                log_error("Invalid node type encountered during prediction");
                claim_unreachable;
            } $end(default);
        } $end(match);
    }
}

fn_((TreeNode_printRecur(const TreeNode* node, u32 depth))(void)) /* NOLINT(misc-no-recursion) */ {
    if (node == null) { return; }

    // Create indentation
    char indent[64] = { 0 };
    for (u32 i = 0; i < depth && i < 31; ++i) {
        strcat(indent, "  ");
    }

    match_(*node) {
        pattern_((TreeNode_leaf)(leaf)) {
            log_info("%sClass: %d", indent, leaf->class_label);
            break;
        } $end(pattern);
        pattern_((TreeNode_decision)(decision)) {
            log_info("%sFeature %u <= %.2f", indent, decision->feature_index, decision->threshold);
            TreeNode_printRecur(decision->left, depth + 1);

            log_info("%sFeature %u > %.2f", indent, decision->feature_index, decision->threshold);
            TreeNode_printRecur(decision->right, depth + 1);
            break;
        } $end(pattern);
        default_() {
            log_error("%sInvalid node type", indent);
            claim_unreachable;
        } $end(default);
    } $end(match);
}

fn_((TreeNode_saveToFileRecur(const TreeNode* node, FILE* file))(E$void) $scope) /* NOLINT(misc-no-recursion) */ {
    if (fwrite(&node->tag, sizeof(node->tag), 1, file) != 1) {
        log_error("Failed to write node tag");
        return_err(fs_File_Err_WriteFailed());
    }

    match_(*node) {
        pattern_((TreeNode_leaf)(leaf)) {
            if (fwrite(&leaf->class_label, sizeof(leaf->class_label), 1, file) != 1) {
                log_error("Failed to write leaf data");
                return_err(fs_File_Err_WriteFailed());
            }
            break;
        } $end(pattern);
        pattern_((TreeNode_decision)(decision)) {
            if (fwrite(&decision->feature_index, sizeof(decision->feature_index), 1, file) != 1
                || fwrite(&decision->threshold, sizeof(decision->threshold), 1, file) != 1) {
                log_error("Failed to write decision node data");
                return_err(fs_File_Err_WriteFailed());
            }
            // Recursively save children
            try_(TreeNode_saveToFileRecur(decision->left, file));
            try_(TreeNode_saveToFileRecur(decision->right, file));
            break;
        } $end(pattern);
        default_() {
            log_error("Invalid node type encountered during saving");
            claim_unreachable;
        } $end(default);
    } $end(match);

    return_ok({});
} $unscoped_(fn);

fn_((TreeNode_loadFromFileRecur(mem_Allocator gpa, FILE* file))(E$P$TreeNode) $scope) /* NOLINT(misc-no-recursion) */ {
    let tag = blk({
        int tag = 0;
        if (fread(&tag, sizeof(tag), 1, file) != 1) {
            log_error("Failed to read node tag");
            return_err(fs_File_Err_ReadFailed());
        }
        blk_return tag;
    });

    if (tag == TreeNode_leaf) {
        i32 class_label = 0;
        if (fread(&class_label, sizeof(class_label), 1, file) != 1) {
            log_error("Failed to read leaf data");
            return_err(fs_File_Err_ReadFailed());
        }

        return_ok(try_(TreeNode_createLeaf(gpa, class_label)));
    } else if (tag == TreeNode_decision) {
        u32 feature_index = 0;
        f32 threshold = 0;
        if (fread(&feature_index, sizeof(feature_index), 1, file) != 1 || fread(&threshold, sizeof(threshold), 1, file) != 1) {
            log_error("Failed to read decision node data");
            return_err(fs_File_Err_ReadFailed());
        }

        // Recursively load children
        let left = try_(TreeNode_loadFromFileRecur(gpa, file));
        let right = try_(TreeNode_loadFromFileRecur(gpa, file));

        return_ok(try_(TreeNode_createDecision(gpa, feature_index, threshold, left, right)));
    } else {
        log_error("Invalid node tag found in file: %d", tag);
        return_err(fs_File_Err_ReadFailed());
    }
} $unscoped_(fn);

// Implementation of Dataset functions
fn_((Dataset_loadFromCSV(mem_Allocator gpa, S_const$u8 filename, bool has_header))(E$Dataset) $guard) {
    // Convert S_const$u8 to C string for compatibility with fopen
    let filename_buf = try_(u_castE$((E$S$u8)(mem_Allocator_alloc(gpa, typeInfo$(u8), filename.len + 1))));
    errdefer_($ignore, mem_Allocator_free(gpa, u_anyS(filename_buf)));

    mem_copyBytes(prefixS(filename_buf, filename.len), filename);
    filename_buf.ptr[filename.len] = '\0';

    FILE* file = fopen((const char*)filename_buf.ptr, "r");
    if (file == null) {
        log_error("Failed to open file: %.*s", (int)filename.len, filename.ptr);
        return_err(fs_File_Err_OpenFailed());
    }
    defer_(let_ignore = fclose(file));

    // First pass: count lines and features
    u32 line_count = 0;
    u32 feature_count = 0;

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line_count++;

        if (line_count == 1) {
            // Count columns in first line
            char* token = strtok(line, ",");
            while (token) {
                feature_count++;
                token = strtok(NULL, ",");
            }
        }
    }

    if (has_header) { line_count--; }

    // Feature count should include label column
    u32 actual_feature_count = feature_count - 1;

    log_debug("Found %u lines and %u features in CSV", line_count, actual_feature_count);

    // Initialize dataset
    Dataset dataset = {
        .gpa = gpa,
        .n_samples = line_count,
        .n_features = actual_feature_count
    };

    // Allocate memory for features and labels
    *dataset.features.as_raw = try_(ArrList_init(typeInfo$(f32), gpa, as$(usize)(line_count)*actual_feature_count));
    errdefer_($ignore, ArrList_fini(dataset.features.as_raw, typeInfo$(f32), gpa));

    *dataset.labels.as_raw = try_(ArrList_init(typeInfo$(i32), gpa, line_count));
    errdefer_($ignore, ArrList_fini(dataset.labels.as_raw, typeInfo$(f32), gpa));

    // Second pass: read data
    rewind(file); // NOLINT

    if (has_header) {
        // Skip header line
        if (!fgets(line, sizeof(line), file)) {
            log_error("Failed to skip header line");
            return_err(fs_File_Err_ReadFailed());
        }
    }

    u32 sample_idx = 0;
    while (fgets(line, sizeof(line), file) && sample_idx < line_count) {
        char* token = strtok(line, ",");
        u32 col_idx = 0;

        while (token && col_idx < feature_count) {
            var value = as$(f32)(atof(token));

            if (col_idx < actual_feature_count) {
                // Feature value
                try_(ArrList_append(dataset.features.as_raw, gpa, u_anyV(value)));
            } else {
                // Label (assumed to be the last column)
                var label = as$(i32)(value);
                try_(ArrList_append(dataset.labels.as_raw, gpa, u_anyV(label)));
            }

            token = strtok(null, ",");
            col_idx++;
        }

        sample_idx++;
    }

    if (sample_idx != line_count) {
        log_warn("Expected %u samples but read only %u", line_count, sample_idx);
    }

    // Clean up the temporary filename string
    mem_Allocator_free(gpa, u_anyS(filename_buf));

    return_ok(dataset);
} $unguarded_(fn);

fn_((Dataset_destroy(Dataset* dataset))(void)) {
    if (dataset == null) { return; }
    ArrList_fini(dataset->features.as_raw, typeInfo$(FieldType$(TypeOf(dataset->features.items), ptr $like_deref)), dataset->gpa);
    ArrList_fini(dataset->labels.as_raw, typeInfo$(FieldType$(TypeOf(dataset->labels.items), ptr $like_deref)), dataset->gpa);
}
