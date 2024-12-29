#include <stdio.h>

#define MAX_ROWS 10000
#define NUM_COLUMNS 10 // Total number of columns in table.txt

int main() 
{
    double myarray[MAX_ROWS][NUM_COLUMNS];
    FILE *inputFile, *outputFile;
    int row = 0;

    inputFile = fopen("table.txt", "r");
    if(inputFile == NULL) 
    {
        printf("Error opening input file.\n");
        return 1;
    }

    // Read data from input file to memory
    while (row < MAX_ROWS &&
           fscanf(inputFile, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                  &myarray[row][0], &myarray[row][1], &myarray[row][2], &myarray[row][3],
                  &myarray[row][4], &myarray[row][5], &myarray[row][6], &myarray[row][7],
                  &myarray[row][8], &myarray[row][9]) == NUM_COLUMNS) 
    {
        row++;
    }

    fclose(inputFile);

    outputFile = fopen("tableout.txt", "w");
    if (outputFile == NULL) 
    {
        printf("Error opening output file.\n");
        return 1;
    }

    // Write data to output file (2nd and 8th columns only)
    for (int i = 0; i < row; i++) 
    {
        fprintf(outputFile, "%lf %lf\n", myarray[i][1], myarray[i][7]);
    }

    fclose(outputFile);
    return 0;
}
