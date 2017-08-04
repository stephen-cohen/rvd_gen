/*
 * rvd_gen.c -	Generate a designer TAL effector RVD sequence from a given
 *		nucleotide sequence
 * 
 * VERSION 1.0 Release - 2017-Aug-03
 *
 * Copyright (c) 2017, Stephen P. Cohen
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific
 *    prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* maximum size of input */
#define LIMIT 41

/* function prototypes and main function */
int get_nucl_seq(char *nucl_seq);
void lowercase(char *nucl_seq, int length);
void str_seq(char *nucl_seq, int length);
void gen_rvd_seq(char *nucl_seq, int length, char *rvd_seq);

int main(int argc, char **argv)
{
	int length;
	char nucl_seq[LIMIT], rvd_seq[LIMIT*3];

	/* if there are no arguments, get a nucleotide sequence from user */
	if(argc == 1) {
		length = get_nucl_seq(nucl_seq);
	/* or try to copy the first argument */
	} else {
		if (strlen(*(argv+1)) < LIMIT) {
			strcpy(nucl_seq, *(argv+1));
			length = strlen(nucl_seq);
		} else {
			printf("Error: Given sequence too long.\n");
			return 1;
		}
	}
	/* convert to lowercase */
	lowercase(nucl_seq, length);	
	/* strengthen the sequence to optimize TALE binding */
	str_seq(nucl_seq, length);
	/* generate an RVD sequence given the strengthened sequence */
	gen_rvd_seq(nucl_seq, length, rvd_seq);

	/* output the RVD sequence and exit normally */
	printf("%s\n",rvd_seq);
	return 0;
}

/* get nucleotide sequence from user input and return length */
int get_nucl_seq(char *nucl_seq)
{
	int i,c;
	printf("Enter nucleotide sequence: ");
	for (i = 0; (c = getchar()) != '\n' && i < LIMIT - 1; i++)
		*(nucl_seq+i) = c;
	*(nucl_seq+i) = '\0';
	return i;
}

/* make nucleotide sequence all lowercase */
void lowercase(char *nucl_seq, int length)
{
	int i;
	for (i = 0; i < length; i++) {
		if (*(nucl_seq+i) >= 'A' && *(nucl_seq+i) <= 'Z')
			*(nucl_seq+i) = *(nucl_seq+i) + 'a' - 'A';
	}
}

/* strengthen sequence to ensure no stretches of >5 weak RVDs if possible */
void str_seq(char *nucl_seq, int length)
{
	int i, j, weak_counter = 0;
	char c;
	for (i = 0; i < length; i++) {
		if (*(nucl_seq+i) == 'c' || *(nucl_seq+i) == 'r')
			weak_counter = 0;
		else
			weak_counter++;

		/* if there are more than 5 weak RVDs, try to change to R *
                 * for NN (a strong RVD)                                  */
		if (weak_counter >= 5) {
			for (j = i; j >= 0; j--) {
				c = *(nucl_seq+j);
				if (c != 't' && c != 'c') {
					*(nucl_seq+j) = 'r';
					weak_counter = i - j;
					break;
				}
			}
		}
	}
}

/* generate an RVD sequence from a strengthened sequence */
void gen_rvd_seq(char *nucl_seq, int length, char *rvd_seq)
{
	int i;
	*rvd_seq = '\0';
	for (i = 0; i < length; i++) {
		switch(*(nucl_seq+i)) {
			case 'a':
				strcat(rvd_seq, "NI-");
				break;
			case 'c':
				strcat(rvd_seq, "HD-");
				break;
			case 'g':
				strcat(rvd_seq, "NK-");
				break;
			case 't':
				strcat(rvd_seq, "NG-");
				break;
			case 'r':
				strcat(rvd_seq, "NN-");
				break;
			default:
				/* escape sequence used to disrupt trigraph */
				strcat(rvd_seq, "?\?-");
		}
	}
	/* get rid of trailing hyphen */
	rvd_seq[length*3-1] = '\0';
}
