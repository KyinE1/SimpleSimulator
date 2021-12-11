#include <iostream>
#include <iomanip>
#include "sml.h"
#include "simplesim.h"

/** Simple simulator to emulate the CPU.
 * 
 * @param memory: To store addresses.
 * @param accumulator: Register for arithmetic.
 * @param instruction_counter: The next instruction.
 * @param instruction_register: A copy of the current instruction.
 * @param operand: Half of the word to hold the MSB of instruction_register. 
 */

/** Set memory array to 7777. */
simplesim::simplesim() {
	for (int i = 0; i < 100; i++)
		memory[i] = 7777;
}

/** Process the instructions.
 *
 * @return: False if there's no valid input. 
 */
bool simplesim::load_program() {
	int count = 0;
	int instruction;

	while (std::cin >> instruction && instruction != -99999) {
		if (not_valid_word(instruction)) {
			std::cout << "*** ABEND: pgm load: invalid word. ***\n";
			return false;
		}

		if (count >= 100) {
			std::cout << "*** ABEND: pgm load: pgm too large. ***\n";
			return false;
		}

		memory[count] = instruction;
		count++;
	}

	return true;
}


/** Used for verifying the specified range of the word. 
 *
 * @param word: Four digit variable. 
 * @return: Whether the word exceeds the range. 
 */
bool simplesim::not_valid_word(int word) const {
	return (word < -9999 || word > 9999);
}

/** Fetch the instruction from memory and execute.
 *
 * @param done: Flag.
 * @return: If false, dump the contents of the memory array. 
 */
void simplesim::execute_program() {
	bool done = false;

	// If valid memory location (0-99), instruction_register will load with the word 
	// and split the instruction_register by placing leftmost two digits into 
	// operation_code register and rightmost two digits in the operand register).
	while (!done) {
		if (instruction_counter >= 100) {
			std::cout << "*** ABEND: addressability error. ***\n";
			return;
		}

		// Instruction fetch.
		instruction_register = memory[instruction_counter];

		// Word split (leftmost 2 digits, rightmost 2 digits).
		operation_code = instruction_register / 100;
		operand = instruction_register % 100;

		// Instruction execute.
		switch (operation_code) {
			case READ: {
				int word;
				std::cin >> word;

				if (word >= -9999 && word <= 9999) {
					memory[operand] = word;
					std::cout << "READ: " << std::internal << std::showpos << std::setfill('0') << std::setw(5) << memory[operand] << std::endl;
				}

				else {
					std::cout << "*** ABEND: illegal input ***\n";
					return;
				}
				
				break;
			}
			
			// Print the word in memory from location of operand. 	
			case WRITE: {
				std::cout << std::internal << std::showpos << std::setfill('0') << std::setw(5) << memory[operand] << std::endl;
				break;
			}
			
			case LOAD: {
				accumulator = memory[operand];
				break;
			}
			
			case STORE: {
				memory[operand] = accumulator;
				break;
			}
			
			case ADD: {
				int result = accumulator + memory[operand];

				if (result < -9999) {
					std::cout << "*** ABEND: underflow. ***\n";
					return;
				}

				if (result > 9999) {
					std::cout << "*** ABEND: overflow. ***\n";
					return;
				}

				accumulator = accumulator + memory[operand];
				break; 
			}
			
			case SUBTRACT: {
				int result = accumulator - memory[operand];

				if (result < -9999) {
					std::cout << "*** ABEND: underflow. ***\n";
					return;
				}

				if (result > 9999) {
					std::cout << "*** ABEND: overflow. ***\n";
					return;
				}

				accumulator = accumulator - memory[operand];
				break;
			}
			
			case MULTIPLY: {
				int result = accumulator * memory[operand];

				if (result < -9999) {
					std::cout << "*** ABEND: underflow. ***\n";
					return;
				}

				if (result > 9999) {
					std::cout << "*** ABEND: overflow. ***\n";
					return;
				}

				accumulator = accumulator * memory[operand];
				break;
			}
			
			case DIVIDE: {
				if (memory[operand] == 0) {
					std::cout << "*** ABEND: attempted division by 0. ***\n";
					return;
				}

				accumulator = accumulator / memory[operand];
				break;
			}
			
			case BRANCH: {
				instruction_counter = operand;
				break;
			}
			
			case BRANCHNEG: {
				if (accumulator < 0)
					instruction_counter = operand;
				else
					instruction_counter++;
				break;
			}
			
			case BRANCHZERO: {
				if (accumulator == 0)
					instruction_counter = operand;
				else
					instruction_counter++;
				break;
			}
			
			case HALT: {
				std::cout << "*** Simplesim execution terminated. ***\n";
				done = true;
				break;
			}
			
			default: {
				std::cout << "*** ABEND: invalid opcode. ***\n";
				return;
			}
		}

		if ((operation_code < BRANCH || operation_code > BRANCHNEG) && !done)
			instruction_counter++;
	}
}


/** Dump the content of the simplesim's registers and memory addresses. */
void simplesim::dump() const {
	int rows = 0;

	// Print the registers. 
	std::cout << "\nREGISTERS:" << std::endl;

	std::cout << std::left << std::setfill(' ') << std::setw(24) << "accumulator:" << std::internal << std::showpos << std::setfill('0') << std::setw(5) << accumulator << std::endl;
	std::cout << std::left << std::setfill(' ') << std::setw(24) << "instruction_counter:" << std::right << std::noshowpos << std::setfill('0') << std::setw(2) << instruction_counter << std::endl;
	std::cout << std::left << std::setfill(' ') << std::setw(24) << "instruction_register:" << std::internal << std::showpos << std::setfill('0') << std::setw(5) << instruction_register << std::endl;
	std::cout << std::left << std::setfill(' ') << std::setw(24) << "operation_code:" << std::internal << std::noshowpos << std::setfill('0') << std::setw(2) << operation_code << std::endl;
	std::cout << std::left << std::setfill(' ') << std::setw(24) << "operand:" << std::internal << std::noshowpos << std::setfill('0') << std::setw(2) << operand << std::endl;

	// Print the memory addresses.
	std::cout << "\nMEMORY:\n";
	for (int columns = 0; columns != 10; columns++) {
		if (columns != 0)
			std::cout << std::setw(6) << std::noshowpos <<std::setfill(' ') << columns;
		else 
			std::cout << std::setw(8) << std::noshowpos <<std::setfill(' ') << columns;
	}

	for (int i = 0; i < 100; i++) {
		if (i % 10 == 0) {
			if (rows != 0)
				std::cout << std::endl << std::noshowpos << rows;				
			else
				std::cout << std::endl << std::noshowpos << " " << rows;
				
			rows = rows + 10;
		}
		
		std::cout << " "; std::cout << std::internal << std::setfill(' ') << std::setfill('0') << std::showpos << std::setw(5) << memory[i];
	}

	std::cout << std::endl;
}
