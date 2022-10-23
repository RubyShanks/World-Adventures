// Steps to create a new skill:
// 1. Update TOTALSKILLSCOUNT
// 2. Update sk[]
// 3. Add a new skill function for new skill
// 4. Update activateSkill function

typedef struct SKILLS
{
    int id, rounds, element;
    char name[41];
    char description[160];
    // Note 1 round means 1 turn.
    float mpcost, stcost, power;
}
SKILLS;

// Must be updated with each skill added!
const int TOTALSKILLSCOUNT = 15;
SKILLS sk[TOTALSKILLSCOUNT];

void SKILLS_initialise()
{
    // NEVER CHANGE IDS
    // SKILL IDS USED: 1, 5, 6, 7, 9, 14, 17, 19, 34, 64, 70, 80, 99, 444
    sk[0].id = 1;
    strcpy(sk[0].name, "Slice");
    strcpy(sk[0].description, "Try to slice the enemy with a sword!");
    sk[0].power = 13;
    sk[0].stcost = sk[0].power;
    sk[0].mpcost = 0;
    sk[0].rounds = -1;
    sk[0].element = ELT_NONE;

    sk[1].id = 7;
    strcpy(sk[1].name, "Gluey Spit");
    strcpy(sk[1].description, "Non poisionous sticky spit attack");
    sk[1].power = 11;
    sk[1].stcost = sk[1].power;
    sk[1].mpcost = 0;
    sk[1].rounds = -1;
    sk[1].element = ELT_NONE;

    sk[2].id = 19;
    strcpy(sk[2].name, "Auto Heal");
    strcpy(sk[2].description, "Recover some health in each round for the next few rounds. Overwrites poison attacks.");
    // % heal per round
    sk[2].power = 2;
    sk[2].stcost = 0;
    sk[2].mpcost = sk[2].power * 10;
    sk[2].rounds = 30;
    sk[2].element = ELT_NONE;

    sk[3].id = 14;
    strcpy(sk[3].name, "Short Range Teleportation");
    strcpy(sk[3].description, "After using this skill you're guaranteed to evade certain no. of attacks in the next few rounds!");
    // Number of evades to occur
    sk[3].power = 3;
    sk[3].mpcost = 6.5 * sk[3].power;
    sk[3].stcost = 0;
    sk[3].rounds = 20;
    sk[3].element = ELT_NONE;

    sk[4].id = 9;
    strcpy(sk[4].name, "Wind Blade");
    strcpy(sk[4].description, "A strong magic infused sword attack.");
    sk[4].power = 20;
    sk[4].mpcost = sk[4].power;
    sk[4].stcost = 0;
    sk[4].rounds = -1;
    sk[4].element = ELT_WIND;

    sk[5].id = 6;
    strcpy(sk[5].name, "Fire Blazer");
    strcpy(sk[5].description, "A fire tornado surrounds and protects you from both phyical and magical damage!");
    // % defence increased
    sk[5].power = 80;
    sk[5].mpcost = 90;
    sk[5].stcost = 0;
    sk[5].rounds = 10;
    sk[5].element = ELT_FIRE;

    sk[6].id = 64;
    strcpy(sk[6].name, "Water Bullet");
    strcpy(sk[6].description, "Concentrated water magic attack delivered at bullet speed!");
    sk[6].power = 13;
    sk[6].mpcost = sk[6].power;
    sk[6].stcost = 0;
    sk[6].rounds = -1;
    sk[6].element = ELT_WATER;

    sk[7].id = 99;
    strcpy(sk[7].name, "Thunder Dancer");
    strcpy(sk[7].description, "A powerful thunder magic attack!");
    sk[7].power = 14;
    sk[7].mpcost = sk[6].power;
    sk[7].stcost = 0;
    sk[7].rounds = -1;
    sk[7].element = ELT_ELEC;

    sk[8].id = 444;
    strcpy(sk[8].name, "Last Resort");
    strcpy(sk[8].description, "Can only used when HP < 10%. A powerful blast that leaves the user at 10 HP and 0 MP and ST. This skill may be used only once");
    sk[8].power = 35;
    sk[8].mpcost = 0;
    sk[8].stcost = 0;
    sk[8].rounds = -1;
    sk[8].element = ELT_NONE;

    sk[9].id = 70;
    strcpy(sk[9].name, "Fire Blade");
    strcpy(sk[9].description, "A strong sword attack infused with fire magic");
    sk[9].power = 21;
    sk[9].mpcost = sk[9].power;
    sk[9].stcost = 0;
    sk[9].rounds = -1;
    sk[9].element = ELT_FIRE;

    sk[10].id = 34;
    strcpy(sk[10].name, "Ice Bullet");
    strcpy(sk[10].description, "Concentrated ice magic attack delivered at bullet speed!");
    sk[10].power = 13;
    sk[10].mpcost = sk[10].power;
    sk[10].stcost = 0;
    sk[10].rounds = -1;
    sk[10].element = ELT_ICE;

    sk[11].id = 5;
    strcpy(sk[11].name, "Water Blade");
    strcpy(sk[11].description, "A strong sword attack infused with water magic");
    sk[11].power = 20;
    sk[11].mpcost = sk[11].power;
    sk[11].stcost = 0;
    sk[11].rounds = -1;
    sk[11].element = ELT_WATER;

    sk[12].id = 80;
    strcpy(sk[12].name, "Poisonous Spit");
    strcpy(sk[12].description, "A poisionous spit attack that damages over time. Overwrites auto heal.");
    sk[12].power = 2;
    sk[12].mpcost = 0;
    sk[12].stcost = sk[12].power * 10;
    sk[12].rounds = 30;
    sk[12].element = ELT_NONE;

    sk[13].id = 17;
    strcpy(sk[13].name, "Earth Spears");
    strcpy(sk[13].description, "Summon rock spears out of the ground and catch your opponenet off guard!");
    sk[13].power = 25;
    sk[13].mpcost = sk[13].power;
    sk[13].stcost = 0;
    sk[13].rounds = -1;
    sk[13].element = ELT_EARTH;

    sk[14].id = 111;
    strcpy(sk[14].name, "Kiken");
    strcpy(sk[14].description, "Can be used only thrice in a battle. Each strike grows in power, with the last one almost sure to instantly defeat a physically inferior opponent");
    sk[14].power = 80;
    sk[14].mpcost = 0;
    sk[14].stcost = 50;
    sk[14].rounds = -1;
    sk[14].element = ELT_NONE;

    // DON'T FORGET TO UPDATE TOTALSKILLSCOUNT when adding new skills!
    // Must update this activateSKILL function when creating a new skill
}

// Simple functions that only require character & SKILLS structure to be defined and are otherwise independent of definition of other functions
// Takes skill ID and return array index
int getSKILLindex(int id);
void deferrmsg1();
// Returns multiplication factor to damage dealt
float elementalAdvantageCheck(character* p, character* px);

// Needs getSKILLindex to be defined
void showSKILLdetails(int id);

void postSkillCard(character* p, character* px);
int skillCost(character *p, character *px, float mp, float st, int skillIndex);
void isEvaded(character* p, character* px, int skillIndex);
void stdmagicalAttackSkill(character* p, character* px, int ind);
void stdphysicalAttackSkill(character* p, character* px, int ind);
void skill_autoHeal(character* p, character* px);
void skill_shortRangeTeleportation(character* p, character* px);
void skill_fireBlazer(character* p, character* px);
void skill_lastResort(character* p, character* px);
void skill_poisonousSpit(character* p, character* px);
void skill_kiken(character* p, character* px, int ind);

int useskillConfirmation(character* px);

// Needs all the skill_<SKILL> type functions to be defined for use
// Must update this function when creating a new skill. Needs getSKILLindex to be defined
void activateSkill(int id, character* p, character* px);

void activateSkill(int id, character* p, character* px)
{
    int ind = getSKILLindex(id);
    switch (ind)
    {
        case 0:
        case 1:
        stdphysicalAttackSkill(p, px, ind); break;
        case 4:
        case 6:
        case 7:
        case 9:
        case 10:
        case 11:
        case 13:
        stdmagicalAttackSkill(p, px, ind); break;
        case 2:
        skill_autoHeal(p, px); break;
        case 3:
        skill_shortRangeTeleportation(p, px); break;
        case 5:
        skill_fireBlazer(p, px); break;
        case 8:
        skill_lastResort(p, px); break;
        case 12:
        skill_poisonousSpit(p, px); break;
        case 14:
        skill_kiken(p, px, 14); break;
        default:
        printf("Error: Selected skill is not defined!\n");
    }
}
int skillDetails(int id, character* p, character* px)
{
    if (getSKILLindex(id) < 0)
    {
        printf("Error: Selected skill not defined\n");
        return -1;
    }
    showSKILLdetails(id);
    if (useskillConfirmation(px) == 0)
    {
        activateSkill(id, p, px);
        return 0;
    }
    else
    {
        return 1;
    }
}
void display_availableSkills(character* p, character* px)
{
    int tmp;
    for (node* tmpn = p->skills; tmpn != NULL; tmpn = tmpn->next)
    {
        int tmp2 = getSKILLindex(tmpn->nodeValue);
        if (tmp2 < 0)
        {
            printf("Error: No index found for selected skill ID\n");
        }
        else
        {
            printf("%d: %s", sk[tmp2].id, sk[tmp2].name);
            int tmpDigits = 0, tmpid = sk[tmp2].id;
            while (tmpid != 0)
            {
                tmpDigits++;
                tmpid /= 10;
            }
            for (int tmp3 = strlen(sk[tmp2].name) + tmpDigits; tmp3 < 50; tmp3++)
            {
                printf(" ");
            }
            printf("(-%.2f ST)    (-%.2f MP)\n", sk[tmp2].stcost, sk[tmp2].mpcost);
        }
    }
    printf("Enter skill ID to view details (Enter 0 to go back):\n");
    scanf("%d", &tmp);
    getchar();
    for (node* tmpn = p->skills; tmpn != NULL; tmpn = tmpn->next)
    {
        if (tmpn->nodeValue == tmp)
        {
            if (skillDetails(tmp, p, px) == 1)
            {
                display_availableSkills(p, px);
            }
            return;
        }
    }
}
void skill_autoHeal(character* p, character* px)
{
    if (skillCost(p, px, sk[2].mpcost, sk[2].stcost, 2) == 1)
    {
        return;
    }
    // Evasion impossible
    // No elemental advantages
    p->autoHeal = sk[2].rounds;
    p->healValue = sk[2].power;
    printf("%s used %s!\n", p->name, sk[2].name);
    postSkillCard(p, px);
}
void skill_shortRangeTeleportation(character* p, character* px)
{
    if (skillCost(p, px, sk[3].mpcost, sk[3].stcost, 3) == 1)
    {
        return;
    }
    // Evasion impossible
    // No elemental advantages
    p->evades = sk[3].power, p->evadeRounds = sk[3].rounds;
    printf("%s used %s!\n", p->name, sk[3].name);
    postSkillCard(p, px);
}
void skill_fireBlazer(character* p, character* px)
{
    if (skillCost(p, px, sk[5].mpcost, sk[5].stcost, 5) == 1)
    {
        return;
    }
    // Evasion impossible
    // No elemental advantages
    p->pdBoost = sk[5].power, p->mdBoost = sk[5].power;
    p->pdBrounds = sk[5].rounds, p->mdBrounds = sk[5].rounds;
    printf("%s used %s! %s's defences have increased by %.0f%%\n", p->name, sk[5].name, p->name, sk[5].power);
    postSkillCard(p, px);
}
void skill_lastResort(character* p, character* px)
{
    if (p->lastResortUsed == 1)
    {
        printf("Last Resort could not be used. This skill can only be used once.\n");
        return;
    }
    // 3rd parameter in function skillCost should be equal to skill index
    if (skillCost(p, px, sk[8].mpcost, sk[8].stcost, 8) == 1)
    {
        return;
    }
    p->lastResortUsed = 1;
    if (!(p->hp < 0.1 * p->maxhp))
    {
        printf("Skill criteria not fulfilled. Last Resort could not be used!\n");
        p->hp = 1, p->st = 0, p->mp = 0;
        postSkillCard(p, px);
        return;
    }
    if (p->isEvaded == 1)
    {
        // third parameter of isEvaded should be equal to skill index
        isEvaded(p, px, 8);
        return;
    }
    p->hp = 1, p->st = 0, p->mp = 0;
    float pow = sk[8].power;
    float dmg = pow * 0.01 * p->maxhp;
    float delta = ((p->ma - px->md) / p->ma) * 100;
    dmg = dmg + ((delta / 100) * dmg);
    if (dmg < 0)
    {
        dmg = 0;
    }
    dmg *= elementalAdvantageCheck(p, px);
    px->hp -= dmg;
    printf("%s used %s!\n", p->name, sk[8].name);
    printf("%s\nHP: %.0f (-%.0f)\nMP: %.2f\nST: %.2f\n", px->name, px->hp * 10, dmg * 10, px->mp, px->st);
    printf("%s\nHP: %.0f\nMP: %.2f\nST: %.2f\n", p->name, p->hp * 10, p->mp, p->st);
}
void skill_poisonousSpit(character* p, character* px)
{
    if (skillCost(p, px, sk[12].mpcost, sk[12].stcost, 12) == 1)
    {
        // 3rd parameter in function skillCost should be equal to skill index
        return;
    }
    if (p->isEvaded == 1)
    {
        // 3rd parameter of isEvaded should be equal to skill index
        isEvaded(p, px, 12);
        return;
    }
    // No elemental advantages
    px->autoHeal = sk[12].rounds;
    px->healValue = -1 * sk[12].power;
    printf("%s used %s!\n", p->name, sk[12].name);
    postSkillCard(p, px);
}
void skill_kiken(character* p, character* px, int ind)
{
    // 3rd parameter in function skillCost should be equal to skill index
    if (skillCost(p, px, sk[ind].mpcost, sk[ind].stcost, ind) == 1)
    {
        return;
    }
    if (p->kikensUsed > 2)
    {
        printf("The skill %s could not be used!\n", sk[ind].name);
        return;
    }
    if (p->isEvaded == 1)
    {
        // 3rd parameter of isEvaded should be equal to skill index
        isEvaded(p, px, ind);
        printf("%s cannot use the skill %s anymore because of missed attack!\n", p->name, sk[ind].name);
        p->kikensUsed = 100;
        return;
    }
    float pow;
    if (p->kikensUsed == 0)
        pow = sk[ind].power / 20;
    else if (p->kikensUsed == 1)
        pow = sk[ind].power / 10;
    else
        pow = sk[ind].power;
    float dmg = pow * 0.01 * p->maxhp;
    float delta = ((p->pa - px->pd) / p->ma) * 100;
    dmg = dmg + ((delta / 100) * dmg);
    if (dmg < 0)
    {
        dmg = 0;
    }
    dmg *= elementalAdvantageCheck(p, px);
    px->hp -= dmg;
    printf("%s used %s!\n", p->name, sk[ind].name);
    printf("%s\nHP: %.0f (-%.0f)\nMP: %.2f\nST: %.2f\n", px->name, px->hp * 10, dmg * 10, px->mp, px->st);
    printf("%s\nHP: %.0f\nMP: %.2f\nST: %.2f\n", p->name, p->hp * 10, p->mp, p->st);
    p->kikensUsed++;
}
int useskillConfirmation(character* px)
{
    int tmp;
    printf("Use skill? (1) Yes (2) No\n");
    scanf("%d", &tmp);
    getchar();
    if (tmp == 1)
    {
        if (px == NULL)
        {
            deferrmsg1();
            return 1;
        }
        return 0;
    }
    return 1;
}
void isEvaded(character* p, character* px, int skillIndex)
{
    printf("%s used the skill %s!\n", p->name, sk[skillIndex].name);
    printf("%s evaded the attack!\n", px->name);
    if (px->evades == 0)
    {
        printf("%s's evade effect has worn off!\n", px->name);
    }
    p->isEvaded = 0;
}
void postSkillCard(character* p, character* px)
{
    printf("%s (Lv %d)\nHP: %.0f\nMP: %.2f\nST: %.2f\n", px->name, px->lv, px->hp * 10, px->mp, px->st);
    printf("%s (Lv %d)\nHP: %.0f\nMP: %.2f\nST: %.2f\n", p->name, p->lv, p->hp * 10, p->mp, p->st);
}

int skillCost(character *p, character* px, float mp, float st, int skillIndex)
{
    if ((p->mp < mp) || (p->st < st))
    {
        printf("Low MP/ST: Can't use this skill right now\n");
        postSkillCard(p, px);
        return 1;
    }
    p->mp -= mp, p->st -= st;
    if (sk[skillIndex].element != ELT_NONE)
    {
        p->element = sk[skillIndex].element, p->eltChangeRounds = 2;
        printf("%s's element has changed for this round!\n", p->name);
    }
    return 0;
}
int getSKILLindex(int ID)
{
    for (int tmp = 0; tmp < TOTALSKILLSCOUNT; tmp++)
    {
        if (sk[tmp].id == ID)
        {
            return tmp;
        }
    }
    return -1;
}
void deferrmsg1()
{
    printf("You can't use this skill right now because you're not in battle\n");
    getchar();
}
void showSKILLdetails(int id)
{
    int ind = getSKILLindex(id);
    printf("%s\n", sk[ind].name);
    printf("%s\n", sk[ind].description);
    printf("Power: %.0f\n", sk[ind].power);
    if (sk[ind].rounds >= 0)
    {
        printf("Validity: %d rounds\n", sk[ind].rounds);
    }
    printf("ST Cost: %.2f    MP Cost: %.2f\n", sk[ind].stcost, sk[ind].mpcost);
}

float elementalAdvantageCheck(character* p, character* px)
{
    // water >> fire
    if (p->element == ELT_WATER && px->element == ELT_FIRE)
    {
        printf("%s gained elemental advantage! (water over fire) Damage increased by 50!%%\n", p->name);
        return 1.5;
    }
    if (p->element == ELT_FIRE && px->element == ELT_WATER)
    {
        printf("%s gained elemental advantage! (water over fire) Damage decreased by 50!%%\n", px->name);
        return 0.5;
    }
    // ice >> water
    if (p->element == ELT_ICE && px->element == ELT_WATER)
    {
        printf("%s gained elemental advantage! (ice over water) Damage increased by 50!%%\n", p->name);
        return 1.5;
    }
    if (p->element == ELT_WATER && px->element == ELT_ICE)
    {
        printf("%s gained elemental advantage! (ice over water) Damage decreased by 50!%%\n", px->name);
        return 0.5;
    }
    // elec >> water
    if (p->element == ELT_ELEC && px->element == ELT_WATER)
    {
        printf("%s gained elemental advantage! (electricity over water) Damage increased by 50!%%\n", p->name);
        return 1.5;
    }
    if (p->element == ELT_WATER && px->element == ELT_ELEC)
    {
        printf("%s gained elemental advantage! (electricity over water) Damage decreased by 50!%%\n", px->name);
        return 0.5;
    }
    // fire >> ice
    if (p->element == ELT_FIRE && px->element == ELT_ICE)
    {
        printf("%s gained elemental advantage! (fire over ice) Damage increased by 50!%%\n", p->name);
        return 1.5;
    }
    if (p->element == ELT_ICE && px->element == ELT_FIRE)
    {
        printf("%s gained elemental advantage! (fire over ice) Damage decreased by 50!%%\n", px->name);
        return 0.5;
    }
    // earth >> elec
    if (p->element == ELT_EARTH && px->element == ELT_ELEC)
    {
        printf("%s gained elemental advantage! (earth over electricity) Damage increased by 50!%%\n", p->name);
        return 1.5;
    }
    if (p->element == ELT_ELEC && px->element == ELT_EARTH)
    {
        printf("%s gained elemental advantage! (earth over electricity) Damage decreased by 50!%%\n", px->name);
        return 0.5;
    }
    // dark >> everything but light
    if (p->element == ELT_DARK && px->element != ELT_LIGHT)
    {
        printf("%s gained elemental advantage! (darkness) Damage increased by 50!%%\n", p->name);
        return 1.25;
    }
    if (p->element != ELT_LIGHT && px->element == ELT_DARK)
    {
        printf("%s gained elemental advantage! (darkness) Damage decreased by 50!%%\n", px->name);
        return 0.75;
    }
    // light >> dark
    if (p->element == ELT_LIGHT && px->element == ELT_DARK)
    {
        printf("%s gained elemental advantage! (light over darkness) Damage increased by 50!%%\n", p->name);
        return 1.5;
    }
    if (p->element == ELT_DARK && px->element == ELT_LIGHT)
    {
        printf("%s gained elemental advantage! (light over darkness) Damage decreased by 50!%%\n", px->name);
        return 0.5;
    }
    // earth >> wind
    if (p->element == ELT_EARTH && px->element == ELT_WIND)
    {
        printf("%s gained elemental advantage! (earth over wind) Damage increased by 50!%%\n", p->name);
        return 1.5;
    }
    if (p->element == ELT_WIND && px->element == ELT_EARTH)
    {
        printf("%s gained elemental advantage! (earth over wind) Damage decreased by 50!%%\n", px->name);
        return 0.5;
    }
    return 1;
}
void stdmagicalAttackSkill(character* p, character* px, int ind)
{
    // 3rd parameter in function skillCost should be equal to skill index
    if (skillCost(p, px, sk[ind].mpcost, sk[ind].stcost, ind) == 1)
    {
        return;
    }
    if (p->isEvaded == 1)
    {
        // 3rd parameter of isEvaded should be equal to skill index
        isEvaded(p, px, ind);
        return;
    }
    float pow = sk[ind].power;
    float dmg = pow * 0.01 * p->maxhp;
    float delta = ((p->ma - px->md) / p->ma) * 100;
    dmg = dmg + ((delta / 100) * dmg);
    if (dmg < 0)
    {
        dmg = 0;
    }
    dmg *= elementalAdvantageCheck(p, px);
    px->hp -= dmg;
    printf("%s used %s!\n", p->name, sk[ind].name);
    printf("%s\nHP: %.0f (-%.0f)\nMP: %.2f\nST: %.2f\n", px->name, px->hp * 10, dmg * 10, px->mp, px->st);
    printf("%s\nHP: %.0f\nMP: %.2f\nST: %.2f\n", p->name, p->hp * 10, p->mp, p->st);
}

void stdphysicalAttackSkill(character* p, character* px, int ind)
{
    // 3rd parameter in function skillCost should be equal to skill index
    if (skillCost(p, px, sk[ind].mpcost, sk[ind].stcost, ind) == 1)
    {
        return;
    }
    if (p->isEvaded == 1)
    {
        // 3rd parameter of isEvaded should be equal to skill index
        isEvaded(p, px, ind);
        return;
    }
    float pow = sk[ind].power;
    float dmg = pow * 0.01 * p->maxhp;
    float delta = ((p->pa - px->pd) / p->ma) * 100;
    dmg = dmg + ((delta / 100) * dmg);
    if (dmg < 0)
    {
        dmg = 0;
    }
    dmg *= elementalAdvantageCheck(p, px);
    px->hp -= dmg;
    printf("%s used %s!\n", p->name, sk[ind].name);
    printf("%s\nHP: %.0f (-%.0f)\nMP: %.2f\nST: %.2f\n", px->name, px->hp * 10, dmg * 10, px->mp, px->st);
    printf("%s\nHP: %.0f\nMP: %.2f\nST: %.2f\n", p->name, p->hp * 10, p->mp, p->st);
}