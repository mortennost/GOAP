#include "GOAPEditor.h"
#include "GOAPStateEditor.h"
//#include "Pins/GOAPAtomPin.h"
//#include "Pins/GOAPAtomPinFactory.h"
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "GOAPStateCustomization.h"


IMPLEMENT_GAME_MODULE(FGOAPEditorModule, GOAPEditor);

DEFINE_LOG_CATEGORY(GOAPEditor)

#define LOCTEXT_NAMESPACE "GOAPEditor"

void FGOAPEditorModule::StartupModule()
{
	UE_LOG(GOAPEditor, Warning, TEXT("GOAPEditorModule: Log Started"));

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//Custom properties
	PropertyModule.RegisterCustomPropertyTypeLayout("GOAPAtom", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGOAPStateCustomization::MakeInstance));

	/*
	//create your factory and shared pointer to it.
	TSharedPtr<FGOAPAtomPinFactory> GOAPAtomPinFactory = MakeShareable(new FGOAPAtomPinFactory());
	//and now register it.
	FEdGraphUtilities::RegisterVisualPinFactory(GOAPAtomPinFactory);
	*/
}

void FGOAPEditorModule::ShutdownModule()
{	
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//Custom properties
	PropertyModule.UnregisterCustomPropertyTypeLayout("GOAPAtom");

	UE_LOG(GOAPEditor, Warning, TEXT("GOAPEditorModule: Log Ended"));
}

#undef LOCTEXT_NAMESPACE