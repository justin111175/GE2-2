#include "TmxObj.h"
#include <Dxlib.h>
#include <sstream>
#include "../common/ImageMng.h"
#include "../InterNet/NetWork.h"
#include "../common/_debug/_DebugConOut.h"
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
		layerData_.try_emplace(layer_node->first_attribute("name")->value(), data_node->value());


	}


	for (auto i : layerData_)
	{

		layer_.try_emplace(i.first, StringChange(i.first));

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
	MesData testMes = { MesType::TMX_SIZE,static_cast<int>(ifp.tellg()) };
	IpNetwork.SendMes(testMes);

	return true;






}

bool TmxObj::SendData(std::string file)
{
	std::ifstream ifp(file);//ファイルを受け取り

	MesData data{ MesType::TMX_DATA };

	std::string str;
	int test = 0;
	while (!ifp.eof()) //eof:最後かどうかのチェック
	{
		data.data[1] = ifp.get();
		IpNetwork.SendMes(data);
		data.data[0]++;

		TRACE("TMX[%d]\n", data.data[0]);
		TRACE("DATA:%d\n", data.data[1]);
		//TRACE("%d\n",test);

	}

	TRACE("最後：%d回\n", data.data[0]);

	//for (auto atb = ifp; atb != nullptr; atb = atb->next_attribute())
	//{


	//	IpNetwork.SendMes(testMes);
	//}
	//for (auto a : mapData_)
	//{
	//	test[0]=mapData_[]
	//	MesData testMes = { MesType::TMX_DATA,};
	//	IpNetwork.SendMes(testMes);

	//}



	return true;







}

std::vector<int> TmxObj::StringChange(std::string string)
{

	std::string indata, outdata;
	indata = layerData_[string];

	std::stringstream Data;
	Data << indata;

	std::vector<int> tmpVector;

	for (int i = 0; i < 32 * 32; i++)
	{
		std::getline(Data, outdata, ',');
		tmpVector.emplace_back(std::atoi(outdata.c_str()));
	}

	return tmpVector;
}
