#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char name [100];
    int year;
    char genre [50];
    float rating;
    struct Node * next;
}Movie;


//Function Definitions
void printOptions();
void clearBuffer();
void printingTheLinkList(Movie *startNode);
void creatingNodeFromMenu(Movie *hNode,Movie **hNodePtr,FILE * fptr,int flag);
void deletingNode(Movie *hNode,Movie **hNodePtr);
void sortingTheList(Movie * hNode, Movie ** hNodePtr);
//Global Variables
int numMovies = 0; //This variable keeps track of the number of movies in the LinkList. It is used for the ID number and also to determine if the LinkList is blank
int input = 0; //This variable will hold the value of the option selected by the user
int scanfReturnValue; //This variable will hold the return value of the scanF for the user's option selction. If the value is 1 then it means it's sucessful if it is 0 then the option entered is invalid.
int exitFlag = 1;//This is used to check if the while loop that takes input from the user wants to break 

int main()
{
    //LinkList Pointers
    Movie * head = NULL, * last = NULL;
   

    //This block of code is going to look for a list.txt file to see if there is a previous movie collection
    FILE *fptr;
    

    puts("Hello Alfred, welcome to your Linked List Movie Collection!");
    while(exitFlag == 1)
    {
            printOptions();
            scanfReturnValue = scanf("%d",&input);
            while(scanfReturnValue != 1 || input > 4 || input < 0)
            {
                clearBuffer();
                puts("That is not a valid option Alfred");
                printOptions();
                scanfReturnValue = scanf("%d",&input);
            }
            switch (input)
            {
                case 0:
                    printingTheLinkList(head);
                    break;
                case 1:
                    if (head == NULL) //This means we are adding the first movie to our collection. So we set our last and Index pointers to point to that node now that the content of that node has been filled
                    {
                        creatingNodeFromMenu(head,&head,fptr,0); 
                    }
                    else //This means we are adding nodes but not the first node. Passing a zero tells the function to grab new memory for a new node. 
                    {
                        creatingNodeFromMenu(head,&head,fptr,1);
                    }
                    break;
                case 2:
                    deletingNode(head,&head);
                    break;
                case 3:
                    sortingTheList(head,&head);
                case 4:
                    puts("Bye Bye");
                    exitFlag = 0;
                    break;
                default:
                    puts("That is not a valid option Alfred");
            }
        
    }
    return 0;
}


void printOptions()
{
    puts("What would you like to do?\n");
    puts("[0] View entire movie collection");
    puts("[1] Add movie to collection");
    puts("[2] Delete movie from collection");
    puts("[3] Sort movie collection");
    puts("[4] Exit the program");
}


void clearBuffer()
{
    while (getchar() != '\n');
}


void creatingNodeFromMenu(Movie *hNode,Movie **hNodePtr,FILE * fptr,int flag)
{
        char currentName [100];
        int currentYear;
        char currentGenre [50];
        float currentRating;
    while(1)
    {
        puts("What is the name of the movie?");
        scanf("%s",currentName);
        clearBuffer();

        puts("What year did the movie come out?");
        scanfReturnValue = scanf("%d",&currentYear);
        while(scanfReturnValue != 1 || currentYear < 1888 || currentYear > 3500)
        {
            clearBuffer();
            puts("That is not a valid option Alfred");
            puts("What year did the movie come out?");
            scanfReturnValue = scanf("%d",&currentYear);
        }
        clearBuffer();

        puts("What is the genre of the movie?");
        scanf("%s",currentGenre);
        clearBuffer();

        puts("What is the IMDB rating of the movie? (0-10)");
        scanfReturnValue = scanf("%f",&currentRating);
        while(scanfReturnValue != 1 || currentRating < 0 || currentRating > 10)
        {
            clearBuffer();
            puts("That is not a valid option Alfred");
            puts("What is the IMDB rating of the movie? (0-10)");
            scanfReturnValue = scanf("%f",&currentRating);
        }
        clearBuffer();
        char confirm;
        puts("Is this information all correct? (Enter 'Y' or 'y' if so)");
        scanf("%c",&confirm);
        if(confirm == 'Y' || confirm == 'y')
            break;
        else
        {
            puts("Starting over... \n");
            continue;
        }
    }
        //Writing to the Struct
        if (flag == 0)
        {
            hNode = (Movie *) malloc(sizeof(Movie));
            strncpy(hNode->name,currentName,99);
            hNode->year = currentYear;
            strncpy(hNode->genre,currentGenre,49);
            hNode->rating = currentRating;
            hNode->next = NULL;
            *hNodePtr = hNode;
        }
        else //It is best to trace out what happens with the nodes here on a piece of paper or on paint
        {
            int alphaFlag = 1;
            Movie * indexNode = hNode;
            Movie * tempNode, *newNode, *prevNode = NULL;
            while(indexNode != NULL)
            {
                if (strcmp(currentName,indexNode->name) < 0)
                {
                    alphaFlag = 0;

                    break;
                }
                prevNode = indexNode;
                indexNode = indexNode->next;
            }

            newNode = (Movie *) malloc(sizeof(Movie));
           
            if (alphaFlag == 0) //This means we are adding in the middle of the LinkList. If this is the case we need to tinker with the next pointers to make sure they are pointing to the correct nodes
            {        
                if(prevNode == NULL) //In this case we are replacing the head of the list
                {
                    newNode->next = indexNode;
                    *hNodePtr = newNode; //This sets our head pointer of our link list to the new node we just created
                }  
                else             //If we don't go to this if statement, then it means we are adding to the end of the list so we don't have to mess with any next pointers
                {
                    tempNode = indexNode;
                    prevNode->next = newNode;
                    newNode ->next = tempNode;
                    tempNode->next = NULL;
                }
            }
            else //This means we are adding at the end of the Link List
            {
                prevNode->next = newNode;
                newNode->next = NULL;
            }
            

            strncpy(newNode->name,currentName,99);
            newNode->year = currentYear;
            strncpy(newNode->genre,currentGenre,49);
            newNode->rating = currentRating;
        }
        
        //Increasing the number of movies
        numMovies++;
        

        //Writing to the .txt file
        fptr = fopen ("list.txt","a");
        fprintf(fptr,"%s\t%d\t%s\t%.2f\n",currentName,currentYear,currentGenre,currentRating);
        printf("Sucessfully added movie '%s' to your collection\n",currentName);

}

void printingTheLinkList(Movie *startNode)
{
    if(startNode == NULL)
    {
        puts("Your collection is empty :(\n");
        return;
    }
    printf("Your Entire Movie Collection (Number of Movies: %d)\n",numMovies);
    while(startNode->next != NULL)
    {
        puts("---------------------------------------------------------------------------------------------------------");
        printf("| Name: %s | Genre: %s | Published: %d | IMDB Rating: %.2f |\n",startNode->name,startNode->genre,startNode->year,startNode->rating);
        startNode = startNode->next;
    }
     puts("---------------------------------------------------------------------------------------------------------");
     printf("| Name: %s | Genre: %s | Published: %d | IMDB Rating: %.2f |\n",startNode->name,startNode->genre,startNode->year,startNode->rating);
     puts("---------------------------------------------------------------------------------------------------------");

}

void deletingNode(Movie *hNode,Movie **hNodePtr)
{
    char currentName[100];
    puts("What is the name of the movie?");
    scanf("%s",currentName);
    clearBuffer();
    Movie * indexNode = hNode;
    Movie * tempNode, *prevNode = NULL;
    while(indexNode != NULL)
    {
        if (strcmp(currentName,indexNode->name) == 0)
        {
            if (prevNode == NULL) //This case will occur if the node we are deleting is the head node of the list
            {
                *hNodePtr = indexNode->next;
                free(indexNode);
            }
            else if (indexNode->next == NULL) //This case will occur if the node we are deleting is the last node of the list
            {
                free(indexNode);
                prevNode->next = NULL;
            }
            else //This is for the in between
            {
                prevNode->next = indexNode->next;
                free(indexNode);
            }
            printf("Sucessfully deleted movie '%s' from your collection\n",currentName);
            numMovies--;
            return;
        }
        prevNode = indexNode;
        indexNode = indexNode->next;
    }

    puts("That movie is not in your collection :(. (Make sure you spelled the name of your movie correctly");
}

void sortingTheList(Movie * hNode, Movie ** hNodePtr)
//We are using a Selection Sort Algorithm to sort this list
//By doing this we are going to search through the list and find the oldest year a movie was published, and place it at the front of the list
//Then we are going to search through the remaining elements and find the second oldest and place that next, and so on and so far
//Each time we search we go through a smaller and smaller list. "topNode" will always be the head of this smaller list
{
    if(numMovies < 2)
    {
        puts("This list is too short to sort :(");
        return;
    }
    Movie * indexNode = hNode,*topNode = hNode;
    Movie * tempNode, *prevNode, *prevNodeOfLowestNode, *lowestNode; //We need to keep track of the lowest node as well as the node previous to the lowest node
    int count = 0;
    while(count < numMovies)
    {
        int lowest = -1;
        while(indexNode != NULL)
        {
            if (lowest == -1)
            {
                lowestNode = indexNode;
                lowest = indexNode->year;
                prevNodeOfLowestNode = NULL;
            }
            else if(indexNode->year < lowest)
            {
                lowestNode = indexNode;
                lowest = indexNode->year;
                prevNodeOfLowestNode = prevNode;
            }
            prevNode = indexNode;
            indexNode = indexNode->next;
        }
        if(count == 0) //In this case we have found the lowest node and we are putting it in front of the list AKA making it the head node
        {
            //Here we placing the node in the correct place
            tempNode = lowestNode;
            if(prevNodeOfLowestNode !=NULL)
                prevNodeOfLowestNode->next = lowestNode->next;
            lowestNode->next = hNode;
            //Here we reassign the head pointer to the lowest node
            *hNodePtr = lowestNode;
            //Here we set the index node for the following iteration (This makes sure we don't look at the node we just placed in the list)
            indexNode = lowestNode->next;
            topNode = lowestNode;
        }
        else
        {
            prevNodeOfLowestNode->next = lowestNode->next;
            tempNode = topNode->next;
            topNode->next = lowestNode;
            lowestNode->next = tempNode;
            indexNode = tempNode;
            topNode = topNode->next;
        }
        count++;
    }
    puts("Sorted list by Year Published");
}