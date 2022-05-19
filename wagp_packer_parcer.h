#include <CRC8.h>
#include "wagp_structures.h"

CRC8 crc_8;

//Класс, в котором формируются пакеты для передачи
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

    short_msg_union pack_tnp()
    {
      return pack_short_msg(TNP_ID);
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

    tpm_msg_NKR_union pack_tpm_NKR(byte id, float coords[3], float ori[3])
    {
      tpm_msg_NKR_union msg;
      msg.msg_form.header_1 = PACK_START_1;
      msg.msg_form.header_2 = PACK_START_2;
      msg.msg_form.robot_id = robot_id;
      msg.msg_form.pack_id = TPM_ID;
      msg.msg_form.data_len = TPM_LENGTH_NKR;
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

    tpm_msg_BLA_union pack_tpm_BLA(byte id, float coords[3])
    {
      tpm_msg_BLA_union msg;
      msg.msg_form.header_1 = PACK_START_1;
      msg.msg_form.header_2 = PACK_START_2;
      msg.msg_form.robot_id = robot_id;
      msg.msg_form.pack_id = TPM_ID;
      msg.msg_form.data_len = TPM_LENGTH_BLA;
      msg.msg_form.point_id = id;
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

// Класс для разбора входящих пакетов
class unpacker
{
  private:
    byte robot_id;
    byte pack_id;
  public:

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
    };

    byte *find_pack (String device)
    {
      static byte find_id_NKR[5];
      static byte find_id_BLA[5];

      static byte lenght[11];
      //SERIAL_TM.println();

      if (device.equals("NKR")) {
        if (SERIAL_NKR.readBytes(recive_bytes, sizeof(recive_bytes)))
        {
          SERIAL_TM.println("Прочитано:" + device);
          for (int i = 0; i < (sizeof(recive_bytes)); i++)
          {
            SERIAL_TM.print(recive_bytes[i], HEX);
            SERIAL_TM.print("\t");
          }
        }
        //Сверяем длины пакетов НКР
        for (byte q = 0; q < number_of_packets; q++) {
          lenght[q] = lenght_NKR[q];
        }
      } else if (device.equals("BLA")) {
        if (SERIAL_BLA.readBytes(recive_bytes, sizeof(recive_bytes)))
        {
          SERIAL_TM.println("Прочитано:" + device);
          //for (int i = 0; i < (sizeof(recive_bytes)); i++)
          //{
          //  SERIAL_TM.print(recive_bytes[i], HEX);
          //  SERIAL_TM.print("\t");
          //}
        }
        //Сверяем длины пакетов БЛА
        for (byte q = 0; q < number_of_packets; q++) {
          lenght[q] = lenght_BLA[q];
        }
      }

      while (i < (sizeof(recive_bytes))) {
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
            //SERIAL_TM.println();                              //когда пакет собрался, выводим его
            //SERIAL_TM.println("Собранный пакет:");
            //for (int r = 0; r <= j; r ++) {
            //  SERIAL_TM.print(pack_bytes[r], HEX);
            //  SERIAL_TM.print("\t");
            //}
            //SERIAL_TM.println();
            //SERIAL_TM.print("Cчетчик i: ");
            //SERIAL_TM.println(i);
            //SERIAL_TM.print("ID пакета:");
            //SERIAL_TM.println(pack_bytes[3], HEX);

            if (device.equals("NKR")) {

              find_id_NKR[count_1_NKR] = pack_bytes[3];

              for (byte f = 0; f < 35; f++)
              {
                if (f <= j) {
                  big_array_NKR[count_1_NKR][f] = pack_bytes[f];
                } else
                {
                  big_array_NKR[count_1_NKR][f] = 0;
                }
              }

              if (count_1_NKR > 3) {
                count_1_NKR = 0;
              } else {
                count_1_NKR += 1;
              }
            } else if (device.equals("BLA"))
            {
              find_id_BLA[count_1_BLA] = pack_bytes[3];

              for (byte f = 0; f < 35; f++)
              {
                if (f <= j) {
                  big_array_BLA[count_1_BLA][f] = pack_bytes[f];
                } else
                {
                  big_array_BLA[count_1_BLA][f] = 0;
                }
              }

              if (count_1_BLA > 3) {
                count_1_BLA = 0;
              } else {
                count_1_BLA += 1;
              }
            }
            SERIAL_TM.print("\t");
            SERIAL_TM.println(pack_bytes[3], HEX);
            unpack(pack_bytes);
            i -= 1;
            status = 0;
            reset_pack();
          } else {
            reset_pack();
          }
        }
        i++;
      }
      i = 0;

      for (int k = 0; k < (sizeof(recive_bytes)); k ++)
      {
        recive_bytes[k] = 0;
      }
      if (device.equals("NKR")) {
        return find_id_NKR;
      } else if (device.equals("BLA")) {
        return find_id_BLA;
      }
    };


    //функция распаковки пакета
    byte unpack (byte* data)
    {
      pack_recived_union rec_msg;
      //      SERIAL_TM.println("Обработано:");
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
            break;
          }
        case TPM_ID:
          {
            tpm_msg_BLA_union msg;
            memcpy(msg.byte_form, data, sizeof(tpm_msg_BLA));
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
            //SERIAL_TM.print(msg.msg_form.orient[0], 6);
            //SERIAL_TM.print("\t");
            //SERIAL_TM.print(msg.msg_form.orient[1], 6);
            //SERIAL_TM.print("\t");
            //SERIAL_TM.print(msg.msg_form.orient[2], 6);
            //SERIAL_TM.print("\t");
            SERIAL_TM.print(msg.msg_form.crc, HEX);
            SERIAL_TM.print("\t");
            SERIAL_TM.print(msg.msg_form.terminator_1, HEX);
            SERIAL_TM.print("\t");
            SERIAL_TM.print(msg.msg_form.terminator_2, HEX);
            SERIAL_TM.print("\t");
            SERIAL_TM.println();
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
            break;
          }
      }
      return data[3];
    };

    byte waiting_pack_id (byte* data)
    {
      //Функция возвращающая Id пакета отклика
      return data[6];
    };

    //Функция ожидания определенного пакета от БЛА
    bool BLA_take_id(byte pack_id)
    {
      //static byte count_step = 6;
      exist = false;
      //Получили массив id  пришедших пакетов в массив array_BLA
      byte* array_BLA_id = find_pack("BLA");
      for (byte s = 0; s < 5; s++)
      {
        if (array_BLA_id[s] == pack_id)
        {
          exist = true;
          //SERIAL_TM.println();
          //SERIAL_TM.println("array_BLA[s] = " + String(array_BLA[s], HEX));
          array_BLA_id[s] = 0;
          //      SERIAL_TM.println("array_BLA[s] = " + String(array_BLA[s], HEX));
          for (byte g = 0; g < 35; g++)
          {
            big_array_BLA[s][g] = 0;
          }
          break;
        }
      }
      return exist;
    };

    //Функция ожидания определенного пакета от БЛА c id пакета
    bool BLA_take_id(byte pack_id, byte wait_pack_id)
    {
      //static byte count_step = 6;
      exist = false;
      //Получили массив id  пришедших пакетов в массив array_BLA
      byte* array_BLA_id = find_pack("BLA");

      for (byte s = 0; s < 5; s++)
      {
        if (array_BLA_id[s] == pack_id)
        {
          //Если мы ищем пакет отклика, то проверяем еще и его содержимое
          //проверяем, совпадают ли id ожидаемого пакета
          if (pack_id == PDR_ID)
          {
            byte arr[35];
            for (byte v = 0; v < 35; v++)
            {
              arr[v] = big_array_BLA[s][v];
            }
            byte wait_id = waiting_pack_id(arr);
            if (wait_id == wait_pack_id)
            {
              exist = true;
            } else
            {
              exist = false;
            }
          } else
          {
            exist = true;
          }
          array_BLA_id[s] = 0;
          for (byte g = 0; g < 35; g++)
          {
            big_array_BLA[s][g] = 0;
          }
          break;
        }
      }
      return exist;
    };


    void take_packs()
    {
      //Получили массив id  пришедших пакетов в массив array_NKR
      byte* array_NKR = find_pack("NKR");
    };

};


class wagp_sender
{
  private:
    byte robot_id;
    byte pack_id;
  public:
    //Функция для отправки пакета на БЛА
    void send_to_BLA(byte* pack_to_send)
    {
      //По id узнаем, что за пакет мы будем отправлять на БЛА
      switch (pack_to_send[3])
      {
        case CTT_ID:
          {
            //Формируем байтовое представление полученного пакета и отпраляем на БЛА (+отладчик)
            short_msg_union msg;
            memcpy(msg.byte_form, pack_to_send, sizeof(short_msg));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));

            break;
          }
        case TCM_ID:
          {
            short_msg_union msg;
            memcpy(msg.byte_form, pack_to_send, sizeof(short_msg));

            break;
          }
        case RTM_ID:
          {
            short_msg_union msg;
            memcpy(msg.byte_form, pack_to_send, sizeof(short_msg));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));

            break;
          }
        case MCM_ID:
          {
            mcm_msg_union msg;
            memcpy(msg.byte_form, pack_to_send, sizeof(mcm_msg));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));

            break;
          }
        case LRQ_ID:
          {
            short_msg_union msg;
            memcpy(msg.byte_form, pack_to_send, sizeof(short_msg));

            break;
          }
        case RTL_ID:
          {
            short_msg_union msg;
            memcpy(msg.byte_form, pack_to_send, sizeof(short_msg));

            break;
          }
        case CTL_ID:
          {
            short_msg_union msg;
            memcpy(msg.byte_form, pack_to_send, sizeof(short_msg));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
            
            break;
          }
        case LCM_ID:
          {
            short_msg_union msg;
            memcpy(msg.byte_form, pack_to_send, sizeof(short_msg));

            break;
          }
        case NPM_ID:
          {
            npm_msg_union msg;
            memcpy(msg.byte_form, pack_to_send, sizeof(npm_msg));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
            //SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
            break;
          }
        case TPM_ID:
          {
            tpm_msg_BLA_union msg;
            memcpy(msg.byte_form, pack_to_send, sizeof(tpm_msg_BLA));
            msg.msg_form.data_len = 14;
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
            //SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));
            break;
          }
        case PDR_ID:
          {
            pack_recived_union msg;
            memcpy(msg.byte_form, pack_to_send, sizeof(pack_recived));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
            SERIAL_BLA.write(msg.byte_form, sizeof(msg.byte_form));
            break;
          }
      }

    };

};
