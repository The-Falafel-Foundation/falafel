CC = gcc
AS = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra \
         -fno-stack-protector \
         -MMD -MP
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -n -T linker.ld

KERNEL = falafel-x86_32.bin
ISO = falafel.iso

C_SRCS := $(shell find kernel -name '*.c')
C_OBJS := $(C_SRCS:.c=.o)

ASM_SRCS := kernel/boot.s
ASM_OBJS := $(ASM_SRCS:.s=.o)

OBJS := $(ASM_OBJS) $(C_OBJS)

DEPS := $(OBJS:.o=.d)

all: $(ISO)

$(ISO): $(KERNEL)
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/
	cp grub.cfg isodir/boot/grub/
	grub-mkrescue -o $(ISO) isodir

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/%.o: kernel/%.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(KERNEL) $(ISO) isodir
	find kernel -name '*.o' -delete
	find kernel -name '*.d' -delete

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

-include $(DEPS)
