#include<cstdio>
#include<string>
#include<cstring>
#include<vector>
#define kind_num 5
using namespace std;
/*
string Warrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" };
红方司令部按照 iceman、lion、wolf、ninja、dragon 的顺序制造武士。
蓝方司令部按照 lion、dragon、ninja、iceman、wolf 的顺序制造武士。
*/

class Camp;
class Warrior
{
private:
    Camp* pc;
    int wa_id;//武士的种类编号 0 dragon 1 ninja 2 iceman 3 lion 4 wolf
    int no;//武士的编号
public:
    //int ap;//攻击力
    static string name[kind_num];
    static int strength[kind_num];
    const string weapons[3] = { "sword", "bomb","arrow" };
    Warrior(Camp* p, int no_, int wa_id_,int time);
};
class Camp
{
private:

    bool sign;
    int wa_num;
    int color;
    int wa_id;//当前要制造的武士是制造序列中的第几个
    int warrior[kind_num];//存放每种武士的数量
    vector<Warrior*> Warriors;
public:
    int HP;
    friend class Warrior;
    static int Seq[2][kind_num]; //武士的制作顺序序列
    void Init(int coco, int lv);
    ~Camp();
    bool Produce(int time);
    string Get_Color();
};
string Warrior::name[kind_num] = { "dragon","ninja","iceman","lion","wolf" };
int Warrior::strength[kind_num];
int Camp::Seq[2][kind_num] = { { 2,3,4,1,0 },{3,0,1,2,4} }; //两个司令部武士的制作顺序序列


class dragon : public Warrior {
public:    
    string weapon;
    double morale;

    dragon(Camp* p, int no_, int wa_id_,int time) :Warrior( p, no_,wa_id_,time) {
        weapon = weapons[no_%3];
        morale = (double)p->HP / strength[0];
        printf("It has a %s,and it's morale is %.2f\n",weapon.c_str(),morale);
    }

};
class ninja : public Warrior {
public:
    string weapon[2];
    ninja(Camp* p, int no_, int wa_id_,int time) :Warrior(p, no_, wa_id_,time) {
        weapon[0]= weapons[no_ % 3];
        weapon[1] = weapons[(no_+1) % 3];
        printf("It has a %s and a %s\n", weapon[0].c_str(), weapon[1].c_str());
    }
};
class iceman : public Warrior {
public:
    string weapon;
    iceman(Camp* p, int no_, int wa_id_,int time) :Warrior(p, no_, wa_id_,time) {
        weapon = weapons[no_ % 3];
        printf("It has a %s\n", weapon.c_str());
    }

};
class lion : public Warrior {
public:
    int loyalty;
    lion(Camp* p, int no_, int wa_id_,int time) :Warrior(p, no_, wa_id_,time) {
        loyalty = p->HP;
        printf("It's loyalty is %d\n", loyalty);
    }
};
class wolf : public Warrior {
public:
    wolf(Camp* p, int no_, int wa_id_, int time):Warrior(p, no_, wa_id_, time){}
};



Warrior::Warrior(Camp* pCamp, int no_, int wa_id_,int time)
{
    no = no_;
    wa_id = wa_id_;
    pc = pCamp;
    string color = pc->Get_Color();
    printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",
        time, color.c_str(), name[wa_id].c_str(), no, strength[wa_id],
        pc->warrior[wa_id], name[wa_id].c_str(), color.c_str());
}

void Camp::Init(int coco, int health)
{
    color = coco;
    HP = health;
    wa_num = 0;
    sign = true;
    wa_id = 0;
    memset(warrior, 0, sizeof(warrior));
}
Camp::~Camp()
{
    Warriors.clear();
}
bool Camp::Produce(int time)
{
    if (!sign) return 0;
    int cnt = 0;
    while (Warrior::strength[Seq[color][wa_id]] > HP && cnt < kind_num) {
        wa_id = (wa_id + 1) % kind_num;
        cnt++;
    }
    int kind = Seq[color][wa_id];
    if (Warrior::strength[kind] > HP) {
        sign = false;
        if (color) printf("%03d blue headquarter stops making warriors\n", time);
        else printf("%03d red headquarter stops making warriors\n", time);
        return false;
    }
    //制作士兵：
    HP -= Warrior::strength[kind];
    wa_id = (wa_id + 1) % kind_num;
    Warrior* newW=NULL;
    warrior[kind]++;
    wa_num++;
    switch (kind)
    {
    case 0: {
        newW = new dragon(this, wa_num, kind, time);
        break;
    }
    case 1: {
        newW = new ninja(this, wa_num, kind, time);
        break;
    }
    case 2: {
        newW = new iceman(this, wa_num, kind, time);
        break;
    }
    case 3: {
        newW = new lion(this, wa_num, kind, time);
        break;
    }
    case 4: {
        newW = new wolf(this, wa_num, kind, time);
        break;
    }
    default:
        break;
    }
    
    Warriors.push_back(newW);
    return true;
}
string Camp::Get_Color()
{
    if (color) return "blue";
    else return "red";
}

int main()
{
    int T, m;
    Camp Red, Blue;
    scanf_s("%d", &T);
    for (int i = 1; i <= T; i++) {
        printf("Case:%d\n", i);
        scanf_s("%d", &m);
        for (int i = 0; i < kind_num; i++)
            scanf_s("%d", &Warrior::strength[i]);
        Red.Init(0, m);
        Blue.Init(1, m);
        int time = 0;
        bool signr, signb;
        do {
            signr= Red.Produce(time);
            signb = Blue.Produce(time);
            time++;
        }while(signr || signb);
    }
    return 0;
}