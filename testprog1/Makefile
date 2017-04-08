ssrc = $(wildcard src/*.s)
csrc = $(wildcard src/*.c) $(wildcard src/libc/*.c)
obj = $(ssrc:.s=.o) $(csrc:.c=.o)

name = testprog1
elf = $(name).elf
bin = $(name).bin

tool_prefix = m68k-elf-

CC = $(tool_prefix)gcc
AS = $(tool_prefix)as
LD = $(tool_prefix)ld
OBJCOPY = $(tool_prefix)objcopy
OBJDUMP = $(tool_prefix)objdump

warn = -pedantic -Wall -Wno-unused-but-set-variable
opt = -Os
def = -D__NO_CTYPE
inc = -Isrc -Isrc/libc

CFLAGS = -m68010 -ffreestanding -fno-builtin -nostdinc $(warn) $(opt) $(inc) $(def)
ASFLAGS = -m68010 $(inc)
LDFLAGS = -m68010 -ffreestanding -nostdlib -T loadprog.ldscript -lgcc

.PHONY: all
all: $(name).mon

$(bin): $(elf)
	$(OBJCOPY) -O binary $< $@

$(elf): $(obj)
	$(CC) -o $@ $(obj) -Wl,-Map -Wl,link.map $(LDFLAGS)

$(name).mon: $(bin)
	bin2moncmd -o $@ $<

.PHONY: disasm
disasm: $(elf)
	$(OBJDUMP) -d $<

.PHONY: clean
clean:
	rm -f $(bin) $(obj) $(elf)
