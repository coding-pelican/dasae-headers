# Project Tree: dasae-headers

```txt
dasae-headers
├─ .clang-format
├─ .clangd
├─ dh
│  ├─ include
│  │  ├─ dh
│  │  │  ├─ ArrDeq.h
│  │  │  ├─ ArrList.h
│  │  │  ├─ ArrMap.h
│  │  │  ├─ ArrPDeq.h
│  │  │  ├─ ArrPQue.h
│  │  │  ├─ ArrQue.h
│  │  │  ├─ ArrSet.h
│  │  │  ├─ ArrStk.h
│  │  │  ├─ ascii.h
│  │  │  ├─ atom.h
│  │  │  ├─ builtin
│  │  │  │  ├─ arch_cfg.h
│  │  │  │  ├─ atom.h
│  │  │  │  ├─ auto.h
│  │  │  │  ├─ comp.h
│  │  │  │  ├─ comp_cfg.h
│  │  │  │  ├─ container_info.h
│  │  │  │  ├─ lambda.h
│  │  │  │  ├─ lang_cfg.h
│  │  │  │  ├─ mem.h
│  │  │  │  ├─ plat_cfg.h
│  │  │  │  ├─ pp
│  │  │  │  │  ├─ common.h
│  │  │  │  │  └─ Tuple.h
│  │  │  │  ├─ pp.h
│  │  │  │  ├─ src_loc.h
│  │  │  │  ├─ static_assert.h
│  │  │  │  └─ type_info.h
│  │  │  ├─ builtin.h
│  │  │  ├─ core
│  │  │  │  ├─ blk.h
│  │  │  │  ├─ Callable.h
│  │  │  │  ├─ chain.h
│  │  │  │  ├─ claim
│  │  │  │  │  ├─ assert.h
│  │  │  │  │  ├─ assert_static.h
│  │  │  │  │  ├─ cfg.h
│  │  │  │  │  └─ unreachable.h
│  │  │  │  ├─ claim.h
│  │  │  │  ├─ cmp.h
│  │  │  │  ├─ debug
│  │  │  │  │  ├─ assert.h
│  │  │  │  │  ├─ assert_static.h
│  │  │  │  │  ├─ cfg.h
│  │  │  │  │  └─ common.h
│  │  │  │  ├─ debug.h
│  │  │  │  ├─ fn.h
│  │  │  │  ├─ op.h
│  │  │  │  ├─ pipe.h
│  │  │  │  ├─ prim
│  │  │  │  │  ├─ bool.h
│  │  │  │  │  ├─ cfg.h
│  │  │  │  │  ├─ flt.h
│  │  │  │  │  ├─ int.h
│  │  │  │  │  ├─ null.h
│  │  │  │  │  ├─ ptr.h
│  │  │  │  │  ├─ struct.h
│  │  │  │  │  ├─ switch.h
│  │  │  │  │  ├─ union.h
│  │  │  │  │  └─ wchar.h
│  │  │  │  ├─ prim.h
│  │  │  │  ├─ range.h
│  │  │  │  ├─ scope
│  │  │  │  │  ├─ common.h
│  │  │  │  ├─ scope.h
│  │  │  │  ├─ src_loc.h
│  │  │  │  └─ type_info.h
│  │  │  ├─ core.h
│  │  │  ├─ fmt
│  │  │  │  └─ common.h
│  │  │  ├─ fmt.h
│  │  │  ├─ fs
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Dir.h
│  │  │  │  └─ File.h
│  │  │  ├─ fs.h
│  │  │  ├─ heap
│  │  │  │  ├─ Arena.h
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ Classic.h
│  │  │  │  ├─ Fixed.h
│  │  │  │  ├─ Page.h
│  │  │  │  └─ ThrdSafe.h
│  │  │  ├─ heap.h
│  │  │  ├─ io
│  │  │  │  ├─ Buf.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Reader.h
│  │  │  │  ├─ stream.h
│  │  │  │  └─ Writer.h
│  │  │  ├─ io.h
│  │  │  ├─ ListDbl.h
│  │  │  ├─ ListSgl.h
│  │  │  ├─ log.h
│  │  │  ├─ main.h
│  │  │  ├─ math
│  │  │  │  ├─ common.h
│  │  │  │  ├─ vec.h
│  │  │  │  └─ vec_types.h
│  │  │  ├─ math.h
│  │  │  ├─ mem
│  │  │  │  ├─ Allocator.h
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ common.h
│  │  │  │  └─ Tracker.h
│  │  │  ├─ mem.h
│  │  │  ├─ os
│  │  │  │  ├─ windows
│  │  │  │  │  ├─ common.h
│  │  │  │  │  └─ mem.h
│  │  │  │  └─ windows.h
│  │  │  ├─ posix.h
│  │  │  ├─ prl
│  │  │  │  ├─ common.h
│  │  │  │  ├─ ErrTrace.h
│  │  │  │  ├─ flt.h
│  │  │  │  ├─ int.h
│  │  │  │  ├─ types
│  │  │  │  │  ├─ Arr.h
│  │  │  │  │  ├─ Err.h
│  │  │  │  │  ├─ ErrRes.h
│  │  │  │  │  ├─ meta.h
│  │  │  │  │  ├─ Opt.h
│  │  │  │  │  ├─ Ptr.h
│  │  │  │  │  ├─ raw.h
│  │  │  │  │  ├─ Sli.h
│  │  │  │  │  ├─ Val.h
│  │  │  │  │  └─ variant.h
│  │  │  │  └─ types.h
│  │  │  ├─ prl.h
│  │  │  ├─ Rand.h
│  │  │  ├─ sort.h
│  │  │  ├─ Str.h
│  │  │  ├─ StrCompHash.h
│  │  │  ├─ TEST
│  │  │  │  └─ cfg.h
│  │  │  ├─ TEST.h
│  │  │  ├─ Thrd
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Cond.h
│  │  │  │  └─ Mtx.h
│  │  │  ├─ Thrd.h
│  │  │  ├─ time
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Duration.h
│  │  │  │  ├─ Instant.h
│  │  │  │  └─ SysTime.h
│  │  │  ├─ time.h
│  │  └─ dh.h
│  ├─ libs
│  ├─ src
│  │  ├─ ArrDeq.c
│  │  ├─ ArrList.c
│  │  ├─ ArrPQue.c
│  │  ├─ ArrQue.c
│  │  ├─ ArrSet.c
│  │  ├─ ArrStk.c
│  │  ├─ ascii.c
│  │  ├─ core_claim_assert.c
│  │  ├─ core_debug_assert.c
│  │  ├─ core_debug_common.c
│  │  ├─ fmt_common.c
│  │  ├─ fs_Dir.c
│  │  ├─ fs_File.c
│  │  ├─ heap_Arena.c
│  │  ├─ heap_Classic.c
│  │  ├─ heap_Fixed.c
│  │  ├─ heap_Page.c
│  │  ├─ heap_ThrdSafe.c
│  │  ├─ io_Buf.c
│  │  ├─ io_common.c
│  │  ├─ io_Reader.c
│  │  ├─ io_stream.c
│  │  ├─ io_Writer.c
│  │  ├─ ListDbl.c
│  │  ├─ ListSgl.c
│  │  ├─ log.c
│  │  ├─ mem_Allocator.c
│  │  ├─ mem_common.c
│  │  ├─ mem_Tracker.c
│  │  ├─ prl_ErrTrace.c
│  │  ├─ prl_types_Err.c
│  │  ├─ Rand.c
│  │  ├─ sort.c
│  │  ├─ Str.c
│  │  ├─ TEST.c
│  │  ├─ Thrd_common.c
│  │  ├─ Thrd_Cond.c
│  │  ├─ Thrd_Mtx.c
│  │  ├─ time_common.c
│  │  ├─ time_Duration.c
│  │  ├─ time_Instant.c
│  │  └─ time_SysTime.c
│  └─ tests
│     ├─ draft
│     │  ├─ draft-async.c
│     │  ├─ draft-async.h
│     │  ├─ draft-async_actor.c
│     │  ├─ draft-async_actor_boids.c
│     │  ├─ draft-async_ex.h
│     │  ├─ draft-async_exec.c
│     │  ├─ draft-async_typing.c
│     ├─ example
│     │  ├─ example-ArrList.c
│     │  ├─ example-atom_producer_consumer.c
│     │  ├─ example-detect_double_free.c
│     │  ├─ example-detect_memory_leak.c
│     │  ├─ example-err_res.c
│     │  ├─ example-fn.c
│     │  ├─ example-hello_world.c
│     │  ├─ example-io_Buf_Reader.c
│     │  ├─ example-io_Buf_Writer.c
│     │  ├─ example-opt.c
│     │  ├─ example-orelse.c
│     │  ├─ example-rev_args_str.c
│     │  ├─ example-srcLoc.c
│     │  ├─ example-Thrd.c
│     │  ├─ example-trace_err.c
│     ├─ sample
│     │  ├─ sample-decision_tree.c
│     │  ├─ sample-particle.c
│     │  ├─ sample-particle_pool.c
│     │  ├─ sample-particle_pool_opt.c
│     │  └─ sample-tetris.c
│     ├─ test-ArrPQue.c
│     ├─ test-ArrSet.c
│     ├─ test-Arr_cat.c
│     ├─ test-Arr_ref.c
│     ├─ test-breaking-guard.c
│     ├─ test-cmp.c
│     ├─ test-defer.c
│     ├─ test-fmt_parse.c
│     ├─ test-heap_Arena.c
│     ├─ test-io_common.c
│     ├─ test-io_Writer-print.c
│     ├─ test-io_Writer-print_simple.c
│     ├─ test-io_Writer-print_w_arg_idx.c
│     ├─ test-ListSgl.c
│     ├─ test-mem_Allocator.c
│     ├─ test-mem_copy.c
│     ├─ test-TEST.c
│     ├─ test-TEST_sample.c
│     └─ test-Thrd.c
├─ dh-examples
│  ├─ .clangd
│  ├─ barnes-hut
│  │  ├─ main
│  │  │  ├─ .clangd
│  │  │  ├─ include
│  │  │  │  ├─ barnes-hut
│  │  │  │  │  ├─ Body.h
│  │  │  │  │  ├─ cfg_values.h
│  │  │  │  │  ├─ QuadTree.h
│  │  │  │  │  ├─ Simulation.h
│  │  │  │  │  ├─ utils.h
│  │  │  │  │  └─ Visualizer.h
│  │  │  │  └─ barnes-hut.h
│  │  │  ├─ lib
│  │  │  └─ src
│  │  │     └─ barnes-hut
│  │  │        ├─ barnes-hut.c
│  │  │        ├─ Body.c
│  │  │        ├─ QuadTree.c
│  │  │        ├─ Simulation.c
│  │  │        ├─ utils.c
│  │  │        └─ Visualizer.c
│  ├─ engine
│  │  ├─ .clangd
│  │  ├─ include
│  │  │  ├─ engine
│  │  │  │  ├─ Backend.h
│  │  │  │  ├─ Canvas.h
│  │  │  │  ├─ Color.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ core
│  │  │  │  │  ├─ Vt100.h
│  │  │  │  │  └─ VT100Ex.h
│  │  │  │  ├─ Grid.h
│  │  │  │  ├─ Input.h
│  │  │  │  ├─ Keyboard.h
│  │  │  │  ├─ KeyButtonStates.h
│  │  │  │  ├─ KeyCode.h
│  │  │  │  ├─ Mouse.h
│  │  │  │  ├─ MouseButton.h
│  │  │  │  ├─ UnicodeCanvas.h
│  │  │  │  ├─ utils.h
│  │  │  │  └─ Window.h
│  │  │  └─ engine.h
│  │  ├─ lib
│  │  ├─ samples
│  │  │  ├─ sample-circle_physics_2d-wip.c
│  │  ├─ src
│  │  │  └─ engine
│  │  │     ├─ Backend.c
│  │  │     ├─ Backend_Internal.h
│  │  │     ├─ Canvas.c
│  │  │     ├─ Color.c
│  │  │     ├─ core
│  │  │     │  ├─ Vt100.c
│  │  │     │  └─ VT100Ex.c
│  │  │     ├─ Input.c
│  │  │     ├─ Keyboard.c
│  │  │     ├─ Mouse.c
│  │  │     ├─ utils.c
│  │  │     └─ Window.c
│  ├─ engine-stub
│  │  └─ VT100Ex
│  │     ├─ .clangd
│  │     ├─ include
│  │     │  └─ VT100Ex.h
│  │     ├─ lib
│  │     └─ src
│  │        └─ VT100Ex.c
│  ├─ subframes
│  │  ├─ .clangd
│  │  ├─ include
│  │  │  └─ subframes.h
│  │  ├─ lib
│  │  ├─ src
│  │  │  └─ subframes.c
│  │  └─ tools
├─ LICENSE
└─ README.md
```
