#include "Map.h"

CMap::CMap(void)
: theScreen_Height(0)
, theScreen_Width(0)
, theNumOfTiles_Height(0)
, theNumOfTiles_Width(0)
, theMap_Height(0)
, theMap_Width(0)
, theNumOfTiles_MapHeight(0)
, theNumOfTiles_MapWidth(0)
, theTileSize(0)
, m_iMapID(999)
{
	theScreenMap.clear();
}

CMap::~CMap(void)
{
	theScreenMap.clear();
}

void CMap::Init(const int theScreen_Height, const int theScreen_Width, const int theNumOfTiles_Height, const int theNumOfTiles_Width, const int theMap_Height, const int theMap_Width, int theTileSize)
{
	this->theScreen_Height		= theScreen_Height;
	this->theScreen_Width		= theScreen_Width;
	this->theNumOfTiles_Height	= theNumOfTiles_Height;
	this->theNumOfTiles_Width	= theNumOfTiles_Width;
	this->theMap_Height			= theMap_Height;
	this->theMap_Width			= theMap_Width;
	this->theTileSize			= theTileSize;

	 theNumOfTiles_MapHeight = (int) (theMap_Height / theTileSize); 
	 theNumOfTiles_MapWidth = (int) (theMap_Width / theTileSize);

	theScreenMap.resize(theNumOfTiles_MapHeight);

	for (int i = 0; i < theNumOfTiles_MapHeight; ++i)
	{
		theScreenMap[i].resize(theNumOfTiles_MapWidth);
	}

}

bool CMap::LoadMap(const std::string mapName)
{
	if (LoadFile(mapName) == true)
	{
		return true;
	}
	std::cout << "Failed to Load TileMap" << std::endl;
	return false;
}

bool CMap::LoadFile(const std::string mapName)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;

	std::ifstream file(mapName.c_str());
	if(file.is_open())
	{
		int i = 0, k = 0;
		while(file.good())
		{
			std::string aLineOfText = "";
			std::getline(file, aLineOfText);

			if (theLineCounter > theNumOfTiles_Height)
				break;

			// If this line is not a comment line, then process it
			if(!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
			{
				if (theLineCounter == 0)
				{
					// This is the first line of the map data file
					std::string token;
					std::istringstream iss(aLineOfText);
					while(getline(iss, token, ','))
					{
						// Count the number of columns
						theMaxNumOfColumns = atoi(token.c_str());
					}
					if ( theMaxNumOfColumns != theNumOfTiles_MapWidth)
					{
						return false;
					}
				}
				else
				{
					int theColumnCounter = 0;

					std::string token;
					std::istringstream iss(aLineOfText);
					while(std::getline(iss, token, ',') && (theColumnCounter < theNumOfTiles_MapWidth))
					{
						theScreenMap[theLineCounter-1][theColumnCounter++] = atoi(token.c_str());
					}
				}
			}

			theLineCounter++;
		}
	}
	return true;
}

int CMap::GetNumOfTiles_Height(void)
{
	return theNumOfTiles_Height;
}

int CMap::GetNumOfTiles_Width(void)
{
	return theNumOfTiles_Width;
}

int CMap::getNumOfTiles_MapHeight(void)
{
	return theNumOfTiles_MapHeight;
}

int CMap::getNumOfTiles_MapWidth(void)
{
	return theNumOfTiles_MapWidth;
}

int CMap::GetTileSize(void)
{
	return theTileSize;
}