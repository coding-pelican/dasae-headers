/* #include "dh/prl.h"

fn_((mem_copyFwds(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    claim_assert_fmt(len$S(dst) == len$S(src), "Length mismatch: dst(%zu) != src(%zu)", len$S(dest), len$S(source));

    for_(($us(dst), $us(src))(d, s) {
        u_copy$P(d, s);
    });
    return dst;
}
#define T_use_copyFwds(_T...) \
    $inline_always $static fn_((tpl_id(mem_copyFwds, _T)(S$(_T) dst, S$(const _T) src))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_copyFwds(u_anyS(dst), u_anyS_const(src)))); \
    }

fn_((mem_copyBwds(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    claim_assert_fmt(len$S(dst) == len$S(src), "Length mismatch: dst(%zu) != src(%zu)", len$S(dest), len$S(source));

    for_($bwd($us(dst), $us(src))(d, s) {
        u_copy$P(d, s);
    });
    return dst;
}
#define T_use_copyBwds(_T...) \
    $inline_always $static fn_((tpl_id(mem_copyBwds, _T)(S$(_T) dst, S$(const _T) src))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_copyBwds(u_anyS(dst), u_anyS_const(src)))); \
    }





pub fn copyForwards(comptime T : type, dest : [] T, source : [] const T) void;
pub fn copyBackwards(comptime T : type, dest : [] T, source : [] const T) void;
pub fn zeroes(comptime T : type) T;
pub fn zeroInit(comptime T : type, init : anytype) T;
pub fn sort;
pub fn sortUnstable;
pub fn sortContext(a : usize, b : usize, context : anytype) void;
pub fn sortUnstableContext(a : usize, b : usize, context : anytype) void;
pub fn order(comptime T : type, lhs : [] const T, rhs : [] const T) math.Order;
pub fn orderZ(comptime T : type, lhs : [*:0] const T, rhs : [*:0] const T) math.Order;
pub fn lessThan(comptime T : type, lhs : [] const T, rhs : [] const T) bool;
pub fn eql(comptime T : type, a : [] const T, b : [] const T) bool;
pub fn indexOfDiff(comptime T : type, a : [] const T, b : [] const T) ? usize;
pub fn span(ptr : anytype) Span(@TypeOf(ptr));
pub fn sliceTo(ptr : anytype, comptime end : std.meta.Elem(@TypeOf(ptr))) SliceTo(@TypeOf(ptr), end);
pub fn len(value : anytype) usize;
pub fn indexOfSentinel(comptime T : type, comptime sentinel : T, p : [*:sentinel] const T) usize;
pub fn allEqual(comptime T : type, slice : [] const T, scalar : T) bool;
pub fn trimStart(comptime T : type, slice : [] const T, values_to_strip : [] const T)[] const T;
pub fn trimEnd(comptime T : type, slice : [] const T, values_to_strip : [] const T)[] const T;
pub fn trim(comptime T : type, slice : [] const T, values_to_strip : [] const T)[] const T;
pub fn indexOfScalar(comptime T : type, slice : [] const T, value : T)                                      ? usize;
pub fn lastIndexOfScalar(comptime T : type, slice : [] const T, value : T)                                  ? usize;
pub fn indexOfScalarPos(comptime T : type, slice : [] const T, start_index : usize, value : T)              ? usize;
pub fn indexOfAny(comptime T : type, slice : [] const T, values : [] const T)                               ? usize;
pub fn lastIndexOfAny(comptime T : type, slice : [] const T, values : [] const T)                           ? usize;
pub fn indexOfAnyPos(comptime T : type, slice : [] const T, start_index : usize, values : [] const T)       ? usize;
pub fn indexOfNone(comptime T : type, slice : [] const T, values : [] const T)                              ? usize;
pub fn lastIndexOfNone(comptime T : type, slice : [] const T, values : [] const T)                          ? usize;
pub fn indexOfNonePos(comptime T : type, slice : [] const T, start_index : usize, values : [] const T)      ? usize;
pub fn indexOf(comptime T : type, haystack : [] const T, needle : [] const T)                               ? usize;
pub fn lastIndexOfLinear(comptime T : type, haystack : [] const T, needle : [] const T)                     ? usize;
pub fn indexOfPosLinear(comptime T : type, haystack : [] const T, start_index : usize, needle : [] const T) ? usize;
pub fn lastIndexOf(comptime T : type, haystack : [] const T, needle : [] const T)                           ? usize;
pub fn indexOfPos(comptime T : type, haystack : [] const T, start_index : usize, needle : [] const T)       ? usize;
pub fn count(comptime T : type, haystack : [] const T, needle : [] const T) usize;
pub fn containsAtLeast(comptime T : type, haystack : [] const T, expected_count : usize, needle : [] const T) bool;
pub fn containsAtLeastScalar(comptime T : type, haystack : [] const T, expected_count : usize, needle : T) bool;
pub fn readVarInt(comptime ReturnType : type, bytes : [] const u8, endian : Endian) ReturnType;
pub fn readVarPackedInt;
pub fn readPackedInt(comptime T : type, bytes : [] const u8, bit_offset : usize, endian : Endian) T;
pub fn writePackedInt(comptime T : type, bytes : [] u8, bit_offset : usize, value : T, endian : Endian) void;
pub fn writeVarPackedInt(bytes : [] u8, bit_offset : usize, bit_count : usize, value : anytype, endian : std.builtin.Endian) void;
pub fn byteSwapAllFields(comptime S : type, ptr : *S) void;
pub fn byteSwapAllElements(comptime Elem : type, slice : [] Elem) void;
pub fn tokenizeAny(comptime T : type, buffer : [] const T, delimiters : [] const T) TokenIterator(T, .any);
pub fn tokenizeSequence(comptime T : type, buffer : [] const T, delimiter : [] const T) TokenIterator(T, .sequence);
pub fn tokenizeScalar(comptime T : type, buffer : [] const T, delimiter : T) TokenIterator(T, .scalar);
pub fn splitSequence(comptime T : type, buffer : [] const T, delimiter : [] const T) SplitIterator(T, .sequence);
pub fn splitAny(comptime T : type, buffer : [] const T, delimiters : [] const T) SplitIterator(T, .any);
pub fn splitScalar(comptime T : type, buffer : [] const T, delimiter : T) SplitIterator(T, .scalar);
pub fn splitBackwardsSequence(comptime T : type, buffer : [] const T, delimiter : [] const T) SplitBackwardsIterator(T, .sequence);
pub fn splitBackwardsAny(comptime T : type, buffer : [] const T, delimiters : [] const T) SplitBackwardsIterator(T, .any);
pub fn splitBackwardsScalar(comptime T : type, buffer : [] const T, delimiter : T) SplitBackwardsIterator(T, .scalar);
pub fn window(comptime T : type, buffer : [] const T, size : usize, advance : usize) WindowIterator(T);
pub fn WindowIterator(comptime T : type) type;
pub fn first(self : *Self)[] const T;
pub fn next(self : *Self) ? [] const T;
pub fn reset(self : *Self) void;
pub fn startsWith(comptime T : type, haystack : [] const T, needle : [] const T) bool;
pub fn endsWith(comptime T : type, haystack : [] const T, needle : [] const T) bool;
pub fn TokenIterator(comptime T : type, comptime delimiter_type : DelimiterType) type;
pub fn next(self : *Self) ? [] const T;
pub fn peek(self : *Self) ? [] const T;
pub fn rest(self : Self)[] const T;
pub fn reset(self : *Self) void;
pub fn SplitIterator(comptime T : type, comptime delimiter_type : DelimiterType) type;
pub fn first(self : *Self)[] const T;
pub fn next(self : *Self) ? [] const T;
pub fn peek(self : *Self) ? [] const T;
pub fn rest(self : Self)[] const T;
pub fn reset(self : *Self) void;
pub fn SplitBackwardsIterator(comptime T : type, comptime delimiter_type : DelimiterType) type;
pub fn first(self : *Self)[] const T;
pub fn next(self : *Self) ? [] const T;
pub fn rest(self : Self)[] const T;
pub fn reset(self : *Self) void;
pub fn join(allocator : Allocator, separator : [] const u8, slices : [] const[] const u8) Allocator.Error ![] u8;
pub fn joinZ(allocator : Allocator, separator : [] const u8, slices : [] const[] const u8) Allocator.Error ![:0] u8;
pub fn concat(allocator : Allocator, comptime T : type, slices : [] const[] const T) Allocator.Error ![] T;
pub fn concatWithSentinel(allocator : Allocator, comptime T : type, slices : [] const[] const T, comptime s : T) Allocator.Error ![:s] T;
        pub fn concatMaybeSentinel(allocator: Allocator, comptime T: type, slices: []const []const T, comptime s: ?T) Allocator.Error![]T ;
        pub fn min(comptime T : type, slice : [] const T) T;
        pub fn max(comptime T : type, slice : [] const T) T;
        pub fn minMax(comptime T : type, slice : [] const T) struct {
            T, T
        };
        pub fn indexOfMin(comptime T : type, slice : [] const T) usize;
        pub fn indexOfMax(comptime T : type, slice : [] const T) usize;
        pub fn indexOfMinMax(comptime T : type, slice : [] const T) struct {
            usize, usize
        };
        pub fn swap(comptime T : type, a : *T, b : *T) void;
        pub fn reverse(comptime T : type, items : [] T) void;
        pub fn next(self : *@This())    ? Element;
        pub fn nextPtr(self : *@This()) ? ElementPointer;
        pub fn reverseIterator(slice : anytype) ReverseIterator(@TypeOf(slice));
        pub fn rotate(comptime T : type, items : [] T, amount : usize) void;
        pub fn replace(comptime T : type, input : [] const T, needle : [] const T, replacement : [] const T, output : [] T) usize;
        pub fn replaceScalar(comptime T : type, slice : [] T, match : T, replacement : T) void;
        pub fn collapseRepeatsLen(comptime T : type, slice : [] T, elem : T) usize;
        pub fn collapseRepeats(comptime T : type, slice : [] T, elem : T)[] T;
        pub fn replacementSize(comptime T : type, input : [] const T, needle : [] const T, replacement : [] const T) usize;
        pub fn replaceOwned(comptime T : type, allocator : Allocator, input : [] const T, needle : [] const T, replacement : [] const T) Allocator.Error ![] T;
        pub fn littleToNative(comptime T : type, x : T) T;
        pub fn bigToNative(comptime T : type, x : T) T;
        pub fn toNative(comptime T : type, x : T, endianness_of_x : Endian) T;
        pub fn nativeTo(comptime T : type, x : T, desired_endianness : Endian) T;
        pub fn nativeToLittle(comptime T : type, x : T) T;
        pub fn nativeToBig(comptime T : type, x : T) T;
        pub fn alignPointerOffset(ptr : anytype, align_to : usize) ? usize;
        pub fn alignPointer(ptr : anytype, align_to : usize)       ? @TypeOf(ptr);
        pub fn asBytes(ptr : anytype) AsBytesReturnType(@TypeOf(ptr));
        pub fn toBytes(value : anytype)[@sizeOf(@TypeOf(value))] u8;
        pub fn bytesAsValue(comptime T : type, bytes : anytype) BytesAsValueReturnType(T, @TypeOf(bytes));
        pub fn bytesToValue(comptime T : type, bytes : anytype) T;
        pub fn bytesAsSlice(comptime T : type, bytes : anytype) BytesAsSliceReturnType(T, @TypeOf(bytes));
        pub fn sliceAsBytes(slice : anytype) SliceAsBytesReturnType(@TypeOf(slice));
        pub fn alignForwardAnyAlign(comptime T : type, addr : T, alignment : T) T;
        pub fn alignForward(comptime T : type, addr : T, alignment : T) T;
        pub fn alignForwardLog2(addr : usize, log2_alignment : u8) usize;
        pub fn doNotOptimizeAway(val : anytype) void;
        pub fn alignBackwardAnyAlign(comptime T : type, addr : T, alignment : T) T;
        pub fn alignBackward(comptime T : type, addr : T, alignment : T) T;
        pub fn isValidAlign(alignment : usize) bool;
        pub fn isValidAlignGeneric(comptime T : type, alignment : T) bool;
        pub fn isAlignedAnyAlign(i : usize, alignment : usize) bool;
        pub fn isAlignedLog2(addr : usize, log2_alignment : u8) bool;
        pub fn isAligned(addr : usize, alignment : usize) bool;
        pub fn isAlignedGeneric(comptime T : type, addr : T, alignment : T) bool;
        pub fn alignInBytes(bytes : [] u8, comptime new_alignment : usize)   ? [] align(new_alignment) u8;
        pub fn alignInSlice(slice : anytype, comptime new_alignment : usize) ? AlignedSlice(@TypeOf(slice), new_alignment);
 */
