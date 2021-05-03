/*
	simplesim.h
	Date Written: 2/12
	Created by: Kyin Edwards
*/

#ifndef SIMPLESIM_H
#define SIMPLESIM_H

class simplesim {
	private:
        int memory[100];
        int accumulator = 0;
        int instruction_counter = 0;
        int instruction_register = 0;
        int operation_code = 0;
        int operand = 0;
        bool not_valid_word(int) const;

	public:
        simplesim();
        bool load_program();
        void execute_program();
        void dump() const;
};

#endif