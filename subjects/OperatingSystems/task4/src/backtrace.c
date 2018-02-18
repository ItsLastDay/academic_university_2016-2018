#include <backtrace.h>
#include <memory.h>
#include <print.h>

#define RBP(x)	__asm__ ("movq %%rbp, %0" : "=rm"(x))
#define RSP(x)	__asm__ ("movq %%rsp, %0" : "=rm"(x))


void __backtrace(uintptr_t rbp, uintptr_t stack_begin, uintptr_t stack_end)
{
	int frame_index = 0;

	while (rbp >= stack_begin && rbp + 16 <= stack_end) {
		const uint64_t *frame = (const uint64_t *)rbp;
		const uintptr_t prev_rbp = frame[0];
		const uintptr_t prev_rip = frame[1];

		if (prev_rbp <= rbp)
			break;

		printf("%d: rip 0x%lx\n", frame_index, (unsigned long)prev_rip);
		rbp = prev_rbp;
		++frame_index;
	}
}

/* Here we assume that stack is PAGE_SIZE long and PAGE_SIZE
 * aligned, alternatively we can export bootstrap stack limits
 * from bootstrap.S */
uintptr_t stack_begin(void)
{
	uintptr_t rsp;

	RSP(rsp);
	return rsp & ~((uintptr_t)(PAGE_SIZE - 1));
}

uintptr_t stack_end(void)
{
	return stack_begin() + PAGE_SIZE;
}

void backtrace(void)
{
	uintptr_t rbp;

	RBP(rbp);
	__backtrace(rbp, stack_begin(), stack_end());
}
