#include "SmartCityHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void USmartCityHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CityTitleText)
		CityTitleText->SetText(FText::FromString(TEXT("SMART CITY")));

	if (PopulationText)
		PopulationText->SetText(FText::FromString(
			FString::Printf(TEXT("Population: %s"), *FString::FromInt(Population))));

	if (BuildingsCountText)
		BuildingsCountText->SetText(FText::FromString(
			FString::Printf(TEXT("Buildings: %d"), TotalBuildings)));

	if (VehiclesCountText)
		VehiclesCountText->SetText(FText::FromString(
			FString::Printf(TEXT("Vehicles: %d"), TotalVehicles)));

	if (TrafficStatusText)
		TrafficStatusText->SetText(FText::FromString(TEXT("Traffic Status: Normal")));

	if (StreetLightsText)
		StreetLightsText->SetText(FText::FromString(TEXT("Street Lights: Standby")));

	if (EmergencyServicesText)
		EmergencyServicesText->SetText(FText::FromString(TEXT("Emergency Services: Active")));

	if (HelpText)
		HelpText->SetText(FText::FromString(
			TEXT("WASD: Move | Mouse: Look | F: Interact | N: Day/Night | E: Emergency | Shift: Sprint | Space: Jump")));

	if (BuildingInfoPanel)
		BuildingInfoPanel->SetVisibility(ESlateVisibility::Collapsed);

	if (EmergencyPanel)
		EmergencyPanel->SetVisibility(ESlateVisibility::Collapsed);

	UpdateDayNightStatus(false);
}

void USmartCityHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!TimeText) return;

	SimulatedHour += InDeltaTime * 0.05f;
	if (SimulatedHour >= 24.0f) SimulatedHour -= 24.0f;

	float DisplayHour = bIsNightTime ? 22.0f + FMath::Fmod(SimulatedHour, 2.0f) : SimulatedHour;
	int32 Hours = FMath::FloorToInt32(FMath::Fmod(DisplayHour, 24.0f));
	int32 Minutes = FMath::FloorToInt32(FMath::Frac(DisplayHour) * 60.0f);

	FString AMPM = Hours >= 12 ? TEXT("PM") : TEXT("AM");
	int32 DisplayH = Hours % 12;
	if (DisplayH == 0) DisplayH = 12;

	TimeText->SetText(FText::FromString(
		FString::Printf(TEXT("%02d:%02d %s"), DisplayH, Minutes, *AMPM)));
}

void USmartCityHUDWidget::ShowBuildingInfo(const FBuildingData& Data)
{
	if (BuildingInfoPanel)
		BuildingInfoPanel->SetVisibility(ESlateVisibility::Visible);

	if (BuildingNameText)
		BuildingNameText->SetText(FText::FromString(Data.BuildingName));

	if (BuildingTypeText)
		BuildingTypeText->SetText(FText::FromString(
			FString::Printf(TEXT("Type: %s"), *Data.BuildingType)));

	if (BuildingCapacityText)
		BuildingCapacityText->SetText(FText::FromString(
			FString::Printf(TEXT("Capacity: %s"), *Data.Capacity)));

	if (BuildingStatusText)
		BuildingStatusText->SetText(FText::FromString(
			FString::Printf(TEXT("Status: %s"), *Data.Status)));
}

void USmartCityHUDWidget::HideBuildingInfo()
{
	if (BuildingInfoPanel)
		BuildingInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void USmartCityHUDWidget::ShowEmergencyAlert(const FString& Message)
{
	if (EmergencyPanel)
		EmergencyPanel->SetVisibility(ESlateVisibility::Visible);

	if (EmergencyText)
		EmergencyText->SetText(FText::FromString(Message));

	if (EmergencyServicesText)
		EmergencyServicesText->SetText(FText::FromString(TEXT("Emergency Services: RESPONDING")));
}

void USmartCityHUDWidget::ShowEmergencyResolved(const FString& Message)
{
	if (EmergencyText)
		EmergencyText->SetText(FText::FromString(Message));

	if (EmergencyServicesText)
		EmergencyServicesText->SetText(FText::FromString(TEXT("Emergency Services: Active")));
}

void USmartCityHUDWidget::HideEmergencyAlert()
{
	if (EmergencyPanel)
		EmergencyPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void USmartCityHUDWidget::UpdateDayNightStatus(bool bIsNight)
{
	bIsNightTime = bIsNight;

	if (DayNightText)
		DayNightText->SetText(FText::FromString(bIsNight ? TEXT("NIGHT") : TEXT("DAY")));

	if (StreetLightsText)
		StreetLightsText->SetText(FText::FromString(
			bIsNight ? TEXT("Street Lights: ON") : TEXT("Street Lights: Standby")));
}
