#include "search.h"
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

int create_database(File_t *file, Mainnode_t *arr[])
{
    char word[30];
    if(file == NULL)
    {
        printf("ERROR.... Unable to open faile\n");
        return FAILURE;
    }

    File_t *temp = file;
    while(temp != NULL)
    {
        FILE *fptr = fopen(temp -> fname, "r");
        if(fptr == NULL)
        {
            printf("ERROR.... Unable to open %s file\n", temp -> fname);
            temp = temp -> link;
            continue;
        }
        while(fscanf(fptr, "%s", word) != EOF)
        {
            insert_word(temp -> fname, word, arr);
        }
        fclose(fptr);
        temp = temp -> link;
    }
    return SUCCESS;
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
int display_database(Mainnode_t *arr[])
{
    for(int i = 0; i < 27; i++)
    {
        if(arr[i] == NULL)
        {
            continue;
        }
        Mainnode_t *M_temp = arr[i];
        while(M_temp != NULL)
        {
            printf("%-5d\t%-5s\t%-5d\t", i, M_temp -> word, M_temp -> file_count);
            Subnode_t *S_temp = M_temp -> sublink;
            while(S_temp != NULL)
            {
                printf("%-5s\t%-5d\t", S_temp -> filename, S_temp -> word_count);
                S_temp = S_temp -> link;
            }
            printf("\n");
            M_temp = M_temp -> link;  
        }
    }
    return SUCCESS;
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
int search_database(char *word, Mainnode_t *arr[])
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

    Mainnode_t *M_temp = arr[index];
    while(M_temp != NULL)
    {
        if(strcmp(M_temp -> word, word) == 0)
        {
            printf("\nWord %s is present in %d files\n", word, M_temp -> file_count);
            Subnode_t *S_temp = M_temp -> sublink;
            while(S_temp != NULL)
            {
                printf("In %s = %d times\n", S_temp -> filename, S_temp -> word_count);
                S_temp = S_temp -> link;
            }
            return SUCCESS;
        }
        M_temp = M_temp -> link;  
    }
    printf("Word %s is not found...\n", word);
    return FAILURE;
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
int update_database(File_t *file, Mainnode_t *arr[])
{
    char fname[30], word[30];
    int index, file_count, word_count;
    printf("Enter the backup file: ");
    scanf("%s", fname);

    if(valid_backupfile(fname) == FAILURE)
    {
        printf("ERROR...\n");
        return FAILURE;
    }

    FILE *fptr = fopen(fname, "r");
    if(fptr == NULL)
    {
        printf("File failed\n");
        return FAILURE;
    }

    while(fscanf(fptr, "#%d;%[^;];%d;", &index, word, &file_count) == 3)
    {
        Mainnode_t *M_newNode = malloc(sizeof(Mainnode_t));
        if(M_newNode == NULL)
        {
            printf("Memory allocation failed....\n");
            return FAILURE;
        }

        strcpy(M_newNode -> word, word);
        M_newNode -> file_count = file_count;
        M_newNode -> sublink = NULL;
        M_newNode -> link = NULL;

        if(arr[index] == NULL)
        {
            arr[index] = M_newNode;
        }
        else
        {
            Mainnode_t *M_temp = arr[index];
            while(M_temp -> link != NULL)
            {
                M_temp = M_temp -> link;
            }
            M_temp -> link = M_newNode;
        }

        for(int i = 0; i < file_count; i++)
        {
            if(fscanf(fptr, "%[^;];%d", fname, &word_count) != 2)
            {
                printf("ERroro");
                return FAILURE;
            }
            Subnode_t *S_newNode = malloc(sizeof(Subnode_t));
            if(S_newNode == NULL)
            {
                printf("Memory allocation failed\n");
                return FAILURE;
            } 

            strcpy(S_newNode -> filename, fname);
            S_newNode -> word_count = word_count;
            S_newNode -> link = NULL;

            if(M_newNode -> sublink == NULL)
            {
                M_newNode -> sublink = S_newNode;
            }
            else
            {
                Subnode_t *S_temp = M_newNode -> sublink;
                while(S_temp -> link != NULL)
                {
                    S_temp = S_temp -> link;
                }
                S_temp -> link = S_newNode;
            }
        }
        fscanf(fptr, "#");
    }
    printf("Database updated successfully....\n");
    return SUCCESS;
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
int save_database(Mainnode_t *arr[])
{
    FILE *fptr = fopen("save.txt","w+");
    if(fptr == NULL)
    {
        printf("ERROR... Unable to open save.csv\n");
        return FAILURE;
    }

    for(int i = 0; i < 27; i++)
    {
        if(arr[i] == NULL)
        {
            continue;
        }
        Mainnode_t *M_temp = arr[i];
        while(M_temp != NULL)
        {
            fprintf(fptr,"#%d;%s;%d;", i, M_temp -> word, M_temp -> file_count);
            Subnode_t *S_temp = M_temp -> sublink;
            while(S_temp != NULL)
            {
                fprintf(fptr, "%s;%d;", S_temp -> filename, S_temp -> word_count);
                S_temp = S_temp -> link;
            }
            fprintf(fptr, "#\n");
            M_temp = M_temp -> link;  
        }
    }
    printf("Database saved successfully....\n");
    return SUCCESS;
}

