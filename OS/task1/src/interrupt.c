#include <interrupt.h>
#include <ints.h>
#include <memory.h>
#include <desc.h>
#include <printf.h>
#include <ioport.h>

static struct IDTEntry idt[258];

static void send_eoi_master()
{
    out8(PIC_CMD_MASTER_PORT, 1 << 5);
}

static void send_eoi_slave()
{
    out8(PIC_CMD_SLAVE_PORT, 1 << 5);
}

void init_interrupt_controllers()
{

    // Initialize Master to [32, 40) entries.
    out8(PIC_CMD_MASTER_PORT, (1 << 4) | 1);
    out8(PIC_DATA_MASTER_PORT, PIC_MASTER_IDT_START);
    out8(PIC_DATA_MASTER_PORT, 1 << 2);
    out8(PIC_DATA_MASTER_PORT, 1);

    // Initialize Slave to [40, 48) entries.
    out8(PIC_CMD_SLAVE_PORT, (1 << 4) | 1);
    out8(PIC_DATA_SLAVE_PORT, PIC_SLAVE_IDT_START);
    out8(PIC_DATA_SLAVE_PORT, 2);
    out8(PIC_DATA_SLAVE_PORT, 1);

    // Mask interrupts on Master.
    // Allow interrupts from Slave and PIT.
    out8(PIC_DATA_MASTER_PORT, (1 << PIC_NUM_LEGS) - 1 - (1 << 2) - (1 << 0));    

    // Mask interrupts on Slave.
    out8(PIC_DATA_SLAVE_PORT, (1 << PIC_NUM_LEGS) - 1);    

    // 'cli' was called in bootstrap.S. Undo it.
    enable_ints();
}

void init_idt()
{
    extern uint64_t interrupt_table[];
    extern uint16_t interrupt_table_size;

    for (int i = 0; i < interrupt_table_size; i++) 
    {
        idt[i].offset_low = interrupt_table[i] & 0xFFFF;
        idt[i].seg_selector = KERNEL_CS;

        idt[i].type = 0;
        idt[i].type |= 1 << 15;
        idt[i].type |= 14 << 8; // Interrupt gate.

        idt[i].offset_mid = (interrupt_table[i] >> 16) & 0xFFFF;
        idt[i].offset_high = interrupt_table[i] >> 32;

        idt[i].smth_null = 0;
    }

    struct desc_table_ptr ptr;
    ptr.size = sizeof(struct IDTEntry) * interrupt_table_size - 1;
    ptr.addr = (uint64_t)idt;

    write_idtr(&ptr);
}


static void interrupt_pic_master(struct InterruptFrame *frame)
{
    int vector = frame->vector;

    if (vector - PIC_MASTER_IDT_START == 0) 
    {
        // PIT is connected to leg #0.
        printf("Received interrupt from PIT\n");
    }

    send_eoi_master();
}

static void interrupt_pic_slave(struct InterruptFrame *frame)
{
    int vector = frame->vector;

    if (vector == 32) 
    {
        // Get rid of "unused variable" warning.
    }

    send_eoi_slave();
    send_eoi_master();
}


void interrupt_route(struct InterruptFrame *frame)
{
    int vector = frame->vector;
    if (vector >= PIC_MASTER_IDT_START && 
            vector < PIC_MASTER_IDT_START + PIC_NUM_LEGS) 
    {
        interrupt_pic_master(frame);        
    } 
    else if (vector >= PIC_SLAVE_IDT_START && 
            vector < PIC_SLAVE_IDT_START + PIC_NUM_LEGS) 
    {
        interrupt_pic_slave(frame);
    } 
    else 
    {
        printf("Interrupt #%d occured. Halting\n", frame->vector);
        while (1);
    }
}


