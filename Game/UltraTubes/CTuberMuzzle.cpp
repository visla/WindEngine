/*
		File: CTuberMuzzle.cpp
		Author: Goran Bogdanovic
*/


#include <CTuberMuzzle.h>
#include <CTuber.h>
#include <WeLight.h>


CTuberMuzzle::CTuberMuzzle(CTuber *MyOwner, int MyID, WeMatrix &RelMatrix)
{
	ID = MyID;
	Owner = MyOwner;
	RelativeTransMatrix = RelMatrix;
	Owner->GetWorld()->AddActor(this);
	
	static WeLight *StaticLight = NULL;
	if (!StaticLight)
	{
		StaticLight = new WeLight();
		WeLightState ls1, ls2;
		ls1.Range = 20.0f;
		ls1.Diffuse = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
		ls2.Range = 0.0f;
		StaticLight->AddKeyState(ls1, 0.02f);
		StaticLight->AddKeyState(ls2, 0.02f);
		StaticLight->IsDynamic = true;
		StaticLight->AffectDynamicActors = true;
	}

	if (MyID == 0)
	{
		Light = new WeLight();
		StaticLight->Clone(Light);
		Light->Location = Location;
		Owner->GetWorld()->AddActor(Light);
	}
	else
        Light = NULL;
}

CTuberMuzzle::~CTuberMuzzle()
{
	if (Light)
		Light->LifeTime = 0.0001f;
	Owner->Muzzle[ID] = NULL;
	Owner = NULL;
}

void CTuberMuzzle::Tick()
{
	WeMatrix MyMatrix, OwnerMatrix;
	Owner->GetTransformation(OwnerMatrix);

	D3DXMatrixMultiply(&MyMatrix, &RelativeTransMatrix, &OwnerMatrix);
	SetTransformation(MyMatrix);

	if (Light)
		Light->Location = Location;

	WeActor::Tick();
}
