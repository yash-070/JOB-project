#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include"food_struct.h"
#include<time.h>
#define MAX_NUM 100

void shuffle(int arr[], int n);
Food *read_food_data(int *count);
Food *insert(Food *first, char a[MAX_NAME], float x, float y, float z, float b);
void search (Food *point, char foodname[MAX_NAME]);
void free_list( Food *head);

int main()
{   
    unsigned int seed_counter = 0;
    srand((unsigned int)(time(NULL) ^ clock() ^ (seed_counter++)));

    int count;
    Food *foods = read_food_data(&count);

    int indices[MAX_NUM];
    for (int i = 0; i < MAX_NUM; i++) {
        indices[i] = i;
    }
    shuffle(indices,MAX_NUM);

    printf("TRY THESE INGREDENTS:\n");
    for (int i = 0; i < 5; i++) {
        int idx = indices[i];
        printf("%d. %s: %.1f kcal, %.1f g protein, %.1f g fat, %.1f g carbs\n",
               i + 1, foods[idx].food_name, foods[idx].calories,
               foods[idx].proteins, foods[idx].fat, foods[idx].carbs);
    }
    free(foods);
 


    Food *head = NULL;
    FILE *fp = fopen("food_data.txt", "r");
    if (fp == NULL) {
        printf("ERROR IN IMPORTING DATA FROM FILE!\n");
        exit(1);
    }

    
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        char name[MAX_NAME] = "";
        float cal = 0, pro = 0, fat = 0, carbs = 0;
        char *token = strtok(line, " \n");
        while (token != NULL) {
            if (sscanf(token, "%f", &cal) == 1) {
                token = strtok(NULL, " \n");
                if (token) sscanf(token, "%f", &pro);
                token = strtok(NULL, " \n");
                if (token) sscanf(token, "%f", &fat);
                token = strtok(NULL, " \n");
                if (token) sscanf(token, "%f", &carbs);
                break;
            } else {
                if (strlen(name) > 0) strcat(name, " ");
                strcat(name, token);
                token = strtok(NULL, " \n");
            }
        }
        if (cal != 0 || pro != 0 || fat != 0 || carbs != 0) {
            head = insert(head, name, cal, pro, fat, carbs);
        }
    }
    fclose(fp);

    
    char choice;
    printf("\nSearching for any food ingredient? (y|n): ");
    scanf(" %c", &choice);
    while (choice != 'n' && choice != 'N') {
        if (choice == 'y' || choice == 'Y') {
            printf("\nEnter the food ingredient name: ");
            char foodname[50];
            scanf(" %[^\n]", foodname); 
            search(head, foodname);
            printf("\nDo you want to search another food ingredient? (y|n): ");
            scanf(" %c", &choice);
        } else {
            printf("Invalid choice\n");
            printf("Searching for any food ingredient? (y|n): ");
            scanf(" %c", &choice);
        }
    }
    
    free_list(head);
    printf("Exiting!\n");
    return 0;


}
Food *read_food_data(int *count)
{

        Food *foods = (Food*)malloc(MAX_NUM * sizeof(Food));
        if (foods == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }
    
        FILE *fp = fopen("food_data.txt", "r");
        if (fp == NULL) {
            printf("Error opening file\n");
            free(foods);
            exit(1);
        }
    
        *count = 0;
        char line[100];
        while (*count < MAX_NUM && fgets(line, sizeof(line), fp)) {
           
            char *token = strtok(line, " \n");
            char temp_name[MAX_NAME] = "";
            float calories = 0, protein = 0, fat = 0, carbs = 0;
            int field_count = 0;
    
           
            while (token != NULL) 
            {
             
                if (sscanf(token, "%f", &calories) == 1)
                {
                    token = strtok(NULL, " \n");
                    if (token) sscanf(token, "%f", &protein);
                    token = strtok(NULL, " \n");
                    if (token) sscanf(token, "%f", &fat);
                    token = strtok(NULL, " \n");
                    if (token) sscanf(token, "%f", &carbs);
                    break;
                } 
                else 
                {
                    if (strlen(temp_name) > 0) strcat(temp_name, " ");
                    strcat(temp_name, token);
                    token = strtok(NULL, " \n");
                }
            }
    
          
            if (calories != 0 || protein != 0 || fat != 0 || carbs != 0) {
                strncpy(foods[*count].food_name, temp_name, MAX_NAME - 1);
                foods[*count].food_name[MAX_NAME - 1] = '\0';
                foods[*count].calories = calories;
                foods[*count].proteins = protein;
                foods[*count].fat = fat;
                foods[*count].carbs = carbs;
                (*count)++;
            }
        }
        rewind(fp);
        fclose(fp);
        return foods;
}

void shuffle(int arr[], int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

Food *insert( Food *first, char a[50], float x, float y, float z, float b) {
    Food *temp = (Food*)malloc(sizeof(Food));
       if (temp == NULL) {
           printf("Memory allocation failed\n");
           exit(1);
       }
       strncpy(temp->food_name, a, MAX_NAME - 1);
       temp->food_name[MAX_NAME - 1] = '\0';
       temp->calories = x;
       temp->proteins = y;
       temp->fat = z;
       temp->carbs = b;
       temp->link = NULL;
       if (first == NULL) {
           first = temp;
       } else {
           temp->link = first;
           first = temp;
       }
       return first;
   }
   
   void search(Food *point, char foodname[50]) {
       int found = 0;
       while (point != NULL) {
           if (strcasecmp(point->food_name, foodname) == 0) {
               printf("%s: %.2f kcal, %.2f g protein, %.2f g fat, %.2f g carbs\n",
                      point->food_name, point->calories, point->proteins, point->fat, point->carbs);
               found = 1;
               break;
           }
           point = point->link;
       }
       if (!found) {
           printf("Food '%s' not found\n", foodname);
       }
   }
   
   void free_list(Food *head) {
       Food *current = head;
       while (current != NULL) {
           Food *temp = current;
           current = current->link;
           free(temp);
       }
   }

   