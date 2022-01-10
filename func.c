#include "func.h"
#include "fatal.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// state == 1 means that the car can arrive the destination
// state == 0 means that the car can't arrive the destination
void Show(int *Station_State, int Station_Num, int state, Oil_Station *Station)
{
    int i, o, tmp = 0;
    for (i = 0; i < Station_Num; i++)
    {
        printf(" %-3.0lf   ", Station[i].Distance_To_Origin);
    }
    printf("\b\b\b------------>The distance from the station to starting point\n");
    for (i = 0; i < Station_Num; i++)
    {
        printf(" %-3.1lf   ", Station[i].Price_Per_Unit_Oil);
    }
    printf("\b\b\b------------>The oil price of each station\n|S|");
    for (i = 0; i < Station_Num - 1; i++)
        printf("----|-|");
    printf("----|E|\n");
    printf(" |");
    for (i = 1; i < Station_Num; i++)
    {
        if (Station_State[i] == 1)
        {

            for (o = 0; o < i - tmp; o++)
                printf("______");
            for (o = 0; o < i - tmp - 1; o++)
                printf("_");
            tmp = i;
            printf("|");
        }
    }
    if (state == 1)
    {
        for (o = 0; o < Station_Num - tmp; o++)
            printf("______");
        for (o = 0; o < Station_Num - tmp - 1; o++)
            printf("_");
        printf("|  ----------->Travel path");
    }
    else
        printf("___E    (E means the ending point,the car can only arrive here)");
    printf("\n");
}

void Swap(Oil_Station *a, Oil_Station *b)
{
    Oil_Station tmp = *a;
    *a = *b;
    *b = tmp;
}

void Greedy_Algorithm(double Tank_Capacity, double Running_Distance_Per_Unit_Oil, double Destination_Distance, Oil_Station *Station, int Station_Num)
{
    if (!Station)
        Error("Invalid Station Information\n");
    int flag, state = 0, i, o;
    int *Station_State = (int *)malloc(sizeof(int) * Station_Num);
    Oil_Station *Station_Copy = (Oil_Station *)malloc(sizeof(Oil_Station) * Station_Num);
    memcpy(Station_Copy, Station, sizeof(Oil_Station) * Station_Num);
    double Distance, Price, Oil_Consumption, Total_Distance;
    double Cur_Oil, Distance_Tmp;
    Oil_Station Cur_Station = Station[0];
    // Array Station saved the information of Stations
    // is ordered By ascending order of price of unit oil
    for (i = 0; i < Station_Num; i++)
    {
        for (o = i; o < Station_Num; o++)
        {
            if (Station[o].Price_Per_Unit_Oil < Station[i].Price_Per_Unit_Oil)
                Swap(&Station[o], &Station[i]);
        }
    }
    memset(Station_State, 0, sizeof(int) * Station_Num);
    Station_State[0] = 1;
    Price = 0;
    Cur_Oil = 0;
    state = 0;
    while (1)
    {
        flag = 0;
        for (i = 0; i < Station_Num; i++)
        {
            Distance = Station[i].Distance_To_Origin - Cur_Station.Distance_To_Origin;
            // The next station must after the current station
            // Thus Distance must bigger than 0
            if (Distance > 0 && Distance <= Tank_Capacity * Running_Distance_Per_Unit_Oil)
            {
                flag = 1;
                // The oil price of the next station is smaller than current station
                if (Station[i].Price_Per_Unit_Oil <= Cur_Station.Price_Per_Unit_Oil)
                {
                    o = i + 1;
                    while (Station[o].Price_Per_Unit_Oil < Cur_Station.Price_Per_Unit_Oil)
                    {
                        Distance_Tmp = Station[o].Distance_To_Origin - Cur_Station.Distance_To_Origin;
                        if (Distance_Tmp > 0 && Distance_Tmp < Distance)
                        {
                            Distance = Distance_Tmp;
                            i = o;
                        }
                        o++;
                    }
                    Oil_Consumption = Distance / Running_Distance_Per_Unit_Oil;
                    // If the oil consumption is bigger than current oil,
                    // need to fill oil so that arrive next station
                    if (Oil_Consumption > Cur_Oil)
                    {
                        Oil_Consumption -= Cur_Oil;
                        Price += Oil_Consumption * Cur_Station.Price_Per_Unit_Oil;
                        Cur_Oil = 0;
                    }
                    else
                    {
                        Cur_Oil = Cur_Oil - Oil_Consumption;
                    }
                    Cur_Station = Station[i];
                    Station_State[Station[i].Id] = 1;
                    break;
                }
                // The oil price of the next station is bigger than current station
                else
                {
                    // The car can't directly arrive the destination
                    if (Destination_Distance > Cur_Station.Distance_To_Origin + Tank_Capacity * Running_Distance_Per_Unit_Oil)
                    {
                        // Oil that need to fill until the tank is full
                        Oil_Consumption = Tank_Capacity - Cur_Oil;
                        Price += Oil_Consumption * Cur_Station.Price_Per_Unit_Oil;
                        // Oil that the running consumpted
                        Oil_Consumption = Distance / Running_Distance_Per_Unit_Oil;
                        Cur_Oil = Tank_Capacity - Oil_Consumption;
                        Cur_Station = Station[i];
                        Station_State[Station[i].Id] = 1;
                        break;
                    }
                    // The car can directly arrive the destination
                    else
                    {
                        Distance = Destination_Distance - Cur_Station.Distance_To_Origin;
                        Oil_Consumption = Distance / Running_Distance_Per_Unit_Oil;
                        Price += Oil_Consumption * Cur_Station.Price_Per_Unit_Oil;
                        // state == 1 means that the car has arrived the destination
                        state = 1;
                        break;
                    }
                }
            }
        }
        if (Cur_Station.Id == Station_Num - 1 && Destination_Distance - Cur_Station.Distance_To_Origin <= Tank_Capacity * Running_Distance_Per_Unit_Oil)
        {
            state = 1;
        }
        // flag == 0
        // The car can't arrive the next station
        if (flag == 0)
        {
            state = 0;
            Total_Distance = Cur_Station.Distance_To_Origin + Tank_Capacity * Running_Distance_Per_Unit_Oil;
            break;
        }
        if (state == 1)
            break;
    }
    // state == 1 The car can arrive the destination
    // state == 0 The car can't arrive the destination
    Show(Station_State, Station_Num, state, Station_Copy);
    state == 1 ? printf("\nThe cheapest oil fee is %.2lf\n\n", Price) : printf("\nThe longest travel distance is %.2lf\n\n", Total_Distance);
}