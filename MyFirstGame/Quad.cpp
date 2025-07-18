#include "Quad.h"
#include "Camera.h"
#include "Texture.h"
#include <DirectXMath.h>
#include <cassert>





Quad::Quad() : pVertexBuffer_(nullptr)
, pIndexBuffer_(nullptr)
, pConstantBuffer_(nullptr), pTexture_(nullptr), pRasterState_(nullptr)
{

}

Quad::~Quad()
{
}

HRESULT Quad::Initialize()
{
	// ���_���
	VERTEX vertices[] =
	{
		{ XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) }, // ����
		{ XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) }, // �E��
		{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) }, // �E��
		{ XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) }  // ����
	};

	// ���_�o�b�t�@
	D3D11_BUFFER_DESC bd_vertex = {};
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data_vertex = {};
	data_vertex.pSysMem = vertices;

	HRESULT hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"���_�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return E_FAIL;
	}

	// �C���f�b�N�X�o�b�t�@
	int indices[] = { 0, 2, 3, 0, 1, 2 };
	D3D11_BUFFER_DESC bd_index = {};
	bd_index.Usage = D3D11_USAGE_DEFAULT;
	bd_index.ByteWidth = sizeof(indices);
	bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data_index = {};
	data_index.pSysMem = indices;

	hr = Direct3D::pDevice->CreateBuffer(&bd_index, &data_index, &pIndexBuffer_);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return E_FAIL;
	}

	// �R���X�^���g�o�b�t�@
	D3D11_BUFFER_DESC cb = {};
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return E_FAIL;
	}

	// ���X�^���C�U�[�X�e�[�g
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;  // �J�����O����
	rasterDesc.FrontCounterClockwise = TRUE;
	rasterDesc.DepthClipEnable = TRUE;

	hr = Direct3D::pDevice->CreateRasterizerState(&rasterDesc, &pRasterState_);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"���X�^���C�U�[�X�e�[�g�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return E_FAIL;
	}

	// �e�N�X�`��
	pTexture_ = new Texture;
	if (FAILED(pTexture_->Load(L"Assets\\Dice.png"))) {
		MessageBox(nullptr, L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", L"�G���[", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void Quad::Draw(XMMATRIX& worldMatrix, XMMATRIX& uvMatrix)
{
	// ���X�^���C�U�[�X�e�[�g���Z�b�g
	Direct3D::pContext->RSSetState(pRasterState_);

	CONSTANT_BUFFER cb = {};
	cb.matWVP = XMMatrixTranspose(worldMatrix * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matUV = XMMatrixTranspose(uvMatrix); // UV�}�g���b�N�X�͒P�ʍs��
	cb.matW = XMMatrixTranspose(worldMatrix);

	D3D11_MAPPED_SUBRESOURCE pdata = {};
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy(pdata.pData, &cb, sizeof(cb));
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);

	// �T���v���[��SRV���Z�b�g
	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	// ���_�E�C���f�b�N�X�o�b�t�@
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	// �R���X�^���g�o�b�t�@
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);

	// �`��
	Direct3D::pContext->DrawIndexed(6, 0, 0);
}

void Quad::Release()
{

	pTexture_->Release();
	SAFE_DELETE(pTexture_);


	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
	SAFE_RELEASE(pRasterState_);

	/*pVertexBuffer_->Release();
	pIndexBuffer_->Release();
	pConstantBuffer_->Release();*/
}
