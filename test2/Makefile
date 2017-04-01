src = $(wildcard src/*.s)
obj = $(src:.s=.o)

name = test
elf = $(name).elf
bin = $(name).bin

tool_prefix = m68k-linux-gnu-

CC = $(tool_prefix)gcc
AS = $(tool_prefix)as
LD = $(tool_prefix)ld
OBJCOPY = $(tool_prefix)objcopy
OBJDUMP = $(tool_prefix)objdump

ASFLAGS = -m68010
LDFLAGS = -T m68kcomp1.ldscript

.PHONY: rom
rom: even.rom odd.rom

$(bin): $(elf)
	$(OBJCOPY) -O binary $< $@

$(elf): $(obj)
	$(LD) -o $@ $(obj) -Map link.map $(LDFLAGS)

even.rom: $(bin)
	byteweave -even $< $@
	truncate -s 8k $@

odd.rom: $(bin)
	byteweave -odd $< $@
	truncate -s 8k $@

.PHONY: progeven
progeven: even.rom
	minipro -p AT28C64B -w $<

.PHONY: progodd
progodd: odd.rom
	minipro -p AT28C64B -w $<

.PHONY: disasm
disasm: $(elf)
	$(OBJDUMP) -d $<

.PHONY: clean
clean:
	rm -f $(bin) $(obj) $(elf) even.bin odd.bin even.rom odd.rom
