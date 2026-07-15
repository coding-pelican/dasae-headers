/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    dh.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2026-07-06 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 */
#pragma once
#ifndef dh__included
#define dh__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin.h" /* no prefix, abs path, pch bundle target */
#include "dh/core.h" /* no prefix, abs path, pch bundle target */
#include "dh/prl.h" /* no prefix, abs path, pch bundle target */

#include "dh/atom.h" /* pch bundle target */
#include "dh/simd.h" /* pch bundle target */
#include "dh/clsr.h" /* pch bundle target */

#include "dh/cmp.h" /* pch bundle target */
#include "dh/m-math.h" /* pch bundle target */
#include "dh/m-math-linalg.h"
#include "dh/m-math-geom.h"
#include "dh/m-math-interp.h"
#include "dh/m-math-ease.h"
#include "dh/mem.h" /* pch bundle target */
#include "dh/u-meta.h" /* pch bundle target */

#include "dh/Rand.h"
#include "dh/hash.h"
#include "dh/crypt.h"
#include "dh/cmpr.h"

#include "dh/search.h"
#include "dh/sort.h"
#include "dh/ascii.h"
#include "dh/utf8.h"
#include "dh/utf16.h"
#include "dh/wtf8.h"
#include "dh/wtf16.h"
#include "dh/unicode.h"

#include "dh/ListSgl.h"
#include "dh/ListDbl.h"
#include "dh/BTree.h"
#include "dh/SegTree.h"

#include "dh/ArrList.h"
#include "dh/ArrStk.h"
#include "dh/ArrDeq.h"
#include "dh/ArrQue.h"
#include "dh/ArrPDeq.h"
#include "dh/ArrPQue.h"

#include "dh/HashMap.h"
#include "dh/HashSet.h"
#include "dh/HashMapSeq.h"
#include "dh/HashSetSeq.h"
#include "dh/BTreeMap.h"
#include "dh/BTreeSet.h"

#include "dh/sys.h" /* abs path, pch bundle target */
#include "dh/c.h" /* abs path, pch bundle target */

#include "dh/fs.h"
#include "dh/time.h"
#include "dh/heap.h"
#include "dh/proc.h"
#include "dh/thrd.h"
#include "dh/co.h"

#include "dh/Future.h"
#include "dh/Sched.h"
#include "dh/exec.h"
#include "dh/conc.h"
#include "dh/io.h"

#include "dh/fmt.h"
#include "dh/regex.h"

#include "dh/log.h"
#include "dh/csv.h"
#include "dh/ini.h"
#include "dh/yaml.h"
#include "dh/toml.h"
#include "dh/json.h"

#include "dh/base32.h"
#include "dh/base64.h"

#include "dh/zip.h"
#include "dh/rar.h"
#include "dh/tar.h"

#include "dh/db.h"
#include "dh/net.h"
#include "dh/http.h"
#include "dh/ws.h"

#include "dh/TEST.h"
#include "dh/start.h"
/* main must be included directly. */
// #include "dh-TEST-main.h" /* no prefix, abs path */
// #include "dh-main.h" /* no prefix, abs path */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dh__included */
