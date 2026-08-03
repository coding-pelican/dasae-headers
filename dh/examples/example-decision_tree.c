/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    example-decision_tree.c
 * @author  Gyeongtae Kim (@dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-07 (date of creation)
 * @updated 2026-07-30 (date of last update)
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
/*========== Includes =======================================================*/

#include "dh-main.h"
#include "dh/heap/Sys.h"
#include "dh/log.h"
#include "dh/mem.h"
#include "dh/fs/common.h"
#include "dh/fs/File.h"
#include "dh/io/Buf.h"
#include "dh/io/Writer.h"
#include "dh/fmt/common.h"
#include "dh/ArrList.h"

/*========== Declarations ===================================================*/

T_alias$((TreeNode_Decision)(struct TreeNode_Decision));
T_alias$((TreeNode_Leaf)(struct TreeNode_Leaf));
T_alias$((TreeNode)(variant_((TreeNode) $T)));
T_use$((TreeNode)(P));
T_use$((P$TreeNode)(O, E));
T_alias$((TreeNode)(variant_((TreeNode $fits($packed))(
    (TreeNode_decision, struct TreeNode_Decision {
        var_(left, P$TreeNode);
        var_(right, P$TreeNode);
        var_(feature_index, u32);
        var_(threshold, f32);
    }),
    (TreeNode_leaf, struct TreeNode_Leaf {
        var_(class_label, i32);
    })
))));
T_use$((TreeNode)(O));
T_use_E$($set(mem_E)(P$TreeNode));
T_use$((TreeNode)(mem_Alctr_create, mem_Alctr_destroy));
$attr($must_check)
$static fn_((TreeNode_createLeaf(mem_Alctr gpa, i32 class_label))(E$P$TreeNode));
$attr($must_check)
$static fn_((TreeNode_createDecision(
    mem_Alctr gpa,
    u32 feature_index, f32 threshold,
    TreeNode* left, TreeNode* right
))(E$P$TreeNode));
$attr($must_check)
$static fn_((TreeNode_createDemo(mem_Alctr gpa))(E$P$TreeNode));
$static fn_((TreeNode_destroyRecur(mem_Alctr gpa, TreeNode* target))(void));
$static fn_((TreeNode_predict(const TreeNode* target, S_const$f32 features))(i32));
$static fn_((TreeNode_printRecur(log_Self logger, const TreeNode* target, u32 depth))(void));
$attr($must_check)
$static fn_((TreeNode_saveToFileRecur(const TreeNode* node, io_Writer writer))(E$void));
$attr($must_check)
$static fn_((TreeNode_countRecur(const TreeNode* node, usize depth))(E$u32));
$attr($must_check)
$static fn_((TreeNode_save(const TreeNode* root, io_Writer writer))(E$void));
$attr($must_check)
$static fn_((TreeNode_loadRecur(mem_Alctr gpa, io_Reader reader, u32* remaining, usize depth))(E$P$TreeNode));
$attr($must_check)
$static fn_((TreeNode_loadDecision(
    mem_Alctr gpa, io_Reader reader,
    u32* remaining, usize depth,
    u32 feature_index, f32 threshold
))(E$P$TreeNode));
$attr($must_check)
$static fn_((TreeNode_load(mem_Alctr gpa, io_Reader reader))(E$P$TreeNode));

$attr($must_check)
$static fn_((tree_file_writeU32(io_Writer writer, u32 value))(E$void));
$attr($must_check)
$static fn_((tree_file_readU32(io_Reader reader))(E$u32));

T_use$((f32)(ArrList));
T_use$((i32)(ArrList));
T_alias$((Dataset)(struct Dataset {
    var_(gpa, mem_Alctr);
    var_(features, ArrList$f32);
    var_(labels, ArrList$i32);
    var_(n_samples, u32);
    var_(n_features, u32);
}));
T_use$((Dataset)(E));
$attr($must_check)
$static fn_((Dataset_loadFromCSV(
    log_Self logger,
    mem_Alctr gpa,
    S_const$u8 filename,
    bool has_header
))(E$Dataset));
$static fn_((Dataset_destroy(Dataset* dataset))(void));

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

#define TreeFile_version u8_(1)
#define TreeFile_max_nodes u32_(1048576)
#define TreeFile_max_depth usize_(1024)
#define TreeFile_NodeTag_decision u8_(0)
#define TreeFile_NodeTag_leaf u8_(1)

/*========== Definitions ====================================================*/

fn_((main(proc_Entry entry))(E$void) $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var default_logger = log_Default_init(
        catch_((io_std_direct())($ignore, io_std_noop)),
        log_Level_debug
    );
    let logger = log_Default_self(&default_logger);
    log_info(logger, u8_l("Starting decision tree application"));

    // Load dataset if a filename was provided, otherwise create a demo tree
    T_use_A$(4096, u8);
    $static var_(arg_scratch, A$$(4096, u8)) $undefined_static;
    var args = proc_Args_iter(entry.args);
    let_ignore = try_(proc_Args_Iter_skip(&args));
    if_some((try_(proc_Args_Iter_next(&args, A_ref$((S$u8)arg_scratch))))(dataset_path)) blk_defer {
        log_info(logger, u8_l("Loading dataset from {:s}"), dataset_path);
        var dataset = try_(Dataset_loadFromCSV(logger, gpa, dataset_path, true));
        defer_(Dataset_destroy(&dataset));
        // Here normally build the tree from the dataset
        // For simplicity, we'll just create a demo tree
        log_info(logger, u8_l("Loaded {:u} samples with {:u} features"), dataset.n_samples, dataset.n_features);
        // Demo tree creation would go here
    } blk_deferral;

    // Create a simple decision tree (manually for demo purposes)
    // In a real implementation, this would be learned from data
    log_info(logger, u8_l("Creating demo decision tree"));
    let root = try_(TreeNode_createDemo(gpa));
    defer_(TreeNode_destroyRecur(gpa, root));

    // Print the tree
    log_info(logger, u8_l("Decision tree structure:"));
    TreeNode_printRecur(logger, root, 0);

    // Save the tree to a file
    let save_path = u8_l("decision_tree.bin");
    let staging_path = u8_l("decision_tree.bin.new");
    let_ignore = catch_((fs_File_delete(staging_path))($ignore, $do_nothing));
    defer_(let_ignore = catch_((fs_File_delete(staging_path))($ignore, $do_nothing)));
    using_() blk_defer {
        var create_flags = fs_File_CreateFlags_default;
        let save_file = try_(fs_File_create(staging_path, create_flags));
        defer_(fs_File_close(save_file));
        let writer = fs_File_writer(save_file);
        try_(TreeNode_save(root, writer));
        try_(fs_File_sync(save_file));
    } blk_deferral;
    try_(fs_File_rename(staging_path, save_path));
    log_info(logger, u8_l("Saved decision tree to decision_tree.bin"));

    // Load the tree from the file
    var loaded_tree = expr_(P$TreeNode $guard)({
        var open_flags = fs_File_OpenFlags_default;
        open_flags.mode = fs_OpenMode_read_only;
        let load_file = try_(fs_File_open(save_path, open_flags));
        defer_(fs_File_close(load_file));
        let reader = fs_File_reader(load_file);
        $break_(try_(TreeNode_load(gpa, reader)));
    }) $unguarded(expr);
    log_info(logger, u8_l("Loaded decision tree from decision_tree.bin"));
    defer_(TreeNode_destroyRecur(gpa, loaded_tree));

    // Verify the loaded tree by printing it
    log_info(logger, u8_l("Loaded tree structure:"));
    TreeNode_printRecur(logger, loaded_tree, 0);

    // Make some predictions
    let_(samples, A$$(3, A$$(4, f32))) = A_init({
        A_init({ 5.1f, 3.5f, 1.4f, 0.2f }), // Setosa
        A_init({ 6.3f, 3.3f, 4.7f, 1.6f }), // Versicolor
        A_init({ 6.5f, 3.0f, 5.2f, 2.0f }) // Virginica
    });
    for_(($rf(0), $s(A_ref(samples)))(i, sample)) {
        let features = A_ref$((S_const$f32)(*sample));
        let prediction = TreeNode_predict(root, features);
        let loaded_prediction = TreeNode_predict(loaded_tree, features);
        claim_assert(prediction == loaded_prediction);
        log_info(
            logger,
            u8_l("Sample {:uz}: Class {:d} (loaded: {:d})"),
            i + 1,
            prediction,
            loaded_prediction
        );
    } $end(for);

    log_info(logger, u8_l("Decision tree application completed successfully"));
    return_ok({});
} $unguarded(fn);

fn_((TreeNode_createLeaf(mem_Alctr gpa, i32 class_label))(E$P$TreeNode) $scope) {
    let node = try_(mem_Alctr_create$TreeNode($trace gpa));
    asg_l((node)(union_of((TreeNode_leaf){ .class_label = class_label })));
    return_ok(node);
} $unscoped(fn);

fn_((TreeNode_createDecision(
    mem_Alctr gpa,
    u32 feature_index,
    f32 threshold,
    TreeNode* left,
    TreeNode* right
))(E$P$TreeNode) $scope) {
    claim_assert_nonnull(left);
    claim_assert_nonnull(right);
    let node = try_(mem_Alctr_create$TreeNode($trace gpa));
    asg_l((node)(union_of((TreeNode_decision){
        .left = left,
        .right = right,
        .feature_index = feature_index,
        .threshold = threshold,
    })));
    return_ok(node);
} $unscoped(fn);

fn_((TreeNode_createDemo(mem_Alctr gpa))(E$P$TreeNode) $guard) {
    var_(leaf_setosa, O$P$TreeNode) = none();
    var_(leaf_versicolor, O$P$TreeNode) = none();
    var_(leaf_virginica, O$P$TreeNode) = none();
    var_(versicolor_virginica, O$P$TreeNode) = none();
    errdefer_($ignore, {
        if_some((leaf_setosa)(node)) TreeNode_destroyRecur(gpa, node);
        if_some((leaf_versicolor)(node)) TreeNode_destroyRecur(gpa, node);
        if_some((leaf_virginica)(node)) TreeNode_destroyRecur(gpa, node);
        if_some((versicolor_virginica)(node)) TreeNode_destroyRecur(gpa, node);
    });

    asg_l((&leaf_setosa)(some(try_(TreeNode_createLeaf(gpa, 0)))));
    asg_l((&leaf_versicolor)(some(try_(TreeNode_createLeaf(gpa, 1)))));
    asg_l((&leaf_virginica)(some(try_(TreeNode_createLeaf(gpa, 2)))));
    asg_l((&versicolor_virginica)(some(try_(TreeNode_createDecision(
        gpa,
        3, // feature index (petal width)
        1.75f, // threshold
        unwrap_(leaf_versicolor),
        unwrap_(leaf_virginica)
    )))));
    asg_l((&leaf_versicolor)(none()));
    asg_l((&leaf_virginica)(none()));

    let root = try_(TreeNode_createDecision(
        gpa,
        2, // feature index (petal length)
        2.45f, // threshold
        unwrap_(leaf_setosa),
        unwrap_(versicolor_virginica)
    ));
    asg_l((&leaf_setosa)(none()));
    asg_l((&versicolor_virginica)(none()));
    return_ok(root);
} $unguarded(fn);

fn_((TreeNode_destroyRecur(mem_Alctr gpa, TreeNode* target))(void)) /* NOLINT(misc-no-recursion) */ {
    match_($ref(target)) {
    case_((TreeNode_leaf)) break $end(case);
    patt_((TreeNode_decision)($ref decision)) {
        TreeNode_destroyRecur(gpa, decision->left);
        TreeNode_destroyRecur(gpa, decision->right);
    } $end(patt);
    default_() {
        claim_unreachable;
    } $end(default);
    } $end(match);
    mem_Alctr_destroy$TreeNode($trace gpa, target);
};

fn_((TreeNode_predict(const TreeNode* target, S_const$f32 features))(i32)) {
    claim_assert_nonnullS(features);
    var_(curr, O$TreeNode) = some(deref(target));
    while_some(curr, node) match_(node) {
    patt_((TreeNode_leaf)(leaf)) return leaf.class_label $end(patt);
    patt_((TreeNode_decision)(decision)) {
        if (features.len <= decision.feature_index) {
            claim_unreachable;
        }
        curr = expr_(O$TreeNode $scope)(
            *S_at((features)[decision.feature_index]) <= decision.threshold
                ? $break_(some(deref(decision.left)))
                : $break_(some(deref(decision.right)))
        ) $unscoped(expr);
    } $end(patt);
    default_() {
        claim_unreachable;
    } $end(default);
    } $end(match);
    claim_unreachable;
};

fn_((TreeNode_printRecur(
    log_Self logger,
    const TreeNode* target,
    u32 depth
))(void)) /* NOLINT(misc-no-recursion) */ {
    claim_assert_nonnull(target);
    var_(indent, A$$(64, u8)) $undefined;
    {
        u32 n = depth <= 31 ? depth * 2 : 62;
        for (u32 i = 0; i < n; i += 2) {
            *A_at((indent)[i]) = u8_c(' ');
            *A_at((indent)[i + 1]) = u8_c(' ');
        }
        *A_at((indent)[n]) = u8_c('\0');
    }
    let indent_z = A_ptr(indent);

    match_($ref(target)) {
    patt_((TreeNode_leaf)($ref leaf)) log_info(logger, u8_l("{:z}Class: {:d}"), indent_z, leaf->class_label) $end(patt);
    patt_((TreeNode_decision)($ref decision)) {
        log_info(logger, u8_l("{:z}Feature {:u} <= {:.2f}"), indent_z, decision->feature_index, decision->threshold);
        TreeNode_printRecur(logger, decision->left, depth + 1);
        log_info(logger, u8_l("{:z}Feature {:u} > {:.2f}"), indent_z, decision->feature_index, decision->threshold);
        TreeNode_printRecur(logger, decision->right, depth + 1);
    } $end(patt);
    default_() {
        log_err(logger, u8_l("{:z}Invalid node type"), indent_z);
        claim_unreachable;
    } $end(default);
    } $end(match);
};

fn_((TreeNode_saveToFileRecur(
    const TreeNode* node, io_Writer writer
))(E$void) $scope) /* NOLINT(misc-no-recursion) */ {
    claim_assert_nonnull(node);

    match_($ref(node)) {
    patt_((TreeNode_leaf)($ref leaf)) {
        try_(io_Writer_writeByte(writer, TreeFile_NodeTag_leaf));
        try_(tree_file_writeU32(writer, bitCast$((u32)(leaf->class_label))));
    } $end(patt);
    patt_((TreeNode_decision)($ref decision)) {
        try_(io_Writer_writeByte(writer, TreeFile_NodeTag_decision));
        try_(tree_file_writeU32(writer, decision->feature_index));
        try_(tree_file_writeU32(
            writer,
            bitCast$((u32)(decision->threshold))
        ));
        try_(TreeNode_saveToFileRecur(decision->left, writer));
        try_(TreeNode_saveToFileRecur(decision->right, writer));
    } $end(patt);
    default_() {
        claim_unreachable;
    } $end(default);
    } $end(match);

    return_ok({});
} $unscoped(fn);

fn_((TreeNode_countRecur(
    const TreeNode* node, usize depth
))(E$u32) $scope) /* NOLINT(misc-no-recursion) */ {
    claim_assert_nonnull(node);
    if (TreeFile_max_depth < depth) {
        return_err(E_cause$fs_File_WriteFailed());
    }
    match_($ref(node)) {
    case_((TreeNode_leaf)) return_ok(u32_(1)) $end(case);
    patt_((TreeNode_decision)($ref decision)) {
        let left = try_(TreeNode_countRecur(decision->left, depth + 1));
        let right = try_(TreeNode_countRecur(decision->right, depth + 1));
        let descendants = orelse_((u32_addChkd(left, right))(
            return_err(E_cause$fs_File_WriteFailed())
        ));
        let total = orelse_((u32_addChkd(u32_(1), descendants))(
            return_err(E_cause$fs_File_WriteFailed())
        ));
        if (TreeFile_max_nodes < total) {
            return_err(E_cause$fs_File_WriteFailed());
        }
        return_ok(total);
    } $end(patt);
    default_() return_err(E_cause$fs_File_WriteFailed()) $end(default);
    } $end(match);
    claim_unreachable;
} $unscoped(fn);

fn_((tree_file_writeU32(
    io_Writer writer, u32 value
))(E$void) $scope) {
    let bytes = mem_writeLE32(value);
    try_(io_Writer_writeBytes(writer, A_ref$((S_const$u8)(bytes))));
    return_ok({});
} $unscoped(fn);

fn_((tree_file_readU32(io_Reader reader))(E$u32) $scope) {
    var_(bytes, mem_ReadLE32Buf) $undefined;
    try_(io_Reader_readExact(reader, A_ref$((S$u8)(bytes))));
    return_ok(mem_readLE32(bytes));
} $unscoped(fn);

fn_((TreeNode_save(
    const TreeNode* root, io_Writer writer
))(E$void) $scope) {
    let node_count = try_(TreeNode_countRecur(root, usize_(0)));
    try_(io_Writer_writeBytes(writer, u8_l("DHTR")));
    try_(io_Writer_writeByte(writer, TreeFile_version));
    try_(tree_file_writeU32(writer, node_count));
    try_(TreeNode_saveToFileRecur(root, writer));
    return_ok({});
} $unscoped(fn);

fn_((TreeNode_loadRecur(
    mem_Alctr gpa, io_Reader reader, u32* remaining, usize depth
))(E$P$TreeNode) $guard) /* NOLINT(misc-no-recursion) */ {
    claim_assert_nonnull(remaining);
    if (*remaining == 0 || TreeFile_max_depth < depth) {
        return_err(E_cause$fs_File_ReadFailed());
    }
    *remaining -= 1;
    let tag = try_(io_Reader_readByte(reader));
    switch (tag) {
    case_((TreeFile_NodeTag_leaf)) {
        let class_label = bitCast$((i32)(try_(tree_file_readU32(reader))));
        return TreeNode_createLeaf(gpa, class_label);
    } $end(case);
    case_((TreeFile_NodeTag_decision)) {
        let feature_index = try_(tree_file_readU32(reader));
        let threshold = bitCast$((f32)(try_(tree_file_readU32(reader))));
        return TreeNode_loadDecision(
            gpa, reader, remaining, depth, feature_index, threshold
        );
    } $end(case);
    default_() return_err(E_cause$fs_File_ReadFailed()) $end(default);
    }
    claim_unreachable;
} $unguarded(fn);

fn_((TreeNode_loadDecision(
    mem_Alctr gpa,
    io_Reader reader,
    u32* remaining,
    usize depth,
    u32 feature_index,
    f32 threshold
))(E$P$TreeNode) $guard) {
    var_(left, O$P$TreeNode) = none();
    var_(right, O$P$TreeNode) = none();
    errdefer_($ignore, {
        if_some((left)(node)) TreeNode_destroyRecur(gpa, node);
        if_some((right)(node)) TreeNode_destroyRecur(gpa, node);
    });
    asg_l((&left)(some(try_(TreeNode_loadRecur(
        gpa, reader, remaining, depth + 1
    )))));
    asg_l((&right)(some(try_(TreeNode_loadRecur(
        gpa, reader, remaining, depth + 1
    )))));
    return TreeNode_createDecision(
        gpa, feature_index, threshold, unwrap_(left), unwrap_(right)
    );
} $unguarded(fn);

fn_((TreeNode_load(
    mem_Alctr gpa, io_Reader reader
))(E$P$TreeNode) $guard) {
    var_(magic, A$$(4, u8)) $undefined;
    try_(io_Reader_readExact(reader, A_ref$((S$u8)(magic))));
    if (!mem_eqlBytes(A_ref$((S_const$u8)(magic)), u8_l("DHTR"))) {
        return_err(E_cause$fs_File_ReadFailed());
    }
    if (try_(io_Reader_readByte(reader)) != TreeFile_version) {
        return_err(E_cause$fs_File_ReadFailed());
    }
    var node_count = try_(tree_file_readU32(reader));
    if (node_count == 0 || TreeFile_max_nodes < node_count) {
        return_err(E_cause$fs_File_ReadFailed());
    }
    var remaining = node_count;
    var_(root, O$P$TreeNode) = none();
    errdefer_($ignore, {
        if_some((root)(node)) TreeNode_destroyRecur(gpa, node);
    });
    asg_l((&root)(some(try_(TreeNode_loadRecur(
        gpa, reader, &remaining, usize_(0)
    )))));
    if (remaining != 0) return_err(E_cause$fs_File_ReadFailed());

    var_(trailing, A$$(1, u8)) $undefined;
    if (try_(io_Reader_read(reader, A_ref$((S$u8)(trailing)))) != 0) {
        return_err(E_cause$fs_File_ReadFailed());
    }
    return_ok(unwrap_(root));
} $unguarded(fn);

fn_((Dataset_loadFromCSV(
    log_Self logger,
    mem_Alctr gpa,
    S_const$u8 filename,
    bool has_header
))(E$Dataset) $guard) {
    var flags = fs_File_OpenFlags_default;
    flags.mode = fs_OpenMode_read_only;
    let file = try_(fs_File_open(filename, flags));
    defer_(fs_File_close(file));

    let unbufd = fs_File_reader(file);
    var_(chunk, A$$(4096, u8)) $undefined;
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

    log_debug(logger, u8_l("Found {:u} lines and {:u} features in CSV"), line_count, actual_feature_count);

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
            var_(col_idx, u32) = 0;
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
        if (samples_read != line_count) {
            log_warn(logger, u8_l("Expected {:u} samples but read only {:u}"), line_count, samples_read);
        }
    }
    return_ok({
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
