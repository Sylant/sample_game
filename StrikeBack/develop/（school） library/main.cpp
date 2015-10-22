#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/counter.hpp"
#include <iostream>
#include <Windows.h>
#include "../../lib/gplib.h"
//3D�����O�����C�u����
#include "../../lib/dx_media_/dx_graphics3d.h"
#include "../../lib/dx_media_/dx3dmeshmanager.h"
//Xinput���C�u����
#include "../../lib/ci_ext/xinput.hpp" 
#include "logo.hpp"
#include "title.hpp"
#include "stage.h"
#include "effect.h"
#include "easing.h"

namespace gplib{
  namespace system{
    const char USERNAME[256] = "Strike Back v1.05";
    const int	WINW = 1040;									//16:10
    const int	WINH = 650;										//
    bool	WindowMode = true;							    	//�t���X�N���[���Ή�
    int		KeyboardMode = 0;									//pad disable:1 pad enable:0
  }
}
int ci_ext::Object::uid_ = 0;

//�Q�[���N���X
//���[�g�ɂȂ�N���X�ŁAmain�Ő錾����B
class Game : public ci_ext::Object
{
  int frame_;
public:
  Game(const std::string& name)
    :
    Object(name),
    frame_(0)
  {
  }

  //�R���X�g���N�^�ł͎�����weak_ptr���擾�ł��Ȃ��̂ŕʓr�|�C���^��o�^����֐����쐬
  void initGame(const std::weak_ptr<Game>& this_object_pointer)
  {
    //�܂��������g�̃|�C���^������
    setWeakPtr(this_object_pointer);

    //�ŏ��ɐ�������I�u�W�F�N�g���L�q
    insertAsChild(new game::Logo("logo"));
//	insertAsChild(new game::Title("title"));
//	insertAsChild(new game::Stage("stage"));
	}

  //�A�N�Z�T
  int frame() const { return frame_; }

  //����@
  void render() override {}
  void update() override { ++frame_; }
};

//�N���X�ɓ���ł��邪�A�����������Ȃ�̂ŃO���[�o���ɂ���
std::shared_ptr<Game> root;

//�G���g���|�C���g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
  return gplib::system::DoWindow(hInstance, hPreInst, lpszCmdLine, nCmdShow);
}

void gplib::system::GameLoop()
{
//  std::cout << "==" << root->frame() << "���" << std::endl;
  XInput::CheckKey();
  //���t���[���Ă�
  //�Q�c�`�揈��
	graph::Draw_Clear();
	root->updateWithChildren();
  effect::Step();
	root->renderWithChildren();
  effect::Draw();
	graph::Draw_Refresh();

  //�C�[�W���O�̍X�V
  easing::Step();
}
bool func()
{
  return true;
}

void gplib::system::OnCreate()
{

  //���[�g�I�u�W�F�N�g����
  root = std::make_shared<Game>("Root");
  root->initGame(root);

  //�G�t�F�N�g���[�h
  effect::Init();
  effect::LoadReses();

  //�t�H���g
  gplib::font::Draw_CreateFont(0, 30, "���C���I");
  gplib::font::Draw_CreateFont(1, 50, "���C���I");
  //�J����������
  gplib::camera::InitCamera(gplib::system::WINW / 2, gplib::system::WINH / 2, 0, 0);

}

void gplib::system::OnDestroy(){}