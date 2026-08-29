#include "InteractableBuilding.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "DayNightManager.h"
#include "Kismet/GameplayStatics.h"

AInteractableBuilding::AInteractableBuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(Root);

	InteriorLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("InteriorLight"));
	InteriorLight->SetupAttachment(Root);
	InteriorLight->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	InteriorLight->SetIntensity(InteriorLightIntensity);
	InteriorLight->SetLightColor(FLinearColor(1.0f, 0.9f, 0.7f));
	InteriorLight->SetAttenuationRadius(600.0f);
	InteriorLight->SetVisibility(false);
}

void AInteractableBuilding::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADayNightManager::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ADayNightManager* Manager = Cast<ADayNightManager>(FoundActors[0]);
		if (Manager)
		{
			Manager->OnDayNightChanged.AddDynamic(this, &AInteractableBuilding::OnDayNightChanged);
			InteriorLight->SetVisibility(Manager->IsNight());
		}
	}
}

void AInteractableBuilding::OnDayNightChanged(bool bIsNight)
{
	InteriorLight->SetVisibility(bIsNight);
}
