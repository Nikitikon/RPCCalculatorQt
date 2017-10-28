struct operands {
    double left;
    double right;
    char operatorType;
};

typedef struct operands operands;


program CALCULATOR_PROG {
        version CALCULATOR_VARS {
            
            double DIV(operands) = 1;
            double PM(double) = 2;
            double PROC(double) = 3;
            double ONETOX(double) = 4;
            double COSINUS(double) = 5;
            double SINUS(double) = 6;
            double MYSQRT(double ) = 7;
            double MYLN(double ) = 8;
            double MYEXP(double ) = 9;
            double MUL(operands) = 10;
            double SUB(operands) = 11;
            double ADD(operands) = 12;
            double EQU(operands) = 13;
            } = 1;
    } = 12344321;
