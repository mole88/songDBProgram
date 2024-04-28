#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 2048
#define INFO_FILE_PATH "./info.txt"
#define DB_FILE_PATH "./db.csv"

struct SongStruct{
    int ID;
    char *Name;
    int AuthorsCount;
    char **Authors;
    char *Album;
    char *Lang;
    char *Genre;
    int Year;
};
typedef struct SongStruct Song;

void printMenu();
void clearScreen();
void stop();
void nGets(char str[], int lim);
void getTextFromFile(char str[], char *path, int lim);
void printInfo();
Song* getDB(char *path, int *dbSize);
Song creatSong(char **params);
char **split(char *str, char sep);
void printSong(Song song);
void printDB(Song *db, int dbSize);
void printHeader();
char** inputSong();
void menuAdd(Song **db, int *dbSize);
void addSong(Song **db, int *dbSize, Song *newSong);
char *songToString(Song song);
void saveDB(char *path, Song *db, int dbSize);
void menuDelete(Song **db, int *dbSize);
int deleteSong(Song **db, int songID, int *dbSize);
void freeSongMem(Song *song);
void menuEdit(Song **db, int dbSize);
int compareByID(const void *a, const void *b);
int compareByYear(const void *a, const void *b);
int compareByName(const void *a, const void *b);
int compareByAlbum(const void *a, const void *b);
int compareByLang(const void *a, const void *b);
int compareByGenre(const void *a, const void *b);
Song *findSongs(Song *db, int dbSize, void *target, int (*compare)(const void*, const void*), int *resultSize);
void menuFind(Song *db, int dbSize);
int enterField();
void menuSort(Song *db, int dbSize);
void editSong(Song *song);

int main() {
    int menuItem;
    int working = 1;
    Song *db = NULL;
    int dbSize = 0;

    db = getDB(DB_FILE_PATH, &dbSize);
    while (working){
        printMenu();
        scanf("%d", &menuItem);
        clearScreen();

        switch (menuItem) {
            case 1: printInfo(); break; //info
            case 2: menuAdd(&db, &dbSize); break;   //add song
            case 3: menuEdit(&db, dbSize); break;  //edit song
            case 4: menuDelete(&db, &dbSize); break;   //delete song
            case 5: printDB(db, dbSize); break;   //add song
            case 6: menuFind(db, dbSize); break;   //find by
            case 7: menuSort(db, dbSize); break; //sort by
            case 8: working = 0; break;   //exit
            default:
                puts("Bad enter");
                break;
        }
        stop();
        clearScreen();
        fflush(stdin);
    }
    return 0;
}
void printMenu(){
    puts("1 - Info\n2 - Add song\n3 - Edit song\n4 - Delete song\n"
         "5 - Print all\n6 - Find by...\n7 - Sort by...\n8 - Exit");
}
void clearScreen(){
    system("cls");
}
void stop(){
    system("pause");
}
void printInfo(){
    char info[STR_SIZE];
    getTextFromFile(info, INFO_FILE_PATH, STR_SIZE);
    puts(info);
}
void getTextFromFile(char str[], char *path, int lim){
    FILE *file = fopen(path, "a+t");
    char c;
    int i = 0;
    while(((c = getc(file)) != EOF) && (i < lim-1)){
        str[i]=c;
        i++;
    }
    fclose(file);
}
void nGets(char str[], int lim){
    char curChar;
    int i = 0;
    while(((curChar=getchar())!='\n')&&(i<lim-1))
    {
        str[i]=curChar;
        i++;
    }
    str[i]='\0';
}
int countLinesInFile(char *path) {
    FILE *file;
    char buffer[STR_SIZE];
    int linesCount = 0;

    file = fopen(path, "r");
    if (file == NULL)
        linesCount = -1;

    while (fgets(buffer, sizeof(buffer), file) != NULL)
        linesCount++;
    fclose(file);

    return linesCount;
}
Song *getDB(char *path, int *dbSize){
    FILE *file = fopen(path, "r");
    int linesCount = countLinesInFile(path)-1;
    char line[STR_SIZE];

    Song *db = malloc(sizeof(Song)*linesCount);
    Song curSong;
    fgets(line, STR_SIZE, file);

    int i = 0;
    while (fgets(line, STR_SIZE, file)){
        curSong = creatSong(split(line, ','));
        db[i] = curSong;
        i++;
    }
    *dbSize = linesCount;

    fclose(file);
    return db;
}
Song creatSong(char **params){
    Song song;

    song.ID = atoi(params[0]);
    song.Name = params[1];
    song.AuthorsCount = atoi(params[2]);
    song.Authors = split(params[3], ';');
    song.Album = params[4];
    song.Lang = params[5];
    song.Genre = params[6];
    song.Year = atoi(params[7]);

    return song;
}
char **split(char *str, char sep)
{
    int wordsCount = 1;
    int len = strlen(str);
    int ind = 0;
    int div = 0;
    char **result = NULL;

    for (int i = 0; i < len; i++)
        if (str[i] == sep)
            wordsCount++;
    result = (char**)malloc(sizeof(char*)*wordsCount);

    for (int j = 0; j < len; ++j) {
        if (str[j] == sep || str[j] == '\n'){
            result[ind] = (char*)malloc(sizeof(char)*STR_SIZE);
            strncpy(result[ind], &str[div], j-div);
            result[ind][j-div] = '\0';
            div = j + 1;
            ind++;
        }
    }
    return result;
}
void printSong(Song song){
    char authorsStr[STR_SIZE];
    authorsStr[0] = '\0';
    strcat(authorsStr, song.Authors[0]);
    for (int i = 1; i < song.AuthorsCount; ++i){
        strcat(authorsStr, ", ");
        strcat(authorsStr, song.Authors[i]);
    }
    printf("%-3d | %-30s | %-40s | %-35s | %-15s | %-15s | %-5d\n",
            song.ID, song.Name, authorsStr, song.Album, song.Lang, song.Genre, song.Year);
}
void printDB(Song *db, int dbSize){
    printHeader();
    for (int i = 0; i < dbSize; ++i)
        printSong(db[i]);
    for (int j = 0; j < 10; ++j)
        printf("%s", "----------------");
    printf("\nSize: %d\n\n", dbSize);
}
void printHeader(){
    printf("%-3s | %-30s | %-40s | %-35s | %-15s | %-15s | %-5s \n",
            "ID", "Name", "Authors", "Album", "Language", "Genre", "Year");
    printf("%-3s | %-30s | %-40s | %-20s | %-15s | %-15s | %-5s \n",
            "---", "------------------------------", "----------------------------------------",
            "-----------------------------------", "---------------", "---------------", "---");
}
void menuAdd(Song **db, int *dbSize){
    Song newSong = creatSong(inputSong());
    addSong(db, dbSize, &newSong);
    saveDB(DB_FILE_PATH, *db, *dbSize);
}
char **inputSong(){
    char **params = NULL;
    char inStr[STR_SIZE];
    char authorsStr[STR_SIZE];
    int authorsCount;
    char menuItems[][STR_SIZE] = {"ID", "name", "authors count", "authors", "album", "language", "genre", "year"};
    authorsStr[0] = '\0';

    params = malloc(sizeof(char*)*8);
    for (int j = 0; j < 8; ++j)
        params[j] = malloc(sizeof(char)*2024);

    fflush(stdin);
    for (int k = 1; k < 8; ++k) {
        if (k == 3){
            for (int i = 0; i < authorsCount; ++i) {
                printf("Enter author %d: ", i+1);
                nGets(inStr, STR_SIZE);
                strcat(authorsStr, inStr);
                strcat(authorsStr, ";");
            }
            strcpy(params[3], authorsStr);
        }
        else{
            printf("Enter song %s: ", menuItems[k]);
            nGets(inStr, STR_SIZE);
            strcpy(params[k], inStr);
            if (k == 2)
                authorsCount = atoi(inStr);
        }
    }
    return params;
}
void addSong(Song **db, int *dbSize, Song *newSong){
    newSong->ID = (*db)[*dbSize-1].ID + 1;
    *db = realloc(*db, sizeof(Song) * (*dbSize + 1));
    (*db)[*dbSize] = *newSong;
    (*dbSize)++;
}
void saveDB(char *path, Song *db, int dbSize){
    FILE *file = fopen(path, "wt");
    fputs("ID,name,authorsCount,authors,album,lang,genre,year\n" ,file);
    for (int i = 0; i < dbSize; ++i)
        fputs(songToString(db[i]), file);
    fclose(file);
}
char *songToString(Song song){
    char *line = malloc(sizeof(char)*STR_SIZE);
    char authorsStr[STR_SIZE];
    authorsStr[0] = '\0';

    for (int i = 0; i < song.AuthorsCount; ++i) {
        strcat(authorsStr, song.Authors[i]);
        strcat(authorsStr, ";");
    }
    sprintf(line, "%d,%s,%d,%s,%s,%s,%s,%d\n",
            song.ID, song.Name, song.AuthorsCount, authorsStr, song.Album, song.Lang, song.Genre, song.Year);
    return line;
}
int deleteSong(Song **db, int songID, int *dbSize) {
    int dFlag = 0;
    for (int i = 0; i < *dbSize; ++i) {
        if (dFlag)
            (*db)[i - 1] = (*db)[i];
        else if ((*db)[i].ID == songID) {
            freeSongMem(&((*db)[i]));
            dFlag = 1;
        }
    }
    if (dFlag) {
        *db = realloc(*db, sizeof(Song) * (*dbSize - 1));
        *dbSize = *dbSize - 1;
    }
    return dFlag;
}
void menuDelete(Song **db, int *dbSize){
    int songID;
    puts("Enter song ID for delete: ");
    scanf("%d", &songID);

    if(deleteSong(db, songID, dbSize))
        puts("Success!!");
    else puts("Song not found");

    saveDB(DB_FILE_PATH, *db, *dbSize);
}
void freeSongMem(Song *song){
    free(song->Name);
    free(song->Genre);
    free(song->Lang);
    free(song->Album);
    for (int i = 0; i < song->AuthorsCount; ++i)
        free(song->Authors[i]);
    free(song->Authors);
}
void menuEdit(Song **db, int dbSize){
    int songID;
    Song *curSong;
    puts("Enter song ID for edit: ");
    scanf("%d", &songID);

    for (int i = 0; i < dbSize; ++i) {
        curSong = &(*db)[i];
        if (curSong->ID == songID){
            printSong(*curSong);
            editSong(curSong);
            break;
        }
    }
}
void editSong(Song *song){
    int fieldNum;
    int intParam;
    char strParam[STR_SIZE];

    fieldNum = enterField();
    switch (fieldNum) {
        case 1: //ID
            puts("You cant edit this!");
            break;
        case 2: //Name
            nGets(strParam, STR_SIZE);
            song->Name = strParam;
            break;
        case 3: //Author
            puts("Enter authors count: ");
            scanf("%d", &intParam);
            song->AuthorsCount = intParam;

            song->Authors = malloc(sizeof(char*)*intParam);
            for (int i = 0; i < intParam; ++i) {
                song->Authors[i] = malloc(sizeof(char)*STR_SIZE);
                printf("Enter author %d: ", i+1);
                nGets(strParam, STR_SIZE);
                song->Authors[i] = strParam;
            }
            break;
        case 4: //Album
            nGets(strParam, STR_SIZE);
            song->Album = strParam;
            break;
        case 5: //Lang
            nGets(strParam, STR_SIZE);
            song->Lang = strParam;
            break;
        case 6: //Genre
            nGets(strParam, STR_SIZE);
            song->Genre = strParam;
            break;
        case 7: //Year
            scanf("%d", &intParam);
            song->Year = intParam;
            break;
        default:
            puts("Bad enter");
            return;
    }
}
int compareByID(const void *a, const void *b){
    int result = ((Song*)a)->ID - ((Song*)b)->ID;
    return result;
}
int compareByYear(const void *a, const void *b){
    int result = ((Song*)a)->Year - ((Song*)b)->Year;
    return result;
}
int compareByName(const void *a, const void *b){
    int result = strcmp(((Song*)a)->Name, ((Song*)b)->Name);
    return result;
}
int compareByAlbum(const void *a, const void *b){
    int result = strcmp(((Song*)a)->Album, ((Song*)b)->Album);
    return result;
}
int compareByLang(const void *a, const void *b){
    int result = strcmp(((Song*)a)->Lang, ((Song*)b)->Lang);
    return result;
}
int compareByGenre(const void *a, const void *b){
    int result = strcmp(((Song*)a)->Genre, ((Song*)b)->Genre);
    return result;
}
int compareByAuthor(const void *a, const void *b){
    int result;
    int flag = 0;
    char authors1Str[STR_SIZE];
    char authors2Str[STR_SIZE];
    authors1Str[0] = '\0';
    authors2Str[0] = '\0';
    Song *song1 = (Song*)a;
    Song *song2 = (Song*)b;

    for (int i = 0; i < song1->AuthorsCount; ++i) {
        for (int j = 0; j < song2->AuthorsCount; ++j) {
            if (strcmp(song1->Authors[i], song2->Authors[j])==0){
                flag = 1;
                break;
            }
        }
        if (flag) break;
    }
    if (flag) result = 0;
    else{
        for (int i = 0; i < song1->AuthorsCount; ++i)
            strcat(authors1Str, song1->Authors[i]);

        for (int j = 0; j < song2->AuthorsCount; ++j)
            strcat(authors2Str, song2->Authors[j]);

        result = strcmp(authors1Str, authors2Str);
    }
    return result;
}
Song *findSongs(Song *db, int dbSize, void *target, int (*compare)(const void*, const void*), int *resultSize)
{
    Song *findSongs = NULL;
    int foundCount = 0;

    for (int i = 0; i < dbSize; ++i) {
        if (compare(target, &db[i]) == 0){
            findSongs = realloc(findSongs, sizeof(Song)*(foundCount+1));
            findSongs[foundCount++] = db[i];
        }
    }
    *resultSize = foundCount;
    return findSongs;
}
void menuFind(Song *db, int dbSize){
    int fieldNum = enterField();
    int intParam;
    char strParam[STR_SIZE];
    Song target; //song struct to usu in compare func
    Song *result = NULL;
    int resultSize = 0;

    puts("Enter search: ");
    fflush(stdin);
    switch (fieldNum) {
        case 1: //ID
            scanf("%d", &intParam);
            target.ID = intParam;
            result = findSongs(db, dbSize, &target, compareByID, &resultSize);
            break;
        case 2: //Name
            nGets(strParam, STR_SIZE);
            target.Name = strParam;
            result = findSongs(db, dbSize, &target, compareByName, &resultSize);
            break;
        case 3: //Author
            nGets(strParam, STR_SIZE);
            target.AuthorsCount = 1;
            target.Authors = malloc(sizeof(char)*2048);
            target.Authors[0] = strParam;
            result = findSongs(db, dbSize, &target, compareByAuthor, &resultSize);
            break;
        case 4: //Album
            nGets(strParam, STR_SIZE);
            target.Album = strParam;
            result = findSongs(db, dbSize, &target, compareByAlbum, &resultSize);
            break;
        case 5: //Lang
            nGets(strParam, STR_SIZE);
            target.Lang = strParam;
            result = findSongs(db, dbSize, &target, compareByLang, &resultSize);
            break;
        case 6: //Genre
            nGets(strParam, STR_SIZE);
            target.Genre = strParam;
            result = findSongs(db, dbSize, &target, compareByGenre, &resultSize);
            break;
        case 7: //Year
            scanf("%d", &intParam);
            target.Year = intParam;
            result = findSongs(db, dbSize, &target, compareByYear, &resultSize);
            break;
        default:
            puts("Bad enter");
            return;
    }
    clearScreen();
    printDB(result, resultSize);
}
int enterField(){
    int fieldNum;
    puts("Enter parameter: ");
    puts("1 - ID\n2 - Name\n3 - Author\n4 - Album\n5 - Language\n6 - Genre\n7 - Year");
    scanf("%d", &fieldNum);

    return fieldNum;
}
void menuSort(Song *db, int dbSize){
    int fieldNum = enterField();

    switch (fieldNum) {
        case 1: //ID
            qsort(db, dbSize, sizeof(Song), compareByID);
            break;
        case 2: //Name
            qsort(db, dbSize, sizeof(Song), compareByName);
            break;
        case 3: //Author
            qsort(db, dbSize, sizeof(Song), compareByAuthor);
            break;
        case 4: //Album
            qsort(db, dbSize, sizeof(Song), compareByAlbum);
            break;
        case 5: //Lang
            qsort(db, dbSize, sizeof(Song), compareByLang);
            break;
        case 6: //Genre
            qsort(db, dbSize, sizeof(Song), compareByGenre);
            break;
        case 7: //Year
            qsort(db, dbSize, sizeof(Song), compareByYear);
            break;
        default:
            puts("Bad enter");
            return;
    }
    clearScreen();
    printDB(db, dbSize);
}