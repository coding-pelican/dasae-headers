0000000000001a70 <folded_16>:
    1a70:	48 81 ec 08 01 00 00 	sub    rsp,0x108
    1a77:	0f 57 c0             	xorps  xmm0,xmm0
    1a7a:	0f 29 84 24 f0 00 00 	movaps XMMWORD PTR [rsp+0xf0],xmm0
    1a81:	00 
    1a82:	0f 29 84 24 e0 00 00 	movaps XMMWORD PTR [rsp+0xe0],xmm0
    1a89:	00 
    1a8a:	0f 29 84 24 d0 00 00 	movaps XMMWORD PTR [rsp+0xd0],xmm0
    1a91:	00 
    1a92:	0f 29 84 24 c0 00 00 	movaps XMMWORD PTR [rsp+0xc0],xmm0
    1a99:	00 
    1a9a:	0f 29 84 24 b0 00 00 	movaps XMMWORD PTR [rsp+0xb0],xmm0
    1aa1:	00 
    1aa2:	0f 29 84 24 a0 00 00 	movaps XMMWORD PTR [rsp+0xa0],xmm0
    1aa9:	00 
    1aaa:	0f 29 84 24 90 00 00 	movaps XMMWORD PTR [rsp+0x90],xmm0
    1ab1:	00 
    1ab2:	0f 29 84 24 80 00 00 	movaps XMMWORD PTR [rsp+0x80],xmm0
    1ab9:	00 
    1aba:	0f 29 44 24 70       	movaps XMMWORD PTR [rsp+0x70],xmm0
    1abf:	0f 29 44 24 60       	movaps XMMWORD PTR [rsp+0x60],xmm0
    1ac4:	0f 29 44 24 50       	movaps XMMWORD PTR [rsp+0x50],xmm0
    1ac9:	0f 29 44 24 40       	movaps XMMWORD PTR [rsp+0x40],xmm0
    1ace:	0f 29 44 24 30       	movaps XMMWORD PTR [rsp+0x30],xmm0
    1ad3:	66 c7 04 24 61 30    	mov    WORD PTR [rsp],0x3061
    1ad9:	c6 44 24 02 20       	mov    BYTE PTR [rsp+0x2],0x20
    1ade:	66 c7 44 24 03 61 31 	mov    WORD PTR [rsp+0x3],0x3161
    1ae5:	c6 44 24 05 20       	mov    BYTE PTR [rsp+0x5],0x20
    1aea:	66 c7 44 24 06 61 32 	mov    WORD PTR [rsp+0x6],0x3261
    1af1:	c6 44 24 08 20       	mov    BYTE PTR [rsp+0x8],0x20
    1af6:	66 c7 44 24 09 61 33 	mov    WORD PTR [rsp+0x9],0x3361
    1afd:	c6 44 24 0b 20       	mov    BYTE PTR [rsp+0xb],0x20
    1b02:	66 c7 44 24 0c 61 34 	mov    WORD PTR [rsp+0xc],0x3461
    1b09:	c6 44 24 0e 20       	mov    BYTE PTR [rsp+0xe],0x20
    1b0e:	66 c7 44 24 0f 61 35 	mov    WORD PTR [rsp+0xf],0x3561
    1b15:	c6 44 24 11 20       	mov    BYTE PTR [rsp+0x11],0x20
    1b1a:	66 c7 44 24 12 61 36 	mov    WORD PTR [rsp+0x12],0x3661
    1b21:	c6 44 24 14 20       	mov    BYTE PTR [rsp+0x14],0x20
    1b26:	66 c7 44 24 15 61 37 	mov    WORD PTR [rsp+0x15],0x3761
    1b2d:	c6 44 24 17 20       	mov    BYTE PTR [rsp+0x17],0x20
    1b32:	66 c7 44 24 18 61 38 	mov    WORD PTR [rsp+0x18],0x3861
    1b39:	c6 44 24 1a 20       	mov    BYTE PTR [rsp+0x1a],0x20
    1b3e:	66 c7 44 24 1b 61 39 	mov    WORD PTR [rsp+0x1b],0x3961
    1b45:	c6 44 24 1d 20       	mov    BYTE PTR [rsp+0x1d],0x20
    1b4a:	66 c7 44 24 1e 61 41 	mov    WORD PTR [rsp+0x1e],0x4161
    1b51:	c6 44 24 20 20       	mov    BYTE PTR [rsp+0x20],0x20
    1b56:	66 c7 44 24 21 61 42 	mov    WORD PTR [rsp+0x21],0x4261
    1b5d:	c6 44 24 23 20       	mov    BYTE PTR [rsp+0x23],0x20
    1b62:	66 c7 44 24 24 61 43 	mov    WORD PTR [rsp+0x24],0x4361
    1b69:	c6 44 24 26 20       	mov    BYTE PTR [rsp+0x26],0x20
    1b6e:	66 c7 44 24 27 61 44 	mov    WORD PTR [rsp+0x27],0x4461
    1b75:	c6 44 24 29 20       	mov    BYTE PTR [rsp+0x29],0x20
    1b7a:	66 c7 44 24 2a 61 45 	mov    WORD PTR [rsp+0x2a],0x4561
    1b81:	c6 44 24 2c 20       	mov    BYTE PTR [rsp+0x2c],0x20
    1b86:	66 c7 44 24 2d 61 46 	mov    WORD PTR [rsp+0x2d],0x4661
    1b8d:	c6 44 24 2f 00       	mov    BYTE PTR [rsp+0x2f],0x0
    1b92:	48 89 e7             	mov    rdi,rsp
    1b95:	e8 86 05 00 00       	call   2120 <puts@plt>
    1b9a:	48 81 c4 08 01 00 00 	add    rsp,0x108
    1ba1:	c3                   	ret
    1ba2:	cc                   	int3
    1ba3:	cc                   	int3
    1ba4:	cc                   	int3
    1ba5:	cc                   	int3
    1ba6:	cc                   	int3
    1ba7:	cc                   	int3
    1ba8:	cc                   	int3
    1ba9:	cc                   	int3
    1baa:	cc                   	int3
    1bab:	cc                   	int3
    1bac:	cc                   	int3
    1bad:	cc                   	int3
    1bae:	cc                   	int3
    1baf:	cc                   	int3

