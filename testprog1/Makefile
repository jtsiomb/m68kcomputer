ssrc = $(wildcard src/*.s)
csrc = $(wildcard src/*.c)
obj = $(ssrc:.s=.o) $(csrc:.c=.o)

name = testprog1
elf = $(name).elf
bin = $(name).bin

tool_prefix = m68k-linux-gnu-

CC = $(tool_prefix)gcc
AS = $(tool_prefix)as
LD = $(tool_prefix)ld
OBJCOPY = $(tool_prefix)objcopy
OBJDUMP = $(tool_prefix)objdump

warn = -pedantic -Wall
opt = -Os
def = -D__NO_CTYPE
inc = -Isrc

CFLAGS = -m68010 -ffreestanding -fno-builtin $(warn) $(opt) $(inc) $(def)
ASFLAGS = -m68010 $(inc)
LDFLAGS = -T loadprog.ldscript -L/usr/lib/gcc-cross/m68k-linux-gnu/6 -lgcc

.PHONY: all
all: $(name).mon

$(bin): $(elf)
	$(OBJCOPY) -O binary $< $@

$(elf): $(obj)
	$(LD) -o $@ $(obj) -Map link.map $(LDFLAGS)

$(name).mon: $(bin)
	bin2moncmd -o $@ $<

.PHONY: disasm
disasm: $(elf)
	$(OBJDUMP) -d $<

.PHONY: clean
clean:
	rm -f $(bin) $(obj) $(elf)
