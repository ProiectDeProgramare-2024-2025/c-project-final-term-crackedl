#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROPERTIES 50
#define FILENAME "properties.txt"
#define FAV_FILENAME "favorites.txt"

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"


typedef struct {
    int id;
    int type;
    int price;
    int size;
    int room;
    int bathrooms;
    int parking;
    int sold;
} Property;

Property properties[MAX_PROPERTIES];
Property *favorites;
int fav_count = 0;

void save_properties_to_file() {
    FILE *file = fopen(FILENAME, "w");
    if (file) {
        for (int i = 0; i < MAX_PROPERTIES; i++) {
            fprintf(file, "%d %d %d %d %d %d %d %d\n", properties[i].id, properties[i].type, properties[i].price, properties[i].size, properties[i].room, properties[i].bathrooms, properties[i].parking, properties[i].sold);
        }
        fclose(file);
    }
}

void save_favorites_to_file() {

    FILE *file = fopen(FAV_FILENAME, "w");
    if (file) {
        for (int i = 0; i < fav_count; i++) {
            fprintf(file, "%d %d %d %d %d %d %d %d\n", favorites[i].id, favorites[i].type, favorites[i].price, favorites[i].size, favorites[i].room, favorites[i].bathrooms, favorites[i].parking, favorites[i].sold);
        }
        fclose(file);
    }
}

void load_properties_from_file() {
    FILE *file = fopen(FILENAME, "r");
    if (file) {
        for (int i = 0; i < MAX_PROPERTIES; i++) {
            fscanf(file, "%d %d %d %d %d %d %d %d", &properties[i].id, &properties[i].type, &properties[i].price, &properties[i].size, &properties[i].room, &properties[i].bathrooms, &properties[i].parking, &properties[i].sold);
        }
        fclose(file);
    } else {
        for (int i = 0; i < MAX_PROPERTIES; i++) {
            properties[i].id = i + 1;
            properties[i].type = rand() % 2;
            properties[i].price = rand() % 150000 + 50000;
            properties[i].size = rand() % 140 + 40;
            properties[i].room = rand() % 5 + 1;
            properties[i].bathrooms = rand() % properties[i].room + 1;
            properties[i].parking = rand() % 2;
            properties[i].sold = 0;
        }
        save_properties_to_file();
    }
}

void load_favorites_from_file() {
    FILE *file = fopen(FAV_FILENAME, "r");
    if (file) {
        fav_count = 0;
        favorites = (Property*)malloc(0);
        Property temp;
        while (fscanf(file, "%d %d %d %d %d %d %d %d", &temp.id, &temp.type, &temp.price, &temp.size, &temp.room, &temp.bathrooms, &temp.parking, &temp.sold) != EOF) {
            favorites = (Property*)realloc(favorites, (fav_count + 1) * sizeof(Property));
            favorites[fav_count++] = temp;
        }
        fclose(file);
    } else {
        favorites = (Property*)malloc(0);
    }
}

void display_properties(Property *list, int count) {
    system("cls");
    printf("\n----- PROPERTY LISTINGS -----\n");
    for (int i = 0; i < count; i++) {
        if (list[i].sold) continue;
        printf("\n[%d] %s%s%s - %s$%d%s, %d sqm, %d rooms, %d bathrooms, Parking: %s%s%s\n",
        list[i].id,
        CYAN, list[i].type ? "House" : "Apartment", RESET,
        GREEN, list[i].price, RESET,
        list[i].size, list[i].room, list[i].bathrooms,
        YELLOW, list[i].parking ? "Yes" : "No", RESET);

    }
}

void add_to_favorites(int id) {
    for (int i = 0; i < fav_count; i++) {
        if (favorites[i].id == id) {
            system("cls");
            printf("\nProperty already in favorites!\n");
            return;
        }
    }
    favorites = (Property*)realloc(favorites, (fav_count + 1) * sizeof(Property));
    favorites[fav_count++] = properties[id - 1];
    save_favorites_to_file();
    system("cls");
    printf("\nProperty added to favorites!\n");
}

void buy_property(int id) {
    for (int i = 0; i < MAX_PROPERTIES; i++) {
        if (properties[i].id == id && !properties[i].sold) {
            properties[i].sold = 1;
            system("cls");
            printf("\nProperty %d purchased successfully!\n", id);
            save_properties_to_file();
            return;
        }
    }
    system("cls");
    printf("\nInvalid selection!\n");
}

void menu() {
    int choice, id;
    system("cls");
    do {
        printf("\n1) See Properties\n2) See Favorites\n3) Buy Property\n4) Add Favorite\n5) Exit\nChoose an option: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                display_properties(properties, MAX_PROPERTIES);
                break;
            case 2:
                display_properties(favorites, fav_count);
                break;
            case 3:
                printf("Enter property ID to buy: ");
                scanf("%d", &id);
                buy_property(id);
                break;
            case 4:
                printf("Add property ID to favorites: ");
                scanf("%d", &id);
                add_to_favorites(id);
                break;
            case 5:
                printf("\nGoodbye!\n");
                break;
            default:
                printf("\nInvalid option!\n");
        }
    } while (choice != 5);
}

int main() {
    srand(time(NULL));
    load_properties_from_file();
    load_favorites_from_file();
    menu();
    free(favorites);
    return 0;
}
