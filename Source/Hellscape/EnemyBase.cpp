// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"
#include "PlayerCharacterBase.h"
#include "DamageData.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	currentHealth = maxHealth;
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::DamageEnemy(float damage, float potency, UAttackDamageType* attackDamageType, FAttackData attackData)
{
	auto damageData = UDamageData::CreateDamageData(damage, potency);

	OnTakeDamage.Broadcast(damageData, attackDamageType, attackData);

		//Resolve Damage
	currentHealth -= damageData->damage;

	SpawnDamageParticles((int)damageData->damage, attackDamageType->color);

	if (currentHealth <= 0)
	{
		//In the future, should invoke an event the playerCharacter that killed this in the future for use in skills.
		Die();
	}
	else
	{
		auto damageType = attackDamageType->damageType;

		statusBuildup[damageType].buildup += potency;

		if (statusBuildup[damageType].buildup >= resistance)
		{
			statusBuildup[damageType].buildup = 0;
			attackData.attacker->ApplyStatusEffect(this, attackDamageType->statusEffect, attackData.damageSource);
		}

	}

	//The damage data is no longer needed after damage is resolved and can be destroyed to free memory.
	damageData->ConditionalBeginDestroy();
}

void AEnemyBase::ReduceStun()
{
	isStunned--;
}