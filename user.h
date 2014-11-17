#define LED_SZ 8        // 8x8LED
#define LED_MAX LED_SZ-1
#define FIELD_SZ 16     // ゲームフィールドサイズ
#define FIELD_MAX FIELD_SZ-1
// ダンジョンの状態 オブジェクトID
#define ID_PASSAGE 0     // 通路
#define ID_WALL 1     // 壁
#define ID_PLAYER 2
#define ID_MOB 3
#define ID_BOMB 4
// 爆弾の定義
#define BOMB_TIMELIMIT 5        // [sec]
typedef unsigned char uchar;    // 1byte
typedef unsigned int  uint;     // 2byte
typedef unsigned long ulong;    // 4byte
enum
{
    BEEP_LOW,
    BEEP_HIGH
};
extern void _sound(uchar tone);
extern volatile uchar sw;
extern volatile uchar led[LED_SZ];
extern volatile uchar gameover;
extern void user_init(void);
extern void user_main(void);
