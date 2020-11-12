#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>

#define NORM 0
#define DNORM 1
#define SPEC 2
#define BIAS 127


/*
    Declare a "typedef struct {} flt;" to contain data for a float
    The data should include:
        An integer for the sign
            (1 for positive, -1 for negative)
        An integer for the exponent value
            (should be bit value to integer minus BIAS or -126 for denormalized)
        A float to contain the value of the mantissa
            (Actual float value extracted from the binary value)
        An integer to contain the mode using the defines above
            (NORM, DNORM and SPEC)
    Example after processing: -15.375 = 1 10000010 11101100000000000000000
        sign = -1
        exp = 3
        man = 0.9218750000
        mode = NORM
*/
typedef struct flt {
    int sign;
    int exp;
    float man;
    int mode;
}FLT;
/*
    Write a function get_flt_bits_int to return an integer with the
    bits copied from a float.
    Example:
        for f = -15.375,
        the bits of int n = 11000001011101100000000000000000
    Look at the slides and code from the float lectures and use the
    indirection trick.  This can easily be done in one line of code.
    The function should accept a float and return an int.
*/
/* Gets the bit representation of a float as an integer. */
int get_flt_bits_int(float f) {
    return *(int *)(&f);
}

/* Gets the first bit and returns 1 if neg and 0 otherwise. */
char get_flt_sign_char(float f) {
    return (get_flt_bits_int(f) >> 31 & 1 == 1) ? '1' : '0';
}

/* Gets the sign bit and returns -1 if neg and 1 otherwise. */
int get_flt_sign_val(float f) {
    return (get_flt_bits_int(f) >> 31 & 1 == 1) ? -1 : 1;
}

/* Gets the exponent part of the float and returns it as a string. */
char * get_flt_exp_str(float f) {
    int n = get_flt_bits_int(f) >> 23;
    char *bits = malloc(sizeof(char) * 8);
    int i;
    for(i = 1; i <= 8; i++) {
        bits[8-i] = (n & 1) + '0';
        n = n >> 1;
    } bits[i] = '\0';
    return bits;
}

/* Gets the value of the exponent part. */
int get_flt_exp_val(float f) {
    int n = get_flt_bits_int(f) >> 23;
    int exp = 0;
    int i;
    for(i = 0; i < 8; i++) {
        if(n & 1 == 1)
            exp += pow(2,i);
        n = n >> 1;
    }
    return exp - BIAS;
}

/* Gets the mode of the float. */
int get_flt_exp_mode(float f) {
    return (get_flt_exp_val(f) == -126) ? DNORM : (get_flt_exp_val(f) == 127) ? SPEC : NORM; 
}

/* Gets the mantissa part of the float and returns it as a string. */
char * get_flt_man_str(float f) {
    int n = get_flt_bits_int(f);
    char *bits = malloc(sizeof(char) * 23);
    int i;
    for(i = 1; i <= 23; i++) {
        bits[23-i] = (n & 1) + '0';
        n = n >> 1;
    } bits[i] = '\0';
    return bits;
}


/*
    Write a function to return a float containing the
    actual float value of the mantissa of a float.  You
    should call get_flt_bits_int to get the bits in an int
    and return the int with the mantissa value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the mantissa bits are 11101100000000000000000
            the actual value of the mantissa is 0.9218750000
    The function should accept a float and return an int.
*/
/* Gets the value of the matissa part as a float. */
float get_flt_man_val(float f) {
    int n = get_flt_bits_int(f);
    float man = 0.0;
    int i;
    for(i = 23; i > 0; i--) {
        if(n & 1 == 1)
            man += pow(2, -i);
        n = n >> 1;
    }
    return man;
}

/*
    Write a function to return a string containing the
    actual binary value of a float in a char array.  You
    should call get_flt_sign_char, get_flt_exp_str and
    get_flt_man_str to get the bits in an char and two
    strings and return the concatenated string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            The sign is '1'
            the exponent is "10000010"
            and the mantissa bits are "11101100000000000000000"
            The string should be formatted as:
                "1 10000010 11101100000000000000000" to clearly
                 1 10000010 1110110000000000000000
                            11101100000000000000000
                separate the 3 parts.
    The function should accept a float and return a string.
*/

char * get_flt_bits_str(float f) {
    
    char * bits = malloc(sizeof(char) * 35);
    int index = -1;
    int i;

    bits[++index] = get_flt_sign_char(f);
    bits[++index] = ' ';

    char * exp = get_flt_exp_str(f);
    for(i = 0; i < 8; i++)
        bits[++index] = exp[i];
    bits[++index] = ' ';
    free(exp);

    char * man = get_flt_man_str(f);
    for(i = 0; i < 23; i++)
        bits[++index] = man[i];
    bits[++index] = '\0';
    free(man);

    return bits;
}


/*
    Write a function to separate the parts of a float
    into a flt struct as described above.  You should
    call get_flt_sign_val, get_flt_exp_mode,
    get_flt_exp_val and get_flt_man_val.
    Hint:  make sure to set exponent to -126 for
    DNORM mode.
*/




/*
    Write a function to print a flt struct to screen.
    It should accept a flt struct and return nothing.
    Hint: Use if statement to print mode.
*/




/*
    Write a function to get the actual float value back
    out of a flt struct.
    Hints:
        The float value produced will depend on the mode.
        To set a float to infinity use the math library constant INFINITY
        To set a float to not-a-number use the math library constant NAN
        Check the slides and text for conditions for NORN, DNORM and SPEC
        You need to return (sign) * M * 2^e
*/




/*
    Write a main function that calls an prints results for
    each function when completed.
    get_flt_sign_char
    get_flt_sign_val

    get_flt_exp_str
    get_flt_exp_val

    get_flt_man_str
    get_flt_man_val

    get_flt_bits_str

    get_flt_val_flt
    print_flt

    get_flt_bits_val
*/
int main(){

    float f = -15.375;

    printf("get_flt_sign_char(%f) = %c\n", f, get_flt_sign_char(f));
    printf("get_flt_sign_val(%f) = %d\n", f, get_flt_sign_val(f));
    printf("get_flt_exp_str(%f) = %s\n", f, get_flt_exp_str(f));
    printf("get_flt_exp_val(%f) = %d\n", f, get_flt_exp_val(f));
    printf("get_flt_exp_mode(%f) = %d\n", f, get_flt_exp_mode(f));
    printf("get_flt_man_str(%f) = %s\n", f, get_flt_man_str(f));
    printf("get_flt_man_val(%f) = %f\n", f, get_flt_man_val(f));
    printf("get_flt_bits_str(%f) = %s\n", f, get_flt_bits_str(f));

}
