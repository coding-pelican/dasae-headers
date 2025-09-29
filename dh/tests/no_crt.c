#ifdef SI_IMPLEMENTATION_MEMORY

inline
bool si_isPowerOfTwo(isize x) {
	return !(x & (x - 1)) && x > 0;
}

inline
isize si_alignForward(isize num, isize alignment) {
	return (isize)si_alignForwardU((usize)num, alignment);
}
inline
usize si_alignForwardU(usize num, isize alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));
	usize align = (usize)alignment;
	return (num + align - 1) & ~(align - 1);
}

SIDEF
isize si_nextPow2(isize num) {
	SI_ASSERT_NOT_NEG(num);

	num -= 1;
	num |= num >> 1;
	num |= num >> 2;
	num |= num >> 4;
	num |= num >> 8;
	num |= num >> 16;
	#if SI_ARCH_IS_64BIT
	num |= num >> 32;
	#endif

	return num + 1;
}


inline void* si_pointerAdd(void* ptr, isize bytes)                  { return (void*)((u8*)ptr + bytes); }
inline void* si_pointerSub(void* ptr, isize bytes)                  { return (void*)((u8*)ptr - bytes); }
inline const void* si_pointerAddConst(const void* ptr, isize bytes) { return (void*)((const u8*)ptr + bytes); }
inline const void* si_pointerSubConst(const void* ptr, isize bytes) { return (void*)((const u8*)ptr - bytes); }
inline isize si_pointerDiff(const void* begin, const void* end)     { return (isize)((const u8*)end - (const u8*)begin); }
inline bool si_pointerBetween(const void* ptr, const void* start, const void* end) { return (ptr >= start) && (ptr <= end); }


SIDEF
isize si_memcopy(void* restrict dst, const void* restrict src, isize size) {
	SI_ASSERT_NOT_NIL(dst);
	SI_ASSERT_NOT_NIL(src);
	SI_ASSERT_NOT_NEG(size);

#ifndef SI_NO_CRT
	memcpy(dst, src, (usize)size);
	return size;
#else
	u8* dest = (u8*)dst;
	const u8* source = (const u8*)src;

	for_range (i, 0, size) {
		dest[i] = source[i];
	}

	return size;
#endif
}

SIDEF
isize si_memmove(void* restrict dst, const void* restrict src, isize size) {
	SI_ASSERT_NOT_NIL(dst);
	SI_ASSERT_NOT_NIL(src);
	SI_ASSERT_NOT_NEG(size);

#ifndef SI_NO_CRT
	memmove(dst, src, (usize)size);
	return size;
#else
	u8* dest = (u8*)dst;
	const u8* source = (const u8*)src;

	if (dest < source || (dest >= source + size)) {
		for_range (i, 0, size) {
		   	dest[i] = source[i];
		}
	}
	else {
		isize i;
		for (i = size - 1; i >= 0; i--) {
			dest[i] = source[i];
		}
	}

	return size;
#endif
}

SIDEF
isize si_memset(void* data, u8 value, isize size) {
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(size);

#ifndef SI_NO_CRT
	memset(data, value, (usize)size);
	return size;
#else
	u8* ptr = (u8*)data;

	for_range (i, 0, size) {
		ptr[i] = value;
	}

	return size;
#endif

}

SIDEF
i32 si_memcompare(const void* ptr1, const void* ptr2, isize size) {
	SI_ASSERT_NOT_NIL(ptr1);
	SI_ASSERT_NOT_NIL(ptr2);
	SI_ASSERT_NOT_NEG(size);

#ifndef SI_NO_CRT
	return memcmp(ptr1, ptr2, (usize)size);
#else
	const u8* left = (const u8*)ptr1;
	const u8* right = (const u8*)ptr2;

	for_range (i, 0, size) {
		if (left[i] != right[i]) {
			return (left[i] - right[i]);
		}
	}

	return 0;
#endif
}

SIDEF
void* si_memchr(const void* data, u8 value, isize size) {
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(size);

#ifndef SI_NO_CRT
	return (void*)memchr(data, value, (usize)size);
#else
	const u8* ptr = (const u8*)data;
	for_range (i, 0, size) {
		if (ptr[i] == value) {
			return (void*)&ptr[i];
		}
	}

	return nil;
#endif
}


SIDEF
isize si_cstrLen(cstring str) {
	SI_ASSERT_NOT_NIL(str);

#ifndef SI_NO_CRT
	return (isize)strlen(str);
#else
	isize i = 0;
	while (str[i] != '\0') { i += 1; }

	return i;
#endif

}

inline
isize si_memmoveLeft(void* src, isize length, isize moveBy) {
	return si_memmove((u8*)src - moveBy, src, length);
}
inline
isize si_memmoveRight(void* src, isize length, isize moveBy) {
	return si_memmove((u8*)src + moveBy, src, length);
}

inline void* si_memcopy_ptr(void* restrict dst, const void* restrict src, isize size) { si_memcopy(dst, src, size); return dst; }
inline void* si_memmove_ptr(void* restrict dst, const void* restrict src, isize size) { si_memmove(dst, src, size); return dst; }
inline void* si_memset_ptr(void* data, u8 value, isize size)  { si_memset(data, value, size); return data; }
