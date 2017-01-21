// Generated by asm2c https://github.com/frranck/asm2c
#ifndef ADDSUB_H__
#define ADDSUB_H__
// /opt/retropie/emulators/retroarch/bin/retroarch -L ./lodsb_libretro.so
// sudo swapon -a
// qemu-system-ppc  -hda ./debian_wheezy_powerpc_standard.qcow2  -L bios -m 1G -g 1024x768x8 -net user,hostfwd=tcp::10022-:22 -net nic

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>

#if defined(_WIN32) || defined(__INTEL_COMPILER)
#define INLINE __inline
#elif defined(__STDC_VERSION__) && __STDC_VERSION__>=199901L
#define INLINE inline
#elif defined(__GNUC__)
#define INLINE __inline__
#else
#define INLINE
#endif

/**
 * is_little_endian: 
 *
 * Checks if the system is little endian or big-endian.
 *
 * Returns: greater than 0 if little-endian,
 * otherwise big-endian.
 **/
static INLINE uint8_t is_little_endian(void)
{
#if defined(__x86_64) || defined(__i386) || defined(_M_IX86) || defined(_M_X64)
    return 1;
#elif defined(MSB_FIRST)
    return 0;
#else
    union
    {
        uint16_t x;
        uint8_t y[2];
    } u;
    
    u.x = 1;
    return u.y[0];
#endif
}


#define realAddress(offset,segment) ((db *)&m+(dd)offset+m.selectors[m.segment])

#define db unsigned char
#define dw unsigned short
#define dd unsigned int

#define VGARAM_SIZE 320*200
#define STACK_SIZE 1024*sizeof(dd)
#define HEAP_SIZE 1024*1024*4
#define NB_SELECTORS 128

#define PUSHAD memcpy (&m.stack[m.stackPointer], &m.eax, sizeof (dd)*8);m.stackPointer+=sizeof(dd)*8
#define POPAD m.stackPointer-=sizeof(dd)*8;memcpy (&m.eax, &m.stack[m.stackPointer], sizeof (dd)*8)

#define PUSH(nbBits,a) memcpy (&m.stack[m.stackPointer], &a, sizeof (a));m.stackPointer+=sizeof(a)
#define POP(nbBits,a) m.stackPointer-=sizeof(a);memcpy (&a, &m.stack[m.stackPointer], sizeof (a))

#define AFFECT_ZF(a) m.ZF=(a==0)
#define AFFECT_CF(a) m.CF=a
#define AFFECT_SF(nbBits, a) m.SF=(a>>(nbBits-1))
#define ISNEGATIVE(nbBits,a) (a & (1 << (nbBits-1)))

// TODO: add missings affected flags on CMP
#define CMP(nbBits,a,nbBitsSrc,b) AFFECT_ZF(a-b);AFFECT_CF(a<b);AFFECT_SF(nbBits,(a-b));
#define OR(nbBits,a,nbBitsSrc,b) a=a|b;AFFECT_ZF(a);AFFECT_SF(nbBits, a)
#define XOR(nbBits,a,nbBitsSrc,b) a=a^b;AFFECT_ZF(a);AFFECT_SF(nbBits, a)
#define AND(nbBits,a,nbBitsSrc,b) a=a&b;AFFECT_ZF(a);AFFECT_SF(nbBits, a)
#define NEG(nbBits,a) AFFECT_CF(a!=0);a=-a;
#define TEST(nbBits,a,nbBitsSrc,b) AFFECT_ZF((a&b));AFFECT_CF(b<a);AFFECT_SF(nbBits, (a&b))

#define SHR(nbBits,a,nbBitsSrc,b) a=a>>b
#define SHL(nbBits,a,nbBitsSrc,b) a=a<<b
#define ROR(nbBits,a,nbBitsSrc,b) a=(a>>b | a<<(nbBits-b))
#define ROL(nbBits,a,nbBitsSrc,b) a=(a<<b | a>>(nbBits-b))

#define READDDp(a) ((dd *) &m.a)
#define READDWp(a) ((dw *) &m.a)
#define READDBp(a) ((db *) &m.a)

#define READDD(a) (*(dd *) &m.a)

// TODO: fix for little endian

//#define READDW(a) (*(dw *) &m.a)
#define READDW(a) (*(dw *)(((db *) &m.a)+(is_little_endian()?0:(sizeOf ## a==2?0:2))))
#define READDBh(a) (*(((db *) &m.a)+(is_little_endian()?1:(sizeOf ## a==2?0:2))))
#define READDBl(a) (*(((db *) &m.a)+(is_little_endian()?0:(sizeOf ## a==2?1:3))))

#define READDBhW(a) (*(((db *) &m.a)+(is_little_endian()?1:0)))
#define READDBhD(a) (*(((db *) &m.a)+(is_little_endian()?1:2)))
#define READDBlW(a) (*(((db *) &m.a)+(is_little_endian()?0:1)))
#define READDBlD(a) (*(((db *) &m.a)+(is_little_endian()?0:3)))


//#define WRITEDDWreg(a,b) m.a = (m.a & 0xffffffffffff0000) | b

#define ADD(nbBits,a,nbBitsSrc,b) a=a+b;AFFECT_ZF(a);AFFECT_CF(a<b);AFFECT_SF(nbBits,a);
#define SUB(nbBits,a,nbBitsSrc,b) a=a-b;AFFECT_ZF(a);AFFECT_CF(b<a);AFFECT_SF(nbBits,a);

// TODO: should affects OF, SF, ZF, AF, and PF
#define INC(nbBits,a) a=a+1;AFFECT_ZF(a)
#define DEC(nbBits,a) a=a-1;AFFECT_ZF(a)

#define JE(label) if (m.ZF) GOTOLABEL(label)
#define JZ(label) JE(label)

#define JNE(label) if (!m.ZF) GOTOLABEL(label)
#define JNZ(label) JNE(label)

#define JNB(label) if (!m.CF) GOTOLABEL(label)
#define JAE(label) JNB(label)
#define JNC(label) JNB(label)

#define JB(label) if (m.CF) GOTOLABEL(label)
#define JC(label) JB(label)
#define JNAE(label) JB(label)

#define JA(label) if (!m.CF && !m.ZF) GOTOLABEL(label)
#define JNBE(label) JA(label)

#define JS(label) if (m.SF) GOTOLABEL(label)
#define JNS(label) if (!m.SF) GOTOLABEL(label)

#define JNA(label) if (m.CF || m.ZF) GOTOLABEL(label)
#define JBE(label) JNA(label)

#define MOV(nbBits,dest,nbBitsSrc,src) dest = src
#define MOVZX(nbBits,dest,nbBitsSrc,src) dest = src
#define MOVSX(nbBits,dest,nbBitsSrc,src) if ISNEGATIVE(nbBitsSrc,src) { dest = ((-1 ^ (( 1 <<nbBitsSrc )-1)) | src ); } else { dest = src; }

// LEA - Load Effective Address
#define LEA(nbBits,dest,nbBitsSrc,src) dest = src

// MOVSx (DF FLAG not implemented)
#define MOVS(a,ecx) src=realAddress(m.esi,ds);dest=realAddress(m.edi,es); \
if (labs(((char *)dest)-((char *)src))<=a) { \
    for(int i=0;i<ecx;i++) {  \
        src=realAddress(m.esi,ds);dest=realAddress(m.edi,es); \
        memmove(dest,src,a);m.edi+=a;m.esi+=a; } \
} else { \
    memmove(dest,src,a*ecx);m.edi+=a*ecx;m.esi+=a*ecx; \
}

#define MOVSB MOVS(1,1)
#define MOVSW MOVS(2,1)
#define MOVSD MOVS(4,1)

#define REP_MOVS(b) MOVS(b,m.ecx)
#define REP_MOVSB REP_MOVS(1)
#define REP_MOVSW REP_MOVS(2)
#define REP_MOVSD REP_MOVS(4)

#define STOS(a) memcpy (realAddress(m.edi,es), &m.eax, a);m.edi+=a
#define STOSB STOS(1)
#define STOSW STOS(2)
#define STOSD STOS(4)

#define REP_STOSB for (int i=0;i<m.ecx;i++) { STOSB; }
#define REP_STOSW for (int i=0;i<m.ecx;i++) { STOSW; }
#define REP_STOSD for (int i=0;i<m.ecx;i++) { STOSD; }

#define LODS(a,b) memcpy (((db *)&m.eax)+b, realAddress(m.esi,ds), a);m.esi+=a
#define LODSB LODS(1,(is_little_endian()?0:3))
#define LODSW LODS(2,(is_little_endian()?0:2))
#define LODSD LODS(4,0)

#define REP_LODS(a,b) for (int i=0;i<m.ecx;i++) { LODS(a,b); }
#define REP_LODSB REP_LODS(1,(is_little_endian()?0:3))
#define REP_LODSW REP_LODS(2,(is_little_endian()?0:2))
#define REP_LODSD REP_LODS(4,0)

// JMP - Unconditional Jump
#define JMP(label) GOTOLABEL(label)
#define GOTOLABEL(a) goto a

#ifndef DEBUG
#define R(a) a
#else
#define R(a) printf("l:%d:%s\n",__LINE__,#a);a
#endif

#define LOOP(label) DEC(32,m.ecx);JNZ(label)

#define CLD m.DF=0
#define STD m.DF=1

void stackDump();
void hexDump (void *addr, int len);
void asm2C_INT(int a);

#define INT(a) asm2C_INT(a);TESTJUMPTOBACKGROUND

#define TESTJUMPTOBACKGROUND  if (m.jumpToBackGround) { CALL(moveToBackGround) };

void asm2C_OUT(int16_t address, int data);
#define OUT(a,b) asm2C_OUT(a,b)
int8_t asm2C_IN(int16_t data);
#define IN(a,b) a = asm2C_IN(b);TESTJUMPTOBACKGROUND

#define STI // TODO: STI not implemented
#define PUSHF
#define POPF
#define NOP

#define CALL(label) \
if (setjmp(jmpbuffer) == 0) { \
    PUSH(?,jmpbuffer); \
    JMP(label); \
}

#define RET POP(x,jmpbuffer);longjmp(jmpbuffer, 0);

typedef struct __attribute__((packed)) Mem {
db var1[3];
dw var2[3];
dd var3[4];
db var4[100];
dd var5[10];
db dummy1[4096];

dd eax;
dd ebx;
dd ecx;
dd edx;
dd esi;
dd edi;
dd ebp;
dd esp;
dw cs;
dw ds;
dw es;
dw fs;
dw gs;
dw ss;
_Bool CF;
_Bool ZF;
_Bool DF;
_Bool SF;
_Bool jumpToBackGround;
_Bool executionFinished;
db exitCode;
db vgaPalette[256*3];
int stackPointer;
dd stack[STACK_SIZE];
dd selectorsPointer;
dd selectors[NB_SELECTORS];
dd heapPointer;
db heap[HEAP_SIZE];
db vgaRam0[VGARAM_SIZE];
db vgaRam[VGARAM_SIZE];
db vgaRam2[VGARAM_SIZE];
char *path;
} Memory;
int program();
#endif



#define sizeOfeax 4
#define sizeOfebx 4
#define sizeOfecx 4
#define sizeOfedx 4
#define sizeOfesi 4
#define sizeOfedi 4
#define sizeOfebp 4
#define sizeOfesp 4
#define sizeOfcs 2
#define sizeOfds 2
#define sizeOfes 2
#define sizeOffs 2
#define sizeOfgs 2
#define sizeOfss 2
#define sizeOfvar1  1
#define sizeOfvar2  2
#define sizeOfvar3  4
#define sizeOfvar4  1
#define sizeOfvar5  4
