// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "NiagaraSystem.h"
#include "CharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);

UCLASS()
class DROWNMENOT_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ACharacterBase();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
    UAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
    class UBasicAttributeSet* BasicAttributeSet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* SpawnSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* SpawnAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* SpawnEffect;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UStaticMeshComponent* HurricaneOrbMeshComponent;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
    EGameplayEffectReplicationMode AscReplicationMode = EGameplayEffectReplicationMode::Mixed;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void PossessedBy(AController* NewController) override;

    virtual void OnRep_PlayerState() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FDeathDelegate OnDeath;

    UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void RestoreHealthToFull();

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void RestoreManaToFull();

    UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void IncreaseMaxMana(float Amount);

    UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
    void SetMaxHealth(float NewMaxHealth);

private:
    FTimerHandle m_AudioStopTimerHandle;
};
