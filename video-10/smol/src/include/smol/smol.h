#ifndef SMOL_H
#define SMOL_H

#include <iostream>
#include <fstream>
#include <stdlib.h>

#if defined(DEBUG) || defined(_DEBUG)
    #define SMOL_DEBUG
#endif

#ifdef SMOL_DEBUG
    #include <stdio.h>
	#define LogMsg(prefix, msg, ...) do { printf("\n%s - ", prefix); printf(msg, __VA_ARGS__); } while(0)
	#define LogMsgAndFileLine(prefix, msg, ...) do { printf("\n%s - ", prefix); printf(msg, __VA_ARGS__); printf("@ %s:%d", __FILE__, __LINE__); } while(0)
	#define LogInfo(msg, ...) LogMsg("[INFO]", msg, __VA_ARGS__)
	#define LogWarning(msg, ...) LogMsgAndFileLine("[WARNING]", msg, __VA_ARGS__)
	#define LogError(msg, ...) LogMsgAndFileLine("[ERROR]", msg, __VA_ARGS__)
#else
  #define SMOL_EMPTY_MACRO do{} while(0)
	#define LogMsg(prefix, msg, ...) SMOL_EMPTY_MACRO
	#define LogInfo(msg, ...) SMOL_EMPTY_MACRO
  #define LogMsgAndFileLine(prefix, msg, ...) SMOL_EMPTY_MACRO
	#define LogInfo(msg, ...) SMOL_EMPTY_MACRO
	#define LogWarning(msg, ...) SMOL_EMPTY_MACRO
	#define LogError(msg, ...) SMOL_EMPTY_MACRO
#endif

#endif