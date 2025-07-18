#pragma once
#include "Quad.h"
class Dice :public Quad
{
private:
	struct QuadData
	{
		XMFLOAT3 position;
		XMFLOAT3 rotation; // Pitch, Yaw, RollÅiìxêîÅj
		XMFLOAT2 scale;
		QuadData(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT2 scl)
			: position(pos), rotation(rot), scale(scl) {
		}
	};

public:
	Dice();
	~Dice();
	HRESULT Initialize()override;
	void Draw(XMMATRIX& worldMatrix, XMMATRIX& uvMatrix) override;
};


