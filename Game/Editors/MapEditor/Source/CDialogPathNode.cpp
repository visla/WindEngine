

#include <CDialogPathNode.h>


CDialogPathNode *CDialogPathNode::Instance()
{
	if (_instance) return _instance;
	_instance = new CDialogPathNode;
	return _instance;
}

CDialogPathNode *CDialogPathNode::_instance = NULL;


bool CDialogPathNode::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_PATHNODE, hParent, Show))
	{
		return false;
	}

	PositionXEdit = new CEditBox(this, IDC_POSITIONX);
	PositionYEdit = new CEditBox(this, IDC_POSITIONY);
	PositionZEdit = new CEditBox(this, IDC_POSITIONZ);

	SequenceNumber = new CEditBox(this, IDC_SEQUENCENUMBER);
	FHDRI = new CEditBox(this, IDC_FHDRI);
	HealingRange = new CEditBox(this, IDC_PATHNODE_HEALINGRANGE);
	HealingSpeed = new CEditBox(this, IDC_PATHNODE_HEALINGSPEED);

	HeadLight = new CComboBox(this, IDC_PATHNODE_HEADLIGHT);
	HeadLight->Clear();
	HeadLight->AddItem("NOTHING");
	HeadLight->AddItem("OFF");
	HeadLight->AddItem("ON");

	NodeType = new CComboBox(this, IDC_PATHNODE_NODETYPE);
	NodeType->Clear();
	NodeType->AddItem("NORMAL");
	NodeType->AddItem("SPLIT");
	NodeType->AddItem("CONNECT");
	NodeType->AddItem("TELEPORTING");
	NodeType->AddItem("TELEPORTED");

	LeftNum = new CEditBox(this, IDC_PATHNODE_LEFT);
	RightNum = new CEditBox(this, IDC_PATHNODE_RIGHT);

	FollowTime = new CEditBox(this, IDC_PATHNODE_FOLLOWTIME);

	Refraction = new CCheckBox(this, IDC_PATHNODE_REFRACTION);
	Shaking = new CCheckBox(this, IDC_PATHNODE_SHAKING);

	ResetButton = new CButton(this, IDC_RESET);
	OKButton = new CButton(this, IDC_OK);

	CanChange = false;


	return true;
}


void CDialogPathNode::Update(WePathNode *pathnode)
{
	CanChange = false;
	PositionXEdit->SetText(pathnode->Location.x);
	PositionYEdit->SetText(pathnode->Location.y);
	PositionZEdit->SetText(pathnode->Location.z);
	SequenceNumber->SetText(pathnode->SequenceNumber);
	FHDRI->SetText(pathnode->FHDRI);
	HealingRange->SetText(pathnode->HealingRange);
	HealingSpeed->SetText(pathnode->HealingSpeed);
	HeadLight->SetSelection(pathnode->HeadLight);
	NodeType->SetSelection(pathnode->NodeType);
	LeftNum->SetText(pathnode->LeftNum);
	RightNum->SetText(pathnode->RightNum);
	FollowTime->SetText(pathnode->FollowTime);
	Refraction->SetChecked(pathnode->Refraction);
	Shaking->SetChecked(pathnode->Shaking);

	CanChange = true;
}

void CDialogPathNode::Show(WePathNode *pathnode)
{
	if (!pathnode) return;
	
	PathNode = pathnode;
	Update(PathNode);

	CDialog::Show();
}


void CDialogPathNode::OnPositionChange()
{
	WeVector Loc;
	Loc.x = PositionXEdit->GetTextAsFloat();
	Loc.y = PositionYEdit->GetTextAsFloat();
	Loc.z = PositionZEdit->GetTextAsFloat();
	PathNode->MoveTo(Loc);
}

void CDialogPathNode::OnClick(DWORD ID)
{
	if (ID == OKButton->GetID())
	{
		Hide();
	}

	else if (ID == ResetButton->GetID())
	{
		WePathNode::ResetSequenceNumbers();
	}

	else if (ID == IDC_PATHNODE_HEADLIGHT)
	{
		PathNode->HeadLight = HeadLight->GetSelectionIndex();
	}

	else if (ID == IDC_PATHNODE_NODETYPE)
	{
		PathNode->NodeType = NodeType->GetSelectionIndex();
	}

	else if (ID == Refraction->GetID())
	{
		PathNode->Refraction = Refraction->GetChecked();
	}

	else if (ID == Shaking->GetID())
	{
		PathNode->Shaking = Shaking->GetChecked();
	}
}

void CDialogPathNode::OnChange(DWORD ID)
{
	if (!CanChange) return;

	if (ID == PositionXEdit->GetID())
	{
		OnPositionChange();
	}

	else if (ID == PositionYEdit->GetID())
	{
		OnPositionChange();
	}

	else if (ID == PositionZEdit->GetID())
	{
		OnPositionChange();
	}

	else if (ID == SequenceNumber->GetID())
	{
		PathNode->SequenceNumber = SequenceNumber->GetTextAsInt();
	}

	else if (ID == FHDRI->GetID())
	{
		PathNode->FHDRI = FHDRI->GetTextAsFloat();
	}
	else if (ID == IDC_PATHNODE_HEALINGRANGE)
	{
		PathNode->HealingRange = HealingRange->GetTextAsFloat();
	}
	else if (ID == IDC_PATHNODE_HEALINGSPEED)
	{
		PathNode->HealingSpeed = HealingSpeed->GetTextAsFloat();
	}
	else if (ID == LeftNum->GetID())
	{
		PathNode->LeftNum = LeftNum->GetTextAsInt();
	}
	else if (ID == RightNum->GetID())
	{
		PathNode->RightNum = RightNum->GetTextAsInt();
	}
	else if (ID == FollowTime->GetID())
	{
		PathNode->FollowTime = FollowTime->GetTextAsFloat();
	}
}
