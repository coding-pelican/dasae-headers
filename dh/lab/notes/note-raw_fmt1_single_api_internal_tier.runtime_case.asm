0000000000001940 <runtime_case>:
    1940:	41 56                	push   r14
    1942:	53                   	push   rbx
    1943:	48 81 ec a8 00 00 00 	sub    rsp,0xa8
    194a:	48 89 fb             	mov    rbx,rdi
    194d:	0f 57 c0             	xorps  xmm0,xmm0
    1950:	0f 29 84 24 90 00 00 	movaps XMMWORD PTR [rsp+0x90],xmm0
    1957:	00 
    1958:	0f 29 84 24 80 00 00 	movaps XMMWORD PTR [rsp+0x80],xmm0
    195f:	00 
    1960:	0f 29 44 24 70       	movaps XMMWORD PTR [rsp+0x70],xmm0
    1965:	0f 29 44 24 60       	movaps XMMWORD PTR [rsp+0x60],xmm0
    196a:	0f 29 44 24 50       	movaps XMMWORD PTR [rsp+0x50],xmm0
    196f:	0f 29 44 24 40       	movaps XMMWORD PTR [rsp+0x40],xmm0
    1974:	0f 29 44 24 30       	movaps XMMWORD PTR [rsp+0x30],xmm0
    1979:	0f 29 44 24 20       	movaps XMMWORD PTR [rsp+0x20],xmm0
    197e:	0f 29 44 24 10       	movaps XMMWORD PTR [rsp+0x10],xmm0
    1983:	0f 29 04 24          	movaps XMMWORD PTR [rsp],xmm0
    1987:	e8 94 01 00 00       	call   1b20 <strlen@plt>
    198c:	49 89 e6             	mov    r14,rsp
    198f:	4c 89 f7             	mov    rdi,r14
    1992:	48 89 de             	mov    rsi,rbx
    1995:	48 89 c2             	mov    rdx,rax
    1998:	e8 13 00 00 00       	call   19b0 <fmt_runtime>
    199d:	4c 89 f7             	mov    rdi,r14
    19a0:	e8 6b 01 00 00       	call   1b10 <puts@plt>
    19a5:	48 81 c4 a8 00 00 00 	add    rsp,0xa8
    19ac:	5b                   	pop    rbx
    19ad:	41 5e                	pop    r14
    19af:	c3                   	ret
