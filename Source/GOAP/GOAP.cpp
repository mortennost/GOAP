// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GOAP.h"
#include "GOAPSettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ModuleInterface.h"
#include "ModuleManager.h"

#define LOCTEXT_NAMESPACE "FGOAPModule"

IMPLEMENT_PRIMARY_GAME_MODULE(FGOAPModule, GOAP, "GOAP");

void FGOAPModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per module (plugins only??)

	// Get settings module
	ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	// Register GOAP settings section
	if (settingsModule != nullptr)
	{
		ISettingsSectionPtr settingsSection = settingsModule->RegisterSettings(
			"Project",															// Will reside in Project Settings window container ..
			"Custom",															// .. within the "Custom" category ..
			"GOAP Settings",													// .. in a new section "GOAP Settings".
			LOCTEXT("GOAPSettingsName", "GOAP"),								// Localized display name
			LOCTEXT("GOAPSettingsDescription", "Configure the GOAP settings."),	// Localized description
			GetMutableDefault<UGOAPSettings>()
		);

		if (settingsSection.IsValid())
		{
			settingsSection->OnModified().BindRaw(this, &FGOAPModule::HandleSettingsSaved);
		}
	}
}

void FGOAPModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.

	// Get settings module
	ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	// Unregister GOAP settings
	if (settingsModule != nullptr)
	{
		settingsModule->UnregisterSettings("Project", "Custom", "GOAP");
	}
}

bool FGOAPModule::HandleSettingsSaved()
{
	return true;
}

#undef LOCTEXT_NAMESPACE

// IMPLEMENT_MODULE(FGOAPModule, GOAP)
 