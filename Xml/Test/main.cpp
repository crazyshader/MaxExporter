#include <iostream>
#include "..\Xml\Xml.h"

#pragma comment(lib, "Xml_d.lib")

using namespace std;

// 遍历结点
bool PrintNode( CXmlSubNode& node )
{
	// 打印节点名
	cout << node.GetName() << endl;

	CXmlSubNode child;
	CXmlNode nodes;

	// 是否有子节点
	if( node.HasChildren() )
	{
		// 获取节点集合
		nodes = node.GetChildren();
		for(unsigned int i = 0; i < nodes.GetCount(); i++ )
		{
			// 获取子节点
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

	// XML文件是否存在
	if (!CXmlDoc::IsFileExist("Test.XML"))
	{
		// 创建XML文件
		if( !xmlDoc.Create( "Test.XML" ) )
		{
			MessageBoxA( NULL, xmlDoc.GetLastError().c_str(), "Create XML file failed!", MB_OK );
			return 1;
		}

		// 增加根节点
		pRootNode = xmlDoc.AddRoot("SceneInfo");

		// 设置属性
 		pRootNode->SetAttribute("FileName", "HuiTaiLang.max");
		// 增加一个子节点和属性
		pRootNode->AddChild("MaterialList")->SetAttribute("Count", "2");
		pRootNode->GetChild("MaterialList")->AddChild("Material")->SetAttribute("index", "1");
		pRootNode->GetChild("MaterialList")->AddChild("Material")->SetAttribute("index", "2");

		// 保存XML文件
		xmlDoc.Save();
		xmlDoc.Close();
	}

	// 打开XML文件
	if( !xmlDoc.Open( "Test.XML" ) )
	{
		MessageBoxA( NULL, xmlDoc.GetLastError().c_str(), "Open XML file failed!", MB_OK );
		return 1;
	}

	// 获取根节点
	pRootNode = xmlDoc.GetRoot("SceneInfo");
	// 获取节点数量
	cout << "Root Node Num:" << pRootNode->GetChildren()->GetCount() << endl;
	// 获取节点名和属性
	cout << pRootNode->GetName() << "\t" << pRootNode->GetAttribute("FileName") << endl;
	// 设置节点属性
	pRootNode->GetChild("MaterialList")->SetAttribute("Count", "two") ;
	cout << pRootNode->GetChild("MaterialList")->GetName() << "\t" <<  pRootNode->GetChild("MaterialList")->GetAttribute("Count") << endl;
	// 增加子节点和设置节点值
	pRootNode->GetChild("MaterialList")->AddChild("Test")->SetString("wujun");
	// 获取节点值
	cout <<  pRootNode->GetChild("MaterialList")->GetChild("Test")->GetString() << endl;
	// 移除当前节点
	xmlDoc.GetSubNode("/SceneInfo/MaterialList/Test")->Remove();

	// 打印所有节点
	PrintNode(*pRootNode);

	// 获取节点
	CXmlSubNode* pNode = xmlDoc.GetSubNode("/SceneInfo/MaterialList");
	cout << pNode->GetName() << "\t" << pNode->GetAttribute("Count") << endl;

	// 获取节点集合
	CXmlNode nodes;
	nodes = xmlDoc.GetNode( "/SceneInfo/MaterialList/*" );

	// 打印节点集合
	for(unsigned int k = 0; k < nodes.GetCount(); k++ )
	{
		cout << nodes[k]->GetName() << endl;
	}

	// 获取子节点
	cout << nodes["Material"]->GetName() << "\t" <<  nodes["Material"]->GetAttribute("index") << endl;

	xmlDoc.Close();
	getchar();

	return 0;
}
