#pragma once
#include <d3d11.h>
#include "string"

class Texture
{

public:
	Texture();
	~Texture();
	HRESULT Load(LPCWSTR fileName);
	void Release();
	HRESULT Initialize();
	ID3D11SamplerState* GetSampler() { return pSampler_; }
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }
private:
	ID3D11SamplerState* pSampler_;
	ID3D11ShaderResourceView* pSRV_;

};