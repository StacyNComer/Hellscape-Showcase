// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageData.h"
#include "AttackDamageType.h"
#include "EnemyBase.generated.h"


USTRUCT(BlueprintType)
struct FStatusBuildupData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float buildup = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8 timesApplied = 0;
};

UCLASS()
class HELLSCAPE_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTakeDamage, UDamageData*, damageData, UAttackDamageType*, attackDamageType, FAttackData, attackData);

	std::map<EStatusEffect, FTimerHandle> statusTimer;

#pragma region UProperties
	//A delegate for handling the effects of an enemy taking damage and status effect buildup. Called by TakeDamage before the actual damage/status buildup is applied.
	UPROPERTY(BlueprintAssignable)
		FTakeDamage OnTakeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float resistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//If above 0, the enemy is stunned and cannot act.
		int32 isStunned = 0;
#pragma endregion

private:
	//Links each damage type to its relavent buildup data.
	std::map<EDamageType, FStatusBuildupData> statusBuildup;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AEnemyBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Calls OnTakeDamage using the data given, then substracts the damage from the enemy's health.
	UFUNCTION(BlueprintCallable)
		void DamageEnemy(float damage, float potency, UAttackDamageType* attackDamageType, FAttackData attackData);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void Die();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SpawnDamageParticles(int damage, FLinearColor textColor);

	UFUNCTION(BlueprintCallable)
		//Reduces the isStunned value by 1. As long as this value is above 1, the enemy will be stunned.
		void ReduceStun();
};
