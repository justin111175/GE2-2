#include "TmxObj.h"
#include <Dxlib.h>
#include <sstream>
#include "../common/ImageMng.h"
#include "../InterNet/NetWork.h"
#include "../common/_debug/_DebugConOut.h"
#include <vector>
#include <bitset>
TmxObj::TmxObj()
{




	LoadTsx("map/map.tsx");

	IpImageMng.GetID(tileset.name,"image/map.png", { tileset.tileWidth,tileset.tileHeight }, { tileset.columns,tileset.tilecount / tileset.columns });

}

TmxObj::~TmxObj()
{
}

void TmxObj::LoadTmx(const char* tmx)
{


	rapidxml::xml_document<> doc;
	rapidxml::file<> input(tmx);
	doc.parse<0>(input.data());

	rapidxml::xml_node<>* node = doc.first_node("map");


	for (rapidxml::xml_attribute<>* atb = node->first_attribute(); atb != nullptr; atb = atb->next_attribute())
	{
		mapData_.try_emplace(atb->name(), atb->value());
		//std::cout << atb->name()<<"  :"<<atb->value() << std::endl;
		//std::cout << /*atb->name("width") << "  :" <<*/ atb->next_attribute("width")->value() << std::endl;
		
	}
	tileset.width = std::atoi(node->first_attribute("width")->value());
	tileset.height = std::atoi(node->first_attribute("height")->value());


	for (rapidxml::xml_node<>* layer_node = node->first_node("layer"); layer_node != nullptr; layer_node = layer_node->next_sibling("layer"))
	{

		rapidxml::xml_node<>* data_node = layer_node->first_node("data");
		layerData_.emplace(layer_node->first_attribute("id")->value(), data_node->value());
		//layerData_[layer_node->first_attribute("name")->value()]
		//layer_.try_emplace(layerData_[layer_node->first_attribute("name")], StringChange(layer_node->first_attribute("name")->value()));
	}


	for (auto i : layerData_)
	{

		layer_.try_emplace(i.first, StringChange(layerData_[i.first]));



	}

}

void TmxObj::LoadTsx(const char* tsx)
{
	rapidxml::xml_node<>* root_node;
	rapidxml::xml_document<> doc;
	rapidxml::file<> file(tsx);
	doc.parse<0>(file.data());
	
	root_node = doc.first_node("tileset");

	tileset.name = root_node->first_attribute("name")->value();
	std::cout << "name : "<<tileset.name << std::endl;

	tileset.tileWidth = std::atoi(root_node->first_attribute("tilewidth")->value());
	std::cout << "tilewidth : " << tileset.tileWidth << std::endl;
	tileset.tileHeight = std::atoi(root_node->first_attribute("tileheight")->value());
	std::cout << "tileheight : " << tileset.tileHeight << std::endl;

	tileset.tilecount = std::atoi(root_node->first_attribute("tilecount")->value());
	std::cout << "tilecount : " << tileset.tilecount << std::endl;
	
	tileset.columns = std::atoi(root_node->first_attribute("columns")->value());
	std::cout << "columns : " << tileset.columns << std::endl;

	tileset.source = root_node->first_node("image")->first_attribute("source")->value();
	std::cout << "source : " << tileset.source << std::endl;






}

void TmxObj::Draw()
{
	for (auto test : layerData_)
	{
		for (int i = 0; i < tileset.width * tileset.height; i++)
		{
			
			if (layer_[test.first][i] - 1 != -1)
			{
				DrawGraph(tileset.tileWidth * (i % tileset.width), tileset.tileHeight * (i / tileset.width), IMAGE_ID("map")[layer_[test.first][i]-1], true);

			}
			
		}
	}
}

bool TmxObj::SendSize(std::string file)
{
	std::ifstream ifp(file);
	ifp.seekg(0, std::ios_base::end);


	MesData testMes = { MesType::TMX_SIZE };
	testMes.data[0] = 200;

	IpNetwork.SendMes(testMes);
	//TRACE("%d\n",)

	return true;


	//reinterpret_cast<int*>(&ifp.tellg()), sizeof(int)



}



void TmxObj::LayerInit(std::string file)
{
	const char* fileName = file.c_str();


	FILE* fp;

	//fp = fopen(file, "r");
	//fopen_s(&fp, file.c_str(), "r");

	//if (fp == NULL)
	//{
	//	printf("%sのオープンに失敗しました。\n", file);
	//	printf("Enterキーで終了。\n");
	//	getchar();
	//}

//	std::cout << fp << std::endl;

	//fclose(fp);
}

void TmxObj::LayerData(std::string string)
{


	LoadTmx(string.c_str());

	for (auto i : layer_)
	{
		for (int q = 0; q < 21*17; q++)
		{
			if (q % 21==20)
			{
				std::cout << layer_[i.first][q]<<std::endl;
			}
			else
			{
				std::cout << layer_[i.first][q]<<",";
			}
			

		}

	}
	//return;
	//std::ofstream ofp("map/testLayer.tmx");

	//std::string data;
	//					
	//
	//
	//
	//std::stringstream streamSTR;
	//std::string test;
	//auto IntChange = [&]() {
	//

	//	std::getline(streamSTR, test, ',');

	//	return atoi(test.c_str());

	//
	//
	//};



	//while (!ifp.eof())
	//{
	//	while (std::getline(ifp, data))
	//	{
	//		if (data.find("data encoding") != std::string::npos)
	//		{
	//			std::getline(ifp, data);
	//			int i = 0;
	//			//streamSTR << data;

	//			while (data.find("/data") == std::string::npos)
	//			{
	//				streamSTR << data;
	//				
	//				//for (int i = 0; i < 21; i++)
	//				{
	//					std::cout<<IntChange();

	//				}

	//				//ofp<< IntChange(data);
	//				

	//				//std::cout << data << std::endl;

	//				i++;
	//				std::getline(ifp, data);

	//			} 
	//			break;

	//		}

	//	}
	//	




	//}
	//TRACE("完了\n");
}

void TmxObj::SendData()
{

	std::ifstream ifp("map/testMap.tmx");

	MesData data{ MesType::TMX_DATA };
	UnionData unionD;
	std::string stringData;

	std::stringstream strInt;
	std::string string;

	int cnt = 0;

	data.sdata = 0;

	if (!ifp.eof())
	{
		int cnt = 0;
		while (std::getline(ifp, stringData))
		{
			do
			{
				std::getline(ifp, stringData);

				if (ifp.eof())
				{
					break;
				}

			} while (stringData.find("data encoding") == std::string::npos);

			while(std::getline(ifp, stringData))
			{

				while (stringData.find("/data") != std::string::npos)
				{
					break;
				}

				//std::getline(ifp, stringData);

				strInt << stringData;
				while (std::getline(strInt, string, ','))
				{
					std::cout << string;
					cnt++;
					if (cnt % 21 != 0)
					{
						std::cout << " ";

					}
					else
					{
						std::cout << std::endl;
							
					}
				}

				strInt.clear();

			}
		}

	}

	


	//for (auto layer : layer_)
	//{
	//	for (int i = 0; i <21*17; i++)
	//	{


	//		unionD.cData[cnt] = layer_[layer.first][i];
	//		
	//		//std::cout << layer_[layer.first][i] << std::endl;
	//		if (i < 21 * 17-1 )
	//		{
	//			i++;
	//			unionD.cData[cnt] += layer_[layer.first][i] << 4;
	//		}

	//		//else
	//		//{
	//		//	
	//		//	unionD.cData[cnt] += layer_[layer.first][i] << 4;

	//		//}


	//		if (cnt != 7)
	//		{
	//			cnt++;
	//		}
	//		else
	//		{
	//			data.data[0] = unionD.iData[0];
	//			data.data[1] = unionD.iData[1];		
	//			IpNetwork.SendMes(data);
	//			data.sdata++;

	//			TRACE("TMX[%d] , DATA1:%d , DATA2:%d\n", data.sdata, data.data[0], data.data[1]);

	//			cnt = 0;
	//		}

	//	}
	//}
	//std::cout << data.sdata <<std::endl;



}

std::vector<int> TmxObj::StringChange(std::string string)
{

	std::string indata, outdata;
	indata = string;

	std::stringstream Data;
	Data << indata;

	std::vector<int> tmpVector;

	for (int i = 0; i < 21 * 17; i++)
	{
		std::getline(Data, outdata, ',');
		tmpVector.emplace_back(std::atoi(outdata.c_str()));
	}

	return tmpVector;
}
