#include "AHurricane.h"

#include "NiagaraSystem.h"

#include "Engine/Engine.h"

// Sets default values
AHurricane::AHurricane()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	NS_HurricaneComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NS_HurricaneComponent->SetupAttachment(RootComponent);

    NS_HurricaneComponent->SetAsset(LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Niagara/NS_Hurricane.NS_Hurricane")));

    NS_HurricaneComponent->SetAutoActivate(false);

    this->SetAnimationDuration(2.f);
    this->SetIsActive(false);

    // initialize with a default trajectory
    PredictHurricaneTrajectory(FVector::ZeroVector, FVector(1000.f, 1000.f, 0.f), FVector(1.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void AHurricane::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHurricane::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsActive && ElapsedTime < AnimationDuration)
    {
        FVector CurrentVelocity = ComputeHurricaneVelocityAtTime(CurrentTrajectory.Start, CurrentTrajectory.Target, CurrentTrajectory.StartDir, ElapsedTime);
        SetActorLocation(GetActorLocation() + CurrentVelocity * DeltaTime);
        ElapsedTime += DeltaTime;
    } else if (IsActive && ElapsedTime >= AnimationDuration) {

        SetIsActive(false);
    }
}

FVector AHurricane::PredictHurricaneTrajectory(FVector Start, FVector Target, FVector StartDir, float TimeStep)
{
    FVector CurrentPos = Start;

    for (float Time = 0.f; Time < AnimationDuration; Time += TimeStep)
    {
        FVector Velocity = ComputeHurricaneVelocityAtTime(Start, Target, StartDir, Time);

        FVector NextPos = CurrentPos + Velocity * TimeStep;


        //DrawDebugLine(GetWorld(), CurrentPos, NextPos, FColor::White, false, 0.01f, 0, 1.f);
        DrawDebugDirectionalArrow(GetWorld(), CurrentPos, CurrentPos + Velocity * 0.001f, 30.f, FColor::Emerald, false, 0.01f, 0, 2.f);

        CurrentPos = NextPos;
    }

    CurrentTrajectory = { Start, Target, StartDir, TimeStep };

    return CurrentPos;
}

void AHurricane::TriggerLaunchHurricane()
{
    SetIsActive(true);
}

void AHurricane::SetAnimationDuration(float d) {
    AnimationDuration = d;

    // ensure animation duration and niagara lifetime always match
    if (NS_HurricaneComponent != nullptr)
    {
        FName userVarName = FName("User.HurricaneLifetime");
        NS_HurricaneComponent->SetVariableFloat(userVarName, d);
    }
}

void AHurricane::SetIsActive(bool a) {
    IsActive = a;
    ElapsedTime = -1.f;
    if (NS_HurricaneComponent != nullptr)
    {
        if (a && !NS_HurricaneComponent->IsActive()) {
            SetActorLocation(CurrentTrajectory.Start);

            NS_HurricaneComponent->Activate(true);
            ElapsedTime = 0.f;
        }
        else if (!a && NS_HurricaneComponent->IsActive()) {
            NS_HurricaneComponent->Deactivate();
        }
    }
}

//FVector AHurricane::ComputeHurricaneVelocityAtTime(FVector Start, FVector Target, FVector StartDir, float t)
//{
//    // Normalize time
//    float alpha = FMath::Clamp(t / AnimationDuration, 0.f, 1.f);
//
//    // Flatten directions to the XZ plane
//    FVector FlatStartDir = StartDir.GetSafeNormal2D();
//    FVector FlatToTarget = (Target - Start).GetSafeNormal2D();
//
//    // Slerp between start and target directions for smooth curvature
//    FVector CurvedDir = FMath::Lerp(FlatStartDir, FlatToTarget, alpha).GetSafeNormal();
//
//    // Compute constant speed to reach target in time
//    float TotalDistance = FVector::Dist2D(Start, Target);
//    float Speed = TotalDistance / AnimationDuration;
//
//    // Final velocity
//    FVector Velocity = CurvedDir * Speed;
//    return Velocity;
//}

FVector AHurricane::ComputeHurricaneVelocityAtTime(FVector Start, FVector Target, FVector StartDir, float t)
{
   // Normalize time
    t = FMath::Clamp(t / AnimationDuration, 0.f, 1.f);

    // Zero out Z so the path stays flat
    FVector FlatStart = FVector(Start.X, Start.Y, 0.f);
    FVector FlatTarget = FVector(Target.X, Target.Y, 0.f);
    FVector FlatStartDir = FVector(StartDir.X, StartDir.Y, 0.f).GetSafeNormal();

    // Define tangents (scaled by distance for smoother shape)
    float Distance = FVector::Dist(FlatStart, FlatTarget);
    FVector StartTangent = FlatStartDir * Distance * 0.5f;

    FVector EndDir = (FlatTarget - FlatStart).GetSafeNormal();
    FVector EndTangent = EndDir * Distance * 0.5f;

    // Hermite basis function derivatives (for velocity)
    float h1 = 6 * t * t - 6 * t;
    float h2 = 3 * t * t - 4 * t + 1;
    float h3 = -6 * t * t + 6 * t;
    float h4 = 3 * t * t - 2 * t;

    // Combine in XY only
    FVector VelocityXY =
        h1 * FlatStart +
        h2 * StartTangent +
        h3 * FlatTarget +
        h4 * EndTangent;

    // Return velocity with zero Z
    return FVector(VelocityXY.X, VelocityXY.Y, 0.f);
}