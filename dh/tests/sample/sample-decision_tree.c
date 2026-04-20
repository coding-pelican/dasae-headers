/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    example-decision_tree.c
 * @author  Gyeongtae Kim (@dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-07 (date of creation)
 * @updated 2026-02-21 (date of last update)
 * @version v0.1-alpha.3
 * @ingroup dal-project/examples
 * @prefix  (none)
 *
 * @brief   Decision tree implementation for classification
 * @details This file implements a basic decision tree classifier that can:
 *          - Build a tree from training data
 *          - Make predictions on new data
 *          - Save/load trees to/from files
 *          - Handle CSV data input
 */
#include "dh-main.h"
#include "dh/log.h"
#include "dh/mem.h"
#include "dh/heap/Page.h"
#include "dh/fs/common.h"
#include "dh/fs/File.h"
#include "dh/io/Buf.h"
#include "dh/io/Writer.h"
#include "dh/fmt/common.h"
#include "dh/ArrList.h"

// Decision tree structures
T_alias$((TreeNode_Decision)(struct TreeNode_Decision));
T_alias$((TreeNode_Leaf)(struct TreeNode_Leaf));
T_alias$((TreeNode)(variant_((TreeNode) $T)));
T_use$((TreeNode)(P));
T_use$((P$TreeNode)(O, E));
T_alias$((TreeNode)(variant_((TreeNode $fits($packed))(
    (TreeNode_decision, struct TreeNode_Decision {
        var_(left, O$P$TreeNode);
        var_(right, O$P$TreeNode);
        var_(feature_index, u32);
        var_(threshold, f32);
    }),
    (TreeNode_leaf, struct TreeNode_Leaf {
        var_(class_label, i32);
    })
))));
T_use$((TreeNode)(O));
// Forward declarations
$attr($must_check)
$static fn_((TreeNode_createLeaf(mem_Alctr gpa, i32 class_label))(E$P$TreeNode));
$attr($must_check)
$static fn_((TreeNode_createDecision(mem_Alctr gpa, u32 feature_index, f32 threshold, TreeNode* left, TreeNode* right))(E$P$TreeNode));
$static fn_((TreeNode_destroyRecur(mem_Alctr gpa, TreeNode* target))(void));
$static fn_((TreeNode_predict(const TreeNode* target, S_const$f32 features))(i32));
$static fn_((TreeNode_printRecur(const TreeNode* target, u32 depth))(void));
$attr($must_check)
$static fn_((TreeNode_saveToFileRecur(const TreeNode* node, io_Writer writer))(E$void));
$attr($must_check)
$static fn_((TreeNode_loadFromFileRecur(mem_Alctr gpa, io_Reader reader))(E$P$TreeNode));

T_use$((f32)(ArrList));
T_use$((i32)(ArrList));
// Dataset structure
T_alias$((Dataset)(struct Dataset {
    var_(gpa, mem_Alctr);
    var_(features, ArrList$f32);
    var_(labels, ArrList$i32);
    var_(n_samples, u32);
    var_(n_features, u32);
}));
T_use$((Dataset)(E));
// Forward declarations
$attr($must_check)
$static fn_((fs_File__openByPath(mem_Alctr gpa, S_const$u8 filename))(E$fs_File));
$attr($must_check)
$static fn_((fs_File__openByPathForWrite(mem_Alctr gpa, S_const$u8 filename))(E$fs_File));
$attr($must_check)
$static fn_((Dataset_loadFromCSV(mem_Alctr gpa, S_const$u8 filename, bool has_header))(E$Dataset));
$static fn_((Dataset_destroy(Dataset* dataset))(void));

// Main function
fn_((main(S$S_const$u8 args))(E$void) $guard) {
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
    var gpa = heap_Page_alctr(&l0$((heap_Page)));
    // Load dataset if a filename was provided, otherwise create a demo tree
    if (args.len > 1) {
        log_info("Loading dataset from {:s}", *S_at((args)[1]));
        var dataset = try_(Dataset_loadFromCSV(gpa, *S_at((args)[1]), true));
        defer_(Dataset_destroy(&dataset));
        // Here normally build the tree from the dataset
        // For simplicity, we'll just create a demo tree
        log_info("Loaded {:u} samples with {:u} features", dataset.n_samples, dataset.n_features);
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
        3, // feature index (petal width)
        1.75f, // threshold
        leaf_versicolor,
        leaf_virginica
    ));
    let root = try_(TreeNode_createDecision(
        gpa,
        2, // feature index (petal length)
        2.45f, // threshold
        leaf_setosa,
        versicolor_virginica
    ));
    defer_(TreeNode_destroyRecur(gpa, root));

    // Print the tree
    log_info("Decision tree structure:");
    TreeNode_printRecur(root, 0);

    // Save the tree to a file
    {
        let save_file = try_(fs_File__openByPathForWrite(gpa, u8_l("decision_tree.bin")));
        defer_(fs_File_close(save_file));
        let writer = fs_File_writer(save_file);
        try_(TreeNode_saveToFileRecur(root, writer));
        log_info("Saved decision tree to decision_tree.bin");
    }

    // Load the tree from the file
    var loaded_tree = expr_(P$TreeNode $guard)({
        let load_file = try_(fs_File__openByPath(gpa, u8_l("decision_tree.bin")));
        defer_(fs_File_close(load_file));
        let reader = fs_File_reader(load_file);
        $break_(try_(TreeNode_loadFromFileRecur(gpa, reader)));
    }) $unguarded(expr);
    log_info("Loaded decision tree from decision_tree.bin");
    defer_(TreeNode_destroyRecur(gpa, loaded_tree));

    // Verify the loaded tree by printing it
    log_info("Loaded tree structure:");
    TreeNode_printRecur(loaded_tree, 0);

    // Make some predictions
    let_(samples, A$$(3, A$$(4, f32))) = A_init({
        A_init({ 5.1f, 3.5f, 1.4f, 0.2f }), // Setosa
        A_init({ 6.3f, 3.3f, 4.7f, 1.6f }), // Versicolor
        A_init({ 6.5f, 3.0f, 5.2f, 2.0f }) // Virginica
    });
    for_(($rf(0), $s(A_ref(samples)))(i, sample)) {
        let prediction = TreeNode_predict(root, A_ref$((S_const$f32)(*sample)));
        log_info("Sample {:uz}: Class {:d}", i + 1, prediction);
    } $end(for);

    log_info("Decision tree application completed successfully");
    return_ok({});
} $unguarded(fn);

T_use_E$($set(mem_E)(P$TreeNode));
T_use$((TreeNode)(mem_Alctr_create, mem_Alctr_destroy));

// Implementation of TreeNode functions
fn_((TreeNode_createLeaf(mem_Alctr gpa, i32 class_label))(E$P$TreeNode) $scope) {
    let node = try_(mem_Alctr_create$TreeNode($trace gpa));
    asg_l((node)(union_of((TreeNode_leaf){ .class_label = class_label })));
    return_ok(node);
} $unscoped(fn);

fn_((TreeNode_createDecision(mem_Alctr gpa, u32 feature_index, f32 threshold, TreeNode* left, TreeNode* right))(E$P$TreeNode) $scope) {
    let node = try_(mem_Alctr_create$TreeNode($trace gpa));
    asg_l((node)(union_of((TreeNode_decision){
        .left = some(left),
        .right = some(right),
        .feature_index = feature_index,
        .threshold = threshold,
    })));
    return_ok(node);
} $unscoped(fn);

fn_((TreeNode_destroyRecur(mem_Alctr gpa, TreeNode* target))(void)) /* NOLINT(misc-no-recursion) */ {
    match_($ref(target)) {
    case_((TreeNode_leaf)) break $end(case);
    pattern_((TreeNode_decision)($ref decision)) {
        if_some((decision->left)(left)) TreeNode_destroyRecur(gpa, left);
        if_some((decision->right)(right)) TreeNode_destroyRecur(gpa, right);
    } $end(pattern);
    default_() {
        log_error("Invalid node type encountered during prediction");
        claim_unreachable;
    } $end(default);
    } $end(match);
    mem_Alctr_destroy$TreeNode($trace gpa, target);
};

fn_((TreeNode_predict(const TreeNode* target, S_const$f32 features))(i32)) {
    claim_assert_nonnullS(features);
    var_(curr, O$TreeNode) = some(deref(target));
    while_some(curr, node) match_(node) {
    pattern_((TreeNode_leaf)(leaf)) return leaf.class_label $end(pattern);
    pattern_((TreeNode_decision)(decision)) {
        if (features.len <= decision.feature_index) {
            log_error("Feature index out of bounds: %u >= %zu", decision.feature_index, features.len);
            claim_unreachable;
        }
        curr = expr_(O$TreeNode $scope)(
            *S_at((features)[decision.feature_index]) <= decision.threshold
                ? $break_(O_deref$((O$TreeNode)(decision.left)))
                : $break_(O_deref$((O$TreeNode)(decision.right)))
        ) $unscoped(expr);
    } $end(pattern);
    default_() {
        log_error("Invalid node type encountered during prediction");
        claim_unreachable;
    } $end(default);
    } $end(match);
    claim_unreachable;
};

fn_((TreeNode_printRecur(const TreeNode* target, u32 depth))(void)) /* NOLINT(misc-no-recursion) */ {
    claim_assert_nonnull(target);
    var_(indent, A$$(64, u8)) = A_zero();
    {
        u32 n = depth <= 31 ? depth * 2 : 62;
        for (u32 i = 0; i < n; i += 2) {
            *A_at((indent)[i]) = u8_c(' ');
            *A_at((indent)[i + 1]) = u8_c(' ');
        }
        *A_at((indent)[n]) = u8_c('\0');
    }
    let indent_z = ptrCast$((const char*)(A_val(indent)));

    match_($ref(target)) {
    pattern_((TreeNode_leaf)($ref leaf)) log_info("{:z}Class: {:d}", indent_z, leaf->class_label) $end(pattern);
    pattern_((TreeNode_decision)($ref decision)) {
        log_info("{:z}Feature {:u} <= {:.2f}", indent_z, decision->feature_index, decision->threshold);
        if_some((decision->left)(left)) { TreeNode_printRecur(left, depth + 1); }
        log_info("{:z}Feature {:u} > {:.2f}", indent_z, decision->feature_index, decision->threshold);
        if_some((decision->right)(right)) { TreeNode_printRecur(right, depth + 1); }
    } $end(pattern);
    default_() {
        log_error("{:z}Invalid node type", indent_z);
        claim_unreachable;
    } $end(default);
    } $end(match);
};

fn_((TreeNode_saveToFileRecur(const TreeNode* node, io_Writer writer))(E$void) $scope) /* NOLINT(misc-no-recursion) */ {
    claim_assert_nonnull(node);
    try_(io_Writer_writeBytes(writer, mem_asBytes(u_anyP(&node->tag))));

    match_($ref(node)) {
    pattern_((TreeNode_leaf)($ref leaf)) {
        try_(io_Writer_writeBytes(writer, mem_asBytes(u_anyP(&leaf->class_label))));
    } $end(pattern);
    pattern_((TreeNode_decision)($ref decision)) {
        try_(io_Writer_writeBytes(writer, mem_asBytes(u_anyP(&decision->feature_index))));
        try_(io_Writer_writeBytes(writer, mem_asBytes(u_anyP(&decision->threshold))));
        if_some((decision->left)(left)) { try_(TreeNode_saveToFileRecur(left, writer)); }
        if_some((decision->right)(right)) { try_(TreeNode_saveToFileRecur(right, writer)); }
    } $end(pattern);
    default_() {
        log_error("Invalid node type encountered during saving");
        claim_unreachable;
    } $end(default);
    } $end(match);

    return_ok({});
} $unscoped(fn);

fn_((TreeNode_loadFromFileRecur(mem_Alctr gpa, io_Reader reader))(E$P$TreeNode) $scope) /* NOLINT(misc-no-recursion) */ {
    var_(tag, FieldType$(TreeNode, tag)) = 0;
    try_(io_Reader_readExact(reader, mem_asBytesMut(u_anyP(&tag))));
    switch (tag) {
    case_((TreeNode_leaf)) {
        var_(class_label, FieldType$(TreeNode_Leaf, class_label)) = 0;
        try_(io_Reader_readExact(reader, mem_asBytesMut(u_anyP(&class_label))));
        return_ok(try_(TreeNode_createLeaf(gpa, class_label)));
    } $end(case);
    case_((TreeNode_decision)) {
        var_(feature_index, FieldType$(TreeNode_Decision, feature_index)) = 0;
        var_(threshold, FieldType$(TreeNode_Decision, threshold)) = 0;
        try_(io_Reader_readExact(reader, mem_asBytesMut(u_anyP(&feature_index))));
        try_(io_Reader_readExact(reader, mem_asBytesMut(u_anyP(&threshold))));
        let left = try_(TreeNode_loadFromFileRecur(gpa, reader));
        let right = try_(TreeNode_loadFromFileRecur(gpa, reader));
        return_ok(try_(TreeNode_createDecision(gpa, feature_index, threshold, left, right)));
    } $end(case);
    default_() {
        log_error("Invalid node tag found in file: {:d}", tag);
        return_err(fs_File_E_ReadFailed());
    } $end(default);
    }
} $unscoped(fn);

#if plat_is_windows
#include "dh/os/windows.h"
#else
#include <fcntl.h>
#include <unistd.h>
#endif /* plat_is_windows */

T_use_E$($set(mem_E)(S$u8));
T_use$((u8)(mem_Alctr_alloc, mem_Alctr_free));

$attr($must_check)
$static fn_((fs_File__openByPath(mem_Alctr gpa, S_const$u8 filename))(E$fs_File) $guard) {
    let filename_buf = try_(mem_Alctr_alloc$u8($trace gpa, filename.len + 1));
    defer_(mem_Alctr_free$u8($trace gpa, filename_buf));
    mem_copyBytes(prefixS(filename_buf, filename.len), filename);
    *S_at((filename_buf)[filename.len]) = '\0';

    let handle = pp_if_(plat_is_windows)(
        pp_then_(ptrCast$((fs_File_Handle)(CreateFileA(
            ptrCast$((LPCSTR)(filename_buf.ptr)),
            GENERIC_READ,
            FILE_SHARE_READ,
            null,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            null
        )))),
        pp_else_(intCast$((fs_File_Handle)(open(ptrCast$((const char*)(filename_buf.ptr)), O_RDONLY)))));
    if (pp_if_(plat_is_windows)(
            pp_then_(handle == ptrCast$((fs_File_Handle)(INVALID_HANDLE_VALUE))),
            pp_else_(as$(i32)(handle) < 0))) {
        log_error("Failed to open file: {:s}", filename);
        return_err(fs_File_E_OpenFailed());
    }
    return_ok(fs_File_Handle_promote(handle));
} $unguarded(fn);

$attr($must_check)
$static fn_((fs_File__openByPathForWrite(mem_Alctr gpa, S_const$u8 filename))(E$fs_File) $guard) {
    let filename_buf = try_(mem_Alctr_alloc$u8($trace gpa, filename.len + 1));
    defer_(mem_Alctr_free$u8($trace gpa, filename_buf));
    mem_copyBytes(prefixS(filename_buf, filename.len), filename);
    *S_at((filename_buf)[filename.len]) = '\0';

    let handle = pp_if_(plat_is_windows)(
        pp_then_(ptrCast$((fs_File_Handle)(CreateFileA(
            ptrCast$((LPCSTR)(filename_buf.ptr)),
            GENERIC_WRITE,
            FILE_SHARE_READ,
            null,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            null
        )))),
        pp_else_(intCast$((fs_File_Handle)(open(ptrCast$((const char*)(filename_buf.ptr)), O_WRONLY | O_CREAT | O_TRUNC, 0666)))));
    if (pp_if_(plat_is_windows)(
            pp_then_(handle == ptrCast$((fs_File_Handle)(INVALID_HANDLE_VALUE))),
            pp_else_(as$(i32)(handle) < 0))) {
        log_error("Failed to open file for writing: {:s}", filename);
        return_err(fs_File_E_OpenFailed());
    }
    return_ok(fs_File_Handle_promote(handle));
} $unguarded(fn);

T_use$((u8)(
    mem_Delim,
    mem_TokzIter,
    mem_TokzIter_next,
    mem_tokzUnit,
    ArrList,
    ArrList_init,
    ArrList_fini,
    ArrList_appendS
));
T_use$((f32)(ArrList_init, ArrList_fini, ArrList_append));
T_use$((i32)(ArrList_init, ArrList_fini, ArrList_append));

// Implementation of Dataset functions
fn_((Dataset_loadFromCSV(mem_Alctr gpa, S_const$u8 filename, bool has_header))(E$Dataset) $guard) {
    let file = try_(fs_File__openByPath(gpa, filename));
    defer_(fs_File_close(file));

    let unbufd = fs_File_reader(file);
    var_(chunk, A$$(4096, u8)) = A_zero();
    var file_bytes = try_(ArrList_init$u8(gpa, as$(usize)(4096)));
    errdefer_($ignore, ArrList_fini$u8(&file_bytes, gpa));
    while (true) {
        let chunk_slice = A_ref$((S$u8)(chunk));
        let n = try_(io_Reader_read(unbufd, chunk_slice));
        if (n == 0) break;
        try_(ArrList_appendS$u8(&file_bytes, gpa, S_prefix((chunk_slice)(n)).as_const));
    }
    let content = file_bytes.items.as_const;

    var_(line_count, u32) = 0;
    var_(feature_count, u32) = 0;
    {
        var line_iter = mem_tokzUnit$u8(content, u8_c('\n'));
        while_some(mem_TokzIter_next$u8(&line_iter), line) {
            line_count++;
            if (line_count == 1) {
                var col_iter = mem_tokzUnit$u8(line, u8_c(','));
                while_some(mem_TokzIter_next$u8(&col_iter), tok) feature_count++;
            }
        }
    }
    if (has_header) line_count--;
    var_(actual_feature_count, u32) = feature_count <= 0 ? 0 : feature_count - 1;

    log_debug("Found {:u} lines and {:u} features in CSV", line_count, actual_feature_count);

    var features = try_(ArrList_init$f32(gpa, (as$(usize)(line_count)) * actual_feature_count));
    errdefer_($ignore, ArrList_fini$f32(&features, gpa));
    var labels = try_(ArrList_init$i32(gpa, line_count));
    errdefer_($ignore, ArrList_fini$i32(&labels, gpa));
    {
        var_(samples_read, u32) = 0;
        var first_line_skipped = false;
        var line_iter = mem_tokzUnit$u8(content, u8_c('\n'));
        while_some(mem_TokzIter_next$u8(&line_iter), line) {
            if (has_header && !first_line_skipped) {
                first_line_skipped = true;
                continue;
            }
            if (samples_read >= line_count) break;

            var col_iter = mem_tokzUnit$u8(line, u8_c(','));
            u32 col_idx = 0;
            while_some(mem_TokzIter_next$u8(&col_iter), tok) {
                if (col_idx >= feature_count) { break; }
                var value = try_(fmt_parse$f32(tok));
                if (col_idx < actual_feature_count) {
                    try_(ArrList_append$f32(&features, gpa, value));
                } else {
                    var label = as$(i32)(value);
                    try_(ArrList_append$i32(&labels, gpa, label));
                }
                col_idx++;
            }
            samples_read++;
        }
        if (samples_read != line_count) log_warn("Expected {:u} samples but read only {:u}", line_count, samples_read);
    }
    return_ok((Dataset){
        .gpa = gpa,
        .features = features,
        .labels = labels,
        .n_samples = line_count,
        .n_features = actual_feature_count,
    });
} $unguarded(fn);

fn_((Dataset_destroy(Dataset* dataset))(void)) {
    if (dataset) {
        ArrList_fini$f32(&dataset->features, dataset->gpa);
        ArrList_fini$i32(&dataset->labels, dataset->gpa);
        asg_l((dataset)(cleared()));
    }
};
