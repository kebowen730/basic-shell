//Kenneth Bowen
//CS 3320
//

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <limits.h>

#define FILE_NUMBER 1024
#define FILE_NAME 2048

//bubble sort
void sort_array(char s[FILE_NUMBER][FILE_NAME], int *index_inc) {
	int i;
	int j;
    for(i=0; i < (*index_inc)-1; i++) {
        for(j = i+1; j < (*index_inc); j++) {
            if(strcmp(s[i], s[j]) > 0) {
                char temp[FILE_NAME];
                strcpy(temp, s[i]);
                strcpy(s[i], s[j]);
                strcpy(s[j], temp);
            }
        }
    }
}

//clear input
void clearInputBuffer(char c) {

    do {
        c = getchar();

    } while(c != '\n' && c != EOF);
}

void read_dir(char s[FILE_NUMBER][FILE_NAME], int *index_inc, DIR *d, struct dirent *de, int flag) {

    int d_type = (flag == 0) ? DT_DIR:DT_REG;

    while((de = readdir(d))) {
        if((de->d_type) & d_type) {
            strcpy(s[*index_inc], de->d_name);
            (*index_inc)++;
        }
    }
}

int main(void) {
    
    
    pid_t child; //Getting process id
    DIR * d;
    struct dirent * de;
    int i, c, k;
    char s[256], cmd[256];
    struct stat buffer;
    time_t t;

    int f_view = 8;
    int f_start_view = 0;
    int d_view = 8;
    int d_start_view = 0;
    char file_s[FILE_NUMBER][FILE_NAME];
    char directory_s[FILE_NUMBER][FILE_NAME];
    int farray_size = 0;
    int darray_size = 0;
    d = opendir(".");
    read_dir(file_s, &farray_size, d, de, 1);
    d = opendir(".");
    read_dir(directory_s, &darray_size, d, de, 0);

    while(1) {
        system("clear");
        t = time(NULL); //getting system time
        printf("\n-----------------------------------------------------\n");
        getcwd(s, 200);
        printf("Current Working Dir: %s \n", s);
        printf("It is now: %s\n", ctime(&t));

        //Reading the files in the current directory.
        c = 0;
        printf("\t\tFILES ( %d / %d )\n\n", f_view/8, (farray_size/8 > 0) ? farray_size/8:1);
        for(i = f_start_view; i < farray_size; i++) {

            if(i <= f_view) {
                stat(file_s[i], &buffer);
                printf("%d", i);
                printf(" | %s \n", file_s[i]);
            }
        }

        printf("\n");
        //Reading subdirectories
        printf("\t\tDIRECTORIES ( %d / %d ) \n\n", d_view/8, (darray_size/8 > 0) ? darray_size/8:1);
        for(i = d_start_view; i < darray_size; i++) {

            if(i <= d_view) {
                stat(directory_s[i], &buffer);
                printf("%d", i);
                printf(" | %s \n", directory_s[i]);
            }
        }

        printf("\n");

        printf("-----------------------------------------------------\n");

        printf("E. Edit\n");
        printf("N. Next\n");
        printf("P. Previous\n");
        printf("R. Run  \n");
        printf("S. Sort\n");
        printf("Q. Quit\n");
        printf("-----------------------------------------------------\n");
        
        printf("\n");
        printf("~$: ");

        c = getchar(); getchar();

        c = tolower(c);

        int index_arr[darray_size];
        switch(c) {               //implements menu options
            case 'q': exit(0);
            case 'e': printf("Edit what?: ");
                scanf("%s", s);
                strcpy(cmd, "vim ");
                strcat(cmd, s);
                system(cmd);
                break;
            case 'r':
                printf("Run what?: ");
                scanf("%[^\n]s", cmd);
                system(cmd);
                printf("\nPress any key to continue\n");
                getchar();
                break;
           
            case 'n':
                printf("-----------------------------------------------------\n");
                printf("D. Directory \t F. Files\n");
                printf("-----------------------------------------------------\n");
                printf("~$: ");
                int input = getchar();
                input = tolower(input);
                switch(input) {
                    case 'd':
                        d_view = (d_view+8 < darray_size) ? d_view+8 : darray_size;
                        d_start_view = (d_start_view+8 < darray_size) ? d_start_view+8 : d_start_view;
                        break;
                    case 'f':
                        f_view = (f_view+8 < farray_size) ? f_view+8 : farray_size;
                        f_start_view = (f_start_view+8 < farray_size) ? f_start_view+8 : f_start_view;
                        break;
                }
                break;
            case 'p':
                printf("-----------------------------------------------------\n");
                printf("D. Directory \t F. Files\n");
                printf("-----------------------------------------------------\n");
                printf("~$: ");
                int input2 = getchar();
                input2 = tolower(input2);
                switch(input2) {
                    case 'd':
                        d_view = (d_view-8 > 8) ? d_view-8 : 8;
                        d_start_view = (d_start_view-8 > 0) ? d_start_view-8 : 0;
                        break;
                    case 'f':
                        f_view = (f_view-8 > 8) ? d_view+8 : 8;
                        f_start_view = (f_start_view-8 > 0) ? f_start_view-8 : 0;
                        break;
                }
                break;

            case 's':
                printf("-----------------------------------------------------\n");
                printf("D. Directory \t F. Files\n");
                printf("-----------------------------------------------------\n");
                printf("~$: ");
                int input3 = getchar();
                input3 = tolower(input3);
                switch(input3) {
                    case 'd':
                        sort_array(directory_s, &darray_size);
                        break;
                    case 'f':
                        sort_array(file_s, &farray_size);
                        break;
                }
                break;

        }
        //clearing input buffer:
        clearInputBuffer(c);
    }
} 
