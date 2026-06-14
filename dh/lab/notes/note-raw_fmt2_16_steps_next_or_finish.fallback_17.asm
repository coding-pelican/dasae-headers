0000000000001c20 <fallback_17>:
    1c20:	53                   	push   rbx
    1c21:	48 81 ec 00 01 00 00 	sub    rsp,0x100
    1c28:	0f 57 c0             	xorps  xmm0,xmm0
    1c2b:	0f 29 84 24 f0 00 00 	movaps XMMWORD PTR [rsp+0xf0],xmm0
    1c32:	00 
    1c33:	0f 29 84 24 e0 00 00 	movaps XMMWORD PTR [rsp+0xe0],xmm0
    1c3a:	00 
    1c3b:	0f 29 84 24 d0 00 00 	movaps XMMWORD PTR [rsp+0xd0],xmm0
    1c42:	00 
    1c43:	0f 29 84 24 c0 00 00 	movaps XMMWORD PTR [rsp+0xc0],xmm0
    1c4a:	00 
    1c4b:	0f 29 84 24 b0 00 00 	movaps XMMWORD PTR [rsp+0xb0],xmm0
    1c52:	00 
    1c53:	0f 29 84 24 a0 00 00 	movaps XMMWORD PTR [rsp+0xa0],xmm0
    1c5a:	00 
    1c5b:	0f 29 84 24 90 00 00 	movaps XMMWORD PTR [rsp+0x90],xmm0
    1c62:	00 
    1c63:	0f 29 84 24 80 00 00 	movaps XMMWORD PTR [rsp+0x80],xmm0
    1c6a:	00 
    1c6b:	0f 29 44 24 70       	movaps XMMWORD PTR [rsp+0x70],xmm0
    1c70:	0f 29 44 24 60       	movaps XMMWORD PTR [rsp+0x60],xmm0
    1c75:	0f 29 44 24 50       	movaps XMMWORD PTR [rsp+0x50],xmm0
    1c7a:	0f 29 44 24 40       	movaps XMMWORD PTR [rsp+0x40],xmm0
    1c7f:	0f 29 44 24 30       	movaps XMMWORD PTR [rsp+0x30],xmm0
    1c84:	0f 29 44 24 20       	movaps XMMWORD PTR [rsp+0x20],xmm0
    1c89:	0f 29 44 24 10       	movaps XMMWORD PTR [rsp+0x10],xmm0
    1c8e:	0f 28 05 7b e9 ff ff 	movaps xmm0,XMMWORD PTR [rip+0xffffffffffffe97b]        # 610 <_IO_stdin_used+0x10>
    1c95:	0f 29 04 24          	movaps XMMWORD PTR [rsp],xmm0
    1c99:	4c 8d 05 bc e9 ff ff 	lea    r8,[rip+0xffffffffffffe9bc]        # 65c <_IO_stdin_used+0x5c>
    1ca0:	48 8d 35 b6 e9 ff ff 	lea    rsi,[rip+0xffffffffffffe9b6]        # 65d <_IO_stdin_used+0x5d>
    1ca7:	48 8d 0d 27 ea ff ff 	lea    rcx,[rip+0xffffffffffffea27]        # 6d5 <_IO_stdin_used+0xd5>
    1cae:	48 89 e3             	mov    rbx,rsp
    1cb1:	ba 77 00 00 00       	mov    edx,0x77
    1cb6:	48 89 df             	mov    rdi,rbx
    1cb9:	45 31 c9             	xor    r9d,r9d
    1cbc:	6a 00                	push   0x0
    1cbe:	41 50                	push   r8
    1cc0:	6a 00                	push   0x0
    1cc2:	41 50                	push   r8
    1cc4:	6a 00                	push   0x0
    1cc6:	41 50                	push   r8
    1cc8:	6a 00                	push   0x0
    1cca:	41 50                	push   r8
    1ccc:	6a 00                	push   0x0
    1cce:	41 50                	push   r8
    1cd0:	6a 00                	push   0x0
    1cd2:	41 50                	push   r8
    1cd4:	6a 00                	push   0x0
    1cd6:	41 50                	push   r8
    1cd8:	6a 00                	push   0x0
    1cda:	41 50                	push   r8
    1cdc:	6a 00                	push   0x0
    1cde:	41 50                	push   r8
    1ce0:	6a 00                	push   0x0
    1ce2:	41 50                	push   r8
    1ce4:	6a 00                	push   0x0
    1ce6:	41 50                	push   r8
    1ce8:	6a 00                	push   0x0
    1cea:	41 50                	push   r8
    1cec:	6a 00                	push   0x0
    1cee:	41 50                	push   r8
    1cf0:	6a 00                	push   0x0
    1cf2:	41 50                	push   r8
    1cf4:	e8 18 00 00 00       	call   1d11 <fmt_runtime>
    1cf9:	48 81 c4 e0 00 00 00 	add    rsp,0xe0
    1d00:	48 89 df             	mov    rdi,rbx
    1d03:	e8 18 04 00 00       	call   2120 <puts@plt>
    1d08:	48 81 c4 00 01 00 00 	add    rsp,0x100
    1d0f:	5b                   	pop    rbx
    1d10:	c3                   	ret

