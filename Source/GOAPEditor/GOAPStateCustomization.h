#include "Editor/DetailCustomizations/Public/DetailCustomizations.h"
#include "Editor/PropertyEditor/Public/IPropertyTypeCustomization.h"
#include "GOAPState.h"
#include "GOAPSettings.h"

#pragma once

class FGOAPStateCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> inStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> inStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:
	UGOAPSettings* _GOAPSettings = GetMutableDefault<UGOAPSettings>();

	TArray<TSharedPtr<FString>> _AvailableOptions;

	TSharedPtr<IPropertyHandle> _StructPropertyHandle;

	TSharedPtr<IPropertyHandle> _KeyHandle;
	TSharedPtr<IPropertyHandle> _ValueHandle;

	TSharedPtr<STextComboBox> _KeyComboBox;
	TSharedPtr<SCheckBox> _ValueCheckBox;

	TSharedPtr<FString> _SelectedString;
	uint8 _Key;
	bool _Value;

	void OnStateValueChanged(TSharedPtr<FString> itemSelected, ESelectInfo::Type selectInfo);
	void OnStateListOpened();
	void OnCheckStateChanged(ECheckBoxState checkState);

	// Property utilities
	TSharedPtr<class IPropertyUtilities> _PropertyUtilities;
};
