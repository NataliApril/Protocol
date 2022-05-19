#define SERIAL_BLA Serial
#define SERIAL_TM Serial3
#define SERIAL_NKR Serial2

//   Формирование паектов
#define PACK_START_1 0x7D
#define PACK_START_2 0x4F

#define PACK_END_1 0x8D
#define PACK_END_2 0x5F

#define NKR_ID 0xB0
#define BLA_ID 0xB1

#define CTT_ID 0xA0
#define TCM_ID 0xA1
#define RTM_ID 0xA2
#define MCM_ID 0xA3
#define LRQ_ID 0xA4
#define RTL_ID 0xA5
#define CTL_ID 0xA6
#define LCM_ID 0xA7
#define NPM_ID 0xA8
#define TPM_ID 0xA9
#define PDR_ID 0xE0
//запрос на получение пакета NPM
#define TNP_ID 0xC0

#define MCM_LENGTH 12
#define NPM_LENGTH 24
#define TPM_LENGTH_NKR 26
#define TPM_LENGTH_BLA 14
#define SHORT_MSG_LEN 2

//*********Массив для хранения получаемых пакетов*********//
byte big_array_NKR [5][35];
byte big_array_BLA [5][35];

//счетчик количества осмотренных точек
byte RTM_count = 0;

const int number_of_packets = 12;

//Массивы с данными о пакетах
byte id[number_of_packets] = {CTT_ID, TCM_ID, RTM_ID, MCM_ID, LRQ_ID, RTL_ID, CTL_ID, LCM_ID, NPM_ID, TPM_ID, PDR_ID, TNP_ID};
byte lenght_NKR[number_of_packets] = {0, 0, 0, MCM_LENGTH, 0, 0, 0, 0, NPM_LENGTH, TPM_LENGTH_NKR, SHORT_MSG_LEN, 0};
byte lenght_BLA[number_of_packets] = {0, 0, 0, MCM_LENGTH, 0, 0, 0, 0, NPM_LENGTH, TPM_LENGTH_BLA, SHORT_MSG_LEN, 0};

//переменные дл выбоора режимов передачи пакета
boolean Manual = false;
boolean Auto = false;

bool flag_NPM = false;

//переменные для разбора полученных пакетов
byte recive_bytes[80] = {0};
byte pack_bytes[50] = {0};
byte data[50] = {0};
static byte packs[10];
byte flag = 0;
int status = 0;
int j = 0;
byte pakage_id;
byte num_pakage;
byte crc;
byte counter = 0;
byte len;
bool packet_ready = false;
int i = 0;
byte count_1_NKR = 0;
byte count_1_BLA = 0;
byte tpm_coord_count = 0;
byte tpm_ori_count = 0;
byte count_tpm_num = 0;
byte waiting_action_BLA = 0;
byte waiting_action_NKR = 0;

//определния для передачи пакетов
String input_string = "";
bool ctt = false;
bool tcm = false;
bool rtm = false;
bool mcm = false;
bool lrq = false;
bool rtl = false;
bool ctl = false;
bool lcm = false;
bool npm = false;
bool tpm = false;
bool pdr = false;
bool coord_1 = false;
bool coord_2 = false;
bool coord_3 = false;

//переменная для нахождения ответа 
static bool exist = false;
////Индикатор пакета
//byte action = 0;
//crc полученног пакета для отправки отклика на НКР
byte crc_pack = 0;
//Создаем массив с пакетом, для текущей работы
byte packet_sender_BLA [35];
byte packet_sender_NKR [35];

byte tpm_pack_BLA[35];
//массив с координатами точки интереса
byte orient_tpm[35];
byte coord_tpm[35];

byte tpm_count = 0;

byte f_NKR = 0;
byte f_BLA = 0;

float coords[3] = {0, 0, 0};

//Таймер отправки запросов на получение пакетов NPM
unsigned long timmer;
//Интерва времения в мс для запросов пакетов NPM
unsigned long interval;
//таймер обновлегия таймера отправки запросов
boolean flag_timmer = true;

//Описание структур
struct short_msg
{
  byte header_1;
  byte header_2;
  byte robot_id;
  byte pack_id;
  short unsigned int data_len;
  byte crc;
  byte terminator_1;
  byte terminator_2;
};

union short_msg_union
{
  short_msg msg_form;
  byte byte_form[sizeof(short_msg)];
};

struct mcm_msg
{
  byte header_1;
  byte header_2;
  byte robot_id;
  byte pack_id;
  short unsigned int data_len;
  float coordinates[3];
  byte crc;
  byte terminator_1;
  byte terminator_2;
};

union mcm_msg_union
{
  mcm_msg msg_form;
  byte byte_form[sizeof(mcm_msg)];
};

struct npm_msg
{
  byte header_1;
  byte header_2;
  byte robot_id;
  byte pack_id;
  short unsigned int data_len;
  float coordinates[3];
  float orientation[3];
  byte crc;
  byte terminator_1;
  byte terminator_2;
};

union npm_msg_union
{
  npm_msg msg_form;
  byte byte_form[sizeof(npm_msg)];
};

struct tpm_msg_NKR
{
  byte header_1;
  byte header_2;
  byte robot_id;
  byte pack_id;
  short unsigned int data_len;
  short unsigned int point_id;
  float coordinates[3];
  float orient[3];
  byte crc;
  byte terminator_1;
  byte terminator_2;
};

union tpm_msg_NKR_union
{
  tpm_msg_NKR msg_form;
  byte byte_form[sizeof(tpm_msg_NKR)];
};

struct tpm_msg_BLA
{
  byte header_1;
  byte header_2;
  byte robot_id;
  byte pack_id;
  short unsigned int data_len;
  short unsigned int point_id;
  float coordinates[3];
  byte crc;
  byte terminator_1;
  byte terminator_2;
};

union tpm_msg_BLA_union
{
  tpm_msg_BLA msg_form;
  byte byte_form[sizeof(tpm_msg_BLA)];
};

struct pack_recived
{
  byte header_1;
  byte header_2;
  byte robot_id;
  byte pack_id;
  short unsigned int data_len;
  byte recived_pack_id;
  byte recived_pack_crc;
  byte crc;
  byte terminator_1;
  byte terminator_2;
};

union pack_recived_union
{
  pack_recived msg_form;
  byte byte_form[sizeof(pack_recived)];
};
