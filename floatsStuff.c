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
    return (exp == 255) ? exp : exp - BIAS;
}

/* Gets the mode of the float. */
int get_flt_exp_mode(float f) {
    return (get_flt_exp_val(f) == -126) ? DNORM : (get_flt_exp_val(f) == 255) ? SPEC : NORM; 
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

/* Get the formatted float bit string. */
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

/* Creates a flt struct to represent the differnet parts of the float. */
struct flt get_flt_val_flt(float f) {

    struct flt flt;

    flt.sign = get_flt_sign_val(f);
    flt.exp = get_flt_exp_val(f);
    flt.man = get_flt_man_val(f);
    flt.mode = get_flt_exp_mode(f);

    return flt;
}

/* Gets the float number that coresponds to the flt struct. */
float get_flt_bits_val(struct flt flt) {
    
    switch (flt.mode) {

    case DNORM:
        /* Denormalized Form */
        return pow(-1,flt.sign) * flt.man * pow(2, -126);

    case SPEC:
        /* +/- ∞ */
        if (flt.man == 0)
            return(flt.sign == -1) ? -INFINITY : INFINITY;

        /* NaN */
        return (flt.sign == -1) ? -NAN : NAN;

    default:
        /* Normalized Form */
        return flt.sign * (1 + flt.man) * pow(2,flt.exp);
    }
}

/* Prints the flt struct to screen. */
void print_flt(struct flt flt) {

    printf("sign = %d\nexp = %d\nman = %f\n", flt.sign, flt.exp, flt.man);

    switch (flt.mode) {

    case DNORM:
        printf("mode = denormalized\n\n");
        break;

    case SPEC:
        printf("mode = special\n\n");
        break;

    default:
        printf("mode = normalized\n\n");
        break;
    }
}

int main(){

    float f;
    /* Read in user input. */
    printf("\nEnter a floating point number: ");
    scanf("%f", &f);
    /* Print out float info. */
    printf("f = %f\n\n", f);

    printf("sig = %c\n", get_flt_sign_char(f));
    printf("s = %d\n\n", get_flt_sign_val(f));

    printf("exp = %s\n", get_flt_exp_str(f));
    printf("e = %d\n\n", get_flt_exp_val(f));

    printf("man = %s\n", get_flt_man_str(f));
    printf("m = %f\n\n", get_flt_man_val(f));

    printf("bits = %s\n", get_flt_bits_str(f));

    struct flt flt = get_flt_val_flt(f);
    print_flt(flt);

    printf("\nff = %f\n\n", get_flt_bits_val(flt));
}
