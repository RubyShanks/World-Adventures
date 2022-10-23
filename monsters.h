// Steps to create a new monster
// 1. Update TOTALMONSTERCOUNT
// 2. Update MONS[]
typedef struct monster
{
    int id, totalSkills;
    char name[41];
    node* skills;
}
monster;
// Must be updated when adding new monsters
const int TOTALMONSTERCOUNT = 3;
monster MONS[TOTALMONSTERCOUNT];
int getMonsterIndex(int monster_id);
node* monsterSkillsAdder(char arr[]);
void MONS_initialise()
{
    // IDs Used: 12, 22, 82
    MONS[0].id = 82;
    strcpy(MONS[0].name, "Lesser Slime");
    MONS[0].skills = NULL;
    MONS[0].totalSkills = 1;
    MONS[0].skills = monsterSkillsAdder("7");
    MONS[1].id = 22;
    strcpy(MONS[1].name, "Slime");
    MONS[1].skills = NULL;
    MONS[1].totalSkills = 4;
    MONS[1].skills = monsterSkillsAdder("7 64 34 80");
    MONS[2].id = 12;
    strcpy(MONS[2].name, "Greater Slime");
    MONS[2].skills = NULL;
    MONS[2].totalSkills = 6;
    MONS[2].skills = monsterSkillsAdder("7 64 34 80 34 5");
}
node* monsterSkillsAdder(char arr[])
{
    int len = strlen(arr), number = 0;
    node* list = NULL;
    for (int tmp = 0; tmp < len; tmp++)
    {
        if (arr[tmp] != ' ')
        {
            number *= 10;
            number += (arr[tmp] - 48);
        }
        else
        {
            list = addNode(list, number);
            number = 0;
        }
    }
    list = addNode(list, number);
    return list;
}
int getMonsterIndex(int monster_id)
{
    for (int tmp = 0; tmp < TOTALMONSTERCOUNT; tmp++)
    {
        if (MONS[tmp].id == monster_id)
        {
            return tmp;
        }
    }
    printf("Error: Monster index not found for given ID\n");
    return -1;
}