#include "WaypointCar.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "DayNightManager.h"
#include "Kismet/GameplayStatics.h"

AWaypointCar::AWaypointCar()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	CarMesh->SetupAttachment(Root);

	LeftHeadlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("LeftHeadlight"));
	LeftHeadlight->SetupAttachment(CarMesh);
	LeftHeadlight->SetRelativeLocation(FVector(150.0f, -50.0f, 50.0f));
	LeftHeadlight->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));
	LeftHeadlight->SetIntensity(20000.0f);
	LeftHeadlight->SetOuterConeAngle(35.0f);
	LeftHeadlight->SetAttenuationRadius(1500.0f);
	LeftHeadlight->SetLightColor(FLinearColor(1.0f, 0.95f, 0.8f));
	LeftHeadlight->SetVisibility(false);

	RightHeadlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("RightHeadlight"));
	RightHeadlight->SetupAttachment(CarMesh);
	RightHeadlight->SetRelativeLocation(FVector(150.0f, 50.0f, 50.0f));
	RightHeadlight->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));
	RightHeadlight->SetIntensity(20000.0f);
	RightHeadlight->SetOuterConeAngle(35.0f);
	RightHeadlight->SetAttenuationRadius(1500.0f);
	RightHeadlight->SetLightColor(FLinearColor(1.0f, 0.95f, 0.8f));
	RightHeadlight->SetVisibility(false);
}

void AWaypointCar::BeginPlay()
{
	Super::BeginPlay();
	ResolveWaypoints();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADayNightManager::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		ADayNightManager* Manager = Cast<ADayNightManager>(FoundActors[0]);
		if (Manager)
		{
			Manager->OnDayNightChanged.AddDynamic(this, &AWaypointCar::OnDayNightChanged);
			SetHeadlights(Manager->IsNight());
		}
	}
}

void AWaypointCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ResolvedWaypoints.Num() == 0) return;

	FVector Target = GetCurrentWaypoint();
	FVector CurrentLocation = GetActorLocation();
	FVector Direction = Target - CurrentLocation;
	float Distance = Direction.Size();

	if (Distance <= WaypointReachDistance)
	{
		AdvanceWaypoint();
		return;
	}

	Direction.Normalize();

	FVector NewLocation = CurrentLocation + Direction * MoveSpeed * DeltaTime;
	SetActorLocation(NewLocation);

	FRotator TargetRotation = Direction.Rotation();
	FRotator CurrentRotation = GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
	SetActorRotation(NewRotation);
}

void AWaypointCar::SetHeadlights(bool bOn)
{
	bHeadlightsOn = bOn;
	LeftHeadlight->SetVisibility(bOn);
	RightHeadlight->SetVisibility(bOn);
}

void AWaypointCar::OnDayNightChanged(bool bIsNight)
{
	SetHeadlights(bIsNight);
}

void AWaypointCar::ResolveWaypoints()
{
	ResolvedWaypoints.Empty();

	for (AActor* WP : WaypointActors)
	{
		if (WP)
		{
			ResolvedWaypoints.Add(WP->GetActorLocation());
		}
	}

	for (const FVector& WP : WaypointLocations)
	{
		ResolvedWaypoints.Add(WP);
	}

	CurrentWaypointIndex = 0;
}

FVector AWaypointCar::GetCurrentWaypoint() const
{
	if (ResolvedWaypoints.IsValidIndex(CurrentWaypointIndex))
	{
		return ResolvedWaypoints[CurrentWaypointIndex];
	}
	return GetActorLocation();
}

void AWaypointCar::AdvanceWaypoint()
{
	CurrentWaypointIndex++;

	if (CurrentWaypointIndex >= ResolvedWaypoints.Num())
	{
		if (bLoopPath)
		{
			CurrentWaypointIndex = 0;
		}
		else
		{
			CurrentWaypointIndex = ResolvedWaypoints.Num() - 1;
		}
	}
}
