#include "search.h"
#include <string.h>
#include <ctype.h>

int validate_arg(char *argv[], int index)
{
    //File extension
    char *dot = strrchr(argv[index], '.');
    if(dot == NULL || strcmp(dot, ".txt") != 0)
    {
        printf("ERROR...Extension must be '.txt'\n");
        return FAILURE;
    }

    //File present or not
    FILE *fptr = fopen(argv[index], "r");
    if(fptr == NULL)
    {
        printf("File does not exist\n");
        return FAILURE;
    }

    //Check the content present or not
    if(fgetc(fptr) == EOF)
    {
        printf("File is empty\n");
        fclose(fptr);
        return FAILURE;
    }
    fclose(fptr);

    //Check duplicate or not
    for(int i = 1; i < index; i++)
    {
        if(strcmp(argv[index], argv[i]) == 0)
        {
            printf("ERROR... Duplicate file found\n");
            return FAILURE;
        }
    }
    return SUCCESS;
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
int insert_at_last(File_t **file, char *file_name)
{
    File_t *newNode = malloc(sizeof(File_t));
    if(newNode == NULL)
    {
        printf("Memory allocation failed\n");
        return FAILURE;
    }
    
    strcpy(newNode -> fname, file_name);
    newNode -> link = NULL;
    if(*file == NULL)
    {
        *file = newNode;
        return SUCCESS;
    }
    File_t *temp = *file;
    while(temp -> link != NULL)
    {
        temp = temp -> link;
    }
    temp -> link = newNode;
    return SUCCESS;
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
void print_list(File_t *file)
{
	if (file == NULL)
	{
		printf("INFO : List is empty\n");
	}
    else
    {
        printf("\nfile -> ");
	    while (file != NULL)		
	    {
		    printf("%s -> ", file -> fname);
		    file = file -> link;
	    }

	    printf("NULL\n");
    }
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
int insert_word(char *fname, char *word, Mainnode_t *arr[])
{
    int index;
    if(islower(word[0]))
    {
        index = word[0] - 'a';
    }
    else if(isupper(word[0]))
    {
        index = word[0] - 'A';
    }
    else
    {
        index = 26;
    }

    if(arr[index] == NULL)
    {
        //Creating memory for subnode
        Subnode_t *S_newNode = malloc(sizeof(Subnode_t));
        if(S_newNode == NULL)
        {
            printf("Memory allocation for subnode is failed\n");
            return FAILURE;
        }
        strcpy(S_newNode -> filename, fname);
        S_newNode -> word_count = 1;
        S_newNode -> link = NULL;

        //Creating memory for mainnode
        Mainnode_t *M_newNode = malloc(sizeof(Mainnode_t));
        if(M_newNode == NULL)
        {
            printf("Memory allocation for mainnode is failed\n");
            return FAILURE;
        }
        strcpy(M_newNode -> word, word);
        M_newNode -> file_count = 1;
        M_newNode -> sublink = S_newNode;
        M_newNode -> link = NULL;

        arr[index] = M_newNode;
        return SUCCESS;
    }
            
    Mainnode_t *M_temp = arr[index];
    Mainnode_t *M_prev = NULL;

    while(M_temp != NULL)
    {
        if(strcmp(M_temp -> word, word) == 0)
        {
            Subnode_t *S_temp = M_temp -> sublink;
            Subnode_t *S_prev = NULL;
            while(S_temp != NULL)
            {
                if(strcmp(S_temp -> filename, fname) == 0)
                {
                    (S_temp -> word_count)++;
                    return SUCCESS;
                }
                S_prev = S_temp;
                S_temp = S_temp -> link;
            }
            Subnode_t *S_newNode = malloc(sizeof(Subnode_t));
            if(S_newNode == NULL)
            {
                printf("Memory allocation for subnode is failed\n");
                return FAILURE;
            }
            strcpy(S_newNode -> filename, fname);
            S_newNode -> word_count = 1;
            S_newNode -> link = NULL;

            S_prev -> link = S_newNode;
            (M_temp -> file_count)++;
        }
        M_prev = M_temp;
        M_temp = M_temp -> link;
    }
    //Creating memory for subnode
    Subnode_t *S_newNode = malloc(sizeof(Subnode_t));
    if(S_newNode == NULL)
    {
        printf("Memory allocation for subnode is failed\n");
        return FAILURE;
    }
    strcpy(S_newNode -> filename, fname);
    S_newNode -> word_count = 1;
    S_newNode -> link = NULL;

    //Creating memory for mainnode
    Mainnode_t *M_newNode = malloc(sizeof(Mainnode_t));
    if(M_newNode == NULL)
    {
        printf("Memory allocation for mainnode is failed\n");
        return FAILURE;
    }
    strcpy(M_newNode -> word, word);
    M_newNode -> file_count = 1;
    M_newNode -> sublink = S_newNode;
    M_newNode -> link = NULL;

    M_prev -> link = M_newNode;
    return SUCCESS;
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
int valid_backupfile(char *filename)
{
    //File extension
    char *dot = strrchr(filename, '.');
    if(dot == NULL || strcmp(dot, ".txt") != 0)
    {
        printf("ERROR...Extension must be '.txt'\n");
        return FAILURE;
    }

    //File present or not
    FILE *fptr = fopen(filename, "r");
    if(fptr == NULL)
    {
        printf("File does not exist\n");
        return FAILURE;
    }

    //Check the content present or not
    if(fgetc(fptr) == EOF)
    {
        printf("File is empty\n");
        fclose(fptr);
        return FAILURE;
    }
    rewind(fptr);

    char start, end;
    start = fgetc(fptr);
    fseek(fptr, -1, SEEK_END);
    end = fgetc(fptr);
    if(end == '\n')
    {
        fseek(fptr, -2, SEEK_END);
        end = fgetc(fptr);
    }

    if(start != '#' || end != '#')
    {
        printf("Start = %c and End = %c\n", start, end);
        printf("ERROR....Invalid Backup file contents\n");
        fclose(fptr);
        return FAILURE;

    }
    fclose(fptr);
    return SUCCESS;
}