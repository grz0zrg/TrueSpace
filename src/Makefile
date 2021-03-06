CC=gcc
STANDARD_FLAGS=-std=c99 -pedantic -Wno-switch -Wno-conversion -Wno-uninitialized -Wno-strict-aliasing \
	-fno-exceptions -ffast-math -fsingle-precision-constant -fno-ident \
    -funsafe-math-optimizations -fvisibility=hidden -fmerge-all-constants \
    -fno-align-functions -fno-align-loops -fno-math-errno #-march=native
OPTI_FLAGS=-Os -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-stack-protector -ftree-vectorize -fopt-info-vec-optimized \
	-fomit-frame-pointer -fno-math-errno -fdata-sections -fno-stack-check \
	 -fno-unroll-loops -Winline -nostartfiles -nodefaultlibs -no-pie -fno-plt -fno-pic #-ffunction-sections
LINKER_FLAGS=-Wl,--gc-sections \
	-Wl,--build-id=none -z norelro -Wl,-z,noseparate-code -Wl,--no-eh-frame-hdr \
	-Wl,--no-ld-generated-unwind-info -Wl,--hash-style=sysv \
	-Wl,-z,nodynamic-undefined-weak
NAME=fb
LZMA_ARGS=--format=lzma -9 --extreme --lzma1=preset=9,lc=0,lp=0,pb=0,nice=32,depth=4,dict=16384 --keep

all:
	rm -f $(NAME).lzma
	rm -f payload
	rm -f payload.bin
	rm -f nm_output
	$(CC) -o $(NAME) $(NAME).c -DWIDTH=$(WIDTH) -DHEIGHT=$(HEIGHT) -DSHIFT=${SHIFT} $(STANDARD_FLAGS) $(OPTI_FLAGS) $(LINKER_FLAGS)
	# source : http://mainisusuallyafunction.blogspot.com/2015/01/151-byte-static-linux-binary-in-rust.html
	# 'smash all the sections together, with no alignment padding, then extract that section as a headerless binary blob'
	# add '-m elf_i386' to ld for 32 bits version (-m32 GCC flag as well and rename elf64.s to elf32.s below)
	ld --gc-sections -e _start -T script.ld -o payload $(NAME)
	objcopy -j combined -O binary payload payload.bin
	# determine the entry point address
	nm -f posix payload | grep '^_start ' | awk '{print $$3}' > nm_output
	# data offset (hex)
	$(eval OFFSET=0)
	$(eval ENTRY=$(shell echo "obase=16;ibase=16;$(shell cat nm_output) + $(OFFSET)" | bc))
	$(eval ENTRY=$(shell perl -e 'print sprintf "%016s\n","$(ENTRY)"'))
	# build the final binary from the binary and custom ELF header
	nasm -f bin -o $(NAME) -D entry=0x$(ENTRY) elf64.s
	wc -c $(NAME)
	#lzma $(LZMA_ARGS) $(NAME)
	#cat unpack_lzma.header $(NAME).lzma > $(NAME)
	#truncate -s -4 $(NAME)
	chmod +x $(NAME)
	rm -f $(NAME).lzma
	rm -f payload
	rm -f payload.bin
	wc -c $(NAME)
	mv $(NAME) ../$(NAME)_$(WIDTH)x$(HEIGHT)x32
	chmod +x ../$(NAME)_$(WIDTH)x$(HEIGHT)x32
