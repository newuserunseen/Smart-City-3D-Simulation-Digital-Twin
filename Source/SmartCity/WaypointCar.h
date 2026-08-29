#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaypointCar.generated.h"

class UStaticMeshComponent;
class USpotLightComponent;

UCLASS()
class SMARTCITY_API AWaypointCar : public AActor
{
	GENERATED_BODY()

public:
	AWaypointCar();

	UFUNCTION(BlueprintCallable, Category = "Lights")
	void SetHeadlights(bool bOn);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CarMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpotLightComponent* LeftHeadlight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpotLightComponent* RightHeadlight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoints")
	TArray<AActor*> WaypointActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoints")
	TArray<FVector> WaypointLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WaypointReachDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bLoopPath = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 3.0f;

private:
	UFUNCTION()
	void OnDayNightChanged(bool bIsNight);

	int32 CurrentWaypointIndex = 0;
	TArray<FVector> ResolvedWaypoints;
	bool bHeadlightsOn = false;

	void ResolveWaypoints();
	FVector GetCurrentWaypoint() const;
	void AdvanceWaypoint();
};
