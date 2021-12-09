#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define READ 0
#define WRITE 1

void process(char *);
void quick(char *, int, int);
int partition(char *, int, int);
void swap(char *, int, int);

int main()
{
    int toParent[2];
    int toChild[2];

    pipe(toParent);
    pipe(toChild);

    int c = fork();

    if (c)
    {
        close(toParent[WRITE]);
        close(toChild[READ]);

        char input[1000];
        input[0] = '\0';

        while (1)
        {
            printf("Input message to give to child (or exit to quit): ");

            fgets(input, sizeof(input) - 1, stdin);

            if (input[strlen(input) - 1] == '\n')
            {
               input[strlen(input) - 1] = '\0'; 
            }

            if (strcmp(input, "exit") == 0)
            {
                break;
            }

            write(toChild[WRITE], input, sizeof(input));
            read(toParent[READ], input, sizeof(input));

            printf("\nMessage recieved from child: %s\n", input);
        }

        close(toParent[READ]);
        close(toChild[WRITE]);
    }
    else
    {
        close(toParent[READ]);
        close(toChild[WRITE]);

        char input[1000];
        int r = read(toChild[READ], input, sizeof(input));
        while (r != 0 && r != -1)
        {
            process(input);
            write(toParent[WRITE], input, sizeof(input));
            
            r = read(toChild[READ], input, sizeof(input));
        }

        close(toParent[WRITE]);
        close(toChild[READ]);
    }
}

void process(char *input)
{
    quick(input, 0, strlen(input));
}

void quick(char *input, int l, int r)
{
    if (l >= r)
    {
        return;
    }

    int ind = partition(input, l, r);
    quick(input, l, ind);
    quick(input, ind + 1, r);
}

int partition(char *input, int l, int r)
{
    int index = l;
    for (int i = l; i < r - 1; i++)
    {
        if (input[i] < input[r - 1])
        {
            swap(input, i, index);
            index++;
        }
    }

    swap(input, index, r - 1);
    return index;
}

void swap(char *input, int a, int b)
{
    char temp = input[a];
    input[a] = input[b];
    input[b] = temp;
}
