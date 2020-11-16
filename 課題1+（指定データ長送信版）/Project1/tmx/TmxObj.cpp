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
	TRACE("マップ初期化開始\n");

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
	TRACE("マップ初期化完了\n");

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



void TmxObj::SendTmx()//host
{

	std::ifstream ifp("map/ObjTest.tmx");


	UnionData unionD;

	Header header;
	
	std::string stringData;

	std::stringstream strInt;
	std::string string;


	int cnt = 0;
	std::vector<int> VecInt;

	MesPacket data;

	
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

				if (stringData.find("/data") == std::string::npos)
				{
					strInt << stringData;

					while (std::getline(strInt, string, ','))
					{
						

						int tmpcnt = cnt / 2;
						if (cnt % 2 == 0)
						{
							
							unionD.cData[tmpcnt] = atoi(string.c_str());
							//std::cout << atoi(string.c_str()) ;
						}
						else
						{
							unionD.cData[tmpcnt] += atoi(string.c_str()) << 4;
							//std::cout << atoi(string.c_str());
						}

						if (cnt != 7)
						{
							cnt++;

						}
						else
						{

							data.insert(data.end(), { unionD.uiData });
							cnt = 0;
						}

						if (data.size() == 178)
						{
							data.insert(data.end(), { unionD.uiData });
						}
					}

					strInt.clear();
				}

				while (stringData.find("/data") != std::string::npos)
				{
					do
					{
						std::getline(ifp, stringData);

						if (ifp.eof())
						{
							break;
						}

					} while (stringData.find("data encoding") == std::string::npos);
				}

			}
		}

	}
	int count = 0;
	IpNetwork.SendMes(MesType::TMX_SIZE,data);
	IpNetwork.SendMes(MesType::TMX_DATA,data);


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
