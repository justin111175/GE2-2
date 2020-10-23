#pragma once


#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>
#include "../InterNet/NetWork.h"

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


class TmxObj
{
public:
	TmxObj();
	~TmxObj();

	void LoadTmx(const char* tmx);
	void LoadTsx(const char* tsx);
	void Draw();
    Tileset tileset;
	bool SendSize(std::string file);
	bool SendData(std::string file);

private:


	std::map<std::string,std::string> layerData_;
	std::map<std::string, std::string> mapData_;

	

	std::map<std::string, std::vector<int>> layer_;
	std::vector<int> StringChange(std::string string);
};

