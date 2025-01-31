#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/LevelStreaming.h"
#include "GameFramework/PlayerStart.h"
#include "TimeManager.generated.h"



UCLASS()
class ECHOESOFTIME_API UTimeManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UTimeManager();

    void SwitchMap(class AMyCharacter* Character);
    friend class AMyCharacter;


protected:
    virtual void BeginPlay() override;

private:
    AMyCharacter* Character;

    APlayerStart* PlayerStart1;
    APlayerStart* PlayerStart2;

    bool bIsPlayerStart1;
};

