// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ECHOESOFTIME_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveProperties")
	FVector Player1Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveProperties")
	FVector Player2Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveProperties")
	FString SavedLevel;
};
