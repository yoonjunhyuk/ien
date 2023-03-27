// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(MyProject2, Log, All);
#define MYLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define MYLOG_S(Verbosity) UE_LOG(MyProject2, Verbosity, TEXT("%s"), *MYLOG_CALLINFO)
#define MYLOG(Verbosity, Format, ...) UE_LOG(MyProject2, Verbosity, TEXT("%s %s"), *MYLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define MYCHECK(Expr, ...) {if(!(Expr)) {MYLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}