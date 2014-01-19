//Âåğñèÿ ïğîøèâêè ÄËß ÊÎÍÒĞÎËËÅĞÀ ATMEGA328

//-------------------------------------------------------------------
#ifndef __rc_1_main_H__
#define __rc_1_main_H__
//-------------------------------------------------------------------
 
//#include <dht11.h>
#include <CyberLib.h>
#include <Arduino.h>
#include <avr/wdt.h>
#include <dht11.h>
//
// ÍÎÌÅĞ ÊÎÍÒĞÎËËÅĞÀ Â ØÈÍÅ - ÏÎÂÒÎĞßŞÙÈÕÑß ÁÛÒÜ ÍÅ ÄÎËÆÍÎ!!!
#define SN 5


#define VERSION "0.6"
#define LIFETM_LEN 60000
#define SEEK_BUTT_DEVISOR 2
 

//-------------------------------------------------------------------
 
//-------------------------------------------------------------------
 
// Put yout declarations here
 
//-------------------------------------------------------------------
 
//===================================================================
// -> DO NOT WRITE ANYTHING BETWEEN HERE...
// 		This section is reserved for automated code generation
// 		This process tries to detect all user-created
// 		functions in main_sketch.cpp, and inject their
// 		declarations into this file.
// 		If you do not want to use this automated process,
//		simply delete the lines below, with "&MM_DECLA" text
//===================================================================
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_BEG@---------------------
int main();
void sendAll();
void seekDD();
void seekButtPress();
void applySets();
void readSets();
void loadSets();
void saveSets();
void changeValue(bool * val);
void initFunc();
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_END@---------------------
// -> ...AND HERE. This space is reserved for automated code generation!
//===================================================================
 
 
//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
 
 
