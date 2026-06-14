0000000000001770 <main>:
    1770:	53                   	push   rbx
    1771:	48 81 ec 80 02 00 00 	sub    rsp,0x280
    1778:	0f 57 c0             	xorps  xmm0,xmm0
    177b:	0f 11 84 24 16 02 00 	movups XMMWORD PTR [rsp+0x216],xmm0
    1782:	00 
    1783:	0f 11 84 24 26 02 00 	movups XMMWORD PTR [rsp+0x226],xmm0
    178a:	00 
    178b:	0f 11 84 24 36 02 00 	movups XMMWORD PTR [rsp+0x236],xmm0
    1792:	00 
    1793:	0f 11 84 24 46 02 00 	movups XMMWORD PTR [rsp+0x246],xmm0
    179a:	00 
    179b:	0f 11 84 24 56 02 00 	movups XMMWORD PTR [rsp+0x256],xmm0
    17a2:	00 
    17a3:	0f 11 84 24 66 02 00 	movups XMMWORD PTR [rsp+0x266],xmm0
    17aa:	00 
    17ab:	0f 11 84 24 70 02 00 	movups XMMWORD PTR [rsp+0x270],xmm0
    17b2:	00 
    17b3:	0f 10 0d 08 ee ff ff 	movups xmm1,XMMWORD PTR [rip+0xffffffffffffee08]        # 5c2 <_IO_stdin_used+0x52>
    17ba:	0f 29 8c 24 00 02 00 	movaps XMMWORD PTR [rsp+0x200],xmm1
    17c1:	00 
    17c2:	48 b8 6c 20 6f 6b 20 	movabs rax,0x636261206b6f206c
    17c9:	61 62 63 
    17cc:	48 89 84 24 0d 02 00 	mov    QWORD PTR [rsp+0x20d],rax
    17d3:	00 
    17d4:	c6 84 24 15 02 00 00 	mov    BYTE PTR [rsp+0x215],0x0
    17db:	00 
    17dc:	c7 84 24 80 00 00 00 	mov    DWORD PTR [rsp+0x80],0x65736163
    17e3:	63 61 73 65 
    17e7:	c7 84 24 83 00 00 00 	mov    DWORD PTR [rsp+0x83],0x203a3165
    17ee:	65 31 3a 20 
    17f2:	66 c7 84 24 87 00 00 	mov    WORD PTR [rsp+0x87],0x4241
    17f9:	00 41 42 
    17fc:	c6 84 24 89 00 00 00 	mov    BYTE PTR [rsp+0x89],0x43
    1803:	43 
    1804:	0f 11 84 24 ee 00 00 	movups XMMWORD PTR [rsp+0xee],xmm0
    180b:	00 
    180c:	0f 11 84 24 de 00 00 	movups XMMWORD PTR [rsp+0xde],xmm0
    1813:	00 
    1814:	0f 11 84 24 ce 00 00 	movups XMMWORD PTR [rsp+0xce],xmm0
    181b:	00 
    181c:	0f 11 84 24 be 00 00 	movups XMMWORD PTR [rsp+0xbe],xmm0
    1823:	00 
    1824:	0f 11 84 24 ae 00 00 	movups XMMWORD PTR [rsp+0xae],xmm0
    182b:	00 
    182c:	0f 11 84 24 9e 00 00 	movups XMMWORD PTR [rsp+0x9e],xmm0
    1833:	00 
    1834:	0f 11 84 24 8e 00 00 	movups XMMWORD PTR [rsp+0x8e],xmm0
    183b:	00 
    183c:	66 c7 84 24 fe 00 00 	mov    WORD PTR [rsp+0xfe],0x0
    1843:	00 00 00 
    1846:	c6 84 24 8c 00 00 00 	mov    BYTE PTR [rsp+0x8c],0x6b
    184d:	6b 
    184e:	66 c7 84 24 8a 00 00 	mov    WORD PTR [rsp+0x8a],0x6f20
    1855:	00 20 6f 
    1858:	c6 84 24 8d 00 00 00 	mov    BYTE PTR [rsp+0x8d],0x0
    185f:	00 
    1860:	0f 11 84 24 e8 01 00 	movups XMMWORD PTR [rsp+0x1e8],xmm0
    1867:	00 
    1868:	0f 11 84 24 d8 01 00 	movups XMMWORD PTR [rsp+0x1d8],xmm0
    186f:	00 
    1870:	0f 11 84 24 c8 01 00 	movups XMMWORD PTR [rsp+0x1c8],xmm0
    1877:	00 
    1878:	0f 11 84 24 b8 01 00 	movups XMMWORD PTR [rsp+0x1b8],xmm0
    187f:	00 
    1880:	0f 11 84 24 a8 01 00 	movups XMMWORD PTR [rsp+0x1a8],xmm0
    1887:	00 
    1888:	0f 11 84 24 98 01 00 	movups XMMWORD PTR [rsp+0x198],xmm0
    188f:	00 
    1890:	48 c7 84 24 f8 01 00 	mov    QWORD PTR [rsp+0x1f8],0x0
    1897:	00 00 00 00 00 
    189c:	48 b8 6c 69 74 65 72 	movabs rax,0x206c61726574696c
    18a3:	61 6c 20 
    18a6:	48 89 84 24 87 01 00 	mov    QWORD PTR [rsp+0x187],rax
    18ad:	00 
    18ae:	48 b8 63 61 73 65 32 	movabs rax,0x6c203a3265736163
    18b5:	3a 20 6c 
    18b8:	48 89 84 24 80 01 00 	mov    QWORD PTR [rsp+0x180],rax
    18bf:	00 
    18c0:	66 c7 84 24 8f 01 00 	mov    WORD PTR [rsp+0x18f],0x7b25
    18c7:	00 25 7b 
    18ca:	66 c7 84 24 95 01 00 	mov    WORD PTR [rsp+0x195],0x6e65
    18d1:	00 65 6e 
    18d4:	c7 84 24 91 01 00 00 	mov    DWORD PTR [rsp+0x191],0x6b6f7420
    18db:	20 74 6f 6b 
    18df:	c6 84 24 97 01 00 00 	mov    BYTE PTR [rsp+0x197],0x0
    18e6:	00 
    18e7:	0f 11 44 24 6a       	movups XMMWORD PTR [rsp+0x6a],xmm0
    18ec:	0f 11 44 24 5a       	movups XMMWORD PTR [rsp+0x5a],xmm0
    18f1:	0f 11 44 24 4a       	movups XMMWORD PTR [rsp+0x4a],xmm0
    18f6:	0f 11 44 24 3a       	movups XMMWORD PTR [rsp+0x3a],xmm0
    18fb:	0f 11 44 24 2a       	movups XMMWORD PTR [rsp+0x2a],xmm0
    1900:	0f 11 44 24 1a       	movups XMMWORD PTR [rsp+0x1a],xmm0
    1905:	48 c7 44 24 78 00 00 	mov    QWORD PTR [rsp+0x78],0x0
    190c:	00 00 
    190e:	c7 44 24 03 65 33 3a 	mov    DWORD PTR [rsp+0x3],0x203a3365
    1915:	20 
    1916:	c7 04 24 63 61 73 65 	mov    DWORD PTR [rsp],0x65736163
    191d:	66 c7 44 24 07 25 7b 	mov    WORD PTR [rsp+0x7],0x7b25
    1924:	c6 44 24 0d 20       	mov    BYTE PTR [rsp+0xd],0x20
    1929:	c7 44 24 09 70 72 65 	mov    DWORD PTR [rsp+0x9],0x7d657270
    1930:	7d 
    1931:	c6 44 24 10 43       	mov    BYTE PTR [rsp+0x10],0x43
    1936:	66 c7 44 24 0e 41 42 	mov    WORD PTR [rsp+0xe],0x4241
    193d:	66 c7 44 24 11 20 25 	mov    WORD PTR [rsp+0x11],0x2520
    1944:	c6 44 24 13 7b       	mov    BYTE PTR [rsp+0x13],0x7b
    1949:	c6 44 24 18 7d       	mov    BYTE PTR [rsp+0x18],0x7d
    194e:	c7 44 24 14 70 6f 73 	mov    DWORD PTR [rsp+0x14],0x74736f70
    1955:	74 
    1956:	c6 44 24 19 00       	mov    BYTE PTR [rsp+0x19],0x0
    195b:	0f 11 84 24 70 01 00 	movups XMMWORD PTR [rsp+0x170],xmm0
    1962:	00 
    1963:	0f 11 84 24 61 01 00 	movups XMMWORD PTR [rsp+0x161],xmm0
    196a:	00 
    196b:	0f 11 84 24 51 01 00 	movups XMMWORD PTR [rsp+0x151],xmm0
    1972:	00 
    1973:	0f 11 84 24 41 01 00 	movups XMMWORD PTR [rsp+0x141],xmm0
    197a:	00 
    197b:	0f 11 84 24 31 01 00 	movups XMMWORD PTR [rsp+0x131],xmm0
    1982:	00 
    1983:	0f 11 84 24 21 01 00 	movups XMMWORD PTR [rsp+0x121],xmm0
    198a:	00 
    198b:	0f 11 84 24 11 01 00 	movups XMMWORD PTR [rsp+0x111],xmm0
    1992:	00 
    1993:	c7 84 24 03 01 00 00 	mov    DWORD PTR [rsp+0x103],0x203a3465
    199a:	65 34 3a 20 
    199e:	c7 84 24 00 01 00 00 	mov    DWORD PTR [rsp+0x100],0x65736163
    19a5:	63 61 73 65 
    19a9:	66 c7 84 24 07 01 00 	mov    WORD PTR [rsp+0x107],0x7b25
    19b0:	00 25 7b 
    19b3:	c6 84 24 09 01 00 00 	mov    BYTE PTR [rsp+0x109],0x20
    19ba:	20 
    19bb:	c6 84 24 0c 01 00 00 	mov    BYTE PTR [rsp+0x10c],0x43
    19c2:	43 
    19c3:	66 c7 84 24 0a 01 00 	mov    WORD PTR [rsp+0x10a],0x4241
    19ca:	00 41 42 
    19cd:	c7 84 24 0d 01 00 00 	mov    DWORD PTR [rsp+0x10d],0x7b2520
    19d4:	20 25 7b 00 
    19d8:	48 8d bc 24 00 02 00 	lea    rdi,[rsp+0x200]
    19df:	00 
    19e0:	e8 bb 00 00 00       	call   1aa0 <puts@plt>
    19e5:	48 8d bc 24 80 00 00 	lea    rdi,[rsp+0x80]
    19ec:	00 
    19ed:	e8 ae 00 00 00       	call   1aa0 <puts@plt>
    19f2:	48 8d 9c 24 80 01 00 	lea    rbx,[rsp+0x180]
    19f9:	00 
    19fa:	48 89 df             	mov    rdi,rbx
    19fd:	e8 9e 00 00 00       	call   1aa0 <puts@plt>
    1a02:	48 89 e7             	mov    rdi,rsp
    1a05:	e8 96 00 00 00       	call   1aa0 <puts@plt>
    1a0a:	48 8d bc 24 00 01 00 	lea    rdi,[rsp+0x100]
    1a11:	00 
    1a12:	e8 89 00 00 00       	call   1aa0 <puts@plt>
    1a17:	48 8d 04 1c          	lea    rax,[rsp+rbx*1]
    1a1b:	48 05 80 00 00 00    	add    rax,0x80
    1a21:	48 01 e0             	add    rax,rsp
    1a24:	48 f7 d8             	neg    rax
    1a27:	48 01 e0             	add    rax,rsp
    1a2a:	48 05 8d 00 00 00    	add    rax,0x8d
    1a30:	48 01 e0             	add    rax,rsp
    1a33:	48 05 97 01 00 00    	add    rax,0x197
    1a39:	48 8d 0c 04          	lea    rcx,[rsp+rax*1]
    1a3d:	48 83 c1 19          	add    rcx,0x19
    1a41:	31 c0                	xor    eax,eax
    1a43:	48 83 f9 db          	cmp    rcx,0xffffffffffffffdb
    1a47:	0f 94 c0             	sete   al
    1a4a:	48 81 c4 80 02 00 00 	add    rsp,0x280
    1a51:	5b                   	pop    rbx
    1a52:	c3                   	ret

