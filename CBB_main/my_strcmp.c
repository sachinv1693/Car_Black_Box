#include "main.h"

/* String compare function which returns 0 if the given strings are equal */
unsigned char my_strcmp(const char* str1, const char* str2)
{
        //A variable to hold string index
        unsigned char index = 0;
        //Both strings are equal in length
        while (str1[index] != '\0' && str2[index] != '\0')
        {
                //If characters are not matching, return the ascii difference
                if (str1[index] != str2[index])
                        return str1[index] - str2[index];
                //Move to next index
                index++;
        }
        //Return 0 is both strings are equal
        return 0;
}

