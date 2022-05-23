
#define SERIAL_BLA Serial1
#define SERIAL_TM Serial

#include "wagp_packer_parcer.h"

wagp_packer BLA_packer("BLA");

bool mode = false;

float orientation[3] = {56.050927, 37.469743, 1.11111};

float coords_data [11][3] =
{
  {56.050863, 37.469761, 3.111111},   //точка 0
  {56.050933, 37.469844, 3.111111},   //точка 1
  {56.050988, 37.469726, 3.111111},   //точка 2
  {56.050919, 37.469645, 3.111111},   //точка 3

  {56.049633, 37.462274, 3.111111},   //точка 4
  {56.049576, 37.462391, 3.111111},   //точка 5
  {56.049643, 37.462481, 3.111111},   //точка 6
  {56.049701, 37.462374, 3.111111},   //точка 7
  {56.049633, 37.462274, 3.111111},   //точка 8
  {56.049576, 37.462391, 3.111111},   //точка 9
  {56.049643, 37.462481, 3.111111}    //точка 10
};

bool coord_flag [11] =
{
  true,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false
};


void setup()
{
  SERIAL_TM.begin(115200);
  SERIAL_BLA.begin(19200);

  //Вводим режим работы контроллера
  // 1 - передача команд, полк=учаемых с клавиатуры
  // 2 - "сквозная передача" получаемых пакетов
  SERIAL_TM.println("Выберете режим ввода");
  while (!mode) {
    if (SERIAL_TM.available() > 0) {
      char d = SERIAL_TM.read();
      if (d == '\n') {
        SERIAL_TM.println(input_string);
        if (input_string.equals("MW"))
        {
          SERIAL_TM.println("MANUAL mode");
          Manual = true;
          mode = true;
        } else if (input_string.equals("AW"))
        {
          SERIAL_TM.println("AUTO mode");
          Auto = true;
          mode = true;
          ctt = true;
        } else {
          SERIAL_TM.println("Mode selection error");
        }
        input_string = "";
      }
      input_string += d;
    }
  }
  //  Auto = true;
  //  ctt = true;
}

void loop()
{
  while (Manual)
  {
    Manual_write();
  }
  while (Auto)
  {
    Auto_write();
  }
}

byte num_point = 0;
byte num_point_npm = 4;

void Manual_write ()
{
  if (SERIAL_TM.available() > 0) {
    //Serial.println("Жду команду:");
    char c = SERIAL_TM.read();
    if (c == '\n') {
      SERIAL_TM.println();
      SERIAL_TM.println(input_string);
      if (input_string.equals("CTT")) {
        //        ctt = true;
        //        while (ctt) {
        num_point = 0;
        num_point_npm = 4;
        SERIAL_TM.println("num_point = " + String(num_point));
        short_msg_union msg = BLA_packer.pack_ctt();
        SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //        Serial1.write(msg.byte_form, sizeof(msg.byte_form));
        //        Serial1.write(msg.byte_form, sizeof(msg.byte_form));
        //          if (find_pack() == PDR_ID)
        //          {
        //            ctt = false;
        //          }
        //        }
      } else if (input_string.equals("TCM")) {
        //        tcm = true;
        //        while (tcm) {
        short_msg_union msg = BLA_packer.pack_tcm();
        SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //          if (find_pack() == PDR_ID)
        //          {
        //            tcm = false;
        //          }
        //        }
      } else if (input_string.equals("RTM")) {
        //        rtm = true;
        //        while (rtm) {
        short_msg_union msg = BLA_packer.pack_rtm();
        SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //          if (find_pack() == PDR_ID)
        //          {
        //            rtm = false;
        //          }
        //        }
      } else if (input_string.equals("MCM")) {
        //        mcm = true;
        //        while (mcm) {
        mcm_msg_union msg;
        float coords_0[3] = {56.050796, 37.469750, 0.111111};
        msg = BLA_packer.pack_mcm(coords_0);
        SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //          if (find_pack() == PDR_ID)
        //          {
        //            mcm = false;
        //          }
        //        }
      } else if (input_string.equals("LRQ")) {
        //        lrq = true;
        //        while (lrq) {
        short_msg_union msg = BLA_packer.pack_lrq();
        SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //          if (find_pack() == PDR_ID)
        //          {
        //            lrq = false;
        //          }
        //        }
      } else if (input_string.equals("RTL")) {
        //        rtl = true;
        //        while (rtl) {
        short_msg_union msg = BLA_packer.pack_rtl();
        SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //          if (find_pack() == PDR_ID)
        //          {
        //            rtl = false;
        //          }
        //        }
      } else if (input_string.equals("CTL")) {
        //        ctl = true;
        //        while (ctl) {
        short_msg_union msg = BLA_packer.pack_ctl();
        SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //          if (find_pack() == PDR_ID)
        //          {
        //            ctl = false;
        //          }
        //        }
      } else if (input_string.equals("LCM")) {
        //        lcm = true;
        //        while (lcm) {
        short_msg_union msg = BLA_packer.pack_lcm();
        SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //          if (find_pack() == PDR_ID)
        //          {
        //            lcm = false;
        //          }
        //        }
      } else if (input_string.equals("NPM")) {
        //        npm = true;
        //        while (npm) {
        npm_msg_union msg;
        float coords_0[3] = {coords_data[num_point_npm][0], coords_data[num_point_npm][1], coords_data[num_point_npm][2]};
        float orientation_0[3] = {0.56, 0.2, 73.23};
        num_point_npm += 1;
        msg = BLA_packer.pack_npm(coords_0, orientation_0);
        SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //          if (find_pack() == PDR_ID)
        //          {
        //            npm = false;
        //          }
        //        }
      } else if (input_string.equals("TPM")) {
        //        tpm = true;
        //        while (tpm) {

        tpm_msg_union msg;
        float coords[3] = {coords_data[num_point][0], coords_data[num_point][1], coords_data[num_point][2]};
        num_point += 1;
        msg = BLA_packer.pack_tpm(num_point, coords, orientation);
        SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
        //SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //        num_point += 1;
        SERIAL_TM.println("num_point = " + String(num_point));
        //          if (find_pack() == PDR_ID)
        //          {
        //            tpm = false;
        //          }
        //        }
      } else if (input_string.equals("PDR")) {
        //        pdr = true;
        //        while (pdr) {
        pack_recived_union msg;
        msg = BLA_packer.pack_pdr(0xA2, 0x00);
        SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //        SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
        //          if (find_pack() == PDR_ID)
        //          {
        //            pdr = false;
        //          }
        //        }
      }
      input_string = "";
    } else {
      input_string += c;
    }
  }
}

void Auto_write ()
{
  while (ctt) {
    short_msg_union msg = BLA_packer.pack_ctt();
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
    //Serial1.write(msg.byte_form, sizeof(msg.byte_form));
    if (find_pack() == PDR_ID)
    {
      ctt = false;
      tcm = true;
    }
  }  while (tcm) {
    short_msg_union msg = BLA_packer.pack_tcm();
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
    if (find_pack() == PDR_ID)
    {
      tcm = false;
      rtm = true;
    }
  } while (rtm) {
    short_msg_union msg = BLA_packer.pack_rtm();
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
    if (find_pack() == PDR_ID)
    {
      rtm = false;
      mcm = true;
    }
  } while (mcm) {
    mcm_msg_union msg;
    float coords_0[3] = {56.050796, 37.469750, 0.111111};
    msg = BLA_packer.pack_mcm(coords_0);
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
    if (find_pack() == PDR_ID)
    {
      mcm = false;
      lrq = true;
    }
  }  while (lrq) {
    short_msg_union msg = BLA_packer.pack_lrq();
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
    if (find_pack() == PDR_ID)
    {
      lrq = false;
      rtl = true;
    }
  } while (rtl) {
    short_msg_union msg = BLA_packer.pack_rtl();
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
    if (find_pack() == PDR_ID)
    {
      rtl = false;
      ctl = true;
    }
  } while (ctl) {
    short_msg_union msg = BLA_packer.pack_ctl();
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
    if (find_pack() == PDR_ID)
    {
      ctl = false;
      lcm = true;
    }
  } while (lcm) {
    short_msg_union msg = BLA_packer.pack_lcm();
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
    if (find_pack() == PDR_ID)
    {
      lcm = false;
      npm = true;
    }
  } while (npm) {
    npm_msg_union msg;
    float coords_0[3] = {56.050796, 37.469750, 0.111111};
    float orientation_0[3] = {0.56, 0.2, 73.23};
    msg = BLA_packer.pack_npm(coords_0, orientation_0);
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
    if (find_pack() == PDR_ID)
    {
      npm = false;
      tpm = true;
    }
  } while (tpm) {
    tpm_msg_union msg;
    float coords_0[3] = {56.050796, 37.469750, 0.111111};
    msg = BLA_packer.pack_tpm(0, coords_0, orientation);
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
    if (find_pack() == PDR_ID)
    {
      tpm = false;
      ctt = true;
    }
  }
  /*while (pdr) {
    pack_recived_union msg;
    msg = BLA_packer.pack_pdr(0x00, 0x00);
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
    Serial1.write(msg.byte_form, sizeof(msg.byte_form));
    if (find_pack() == PDR_ID)
    {
      pdr = false;
    }
    }*/
}

//функция распаковки пакета
byte unpack (byte* data)
{
  pack_recived_union rec_msg;
  SERIAL_TM.println("Обработано:");
  //================================================================================//
  //                                  Иницииуруем пакет по его id                   //
  //================================================================================//
  switch (data[3])
  {
    case CTT_ID:
      {
        short_msg_union msg;
        memcpy(msg.byte_form, data, sizeof(short_msg));
        SERIAL_TM.println("CTT pack");
        SERIAL_TM.print(msg.msg_form.header_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.header_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.robot_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.data_len, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.println();
        //        SERIAL_TM.println("Отправлен ответ");
        //        pack_recived_union rec_msg = BLA_packer.pack_pdr(CTT_ID, msg.msg_form.crc);
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        break;
      }
    case TCM_ID:
      {
        short_msg_union msg;
        memcpy(msg.byte_form, data, sizeof(short_msg));
        SERIAL_TM.println("TCM pack");
        SERIAL_TM.print(msg.msg_form.header_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.header_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.robot_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.data_len, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.println();
        //        SERIAL_TM.println("Отправлен ответ");
        //        pack_recived_union
        //        rec_msg = BLA_packer.pack_pdr(TCM_ID, msg.msg_form.crc);
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        break;
      }
    case RTM_ID:
      {
        short_msg_union msg;
        memcpy(msg.byte_form, data, sizeof(short_msg));
        SERIAL_TM.println("RTM pack");
        SERIAL_TM.print(msg.msg_form.header_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.header_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.robot_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.data_len, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
        SERIAL_TM.print("\t");
        //        SERIAL_TM.println("Отправлен ответ");
        //        pack_recived_union rec_msg = BLA_packer.pack_pdr(RTM_ID, msg.msg_form.crc);
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        break;
      }
    case MCM_ID:
      {
        mcm_msg_union msg;
        memcpy(msg.byte_form, data, sizeof(mcm_msg));
        SERIAL_TM.println("MCM pack");
        SERIAL_TM.print(msg.msg_form.header_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.header_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.robot_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.data_len, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.coordinates[0], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.coordinates[1], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.coordinates[2], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.println();
        //        SERIAL_TM.println("Отправлен ответ");
        //        pack_recived_union rec_msg = BLA_packer.pack_pdr(MCM_ID, msg.msg_form.crc);
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        break;
      }
    case LRQ_ID:
      {
        short_msg_union msg;
        memcpy(msg.byte_form, data, sizeof(short_msg));
        SERIAL_TM.println("LRQ pack");
        SERIAL_TM.print(msg.msg_form.header_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.header_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.robot_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.data_len, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.println();
        //        SERIAL_TM.println("Отправлен ответ");
        //        pack_recived_union rec_msg = BLA_packer.pack_pdr(LRQ_ID, msg.msg_form.crc);
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        break;
      }
    case RTL_ID:
      {
        short_msg_union msg;
        memcpy(msg.byte_form, data, sizeof(short_msg));
        SERIAL_TM.println("RTL pack");
        SERIAL_TM.print(msg.msg_form.header_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.header_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.robot_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.data_len, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.println();
        //        SERIAL_TM.println("Отправлен ответ");
        //        pack_recived_union rec_msg = BLA_packer.pack_pdr(RTL_ID, msg.msg_form.crc);
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        break;
      }
    case CTL_ID:
      {
        short_msg_union msg;
        memcpy(msg.byte_form, data, sizeof(short_msg));
        SERIAL_TM.println("CTL pack");
        SERIAL_TM.print(msg.msg_form.header_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.header_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.robot_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.data_len, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.println();
        //        SERIAL_TM.println("Отправлен ответ");
        //        pack_recived_union rec_msg = BLA_packer.pack_pdr(CTL_ID, msg.msg_form.crc);
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        break;
      }
    case LCM_ID:
      {
        short_msg_union msg;
        memcpy(msg.byte_form, data, sizeof(short_msg));
        SERIAL_TM.println("LCM pack");
        SERIAL_TM.print(msg.msg_form.header_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.header_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.robot_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.data_len, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.println();
        //        SERIAL_TM.println("Отправлен ответ");
        //        pack_recived_union rec_msg = BLA_packer.pack_pdr(LCM_ID, msg.msg_form.crc);
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        break;
      }
    case NPM_ID:
      {
        npm_msg_union msg;
        memcpy(msg.byte_form, data, sizeof(npm_msg));
        SERIAL_TM.println("NPM pack");
        SERIAL_TM.print(msg.msg_form.header_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.header_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.robot_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.data_len, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.coordinates[0], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.coordinates[1], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.coordinates[2], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.orientation[0], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.orientation[1], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.orientation[2], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.println();
        //        SERIAL_TM.println("Отправлен ответ");
        //        pack_recived_union rec_msg = BLA_packer.pack_pdr(NPM_ID, msg.msg_form.crc);
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        break;
      }
    case TPM_ID:
      {
        tpm_msg_union msg;
        memcpy(msg.byte_form, data, sizeof(tpm_msg));
        SERIAL_TM.println("TPM pack");
        SERIAL_TM.print(msg.msg_form.header_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.header_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.robot_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.data_len, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.point_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.coordinates[0], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.coordinates[1], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.coordinates[2], 6);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.println();
        //        SERIAL_TM.println("Отправлен ответ");
        //        pack_recived_union rec_msg = BLA_packer.pack_pdr(TPM_ID, msg.msg_form.crc);
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        break;
      }
    case PDR_ID:
      {
        pack_recived_union msg;
        memcpy(msg.byte_form, data, sizeof(pack_recived));
        SERIAL_TM.println("PDR pack");
        SERIAL_TM.print(msg.msg_form.header_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.header_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.robot_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.data_len, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.recived_pack_id, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.recived_pack_crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.crc, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
        SERIAL_TM.print("\t");
        SERIAL_TM.println();
        //        SERIAL_TM.println("Отправлен ответ");
        //        pack_recived_union rec_msg = BLA_packer.pack_pdr(PDR_ID, msg.msg_form.crc);
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        //        SERIAL_BLA.write(rec_msg.byte_form, sizeof(rec_msg.byte_form));
        break;
      }
  }
  return data[3];
}


void reset_pack()
{
  status = 0;
  j = 0;
  flag = 0;
  pakage_id = 0;
  num_pakage = 0;
  packet_ready = false;
  for (int i = 0; i < (sizeof(pack_bytes)); i ++)
  {
    pack_bytes[i] = 0;
  }
}


byte find_pack ()
{
  if (SERIAL_BLA.readBytes(recive_bytes, sizeof(recive_bytes)))
  {
    SERIAL_TM.println("Прочитано:");
    for (int i = 0; i < (sizeof(recive_bytes)); i++)
    {
      SERIAL_TM.print(recive_bytes[i], HEX);
      SERIAL_TM.print("\t");
    }
    SERIAL_TM.println();
  }
  packet_ready = false;

  //разбираем полученный массив байтов
  if (!packet_ready) {
    if (i >= 50) {
      packet_ready = true;
    }
    //for (int i = 0; i < (sizeof(recive_bytes)); i++) {
    while ((status <= 8) & (i < (sizeof(recive_bytes)))) {
      //находим первый стартовый байт
      if (status == 0) {
        if (recive_bytes[i] == PACK_START_1) {
          status = 1;
          pack_bytes[j] = recive_bytes[i];
          j += 1;
        } else {
          reset_pack();
        }
      } else if (status == 1) {
        if (recive_bytes[i] == PACK_START_2) {
          status = 2;
          pack_bytes[j] = recive_bytes[i];
          j += 1;
        } else {
          reset_pack();
        }
      } else if (status == 2) {
        if ((recive_bytes[i] == NKR_ID) || (recive_bytes[i] == BLA_ID)) {
          status = 3;
          pack_bytes[j] = recive_bytes[i];
          j += 1;
        } else {
          reset_pack();
        }
      } else if (status == 3) {
        for (int k = 0; k < sizeof(id); k++) {
          if (recive_bytes[i] == id[k]) {
            //ставим метку о том, что id из списка найден
            flag += 1;
            //запоминаем id получаемого пакета
            pakage_id = recive_bytes[i];
            //запоминает номер паекта
            num_pakage = k;
            status = 4;
            pack_bytes[j] = recive_bytes[i];
            j += 1;
          }
        } if (flag == 0) {
          //id пакета не был найден
          reset_pack();
        }
        //сбрасываем значение метки
        flag = 0;
      } else if (status == 4) {
        if (recive_bytes[i] == lenght[num_pakage]) {
          status = 5;
          pack_bytes[j] = recive_bytes[i];
          j += 1;
        } else {
          reset_pack();
        }
      } else if (status == 5) {
        if (counter < (lenght[num_pakage]) + 1) {
          counter += 1;
          pack_bytes[j] = recive_bytes[i];
          j += 1;
        } else {
          status = 6;
          counter = 0;
        }
      } else if (status == 6) {
        crc_8.reset();
        for (int h = 0; h < j; h++)
        {
          crc_8.add(pack_bytes[h]);
        }
        crc = crc_8.getCRC();
        if ((recive_bytes[i - 1] == crc) || (recive_bytes[i - 1] == 0)) {
          status = 7;
          pack_bytes[j] = recive_bytes[i - 1];
          j += 1;
        } else {
          reset_pack();
        }
      } else if (status == 7) {
        if (recive_bytes[i - 1] == PACK_END_1) {
          status = 8;
          pack_bytes[j] = recive_bytes[i - 1];
          j += 1;
        } else {
          reset_pack();
        }
      } else if (status == 8) {
        if (recive_bytes[i - 1] == PACK_END_2) {
          pack_bytes[j] = recive_bytes[i - 1];
          packet_ready = true;
          status = 9;
        } else {
          reset_pack();
        }
      }
      i++;
    }
  }
  SERIAL_TM.println("Собранный пакет:");
  for (int r = 0; r <= j; r ++) {
    SERIAL_TM.print(pack_bytes[r], HEX);
    SERIAL_TM.print("\t");
  }
  SERIAL_TM.println();
  byte find_id = unpack(pack_bytes);
  packet_ready = false;
  i = 0;
  return (find_id);
  reset_pack();
}
