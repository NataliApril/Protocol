
#include "wagp_packer_parcer.h"

wagp_packer BLA_packer("BLA");

unpacker BLA_unpacker;

wagp_sender BLA_sender;

void setup()
{
  SERIAL_TM.begin(115200);
  SERIAL_BLA.begin(9600);
  SERIAL_NKR.begin(19200);
}

void loop()
{
  //BLA_unpacker.take_packs();
  NKR_send();
  BLA_send();
  take_NPM_pack();
}


void NKR_send(void)
{
  //Индикатор пакета
  byte action_NKR = 0;

  //Получили массив id  пришедших пакетов в массив array_NKR
  byte* array_NKR = BLA_unpacker.find_pack("NKR");

  if (f_NKR < 5)
  {
    //Ищем ненулевой элемент массива
    if (array_NKR[f_NKR] != 0) {
      action_NKR = array_NKR[f_NKR];
      //      SERIAL_TM.println("action" + action);
      for (byte r = 0; r < 35; r++)
      {
        packet_sender_NKR[r] = big_array_NKR[f_NKR][r];
        //SERIAL_TM.print(packet_sender[r], HEX);
        //SERIAL_TM.print("\t");
      }
      //SERIAL_TM.println();
    }
    else {}
    //Если мы нашли таковой, то отправляем его на "отработку" с помощью action
    //В массивк big_array_NKR хранится полученный пакет полностью, на месте с id = f
    //
    //    SERIAL_TM.println("waiting NKR =" + String(waiting_action_NKR, HEX));
    //    SERIAL_TM.println("action NKR =" + String(action_NKR, HEX));

    switch (action_NKR)
    {
      case (CTT_ID):          //"Взлет разрешен"
        {
          if (waiting_action_NKR == action_NKR)
          {
            crc_pack = packet_sender_NKR[6];
            //SERIAL_BLA.println("crc_pack" + crc_pack);
            //Ждет ответа о получении
            while (!BLA_unpacker.BLA_take_id(PDR_ID, CTT_ID))
            {
              //Отправляем пакет на БЛА
              BLA_sender.send_to_BLA(packet_sender_NKR);
              //SERIAL_TM.write(packet_sender, sizeof(packet_sender));
            }

            //Отправляем отклик о получении на НКР

            pack_recived_union msg = BLA_packer.pack_pdr(CTT_ID, crc_pack);
            SERIAL_NKR.write(msg.byte_form, sizeof(msg.byte_form));
            //SERIAL_TM.println("PRC отправлен на НКР");

            //Запоминаем одижаемое действие от БЛА
            waiting_action_BLA = TCM_ID;
            //Запоминаем одижаемое действие от НКР
            waiting_action_NKR = MCM_ID;
            //Поле отработки case с нужным id, занулем его в массиве
            //чтобы запомнить, что он выполнен
            array_NKR[f_NKR] = 0;
            //Отчищаем строку массива, с обработанным пакетом
            for (byte t = 0; t < 35; t++) {
              big_array_NKR[f_NKR][t] = 0;
            }
          }
          else {}
          break;
        }

      case (TCM_ID):          //"Взлет завершен"
        {
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_NKR[f_NKR] = 0;
          break;
        }
      case (RTM_ID):          //"Начало движения"
        {
          if (waiting_action_NKR == action_NKR)
          {
            crc_pack = packet_sender_NKR[6];

            while (!BLA_unpacker.BLA_take_id(PDR_ID, RTM_ID))
            {
              //Отправляем пакет на БЛА
              BLA_sender.send_to_BLA(packet_sender_NKR);
            }
            //SERIAL_TM.println("Отправлен RTM на БЛА");

            //Отправляем отклик о получении на НКР
            pack_recived_union msg = BLA_packer.pack_pdr(RTM_ID, crc_pack);
            SERIAL_NKR.write(msg.byte_form, sizeof(msg.byte_form));
            //SERIAL_TM.write(msg.byte_form, sizeof(msg.byte_form));

            //Запоминаем одижаемое действие от НКР
            waiting_action_NKR = MCM_ID;

            //Поcле отработки case с нужным id, занулем его в массиве
            //чтобы запомнить, что он выполнен
            array_NKR[f_NKR] = 0;
            //Отчищаем строку массива, с обработанным пакетом
            for (byte t = 0; t < 35; t++) {
              big_array_NKR[f_NKR][t] = 0;
            }
          } else {}
          break;
        }
      case (MCM_ID):                                    //"Осмотр завершен"
        {
          if (waiting_action_NKR == action_NKR)
          {
            crc_pack = packet_sender_NKR[18];
            while (!BLA_unpacker.BLA_take_id(PDR_ID, MCM_ID))
            {
              //Отправляем пакет на БЛА
              BLA_sender.send_to_BLA(packet_sender_NKR);
              //SERIAL_TM.println("Отправляю МСМ");
            }
            //Запоминаем одижаемое действие от БЛА
            waiting_action_NKR = RTM_ID;

            pack_recived_union msg = BLA_packer.pack_pdr(MCM_ID, crc_pack);
            SERIAL_NKR.write(msg.byte_form, sizeof(msg.byte_form));

            //Поле отработки case с нужным id, занулем его в массиве
            //чтобы запомнить, что он выполнен
            array_NKR[f_NKR] = 0;
            //Отчищаем строку массива, с обработанным пакетом
            for (byte t = 0; t < 35; t++) {
              big_array_NKR[f_NKR][t] = 0;
            }
          } else {}
          break;
        }
      case (LRQ_ID):                                                  //"Запрос на посадку"
        {
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_NKR[f_NKR] = 0;
          break;
        }
      case (RTL_ID):          //"Готов к посадке"
        {
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_NKR[f_NKR] = 0;
          break;
        }
      case (CTL_ID):          //"Посадка разрешена"
        {
          if (waiting_action_NKR == action_NKR)
          {
            crc_pack = packet_sender_NKR[6];
            while (!BLA_unpacker.BLA_take_id(PDR_ID, CTL_ID))
            {
              //Отправляем пакет на БЛА
              BLA_sender.send_to_BLA(packet_sender_NKR);
            }
            //Запоминаем ожидаемое действие БЛА
            waiting_action_BLA == LCM_ID;

            //Поле отработки case с нужным id, занулем его в массиве
            //чтобы запомнить, что он выполнен
            array_NKR[f_NKR] = 0;
            //Отправляем отклик о получении на НКР
            pack_recived_union msg = BLA_packer.pack_pdr(CTL_ID, crc_pack);
            SERIAL_NKR.write(msg.byte_form, sizeof(msg.byte_form));
            //Отчищаем строку массива, с обработанным пакетом
            for (byte t = 0; t < 35; t++) {
              big_array_NKR[f_NKR][t] = 0;
            }
          } else {}
          break;
        }

      case (LCM_ID):          //"Посадка завершена"
        {
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_NKR[f_NKR] = 0;
        }

      case (NPM_ID):          //"Параметры навигации"
        {
          crc_pack = packet_sender_NKR[30];
          //flag_NPM = true;
          while (!BLA_unpacker.BLA_take_id(PDR_ID, NPM_ID))
          {
            BLA_sender.send_to_BLA(packet_sender_NKR);
          }

          pack_recived_union msg = BLA_packer.pack_pdr(NPM_ID, crc_pack);
          SERIAL_NKR.write(msg.byte_form, sizeof(msg.byte_form));

          flag_NPM = false;
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_NKR[f_NKR] = 0;
          //Отчищаем строку массива, с обработанным пакетом
          for (byte t = 0; t < 35; t++) {
            big_array_NKR[f_NKR][t] = 0;
          }
          break;
        }

      case (TPM_ID):          //"Параметры траектории"
        {
          crc_pack = packet_sender_NKR[32];
          //SERIAL_TM.println("crc_pack" + crc_pack);
          tpm_count = 0;
          //Формируем из пришедшего пакета пакет с точкой интереса
          //и пакет с координатой
          if (count_tpm_num == 0)
          {
            for (byte d = 0; d < 35; d++)
            {
              //SERIAL_TM.print(packet_sender[d], HEX);
              //SERIAL_TM.print("\t");
              //Записываем только ориентацию - точку интереса
              if ((d > 19) & (d < 32))
              {
                orient_tpm[tpm_ori_count] = packet_sender_NKR[d];
                //SERIAL_TM.println("pack byte = " + String(packet_sender[d]) +
                //" " + "orient_tpm = " + String(orient_tpm[tpm_ori_count]));
                //SERIAL_TM.print("id byte = " + String(d)); SERIAL_TM.print("\t");
                //SERIAL_TM.print(orient_tpm[tpm_ori_count], HEX); SERIAL_TM.println();
                tpm_ori_count ++;
              }
              //Записываем только координаты путевой точки
              else if ((d > 7) & (d < 20))
              {
                coord_tpm[tpm_coord_count] = packet_sender_NKR[d];
                //SERIAL_TM.print("id byte = " + String(d)); SERIAL_TM.print("\t");
                //SERIAL_TM.print(coord_tpm[tpm_coord_count], HEX); SERIAL_TM.println();
                tpm_coord_count ++;
              }
              else
              {
                //на позициях, где должны быть id точки и
                //координаты/ориентация пропускаем
                if (tpm_count == 7)
                {
                  tpm_count += 12;
                }
                tpm_pack_BLA[tpm_count] = packet_sender_NKR[d];
                //SERIAL_TM.print("id byte = " + String(d)); SERIAL_TM.print("\t");
                //SERIAL_TM.print(tpm_pack_BLA[tpm_count], HEX); SERIAL_TM.println();
                tpm_count ++;
              }
            }
            //SERIAL_TM.println();
            //Собираем пакет с точкой интереса (ориентация)
            tpm_pack_BLA[6] = 0;
            tpm_pack_BLA[7] = 0;
            for (byte d = 0; d < 12; d++)
            {
              tpm_pack_BLA[d + 8] = orient_tpm[d];
              //SERIAL_TM.print(tpm_pack_BLA[d+8]); SERIAL_TM.print("\t");
            }
            //for (byte d = 0; d < 35; d++)
            //{
            //  SERIAL_TM.print(tpm_pack_BLA[d], HEX); SERIAL_TM.print("\t");
            //}
            //SERIAL_TM.println();

            while (!BLA_unpacker.BLA_take_id(PDR_ID, TPM_ID))
            {
              //Отправляем пакет на БЛА
              BLA_sender.send_to_BLA(tpm_pack_BLA);
              //SERIAL_TM.println("Отправляю пакет на БЛА");
              //for (byte c = 0; c < 35; c++) {
              //  SERIAL_TM.print(tpm_pack_BLA[c], HEX);
              //  SERIAL_TM.print("\t");
              //}
              //SERIAL_TM.println();
            }

            //Собираем пакет с путевой точкой
            tpm_pack_BLA[6] = 1;
            tpm_pack_BLA[7] = 0;
            for (byte d = 0; d < 12; d++)
            {
              tpm_pack_BLA[d + 8] = coord_tpm[d];
            }
            while (!BLA_unpacker.BLA_take_id(PDR_ID, TPM_ID))
            {
              //Отправляем пакет на БЛА
              BLA_sender.send_to_BLA(tpm_pack_BLA);
              //SERIAL_TM.println("Отправляю пакет на БЛА");
              //for (byte c = 0; c < 35; c++) {
              //  SERIAL_TM.print(tpm_pack_BLA[c], HEX);
              //  SERIAL_TM.print("\t");
              //}
              //SERIAL_TM.println();
            }
          }
          else
          {
            //SERIAL_TM.println("Передаю на БЛА");
            for (byte d = 0; d < 35; d++)
            {
              //SERIAL_TM.print(packet_sender[d], HEX);
              //SERIAL_TM.print("\t");
              //Записываем в пакет для передачи только координаты
              if ((d < 20) || (d > 31))
              {
                tpm_pack_BLA[tpm_count] = packet_sender_NKR[d];
                tpm_count ++;
              }
            }
            //SERIAL_TM.println();

            //Ждет ответа о получении
            while (!BLA_unpacker.BLA_take_id(PDR_ID, TPM_ID))
            {
              //Отправляем пакет на БЛА
              BLA_sender.send_to_BLA(tpm_pack_BLA);
              //SERIAL_TM.println("Отправляю пакет на БЛА");
              //for (byte c = 0; c < 35; c++) {
              //  SERIAL_TM.print(tpm_pack_BLA[c], HEX);
              //  SERIAL_TM.print("\t");
              //}
              //SERIAL_TM.println();
            }
          }
          count_tpm_num += 1;

          //Ожидаемый пакет от НКР
          waiting_action_NKR = CTT_ID;

          //Отправляем отклик о получении на НКР
          pack_recived_union msg = BLA_packer.pack_pdr(TPM_ID, crc_pack);
          SERIAL_NKR.write(msg.byte_form, sizeof(msg.byte_form));
          //SERIAL_TM.println("PRC отправлен на НКР");

          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_NKR[f_NKR] = 0;
          //Отчищаем строку массива, с обработанным пакетом
          for (byte t = 0; t < 35; t++) {
            big_array_NKR[f_NKR][t] = 0;
            tpm_pack_BLA[t] = 0;
          }
          break;
        }

      case (PDR_ID):          //"Отклик получения пакета"
        {
          //Проверяю, не пришел ли отклик от НКР на пакет RTM_ID
          //отправленный от БЛА
          //Если отклик пришел, то его нужно отправтить на БЛА
          if (BLA_unpacker.waiting_pack_id(packet_sender_NKR) == RTM_ID)
          {
            while (!BLA_unpacker.BLA_take_id(PDR_ID, PDR_ID))
            {
              BLA_sender.send_to_BLA(packet_sender_NKR);
            }
          } else {}
          
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_NKR[f_NKR] = 0;
          //Отчищаем строку массива, с обработанным пакетом
          for (byte t = 0; t < 35; t++) {
            big_array_NKR[f_NKR][t] = 0;
          }
          break;
        }

      default:
        {
          //do nothing
          break;
        }
    }
    //Добавляем к счетчику
    f_NKR++;
    //Приводим к id кольцевого массива
    if (f_NKR > 4)
    {
      f_NKR = 0;
    }
  }
}

void BLA_send()
{
  //Индикатор пакета
  byte action_BLA = 0;

  //Получили массив id  пришедших пакетов в массив array_NKR
  byte* array_BLA = BLA_unpacker.find_pack("BLA");

  if (f_BLA < 5)
  {
    //    SERIAL_TM.println("action BLA_2 =" + String(action_BLA));
    //Ищем ненулевой элемент массива
    if (array_BLA[f_BLA] != 0) {
      action_BLA = array_BLA[f_BLA];
      //      SERIAL_TM.println("action BLA_2 = " + String(action_BLA, HEX));
      //SERIAL_TM.println("action" + action);
      for (byte r = 0; r < 35; r++)
      {
        packet_sender_BLA[r] = big_array_BLA[f_BLA][r];
        //SERIAL_TM.print(packet_sender[r], HEX);
        //SERIAL_TM.print("\t");
      }
      //SERIAL_TM.println();
    }
    else {}

    //    SERIAL_TM.println("waiting BLA =" + String(waiting_action_BLA, HEX));
    //    SERIAL_TM.println("action BLA =" + String(action_BLA, HEX));

    switch (action_BLA)
    {
      case (CTT_ID):
        {
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_BLA[f_BLA] = 0;
          //Отчищаем строку массива, с обработанным пакетом
          for (byte t = 0; t < 35; t++) {
            big_array_BLA[f_BLA][t] = 0;
          }
          break;
        }

      case (TCM_ID):
        {
          if (waiting_action_BLA == action_BLA)
          {
            //SERIAL_TM.println("Получен Взлет завершен");
            pack_recived_union msg_BLA = BLA_packer.pack_pdr(TCM_ID, 0);                 //отправляю "пакет получен"
            SERIAL_BLA.write(msg_BLA.byte_form, sizeof(msg_BLA.byte_form));
            SERIAL_BLA.write(msg_BLA.byte_form, sizeof(msg_BLA.byte_form));
            //SERIAL_TM.write(msg_BLA.byte_form, sizeof(msg_BLA.byte_form));

            short_msg_union msg_NKR = BLA_packer.pack_short_msg(TCM_ID);
            SERIAL_NKR.write(msg_NKR.byte_form, sizeof(msg_NKR.byte_form));
            //            SERIAL_TM.println("Отправлен ТСМ на НКР");
            //            SERIAL_NKR.write(msg_NKR.byte_form, sizeof(msg_NKR.byte_form));

            //Следующий ожидаемый пакет от БЛА
            waiting_action_BLA = MCM_ID;
            //Следующий ожидаемый пакет от НКР
            waiting_action_NKR = MCM_ID;

            //Поле отработки case с нужным id, занулем его в массиве
            //чтобы запомнить, что он выполнен
            array_BLA[f_BLA] = 0;
            //Отчищаем строку массива, с обработанным пакетом
            for (byte t = 0; t < 35; t++) {
              big_array_BLA[f_BLA][t] = 0;
            }
          } else {}

          break;
        }

      case (RTM_ID):
        {
          if (waiting_action_BLA == action_BLA)
          {
            pack_recived_union msg_BLA = BLA_packer.pack_pdr(RTM_ID, 0);                 //отправляю "пакет получен"
            SERIAL_BLA.write(msg_BLA.byte_form, sizeof(msg_BLA.byte_form));
            SERIAL_BLA.write(msg_BLA.byte_form, sizeof(msg_BLA.byte_form));

            short_msg_union msg_NKR = BLA_packer.pack_rtm();
            SERIAL_NKR.write(msg_NKR.byte_form, sizeof(msg_NKR.byte_form));
            SERIAL_TM.write(msg_NKR.byte_form, sizeof(msg_NKR.byte_form));

            //Запоминаем ожидаемое действие БЛА
            waiting_action_BLA = MCM_ID;

            //Ожидаем отклика  о получении RTM от БЛА на НКР
            //waiting_action_BLA = PDR_ID;

            //Поле отработки case с нужным id, занулем его в массиве
            //чтобы запомнить, что он выполнен
            array_BLA[f_BLA] = 0;
            //Отчищаем строку массива, с обработанным пакетом
            for (byte t = 0; t < 35; t++) {
              big_array_BLA[f_BLA][t] = 0;
            }
          } else {}
          break;
        }

      case (MCM_ID):
        {
          if (waiting_action_BLA == action_BLA)
          {
            //SERIAL_TM.println("Получен Осмотр завершен");
            pack_recived_union msg_BLA_2 = BLA_packer.pack_pdr(MCM_ID, 0);                 //отправляю "пакет получен"
            SERIAL_BLA.write(msg_BLA_2.byte_form, sizeof(msg_BLA_2.byte_form));
            //SERIAL_BLA.write(msg_BLA_2.byte_form, sizeof(msg_BLA_2.byte_form));
            //SERIAL_TM.write(msg_BLA_2.byte_form, sizeof(msg_BLA_2.byte_form));

            mcm_msg_union msg_NKR_2 = BLA_packer.pack_mcm(coords);
            SERIAL_NKR.write(msg_NKR_2.byte_form, sizeof(msg_NKR_2.byte_form));
            //            SERIAL_TM.println("Отправлен МСМ на НКР");
            //            SERIAL_NKR.write(msg_NKR_2.byte_form, sizeof(msg_NKR_2.byte_form));

            //Следующий ожидаемый пакет от БЛА
            waiting_action_BLA = RTM_ID;

            //Поле отработки case с нужным id, занулем его в массиве
            //чтобы запомнить, что он выполнен
            array_BLA[f_BLA] = 0;
            //Отчищаем строку массива, с обработанным пакетом
            for (byte t = 0; t < 35; t++) {
              big_array_BLA[f_BLA][t] = 0;
            }
          } else {}

          break;
        }

      case (LRQ_ID):
        {
          pack_recived_union msg_BLA_2 = BLA_packer.pack_pdr(LRQ_ID, 0);                 //отправляю "пакет получен"
          SERIAL_BLA.write(msg_BLA_2.byte_form, sizeof(msg_BLA_2.byte_form));

          short_msg_union msg = BLA_packer.pack_lrq();
          SERIAL_NKR.write(msg.byte_form, sizeof(msg.byte_form));

          //Запоминаем ожидаемое действие от НКР
          waiting_action_NKR = CTL_ID;
          //Запоминаем ожидаемое действие от БЛА
          waiting_action_BLA = LCM_ID;
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_BLA[f_BLA] = 0;
          //Отчищаем строку массива, с обработанным пакетом
          for (byte t = 0; t < 35; t++) {
            big_array_BLA[f_BLA][t] = 0;
          }
          break;
        }

      case (LCM_ID):
        {
          if (waiting_action_BLA == action_BLA)
          {
            pack_recived_union msg_BLA_2 = BLA_packer.pack_pdr(LCM_ID, 0);                 //отправляю "пакет получен"
            SERIAL_BLA.write(msg_BLA_2.byte_form, sizeof(msg_BLA_2.byte_form));

            short_msg_union msg_NKR = BLA_packer.pack_lcm();
            SERIAL_NKR.write(msg_NKR.byte_form, sizeof(msg_NKR.byte_form));

            waiting_action_BLA == 0;
            waiting_action_NKR == 0;

            //Поле отработки case с нужным id, занулем его в массиве
            //чтобы запомнить, что он выполнен
            array_BLA[f_BLA] = 0;
            //Отчищаем строку массива, с обработанным пакетом
            for (byte t = 0; t < 35; t++) {
              big_array_BLA[f_BLA][t] = 0;
            }
          } else {}
          break;
        }

      case (RTL_ID):
        {
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_BLA[f_BLA] = 0;
          //Отчищаем строку массива, с обработанным пакетом
          for (byte t = 0; t < 35; t++) {
            big_array_BLA[f_BLA][t] = 0;
          }
          break;
        }

      case (NPM_ID):
        {
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_BLA[f_BLA] = 0;
          //Отчищаем строку массива, с обработанным пакетом
          for (byte t = 0; t < 35; t++) {
            big_array_BLA[f_BLA][t] = 0;
          }
          break;
        }

      case (TPM_ID):
        {
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_BLA[f_BLA] = 0;
          //Отчищаем строку массива, с обработанным пакетом
          for (byte t = 0; t < 35; t++) {
            big_array_BLA[f_BLA][t] = 0;
          }
          break;
        }

      case (PDR_ID):
        {
          //Поле отработки case с нужным id, занулем его в массиве
          //чтобы запомнить, что он выполнен
          array_BLA[f_BLA] = 0;
          //Отчищаем строку массива, с обработанным пакетом
          for (byte t = 0; t < 35; t++) {
            big_array_BLA[f_BLA][t] = 0;
          }
          break;
        }

      default:
        {
          //do nothing
          break;
        }
    }
    //Добавляем к счетчику
    f_BLA++;
    //Приводим к id кольцевого массива
    if (f_BLA > 4)
    {
      f_BLA = 0;
    }
  }
}


//отправка запросов
void take_NPM_pack()
{
  //Задаем промежуток вемени для отправки запросов на получение пактов NPM
  interval = 10000; //Знамение врнемени в мс

  if (waiting_action_NKR == MCM_ID)
  {
    if (flag_timmer)
    {
      timmer = millis();
      flag_timmer = !flag_timmer;
    }
    if ((millis() - timmer) > interval)
    {
      if (!flag_NPM)
      {
        //Отправляем запрос на получения пакета NPM
        short_msg_union msg_NKR = BLA_packer.pack_tnp();
        SERIAL_NKR.write(msg_NKR.byte_form, sizeof(msg_NKR.byte_form));
        SERIAL_TM.write(msg_NKR.byte_form, sizeof(msg_NKR.byte_form));
        flag_NPM = true;
      } else {}
      flag_timmer = !flag_timmer;
    } else {}
  } else {}
}