# Project Tree: dasae-headers

```txt
dasae-headers
├─ .clang-format
├─ .clangd
├─ dh
│  ├─ include
│  │  ├─ dh
│  │  │  ├─ builtin.h
│  │  │  ├─ builtin
│  │  │  │  ├─ pp.h
│  │  │  │  ├─ pp
│  │  │  │  │  ├─ common.h
│  │  │  │  │  └─ Tuple.h
│  │  │  │  ├─ lang_cfg.h
│  │  │  │  ├─ arch_cfg.h
│  │  │  │  ├─ plat_cfg.h
│  │  │  │  ├─ comp_cfg.h
│  │  │  │  ├─ comp.h
│  │  │  │  ├─ src_loc.h
│  │  │  │  ├─ static_assert.h
│  │  │  │  ├─ auto.h
│  │  │  │  ├─ lambda.h
│  │  │  │  ├─ type_info.h
│  │  │  │  ├─ container_info.h
│  │  │  │  ├─ mem.h
│  │  │  │  └─ atom.h
│  │  │  ├─ core.h
│  │  │  ├─ core
│  │  │  │  ├─ prim.h
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
│  │  │  │  ├─ fn.h
│  │  │  │  ├─ Callable.h
│  │  │  │  ├─ claim.h
│  │  │  │  ├─ claim
│  │  │  │  │  ├─ cfg.h
│  │  │  │  │  ├─ assert.h
│  │  │  │  │  ├─ assert_static.h
│  │  │  │  │  └─ unreachable.h
│  │  │  │  ├─ debug.h
│  │  │  │  ├─ debug
│  │  │  │  │  ├─ cfg.h
│  │  │  │  │  ├─ assert.h
│  │  │  │  │  ├─ assert_static.h
│  │  │  │  │  └─ common.h
│  │  │  │  ├─ range.h
│  │  │  │  ├─ op.h
│  │  │  │  ├─ cmp.h
│  │  │  │  ├─ pipe.h
│  │  │  │  ├─ chain.h
│  │  │  │  ├─ blk.h
│  │  │  │  ├─ scope.h
│  │  │  │  ├─ scope
│  │  │  │  │  └─ common.h
│  │  │  │  ├─ src_loc.h
│  │  │  │  ├─ type_info.h
│  │  │  │  └─ struct_layout.h
│  │  │  ├─ prl.h
│  │  │  ├─ prl
│  │  │  │  └─ types.h
│  │  │  │  ├─ types
│  │  │  │  │  ├─ raw.h
│  │  │  │  │  ├─ Val.h
│  │  │  │  │  ├─ Ptr.h
│  │  │  │  │  ├─ Arr.h
│  │  │  │  │  ├─ Sli.h
│  │  │  │  │  ├─ Opt.h
│  │  │  │  │  ├─ ErrRes.h
│  │  │  │  │  ├─ Err.h
│  │  │  │  │  └─ variant.h
│  │  │  │  │  ├─ meta.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ ErrTrace.h
│  │  │  │  ├─ int.h
│  │  │  │  ├─ flt.h
│  │  │  ├─ atom.h // Atomic Value
│  │  │  ├─ simd.h // SIMD Vector
│  │  │  ├─ cmp.h // Eql + Ord
│  │  │  ├─ math.h
│  │  │  ├─ math
│  │  │  │  ├─ common.h
│  │  │  │  └─ vec_types.h
│  │  │  │  ├─ vec.h
│  │  │  │  └─ quat_types.h
│  │  │  │  ├─ quat.h
│  │  │  │  └─ mat_types.h
│  │  │  │  ├─ mat.h
│  │  │  │  └─ geom_types.h
│  │  │  │  ├─ geom.h
│  │  │  │  └─ ease.h
│  │  │  ├─ mem.h
│  │  │  ├─ mem
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Allocator.h
│  │  │  │  └─ Tracker.h
│  │  │  ├─ meta.h
│  │  │  ├─ sort.h
│  │  │  ├─ search.h
│  │  │  ├─ ascii.h
│  │  │  ├─ utf.h
│  │  │  ├─ ListSgl.h
│  │  │  ├─ ListDbl.h
│  │  │  ├─ BTree.h
│  │  │  ├─ SegTree.h
│  │  │  ├─ ArrList.h
│  │  │  ├─ ArrStk.h
│  │  │  ├─ ArrDeq.h
│  │  │  ├─ ArrQue.h
│  │  │  ├─ ArrPDeq.h
│  │  │  ├─ ArrPQue.h
│  │  │  ├─ Hash.h
│  │  │  ├─ HashMap.h
│  │  │  ├─ HashSet.h
│  │  │  ├─ HashMapSeq.h
│  │  │  ├─ HashSetSeq.h
│  │  │  ├─ BTreeMap.h
│  │  │  ├─ BTreeSet.h
│  │  │  ├─ os.h
│  │  │  ├─ os
│  │  │  │  ├─ windows
│  │  │  │  │  ├─ common.h
│  │  │  │  │  └─ mem.h
│  │  │  │  └─ windows.h
│  │  │  ├─ posix.h
│  │  │  ├─ Thrd.h
│  │  │  ├─ Thrd
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Mtx.h
│  │  │  │  ├─ Cond.h
│  │  │  │  ├─ RwLock.h
│  │  │  │  ├─ Sem.h
│  │  │  │  ├─ ResetEvent.h
│  │  │  │  └─ WaitGroup.h
│  │  │  ├─ Co.h // Coroutine
│  │  │  ├─ async.h
│  │  │  ├─ mp.h // Multi-Processing
│  │  │  ├─ heap.h
│  │  │  ├─ heap
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ Classic.h
│  │  │  │  ├─ Page.h
│  │  │  │  ├─ Fixed.h
│  │  │  │  ├─ Arena.h
│  │  │  │  ├─ Pool.h
│  │  │  │  └─ ThrdSafe.h
│  │  │  ├─ time.h
│  │  │  ├─ time
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Duration.h
│  │  │  │  ├─ Instant.h
│  │  │  │  └─ SysTime.h
│  │  │  ├─ Rand.h
│  │  │  ├─ fs.h
│  │  │  ├─ fs
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Dir.h
│  │  │  │  └─ File.h
│  │  │  ├─ io.h
│  │  │  ├─ io
│  │  │  │  ├─ common.h
│  │  │  │  ├─ stream.h
│  │  │  │  ├─ Reader.h
│  │  │  │  ├─ Writer.h
│  │  │  │  └─ Buf.h
│  │  │  ├─ fmt.h
│  │  │  ├─ fmt
│  │  │  │  └─ common.h
│  │  │  ├─ log.h
│  │  │  ├─ proc.h
│  │  │  ├─ net.h
│  │  │  ├─ http.h
│  │  │  ├─ TEST.h
│  │  │  ├─ TEST
│  │  │  │  └─ cfg.h
│  │  │  ├─ main.h
│  │  └─ dh.h
│  ├─ src
│  │  └─ ...
│  ├─ libs
│  │  └─ BlocksRuntime
│  │     ├─ include
│  │     └─ src
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
├─ dh-c.c
├─ install.sh
├─ README.md
├─ README.ko.md
└─ LICENSE
```
