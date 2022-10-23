#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "utilities.h"

typedef struct node
{
    int nodeValue;
    struct node* next;
}
node;
node* addNode(node* n, int value);

typedef struct character
{
    float hp, pa, ma, pd, md, EXP, expthr, maxhp, mp, st;
    int lv, skillCount, element, trueElement;
    char name[31];
    node* skills;

    // Used for battle
    // autoHeal = no. of rounds; healValue = percentage health recovered
    // paBoost = %boost in true pa
    // paBrounds = number of rounds for which boost is valid
    float healValue, paBoost, pdBoost, maBoost, mdBoost, paBrounds, pdBrounds, maBrounds, mdBrounds, eltChangeRounds;
    float truepa, truepd, truema, truemd;
    int autoHeal, evades, evadeRounds, isEvaded, lastResortUsed, kikensUsed;
    int escapes[3];
}
character;


#include "skills.h"
#include "monsters.h"

// Simple functions that only require character structure to be defined and are otherwise independent of definition of other functions
void BattleStatsRestore(character* p, int isMonster);
void preBattleStatsRestore(character* p);
void postBattleStatsRestore(character* p);
void statsBooster(character* p);
void freeLinkedList(node* n);
// Note must be run before exiting from program
void freeMemoryAtExit(character* p1, character* p2);
void autoRecovery(character* p1, character* p2, float inc);
void elementChangeCheck(character* p);
int midBattleEscape(character* p, character* px);
// Parameter should be a character string of monster ids separated by spaces
int randomMonsterGenerator(char* arr);


void monster_initialize(character *p, int monster_id, int level);
void display_stats(character *p);
void character_levelup(character *p);
int saveGame(character* p);
int loadGame(character* p);
// Must be after saveGame
int newGame(character *p);
void expCheck(character *p);
// Must be after loadGame and newGame
int titleScreen(character *p);
void battleSimulate(character *p1, character* p2, int isMonster);
void addSkill(character *p, int skillID);
void hospital(character* p);
void autohealCheck(character*p, character* px);
void evadeCheck(character *p, character* px);
void statsBoostCheck(character *p);
// Needs addSkill to be defined
void skillSelection(character* p, int arr[], int n);
// Must be defined after expCheck, postBattleStatsRestore and BattleStatsRestore
void endBattle(character* winner, character* loser);

int main(void)
{
    srand(time(0));
    SKILLS_initialise();
    MONS_initialise();
    int tmp, tmp2, tmpLower, tmpUpper, tmptitlesc = 1;
    char* tmps;
    character player1, player2;
    character monster1;
    player1.skills = NULL, player2.skills = NULL, monster1.skills = NULL;
    while (1)
    {
        tmp = titleScreen(&player1);
        if (tmp == 1)
        {
            freeMemoryAtExit(&player1, &player2);
            return 1;
        }
        else if (tmp == -1)
        {
            freeMemoryAtExit(&player1, &player2);
            return 0;
        }
        tmptitlesc = 1;
        while (tmptitlesc == 1)
        {
            printf("Select Action: (1) Monster Battle (2) Multiplayer battle (3) Save game (4) Hospital (5) My Skills (6) My Stats (7) Back to Title Screen\n");
            printf("(8) Developer Hacks\n");
            scanf("%d", &tmp);
            getchar();
            switch (tmp)
            {
                case 1:
                tmpLower = player1.lv - 9;
                if (tmpLower < 1)
                    tmpLower = 1;
                tmpUpper = player1.lv;
                if (tmpUpper > 30)
                    tmpUpper = 30;
                monster_initialize(&monster1, randomMonsterGenerator("12 82 22"), RNG(tmpLower, tmpUpper));
                printf("\e[1;1H\e[2J");
                display_stats(&monster1);
                display_stats(&player1);
                printf("Enter (1) Fight (2) Retreat\n");
                scanf("%d", &tmp2);
                if (tmp2 != 2)
                {
                    battleSimulate(&player1, &monster1, 1);
                    saveGame(&player1);
                }
                else
                {
                    printf("%s fleed\n", player1.name);
                }
                break;
                case 2:
                if (loadGame(&player2) == 0)
                {
                    hospital(&player2);
                    printf("\e[1;1H\e[2J");
                    display_stats(&player2);
                    display_stats(&player1);
                    getchar();
                    battleSimulate(&player1, &player2, 0);
                    saveGame(&player1);
                    saveGame(&player2);
                }
                break;
                case 3:
                saveGame(&player1); break;
                case 4:
                hospital(&player1); break;
                case 5:
                printf("\e[1;1H\e[2J");
                display_availableSkills(&player1, NULL);
                printf("\e[1;1H\e[2J");
                break;
                case 6:
                printf("\e[1;1H\e[2J");
                display_stats(&player1);
                getchar();
                printf("\e[1;1H\e[2J");
                break;
                case 8:
                character_levelup(&player1);
                player1.EXP = 0;
                break;
                default:
                tmptitlesc = 0;
            }
        }
    }
}

void monster_initialize(character *p, int monster_id, int level)
{
    p->hp = (10 * pow(1.1, level - 1)) * (MONSTREN + 100) * 0.01;
    p->pa = (10 * level)  * (MONSTREN + 100) * 0.01, p->ma = (10 * level) * (MONSTREN + 100) * 0.01;
    p->pd = (10 * level) * (MONSTREN + 100) * 0.01, p->md = (10 * level) * (MONSTREN + 100) * 0.01;
    p->EXP = 0, p->lv = level, p->expthr = 10 * pow(1.2, level - 1), p->maxhp = p->hp, p->trueElement = ELT_NONE;
    int tmpMIndex = getMonsterIndex(monster_id);
    strcpy(p->name, MONS[tmpMIndex].name);
    p->skillCount = MONS[tmpMIndex].totalSkills;
    p->skills = MONS[tmpMIndex].skills;
    p->mp = 100, p->st = 100;
    p->autoHeal = MONAUTOHEALROUNDS, p->healValue = MONSTREN / 10;
}
void display_stats(character *p)
{
    printf("%s\n", p->name);
    printf("LV: %d\n", p->lv);
    printf("HP: %.0f/%.0f\n", p->hp * 10, p->maxhp * 10);
    printf("MP: %.2f%%\n", p->mp);
    printf("ST: %.2f%%\n", p->st);
    printf("PA: %.0f\n", p->pa);
    printf("PD: %.0f\n", p->pd);
    printf("MA: %.0f\n", p->ma);
    printf("MD: %.0f\n", p->md);
    printf("Skills Owned: %d\n", p->skillCount);
    printf("EXP: %.0f/%.0f\n", p->EXP, p->expthr);
}
void character_levelup(character *p)
{
    int tmp;
    p->maxhp *= 1.1;
    p->pa += RNG(5, 15), p->ma += RNG(5, 15), p->pd += RNG(5, 15), p->md += RNG(5, 15), p->EXP = p->EXP - p->expthr, p->lv += 1, p->expthr *= 1.2;
    printf("%s leveled up to lv %d!\n", p->name, p->lv);
    display_stats(p);
    getchar();
    if (p->lv == 3)
    {
        int tmparr[2] = {sk[2].id, sk[3].id};
        skillSelection(p, tmparr, 2);
    }
    else if (p->lv == 7)
    {
        int tmparr[2] = {sk[4].id, sk[5].id};
        skillSelection(p, tmparr, 2);
    }
    else if (p->lv == 10)
    {
        int tmparr[2] = {sk[9].id, sk[11].id};
        skillSelection(p, tmparr, 2);
    }
    else if (p->lv == 13)
    {
        int tmparr[3] = {sk[6].id, sk[7].id, sk[10].id};
        skillSelection(p, tmparr, 3);
    }
    else if (p->lv == 15)
    {
        int tmparr[3] = {sk[8].id, sk[13].id, sk[14].id};
        skillSelection(p, tmparr, 3);
    }
}
int saveGame(character* p)
{
    char tmps[35];
    strcpy(tmps, p->name);
    strcat(tmps, ".txt");
    FILE* file = fopen(tmps, "w");
    if (file == NULL)
    {
        printf("Error: Unable to open file\n");
        return 1;
    }
    fwrite(&(p->hp), 4, 1, file);
    fwrite(&(p->mp), 4, 1, file);
    fwrite(&(p->st), 4, 1, file);
    fwrite(&(p->pa), 4, 1, file);
    fwrite(&(p->ma), 4, 1, file);
    fwrite(&(p->pd), 4, 1, file);
    fwrite(&(p->md), 4, 1, file);
    fwrite(&(p->EXP), 4, 1, file);
    fwrite(&(p->maxhp), 4, 1, file);
    fwrite(&(p->lv), 4, 1, file);
    fwrite(&(p->name), 31, 1, file);
    fwrite(&(p->skillCount), 4, 1, file);
    for (node* tmpn = p->skills; tmpn != NULL; tmpn = tmpn->next)
    {
        fwrite(&(tmpn->nodeValue), 4, 1, file);
    }
    fclose(file);
    printf("Game file saved!\n");
    return 0;
}
int loadGame(character* p)
{
    char tmps[35];
    int tmp;
    printf("Enter player name:\n");
    scanf("%s", p->name);
    getchar();
    strcpy(tmps, p->name);
    strcat(tmps, ".txt");
    FILE* file = fopen(tmps, "r");
    if (file == NULL)
    {
        printf("Error: Unable to open file\n");
        return 1;
    }
    fread(&(p->hp), 4, 1, file);
    fread(&(p->mp), 4, 1, file);
    fread(&(p->st), 4, 1, file);
    fread(&(p->pa), 4, 1, file);
    fread(&(p->ma), 4, 1, file);
    fread(&(p->pd), 4, 1, file);
    fread(&(p->md), 4, 1, file);
    fread(&(p->EXP), 4, 1, file);
    fread(&(p->maxhp), 4, 1, file);
    fread(&(p->lv), 4, 1, file);
    fread(&(p->name), 31, 1, file);
    fread(&(p->skillCount), 4, 1, file);
    p->skills = NULL;
    for (int tmp2 = 0; tmp2 < p->skillCount; tmp2++)
    {
        fread(&tmp, 4, 1, file);
        p->skills = addNode(p->skills, tmp);
    }
    p->expthr = 10 * pow(1.2, p->lv - 1);
    p->autoHeal = 0, p->healValue = 0;
    p->trueElement = ELT_NONE;
    fclose(file);
    printf("Game file loaded successfully!\n");
    return 0;
}
int newGame(character *p)
{
    p->hp = 10, p->pa = 10, p->ma = 10, p->pd = 10, p->md = 10, p->EXP = 0, p->lv = 1, p->expthr = 10, p->maxhp = 10, p->mp = 100, p->st = 100;
    p->skillCount = 0, p->skills = NULL, p->autoHeal = 0, p->healValue = 0, p->trueElement = ELT_NONE;
    addSkill(p, sk[0].id);
    printf("Enter player name:\n");
    scanf("%s", p->name);
    for (int tmp = 0; tmp < 4; tmp++)
        character_levelup(p);
    p->hp = p->maxhp, p->EXP = 0;
    if (saveGame(p) == 1)
    {
        return 1;
    }
    return 0;
}
void expCheck(character *p)
{
    while (p->EXP >= p->expthr)
    {
        character_levelup(p);
    }
}
int titleScreen(character *p)
{
    printf("\e[1;1H\e[2J");
    int tmp;
    printf("HEART OF ADVENTURE\n");
    printf("Select Option: (1) Load Game (2) New Game (3) Exit\n");
    scanf("%d", &tmp);
    getchar();
    if (tmp == 1)
    {
        if (loadGame(p) == 1)
        {
            return 1;
        }
    }
    else if (tmp == 2)
    {
        if (newGame(p) == 1)
        {
            return 1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}
void battleSimulate(character *p1, character* p2, int isMonster)
{
    preBattleStatsRestore(p1);
    preBattleStatsRestore(p2);
    BattleStatsRestore(p1, 0);
    BattleStatsRestore(p2, isMonster);
    int winner = 0, turn = rand() % 2;
    while (winner == 0)
    {
        printf("\e[1;1H\e[2J");
        if (p1->hp > p1->maxhp)
            p1->hp = p1->maxhp;
        if (p2->hp > p2->maxhp)
            p2->hp = p2->maxhp;
        statsBooster(p1);
        statsBooster(p2);
        postSkillCard(p1, p2);
        character* p = (turn == 0)? p1: p2;
        character *px = (turn == 0)? p2 : p1;
        printf("%s's move!\n", p->name);
        if (isMonster == 0 || turn == 0)
        {
            int tmp;
            printf("Enter (1) Use Skill\n");
            scanf("%d", &tmp);
            getchar();
            switch (tmp)
            {
                case 1:
                display_availableSkills(p, px);
                break;
            }
        }
        else
        {
            getchar();
            int tmp = rand() % (p->skillCount);
            node* tmpn;
            for (tmpn = p->skills; tmp > 0; tmp--)
            {
                tmpn = tmpn->next;
            }
            activateSkill(tmpn->nodeValue, p, px);
        }
        if (p1->hp < 1)
        {
            winner = 2;
            endBattle(p2, p1);
            return;
        }
        else if (p2->hp < 1)
        {
            winner = 1;
            endBattle(p1, p2);
            return;
        }
        if (turn == 0 || isMonster == 0)
        {
            if (p->escapes[0] == 1 && px->hp <= .25 * px->maxhp)
            {
                p->escapes[0] = 0;
                if (midBattleEscape(p, px) == 1)
                {
                    return;
                }
            }
            else if (p->escapes[1] == 1 && px->hp <= .5 * px->maxhp && px->hp > .25 * px->maxhp)
            {
                p->escapes[1] = 0;
                if (midBattleEscape(p, px) == 1)
                {
                    return;
                }
            }
            else if (p->escapes[2] == 1 && px->hp <= .75 * px->maxhp && px->hp > .5 * px->maxhp)
            {
                p->escapes[2] = 0;
                if (midBattleEscape(p, px) == 1)
                {
                    return;
                }
            }
        }
        autoRecovery(p1, p2, 2.5);
        turn = (turn + 1) % 2;

        // Basically these functions deduct rounds as rounds are completed from effects based on rounds
        autohealCheck(p1, p2);
        autohealCheck(p2, p1);
        evadeCheck(p1, p2);
        evadeCheck(p2, p1);
        statsBoostCheck(p1);
        statsBoostCheck(p2);
        elementChangeCheck(p1);
        elementChangeCheck(p2);
        getchar();
    }
}
node* addNode(node* n, int value)
{
    node* tmp = malloc(sizeof(node));
    tmp->nodeValue = value;
    tmp->next = n;
    return tmp;
}
void addSkill(character *p, int skillID)
{
    p->skillCount++;
    p->skills = addNode(p->skills, skillID);
}
void hospital(character* p)
{
    p->hp = p->maxhp, p->st = 100, p->mp = 100;
    printf("%s's life was fully restored!\n", p->name);
}
void autohealCheck(character*p, character* px)
{
    if (p->autoHeal > 0)
    {
        float tmpf = p->hp;
        if (p->healValue > 0)
        {
            p->hp += p->maxhp * p->healValue * 0.01;
            if (p->hp > p->maxhp)
            {
                p->hp = p->maxhp;
            }
        }
        else
        {
            p->hp += px->maxhp * p->healValue * 0.01;
            if (p->hp < 0)
            {
                p->hp = 0;
            }
        }
        p->autoHeal--;
        tmpf = p->hp - tmpf;
        if (p->healValue > 0)
        {
            printf("%s was auto healed! HP: %.0f (+%.0f)\n", p->name, p->hp * 10, tmpf * 10);
        }
        else
        {
            printf("%s was poisoned! HP: %.0f (%.0f)\n", p->name, p->hp * 10, tmpf * 10);
        }
        if (p->autoHeal == 0)
        {
            if (p->healValue > 0)
            {
                printf("%s's autoHeal effect has worn off!\n", p->name);
            }
            else
            {
                printf("%s's poison effect has worn off!\n", p->name);
            }
        }
    }
}
void evadeCheck(character *p, character *px)
{
    if (px->isEvaded == 1)
    {
        return;
    }
    if (p->evades > 0)
    {
        int tmp = ((float) p->evades / p->evadeRounds) * 100;
        if (tmp >= RNG(0, 100))
        {
            p->evades--;
            p->evadeRounds--;
            px->isEvaded = 1;
            return;
        }
        p->evadeRounds--;
    }
}
void statsBoostCheck(character* p)
{
    if (p->paBoost != 0)
    {
        p->paBrounds--;
        if (p->paBrounds == 0)
        {
            p->paBoost = 0;
            printf("%s's PA boost effect has worn off!\n", p->name);
        }
    }
    if (p->pdBoost != 0)
    {
        p->pdBrounds--;
        if (p->pdBrounds == 0)
        {
            p->pdBoost = 0;
            printf("%s's PD boost effect has worn off!\n", p->name);
        }
    }
    if (p->maBoost != 0)
    {
        p->maBrounds--;
        if (p->maBrounds == 0)
        {
            p->maBoost = 0;
            printf("%s's MA boost effect has worn off!\n", p->name);
        }
    }
    if (p->mdBoost != 0)
    {
        p->mdBrounds--;
        if (p->mdBrounds == 0)
        {
            p->mdBoost = 0;
            printf("%s's MD boost effect has worn off!\n", p->name);
        }
    }
}
void statsBooster(character* p)
{
    p->pa = p->truepa + (p->truepa * p->paBoost / 100);
    p->pd = p->truepd + (p->truepd * p->pdBoost / 100);
    p->ma = p->truema + (p->truema * p->maBoost / 100);
    p->md = p->truemd + (p->truemd * p->mdBoost / 100);
}
void BattleStatsRestore(character* p, int isMonster)
{
    p->paBoost = 0;
    p->pdBoost = 0;
    p->maBoost = 0;
    p->mdBoost = 0;
    p->paBrounds = 0;
    p->pdBrounds = 0;
    p->maBrounds = 0;
    p->mdBrounds = 0;
    p->evades = 0;
    p->evadeRounds = 0;
    p->isEvaded = 0;
    p->element = p->trueElement;
    if (isMonster == 0)
    {
        p->autoHeal = 0;
        p->healValue = 0;
    }
    p->lastResortUsed = 0;
    p->escapes[0] = 1, p->escapes[1] = 1, p->escapes[2] = 1;
    p->kikensUsed = 0;
}
void preBattleStatsRestore(character* p)
{
    p->truepa = p->pa, p->truepd = p->pd, p->truema = p->ma, p->truemd = p->md;
}
void postBattleStatsRestore(character* p)
{
    p->pa = p->truepa;
    p->ma = p->truema;
    p->pd = p->truepd;
    p->md = p->truemd;
}

void freeLinkedList(node* n)
{
    for (node* tmpn = n; tmpn != NULL;)
    {
        node* tmpn2 = tmpn;
        tmpn = tmpn->next;
        free(tmpn2);
    }
}
void freeMemoryAtExit(character* p1, character* p2)
{
    freeLinkedList(p1->skills);
    freeLinkedList(p2->skills);
    for (int tmp = 0; tmp < TOTALMONSTERCOUNT; tmp++)
    {
        freeLinkedList(MONS[tmp].skills);
    }
}
void skillSelection(character* p, int arr[], int n)
{
    printf("%s can learn one of the following skills. Select preference:\n", p->name);
    for (int tmp = 0; tmp < n; tmp++)
    {
        printf("(%d) ", tmp + 1);
        showSKILLdetails(arr[tmp]);
    }
    printf("Your choice: ");
    int tmp;
    scanf("%d", &tmp);
    if (tmp < n && tmp >= 1)
    {
        addSkill(p, arr[tmp - 1]);
        printf("%s learned the skill %s!\n", p->name, sk[getSKILLindex(arr[tmp - 1])].name);
    }
    else
    {
        addSkill(p, arr[n - 1]);
        printf("%s learned the skill %s!\n", p->name, sk[getSKILLindex(arr[n - 1])].name);
    }
    getchar();
    printf("\e[1;1H\e[2J");
}
void autoRecovery(character* p1, character* p2, float inc)
{
    p1->st += inc, p1->mp += inc, p2->st += inc, p2->mp += inc;
    if (p1->st > 100)
        p1->st = 100;
    if (p1->mp > 100)
        p1->mp = 100;
    if (p2->st > 100)
        p2->st = 100;
    if (p2->mp > 100)
        p2->mp = 100;
}
void endBattle(character* winner, character* loser)
{
    printf("%s fainted!\n%s wins!\n", loser->name, winner->name);
    getchar();
    winner->EXP += loser->expthr;
    loser->hp = 0;
    postBattleStatsRestore(loser);
    postBattleStatsRestore(winner);
    BattleStatsRestore(loser, 0);
    BattleStatsRestore(winner, 0);
    expCheck(winner);
    getchar();
    printf("\e[1;1H\e[2J");
}
void elementChangeCheck(character* p)
{
    if (p->element != p->trueElement)
    {
        p->eltChangeRounds--;
        if (p->eltChangeRounds == 0)
        {
            p->element = p->trueElement;
            printf("%s's element has been set back to their true element!\n", p->name);
        }
    }
}
int midBattleEscape(character* p, character* px)
{
    int tmp;
    printf("%s has an opportunity to escape! Escape? (0) Yes (1) No\n", p->name);
    scanf("%d", &tmp);
    getchar();
    if (tmp == 0)
    {
        printf("%s fleed!\n", p->name);
        postBattleStatsRestore(p);
        postBattleStatsRestore(px);
        BattleStatsRestore(p, 0);
        BattleStatsRestore(px, 0);
        getchar();
        printf("\e[1;1H\e[2J");
        return 1;
    }
    return 0;
}
int randomMonsterGenerator(char* arr)
{
    int len = strlen(arr);
    int number = 0, entries = 1;
    node* list = NULL;
    char* tmpc;
    for (int tmp = 0; tmp < len; tmp++)
    {
        if (arr[tmp] != ' ')
        {
            number *= 10;
            number += arr[tmp] - 48;
        }
        else
        {
            list = addNode(list, number);
            number = 0;
            entries++;
        }
    }
    list = addNode(list, number);
    int tmp = rand() % entries, tmp2 = 0;
    node* tmpn;
    for (tmpn = list; tmp2 < tmp; tmpn = tmpn->next)
    {
        tmp2++;
    }
    tmp = tmpn->nodeValue;
    freeLinkedList(list);
    return tmp;
}