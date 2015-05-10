#include "Menu.hpp"

namespace GTA
{
	Menu::Menu(System::String ^headerCaption, array<MenuItem ^> ^items)
	{
		//Put the items in the item stack
		//The menu itself will be initialized when it gets added to the viewport
		for each (MenuItem ^item in items)
		{
			mItems->Add(item);
			item->Parent = this;
		}

		//Set defaults for the properties
		HeaderColor = System::Drawing::Color::FromArgb(200, 255, 20, 147);
		HeaderTextColor = System::Drawing::Color::White;
		HeaderFont = 1;
		HeaderTextScale = 0.5f;
		HeaderCentered = true;
		FooterColor = System::Drawing::Color::FromArgb(200, 255, 182, 193);
		FooterTextColor = System::Drawing::Color::Black;
		FooterFont = 0;
		FooterTextScale = 0.4f;
		FooterCentered = false;
		SelectedItemColor = System::Drawing::Color::FromArgb(200, 255, 105, 180);
		UnselectedItemColor = System::Drawing::Color::FromArgb(200, 176, 196, 222);
		SelectedTextColor = System::Drawing::Color::Black;
		UnselectedTextColor = System::Drawing::Color::DarkSlateGray;
		ItemFont = 0;
		ItemTextScale = 0.4f;
		ItemTextCentered = true;
		Caption = headerCaption;

		Width = 200;
		HeaderHeight = 30;
		FooterHeight = 60;
		ItemHeight = 30;
		HasFooter = true;
	}

	void Menu::Draw()
	{
		Draw(System::Drawing::Point());
	}

	void Menu::Draw(System::Drawing::Point offset)
	{
		if (mHeaderRect == nullptr || mHeaderText == nullptr || (HasFooter && (mFooterRect == nullptr || mFooterText == nullptr))) return;
		if (HasFooter)
		{
			mFooterRect->Draw(offset.X, offset.Y);
			mFooterText->Draw(offset.X, offset.Y);
		}
		mHeaderRect->Draw(offset.X, offset.Y);
		mHeaderText->Draw(offset.X, offset.Y);
		for each (MenuItem ^item in mItems)
		{
			item->Draw(offset);
		}
	}

	void Menu::Initialize()
	{
		int currentY = HeaderHeight;
		System::Drawing::Size itemSize = System::Drawing::Size(Width, ItemHeight);
		for each (MenuItem ^item in mItems)
		{
			item->SetOriginAndSize(System::Drawing::Point(0, currentY), itemSize);
			currentY += ItemHeight;
		}
		mSelectedIndex = 0;
		mFooterDescription = mItems[mSelectedIndex]->Description;
		mItems[mSelectedIndex]->Select();

		int itemsHeight = mItems->Count * ItemHeight;
		mHeaderRect = gcnew UIRectangle(System::Drawing::Point(),
			System::Drawing::Size(Width, HeaderHeight), HeaderColor);
		if (HasFooter) mFooterRect = gcnew UIRectangle(System::Drawing::Point(0, HeaderHeight + itemsHeight),
			System::Drawing::Size(Width, FooterHeight), FooterColor);

		mHeaderText = gcnew UIText(Caption,
			HeaderCentered ? System::Drawing::Point(Width / 2, 0) : System::Drawing::Point(),
			HeaderTextScale,
			HeaderTextColor,
			HeaderFont,
			HeaderCentered);

		if (HasFooter) mFooterText = gcnew UIText(mFooterDescription,
			FooterCentered ? System::Drawing::Point(Width / 2, HeaderHeight + itemsHeight) : System::Drawing::Point(0, HeaderHeight + itemsHeight),
			FooterTextScale,
			FooterTextColor,
			FooterFont,
			FooterCentered);
	}

	void Menu::OnOpen()
	{
	}

	void Menu::OnClose()
	{
	}

	void Menu::OnActivate()
	{
		if (mSelectedIndex < 0 || mSelectedIndex >= mItems->Count) return;
		mItems[mSelectedIndex]->Activate();
	}

	void Menu::OnChangeSelection(bool down)
	{
		int newIndex = down ? mSelectedIndex + 1 : mSelectedIndex - 1;
		if (newIndex < 0) newIndex = 0;
		if (newIndex >= mItems->Count) newIndex = mItems->Count - 1;
		mItems[mSelectedIndex]->Deselect();
		mSelectedIndex = newIndex;
		mFooterDescription = mItems[mSelectedIndex]->Description;
		mItems[mSelectedIndex]->Select();

		//Update footer
		int itemsHeight = mItems->Count * ItemHeight;
		if (HasFooter) mFooterText = gcnew UIText(mFooterDescription,
			FooterCentered ? System::Drawing::Point(Width / 2, HeaderHeight + itemsHeight) : System::Drawing::Point(0, HeaderHeight + itemsHeight),
			FooterTextScale,
			FooterTextColor,
			FooterFont,
			FooterCentered);
	}

	void Menu::OnChangeItem(bool right)
	{
		if (mSelectedIndex < 0 || mSelectedIndex >= mItems->Count) return;
		mItems[mSelectedIndex]->Change(right);
	}

	MessageBox::MessageBox(System::String ^caption, System::Action ^yes, System::Action ^no)
		: mYesAction(yes), mNoAction(no)
	{
		HeaderColor = System::Drawing::Color::FromArgb(200, 255, 20, 147);
		HeaderTextColor = System::Drawing::Color::White;
		HeaderFont = 1;
		HeaderTextScale = 0.5f;
		HeaderCentered = true;
		SelectedItemColor = System::Drawing::Color::FromArgb(200, 255, 105, 180);
		UnselectedItemColor = System::Drawing::Color::FromArgb(200, 176, 196, 222);
		SelectedTextColor = System::Drawing::Color::Black;
		UnselectedTextColor = System::Drawing::Color::DarkSlateGray;
		ItemFont = 0;
		ItemTextScale = 0.4f;
		ItemTextCentered = true;
		Caption = caption;

		Width = 200;
		Height = 50;
		ButtonHeight = 30;
		OkCancel = false;
	}

	void MessageBox::Draw()
	{
		Draw(System::Drawing::Point());
	}

	void MessageBox::Draw(System::Drawing::Point offset)
	{
		mBodyRect->Draw(offset.X, offset.Y);
		mText->Draw(offset.X, offset.Y);
		mYesRect->Draw(offset.X, offset.Y);
		mNoRect->Draw(offset.X, offset.Y);
		mYesText->Draw(offset.X, offset.Y);
		mNoText->Draw(offset.X, offset.Y);
	}

	void MessageBox::Initialize()
	{
		mBodyRect = gcnew UIRectangle(Position, System::Drawing::Size(Width, Height), HeaderColor);
		mText = gcnew UIText(Caption, HeaderCentered ? System::Drawing::Point(Position.X + Width / 2, Position.Y) : Position, HeaderTextScale, HeaderTextColor, HeaderFont, HeaderCentered);
		mYesRect = gcnew UIRectangle(System::Drawing::Point(Position.X, Height + Position.Y), System::Drawing::Size(Width / 2, ButtonHeight), UnselectedItemColor);
		mNoRect = gcnew UIRectangle(System::Drawing::Point(Position.X + Width / 2, Height + Position.Y), System::Drawing::Size(Width / 2, ButtonHeight), UnselectedItemColor);
		mYesText = gcnew UIText(OkCancel ? "OK" : "Yes", System::Drawing::Point(Position.X + Width / 4, Position.Y + Height), ItemTextScale, UnselectedTextColor, ItemFont, ItemTextCentered);
		mNoText = gcnew UIText(OkCancel ? "Cancel" : "No", System::Drawing::Point(Position.X + Width / 4 * 3, Position.Y + Height), ItemTextScale, UnselectedTextColor, ItemFont, ItemTextCentered);
	}

	void MessageBox::OnOpen()
	{
	}

	void MessageBox::OnClose()
	{
	}

	void MessageBox::OnActivate()
	{
		if (mSelection) mYesAction->Invoke();
		else mNoAction->Invoke();
		Parent->PopMenu();
	}

	void MessageBox::OnChangeSelection(bool _A)
	{
	}

	void MessageBox::OnChangeItem(bool _A)
	{
		mSelection = !mSelection;
		if (mSelection)
		{
			mYesRect->Color = SelectedItemColor;
			mNoRect->Color = UnselectedItemColor;
			mYesText->Color = SelectedTextColor;
			mNoText->Color = UnselectedTextColor;
		}
		else
		{
			mNoRect->Color = SelectedItemColor;
			mYesRect->Color = UnselectedItemColor;
			mNoText->Color = SelectedTextColor;
			mYesText->Color = UnselectedTextColor;
		}
	}
}