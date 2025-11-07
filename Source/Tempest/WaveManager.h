// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tempest/GameplayAbilitySystem/Characters/CharacterBase.h"
#include "Containers/Array.h"
#include "Delegates/DelegateCombinations.h"
#include "WaveManager.generated.h"

UENUM(BlueprintType)
enum class EOverallWaveType : uint8
{
	ProcedurallyGenerated UMETA(DisplayName = "ProcedurallyGenerated"),
	EasyWavePreset UMETA(DisplayName = "EasyWavePreset"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnWaveInfoChanged,
	int32, CurrentEnemyCount,
	int32, TotalWaveEnemyCount,
	int32, CurrentWaveNumber
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnWaveCompleted,
	int32, WaveStarting
);

UCLASS()
class TEMPEST_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveManager();
	
	// List of classes an enemy can be spawned from
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Characteristics")
	TArray<TSubclassOf<ACharacterBase>> EnemyClassPool;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Characteristics")
	//should make a uproperty when functionality is present for different wave presets
	EOverallWaveType OverallWaveType{ EOverallWaveType::EasyWavePreset };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TArray<USceneComponent*> SpawnPointSceneComponents;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn Parameters")
	TArray<FVector> SpawnPointLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
	int NumSpawnPoints = 3;
private:
	int CurrentWave = 0;
	int NumEnemiesKilledThisWave = 0;
	int EnemyCountThisWave = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartNextWave();

	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnWaveInfoChanged OnWaveInfoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnWaveCompleted OnWaveCompleted;

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void GetCurrentWaveInfo(int32& OutCurrentEnemyCount, int32& OutTotalWaveEnemyCount, int32& OutCurrentWaveNumber) const
	{
		OutCurrentEnemyCount = EnemyCountThisWave - NumEnemiesKilledThisWave;
		OutTotalWaveEnemyCount = EnemyCountThisWave;
		OutCurrentWaveNumber = CurrentWave;
	}

private:
	void StartNextProceduralWave(int EnemyCount);
	void StartNextEasyWavePreset(int WaveNumber);

	void SpawnEnemiesForWave(int EnemyCount, bool isBossWave = false);

	void ProcessWaveEnd();

	void ProcessWin();

	UFUNCTION()
	void OnCharacterDied();

};
