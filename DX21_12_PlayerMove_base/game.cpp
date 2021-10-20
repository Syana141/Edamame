//*****************************************************************************
// DX21�@�T���v���\�[�X
// �Q�[���̃��C���ƂȂ鏈���������Ă����\�[�X�t�@�C��  �ِ��E�]��
//*****************************************************************************

#include "game.h"
#include "direct3d.h"
#include "GameTimer.h"
#include "input.h"
#include "GameObject.h"
#include "ObjectGenerator.h"
#include "Sprite.h"
#include <math.h>

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

// ���_�P�������\���\����
struct VERTEX_POSTEX {
	float x, y, z;  // ���_�̈ʒu

	float u, v;  // �e�N�X�`����UV���W
};


//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define MAX_SPRITE  64
// �|���S���Q�łP�̎l�p�`�i�X�v���C�g�j�B�|���S���͂R���_�Ȃ̂ŁA�P�X�v���C�g�͂U���_�B
#define VERTEX_PER_SPRITE  (3*2)
#define VERTEX_BUFFER_SIZE  (MAX_SPRITE*sizeof(VERTEX_POSTEX)*VERTEX_PER_SPRITE)


// �h���S���̔�����
#define MAX_OBJECT   4

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

ID3D11Buffer* gpVertexBuffer;  // ���_�o�b�t�@�p�̕ϐ�

ID3D11ShaderResourceView* gpTexture; // �e�N�X�`���p�ϐ�

GameObject gObjects[MAX_OBJECT];  // �I�u�W�F�N�g�z��
GameObject* gpPlayer = gObjects + 1;

//�e�N�X�`���B
Sprite* texter;
Sprite* texter1;

//*****************************************************************************
// �֐��̒�`�@��������@��
//*****************************************************************************

BOOL Game_Initialize()
{
	HRESULT hr;

	// ���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = VERTEX_BUFFER_SIZE;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = Direct3D_GetDevice()->CreateBuffer(&bufferDesc, NULL, &gpVertexBuffer);

	if (FAILED(hr))
		return FALSE;
	// ���@���_�o�b�t�@�쐬�@�����܂�

	// �����Ŏg�p�������摜�t�@�C�����e�N�X�`���Ƃ��ēǂݍ���
	// �������F�摜�t�@�C�����B�����t�H���_�ɓ����Ă���Ȃ�t�H���_�����ꏏ�ɏ����B
	// �������F�ǂݍ��񂾃e�N�X�`��������ϐ����w��
	hr = Direct3D_LoadTexture("assets/texture.png", &gpTexture);

	if (FAILED(hr)) {
		return FALSE;
	}

	// �Q�[�����Ԃ̏����������AFPS��60�ɐݒ肵���B
	GameTimer_Initialize(60);
	
	// �z��O�Ԃɔw�i�X�v���C�g���Z�b�g����
	ObjectGenerator_SetBG(&gObjects[0]);

	// �v���C���[�i�z��P�ԁj�ɗE�҃X�v���C�g���Z�b�g����
	ObjectGenerator_Character32x32(gpPlayer, 0);
	gpPlayer->posX = -0.3f;
	gpPlayer->posY = -0.5f;

	// �z��Q�ԂɃS�[�X�g�X�v���C�g���Z�b�g����
	ObjectGenerator_Character32x32(&gObjects[2], 1);
	gObjects[2].posX =  0.3f;
	gObjects[2].posY = -0.5f;

	texter = new Sprite("assets/dora01.png", 3, 4);
	texter->SetSize(1, 1);
	texter->SetPart(1, 1);

	texter1 = new Sprite("assets/char02.png", 3, 4);
	texter1->SetSize(1, 1);
	texter1->SetPart(1, 1);
	return TRUE;
}


// �Q�[�����[�v���Ɏ��s�������A�Q�[���̏����������֐�
void Game_Update()
{
	Input_Update();  // ���̃Q�[���Ŏg���L�[�̉�����Ԃ𒲂ׂĕۑ�

	CharController* pPlayerCtrl = &gpPlayer->charController;

	if (Input_GetKeyPress(VK_LEFT)) {  // ��
		CharController_Accelerate(pPlayerCtrl, ACCEL_LEFT);
		pPlayerCtrl->direction = DIR_LEFT;
	}
	if (Input_GetKeyPress(VK_RIGHT)) {  // �E
		CharController_Accelerate(pPlayerCtrl, ACCEL_RIGHT);
		pPlayerCtrl->direction = DIR_RIGHT;
	}

	gpPlayer->animator.speed = fabsf(pPlayerCtrl->moveSpeedX / pPlayerCtrl->maxMoveSpeedX * 20.0f);

	CharController* pEnemyCtrl = &gObjects[2].charController;

	if (Input_GetKeyPress('A')) {  // ��
		CharController_Accelerate(pEnemyCtrl, ACCEL_LEFT);
		pEnemyCtrl->direction = DIR_LEFT;
	}
	if (Input_GetKeyPress('D')) {  // �E
		CharController_Accelerate(pEnemyCtrl, ACCEL_RIGHT);
		pEnemyCtrl->direction = DIR_RIGHT;
	}

	// �����Ă����Animator��speed�Ƀv���X�̒l������A�����ĂȂ���΂O������
	gObjects[2].animator.speed = 1;

	// �|���S���̒��_���`
	// ���_������Ń|���S�����`������Ƃ��̖@��
	// �E���_�́A�z��̔ԍ��̎Ⴂ���ԂɎg�p�����
	// �E���_������Ń|���S�����`�����ꂽ�Ƃ��A���̌��я������v���ɂȂ�ꍇ�����|���S�����\�������
	VERTEX_POSTEX vx[MAX_SPRITE*VERTEX_PER_SPRITE];

	// �I�u�W�F�N�g�z���XY�v�Z�AUV�v�Z�A���_�z��ւ̓K�p���ꊇ����
	for (int i = 0; i < MAX_OBJECT; i++) {
		
		GameObject_Update(&gObjects[i]);
		// XY�v�Z
		FRECT xy = GameObject_GetXY( &gObjects[i] );
		// UV�v�Z
		FRECT uv = GameObject_GetUV( gObjects+i );
		// ���_�z��ւ̓K�p
		vx[0 + i * 6 + 0] = { xy.left, xy.top, 0, uv.left, uv.top },  // �P�߂̒��_   �@����
		vx[0 + i * 6 + 1] = { xy.right, xy.top, 0, uv.right, uv.top },  // �Q�߂̒��_�@�A�E��
		vx[0 + i * 6 + 2] = { xy.right,  xy.bottom, 0, uv.right, uv.bottom },  // �R�߂̒��_�@�B�E��

		vx[0 + i * 6 + 3] = { xy.right,  xy.bottom, 0, uv.right, uv.bottom },  // �S�߂̒��_�@�B�E��
		vx[0 + i * 6 + 4] = { xy.left,  xy.bottom, 0, uv.left, uv.bottom },  // �T�߂̒��_�@�C����
		vx[0 + i * 6 + 5] = { xy.left,  xy.top, 0, uv.left, uv.top };  // �U�߂̒��_�@�@  �@����@�@  �@����
	}

	// ���_�o�b�t�@�̃f�[�^��vx�z��̃f�[�^�ōX�V����
	Direct3D_GetContext()->UpdateSubresource(
		gpVertexBuffer,		// �X�V�Ώۂ̃o�b�t�@
		0,				// �C���f�b�N�X(0)
		NULL,				// �X�V�͈�(nullptr)
		vx,		// ���f�f�[�^
		0,				// �f�[�^��1�s�̃T�C�Y(0)
		0);				// 1�[�x�X���C�X�̃T�C�Y(0)
}


void Game_Draw()
{
	// ��ʃN���A�i�w��F�œh��Ԃ��j
	// �h��Ԃ��F�̎w��i�����F0.0f�`1.0f�j
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	Direct3D_GetContext()->ClearRenderTargetView(Direct3D_GetRenderTargetView(), clearColor);

	// ���@���O�̕`�揈���������ɏ��� *******

	// �`��Ŏg�p����e�N�X�`�����w�肷��֐��Ăяo��
	// �����Ŏw�肳�ꂽ�e�N�X�`�����A�s�N�Z���V�F�[�_�[�̃O���[�o���ϐ��ɃZ�b�g�����
	Direct3D_GetContext()->PSSetShaderResources(0, 1, &gpTexture);

	// �`��Ɏg�����_�o�b�t�@���w�肷��
	UINT strides = sizeof(VERTEX_POSTEX);
	UINT offsets = 0;
	Direct3D_GetContext()->IASetVertexBuffers(0, 1, &gpVertexBuffer, &strides, &offsets);   // gpVertexBuffer�����_�o�b�t�@

	// ����܂ł̐ݒ�Ŏ��ۂɕ`�悷��
	Direct3D_GetContext()->Draw(MAX_SPRITE*VERTEX_PER_SPRITE, 0);

	texter->Draw();
	texter1->Draw();
	// ���@���O�̕`�揈���������ɏ��� *******

	// �_�u���E�o�b�t�@�̃f�B�X�v���C�̈�ւ̃R�s�[����
	Direct3D_GetSwapChain()->Present(0, 0);
}

void Game_Relese()
{
	COM_SAFE_RELEASE(gpTexture);  // �e�N�X�`����ǂݍ��񂾂�A�Y�ꂸ�����[�X���邱��
	COM_SAFE_RELEASE(gpVertexBuffer); // ���_�o�b�t�@���쐬������A�Y�ꂸ�Ƀ����[�X���邱��
}

