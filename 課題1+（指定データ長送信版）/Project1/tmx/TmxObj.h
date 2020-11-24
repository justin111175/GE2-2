#pragma once

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>

struct Layer
{
	std::string name;
	std::vector<int> data;
};

struct Tileset {
    std::string name;
    int tileWidth;
    int tileHeight;
	

	int width;
	int height;
	int tilecount;
	int columns;
    char* source;

};



//mapDataBase,mapData
using PairMap = std::pair<std::vector<int>, std::vector<int*>>;

using VecMap = std::vector<PairMap>;

class TmxObj
{
public:
	TmxObj();
	~TmxObj();

	void LoadTmx(const char* tmx);
	void LoadTsx(const char* tsx);
	void Draw();
    Tileset tileset;



	void SendTmx();
	std::map<std::string, std::vector<int>> layer_;

	VecMap GetMap();

private:

	std::map<std::string,std::string> layerData_;
	std::map<std::string, std::string> mapData_;
	VecMap pairMap_;


	std::vector<int> StringChange(std::string string);
};

