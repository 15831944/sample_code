#pragma once

#include "vgBaseEntry.h"
#include <vgKernel/vgkUniqueID.h>
#include <vgMesh/vgmMeshItem.h>
//*#include "vgUIController.h"*/
#include <vgKernel/vgkRendererObserver.h>
#include "vgPropertyPage.h"
#include <vgCoordinate.h>
#include <vgDatabaseEntry.h>

#include "vgDatabasePage.h"

class vgMeshEntry : public vgDatabaseEntry
{
public:
	vgMeshEntry( vgMesh::MeshItem* meshitem );

	virtual ~vgMeshEntry(void)
	{
		((vgMesh::MeshItem*)m_Renderer)->unregisterObserver( this );
	}



	int GetSelectedImage() { return 1; }

	int GetUnSelectedImage() { return 2; }

	// �̳���Changed������Object�޸ĺ����Node
	void onChanged(int eventId, void* param);

	void UpDataProp() {}

	CMenu* GetContextMenu();

	// ��ѡʱ����Tabҳ�棬��DatabaseTabPage
	virtual void AddSingleTabPage();

	// ��ѡʱ����Tabҳ��
	virtual void AddGroupTabPage();

public:

	bool _visible;

	// ��ѡʱ���޸����ݵĸ���
	float m_posX;
	float m_posY;
	float m_posZ;
	float m_posZMinus;

	float squareDistNear;
	float squareDistFar;

	string  strNumberOfTran;

	bool _isLodMesh;

protected:
	// ��д������PropertyChanged�������ֱ���single��Group��PropertyChanged
	virtual void OnGroupPropertyChanged(string paramName);

	virtual void OnSinglePropertyChanged(string paramName);

private:
	// �û��Զ��庯����ʵ��Mesh�ض���Group�༭����
	void OnGroupChanged(float sdx, float sdy, float sdz);

private:

	// group�༭������
	static float s_group_posX;
	static float s_group_posY;
	static float s_group_posZ;
	static float s_group_posZMinus;


	static float   s_group_squareDistNear;
	static float   s_group_squareDistFar;

	static string  s_group_item_name;

	static bool    s_group_onEdit;

	static bool s_isLodMesh;

	//��ɫ�༭
	COLORREF _colorRef;
	float m_fColorRatio;

};