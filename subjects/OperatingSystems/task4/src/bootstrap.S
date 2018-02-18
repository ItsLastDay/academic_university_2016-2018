#include <memory.h>

#define MB_HEADER_MAGIC	0x1BADB002
#define MB_HEADER_FLAGS	((1 << 16) | (1 << 1))
#define MB_HEADER_CKSUM	-(MB_HEADER_MAGIC + MB_HEADER_FLAGS)

	.section .bootstrap, "ax"
	.code32
	.global start32
	.extern main


	.align 16
start32:
	jmp 1f


	.align 16
multiboot_header:
	.long MB_HEADER_MAGIC
	.long MB_HEADER_FLAGS
	.long MB_HEADER_CKSUM
	.long multiboot_header
	.long text_phys_begin
	.long data_phys_end
	.long bss_phys_end
	.long start32


	.align 16
gdt:
	.quad 0x0000000000000000
	.quad 0x00209b0000000000 // 64 bit ring0 code segment
	.quad 0x0020930000000000 // 64 bit ring0 data segment
	.quad 0x0020fb0000000000 // 64 bit ring3 code segment
	.quad 0x0020f30000000000 // 64 bit ring3 data segment
	.quad 0x0000000000000000 // reserve for TSS
	.quad 0x0000000000000000
	.quad 0x00cf9b000000ffff // 32 bit ring0 code segment
	.quad 0x00cf93000000ffff // 32 bit ring0 data segment
	.quad 0x0000000000000000
gdt_ptr:
	.word (gdt_ptr - gdt - 1)
	.long gdt
gdt_ptr64:
	.word (gdt_ptr - gdt - 1)
	.quad (gdt + VIRTUAL_BASE) 
idt_ptr:
	.word 0
	.quad 0

	.align 16
	.space 0x100
stack_top:

1:
	movl $stack_top, %esp
	pushl %ebx
	pushl %eax
	call videomem_clrscr
	popl %eax
	cmpl $0x2badb002, %eax
	je 1f

	pushl $not_multiboot
	jmp fail

1:
	call check_long_mode
	call setup_mapping

#define EFER_MSR	0xC0000080
#define EFER_LM		(1 << 8)
	movl $EFER_MSR, %ecx
	rdmsr
	orl $EFER_LM, %eax
	wrmsr
	
	pushl $enable_paging
	call videomem_puts
	addl $4, %esp

#define CR0_PG		(1 << 31)
#define CR0_NE		(1 << 5)
	movl %cr0, %eax
	orl $(CR0_PG | CR0_NE), %eax
	movl %eax, %cr0

	pushl $enter_long_mode
	call videomem_puts
	addl $4, %esp

	popl %ebx
	lidt idt_ptr
	lgdt gdt_ptr
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	ljmpl $0x08, $start64


#define PTE_PRESENT	(1 << 0)
#define PTE_WRITE	(1 << 1)
#define PTE_LARGE	(1 << 7)
#define CR4_PAE		(1 << 5)

setup_mapping:
	pushl $setup_paging
	call videomem_puts
	addl $4, %esp

	movl $(bootstrap_pml3 - VIRTUAL_BASE), %edi
	movl $(PTE_PRESENT | PTE_WRITE | PTE_LARGE), %eax
	movl $0, %ebx
	movl %eax, (%edi)
	movl %ebx, 4(%edi)
	movl %eax, 4080(%edi)
	movl %ebx, 4084(%edi)
	add $0x40000000, %eax
	adc $0, %ebx
	movl %eax, 8(%edi)
	movl %ebx, 12(%edi)
	movl %eax, 4088(%edi)
	movl %ebx, 4092(%edi)
	add $0x40000000, %eax
	adc $0, %ebx
	movl %eax, 16(%edi)
	movl %ebx, 20(%edi)
	add $0x40000000, %eax
	adc $0, %ebx
	movl %eax, 24(%edi)
	movl %ebx, 28(%edi)

	movl $(bootstrap_pml4 - VIRTUAL_BASE), %edi
	movl $(bootstrap_pml3 - VIRTUAL_BASE + PTE_PRESENT + PTE_WRITE), %eax
	movl %eax, (%edi)
	movl %eax, 2048(%edi)
	movl %eax, 4088(%edi)

	movl %cr4, %eax
	orl $CR4_PAE, %eax
	movl %eax, %cr4

	movl $(bootstrap_pml4 - VIRTUAL_BASE), %eax
	movl %eax, %cr3
	ret


check_long_mode:
	subl $4, %esp
	movl $checking_cpuid, (%esp)
	call videomem_puts

	pushf
	pushf
	xorl $(1 << 21), (%esp)
	popf
	pushf
	popl %eax
	xor (%esp), %eax
	popf
	test $(1 << 21), %eax
	jne 1f

	movl $cpuid_not_available, (%esp)
	jmp fail
1:
	movl $checking_x86_64, (%esp)
	call videomem_puts

	movl $0x80000000, %eax
	cpuid
	cmpl $0x80000001, %eax
	jnb 1f

	movl $no_extended_cpuid, (%esp)
	jmp fail

1:
	movl $0x80000001, %eax
	cpuid
	test $(1 << 29), %edx
	jnz 1f

	movl $x86_64_not_available, (%esp)
	jmp fail

1:
	addl $4, %esp
	ret


fail:
	call videomem_puts
halt:	jmp halt


not_multiboot:
	.asciz "Not a multiboot compatible bootloader!\n";
no_memory_map:
	.asciz "No memory map provided!\n";
checking_cpuid:
	.asciz "Checking CPUID instruction...\n"
cpuid_not_available:
	.asciz "CPUID instruction is not available!\n"
checking_x86_64:
	.asciz "Checking long mode...\n"
no_extended_cpuid:
	.asciz "There is no extended CPUID function available!\n"
x86_64_not_available:
	.asciz "Long mode is not available!\n"
setup_paging:
	.asciz "Setup bootstrap paging...\n"
enter_long_mode:
	.asciz "Enter long mode...\n"
enable_paging:
	.asciz "Enable paging...\n"


	.code64
start64:
	lgdt gdt_ptr64
	movabsq $bootstrap_stack_top, %rax
	movq %rax, %rsp

	/* ebx is 32-bit long, while we use 64-bit long address,
	 * make sure that higher 32 bits don't contain garbage. */
	xorq %rdi, %rdi
	movl %ebx, %edi

	cld
	movabsq $main, %rax
	call *%rax

	cli
1:
	hlt
	jmp 1b


	.bss
	.align PAGE_SIZE
	.space PAGE_SIZE
bootstrap_stack_top:
	.align PAGE_SIZE
bootstrap_pml4:
	.space PAGE_SIZE
bootstrap_pml3:
	.space PAGE_SIZE
