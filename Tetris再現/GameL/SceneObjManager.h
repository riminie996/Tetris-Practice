#pragma once

//STL�f�o�b�O�@�\��OFF�ɂ���
#define _SECURE_SCL	(0)  
#define _HAS_ITERATOR_DEBUGGING	(0)  

#include <memory>
#include <list>
#include <algorithm>
#include <Windows.h>
//���������[�N���o�p
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

#define new DBG_NEW
#endif  // _DEBUG
using std::list;
using std::shared_ptr;

//using namespace std;

namespace GameL
{
	//�p�����ۗp�@�I�u�W�F�N�g�N���X
	class CObj
	{
		public:		
			CObj();
			virtual ~CObj(){};
			virtual void Init()=0;		//�C�j�V�����C�Y
			virtual void Action()=0;	//�A�N�V����
			virtual void Draw()=0;		//�h���[
			virtual void Delete() {}
			void		 SetPrio(unsigned int prio);	//�D�揇�ʕύX
			unsigned int GetPrio();
			void		 SetName(int name);				//�I�u�W�F�N�g���OID�ύX
			int			 GetName();
			void		 SetStatus(bool exe);			//�I�u�W�F�N�g�̏�ԕύX
			bool		 GetStatus();
			int			GetActTime();
			int m_act_time;
			int m_draw_time;
		private:
			unsigned int m_prio;		//�`��D�揇��
			int			 m_name;		//�I�u�W�F�N�g�̖��OID
			bool		 m_status;		//�I�u�W�F�N�g�̏��
	};

	//�I�u�W�F�N�g�}�l�[�W���[
	typedef class CSceneObjManager
	{
		public:
			static void Init();
			static void Delete();
			static void InsertObj(CObj* obj,int name,unsigned int prio);//�I�u�W�F�N�g�o�^
			static void ObjAction();			//�A�N�V�������s
			static void ObjDraw();				//�h���[���s
			static void ListDeleteSceneObj();	//�I�u�W�F�N�g���X�g�폜���߃��\�b�h
			static CObj* GetObj(int name);		//OBJ_NEME����I�u�W�F�N�g���擾
			static void ResetPrio();			//�D�揇�ʕύX
			static void ObjSubThrActionDraw();
		private:
			static list<shared_ptr<CObj>>* m_ListData;	//�Q�[�����s���X�g
	}Objs;
};
