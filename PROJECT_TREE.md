# Project Tree: dasae-headers

```txt
dasae-headers
├─ compile_flags.txt
├─ .clangd
├─ .clang-format
├─ install.sh
├─ dh
│  ├─ .clangd
│  ├─ include
│  │  ├─ dh
│  │  │  ├─ ArrDeq.h
│  │  │  ├─ ArrList.h
│  │  │  ├─ ArrPDeq.h
│  │  │  ├─ ArrPQue.h
│  │  │  ├─ ArrQue.h
│  │  │  ├─ ArrStk.h
│  │  │  ├─ ascii.h
│  │  │  ├─ atom.h
│  │  │  ├─ builtin.h
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
│  │  │  │  ├─ pp.h
│  │  │  │  ├─ pp
│  │  │  │  │  ├─ common.h
│  │  │  │  │  └─ Tuple.h
│  │  │  │  ├─ prefetch.h
│  │  │  │  ├─ src_loc.h
│  │  │  │  ├─ static_assert.h
│  │  │  │  └─ type_info.h
│  │  │  ├─ Clsr.h
│  │  │  ├─ cmp.h
│  │  │  ├─ Co.h
│  │  │  ├─ Co
│  │  │  │  └─ Fiber.h
│  │  │  ├─ core
│  │  │  │  ├─ chain.h
│  │  │  │  ├─ claim.h
│  │  │  │  ├─ claim
│  │  │  │  │  ├─ assert.h
│  │  │  │  │  ├─ assert_static.h
│  │  │  │  │  ├─ cfg.h
│  │  │  │  │  └─ unreachable.h
│  │  │  │  ├─ cmp.h
│  │  │  │  ├─ debug.h
│  │  │  │  ├─ debug
│  │  │  │  │  ├─ assert.h
│  │  │  │  │  ├─ assert_static.h
│  │  │  │  │  ├─ cfg.h
│  │  │  │  │  ├─ common.h
│  │  │  │  │  ├─ src_loc.h
│  │  │  │  │  ├─ StackTrace.h
│  │  │  │  │  └─ type_info.h
│  │  │  │  ├─ fn.h
│  │  │  │  ├─ op.h
│  │  │  │  ├─ pipe.h
│  │  │  │  ├─ pri.h
│  │  │  │  ├─ pri
│  │  │  │  │  ├─ cfg.h
│  │  │  │  │  ├─ enum.h
│  │  │  │  │  └─ switch.h
│  │  │  │  ├─ range.h
│  │  │  │  ├─ scope.h
│  │  │  │  ├─ scope
│  │  │  │  │  ├─ common.h
│  │  │  │  │  ├─ defer.h
│  │  │  │  │  └─ return.h
│  │  │  │  ├─ src_loc.h
│  │  │  │  └─ type_info.h
│  │  │  ├─ core.h
│  │  │  ├─ crypt.h
│  │  │  ├─ fmt.h
│  │  │  ├─ fmt
│  │  │  │  ├─ cfg.h
│  │  │  │  └─ common.h
│  │  │  ├─ fs.h
│  │  │  ├─ fs
│  │  │  │  ├─ base.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Dir.h
│  │  │  │  ├─ File.h
│  │  │  │  ├─ File
│  │  │  │  │  ├─ Atom.h
│  │  │  │  │  ├─ MemMap.h
│  │  │  │  │  └─ self.h
│  │  │  │  └─ path.h
│  │  │  ├─ hash.h
│  │  │  ├─ HashMap.h
│  │  │  ├─ HashSet.h
│  │  │  ├─ heap.h
│  │  │  ├─ heap
│  │  │  │  ├─ Arena.h
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ Classic.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Fixed.h
│  │  │  │  ├─ Page.h
│  │  │  │  ├─ Pool.h
│  │  │  │  ├─ Sbrk.h
│  │  │  │  ├─ Smp.h
│  │  │  │  ├─ Sys.h
│  │  │  │  ├─ ThrdSafe.h
│  │  │  │  ├─ vmap.h
│  │  │  │  └─ vmem.h
│  │  │  ├─ io.h
│  │  │  ├─ io
│  │  │  │  ├─ base.h
│  │  │  │  ├─ Buf.h
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Fixed.h
│  │  │  │  ├─ Reader.h
│  │  │  │  ├─ stream.h
│  │  │  │  └─ Writer.h
│  │  │  ├─ Limit.h
│  │  │  ├─ ListDbl.h
│  │  │  ├─ ListSgl.h
│  │  │  ├─ log.h
│  │  │  ├─ math.h
│  │  │  ├─ math
│  │  │  │  ├─ common.h
│  │  │  │  ├─ ease.h
│  │  │  │  ├─ geom.h
│  │  │  │  ├─ geom_types.h
│  │  │  │  ├─ interp.h
│  │  │  │  ├─ mat.h
│  │  │  │  ├─ mat_types.h
│  │  │  │  ├─ quat.h
│  │  │  │  ├─ quat_types.h
│  │  │  │  ├─ vec.h
│  │  │  │  └─ vec_types.h
│  │  │  ├─ mem.h
│  │  │  ├─ mem
│  │  │  │  ├─ Alctr.h
│  │  │  │  ├─ AlcTrace.h
│  │  │  │  ├─ base.h
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ common.h
│  │  │  │  └─ dyn.h
│  │  │  ├─ meta.h
│  │  │  ├─ net.h
│  │  │  ├─ net
│  │  │  │  ├─ base.h
│  │  │  │  ├─ Clt.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Sock.h
│  │  │  │  ├─ Stream.h
│  │  │  │  └─ Svr.h
│  │  │  ├─ os.h
│  │  │  ├─ os
│  │  │  │  ├─ windows.h
│  │  │  │  └─ windows
│  │  │  │     ├─ auth.h
│  │  │  │     ├─ base.h
│  │  │  │     ├─ cfg.h
│  │  │  │     ├─ common.h
│  │  │  │     ├─ console.h
│  │  │  │     ├─ crypt.h
│  │  │  │     ├─ debug.h
│  │  │  │     ├─ dll.h
│  │  │  │     ├─ except.h
│  │  │  │     ├─ file.h
│  │  │  │     ├─ handle.h
│  │  │  │     ├─ io.h
│  │  │  │     ├─ mem.h
│  │  │  │     ├─ nls.h
│  │  │  │     ├─ nt.h
│  │  │  │     ├─ pp-def
│  │  │  │     │  ├─ guard.h
│  │  │  │     │  └─ unguarded.h
│  │  │  │     ├─ proc.h
│  │  │  │     ├─ sock.h
│  │  │  │     ├─ socket.h
│  │  │  │     ├─ sync.h
│  │  │  │     ├─ sysinfo.h
│  │  │  │     └─ thrd.h
│  │  │  ├─ posix.h
│  │  │  ├─ prl
│  │  │  │  ├─ base.h
│  │  │  │  ├─ base
│  │  │  │  │  ├─ exts.h
│  │  │  │  │  ├─ refs.h
│  │  │  │  │  └─ vals.h
│  │  │  │  ├─ Clsr.h
│  │  │  │  ├─ Co.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ CompHash.h
│  │  │  │  ├─ Err.h
│  │  │  │  ├─ ErrTrace.h
│  │  │  │  ├─ flt.h
│  │  │  │  ├─ int.h
│  │  │  │  ├─ meta.h
│  │  │  │  ├─ raw.h
│  │  │  │  ├─ simd.h
│  │  │  │  ├─ tpl.h
│  │  │  │  └─ tpl
│  │  │  │     ├─ Arr.h
│  │  │  │     ├─ ErrRes.h
│  │  │  │     ├─ ErrSet.h
│  │  │  │     ├─ Opt.h
│  │  │  │     ├─ Ptr.h
│  │  │  │     ├─ Sli.h
│  │  │  │     ├─ Tup.h
│  │  │  │     ├─ Val.h
│  │  │  │     └─ variant.h
│  │  │  ├─ prl.h
│  │  │  ├─ proc.h
│  │  │  ├─ proc
│  │  │  │  ├─ base.h
│  │  │  │  ├─ Child.h
│  │  │  │  └─ common.h
│  │  │  ├─ Rand.h
│  │  │  ├─ Range.h
│  │  │  ├─ search.h
│  │  │  ├─ simd.h
│  │  │  ├─ sort.h
│  │  │  ├─ TEST.h
│  │  │  ├─ TEST
│  │  │  │  └─ cfg.h
│  │  │  ├─ Thrd.h
│  │  │  ├─ Thrd
│  │  │  │  ├─ base.h
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Cond.h
│  │  │  │  ├─ ftx.h
│  │  │  │  ├─ Mtx.h
│  │  │  │  ├─ ResetEvent.h
│  │  │  │  ├─ RWLock.h
│  │  │  │  ├─ self.h
│  │  │  │  ├─ Sem.h
│  │  │  │  └─ WaitGroup.h
│  │  │  ├─ time.h
│  │  │  ├─ time
│  │  │  │  ├─ cfg.h
│  │  │  │  ├─ common.h
│  │  │  │  ├─ Duration.h
│  │  │  │  ├─ Instant.h
│  │  │  │  └─ SysTime.h
│  │  │  ├─ unicode.h
│  │  │  ├─ utf16.h
│  │  │  ├─ utf8.h
│  │  │  ├─ wtf16.h
│  │  │  └─ wtf8.h
│  │  ├─ dh-TEST-main.h
│  │  ├─ dh-main.h
│  │  └─ dh.h
│  ├─ src
│  │  └─ dh
│  │     ├─ ArrDeq.c
│  │     ├─ ArrList.c
│  │     ├─ ArrPDeq.c
│  │     ├─ ArrPQue.c
│  │     ├─ ArrQue.c
│  │     ├─ ArrStk.c
│  │     ├─ ascii.c
│  │     ├─ cmp.c
│  │     ├─ core
│  │     │  ├─ claim
│  │     │  │  └─ assert.c
│  │     │  └─ debug
│  │     │     ├─ assert.c
│  │     │     ├─ common.c
│  │     │     └─ StackTrace.c
│  │     ├─ fmt
│  │     │  ├─ common.c
│  │     │  └─ private
│  │     │     ├─ impl
│  │     │     │  ├─ flt.c
│  │     │     │  ├─ ryu.c
│  │     │     │  └─ ryu.h
│  │     │     └─ share.h
│  │     ├─ fs
│  │     │  ├─ Dir.c
│  │     │  ├─ File
│  │     │  │  ├─ Atom.c
│  │     │  │  ├─ MemMap.c
│  │     │  │  └─ self.c
│  │     │  ├─ path.c
│  │     │  └─ private
│  │     │     └─ share.h
│  │     ├─ hash.c
│  │     ├─ HashMap.c
│  │     ├─ HashSet.c
│  │     ├─ heap
│  │     │  ├─ Arena.c
│  │     │  ├─ Classic.c
│  │     │  ├─ Fixed.c
│  │     │  ├─ Page.c
│  │     │  ├─ Sbrk
│  │     │  │  ├─ Arena.c
│  │     │  │  ├─ Fixed.c
│  │     │  │  └─ Sys.c
│  │     │  ├─ Sbrk.c
│  │     │  ├─ Smp.c
│  │     │  ├─ Sys.c
│  │     │  ├─ ThrdSafe.c
│  │     │  ├─ vmap.c
│  │     │  └─ vmem.c
│  │     ├─ io
│  │     │  ├─ Buf.c
│  │     │  ├─ common.c
│  │     │  ├─ Fixed.c
│  │     │  ├─ Reader.c
│  │     │  ├─ stream.c
│  │     │  └─ Writer.c
│  │     ├─ ListDbl.c
│  │     ├─ ListSgl.c
│  │     ├─ log.c
│  │     ├─ mem
│  │     │  ├─ Alctr.c
│  │     │  ├─ AlcTrace.c
│  │     │  └─ common.c
│  │     ├─ net
│  │     │  ├─ Clt.c
│  │     │  ├─ common.c
│  │     │  ├─ private
│  │     │  │  └─ share.h
│  │     │  ├─ Stream.c
│  │     │  └─ Svr.c
│  │     ├─ prl
│  │     │  ├─ Err.c
│  │     │  └─ ErrTrace.c
│  │     ├─ proc
│  │     │  ├─ Child.c
│  │     │  └─ common.c
│  │     ├─ Rand.c
│  │     ├─ search.c
│  │     ├─ sort.c
│  │     ├─ TEST.c
│  │     ├─ Thrd
│  │     │  ├─ common.c
│  │     │  ├─ Cond.c
│  │     │  ├─ ftx.c
│  │     │  ├─ Mtx.c
│  │     │  ├─ ResetEvent.c
│  │     │  ├─ RWLock.c
│  │     │  ├─ Sem.c
│  │     │  └─ WaitGroup.c
│  │     ├─ time
│  │     │  ├─ common.c
│  │     │  ├─ Duration.c
│  │     │  ├─ Instant.c
│  │     │  └─ SysTime.c
│  │     ├─ unicode.c
│  │     ├─ utf16.c
│  │     ├─ utf8.c
│  │     ├─ wtf16.c
│  │     └─ wtf8.c
│  ├─ lab
│  │  └─ drafts
│  │     └─ io_conc
│  │        ├─ ARCH.md
│  │        ├─ dh
│  │        │  ├─ exec.h
│  │        │  ├─ exec
│  │        │  │  ├─ base.h
│  │        │  │  ├─ common.c
│  │        │  │  ├─ common.h
│  │        │  │  ├─ Coop.c
│  │        │  │  ├─ Coop.h
│  │        │  │  ├─ Evented.h
│  │        │  │  ├─ Evented
│  │        │  │  │  ├─ IOCP.c
│  │        │  │  │  ├─ IOCP.h
│  │        │  │  │  ├─ self.c
│  │        │  │  │  └─ self.h
│  │        │  │  ├─ Fiber.c
│  │        │  │  ├─ Fiber.h
│  │        │  │  ├─ FiberGrow.c
│  │        │  │  ├─ FiberGrow.h
│  │        │  │  ├─ Lane.c
│  │        │  │  ├─ Lane.h
│  │        │  │  ├─ LaneTimed.c
│  │        │  │  ├─ LaneTimed.h
│  │        │  │  ├─ Para.c
│  │        │  │  ├─ Para.h
│  │        │  │  ├─ Preem.c
│  │        │  │  ├─ Preem.h
│  │        │  │  ├─ Seq.c
│  │        │  │  ├─ Seq.h
│  │        │  │  ├─ Task.c
│  │        │  │  ├─ Task.h
│  │        │  │  └─ Timer.h
│  │        │  ├─ fs.h
│  │        │  ├─ fs
│  │        │  │  ├─ base.h
│  │        │  │  ├─ common.h
│  │        │  │  ├─ Dir.c
│  │        │  │  ├─ Dir.h
│  │        │  │  ├─ File.h
│  │        │  │  ├─ File
│  │        │  │  │  ├─ Atom.c
│  │        │  │  │  ├─ Atom.h
│  │        │  │  │  ├─ MemMap.c
│  │        │  │  │  ├─ MemMap.h
│  │        │  │  │  ├─ self.c
│  │        │  │  │  └─ self.h
│  │        │  │  ├─ path.c
│  │        │  │  ├─ path.h
│  │        │  │  ├─ Self.c
│  │        │  │  └─ Self.h
│  │        │  ├─ Future.h
│  │        │  ├─ Future
│  │        │  │  ├─ base.h
│  │        │  │  ├─ self.c
│  │        │  │  └─ self.h
│  │        │  ├─ io.h
│  │        │  ├─ io
│  │        │  │  ├─ base.h
│  │        │  │  ├─ Buf.c
│  │        │  │  ├─ Buf.h
│  │        │  │  ├─ cfg.h
│  │        │  │  ├─ common.c
│  │        │  │  ├─ common.h
│  │        │  │  ├─ Fixed.c
│  │        │  │  ├─ Fixed.h
│  │        │  │  ├─ Reader.c
│  │        │  │  ├─ Reader.h
│  │        │  │  ├─ Self.c
│  │        │  │  ├─ Self.h
│  │        │  │  ├─ stream.c
│  │        │  │  ├─ stream.h
│  │        │  │  ├─ Writer.c
│  │        │  │  └─ Writer.h
│  │        │  ├─ net.h
│  │        │  ├─ net
│  │        │  │  ├─ base.h
│  │        │  │  ├─ Clt.c
│  │        │  │  ├─ Clt.h
│  │        │  │  ├─ common.h
│  │        │  │  ├─ Self.c
│  │        │  │  ├─ Self.h
│  │        │  │  ├─ Sock.h
│  │        │  │  ├─ Stream.c
│  │        │  │  ├─ Stream.h
│  │        │  │  ├─ Svr.c
│  │        │  │  └─ Svr.h
│  │        │  ├─ proc.h
│  │        │  ├─ proc
│  │        │  │  ├─ base.h
│  │        │  │  ├─ Child.c
│  │        │  │  ├─ Child.h
│  │        │  │  ├─ common.h
│  │        │  │  ├─ Self.c
│  │        │  │  └─ Self.h
│  │        │  ├─ Sched.h
│  │        │  ├─ Sched
│  │        │  │  ├─ base.h
│  │        │  │  ├─ self.c
│  │        │  │  └─ self.h
│  │        │  ├─ Thrd.h
│  │        │  ├─ Thrd
│  │        │  │  ├─ base.h
│  │        │  │  ├─ cfg.h
│  │        │  │  ├─ common.h
│  │        │  │  ├─ Cond.c
│  │        │  │  ├─ Cond.h
│  │        │  │  ├─ ftx.c
│  │        │  │  ├─ ftx.h
│  │        │  │  ├─ Mtx.c
│  │        │  │  ├─ Mtx.h
│  │        │  │  ├─ ResetEvent.c
│  │        │  │  ├─ ResetEvent.h
│  │        │  │  ├─ RWLock.c
│  │        │  │  ├─ RWLock.h
│  │        │  │  ├─ self.c
│  │        │  │  ├─ self.h
│  │        │  │  ├─ Sem.c
│  │        │  │  ├─ Sem.h
│  │        │  │  ├─ WaitGroup.c
│  │        │  │  └─ WaitGroup.h
│  │        │  ├─ time.h
│  │        │  └─ time
│  │        │     ├─ base.h
│  │        │     ├─ cfg.h
│  │        │     ├─ Clock.h
│  │        │     ├─ common.h
│  │        │     ├─ Dur.c
│  │        │     ├─ Dur.h
│  │        │     ├─ Inst.c
│  │        │     ├─ Inst.h
│  │        │     ├─ self.h
│  │        │     └─ self
│  │        │       ├─ Awake.c
│  │        │       ├─ Awake.h
│  │        │       ├─ Boot.c
│  │        │       ├─ Boot.h
│  │        │       ├─ internal.c
│  │        │       ├─ internal.h
│  │        │       ├─ Proc.c
│  │        │       ├─ Proc.h
│  │        │       ├─ Real.c
│  │        │       ├─ Real.h
│  │        │       ├─ Thrd.c
│  │        │       └─ Thrd.h
│  │        ├─ examples
│  │        │  └─ actor
│  │        │     ├─ actor.c
│  │        │     ├─ actor.h
│  │        │     └─ main.c
│  │        ├─ sample-count.c
│  │        ├─ sample-multi_game_loop.c
│  │        ├─ sample-typing.c
│  │        ├─ sample.go
│  │        ├─ sample.raw.c
│  │        ├─ sample.rs
│  │        ├─ sample.zig
│  │        └─ test-seq-coop.c
│  ├─ samples
│  │  ├─ sample-ArrList.c
│  │  ├─ sample-detect_double_free.c
│  │  ├─ sample-detect_memory_leak.c
│  │  ├─ sample-err_res.c
│  │  ├─ sample-hello_world.c
│  │  ├─ sample-io_Buf_Reader.c
│  │  ├─ sample-io_Buf_Writer.c
│  │  ├─ sample-opt.c
│  │  ├─ sample-orelse.c
│  │  ├─ sample-rev_args_str.c
│  │  ├─ sample-srcLoc.c
│  │  ├─ sample-str.c
│  │  ├─ sample-Thrd.c
│  │  └─ sample-trace_err.c
│  ├─ examples
│  │  ├─ example-decision_tree.c
│  └─ tests
│     ├─ test-ArrPDeq.c
│     ├─ test-ArrPQue.c
│     ├─ test-ascii.c
│     ├─ test-Clsr.c
│     ├─ test-cmp.c
│     ├─ test-core_chain.c
│     ├─ test-core_fn-breaking-guard.c
│     ├─ test-core_fn-defer.c
│     ├─ test-fmt-parse.c
│     ├─ test-fs_File.c
│     ├─ test-fs_File_MemMap.c
│     ├─ test-fs_path.c
│     ├─ test-hash.c
│     ├─ test-HashMap.c
│     ├─ test-HashSet.c
│     ├─ test-heap_Arena.c
│     ├─ test-heap_Classic.c
│     ├─ test-heap_common.c
│     ├─ test-heap_Fixed.c
│     ├─ test-heap_Page.c
│     ├─ test-heap_Sbrk.c
│     ├─ test-heap_Smp.c
│     ├─ test-heap_Sys.c
│     ├─ test-io_Buf_Reader.c
│     ├─ test-io_common.c
│     ├─ test-io_Reader.c
│     ├─ test-io_Writer-newline.c
│     ├─ test-io_Writer-print.c
│     ├─ test-io_Writer-print_simple.c
│     ├─ test-io_Writer-print_w_arg_idx.c
│     ├─ test-ListSgl.c
│     ├─ test-mem-basic.c
│     ├─ test-mem-build.c
│     ├─ test-mem-find.c
│     ├─ test-mem-iter.c
│     ├─ test-meta.c
│     ├─ test-net.c
│     ├─ test-prl_Err.c
│     ├─ test-prl_ErrTrace.c
│     ├─ test-prl_tpl_Arr-cat.c
│     ├─ test-prl_tpl_Arr-ref.c
│     ├─ test-prl_tpl_Opt.c
│     ├─ test-proc.c
│     ├─ test-Rand.c
│     ├─ test-sort-pdq.c
│     ├─ test-TEST-sample.c
│     ├─ test-TEST.c
│     ├─ test-Thrd.c
│     └─ test-time.c
├─ dh-c
│  ├─ .clangd
│  ├─ gen-makefile.sh
│  ├─ include
│  │  ├─ dal-c-ext.h
│  │  ├─ dal-c-ext
│  │  │  ├─ ArrStr.h
│  │  │  ├─ base.h
│  │  │  ├─ dir.h
│  │  │  ├─ dsl.h
│  │  │  ├─ env.h
│  │  │  ├─ file.h
│  │  │  ├─ path.h
│  │  │  ├─ pp.h
│  │  │  ├─ proc.h
│  │  │  └─ str.h
│  │  └─ dal-c.h
│  ├─ src
│  │  ├─ dal-c-ext
│  │  │  ├─ ArrStr.c
│  │  │  ├─ dir.c
│  │  │  ├─ env.c
│  │  │  ├─ file.c
│  │  │  ├─ path.c
│  │  │  ├─ proc.c
│  │  │  └─ str.c
│  │  ├─ dal-c.c
│  │  └─ dal-c
│  │     ├─ build.c
│  │     ├─ Cmd.c
│  │     ├─ internal.h
│  │     └─ Project.c
│  ├─ lab
│  │  ├─ compile_flags.txt
│  │  ├─ .clangd
│  │  ├─ pkg-deps
│  │  │  ├─ A
│  │  │  │  ├─ .clangd
│  │  │  │  ├─ include
│  │  │  │  │  └─ A.h
│  │  │  │  ├─ src
│  │  │  │  │  └─ A.c
│  │  │  │  ├─ other
│  │  │  │  │  └─ AA
│  │  │  │  │     ├─ .clangd
│  │  │  │  │     ├─ include
│  │  │  │  │     │  └─ AA.h
│  │  │  │  │     └─ src
│  │  │  │  │        └─ AA.c
│  │  │  ├─ B
│  │  │  │  ├─ .clangd
│  │  │  │  ├─ include
│  │  │  │  │  └─ B.h
│  │  │  │  ├─ src
│  │  │  │  │  └─ B.c
│  │  │  │  └─ test
│  │  │  │     └─ test-B.c
│  │  │  ├─ B-stub
│  │  │  │  ├─ .clangd
│  │  │  │  ├─ include
│  │  │  │  │  └─ B-stub.h
│  │  │  │  └─ src
│  │  │  │     └─ B-stub.c
│  │  │  ├─ C
│  │  │  │  ├─ .clangd
│  │  │  │  ├─ include
│  │  │  │  │  └─ C.h
│  │  │  │  ├─ src
│  │  │  │  │  └─ C.c
│  │  │  │  └─ test
│  │  │  │     └─ test-C.c
│  │  │  └─ extra
│  │  │     ├─ edition
│  │  │     │  └─ bar
│  │  │     │     ├─ .clangd
│  │  │     │     ├─ include
│  │  │     │     │  └─ bar.h
│  │  │     │     └─ src
│  │  │     │        └─ bar.c
│  │  │     └─ foo
│  │  │        ├─ .clangd
│  │  │        ├─ include
│  │  │        │  └─ foo.h
│  │  │        └─ src
│  │  │           └─ foo.c
│  │  ├─ pkg-header_only
│  │  │  └─ myheader
│  │  │     ├─ .clangd
│  │  │     ├─ include
│  │  │     │  └─ myheader.h
│  │  │     ├─ examples
│  │  │     │  └─ example-usage.c
│  │  │     └─ tests
│  │  │        ├─ test-fromS.c
│  │  │        ├─ test-height.c
│  │  │        ├─ test-len.c
│  │  │        ├─ test-toS.c
│  │  │        └─ test-width.c
│  │  ├─ pkg-no_dsl_contract
│  │  │  ├─ .clangd
│  │  │  └─ source
│  │  │     ├─ main.c
│  │  │     └─ project-only.c
│  │  ├─ target-root-compat
│  │  │  ├─ .clangd
│  │  │  ├─ include
│  │  │  │  └─ target-root-compat.h
│  │  │  ├─ src
│  │  │  │  └─ compat.c
│  │  │  ├─ samples
│  │  │  │  └─ sample-usage.c
│  │  │  └─ examples
│  │  │     └─ example-usage.c
│  │  └─ target-root-contract
│  │     ├─ .clangd
│  │     ├─ cmd
│  │     │  ├─ runner1
│  │     │  │  └─ main.c
│  │     │  └─ runner2
│  │     │     └─ main.c
│  │     ├─ include
│  │     │  └─ target-root-contract.h
│  │     ├─ internal
│  │     │  └─ hidden.c
│  │     ├─ pkg
│  │     │  └─ feature.c
│  │     ├─ plugins
│  │     │  └─ render
│  │     │     └─ plugin.c
│  │     └─ src
│  │        └─ core.c
│  └─ tests
│     ├─ fixture
│     │  ├─ deps-graph
│     │  │  ├─ A
│     │  │  │  ├─ include
│     │  │  │  │  └─ A.h
│     │  │  │  └─ src
│     │  │  │     └─ A.c
│     │  │  ├─ B
│     │  │  │  ├─ inc
│     │  │  │  │  └─ B.h
│     │  │  │  └─ src
│     │  │  │     └─ B.c
│     │  │  └─ C
│     │  │     └─ source
│     │  │        └─ main.c
│     │  ├─ lib-kind-project
│     │  │  ├─ include
│     │  │  │  └─ core.h
│     │  │  └─ src
│     │  │     └─ core.c
│     │  ├─ plain-project
│     │  │  ├─ source
│     │  │  │  └─ main.c
│     │  │  └─ tests
│     │  │     └─ test-smoke.c
│     │  └─ recursive-dsl-project
│     │     ├─ include
│     │     │  └─ fixture.h
│     │     ├─ source
│     │     │  └─ fixture.c
│     │     └─ tests
│     │        └─ test-recursive-dsl.c
│     ├─ test-dal-c.c
│     └─ run-tests.sh
├─ dh-examples
│  ├─ .clangd
│  ├─ compile_flags.txt
│  ├─ dacolor
│  │  ├─ .clangd
│  │  ├─ include
│  │  │  └─ dacolor.h
│  │  └─ src
│  │     └─ dacolor.c
│  ├─ dage
│  │  ├─ .clangd
│  │  ├─ examples
│  │  │  ├─ example-circ_phys_2d.c
│  │  │  └─ example-usage.c
│  │  ├─ include
│  │  │  ├─ dage-base.h
│  │  │  ├─ dage-context.h
│  │  │  ├─ dage-context
│  │  │  │  ├─ Runtime.h
│  │  │  │  └─ Runtime
│  │  │  │     ├─ VT100.h
│  │  │  │     └─ WSI.h
│  │  │  │     └─ WSI
│  │  │  │        └─ cfg.h
│  │  │  ├─ dage-input.h
│  │  │  ├─ dage-input
│  │  │  │  ├─ Keyboard.h
│  │  │  │  ├─ KeyBtnFlags.h
│  │  │  │  ├─ KeyCode.h
│  │  │  │  ├─ KeyMods.h
│  │  │  │  ├─ Mouse.h
│  │  │  │  └─ MouseBtn.h
│  │  │  ├─ dage-render.h
│  │  │  ├─ dage-render
│  │  │  │  ├─ base.h
│  │  │  │  ├─ Canvas.h
│  │  │  │  ├─ CanvasView.h
│  │  │  │  └─ Viewport.h
│  │  │  ├─ dage-runtime.h
│  │  │  ├─ dage-runtime
│  │  │  │  ├─ Backend.h
│  │  │  │  ├─ Event.h
│  │  │  │  ├─ Input.h
│  │  │  │  └─ Window.h
│  │  │  └─ dage.h
│  │  ├─ lib
│  │  └─ src
│  │     ├─ dage-context
│  │     │  ├─ Runtime.c
│  │     │  └─ Runtime
│  │     │     ├─ VT100.c
│  │     │     └─ WSI.c
│  │     ├─ dage-render
│  │     │  ├─ Canvas.c
│  │     │  └─ Viewport.c
│  │     └─ dage-runtime
│  │        ├─ Backend.c
│  │        ├─ Input.c
│  │        └─ Window.c
│  ├─ dansi
│  │  ├─ .clangd
│  │  ├─ include
│  │  │  ├─ dansi-core.h
│  │  │  ├─ dansi-core
│  │  │  │  ├─ attr.h
│  │  │  │  ├─ color.h
│  │  │  │  ├─ cursor.h
│  │  │  │  ├─ device.h
│  │  │  │  ├─ Event.h
│  │  │  │  ├─ line.h
│  │  │  │  ├─ mode.h
│  │  │  │  ├─ Palette4bit.h
│  │  │  │  ├─ Palette8bit.h
│  │  │  │  ├─ screen.h
│  │  │  │  ├─ scroll.h
│  │  │  │  ├─ Seq.h
│  │  │  │  ├─ style.h
│  │  │  │  ├─ title.h
│  │  │  │  └─ utils.h
│  │  │  ├─ dansi-kitty.h
│  │  │  ├─ dansi-kitty
│  │  │  ├─ dansi-sixel.h
│  │  │  ├─ dansi-sixel
│  │  │  ├─ dansi-xterm.h
│  │  │  ├─ dansi-xterm
│  │  │  │  ├─ mouse.h
│  │  │  │  └─ utils.h
│  │  │  └─ dansi.h
│  │  ├─ src
│  │  │  ├─ dansi-core
│  │  │  │  ├─ attr.c
│  │  │  │  ├─ color.c
│  │  │  │  ├─ cursor.c
│  │  │  │  ├─ device.c
│  │  │  │  ├─ Event.c
│  │  │  │  ├─ line.c
│  │  │  │  ├─ mode.c
│  │  │  │  ├─ screen.c
│  │  │  │  ├─ scroll.c
│  │  │  │  ├─ Seq.c
│  │  │  │  ├─ style.c
│  │  │  │  ├─ title.c
│  │  │  │  └─ utils.c
│  │  │  └─ dansi-xterm
│  │  │     └─ mouse.c
│  │  ├─ tests
│  │  │  ├─ test-core_control.c
│  │  │  ├─ test-core_cursor.c
│  │  │  ├─ test-core_device.c
│  │  │  ├─ test-core_event.c
│  │  │  ├─ test-core_request.c
│  │  │  ├─ test-core_screen.c
│  │  │  ├─ test-core_Seq.c
│  │  │  ├─ test-core_utils.c
│  │  │  ├─ test-support.h
│  │  │  └─ test-xterm_mouse.c
│  │  └─ README.md
│  ├─ daterm
│  │  ├─ .clangd
│  │  ├─ include
│  │  │  ├─ daterm-context.h
│  │  │  ├─ daterm-context
│  │  │  │  ├─ ANSI.h
│  │  │  │  ├─ ANSI
│  │  │  │  │  └─ private.h
│  │  │  │  └─ Virt.h
│  │  │  ├─ daterm-runtime.h
│  │  │  ├─ daterm-runtime
│  │  │  │  ├─ base.h
│  │  │  │  ├─ Event.h
│  │  │  │  └─ Term.h
│  │  │  └─ daterm.h
│  │  ├─ src
│  │  │  ├─ daterm-context
│  │  │  │  ├─ ANSI.c
│  │  │  │  └─ Virt.c
│  │  │  └─ daterm-runtime
│  │  │     └─ Term.c
│  │  ├─ examples
│  │  │  ├─ example-color.c
│  │  │  ├─ example-event.c
│  │  │  └─ example-screen.c
│  │  ├─ tests
│  │  │   ├─ test-context_ANSI.c
│  │  │   └─ test-context_Virt.c
│  │  └─ README.md
│  ├─ datext
│  ├─ dapty
│  ├─ extra
│  │  ├─ compile_flags.txt
│  │  ├─ .clangd
│  │  ├─ coding-pelican.c
│  └─ Grid
│     ├─ .clangd
│     ├─ include
│     │  └─ Grid.h
│     ├─ examples
│     │  └─ example-usage.c
│     └─ tests
│        ├─ test-fromS.c
│        ├─ test-height.c
│        ├─ test-len.c
│        ├─ test-toS.c
│        └─ test-width.c
├─ dh-extras
│  ├─ compile_flags.txt
│  ├─ .clangd
│  ├─ heap
│  │  └─ TLSF
│  │     └─ README.md
│  └─ os
│     ├─ linux
│     │  └─ x11
│     │     ├─ .clangd
│     │     └─ include
│     │        └─ x11.h
│     └─ windows
│        └─ wnd
│           ├─ .clangd
│           └─ include
│              └─ wnd.h
├─ README.md
├─ BUILD.md
├─ PROJECT_TREE.md
└─ LICENSE
```
