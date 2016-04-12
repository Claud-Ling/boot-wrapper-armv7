
#ifndef __MONZA_IO__
#define __MONZA_IO__

#include <io.h>

#define __IO_ADDR(a) (((a) & 0x1FFFFFFF) | 0xF0000000)	/* SX6 I/O Addr */
#define TridSetBits(RegValue, StartBit, Bits, NewValue)					\
		do{									\
			(RegValue) &= ~(((0x1<<(Bits))-1)<<(StartBit));         	\
			(RegValue) |= ((NewValue)&((0x1<<(Bits))-1))<<(StartBit);	\
		}while(0)

#define TridGetBits(RegValue, StartBit, Bits) (((RegValue) >> (StartBit)) & ((0x1<<(Bits))-1))

#define MWriteReg(type, addr, val, mask)				\
		do{							\
			int __tmp = _ReadReg##type((unsigned int)addr);	\
			__tmp = (__tmp & (~(mask))) | ((val) & (mask));	\
			_WriteReg##type((unsigned int)addr, __tmp);	\
		}while(0)
#define MWriteRegByte(addr, val, mask)  MWriteReg(Byte, addr, val, mask)
#define MWriteRegHWord(addr, val, mask) MWriteReg(HWord, addr, val, mask)
#define MWriteRegWord(addr, val, mask)  MWriteReg(Word, addr, val, mask)

static inline  unsigned char _ReadRegByte(unsigned int addr)
        {return readb(__IO_ADDR(addr));}

static inline  unsigned short _ReadRegHWord(unsigned int addr)
        {return readw(__IO_ADDR(addr));}

static inline  unsigned int _ReadRegWord(unsigned int addr)
        {return readl(__IO_ADDR(addr));}

static inline  void  _WriteRegByte(unsigned int addr, unsigned char val)
        {writeb(val,__IO_ADDR(addr));}

static inline  void  _WriteRegHWord(unsigned int addr, unsigned short val)
        {writew(val,__IO_ADDR(addr));}

static inline  void  _WriteRegWord(unsigned int addr, unsigned int val)
        {writel(val,__IO_ADDR(addr));}

#define ReadReg(type, a)	_ReadReg##type((unsigned int)(a))
#define WriteReg(type, a, v)	_WriteReg##type((unsigned int)(a), v)
#define ReadRegByte(a)		ReadReg(Byte, a)
#define ReadRegHWord(a) 	ReadReg(HWord, a)
#define ReadRegWord(a)		ReadReg(Word, a)
#define WriteRegByte(a, v)	WriteReg(Byte, a, v)
#define WriteRegHWord(a, v)	WriteReg(HWord, a, v)
#define WriteRegWord(a, v)	WriteReg(Word, a, v)
#endif
