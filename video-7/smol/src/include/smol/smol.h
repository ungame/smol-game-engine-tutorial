#ifndef SMOL_H
#define SMOL_H

#include <iostream>
#include <fstream>
#include <stdlib.h>

// I created this function because I cannot print logs in console...
void WriteLog(const char* filename, const char* message, const char* file = __FILE__, const int line = __LINE__) 
{
	char filepath[100];
	const char* rootDir = "../../../";

	strcpy_s(filepath, sizeof(filepath), rootDir);
	strcat_s(filepath, sizeof(filepath), filename);

	char content[1024];
	const int contentLen = sizeof(content);
	strcpy_s(content, contentLen, message);

	strcat_s(content, contentLen, "@ ");	
	strcat_s(content, contentLen, file);
	strcat_s(content, contentLen, ":");

	char l[256];
	sprintf_s(l, sizeof(l), "%d", line);
	strcat_s(content, contentLen, l);

	std::ofstream myfile;
	myfile.open(filepath);
	myfile << content;
	myfile.close();
}

void WriteLogInfo(const char* message, const char* file = __FILE__, const int line = __LINE__) {
	WriteLog("engine.log", message, file, line);
}

void WriteLogError(const char* message, const char* file = __FILE__, const int line = __LINE__) {
	WriteLog("error.log", message, file, line);
}

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