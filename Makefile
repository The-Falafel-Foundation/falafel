CC = gcc
AS = nasm
LD = ld

CFLAGS = -m64 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector -mno-red-zone
ASFLAGS = -f elf64
LDFLAGS = -n -T linker.ld

SRCS = kernel/main.c
OBJS = kernel/boot.o kernel/main.o
KERNEL = falafel-x86_64.bin
ISO = falafel.iso

all: $(ISO)

$(ISO): $(KERNEL)
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/
	cp grub.cfg isodir/boot/grub/
	grub-mkrescue -o $(ISO) isodir

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $(KERNEL) $(OBJS)

kernel/boot.o: kernel/boot.s
	$(AS) $(ASFLAGS) $< -o $@

kernel/main.o: kernel/main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf kernel/*.o $(KERNEL) $(ISO) isodir

run: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO)
