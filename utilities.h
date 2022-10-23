const float MONSTREN = 50; // Percentage elevation in stats of monsters
const int MONAUTOHEALROUNDS = 20;
const int ELT_NONE = 1;
const int ELT_FIRE = 2;
const int ELT_WATER = 3;
const int ELT_ELEC = 4;
const int ELT_DARK = 5;
const int ELT_LIGHT = 6;
const int ELT_EARTH = 7;
const int ELT_ICE = 8;
const int ELT_WIND = 9;

int RNG(int lower, int upper);

int RNG(int lower, int upper)
{
    int x = rand() % (upper + 1);
    while (x < lower)
    {
        x = rand() % (upper + 1);
    }
    return x;
}

