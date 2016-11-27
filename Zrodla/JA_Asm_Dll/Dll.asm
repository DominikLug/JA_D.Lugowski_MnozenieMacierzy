.data
zmienna DWORD ?
.code

Foo PROC
	
	mov zmienna, 8
	mov rax, zmienna
	ret

Foo endp

end