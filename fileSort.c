#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

typedef struct node
{
    char* data;
    struct node * prev;
    struct node * next;
}node;



int comparatorInt(void* a, void* b){
	int c = atoi((char*)a);
	int d = atoi((char*)b);
	return c - d;
}

int comparatorStr(void* a, void* b){
    char* c = (char*)a;
    char* d = (char*)b;
    int cl = strlen(c);
    int dl = strlen(d);
    int i = 0;
    while (i < cl && i < dl){
        if (c[i] > d[i]){
            return 1;
        } else if (c[i] < d[i]){
            return -1;
        } else {
            i++;
        }
    }
    if (i == cl && i != dl){
        return -1;
    } else if (i != cl && i == dl){
        return 1;
    } else {
        return 0;
    }
}

int insertionSort(void* toSort, int (*comparator)(void*, void*) ){
    node * sort = (node *)toSort;
    char key[256];
    node * ptr = sort;
    ptr = ptr->next;
    node * prev;
	while (ptr != NULL){
		strcpy(key, ptr->data);
		prev = ptr->prev;
        char* temp = prev->data;
		while (prev != NULL && comparator(temp, key) > 0){
			strcpy(prev->next->data, prev->data);
			prev = prev->prev;
			if (prev != NULL) {
                temp = prev->data;
			}
		}
		if (prev == NULL){
            strcpy(sort->data, key);
		} else {
            strcpy(prev->next->data, key);
		}
		ptr = ptr->next;
	}
	return 0;
}

void printlist(node * ptr){
    while (ptr != NULL){
        printf("|%s|\n", ptr->data);
        ptr = ptr->next;
    }
}


int main(int argc, char** argv){
    if (argc < 3){
        printf("Not enough arguments\n");
        return -1;
    }
    node * sort = malloc(sizeof(node));
    sort->data = malloc(sizeof(char) * 256);
    bzero(sort->data, sizeof(char) * 256);
    sort->prev = NULL;
    sort->next = NULL;
    node * ptr = sort;
	int fd = open(argv[2], O_RDONLY);
	char buffer = '!';
    int readed = 0;
    char* str = malloc(sizeof(char) * 256);
    str[0] = '\0';
    do{
        readed = read(fd, &buffer, 1);
        if (readed != 0){
			if (buffer != ' '){
				if (isdigit(buffer) || isalpha(buffer)){
					char* temp = malloc(sizeof(char) * 2);
		        	temp[0] = buffer;
		        	temp[1] = '\0';
		        	strcat(str, temp);
		        } else if (buffer == ',' || buffer == '\n' ){
		        	strcpy(ptr->data, str);
		        	node * temp = malloc(sizeof(node));
		        	temp->data = malloc(sizeof(char) * 256);
                    bzero(temp->data, sizeof(char) * 256);
		        	temp->prev = ptr;
		        	temp->next = NULL;
		        	ptr->next = temp;
		        	ptr = ptr->next;
		        	bzero(str, sizeof(char) * 256);
		        }
			}

        }
    } while(readed != 0);

    printf("Before Sorting: \n");
    printlist(sort);
    //Figure out is num or char
    if(isdigit(sort->data[0])){
        insertionSort(sort, comparatorInt);
    } else {
        insertionSort(sort, comparatorStr);
    }

    printf("After Sorting: \n");
    printlist(sort);

	printf("End of File\n");

	return 0;
}



//hello
