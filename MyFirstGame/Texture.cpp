#include "Texture.h"
#include "Direct3D.h"
#include "Quad.h"
#include <DirectXTex.h>
#include <wincodec.h>
#pragma comment(lib, "DirectXTex.lib")

using namespace DirectX;
Texture::Texture()
	: pSampler_(nullptr), pSRV_(nullptr)
{
}

Texture::~Texture()
{
}

HRESULT Texture::Load(LPCWSTR fileName)
{
	TexMetadata metadata; //�摜�̕t�����

	ScratchImage image;   //�摜�{��

	HRESULT hr;

	//���ۂɓǂ�ł䂭���@�@�@�@�@ 

	hr = LoadFromWICFile(fileName, WIC_FLAGS::WIC_FLAGS_NONE,

		&metadata, image);



	if (FAILED(hr))

	{

		return S_FALSE;

	}

	D3D11_SAMPLER_DESC SamDesc{};
	// MEMO: ���`��Ԃ��n�[�h�E�F�A�ł���Ă����炵��
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	// MEMO: 0~1�͈̔͂��o���ꍇ�ǂ����邩
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = Direct3D::pDevice->CreateSamplerState(&SamDesc, &pSampler_);

	if (FAILED(hr))
	{
		return hr;
	}

	// �V�F�[�_�[���\�[�X�r���[
	D3D11_SHADER_RESOURCE_VIEW_DESC srv{};

	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// MEMO: ������3D�̃e�N�X�`�����w��ł���
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;  // LOD�̃~�b�v�}�b�v���x��


	hr = CreateShaderResourceView(
		Direct3D::pDevice,
		image.GetImages(),
		image.GetImageCount(),
		metadata,
		&pSRV_);

	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}
void Texture::Release()
{

	pSampler_->Release();


	pSRV_->Release();

}


