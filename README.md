# Smart City 3D Visualization — Unreal Engine 5

A lightweight 3D smart-city environment built with Unreal Engine 5 and C++. Walk around a small city, observe traffic, toggle day/night, inspect buildings, and trigger emergency alerts.

## Features

- **3D City Exploration** — First-person movement with WASD + mouse look
- **Traffic Lights** — Auto-cycling Red → Green → Yellow at intersections
- **Moving Cars** — Vehicles follow waypoint paths along roads
- **Day/Night Toggle** — Press N to switch; street lights, building lights, and car headlights respond
- **Building Information** — Look at a building and press F to see its info panel
- **Emergency Alert** — Press E to trigger an accident alert that auto-resolves

## Controls

| Key | Action |
|-----|--------|
| WASD | Move |
| Mouse | Look around |
| Shift | Sprint |
| Space | Jump |
| F | Interact with building |
| N | Toggle Day/Night |
| E | Trigger emergency |

## Requirements

- Unreal Engine 5.4+
- Visual Studio 2022 or Rider

## Quick Start

1. Right-click `SmartCity.uproject` → **Generate Visual Studio project files**
2. Open the solution and build (Development Editor / Win64)
3. Open the project in Unreal Editor
4. Create a level with a Directional Light, Sky Atmosphere, and SkyLight
5. Set up Enhanced Input assets (see Editor Setup below)
6. Place actors and play

## Editor Setup

### Enhanced Input

Create these Input Action assets in `Content/Blueprints/Input/`:

| Asset | Value Type |
|-------|-----------|
| IA_Move | Axis2D (Vector2D) |
| IA_Look | Axis2D (Vector2D) |
| IA_Jump | Digital (Bool) |
| IA_Sprint | Digital (Bool) |
| IA_Interact | Digital (Bool) |
| IA_ToggleDayNight | Digital (Bool) |
| IA_TriggerEmergency | Digital (Bool) |

Create an Input Mapping Context `IMC_Default` and bind the actions to WASD, Mouse XY, Space, Shift, F, N, and E respectively.

### Blueprints

1. Create `BP_SmartCityCharacter` (parent: SmartCityPlayerCharacter) — assign all Input Actions, Mapping Context, and HUD Widget Class
2. Create `BP_SmartCityGameMode` (parent: SmartCityGameMode) — set Default Pawn to `BP_SmartCityCharacter`
3. Create `WBP_SmartCityHUD` (parent: SmartCityHUDWidget) — design the UI with widget names matching the C++ `BindWidgetOptional` fields
4. Set the Game Mode Override in World Settings

### Level Actors

Place one of each manager in the level:
- **DayNightManager** — assign your Directional Light to the Sun Light Actor property
- **EmergencyManager** — set the accident location and optionally a marker Blueprint

Place as many as needed:
- **TrafficLight** — at intersections (use InitialDelay to offset timing)
- **WaypointCar** — assign waypoint actors for the path
- **StreetLight** — along roads
- **InteractableBuilding** — for Hospital, Police Station, etc. (fill in BuildingInfo data)

## Project Structure

```
SmartCity/
├── SmartCity.uproject
├── Config/
│   ├── DefaultEngine.ini
│   ├── DefaultGame.ini
│   └── DefaultInput.ini
└── Source/
    ├── SmartCity.Target.cs
    ├── SmartCityEditor.Target.cs
    └── SmartCity/
        ├── SmartCity.Build.cs
        ├── SmartCity.h / .cpp
        ├── SmartCityGameMode.h / .cpp
        ├── SmartCityPlayerCharacter.h / .cpp
        ├── TrafficLight.h / .cpp
        ├── WaypointCar.h / .cpp
        ├── DayNightManager.h / .cpp
        ├── StreetLight.h / .cpp
        ├── InteractableBuilding.h / .cpp
        ├── EmergencyManager.h / .cpp
        └── SmartCityHUDWidget.h / .cpp
```

## Demo Flow

Start → Enter city → Explore → See cars moving → Traffic lights cycling → Press N for night → Lights turn on → Inspect Hospital (F) → Inspect Police Station (F) → Trigger accident (E) → Alert appears → Auto-resolves

## License

MIT
