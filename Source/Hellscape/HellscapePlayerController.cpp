// Fill out your copyright notice in the Description page of Project Settings.


#include "HellscapePlayerController.h"
#include "PlayerCharacterBase.h"

void AHellscapePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	playerCharacter = Cast<APlayerCharacterBase>(InPawn);
}

void AHellscapePlayerController::OnUnPossess()
{
	playerCharacter = nullptr;
}

void AHellscapePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(
		"CharacterView",
		IE_Pressed,
		this,
		&AHellscapePlayerController::TogglePlayerCharacterMenu
	);
}

void AHellscapePlayerController::TogglePlayerCharacterMenu()
{
	if (playerCharacter != nullptr)
	{
		if (playerCharacter->inCharacterMenu)
		{
				//Menu toggled off

			GetPawn()->EnableInput(this);

			//Disable the mouse
			bShowMouseCursor = false;
			bEnableClickEvents = false;
			bEnableMouseOverEvents = false;

			SetInputMode(FInputModeGameOnly());
		}
		else
		{
				//Menu toggled on	

			GetPawn()->DisableInput(this);

			//Enable the mouse
			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;

			SetInputMode(FInputModeGameAndUI());
		}
			
		//The "inCharacterMenu" variable is toggled in this function.
		playerCharacter->ToggleCharacterMenu();
	}
}
