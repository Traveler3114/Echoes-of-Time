// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "EngineUtils.h"


FString UMyGameInstance::GetLevelPath()
{
	FString CurrentLevel = GetWorld()->GetPackage()->GetName();
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	return CurrentLevel;
}