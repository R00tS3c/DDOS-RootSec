#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char encodes[] = { 
	'<', '>', '@', 'o', '$', ':', ',', '.', 'l', '+', '*', '^', '?', '=', ')', '(', 
	'|', 'A', 'B', '&', '%', ';', 'D', '"', '!', 'w', 'k', 'U', 'x', 'z', 'v', 'u', 
	't', 's', 'r', 'q', 'p', '_', 'n', 'm', '-', 'i', 'h', 'g', 'f', 'F', 'C', 'c', 
	'b', 'a', '~', 'K', '2', '3', '4', '5', '6', '7', '8', '9', 'e', 'y', 'd', '1',
    'X', 'S', 'N', 'Q', 'W', 'T', 'Z', 'M', 'I', 'R', 'H', 'G', 'V', 'O', 'Y', 'L',
    'j', 'P', 'J', 'E', '/', ']', '['
};
char decodes[] = { 
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
    'z', 'y', 'w', 'x', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'Z', 'Y', 'W', 'X', '|', ':', 
    '.', ' ', '!', '#', '-', '/', ';', '&', '*', '\'', '\"', '\n', '\r', '\0', '>',
    '$', '%', '(', ')', ',', '?', '=', '@'
};



char *encode(char *str)
{
	int x = 0, i = 0, c;
    char encoded[512];
	memset(encoded, 0, sizeof(encoded));
	while(x < strlen(str))
	{
		for(c = 0; c <= sizeof(decodes); c++)
		{
			if(str[x] == decodes[c])
			{
				encoded[i] = encodes[c];
				i++;
			}
		}
		x++;
	}
	encoded[i] = '\0';
    char *ptrEncoded = strdup(encoded);
	return ptrEncoded;
}

char *decode(char *str)
{
	int x = 0, i = 0, c;
    char decoded[512];
	memset(decoded, 0, sizeof(decoded));
	while(x < strlen(str))
	{
		for(c = 0; c <= sizeof(encodes); c++)
		{
			if(str[x] == encodes[c])
			{
				decoded[i] = decodes[c];
				i++;
			}
		}
		x++;
	}
	decoded[i] = '\0';
    char *ptrDecoded = strdup(decoded);
	return ptrDecoded;
}

int main(int argc, char *argv[]){

FILE *ptr_file;
char buf[1000];

ptr_file =fopen("input.txt","r");
if (!ptr_file) return 1;

while (fgets(buf,1000, ptr_file)!=NULL){
	printf("%s - Decoded\n",decode(buf)); 
	printf("%s - Encoded\n",encode(buf));
}

}