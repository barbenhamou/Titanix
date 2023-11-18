#GCC

COMPILER = gcc
C_FLAGS = -c -g -nostdlib -fno-builtin -nostdinc -fno-stack-protector -I./include

#LINKER

LINKER = ld
LINKER_FLAGS = -n -Ttext 0x7c00

#ASM

ASSEMBLER = nasm
ASM_FLAGS = -f elf64

#IMPORTANT RANDOM STAFF

C_FILES = $(shell find src/ -name '*.c')
ASM_FILES = $(shell find src/ -name '*.s')
OBJ_FILES = $(addprefix build/c/, $(notdir $(C_FILES:.c=.o)))  $(addprefix build/asm/, $(notdir $(ASM_FILES:.s=.o)))

LINKER_SCRIPT := linker.ld
GRUB_CFG := isofiles/boot/grub/grub.cfg

default: run

build: build/os.iso

run: build
		qemu-system-x86_64 -cdrom build/os.iso -D log.txt -vga std -serial stdio -smp cores=1 -m 1024,maxmem=8G

gdb: build
		qemu-system-x86_64 -cdrom build/os.iso -s -S -D log.txt -vga std -serial stdio -smp cores=1 -m 1024,maxmem=8G & gdb

build/c/%.o: src/*/*%.c
		mkdir -p build
		mkdir -p build/c
		$(COMPILER) $(C_FLAGS) -o $@ $<

build/asm/%.o: src/*/*%.s
		mkdir -p build
		mkdir -p build/asm
		$(ASSEMBLER) $(ASM_FLAGS) -o $@ $<

build/kernel.so: $(OBJ_FILES) $(LINKER_SCRIPT)
		$(LINKER) $(LINKER_FLAGS) -T $(LINKER_SCRIPT) $(OBJ_FILES) -o $@

build/os.iso: build/kernel.so $(GRUB_CFG)
		mkdir -p build/isofiles/boot/grub
		cp $(GRUB_CFG) build/isofiles/boot/grub
		cp build/kernel.so build/isofiles/boot/
		grub-mkrescue -o $@ build/isofiles

clean:
		rm -rf build
