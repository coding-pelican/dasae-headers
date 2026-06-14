0000000000001bb0 <folded_few>:
    1bb0:	48 81 ec 88 00 00 00 	sub    rsp,0x88
    1bb7:	0f 57 c0             	xorps  xmm0,xmm0
    1bba:	0f 29 44 24 70       	movaps XMMWORD PTR [rsp+0x70],xmm0
    1bbf:	0f 29 44 24 60       	movaps XMMWORD PTR [rsp+0x60],xmm0
    1bc4:	0f 29 44 24 50       	movaps XMMWORD PTR [rsp+0x50],xmm0
    1bc9:	0f 29 44 24 40       	movaps XMMWORD PTR [rsp+0x40],xmm0
    1bce:	0f 29 44 24 30       	movaps XMMWORD PTR [rsp+0x30],xmm0
    1bd3:	0f 29 44 24 20       	movaps XMMWORD PTR [rsp+0x20],xmm0
    1bd8:	0f 29 44 24 10       	movaps XMMWORD PTR [rsp+0x10],xmm0
    1bdd:	48 b8 63 61 73 65 20 	movabs rax,0x6b6f7b2065736163
    1be4:	7b 6f 6b 
    1be7:	48 89 04 24          	mov    QWORD PTR [rsp],rax
    1beb:	66 c7 44 24 08 7d 20 	mov    WORD PTR [rsp+0x8],0x207d
    1bf2:	c7 44 24 0a 25 7b 20 	mov    DWORD PTR [rsp+0xa],0x58207b25
    1bf9:	58 
    1bfa:	66 c7 44 24 0e 20 58 	mov    WORD PTR [rsp+0xe],0x5820
    1c01:	48 89 e7             	mov    rdi,rsp
    1c04:	e8 17 05 00 00       	call   2120 <puts@plt>
    1c09:	48 81 c4 88 00 00 00 	add    rsp,0x88
    1c10:	c3                   	ret
    1c11:	cc                   	int3
    1c12:	cc                   	int3
    1c13:	cc                   	int3
    1c14:	cc                   	int3
    1c15:	cc                   	int3
    1c16:	cc                   	int3
    1c17:	cc                   	int3
    1c18:	cc                   	int3
    1c19:	cc                   	int3
    1c1a:	cc                   	int3
    1c1b:	cc                   	int3
    1c1c:	cc                   	int3
    1c1d:	cc                   	int3
    1c1e:	cc                   	int3
    1c1f:	cc                   	int3

