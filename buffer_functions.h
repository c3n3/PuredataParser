#pragma once
#include <string>
#include <iostream>
#include <string.h>
#include "Timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std::string_literals;

#define TAB_TO_SPACES 4

int resize(char** str, int newSize) {
    Timer t("Resize -> ");
	char* temp = (char*)realloc(*str, sizeof(char) * newSize);
	if (temp == NULL) {
		return 0;
	}
    *str = temp;
	return 1;
}

int expand(char** str, int start, int moves, int& bufLen) {
	while (bufLen < strlen(*str) + 1 + moves) { // makes sure the resize is possible
		bufLen = bufLen * 2;
		if (!resize(str, bufLen)) return -1; // failed
	}
    int length = strlen(*str);
	// for (int i = moves; i > -1; i--) {
	// 	(*str)[i + length] = (*str)[length - moves + i];
	// }
    for (int i = length; i >= start; i--) {
        (*str)[i + moves] = (*str)[i];
    }
    
    for (int i = start; i < start + moves; i++) {
        (*str)[i] = '_';
    }
    return 1;
}

void shrink(char* str, int start, int len) {
	for (int k = 0; k < len; k++) {
		int length = strlen(str);
		for (int i = length - 1; i >= start; i--) {
			char temp = str[i];
			str[i] = str[length];
			str[length] = temp;
		}
	}

}

void concat(char** buf, int index, int& buflen, const char* add) {
    if (index + strlen(add) > buflen) {
        buflen *= 2;
        resize(buf, buflen);
    }
    for (int i = 0; i < strlen(add); i++) {
        (*buf)[i + index] = add[i];
    }
}

void replaceNumberWithNumber(char** str, int number, int startIndex, int& buflen) {
    Timer t("Replacement -> ");
    int end;
    int dig;
    for (end = startIndex; end < strlen(*str) && ((*str)[end] > '/' && (*str)[end] < ':'); end++);
    if (end == startIndex) return;
    end--;
    while (number > 0) {
        dig = number % 10;
        number = number / 10;
        if (startIndex > end) { // we need to resize as the string needs a number with more chars than original
            int spaces = 0;
            int temp = number;
            for (;temp != 0; temp /= 10) {spaces++;};
            expand(str, startIndex, spaces + 1, buflen);
            end += spaces + 1;
        }
        (*str)[end--] = dig + '0';
    }
    if (startIndex < end) {
        shrink(*str, startIndex, end - startIndex + 1);
    }
}
 
 void isNumber(const char * str, int number) {
     
 }

 int charPtrToInt(const char* str, int length) {
     int ret = 0;
     for (int i = 0; i < length; i++) {
         ret *= 10;
         ret += (str[i] - '0');
     }
     return ret;
 }

 void offsetNumberWithNumber(char** str, int offset, int startIndex, int& buflen) {
    int end;
    int dig;
    for (end = startIndex; end < strlen(*str) && ((*str)[end] > '/' && (*str)[end] < ':'); end++);
    if (end == startIndex) return;
    int target = charPtrToInt(&(*str)[startIndex], end - startIndex);
    int number = target + offset;
    end--;
    while (number > 0) {
        dig = number % 10;
        number = number / 10;
        if (startIndex > end) { // we need to resize as the string needs a number with more chars than original
            int spaces = 0;
            int temp = number;
            for (;temp != 0; temp /= 10) {spaces++;};
            expand(str, startIndex, spaces + 1, buflen);
            end += spaces + 1;
        }
        (*str)[end--] = dig + '0';
    }
    if (startIndex < end) {
        shrink(*str, startIndex, end - startIndex + 1);
    }
}
