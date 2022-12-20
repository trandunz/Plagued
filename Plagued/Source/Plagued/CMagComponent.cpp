#include "CMagComponent.h"

int8 UCMagComponent::AddAmmo(int8 _amount)
{
	for(int i = _amount; CurrentAmmo < MaxAmmo; i--)
	{
		CurrentAmmo++;
		_amount--;
	}
	return _amount;
}

void UCMagComponent::FillMag()
{
	CurrentAmmo = MaxAmmo;
}

void UCMagComponent::EjectRound()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
	}
}

bool UCMagComponent::IsEmpty()
{
	return CurrentAmmo <= 0;
}
