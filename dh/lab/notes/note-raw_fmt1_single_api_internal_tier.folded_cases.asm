00000000000017f0 <folded_cases>:
    17f0:	48 81 ec 28 01 00 00 	sub    rsp,0x128
    17f7:	0f 57 c0             	xorps  xmm0,xmm0
    17fa:	0f 11 44 24 18       	movups XMMWORD PTR [rsp+0x18],xmm0
    17ff:	0f 11 44 24 28       	movups XMMWORD PTR [rsp+0x28],xmm0
    1804:	0f 11 44 24 38       	movups XMMWORD PTR [rsp+0x38],xmm0
    1809:	0f 11 44 24 48       	movups XMMWORD PTR [rsp+0x48],xmm0
    180e:	48 c7 44 24 58 00 00 	mov    QWORD PTR [rsp+0x58],0x0
    1815:	00 00 
    1817:	48 b8 63 61 73 65 30 	movabs rax,0x6f7b203065736163
    181e:	20 7b 6f 
    1821:	48 89 04 24          	mov    QWORD PTR [rsp],rax
    1825:	c7 44 24 07 6f 6b 7d 	mov    DWORD PTR [rsp+0x7],0x207d6b6f
    182c:	20 
    182d:	66 c7 44 24 0b 25 7b 	mov    WORD PTR [rsp+0xb],0x7b25
    1834:	c6 44 24 0d 20       	mov    BYTE PTR [rsp+0xd],0x20
    1839:	c7 44 24 0e 77 6f 72 	mov    DWORD PTR [rsp+0xe],0x6c726f77
    1840:	6c 
    1841:	c6 44 24 12 64       	mov    BYTE PTR [rsp+0x12],0x64
    1846:	c7 44 24 13 20 31 32 	mov    DWORD PTR [rsp+0x13],0x33323120
    184d:	33 
    184e:	c6 44 24 17 00       	mov    BYTE PTR [rsp+0x17],0x0
    1853:	c7 44 24 60 63 61 73 	mov    DWORD PTR [rsp+0x60],0x65736163
    185a:	65 
    185b:	66 c7 44 24 64 31 20 	mov    WORD PTR [rsp+0x64],0x2031
    1862:	0f 11 84 24 b0 00 00 	movups XMMWORD PTR [rsp+0xb0],xmm0
    1869:	00 
    186a:	0f 11 84 24 a3 00 00 	movups XMMWORD PTR [rsp+0xa3],xmm0
    1871:	00 
    1872:	0f 11 84 24 93 00 00 	movups XMMWORD PTR [rsp+0x93],xmm0
    1879:	00 
    187a:	0f 11 84 24 83 00 00 	movups XMMWORD PTR [rsp+0x83],xmm0
    1881:	00 
    1882:	0f 11 44 24 73       	movups XMMWORD PTR [rsp+0x73],xmm0
    1887:	c6 44 24 68 43       	mov    BYTE PTR [rsp+0x68],0x43
    188c:	66 c7 44 24 66 41 42 	mov    WORD PTR [rsp+0x66],0x4241
    1893:	48 b8 20 6a 6b 6c 2d 	movabs rax,0x6e6d352d6c6b6a20
    189a:	35 6d 6e 
    189d:	48 89 44 24 69       	mov    QWORD PTR [rsp+0x69],rax
    18a2:	66 c7 44 24 71 6f 00 	mov    WORD PTR [rsp+0x71],0x6f
    18a9:	0f 11 84 24 d6 00 00 	movups XMMWORD PTR [rsp+0xd6],xmm0
    18b0:	00 
    18b1:	0f 11 84 24 e6 00 00 	movups XMMWORD PTR [rsp+0xe6],xmm0
    18b8:	00 
    18b9:	0f 11 84 24 f6 00 00 	movups XMMWORD PTR [rsp+0xf6],xmm0
    18c0:	00 
    18c1:	0f 11 84 24 06 01 00 	movups XMMWORD PTR [rsp+0x106],xmm0
    18c8:	00 
    18c9:	0f 11 84 24 10 01 00 	movups XMMWORD PTR [rsp+0x110],xmm0
    18d0:	00 
    18d1:	48 b8 32 20 25 7b 70 	movabs rax,0x7d6572707b252032
    18d8:	72 65 7d 
    18db:	48 89 84 24 c4 00 00 	mov    QWORD PTR [rsp+0xc4],rax
    18e2:	00 
    18e3:	c7 84 24 c0 00 00 00 	mov    DWORD PTR [rsp+0xc0],0x65736163
    18ea:	63 61 73 65 
    18ee:	c6 84 24 cc 00 00 00 	mov    BYTE PTR [rsp+0xcc],0x20
    18f5:	20 
    18f6:	48 b8 58 20 25 7b 70 	movabs rax,0x74736f707b252058
    18fd:	6f 73 74 
    1900:	48 89 84 24 cd 00 00 	mov    QWORD PTR [rsp+0xcd],rax
    1907:	00 
    1908:	c6 84 24 d5 00 00 00 	mov    BYTE PTR [rsp+0xd5],0x7d
    190f:	7d 
    1910:	48 89 e7             	mov    rdi,rsp
    1913:	e8 f8 01 00 00       	call   1b10 <puts@plt>
    1918:	48 8d 7c 24 60       	lea    rdi,[rsp+0x60]
    191d:	e8 ee 01 00 00       	call   1b10 <puts@plt>
    1922:	48 8d bc 24 c0 00 00 	lea    rdi,[rsp+0xc0]
    1929:	00 
    192a:	e8 e1 01 00 00       	call   1b10 <puts@plt>
    192f:	48 81 c4 28 01 00 00 	add    rsp,0x128
    1936:	c3                   	ret
    1937:	cc                   	int3
    1938:	cc                   	int3
    1939:	cc                   	int3
    193a:	cc                   	int3
    193b:	cc                   	int3
    193c:	cc                   	int3
    193d:	cc                   	int3
    193e:	cc                   	int3
    193f:	cc                   	int3
