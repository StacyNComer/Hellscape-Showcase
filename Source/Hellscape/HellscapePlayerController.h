// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HellscapePlayerController.generated.h"

class APlayerCharacterBase;

/**
 * 
 */
UCLASS()
class HELLSCAPE_API AHellscapePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//A cast for the player character possessed by the controller.
	APlayerCharacterBase* playerCharacter;

public:
	virtual void SetupInputComponent() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

private:
	/*
	Toogles whether or not the owning player's menu is being viewed. This must be done from here due to the controls being set to UI only.
	*/
	void TogglePlayerCharacterMenu();
};
