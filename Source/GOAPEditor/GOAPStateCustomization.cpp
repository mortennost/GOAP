#include "GOAPEditor.h"
#include "GOAPStateCustomization.h"
#include "GOAPState.h"
#include "STextComboBox.h"
#include "SCheckBox.h"
#include "Editor/PropertyEditor/Public/PropertyHandle.h"
#include "Editor/PropertyEditor/Public/DetailWidgetRow.h"
#include "IPropertyUtilities.h"

#define LOCTEXT_NAMESPACE "GOAPStateCustomization"

TSharedRef<IPropertyTypeCustomization> FGOAPStateCustomization::MakeInstance()
{
	return MakeShareable(new FGOAPStateCustomization());
}

void FGOAPStateCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> inStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	_StructPropertyHandle = inStructPropertyHandle;

	// Get the available options (WorldState) from settings class
	for (auto& stateString : _GOAPSettings->WorldStates)
	{
		_AvailableOptions.Add(MakeShareable<FString>(new FString(stateString)));
	}

	// Now fetch the handles to our child properties, and get their value
	uint32 numChildren;
	_StructPropertyHandle->GetNumChildren(numChildren);

	for (uint32 i = 0; i < numChildren; i++)
	{
		const TSharedPtr<IPropertyHandle> childHandle = _StructPropertyHandle->GetChildHandle(i);

		// Child handle is KEY
		if (childHandle->GetProperty()->GetName() == TEXT("Key"))
		{
			_KeyHandle = childHandle;
			childHandle->GetValue(_Key);
		}

		// Child handle is VALUE
		if (childHandle->GetProperty()->GetName() == TEXT("Value"))
		{
			_ValueHandle = childHandle;
			childHandle->GetValue(_Value);
		}
	}

	// Make sure that if we've changed option, we don't go out of bounds
	if (_Key >= _AvailableOptions.Num())
	{
		_Key = 0;
	}

	_SelectedString = _GOAPSettings->GetStringForByte(_Key);

	check(_KeyHandle.IsValid());
	check(_ValueHandle.IsValid());

	// Set up the new header layout
	HeaderRow
		.NameContent()
		[
			_StructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MinDesiredWidth(500)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			[
				SAssignNew(_KeyComboBox, STextComboBox)
				.OptionsSource(&_AvailableOptions)
				.OnSelectionChanged(this, &FGOAPStateCustomization::OnStateValueChanged)
				.InitiallySelectedItem(_AvailableOptions[_Key])
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			[
				SAssignNew(_ValueCheckBox, SCheckBox)
				.OnCheckStateChanged(this, &FGOAPStateCustomization::OnCheckStateChanged)
				.IsChecked(_Value ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
			]
		];
}

void FGOAPStateCustomization::CustomizeChildren(TSharedRef<class IPropertyHandle> inStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	// Create further customization here if/when needed
}

void FGOAPStateCustomization::OnStateValueChanged(TSharedPtr<FString> itemSelected, ESelectInfo::Type selectInfo)
{
	if (itemSelected.IsValid())
	{
		// Fetches the byte index for this string from the settings class
		for (int32 i = 0; i < _AvailableOptions.Num(); i++)
		{
			if (_AvailableOptions[i] == itemSelected)
			{
				_Key = _GOAPSettings->GetByteKey(itemSelected);
				_KeyHandle->SetValue(_Key);
			}
		}
	}
}

void FGOAPStateCustomization::OnStateListOpened()
{

}

void FGOAPStateCustomization::OnCheckStateChanged(ECheckBoxState checkState)
{
	if (checkState == ECheckBoxState::Checked)
	{
		_ValueHandle->SetValue(true);
	}
	else
	{
		_ValueHandle->SetValue(false);
	}
}

#undef LOCTEXT_NAMESPACE