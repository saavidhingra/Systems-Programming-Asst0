#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct node
{
    char* data;
    struct node * prev;
    struct node * next;
}node;



int comparatorInt(void* a, void* b){
	long c = atol((char*)a);
	long d = atol((char*)b);
	long result = c - d;
	if (result > 0){
        return 1;
	} else if (result < 0){
        return -1;
	} else {
        return 0;
	}
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
    //printf("Using insertion Sort\n");
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

int partition(node * arr, int lo, int hi, int (*comparator)(void*, void*)){
    char* pivot = malloc(sizeof(char) * 256);
    node * ptr = arr;
    int i = 0;
    while (i != lo){
        ptr = ptr->next;
        i++;
    }
    strcpy(pivot, ptr->data);
    node * ptrpivot = ptr;
    node * ptrl = ptr->next;
    ptr = arr;
    i = 0;
    while (i != hi){
        ptr = ptr->next;
        i++;
    }
    node * ptrr = ptr;

    int left = lo + 1;
    int right = hi;
    while (7 > 2){
        while (left <= right){
            if (comparator(ptrl->data, pivot) < 0){
                left++;
                ptrl = ptrl->next;
            } else {
                break;
            }
        }

        while (right > left){
            if (comparator(ptrr->data, pivot) < 0){
                break;
            } else {
                right--;
                ptrr = ptrr->prev;
            }
        }

        if (left >= right){
            break;
        }

        //Swap elements at left and right
        char buffer[256];
        strcpy(buffer, ptrl->data);
        strcpy(ptrl->data, ptrr->data);
        strcpy(ptrr->data, buffer);

        left++;
        right--;
        ptrl = ptrl->next;
        ptrr = ptrr->prev;
    }

    //Swap elements at left-1 with pivot
    left--;
    i = 0;
    ptrl = arr;
    while (i != left){
        ptrl = ptrl->next;
        i++;
    }
    strcpy(ptrpivot->data, ptrl->data);
    strcpy(ptrl->data, pivot);
    free(pivot);
    return left;

}

void quickSortR(node * arr, int lo, int hi, int (*comparator)(void*, void*)){
    if ((hi-lo) <= 0) return; // Less than 2 items

    int splitPoint = partition(arr, lo, hi, comparator);
    quickSortR(arr, lo, splitPoint - 1, comparator);
    quickSortR(arr, splitPoint + 1, hi, comparator);
}

int quickSort(void* toSort, int (*comparator)(void*, void*)){
    //printf("Using QuickSort\n");
    node * sort = (node*)toSort;
    //Find out the length of the linked list
    int length = 0;
    node * ptr = sort;
    while (ptr != NULL){
        length++;
        ptr = ptr->next;
    }
    if (length <= 1){
        return; //Already sorted
    }
    quickSortR(sort, 0, length - 1, comparator);
    return 0;
}

void printlist(node * ptr){
    while (ptr != NULL){
        printf("%s\n", ptr->data);
        ptr = ptr->next;
    }
}

void freelist(node * sort){
    while (sort != NULL){
        node * temp = sort;
        sort = sort->next;
        temp->prev = NULL;
        temp->next = NULL;
        free(temp->data);
        free(temp);
    }
}


int main(int argc, char** argv){
    if (argc < 3){
        printf("Not enough arguments\n");
        exit(0);
    }
    node * sort = malloc(sizeof(node));
    sort->data = malloc(sizeof(char) * 256);
    bzero(sort->data, sizeof(char) * 256);
    sort->prev = NULL;
    sort->next = NULL;
    node * ptr = sort;
	int fd = open(argv[2], O_RDONLY);
	if (fd == -1){
        printf("Error opening file, EXITTING PROGRAM\n");
        freelist(sort);
        exit(0);
	}
	char buffer = '!';
    int readed = 0;
    char* str = malloc(sizeof(char) * 256);
    str[0] = '\0';
    bool emptyFileCheck = true;
    do{
        readed = read(fd, &buffer, 1);
        if (emptyFileCheck){
            if (readed == 0) { //First check of file and the file is empty
                printf("The file given is empty, EXITTING PROGRAM\n");
                freelist(sort);
                free(str);
                exit(0);
            } else {
                emptyFileCheck = false;
            }
        }

        if (readed != 0){
			if (buffer != ' '){
				if (isdigit(buffer) || isalpha(buffer) || buffer == '-'){
					char* temp = malloc(sizeof(char) * 2);
		        	temp[0] = buffer;
		        	temp[1] = '\0';
		        	strcat(str, temp);
		        	free(temp);
		        } else if ((buffer == ',' || buffer == '\n') && strlen(str) != 0 ){
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
    //Delete the last node since it will always be empty
    free(str);
    node * temp = ptr;
    ptr = ptr->prev;
    if (ptr == NULL){
        //That means nothing was put in the list
        printf("The file given is empty or no values were given, EXITTING PROGRAM\n");
        freelist(sort);
        exit(0);

    }
    ptr->next = NULL;
    //Free temp
    temp->prev = NULL;
    free(temp->data);
    free(temp);

    //Figure out is num or char
    if(isdigit(sort->data[0]) || (sort->data[0] == '-' && isdigit(sort->data[1]))){
        if (argv[1][0] == '-' && argv[1][1] == 'i'){
            //printf("Before Sorting: \n");
            //printlist(sort);
            insertionSort(sort, comparatorInt);
        } else if (argv[1][0] == '-' && argv[1][1] == 'q'){
            //printf("Before Sorting: \n");
            //printlist(sort);
            quickSort(sort, comparatorInt);
        } else {
            printf("Invalid Entry: Please use either -i or -q for insertion sort or quicksort\n");
            freelist(sort);
            exit(0);
        }
    } else if (isalpha(sort->data[0])){
        if (argv[1][0] == '-' && argv[1][1] == 'i'){
            //printf("Before Sorting: \n");
            //printlist(sort);
            insertionSort(sort, comparatorStr);
        } else if (argv[1][0] == '-' && argv[1][1] == 'q'){
            //printf("Before Sorting: \n");
            //printlist(sort);
            quickSort(sort, comparatorStr);
        } else {
            printf("Invalid Entry: Please use either -i or -q for insertion sort or quicksort\n");
            freelist(sort);
            exit(0);
        }
    } else {
        printf("Invalid Entry: The first character is neither a num or a letter please enter a new file\n");
        freelist(sort);
        exit(0);
    }

    //printf("After Sorting: \n");
    printlist(sort);

	//printf("End of File\n");
    //Free Linked list
    freelist(sort);
	close(fd);

	return 0;
}



//hello
