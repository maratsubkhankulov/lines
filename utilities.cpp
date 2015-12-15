// Utility function bodies

#include "utilities.h"
#include "stdlib.h"
#include "math.h"

float makePositive(float x) // refactor this for any number type later on
{
    if (x < 0) return (0-x);
    return x;
}

int intMax(int a, int b)
{
    if (a > b) return a;
    return b;
}

// Returns the index of the largest element - perhaps employ merge sort
int maxIndex(int array[], int arraySize)
{
    int index = 0;
    for (int i = 0; i < arraySize; i++)
    {
        if (array[i] > array[index])
        {
            index = i;
        }
    }
    return index;
}

void concat(char* target, char* source)
{
    int targetIndex = 0;
    while (target[targetIndex])
    {
        targetIndex++;
    }
    
    int sourceIndex = 0;
    while (source[sourceIndex])
    {
        target[targetIndex] = source[sourceIndex];
        sourceIndex++;
        targetIndex++;
    }
    
    target[targetIndex] = '\0';
}

void wait(float time)
{
    float elapsedTime = 0;
    while(elapsedTime < time)
    {
        elapsedTime += 0.001;
    }
}

/*void toString(int* array, int length, char* str)
{
    int strIndex = 0;
    for (int n = 0; n < length; n++)
    {
        char strNumber[2];
        itoa(array[n], strNumber, 10);
        str[strIndex] = strNumber[0];
        str[strIndex+1] = ',';
        str[strIndex+2] = ' ';
        strIndex += 3;
    }
    
    str[strIndex] = '\0';
}*/
//
//void toString(int x, char* str)   // Maybe completely redundant
//{ 
//    int strIndex = 0;
//    basePower = floor(log10(x));
//    if (x == 0)
//    {
//        strIndex++;
//        str[strIndex] = '0';
//    } else
//    {    
//        for(int n = basePower; n > 0; n++); 
//        {
//            int r = x - x/(10^n);
//            char c
//        }
//        


