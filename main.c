#include "fatal.h"
#include "func.h"
#include <stdio.h>
#include <windows.h>
#define MAX_STATION 50

int main()
{
    FILE *fq = NULL;
    int i, Station_Number;
    double Tank_Capacity, Running_Distance_Per_Unit_Oil, Destination_Distance, Origin_Price_Per_Unit_Oil;
    Oil_Station Stations[MAX_STATION];
    // Open data file
    if ((fq = fopen("./Data/Data.TXT", "r")) == NULL)
        Error("Fail to open file!\n");
    while (1)
    {
        printf("Type 1 to read data , 0 to exit\n>");
        scanf("%d", &i);
        switch (i)
        {
        case 0:
            system("pause");
            return 0;
        case 1:
            // Read data in the data file
            if (fscanf(fq, "%lf %lf %lf", &Tank_Capacity, &Destination_Distance, &Running_Distance_Per_Unit_Oil) == EOF)
                Error("Read EOF of data file\n");
            // Read data in the data file
            fscanf(fq, "%d", &Station_Number);
            for (i = 0; i < Station_Number; i++)
            {
                // Read data in the data file
                fscanf(fq, "%lf %lf\n", &Stations[i].Price_Per_Unit_Oil, &Stations[i].Distance_To_Origin);
                Stations[i].Id = i;
            }
            // Using Greedy Algorithm to find answer
            Greedy_Algorithm(Tank_Capacity, Running_Distance_Per_Unit_Oil, Destination_Distance, Stations, Station_Number);
            break;
        default:
            break;
        }
    }
    fclose(fq);
    system("pause");
    return 0;
}
