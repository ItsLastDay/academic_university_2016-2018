#/usr/bin/python3


def save_regs(regs):
    def save_reg(name):
        return "\tpushq %{}".format(name)
    return "\n".join(map(save_reg, regs));

def restore_regs(regs):
    def restore_reg(name):
        return "\tpopq %{}".format(name)
    return "\n".join(map(restore_reg, reversed(regs)))

def entry_without_error(isr, common):
    name = "isr{}".format(isr)
    entry = "{}:\n\tpushq ${}\n\tpushq ${}\n\tjmp {}"
    return entry.format(name, 0, isr, common)

def entry_with_error(isr, common):
    name = "isr{}".format(isr)
    entry = "{}:\n\tpushq ${}\n\tjmp {}"
    return entry.format(name, isr, common)

def generate(entries, handler):
    with_err = [8, 10, 11, 12, 13, 14, 17]
    common = "common"
    regs = [
        "rdi", "rsi", "rdx", "rcx", "rax", "r8", "r9",
        "r10", "r11", "rbx", "rbp", "r12", "r13", "r14", "r15"
    ]

    print("\t.text")
    for i in range(256):
        if i in with_err:
            print(entry_with_error(i, common))
        else:
            print(entry_without_error(i, common))

    print()

    print("\t.extern {}".format(handler))
    print("common:")
    print(save_regs(regs))
    print("\tmovq %rsp, %rdi")
    print("\tcld")
    print("\tcall {}".format(handler))
    print(restore_regs(regs))
    print("\taddq $0x10, %rsp")
    print("\tiretq")

    print()

    print("\t.global {}".format(entries))
    print("{}:".format(entries))
    for i in range(256):
        print("\t.quad isr{}".format(i))


generate("__int_entry", "__int_handler")
