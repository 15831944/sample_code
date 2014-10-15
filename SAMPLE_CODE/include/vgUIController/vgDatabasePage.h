#ifndef VG_DATABASEPAGE_H
#define VG_DATABASEPAGE_H

#include <vgUIController/vgPropertyPage.h>
#include <vgEntry/vgBaseEntry.h>

typedef struct DatabaseProperties_t
{
	// unsigned int	dataType;			// �����������
	string			label;				// ��������ʾ����
	string			recordLable;			// �������ڱ��е��ֶ���
	_variant_t		data;
	CBCGPProp*		pProp;				// ���ɵ����Զ���ָ��
} DatabaseProperties;


class  VGUI_EXPORT vgDatabasePage : public vgPropertyPage
{
public:

	void ConnectNode(vgBaseEntry* node, std::vector<DatabaseProperties>& propParamList);
	
	virtual LRESULT OnPropertyChanged(WPARAM w, LPARAM l);

};

#endif

