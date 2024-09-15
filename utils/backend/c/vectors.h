/*
 * vectors.h
 *
 * Description:
 * This header file defines a set of generic vector data structures and associated
 * functions for managing dynamic arrays of various data types and pointers in C.
 *
 * A vector is a dynamic array that can automatically grow in size as needed,
 * providing efficient storage and manipulation of elements. This implementation
 * includes vectors for fundamental types (int, float, double, char, unsigned char)
 * and for pointers to these types, allowing flexible management of dynamic data.
 *
 * Features:
 * - Dynamic resizing: Automatically increases the capacity of the vector when needed.
 * - Element management: Supports appending elements, deleting elements, and shrinking
 *   the vector to fit its current size.
 * - Display functions: Provides functions to print the contents of the vector for
 *   debugging and visualization.
 *
 * Usage:
 * - To use a vector, include this header file in your source file.
 * - Ensure that you include `stdio.h` and `stdlib.h` before including this header file
 *   as it relies on functions from these standard libraries.
 * - Create a vector instance using the provided `createVector` function.
 * - Use `appendVector` functions to add elements to the vector.
 * - When done, use `deleteVector` functions to free the allocated memory and clean up.
 * - Call `displayVector` functions to print the contents of the vector.
 *
 * Example:
 *
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include "vectors.h"
 *
 * int main() {
 *     vector_int v;
 *     createVectorInt(&v);
 *
 *     for (int i = 0; i < 10; i++) {
 *         appendVectorInt(&v, i);
 *     }
 *
 *     displayVectorInt(&v);
 *     deleteVectorInt(&v);
 *
 *     return 0;
 * }
 *
 * Note:
 * - Make sure to include `stdio.h` and `stdlib.h` in your source files before including
 *   this header file to avoid compilation errors.
 * - Ensure that any pointers stored in the vector are managed properly to avoid memory
 *   leaks or invalid memory access.
 * - This implementation uses `malloc`, `realloc`, and `free` for dynamic memory
 *   management.
 *
 * Author:
 * Srinath Gudi
 *
 * History:
 * - 13/9/2024 - Initial creation.
 */




#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>

 // Vector for int
typedef struct {
    int* array;
    int size;
    int capacity;
} vector_int;

void createVectorInt(vector_int* vector) {
    vector->array = (int*)malloc(sizeof(int));
    vector->size = 0;
    vector->capacity = 1;
}

void appendVectorInt(vector_int* vector, int data) {
    if (vector->capacity == vector->size) {
        vector->capacity *= 2;
        vector->array = (int*)realloc(vector->array, vector->capacity * sizeof(int));
    }
    vector->array[vector->size] = data;
    vector->size++;
}

void deleteVectorInt(vector_int* vector) {
    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

void shrinkToFitVectorInt(vector_int* vector) {
    if (vector->size != vector->capacity) {
        vector->array = (int*)realloc(vector->array, sizeof(int) * vector->size);
        vector->capacity = vector->size;
    }
}

void deleteElementVectorInt(vector_int* vector, int index) {
    for (int i = index; i < vector->size - 1; i++) {
        vector->array[i] = vector->array[i + 1];
    }
    vector->size--;
}

void displayVectorInt(vector_int* vector) {
    printf("{");
    for (int i = 0; i < vector->size; i++) {
        printf("%d", vector->array[i]);
        if (i != vector->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

// Vector for float
typedef struct {
    float* array;
    int size;
    int capacity;
} vector_float;

void createVectorFloat(vector_float* vector) {
    vector->array = (float*)malloc(sizeof(float));
    vector->size = 0;
    vector->capacity = 1;
}

void appendVectorFloat(vector_float* vector, float data) {
    if (vector->capacity == vector->size) {
        vector->capacity *= 2;
        vector->array = (float*)realloc(vector->array, vector->capacity * sizeof(float));
    }
    vector->array[vector->size] = data;
    vector->size++;
}

void deleteVectorFloat(vector_float* vector) {
    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

void shrinkToFitVectorFloat(vector_float* vector) {
    if (vector->size != vector->capacity) {
        vector->array = (float*)realloc(vector->array, sizeof(float) * vector->size);
        vector->capacity = vector->size;
    }
}

void deleteElementVectorFloat(vector_float* vector, int index) {
    for (int i = index; i < vector->size - 1; i++) {
        vector->array[i] = vector->array[i + 1];
    }
    vector->size--;
}

void displayVectorFloat(vector_float* vector) {
    printf("{");
    for (int i = 0; i < vector->size; i++) {
        printf("%f", vector->array[i]);
        if (i != vector->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

// Vector for double
typedef struct {
    double* array;
    int size;
    int capacity;
} vector_double;

void createVectorDouble(vector_double* vector) {
    vector->array = (double*)malloc(sizeof(double));
    vector->size = 0;
    vector->capacity = 1;
}

void appendVectorDouble(vector_double* vector, double data) {
    if (vector->capacity == vector->size) {
        vector->capacity *= 2;
        vector->array = (double*)realloc(vector->array, vector->capacity * sizeof(double));
    }
    vector->array[vector->size] = data;
    vector->size++;
}

void deleteVectorDouble(vector_double* vector) {
    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

void shrinkToFitVectorDouble(vector_double* vector) {
    if (vector->size != vector->capacity) {
        vector->array = (double*)realloc(vector->array, sizeof(double) * vector->size);
        vector->capacity = vector->size;
    }
}

void deleteElementVectorDouble(vector_double* vector, int index) {
    for (int i = index; i < vector->size - 1; i++) {
        vector->array[i] = vector->array[i + 1];
    }
    vector->size--;
}

void displayVectorDouble(vector_double* vector) {
    printf("{");
    for (int i = 0; i < vector->size; i++) {
        printf("%lf", vector->array[i]);
        if (i != vector->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

// Vector for char
typedef struct {
    char* array;
    int size;
    int capacity;
} vector_char;

void createVectorChar(vector_char* vector) {
    vector->array = (char*)malloc(sizeof(char));
    vector->size = 0;
    vector->capacity = 1;
}

void appendVectorChar(vector_char* vector, char data) {
    if (vector->capacity == vector->size) {
        vector->capacity *= 2;
        vector->array = (char*)realloc(vector->array, vector->capacity * sizeof(char));
    }
    vector->array[vector->size] = data;
    vector->size++;
}

void deleteVectorChar(vector_char* vector) {
    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

void shrinkToFitVectorChar(vector_char* vector) {
    if (vector->size != vector->capacity) {
        vector->array = (char*)realloc(vector->array, sizeof(char) * vector->size);
        vector->capacity = vector->size;
    }
}

void deleteElementVectorChar(vector_char* vector, int index) {
    for (int i = index; i < vector->size - 1; i++) {
        vector->array[i] = vector->array[i + 1];
    }
    vector->size--;
}

void displayVectorChar(vector_char* vector) {
    printf("{");
    for (int i = 0; i < vector->size; i++) {
        printf("'%c'", vector->array[i]);
        if (i != vector->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

// Vector for unsigned char
typedef struct {
    unsigned char* array;
    int size;
    int capacity;
} vector_uchar;

void createVectorUChar(vector_uchar* vector) {
    vector->array = (unsigned char*)malloc(sizeof(unsigned char));
    vector->size = 0;
    vector->capacity = 1;
}

void appendVectorUChar(vector_uchar* vector, unsigned char data) {
    if (vector->capacity == vector->size) {
        vector->capacity *= 2;
        vector->array = (unsigned char*)realloc(vector->array, vector->capacity * sizeof(unsigned char));
    }
    vector->array[vector->size] = data;
    vector->size++;
}

void deleteVectorUChar(vector_uchar* vector) {
    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

void shrinkToFitVectorUChar(vector_uchar* vector) {
    if (vector->size != vector->capacity) {
        vector->array = (unsigned char*)realloc(vector->array, sizeof(unsigned char) * vector->size);
        vector->capacity = vector->size;
    }
}

void deleteElementVectorUChar(vector_uchar* vector, int index) {
    for (int i = index; i < vector->size - 1; i++) {
        vector->array[i] = vector->array[i + 1];
    }
    vector->size--;
}

void displayVectorUChar(vector_uchar* vector) {
    printf("{");
    for (int i = 0; i < vector->size; i++) {
        printf("%u", vector->array[i]);
        if (i != vector->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

// Vector for int pointers
typedef struct {
    int** array;
    int size;
    int capacity;
} vector_int_ptr;

void createVectorIntPtr(vector_int_ptr* vector) {
    vector->array = (int**)malloc(sizeof(int*));
    vector->size = 0;
    vector->capacity = 1;
}

void appendVectorIntPtr(vector_int_ptr* vector, int* data) {
    if (vector->capacity == vector->size) {
        vector->capacity *= 2;
        vector->array = (int**)realloc(vector->array, vector->capacity * sizeof(int*));
    }
    vector->array[vector->size] = data;
    vector->size++;
}

void deleteVectorIntPtr(vector_int_ptr* vector) {
    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

void shrinkToFitVectorIntPtr(vector_int_ptr* vector) {
    if (vector->size != vector->capacity) {
        vector->array = (int**)realloc(vector->array, sizeof(int*) * vector->size);
        vector->capacity = vector->size;
    }
}

void deleteElementVectorIntPtr(vector_int_ptr* vector, int index) {
    for (int i = index; i < vector->size - 1; i++) {
        vector->array[i] = vector->array[i + 1];
    }
    vector->size--;
}

void displayVectorIntPtr(vector_int_ptr* vector) {
    printf("{");
    for (int i = 0; i < vector->size; i++) {
        printf("%p", (void*)vector->array[i]);
        if (i != vector->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

// Vector for float pointers
typedef struct {
    float** array;
    int size;
    int capacity;
} vector_float_ptr;

void createVectorFloatPtr(vector_float_ptr* vector) {
    vector->array = (float**)malloc(sizeof(float*));
    vector->size = 0;
    vector->capacity = 1;
}

void appendVectorFloatPtr(vector_float_ptr* vector, float* data) {
    if (vector->capacity == vector->size) {
        vector->capacity *= 2;
        vector->array = (float**)realloc(vector->array, vector->capacity * sizeof(float*));
    }
    vector->array[vector->size] = data;
    vector->size++;
}

void deleteVectorFloatPtr(vector_float_ptr* vector) {
    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

void shrinkToFitVectorFloatPtr(vector_float_ptr* vector) {
    if (vector->size != vector->capacity) {
        vector->array = (float**)realloc(vector->array, sizeof(float*) * vector->size);
        vector->capacity = vector->size;
    }
}

void deleteElementVectorFloatPtr(vector_float_ptr* vector, int index) {
    for (int i = index; i < vector->size - 1; i++) {
        vector->array[i] = vector->array[i + 1];
    }
    vector->size--;
}

void displayVectorFloatPtr(vector_float_ptr* vector) {
    printf("{");
    for (int i = 0; i < vector->size; i++) {
        printf("%p", (void*)vector->array[i]);
        if (i != vector->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

// Vector for double pointers
typedef struct {
    double** array;
    int size;
    int capacity;
} vector_double_ptr;

void createVectorDoublePtr(vector_double_ptr* vector) {
    vector->array = (double**)malloc(sizeof(double*));
    vector->size = 0;
    vector->capacity = 1;
}

void appendVectorDoublePtr(vector_double_ptr* vector, double* data) {
    if (vector->capacity == vector->size) {
        vector->capacity *= 2;
        vector->array = (double**)realloc(vector->array, vector->capacity * sizeof(double*));
    }
    vector->array[vector->size] = data;
    vector->size++;
}

void deleteVectorDoublePtr(vector_double_ptr* vector) {
    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

void shrinkToFitVectorDoublePtr(vector_double_ptr* vector) {
    if (vector->size != vector->capacity) {
        vector->array = (double**)realloc(vector->array, sizeof(double*) * vector->size);
        vector->capacity = vector->size;
    }
}

void deleteElementVectorDoublePtr(vector_double_ptr* vector, int index) {
    for (int i = index; i < vector->size - 1; i++) {
        vector->array[i] = vector->array[i + 1];
    }
    vector->size--;
}

void displayVectorDoublePtr(vector_double_ptr* vector) {
    printf("{");
    for (int i = 0; i < vector->size; i++) {
        printf("%p", (void*)vector->array[i]);
        if (i != vector->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

// Vector for char pointers
typedef struct {
    char** array;
    int size;
    int capacity;
} vector_char_ptr;

void createVectorCharPtr(vector_char_ptr* vector) {
    vector->array = (char**)malloc(sizeof(char*));
    vector->size = 0;
    vector->capacity = 1;
}

void appendVectorCharPtr(vector_char_ptr* vector, char* data) {
    if (vector->capacity == vector->size) {
        vector->capacity *= 2;
        vector->array = (char**)realloc(vector->array, vector->capacity * sizeof(char*));
    }
    vector->array[vector->size] = data;
    vector->size++;
}

void deleteVectorCharPtr(vector_char_ptr* vector) {
    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

void shrinkToFitVectorCharPtr(vector_char_ptr* vector) {
    if (vector->size != vector->capacity) {
        vector->array = (char**)realloc(vector->array, sizeof(char*) * vector->size);
        vector->capacity = vector->size;
    }
}

void deleteElementVectorCharPtr(vector_char_ptr* vector, int index) {
    for (int i = index; i < vector->size - 1; i++) {
        vector->array[i] = vector->array[i + 1];
    }
    vector->size--;
}

void displayVectorCharPtr(vector_char_ptr* vector) {
    printf("{");
    for (int i = 0; i < vector->size; i++) {
        printf("%p", (void*)vector->array[i]);
        if (i != vector->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

// Vector for unsigned char pointers
__declspec(dllexport) typedef struct {
    unsigned char** array;
    int size;
    int capacity;
} vector_uchar_ptr;

void createVectorUCharPtr(vector_uchar_ptr* vector) {
    vector->array = (unsigned char**)malloc(sizeof(unsigned char*));
    vector->size = 0;
    vector->capacity = 1;
}

void appendVectorUCharPtr(vector_uchar_ptr* vector, unsigned char* data) {
    if (vector->capacity == vector->size) {
        vector->capacity *= 2;
        vector->array = (unsigned char**)realloc(vector->array, vector->capacity * sizeof(unsigned char*));
    }
    vector->array[vector->size] = data;
    vector->size++;
}

void deleteVectorUCharPtr(vector_uchar_ptr* vector) {
    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

void shrinkToFitVectorUCharPtr(vector_uchar_ptr* vector) {
    if (vector->size != vector->capacity) {
        vector->array = (unsigned char**)realloc(vector->array, sizeof(unsigned char*) * vector->size);
        vector->capacity = vector->size;
    }
}

void deleteElementVectorUCharPtr(vector_uchar_ptr* vector, int index) {
    for (int i = index; i < vector->size - 1; i++) {
        vector->array[i] = vector->array[i + 1];
    }
    vector->size--;
}

void displayVectorUCharPtr(vector_uchar_ptr* vector) {
    printf("{");
    for (int i = 0; i < vector->size; i++) {
        printf("%p", (void*)vector->array[i]);
        if (i != vector->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

#endif // VECTOR_H