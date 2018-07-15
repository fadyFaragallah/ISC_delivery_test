#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

void portF_init(void);
void systick_init(void);
void portA_init(void);
char readChar(void);
void printChar(char c);
void printString(char* string);
void ADC_init(void);
int read_temp(void);
void portF_init(void);
int charToInt(char);
void printNum(uint32_t);
#endif /* FUNCTIONS_H_ */
