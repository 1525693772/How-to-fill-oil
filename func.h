typedef struct Oil_Station
{
    double Distance_To_Origin;
    double Price_Per_Unit_Oil;
    int Id;
} Oil_Station;

void Show(int *Station_State, int Station_Num, int state, Oil_Station *Station);

void Swap(Oil_Station *a, Oil_Station *b);

void Greedy_Algorithm(double Tank_Capacity, double Running_Distance_Per_Unit_Oil, double Destination_Distance, Oil_Station *Station, int Station_Num);
