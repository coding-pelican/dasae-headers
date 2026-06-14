0000000000001ff0 <dynamic_case>:
    1ff0:	41 56                	push   r14
    1ff2:	53                   	push   rbx
    1ff3:	48 81 ec 88 00 00 00 	sub    rsp,0x88
    1ffa:	49 89 fe             	mov    r14,rdi
    1ffd:	0f 57 c0             	xorps  xmm0,xmm0
    2000:	0f 29 44 24 70       	movaps XMMWORD PTR [rsp+0x70],xmm0
    2005:	0f 29 44 24 60       	movaps XMMWORD PTR [rsp+0x60],xmm0
    200a:	0f 29 44 24 50       	movaps XMMWORD PTR [rsp+0x50],xmm0
    200f:	0f 29 44 24 40       	movaps XMMWORD PTR [rsp+0x40],xmm0
    2014:	0f 29 44 24 30       	movaps XMMWORD PTR [rsp+0x30],xmm0
    2019:	0f 29 44 24 20       	movaps XMMWORD PTR [rsp+0x20],xmm0
    201e:	0f 29 44 24 10       	movaps XMMWORD PTR [rsp+0x10],xmm0
    2023:	0f 29 04 24          	movaps XMMWORD PTR [rsp],xmm0
    2027:	e8 04 01 00 00       	call   2130 <strlen@plt>
    202c:	4c 8d 05 29 e6 ff ff 	lea    r8,[rip+0xffffffffffffe629]        # 65c <_IO_stdin_used+0x5c>
    2033:	48 8d 0d 9d e6 ff ff 	lea    rcx,[rip+0xffffffffffffe69d]        # 6d7 <_IO_stdin_used+0xd7>
    203a:	48 89 e3             	mov    rbx,rsp
    203d:	48 89 df             	mov    rdi,rbx
    2040:	4c 89 f6             	mov    rsi,r14
    2043:	48 89 c2             	mov    rdx,rax
    2046:	45 31 c9             	xor    r9d,r9d
    2049:	6a 00                	push   0x0
    204b:	41 50                	push   r8
    204d:	6a 00                	push   0x0
    204f:	41 50                	push   r8
    2051:	6a 00                	push   0x0
    2053:	41 50                	push   r8
    2055:	6a 00                	push   0x0
    2057:	41 50                	push   r8
    2059:	6a 00                	push   0x0
    205b:	41 50                	push   r8
    205d:	6a 00                	push   0x0
    205f:	41 50                	push   r8
    2061:	6a 00                	push   0x0
    2063:	41 50                	push   r8
    2065:	6a 00                	push   0x0
    2067:	41 50                	push   r8
    2069:	6a 00                	push   0x0
    206b:	41 50                	push   r8
    206d:	6a 00                	push   0x0
    206f:	41 50                	push   r8
    2071:	6a 00                	push   0x0
    2073:	41 50                	push   r8
    2075:	6a 00                	push   0x0
    2077:	41 50                	push   r8
    2079:	6a 00                	push   0x0
    207b:	41 50                	push   r8
    207d:	6a 00                	push   0x0
    207f:	41 50                	push   r8
    2081:	e8 8b fc ff ff       	call   1d11 <fmt_runtime>
    2086:	48 81 c4 e0 00 00 00 	add    rsp,0xe0
    208d:	48 89 df             	mov    rdi,rbx
    2090:	e8 8b 00 00 00       	call   2120 <puts@plt>
    2095:	48 81 c4 88 00 00 00 	add    rsp,0x88
    209c:	5b                   	pop    rbx
    209d:	41 5e                	pop    r14
    209f:	c3                   	ret

