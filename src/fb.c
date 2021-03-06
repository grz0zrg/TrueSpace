#include <sys/mman.h>
#include <fcntl.h>

#ifdef __x86_64
    #include <asm/unistd_64.h>
#else
    #include <asm/unistd_32.h>
#endif

// framebuffer settings (width / height / components should match actual framebuffer settings)
#define FRAMEBUFFER_COMPONENTS 4
#define FRAMEBUFFER_LENGTH (WIDTH * HEIGHT * FRAMEBUFFER_COMPONENTS)

inline static int sys_open(char *filepath, int flags, int mode) {
    int r;
#ifdef __x86_64
    __asm__ volatile("syscall"
        : "=a"(r)
        : "0"(__NR_open), "D"(filepath), "S"(flags)//, "d"(mode)
        : "cc", "rcx", "r11", "memory");
#else
    __asm__ volatile("int $0x80"
        : "=a"(r)
        : "0"(__NR_open), "b"(filepath), "c"(flags), "d"(mode)
        : "cc", "edi", "esi", "memory");
#endif
  return r;
}

inline static unsigned char *sys_mmap(unsigned int *addr, unsigned long length, unsigned long prot, unsigned long flags, unsigned long fd) {
#ifdef __x86_64
    register volatile int r10 __asm__ ("r10") = flags; register volatile int r8 __asm__ ("r8") = fd; unsigned char *r;
    __asm__ volatile ("syscall" : "=a" (r) : "a" (__NR_mmap)/*, "D" (addr)*/, "S" (length), "d" (prot), "r" (r10), "r" (r8) : "cc", "memory", "r11", "rcx");
    return r;
#else
    unsigned int args[2] = { (unsigned int)addr, 0 };
    unsigned int *r;

    __asm__ __volatile__("push %%ebp\n"
                        "movl 4(%%ebx), %%ebp\n"
                        "movl 0(%%ebx), %%ebx\n"
                        "int $0x80\n"
                        "pop %%ebp\n"
                        : "=a"(r)
                        : "a"(__NR_mmap2), "b"(&args),
                            "c"(length), "d"(prot), "S"(flags), "D"(fd));

    return r;
#endif
}

void _start() {
    // 0x00400007 : see elf64.s (this is /dev/fb0 string address packed into the ELF padding / ABI field)
    int fbfd = sys_open((char *)0x00400007, O_RDWR, 0);
    unsigned char *buffer = (unsigned char *)sys_mmap(0, FRAMEBUFFER_LENGTH, PROT_READ|PROT_WRITE, MAP_SHARED, fbfd);

    int frame, index, x, y, ox, oy;

    while (1) {
        // minsky circle (somewhat)
        x = ox - (oy >> 1);
        y = (oy >> 1) + (x - (index >> 11));

        index = (((x << 1) ^ y) + (y + (frame >> 13)) * WIDTH) * 4;

        // clamp
        index %= WIDTH * HEIGHT * 4;
        if (index < 0) index = 0;

        buffer[index + 0] += 2;
        buffer[index + 1] += 2;
        buffer[index + 2] += 1;

        ox = x;
        oy = y;

        frame += 1;
    }
}
