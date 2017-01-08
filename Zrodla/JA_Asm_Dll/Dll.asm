.data
tabA dq ?
tabB dq ?
tabC dq ?

rowA dq ?
rowB dq ?
rowC dq ?
elmCptr dq ?

sizeX dq ?
sizeY dq ?
sizeZ dq ?

range0 dq ?
range1 dq ?

NotEven dq 1
NotEvenPass dq 0
.code

Foo PROC
	emms
	movsxd r9,dword ptr [rdx]
	xor rax,rax
	
	mov rax,qword ptr[rcx]
	mov tabA,rax
	mov rax,qword ptr[rcx+8]
	mov tabB,rax
	mov rax,qword ptr[rcx+16]
	mov tabC,rax
	
	mov eax,dword ptr[rdx]
	mov range0,rax
	mov eax,dword ptr[rdx+4]
	mov range1,rax

	mov eax,dword ptr[r8]
	mov sizeX,rax
	mov eax,dword ptr[r8+4]
	mov sizeY,rax
	mov eax,dword ptr[r8+8]
	mov sizeZ,rax

	
	mov rax,tabA
	mov rax,qword ptr[rax]
	mov rowA,rax

	mov rax,tabC
	mov rax,qword ptr[rax]
	mov rowC,rax

	mov r9,range0

	loop_r:
		cmp r9,range1
		jge loop_re

		mov rax,r9
		imul rax,8
		add rax,tabA
		mov rax,qword ptr[rax]
		mov rowA, rax
		
		mov rax,r9
		imul rax,8
		add rax,tabC
		mov rax,qword ptr[rax]
		mov rowC, rax

		mov r10,0
		loop_j:
			cmp r10,sizeZ
			jge loop_je

			mov rax,r10
			imul rax,8
			add rax,rowC
			mov elmCptr, rax
			
			mov rax,r10
			imul rax,8
			add rax,tabB
			mov rax,qword ptr[rax]
			mov rowB, rax

			mov r11,0			
			loop_k:
				cmp r11,sizeY
				jge loop_ke

			 	mov rax,r11
				imul rax,8
				add rax,rowB
				movaps xmm2,oword ptr[rax] ; tu zmiana od B
				
				mov rax,r11
				imul rax,8
				add rax,rowA
				movaps xmm1,oword ptr[rax] 

				mov rax,elmCptr
				movsd xmm3,qword ptr[rax]
				mulpd xmm1,xmm2
				haddpd xmm1,xmm1
				addpd xmm3,xmm1

				movq qword ptr[rax],xmm3
				

				add r11,2

				jmp loop_k
			loop_ke:

			add r10,1 
			jmp loop_j
	loop_je:
		
		add r9,1
		jmp loop_r
loop_re:	
mov rax,1
ret


	Foo endp
end

