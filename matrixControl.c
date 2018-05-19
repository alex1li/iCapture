#include "matrixControl.h"


/*This file containts functions needed to initalizes and control the 8x8 LED Board*/

struct LetterA ABar; // struct for each letter
struct LetterB BBar;
struct LetterC CBar;
struct LetterD DBar;
struct LetterD EBar;

uint8_t ARow = 0; // keeps track of which row on LED the A column is on
uint8_t BRow = 0; // keeps track of which row on LED the B column is on
uint8_t CRow = 0; // keeps track of which row on LED the C column is on
uint8_t DRow = 0; // keeps track of which row on LED the D column is on
uint8_t ERow = 0; // keeps track of which row on LED the E column is on.

uint8_t rowValues[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // keeps track of LEDs on in each row

void initStructs() {
	
	/* This function initalizes the structs created for each letter. 
		 It initally requires one click from the IClicker in order to
		 increment the bar for each letter.
	*/
	ABar.capacity = 1; 
	ABar.value = 0;
	BBar.capacity = 1; 
	BBar.value = 0;
	CBar.capacity = 1; 
	CBar.value = 0;
	DBar.capacity = 1; 
	DBar.value = 0;
	EBar.capacity = 1; 
	EBar.value = 0;
	
}

void initBoard() {
	
	/*
	This function initializes the I2C registers needed to write to the
	LED matrix.
	
	*/
		Register_Write2(0x21, 0x00);
    Register_Write2(0x81, 0x00);
    Register_Write2(0xE7, 0x00);
	
}

void clearBoard() {
	
	/*
	This function clears the LED matrix by writing to the 
	appropriate registers. It also resets the global variables
	rowValues[], ARow, and the structs for each bar.
	*/
	
	  Register_Write2(0x00, 0x00);
		Register_Write2(0x02, 0x00);
   	Register_Write2(0x04, 0x00);
		Register_Write2(0x06, 0x00);
		Register_Write2(0x08, 0x00);
		Register_Write2(0x0A, 0x00);
		Register_Write2(0x0C, 0x00);
		Register_Write2(0x0E, 0x00);
	 
	  rowValues[0] = 0x00; // all rows do not have any LEDs lighted up
	  rowValues[1] = 0x00;
	  rowValues[2] = 0x00;
	  rowValues[3] = 0x00;
	  rowValues[4] = 0x00;
	  rowValues[5] = 0x00;
	  rowValues[6] = 0x00;
	  rowValues[7] = 0x00;
	
		ARow = 0; // each letter is on row 0
		BRow = 0;
		CRow = 0;
		DRow = 0;
		ERow = 0;
		
		ABar.capacity = 1; // capacity is reset to 1
		BBar.capacity = 1;
		CBar.capacity = 1;
		DBar.capacity = 1;
		EBar.capacity = 1;
}

void divideBoard() {
	
	/*
	This function takes all the bars on the current matrix
	and divides them in half. It also multiplies the 
	capacity of each bar by two, i.e. it requires 
	twice as many clicks to increment a bar by one dot after
	the function is called.
	*/
	
	
	int ACount = ARow / 2; // divide bar in half
	int BCount = BRow / 2;
	int CCount = CRow / 2;
	int DCount = DRow / 2;
	int ECount = ERow / 2;
	

	clearBoard();
	
	for(int a = 0; a < ACount; a++) {
		ABar.value = ABar.capacity; 
		ABarIncrement(); // increment half the bar
		
	}
	
	for(int b = 0; b < BCount; b++) {
		
		BBar.value = BBar.capacity;
		BBarIncrement();
		
	}
	
	for(int c = 0; c < CCount; c++) {
		
		CBar.value = CBar.capacity;
		CBarIncrement();
		
	}
	
	for(int d = 0; d < DCount; d++) {
		
		DBar.value = DBar.capacity;
		DBarIncrement();
	}
	
	for(int e = 0; e < ECount; e++) {
		EBar.value = EBar.capacity;
		EBarIncrement();
	}
	
	ABar.capacity *= 2; // double the capacity
	BBar.capacity *= 2;
	CBar.capacity *= 2;
	DBar.capacity *= 2;
	EBar.capacity *= 2;
	
}

void ABarIncrement() {
	
	/*
	This function increments the column allocated on the 
	LED matrix by one dot if the number of clicks required 
	to increment matches the number of clicks seen.
	
	*/
	
	if(ABar.value < ABar.capacity) {
		return; // do not increment if we have not seen the required clicks
	}
	
	if(ARow == 8) {
		divideBoard(); // divide the board when we reach the top
		return;
		
	}
	
	uint8_t nextRow = rowValues[ARow];
	uint8_t newValue = nextRow | AIncrement; 
	rowValues[ARow] = newValue; // update the current row
	uint8_t hexRow = 0x00;
	
	for(int x = 0; x < ARow; x++) {
		hexRow += 0x02; // calculate the hex value
	}
	Register_Write2(hexRow, newValue); // write to row with appropriate value
	
  ARow = ARow +1;
	
	ABar.value = 0;
}


void BBarIncrement() {
	
		/*
	This function increments the column allocated on the 
	LED matrix for B answers by one dot if the number of clicks required 
	to increment matches the number of clicks seen.
	*/
	
	
	if(BBar.value < BBar.capacity) {
		return; // do not increment if we have not seen the required clicks
	}
	
	if(BRow == 8) {
		divideBoard();  // divide the board when we reach the top
		return;
	}
	
 	uint8_t nextRow = rowValues[BRow];
	uint8_t newValue = nextRow | BIncrement;
	rowValues[BRow] = newValue; // update the current row
	uint8_t hexRow = 0x00;
	
	for(int x = 0; x < BRow; x++) {
		hexRow += 0x02; // calculate the hex value
	}
	Register_Write2(hexRow, newValue); // write to row again with appropriate value

  BRow = BRow + 1;
		
	BBar.value = 0;
	
}

void CBarIncrement() {
	
			/*
	This function increments the column allocated on the 
	LED matrix for C answers by one dot if the number of clicks required 
	to increment matches the number of clicks seen.
	*/
	
	if(CBar.value < CBar.capacity) {
		return; // do not increment if we have not seen the required clicks
	}
	
	if(CRow == 8) {
		divideBoard(); // divide the board when we reach the top
		return;
	}
	
	
 	uint8_t nextRow = rowValues[CRow];
	uint8_t newValue = nextRow | CIncrement;
	rowValues[CRow] = newValue;  // update the current row
	uint8_t hexRow = 0x00;
	
	for(int x = 0; x < CRow; x++) {
		hexRow += 0x02; // calculate the hex value
	} 
	Register_Write2(hexRow, newValue); // write to row again with appropriate value
	
	
  CRow = CRow + 1;
	CBar.value = 0;
}

void DBarIncrement() {
	
			/*
	This function increments the column allocated on the 
	LED matrix for D answers by one dot if the number of clicks required 
	to increment matches the number of clicks seen.
	*/
	
	if(DBar.value < DBar.capacity) {
		return;  // do not increment if we have not seen the required clicks
	} 
	
	if(DRow == 8) { 
		divideBoard();  // divide the board when we reach the top
		return;
	}
 	
 	uint8_t nextRow = rowValues[DRow];
	uint8_t newValue = nextRow | DIncrement;
	rowValues[DRow] = newValue;  // update the current row
	uint8_t hexRow = 0x00;
	
	for(int x = 0; x < DRow; x++) {
		hexRow += 0x02;  // calculate the hex value
	}
	Register_Write2(hexRow, newValue); // write to row again with appropriate value
	
	
  DRow = DRow + 1;
	
	DBar.value = 0;

}

void EBarIncrement() {
	
	/*
	This function increments the column allocated on the 
	LED matrix for E answers by one dot if the number of clicks required 
	to increment matches the number of clicks seen.
	*/
	
	
	if(EBar.value < EBar.capacity) {
		return; // do not increment if we have not seen the required clicks
	}
	
	if(ERow == 8) {
		divideBoard(); // divide the board when we reach the top
		return;
	}
 	
 	
 	uint8_t nextRow = rowValues[ERow];
	uint8_t newValue = nextRow | EIncrement;
	rowValues[ERow] = newValue; // update the current row
	uint8_t hexRow = 0x00;
	
	for(int x = 0; x < ERow; x++) {
		hexRow += 0x02;  // calculate the hex value
	}
	Register_Write2(hexRow, newValue);  // write to row again with appropriate value
	
	
  ERow = ERow + 1;
	EBar.value = 0;

}

void writeA() {
	
	/*
	This function writes the registers necessary
	to display an "A" on the LED matrix that we 
	designed.
	*/
	
	Register_Write2(0x00, 0xC0);
	Register_Write2(0x02, 0xC0);
	Register_Write2(0x04, 0xC0);
	Register_Write2(0x06, 0xC0);
	Register_Write2(0x08, 0xff);
	Register_Write2(0x0A, 0xC0);
	Register_Write2(0x0C, 0xC0);
	Register_Write2(0x0E, 0x3f);
	
}


	
	
void writeB() {
	
		
	/*
	This function writes the registers necessary
	to display a "B" on the LED matrix that we 
	designed.
	*/
	
	
	Register_Write2(0x00, 0x7f);
	Register_Write2(0x02, 0xC0);
	Register_Write2(0x04, 0xC0);
	Register_Write2(0x06, 0xC0);
	Register_Write2(0x08, 0xff);
	Register_Write2(0x0A, 0xC0);
	Register_Write2(0x0C, 0xC0);
	Register_Write2(0x0E, 0x7f);
	
}

void writeC() {
	
		/*
	This function writes the registers necessary
	to display a "C" on the LED matrix that we 
	designed.
	*/
	
	Register_Write2(0x00, 0x3f);
	Register_Write2(0x02, 0xC0);
	Register_Write2(0x04, 0x40);
	Register_Write2(0x06, 0x40);
	Register_Write2(0x08, 0x40);
	Register_Write2(0x0A, 0x40);
	Register_Write2(0x0C, 0xC0);
	Register_Write2(0x0E, 0x3f);
	
}

void writeD() {
	
	/*
	This function writes the registers necessary
	to display a "D" on the LED matrix that we 
	designed.
	*/
	
	Register_Write2(0x00, 0x7f);
	Register_Write2(0x02, 0xC0);
	Register_Write2(0x04, 0xC0);
	Register_Write2(0x06, 0xC0);
	Register_Write2(0x08, 0xC0);
	Register_Write2(0x0A, 0xC0);
	Register_Write2(0x0C, 0xC0);
	Register_Write2(0x0E, 0x7f);
	
}

void writeE() {
	
	/*
	This function writes the registers necessary
	to display a "E" on the LED matrix that we 
	designed.
	*/
	
	Register_Write2(0x00, 0xff);
	Register_Write2(0x02, 0x40);
	Register_Write2(0x04, 0x40);
	Register_Write2(0x06, 0x40);
	Register_Write2(0x08, 0xff);
	Register_Write2(0x0A, 0x40);
	Register_Write2(0x0C, 0x40);
	Register_Write2(0x0E, 0xff);
	
}

void ABarPlus(){ //Increse the value of ABar struct. indicating a new answer received
	ABar.value +=1;
}

void BBarPlus(){ //Increse the value of ABar struct. indicating a new answer received
	BBar.value +=1;
}

void CBarPlus(){ //Increse the value of ABar struct. indicating a new answer received
	CBar.value +=1;
}

void DBarPlus(){ //Increse the value of ABar struct. indicating a new answer received
	DBar.value +=1;
}

void EBarPlus(){ //Increse the value of ABar struct. indicating a new answer received
	EBar.value +=1;
}

static int32_t Register_Write2 (uint8_t reg, uint8_t val) {	
	/*
	This function takes in the uint_8 parameters reg and val 
	and writes to the corresponding reg with value val
	*/
	
  uint8_t data[2];
  data[0] = reg;
  data[1] = val;
  ptrI2C->MasterTransmit(FXOS8700_I2C_ADDR, data, 2, false);
  while (ptrI2C->GetStatus().busy);
  if (ptrI2C->GetDataCount() != 2) { return -1; }

  return 0;
}
