#include<stdio.h>
#define MAX_NAME 50
typedef struct food_data
{
    char food_name[MAX_NAME];
    float calories;
    float proteins;
    float fat;
    float carbs;
    struct food *link;
}Food;

Food *insert(Food *first,char a[MAX_NAME],float x,float y,float z,float b );