#include <CRC8.h>

#include "wagp_structures.h"

CRC8 crc_8;

/*class wagp_parcer
  {
    //     Statuses:
    //     0: 'Searching for pack start',
    //     1: 'Found first starter byte',
    //     2: 'Pack start found getting robot ID',
    //     3: 'Got robot ID gettting pack ID',
    //     4: 'Got pack ID gettintg payload',
    //     5: 'Got payload getting and checking CRC8',
    //     6: 'Checking packet end',
    //     7: 'Found first end byte seraching second and parsing payload'

  private:
    byte robot_id;
    byte pack_id;
  public:
    wagp_parcer(String robot = "BLA")
    {
      if (robot == "NKR") {
        robot_id = NKR_ID;
      } else if (robot == "BLA") {
        robot_id = BLA_ID;
      }
    };

    discardinbuff(byte recive_bytes[])
    {
      for (int i = 0; i < (sizeof(recive_bytes)); i++) {
        recive_bytes[i] = 0;
      }
    };

    //==========================================================================//
    //                        Функция cброса накопленных значений               //
    //==========================================================================//
    reset_pack()
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
    };

    //==========================================================================//
    // Функция получения последовательности байтов паекта протокола общения     //
    //==========================================================================//

    byte find_pack(byte recive_bytes[])
    {
      //получаем массив байтов
      if (Serial1.readBytes(recive_bytes, sizeof(recive_bytes)))
      {
        Serial.println("Прочитано:");
        for (int i = 0; i < (sizeof(recive_bytes)); i++)
        {
          Serial.print(recive_bytes[i], HEX);
          Serial.print("\t");
        }
        Serial.println();
        packet_ready = false;
        //разбираем полученный массив байтов
        while (!packet_ready) {
          Serial.println(i);
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
              if (recive_bytes[i - 1] == crc) {
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
                tpm_msg_union msg;
                memcpy(msg.byte_form, pack_bytes, sizeof(tpm_msg));
                return msg.msg_form.header_1;
                status = 9;
              } else {
                reset_pack();
              }
            }
            i++;
          }
        }
        //        Serial.println("Собранный пакет:");
        //        for (int r = 0; r <= j; r ++) {
        //          Serial.print(pack_bytes[r], HEX);
        //          Serial.print("\t");
        //        }
        //        Serial.println();
        //        switch (pack_bytes[3])
        //        {
        //          case CTT_ID:
        //            {
        //              return ("CTT pack");
        //              break;
        //            }
        //          case TCM_ID:
        //            {
        //              return ("TCM pack");
        //              break;
        //            }
        //          case RTM_ID:
        //            {
        //              return ("RTM pack");
        //              break;
        //            }
        //          case MCM_ID:
        //            {
        //              return (MCM_ID);
        //              //return ("MCM pack");
        //              break;
        //            }
        //          case TPM_ID:
        //            {
        //              return ("TPM pack");
        //              tpm_msg_union msg;
        //              memcpy(msg.byte_form, data, sizeof(tpm_msg));
        //
        //              Serial.print(msg.msg_form.coordinates[0]);
        //              Serial.print("\t");
        //              Serial.print(msg.msg_form.coordinates[1]);
        //              Serial.print("\t");
        //              Serial.println(msg.msg_form.coordinates[2]);
        //              break;
        //            }
        //        }
        //return (unpack(pack_bytes));
        packet_ready = false;
        i = 0;
        //        tpm_msg_union msg;
        //        memcpy(msg.byte_form, pack_bytes, sizeof(tpm_msg));
        //        return msg.msg_form.header_1;
        reset_pack();
      }

    };

    //==========================================================================//
    //         Функция распаковки и идентификации полученного пакета            //
    //==========================================================================//
    unpack (byte* data)
    {
      Serial.println("На входе функции_2:");
      for (int i = 0; i < (sizeof(tpm_msg_union)); i++) {
        Serial.print(data[i], HEX);
        Serial.print("\t");
      }
      Serial.println();

      switch (data[3])
      {
        case CTT_ID:
          {
            return ("CTT pack");
            break;
          }
        case TCM_ID:
          {
            return ("TCM pack");
            break;
          }
        case RTM_ID:
          {
            return ("RTM pack");
            break;
          }
        case MCM_ID:
          {
            return ("MCM pack");
            break;
          }
        case TPM_ID:
          {
            return ("TPM pack");
            tpm_msg_union msg;
            memcpy(msg.byte_form, data, sizeof(tpm_msg));

            Serial.print(msg.msg_form.coordinates[0]);
            Serial.print("\t");
            Serial.print(msg.msg_form.coordinates[1]);
            Serial.print("\t");
            Serial.println(msg.msg_form.coordinates[2]);
            break;
          }
      }

      mcm_msg_union msg;
      memcpy(msg.byte_form, data, sizeof(mcm_msg));

      return (data[3]);
    };

  };*/


class wagp_packer
{
  private:
    byte robot_id;
    byte pack_id;
  public:
    wagp_packer(String robot = "NKR")
    {
      if (robot == "NKR") {
        robot_id = NKR_ID;
      } else if (robot == "BLA") {
        robot_id = BLA_ID;
      }
    };

    short_msg_union pack_short_msg(byte pack_id = CTT_ID)
    {
      short_msg_union msg;
      msg.msg_form.header_1 = PACK_START_1;
      msg.msg_form.header_2 = PACK_START_2;
      msg.msg_form.robot_id = robot_id;
      msg.msg_form.pack_id = pack_id;
      msg.msg_form.data_len = 0;
      crc_8.reset();
      for (int i = 0; i < sizeof(msg.byte_form) - 3; i++)
      {
        crc_8.add(msg.byte_form[i]);
      }
      msg.msg_form.crc = crc_8.getCRC();
      msg.msg_form.terminator_1 = PACK_END_1;
      msg.msg_form.terminator_2 = PACK_END_2;
      return msg;
    };

    short_msg_union pack_ctt()
    {
      return pack_short_msg(CTT_ID);
    };

    short_msg_union pack_tcm()
    {
      return pack_short_msg(TCM_ID);
    };

    short_msg_union pack_rtm()
    {
      return pack_short_msg(RTM_ID);
    };

    short_msg_union pack_lrq()
    {
      return pack_short_msg(LRQ_ID);
    };

    short_msg_union pack_rtl()
    {
      return pack_short_msg(RTL_ID);
    };

    short_msg_union pack_ctl()
    {
      return pack_short_msg(CTL_ID);
    };

    short_msg_union pack_lcm()
    {
      return pack_short_msg(LCM_ID);
    };

    mcm_msg_union pack_mcm(float coords[3])
    {
      mcm_msg_union msg;
      msg.msg_form.header_1 = PACK_START_1;
      msg.msg_form.header_2 = PACK_START_2;
      msg.msg_form.robot_id = robot_id;
      msg.msg_form.pack_id = MCM_ID;
      msg.msg_form.data_len = MCM_LENGTH;
      msg.msg_form.coordinates[0] = coords[0];
      msg.msg_form.coordinates[1] = coords[1];
      msg.msg_form.coordinates[2] = coords[2];
      crc_8.reset();
      for (int i = 0; i < sizeof(msg.byte_form) - 3; i++)
      {
        crc_8.add(msg.byte_form[i]);
      }
      msg.msg_form.crc = crc_8.getCRC();
      msg.msg_form.terminator_1 = PACK_END_1;
      msg.msg_form.terminator_2 = PACK_END_2;
      return msg;
    };

     npm_msg_union pack_npm(float coords[3], float orientation[3])
    {
      npm_msg_union msg;
      msg.msg_form.header_1 = PACK_START_1;
      msg.msg_form.header_2 = PACK_START_2;
      msg.msg_form.robot_id = robot_id;
      msg.msg_form.pack_id = NPM_ID;
      msg.msg_form.data_len = NPM_LENGTH;
      msg.msg_form.coordinates[0] = coords[0];
      msg.msg_form.coordinates[1] = coords[1];
      msg.msg_form.coordinates[2] = coords[2];
      msg.msg_form.orientation[0] = orientation[0];
      msg.msg_form.orientation[1] = orientation[1];
      msg.msg_form.orientation[2] = orientation[2];
      crc_8.reset();
      for (int i = 0; i < sizeof(msg.byte_form) - 3; i++)
      {
        crc_8.add(msg.byte_form[i]);
      }
      msg.msg_form.crc = crc_8.getCRC();
      msg.msg_form.terminator_1 = PACK_END_1;
      msg.msg_form.terminator_2 = PACK_END_2;
      return msg;
    };

    tpm_msg_union pack_tpm(byte id, float coords[3], float ori[3])
    {
      tpm_msg_union msg;
      msg.msg_form.header_1 = PACK_START_1;
      msg.msg_form.header_2 = PACK_START_2;
      msg.msg_form.robot_id = robot_id;
      msg.msg_form.pack_id = TPM_ID;
      msg.msg_form.data_len = TPM_LENGTH;
      msg.msg_form.point_id = id;
      msg.msg_form.coordinates[0] = coords[0];
      msg.msg_form.coordinates[1] = coords[1];
      msg.msg_form.coordinates[2] = coords[2];
      msg.msg_form.orient[0] = ori[0];
      msg.msg_form.orient[1] = ori[1];
      msg.msg_form.orient[2] = ori[2];
      crc_8.reset();
      for (int i = 0; i < sizeof(msg.byte_form) - 3; i++)
      {
        crc_8.add(msg.byte_form[i]);
      }
      msg.msg_form.crc = crc_8.getCRC();
      msg.msg_form.terminator_1 = PACK_END_1;
      msg.msg_form.terminator_2 = PACK_END_2;
      return msg;
    };

    pack_recived_union pack_pdr(byte rec_pack_id, byte rec_pack_crc)
    {
      pack_recived_union msg;
      msg.msg_form.header_1 = PACK_START_1;
      msg.msg_form.header_2 = PACK_START_2;
      msg.msg_form.robot_id = robot_id;
      msg.msg_form.pack_id = PDR_ID;
      msg.msg_form.data_len = SHORT_MSG_LEN;
      msg.msg_form.recived_pack_id = rec_pack_id;
      msg.msg_form.recived_pack_crc = rec_pack_crc;
      crc_8.reset();
      for (int i = 0; i < sizeof(msg.byte_form) - 3; i++)
      {
        crc_8.add(msg.byte_form[i]);
      }
      msg.msg_form.crc = crc_8.getCRC();
      msg.msg_form.terminator_1 = PACK_END_1;
      msg.msg_form.terminator_2 = PACK_END_2;
      return msg;
    };
};
