// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffectData.generated.h"

/**
 * 
 */
UCLASS()
class HELLSCAPE_API UStatusEffectData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float intensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float duration;
	
	UFUNCTION(Blueprintcallable)
		static UStatusEffectData* CreateStatusEffectData(float statusIntensity, float statusDuration)
	{
		auto instance = NewObject<UStatusEffectData>();
		instance->intensity = statusIntensity;
		instance->duration = statusDuration;

		return instance;
	}
};
