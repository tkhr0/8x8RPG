#include "user.h"


// 構造体
typedef struct player_s{
    uchar dir;
    uchar x;
    uchar y;
    uchar hp;
}player_t;

typedef struct mob_s{
    uchar active;
    uchar atack;
    uchar x;
    uchar y;
    uchar hp;
} mob_t;
// ローカル関数
static void MoveEnemy(void);
static void MoveBullet(void);
static void MoveFort(void);
static void UpdateLED(void);
// グローバル変数
volatile uchar sw = 0;      // 押しボタン
volatile uchar led[LED_SZ]; // マトリクスLED
volatile uchar gameover = 0;// ゲーム終了フラグ
// ローカル変数
volatile ulong field[FIELD_SZ] = {0};
volatile player_t player;
volatile uchar marker_f;
volatile uchar print = 0x00;
volatile uchar print1 = 0x00;
volatile uchar print2 = 0x00;

// プロトタイプ
void initField(void);
void changeDirection(void);
uchar searchFront(uchar, uchar, uchar);
void walk(void);
void showMarker(void);
void showDungeon(void);

/*
    フレームワークから呼ばれる関数群
*/
/*
    ユーザ処理の初期化
 */
void user_init(void)
{
    gameover = 0;
    player.x = 26;
    player.y = 2;
    player.dir = 0;
    marker_f = 1;
    initField();
}
/*
    ユーザ処理
 */
void user_main(void)
{
    MoveEnemy();
    MoveBullet();
    MoveFort();
    UpdateLED();
}
/*
    敵の移動
 */
static void MoveEnemy(void)
{
}
/*
    弾の移動
 */
static void MoveBullet(void)
{
}

/*
    プレイヤーの移動
 */
static void MoveFort(void) {
    switch(sw){
        case 1:
            if(searchFront(player.x, player.y, player.dir) == P)
                walk();
            break;
        case 2:
            changeDirection();
            break;
    }
}
/*
    LED表示の更新
 */
static void UpdateLED(void)
{

    showDungeon();
    led[3] |= 0x10; // プレイヤーの位置
    // 方向
    showMarker();

    print = player.x;
    //print1 = player[1];

    led[0] = print;
    led[1] = print1;
    led[6] = print2;
}

/*
    オリジナル関数群
*/
// フィールドの初期化
void initField(){
    uchar i;
    /*
    for(i=0; i<FIELD_SZ; i++){
        //if(i==0 || i==FIELD_SZ-1)
            field[i] = 0xffffffff;
        //else
        //    field[i] = 0x80000001;
    }
    */

    field[31] = 0b11111111111111111111111111111111;
    for(i=1; i<31; i++)
        field[i]  = 0b10000000000000000000000000000001;
    field[0]  = 0b11111111111111111111111111111111;

}

// プレイヤーの移動方向を変更
void changeDirection(){
    player.dir = (player.dir + 1) % 4;
    marker_f = 1;
}

// 前方のオブジェクトを返す
// pos, direction
uchar searchFront(uchar x, uchar y, uchar dir){
    ulong mask = (0x80000000 >> x);   // マスク

    switch(dir){
        case 0:     // 右の値を返す
            return ((field[y] & (mask>>1)) >> (FIELD_SZ - (x+2)));
            break;
        case 1:     // 上
            return ((field[y+1] & mask) >> (FIELD_SZ - (x+1)));
            break;
        case 2:     // 左
            return ((field[y] & (mask<<1)) >> (FIELD_SZ - x));
            break;
        case 3:     // 下
            return ((field[y-1] & mask) >> (FIELD_SZ - (x+1)));
            break;
    }
}

/*
    プレイヤーの移動
    directionに従ってプレイヤーの座標を更新する
*/
void walk(){
    switch(player.dir){
        case 0: player.x++;    // 右
            break;
        case 1: player.y++;    // 上
            break;
        case 2: player.x--;    // 左
            break;
        case 3: player.y--;    // 下
            break;
    }
    marker_f = 0;
}

// プレイヤーの進行方向を示すマーカーを表示する
void showMarker(){
    if(marker_f){
        switch(player.dir){
        case 0: led[3] |= 0x08;    // 右
            break;                      
        case 1: led[4] |= 0x10;    // 上
            break;                      
        case 2: led[3] |= 0x20;    // 左
            break;                      
        case 3: led[2] |= 0x10;    // 下
            break;
        }
        led[7] = player.dir;
    }
}

void showDungeon(){
    uchar i;
    ulong mask;    // マスク
    signed char curx;  // 左端を基準にする
    signed char cury = player.y-4;  // 下を基準に
    ulong row;
    for(i=0; i<8; i++){
        led[i] = 0x00;
        print1 = 0;
        // ダンジョンの表示
        if(player.x-3 < 0){   // 左端
            curx = player.x - 3;
            mask = 0xff000000;
            led[i] = (uchar)((field[cury+i] & (mask << (-curx))) >> (FIELD_SZ - LED_SZ - curx));
        }else if(3 <= curx && curx < FIELD_MAX-4){  // 真ん中
        }else if(FIELD_MAX-4 < player.x){  // 右端
            mask = (0xff >> (player.x - (FIELD_MAX - 4)));
            led[i] = (uchar)((field[cury+i] & mask) << (player.x - (FIELD_MAX - 4)));
        }
        /*
        if(curx < 0)     // フィールドの左端を壁で表示
            led[i] |= (0xff << (LED_SZ - (curx * (-1))));

        else if((FIELD_SZ - LED_SZ) < curx)  // フィールドの右端を壁で表示
            led[i] |= ~(0xff << (curx - (FIELD_SZ - LED_SZ - 1)));
           if(0 < cury && cury < FIELD_SZ)
           if(i < (cury*(-1)))
           led[i] |= 0xff;
         */
        // ダンジョンの表示　ここまで
    }
}

