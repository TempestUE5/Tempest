#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NiagaraComponent.h"

#include "AHurricane.generated.h"

struct HurricaneTrajectoryData
{
	FVector Start;
	FVector Target;
	FVector StartDir;
	float TimeStep;
};

UCLASS()
class DROWNMENOT_API AHurricane : public AActor
{
	GENERATED_BODY()
	
public:	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Render")
    UNiagaraComponent* NS_HurricaneComponent;

	// Sets default values for this actor's properties
	AHurricane();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "HurricaneBehavior")
	void PredictHurricaneTrajectory(FVector Start, FVector Target, FVector StartDir, float TimeStep = 0.25);

	UFUNCTION(BlueprintCallable, Category = "HurricaneBehavior")
	void TriggerLaunchHurricane();

	UFUNCTION(BlueprintCallable, Category = "HurricaneBehavior")
	void SetAnimationDuration(float Duration);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool IsActive;
	float AnimationDuration;
	
    float ElapsedTime;
	
    HurricaneTrajectoryData CurrentTrajectory;

	void SetIsActive(bool);

	FVector ComputeHurricaneVelocityAtTime(FVector Start, FVector Target, FVector StartDir, float t);
};
