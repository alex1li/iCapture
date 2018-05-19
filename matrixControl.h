#include "Driver_I2C.h"

#define EIncrement								0x80 
#define DIncrement 								0x01
#define CIncrement								0x04
#define BIncrement								0x10
#define AIncrement 								0x40



#ifndef FXOS8700_I2C_PORT
#define FXOS8700_I2C_PORT       0
#endif

#ifndef FXOS8700_I2C_ADDR
#define FXOS8700_I2C_ADDR       	0x70 // memory address of LED matrix
#endif

#define FXOS8700_ID             	0xC7

/* I2C Driver */
#define _I2C_Driver_(n)  Driver_I2C##n
#define  I2C_Driver_(n) _I2C_Driver_(n)
extern ARM_DRIVER_I2C    I2C_Driver_(FXOS8700_I2C_PORT);
#define ptrI2C         (&I2C_Driver_(FXOS8700_I2C_PORT))

struct LetterA {
	int value; // number of clicks seen
	int capacity; // number of clicks before incrementing
};

struct LetterB {
	int value;
	int capacity;
};

struct LetterC {
	int value;
	int capacity;
};

struct LetterD {
	int value;
	int capacity;
};


struct LetterE {
	int value;
	int capacity;
};

static int32_t Register_Write2 (uint8_t reg, uint8_t val);
void ABarIncrement();
void BBarIncrement();
void CBarIncrement();
void DBarIncrement();
void EBarIncrement();
void initBoard();
void clearBoard();
void writeA();
void writeB();
void writeC();
void writeD();
void writeE();
void divideBoard();
void initStructs();

void ABarPlus();
void BBarPlus();
void CBarPlus();
void DBarPlus();
void EBarPlus();

