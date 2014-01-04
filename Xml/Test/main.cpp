#include <iostream>
#include "..\Xml\Xml.h"

#pragma comment(lib, "Xml_d.lib")

using namespace std;

// �������
bool PrintNode( CXmlSubNode& node )
{
	// ��ӡ�ڵ���
	cout << node.GetName() << endl;

	CXmlSubNode child;
	CXmlNode nodes;

	// �Ƿ����ӽڵ�
	if( node.HasChildren() )
	{
		// ��ȡ�ڵ㼯��
		nodes = node.GetChildren();
		for(unsigned int i = 0; i < nodes.GetCount(); i++ )
		{
			// ��ȡ�ӽڵ�
			child = nodes.GetItem(i); 
			PrintNode( child );
		}
	}

	return true;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	CXmlDoc xmlDoc;
	CXmlSubNode* pRootNode = NULL;

	// XML�ļ��Ƿ����
	if (!CXmlDoc::IsFileExist("Test.XML"))
	{
		// ����XML�ļ�
		if( !xmlDoc.Create( "Test.XML" ) )
		{
			MessageBoxA( NULL, xmlDoc.GetLastError().c_str(), "Create XML file failed!", MB_OK );
			return 1;
		}

		// ���Ӹ��ڵ�
		pRootNode = xmlDoc.AddRoot("SceneInfo");

		// ��������
 		pRootNode->SetAttribute("FileName", "HuiTaiLang.max");
		// ����һ���ӽڵ������
		pRootNode->AddChild("MaterialList")->SetAttribute("Count", "2");
		pRootNode->GetChild("MaterialList")->AddChild("Material")->SetAttribute("index", "1");
		pRootNode->GetChild("MaterialList")->AddChild("Material")->SetAttribute("index", "2");

		// ����XML�ļ�
		xmlDoc.Save();
		xmlDoc.Close();
	}

	// ��XML�ļ�
	if( !xmlDoc.Open( "Test.XML" ) )
	{
		MessageBoxA( NULL, xmlDoc.GetLastError().c_str(), "Open XML file failed!", MB_OK );
		return 1;
	}

	// ��ȡ���ڵ�
	pRootNode = xmlDoc.GetRoot("SceneInfo");
	// ��ȡ�ڵ�����
	cout << "Root Node Num:" << pRootNode->GetChildren()->GetCount() << endl;
	// ��ȡ�ڵ���������
	cout << pRootNode->GetName() << "\t" << pRootNode->GetAttribute("FileName") << endl;
	// ���ýڵ�����
	pRootNode->GetChild("MaterialList")->SetAttribute("Count", "two") ;
	cout << pRootNode->GetChild("MaterialList")->GetName() << "\t" <<  pRootNode->GetChild("MaterialList")->GetAttribute("Count") << endl;
	// �����ӽڵ�����ýڵ�ֵ
	pRootNode->GetChild("MaterialList")->AddChild("Test")->SetString("wujun");
	// ��ȡ�ڵ�ֵ
	cout <<  pRootNode->GetChild("MaterialList")->GetChild("Test")->GetString() << endl;
	// �Ƴ���ǰ�ڵ�
	xmlDoc.GetSubNode("/SceneInfo/MaterialList/Test")->Remove();

	// ��ӡ���нڵ�
	PrintNode(*pRootNode);

	// ��ȡ�ڵ�
	CXmlSubNode* pNode = xmlDoc.GetSubNode("/SceneInfo/MaterialList");
	cout << pNode->GetName() << "\t" << pNode->GetAttribute("Count") << endl;

	// ��ȡ�ڵ㼯��
	CXmlNode nodes;
	nodes = xmlDoc.GetNode( "/SceneInfo/MaterialList/*" );

	// ��ӡ�ڵ㼯��
	for(unsigned int k = 0; k < nodes.GetCount(); k++ )
	{
		cout << nodes[k]->GetName() << endl;
	}

	// ��ȡ�ӽڵ�
	cout << nodes["Material"]->GetName() << "\t" <<  nodes["Material"]->GetAttribute("index") << endl;

	xmlDoc.Close();
	getchar();

	return 0;
}
