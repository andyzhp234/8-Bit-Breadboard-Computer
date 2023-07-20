#include <LiquidCrystal.h>

// List out all Basic Assembly Instructions
void listCommands() {
  String OPCODE[] = {
    "0000 : NOP", // No Operations
    "0001 : LDA", // Load from memory to A
    "0010 : ADD", // Add from memory with current A and store in A
    "0011 : SUB", // Same above but sub
    "0100 : STA", // Store value in Register A to Memory
    "0101 : LDI", // Load Immediate values To A
    "0110 : JMP", // JMP to specific lines
    "0111 : JC",  // Jump when Carry flag is on
    "1000 : JZ",  // Jump when zero flag is on
    "1110 : OUT", // Output to 7-segment LED
    "1111 : HLT", // Halt the program
  };
}

String getOpcode(String opcode) {
  if (opcode == "NOP") {
    return "0000";
  } else if (opcode == "LDA") {
    return "0001";
  } else if (opcode == "ADD") {
    return "0010";
  } else if (opcode == "SUB") {
    return "0011";
  } else if (opcode == "STA") {
    return "0100";
  } else if (opcode == "LDI") {
    return "0101";
  } else if (opcode == "JMP") {
    return "0110";
  } else if (opcode == "JC") {
    return "0111";
  } else if (opcode == "JZ") {
    return "1000";
  } else if (opcode == "OUT") {
    return "1110";
  } else if (opcode == "HLT") {
    return "1111";
  } else {
    return "Invalid opcode";
  }
}

void signalWrite() {
  digitalWrite(10, LOW);
  delay(500);
  digitalWrite(10, HIGH);
  delay(1000);
}

void writeMemoryAddress(String location) {
  analogWrite(A0, (location[0] - '0') * 255);
  analogWrite(A1, (location[1] - '0') * 255);
  analogWrite(A2, (location[2] - '0') * 255);
  analogWrite(A3, (location[3] - '0') * 255);
  delay(500);
}

void writeCode(String location, String opcode, String values) {
  // first go to the correct memory address
  writeMemoryAddress(location);

  // opcode
  String opcodeStr = getOpcode(opcode);
  digitalWrite(9, opcodeStr[0] - '0');
  digitalWrite(8, opcodeStr[1] - '0');
  digitalWrite(7, opcodeStr[2] - '0');
  digitalWrite(6, opcodeStr[3] - '0');

  // values
  digitalWrite(5, values[0] - '0');
  digitalWrite(4, values[1] - '0');
  digitalWrite(3, values[2] - '0');
  digitalWrite(2, values[3] - '0');

  signalWrite();
}

void writeDirectMemoryValues(String location, String values) {
  writeMemoryAddress(location);

  // values
  digitalWrite(9, values[0] - '0');
  digitalWrite(8, values[1] - '0');
  digitalWrite(7, values[2] - '0');
  digitalWrite(6, values[3] - '0');
  digitalWrite(5, values[4] - '0');
  digitalWrite(4, values[5] - '0');
  digitalWrite(3, values[6] - '0');
  digitalWrite(2, values[7] - '0');
  
  signalWrite();
}

void loopBackForth() {
  writeCode("0000", "OUT", "0000");
  writeCode("0001", "ADD", "1111");
  writeCode("0010", "JC",  "0100");
  writeCode("0011", "JMP", "0000");
  writeCode("0100", "SUB", "1111");
  writeCode("0101", "OUT", "0000");
  writeCode("0110", "JZ",  "0000");
  writeCode("0111", "JMP", "0100");
  writeCode("1000", "HLT", "0000");

  writeCode("1001", "NOP", "0000");
  writeCode("1010", "NOP", "0000");
  writeCode("1011", "NOP", "0000");
  writeCode("1100", "NOP", "0000");
  writeCode("1101", "NOP", "0000");
  writeCode("1110", "NOP", "0000");

  writeDirectMemoryValues("1111", "00000001");
}

void multiply(int x, int y) {
  writeCode("0000", "LDA", "1110");
  writeCode("0001", "SUB", "1100");
  writeCode("0010", "JC",  "0110");
  writeCode("0011", "LDA", "1101");
  writeCode("0100", "OUT", "0000");
  writeCode("0101", "HLT", "0000");
  writeCode("0110", "STA", "1110");
  writeCode("0111", "LDA", "1101");
  writeCode("1000", "ADD", "1111");
  writeCode("1001", "STA", "1101");
  writeCode("1010", "JMP", "0000");
  writeCode("1011", "NOP", "0000");
  writeDirectMemoryValues("1100", "00000001");
  writeDirectMemoryValues("1101", "00000000");

  String xStr = String(x, BIN);
  String yStr = String(y, BIN);
  while (xStr.length() < 8) { xStr = "0" + xStr;}
  while (yStr.length() < 8) { yStr = "0" + yStr;}

  writeDirectMemoryValues("1110", xStr);
  writeDirectMemoryValues("1111", yStr);
}

void helloWorld() {
  writeDirectMemoryValues("1111", "01001000"); // H
  writeDirectMemoryValues("1110", "01000101"); // E
  writeDirectMemoryValues("1101", "01001100"); // L
  writeDirectMemoryValues("1100", "01001111"); // O

  writeCode("0000", "LDA", "1111"); // H
  writeCode("0001", "OUT", "0000"); //

  writeCode("0010", "LDA", "1110"); // E
  writeCode("0011", "OUT", "0000"); //

  writeCode("0100", "LDA", "1101"); // L
  writeCode("0101", "OUT", "0000"); // 
  writeCode("0110", "OUT", "0000"); // 

  writeCode("0111", "LDA", "1100"); // O
  writeCode("1000", "OUT", "0000"); // 

  writeCode("1001", "HLT", "0000");
}

// Fibonacci Sequence
void fibonacci_sequence() {
  writeCode("0000", "LDI", "0001");
  writeCode("0001", "STA", "1110");
  writeCode("0010", "LDI", "0000");
  writeCode("0011", "STA", "1111");
  writeCode("0100", "OUT", "0000");
  writeCode("0101", "LDA", "1110");
  writeCode("0110", "ADD", "1111");
  writeCode("0111", "STA", "1110");
  writeCode("1000", "OUT", "0000");
  writeCode("1001", "LDA", "1111");
  writeCode("1010", "ADD", "1110");
  writeCode("1011", "JC", "1101");
  writeCode("1100", "JMP", "0011");
  writeCode("1101", "HLT", "0000");
  writeDirectMemoryValues("1110", "00000000"); // x
  writeDirectMemoryValues("1111", "00000001"); // y
}

// RAM is limited
// Can only write 16 Lines of code, including storages of temp values....
void mainProgram() {
  // multiply(2, 10);
  // loopBackForth();
  // helloWorld();
  fibonacci_sequence();
}

// Analog A0, A1, A2, A3 Will be used to represent memory Values
// Digital Pin 10 will be used to signal a write.
// Digital 9, 8, 7, 6, 5, 4, 3, 2 will be used as values that will be write into RAM
void setup() {
  Serial.begin(57600);
  // Set Address Pins
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  // Set Value Pins
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  // Write Signal
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  // write the main program into RAM
  mainProgram();
}

void loop() {}
