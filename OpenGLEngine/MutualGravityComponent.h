#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct MutualGravityComponent
	{
		MutualGravityComponent(float _gravityVal = 10, 
			ECSEntity _connectedEntity1 = ECSEntity(),
			ECSEntity _connectedEntity2 = ECSEntity(),
			ECSEntity _connectedEntity3 = ECSEntity(), 
			ECSEntity _connectedEntity4 = ECSEntity(), 
			ECSEntity _connectedEntity5 = ECSEntity(), 
			ECSEntity _connectedEntity6 = ECSEntity(), 
			ECSEntity _connectedEntity7 = ECSEntity(), 
			ECSEntity _connectedEntity8 = ECSEntity(), 
			ECSEntity _connectedEntity9 = ECSEntity(), 
			ECSEntity _connectedEntity10 = ECSEntity())
			:gravityVal(_gravityVal), 
			connectedEntity1(_connectedEntity1), connectedEntity2(_connectedEntity2), connectedEntity3(_connectedEntity3),
			connectedEntity4(_connectedEntity4), connectedEntity5(_connectedEntity5), connectedEntity6(_connectedEntity6),
			connectedEntity7(_connectedEntity7), connectedEntity8(_connectedEntity8), connectedEntity9(_connectedEntity9),
			connectedEntity10(_connectedEntity10)
		{

		}
		float gravityVal;
		ECSEntity connectedEntity1;
		ECSEntity connectedEntity2;
		ECSEntity connectedEntity3;
		ECSEntity connectedEntity4;
		ECSEntity connectedEntity5;
		ECSEntity connectedEntity6;
		ECSEntity connectedEntity7;
		ECSEntity connectedEntity8;
		ECSEntity connectedEntity9;
		ECSEntity connectedEntity10;
	};
}
