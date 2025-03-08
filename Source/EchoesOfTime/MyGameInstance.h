// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ECHOESOFTIME_API UMyGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Level")
	FString GetLevelPath();
	
};
