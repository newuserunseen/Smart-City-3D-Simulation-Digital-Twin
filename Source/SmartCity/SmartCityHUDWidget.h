#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractableBuilding.h"
#include "SmartCityHUDWidget.generated.h"

class UTextBlock;
class UVerticalBox;

UCLASS()
class SMARTCITY_API USmartCityHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowBuildingInfo(const FBuildingData& Data);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideBuildingInfo();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowEmergencyAlert(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowEmergencyResolved(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideEmergencyAlert();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateDayNightStatus(bool bIsNight);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// --- Main Stats Panel ---
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CityTitleText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* PopulationText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* BuildingsCountText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* VehiclesCountText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TrafficStatusText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* StreetLightsText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* EmergencyServicesText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TimeText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* DayNightText;

	// --- Building Info Panel ---
	UPROPERTY(meta = (BindWidgetOptional))
	UVerticalBox* BuildingInfoPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* BuildingNameText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* BuildingTypeText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* BuildingCapacityText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* BuildingStatusText;

	// --- Emergency Panel ---
	UPROPERTY(meta = (BindWidgetOptional))
	UVerticalBox* EmergencyPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* EmergencyText;

	// --- Help ---
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* HelpText;

	// --- Simulated City Data ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CityData")
	int32 Population = 25000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CityData")
	int32 TotalBuildings = 120;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CityData")
	int32 TotalVehicles = 450;

private:
	float SimulatedHour = 10.0f;
	bool bIsNightTime = false;
};
