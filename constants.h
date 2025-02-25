#ifndef CONSTANTS_H
#define CONSTANTS_H

#include<string>

namespace ME {

//communication frame consts
const char FIELD_SEP = '\x1F';
//const char FRAME_END = '\n';
const char FRAME_END = '\x1E';

//config defaults
const std::string PHS_IP = "127.0.0.1";
const int PHS_PORT = 9000;
const std::string COM_PORT = "/dev/ttyS0";
const bool COM_SET_PARAMS = false; //if true app will modify system settings for COM port
const int COM_SPEED = 9600;
const int COM_BITS = 8;
const int COM_PARITY = 0;
const int COM_STOP_BITS = 1;
const int COM_FLOW_CTRL = 0;
const bool COM_BEEP_EN = false;
const int ST_NR = 0;
const bool MW_FULLSCREEN = true;
const bool SCREENSAVER_ON = true;

//serial port consts
const char SP_FRAME_END = '\r';
const char SP_BEEP = '\x07';

//mainwindows elements stylesheets & texts

//const std::string STATION_NR_TXT = "STANOWISKO   ";
const std::string STATION_NR_PREFIX = "B-";
//const std::string PROGRESS_TXT_DEF = "POSTĘPY KOMPLETACJI";
//const std::string PROGRESS_TXT_DL = " DLA POJEMNIKA: ";
//const std::string CONT_BUTTON_DEF_TXT = "";
//const std::string CONT_BUTTON_DL_TXT = "Zakończ\n";

const std::string DEF_STYLESHEET = "";

const std::string CONT_STYLESHEET_0 = "background-color: rgb(160, 160, 200);"; //niebieski
const std::string CONT_STYLESHEET_1 = "background-color: rgb(255, 170, 0);"; //pomarańczowy

const std::string PRD_STYLESHEET_0 = "background-color: rgb(255, 255, 255);"; //biały
const std::string PRD_STYLESHEET_1 = "background-color: rgb(50, 255, 75);"; //zielony
const std::string PRD_STYLESHEET_2 = "background-color: rgb(255, 170, 0);"; //żółto-pomarańczowy
const std::string PRD_STYLESHEET_3 = "background-color: rgb(255, 50, 50);"; //czerwony

const std::string LOC_STYLESHEET_0 = "background-color: rgb(238, 235, 232);"; //domyślny szary
const std::string LOC_STYLESHEET_1 = "background-color: rgb(255, 50, 50);"; //czerwony
const std::string LOC_STYLESHEET_2 = "background-color: rgb(85, 170, 255);"; //niebieski
const std::string LOC_STYLESHEET_3 = "background-color: rgb(255, 255, 0);"; //żółty
const std::string LOC_STYLESHEET_4 = "background-color: rgb(50, 255, 75);"; //żielony

const std::string LFRAME_STYLESHEET_0 = ""; //domyślny szary
const std::string LFRAME_STYLESHEET_1 = "background-color: rgb(255, 85, 0);"; //pomarańczowy

//optdial

const int OPTDIAL_COLS = 3;

const std::string OPTBUTTON_FONT = "font: 75 12pt ""MS Shell Dlg 2""";

//other

const int EAN_INP_MAX_LENGTH = 13;

}

#endif // CONSTANTS_H
