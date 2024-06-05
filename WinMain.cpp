//�C���N���[�h
#include <Windows.h>
#include <tchar.h>
#include "Direct3D.h"
#include "Quad.h"
#include "Camera.h"

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//�萔�錾
const wchar_t* WIN_CLASS_NAME = L"SampleGame";  //�E�B���h�E�N���X��
const wchar_t* APP_NAME = L"�T���v���Q�[��";
const int WINDOW_WIDTH = 800;  //�E�B���h�E�̕�
const int WINDOW_HEIGHT = 600; //�E�B���h�E�̍���

//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//�G���g���[�|�C���g
//�A�v���P�[�V�����@�v���O���~���O�@�C���^�[�t�F�[�X = api
//���炩�̃A�v���P�[�V���������ۂɕ֗��@�\���܂Ƃ߂�����
//�\�t�g�E�F�A�@�f�x���b�v�����g�@�L�b�g = sdk
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	//�E�B���h�E�N���X�i�݌v�}�j���쐬
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);             //���̍\���̂̃T�C�Y
	wc.hInstance = hInstance;                   //�C���X�^���X�n���h��
	wc.lpszClassName = WIN_CLASS_NAME;            //�E�B���h�E�N���X��
	wc.lpfnWndProc = WndProc;                   //�E�B���h�E�v���V�[�W��
	wc.style = CS_VREDRAW | CS_HREDRAW;         //�X�^�C���i�f�t�H���g�j
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //�A�C�R��
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //�������A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //�}�E�X�J�[�\��
	wc.lpszMenuName = NULL;                     //���j���[�i�Ȃ��j
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //�w�i�i���j
	RegisterClassEx(&wc); //�N���X��o�^

	//�E�B���h�E�T�C�Y�̌v�Z(+�i�\���̈��WINDOW_WIDT*WINDOW_HEIGHT
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;     //�E�B���h�E��
	int winH = winRect.bottom - winRect.top;     //�E�B���h�E����

	//�E�B���h�E���쐬
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //�E�B���h�E�N���X��
		APP_NAME,     //�^�C�g���o�[�ɕ\��������e
		WS_OVERLAPPEDWINDOW, //�X�^�C���i���ʂ̃E�B���h�E�j
		CW_USEDEFAULT,       //�\���ʒu���i���܂����j
		CW_USEDEFAULT,       //�\���ʒu��i���܂����j
		winW,                //�E�B���h�E��
		winH,                //�E�B���h�E����
		NULL,                //�e�E�C���h�E�i�Ȃ��j
		NULL,                //���j���[�i�Ȃ��j
		hInstance,           //�C���X�^���X
		NULL                 //�p�����[�^�i�Ȃ��j
	);

	//�E�B���h�E��\��
	ShowWindow(hWnd, nCmdShow);

	//Direct3D������
	Direct3D::Initialize(winW, winH, hWnd);

	Camera::Initialize();

	Quad* q;
	q = new Quad();
	q->Initialize();
	//if (FAILED(hr))
	//{
	//	return 0;
	//}

	//���b�Z�[�W���[�v�i�����N����̂�҂j
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//���b�Z�[�W����
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//���b�Z�[�W�Ȃ�
		else
		{
			//�Q�[���̏���
			Camera::Update();
			Direct3D::BeginDraw();

			//1�x����]����ϐ�
			//static float rot = 0;
			//rot+= 0.5;
			//XMMATRIX rmat = XMMatrixRotationY(XMConvertToRadians((float)rot));
			static float factor = 0.0;
			factor += 0.01;
			//float scale = 1.5 + sin(factor);
			//XMMATRIX smat = XMMatrixScaling(scale, scale, scale);
			////���O�̕`�揈����ǉ����Ă���
			//XMMATRIX tmat = XMMatrixTranslation(2.0 * sin(factor), 0, 0);
			XMMATRIX tmat = XMMatrixTranslation(3.0*cos(factor), 3.0*sin(factor), 0);
			//XMMATRIX mat = rmat * smat * tmat;
			//�P�ʍs��́A������1�Ɠ���
			XMMATRIX mat = XMMatrixIdentity();//Identity�͒P�ʍs��Ƃ����Ӗ�
			mat = tmat;
			q->Draw(mat);

			//�`�揈��
			Direct3D::EndDraw();

		}
	}
	//SAFE_DELETE(quad);
	/*q->Release();*/
	Direct3D::Release();
	
	return 0;
}

//�E�B���h�E�v���V�[�W���i�������������ɂ�΂��֐��j
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);  //�v���O�����I��
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
