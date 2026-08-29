#include "EmergencyManager.h"

AEmergencyManager::AEmergencyManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEmergencyManager::TriggerEmergency()
{
	if (CurrentState == EEmergencyState::Active) return;

	CurrentState = EEmergencyState::Active;

	FString AlertMessage = FString::Printf(
		TEXT("ACCIDENT DETECTED\nLocation: %s\nEmergency Response Required"),
		*AccidentLocationName);
	OnEmergencyStateChanged.Broadcast(CurrentState, AlertMessage);

	if (MarkerActorClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnedMarker = GetWorld()->SpawnActor<AActor>(
			MarkerActorClass, AccidentLocation, FRotator::ZeroRotator, SpawnParams);
	}

	GetWorldTimerManager().SetTimer(
		ResolutionTimerHandle, this, &AEmergencyManager::ResolveEmergency, ResolutionTime, false);
}

void AEmergencyManager::ResolveEmergency()
{
	CurrentState = EEmergencyState::Resolved;

	FString ResolvedMessage = TEXT("EMERGENCY RESOLVED\nAll units returning to station");
	OnEmergencyStateChanged.Broadcast(CurrentState, ResolvedMessage);

	if (SpawnedMarker)
	{
		SpawnedMarker->Destroy();
		SpawnedMarker = nullptr;
	}

	GetWorldTimerManager().SetTimer(
		ResolutionTimerHandle, this, &AEmergencyManager::ClearEmergency, 3.0f, false);
}

void AEmergencyManager::ClearEmergency()
{
	CurrentState = EEmergencyState::None;
	OnEmergencyStateChanged.Broadcast(CurrentState, TEXT(""));
}
