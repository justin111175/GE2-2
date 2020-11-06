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


class TmxObj
{
public:
	TmxObj();
	~TmxObj();

	void LoadTmx(const char* tmx);
	void LoadTsx(const char* tsx);
	void Draw();
    Tileset tileset;
	bool SendTmxSize(std::string file);



	void SendTmx();
private:

	std::map<std::string,std::string> layerData_;
	std::map<std::string, std::string> mapData_;


	std::map<std::string, std::vector<int>> layer_;
	std::vector<int> StringChange(std::string string);
};

