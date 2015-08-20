#include "World.h"
#include "MyMath.h"

#include <algorithm>
#include <Windows.h>


World::World(WORLD_ID worldID)
	: worldWidth(0)
	, worldHeight(0)
{
	this->WorldID = worldID;

	exitConnectionList.clear();
	exitList.clear();
}


World::~World(void)
{
	exitConnectionList.clear();
	exitList.clear();
}


void World::initWorld()
{
	worldWidth = 0; worldHeight = 0;

	for (unsigned i = 0; i < m_roomList.size(); i++)
	{
		if ((int)(m_roomList[i]->getWorldPositionX()+m_roomList[i]->backgroundData[0].size()) > worldHeight)
		{
			worldWidth = m_roomList[i]->getWorldPositionX()+m_roomList[i]->backgroundData[0].size();
		}

		if ((int)(m_roomList[i]->getWorldPositionY()+m_roomList[i]->backgroundData.size()) > worldHeight)
		{
			worldHeight = m_roomList[i]->getWorldPositionY()+m_roomList[i]->backgroundData.size();
		}
	}

	this->SetTileSize(m_roomList[0]->GetTileSize());
	this->setNumOfTiles_MapHeight(worldHeight+11); // 10 space buffer below
	this->setNumOfTiles_MapWidth(worldWidth+10); // 10 space buffer to the right

	foregroundData.resize(this->getNumOfTiles_MapHeight());
	for (int i = 0; i < this->getNumOfTiles_MapHeight(); ++i)
		foregroundData[i].resize(this->getNumOfTiles_MapWidth());

	collisionData.resize(this->getNumOfTiles_MapHeight());
	for (int i = 0; i < this->getNumOfTiles_MapHeight(); ++i)
		collisionData[i].resize(this->getNumOfTiles_MapWidth());

	sceneryData.resize(this->getNumOfTiles_MapHeight());
	for (int i = 0; i < this->getNumOfTiles_MapHeight(); ++i)
		sceneryData[i].resize(this->getNumOfTiles_MapWidth());

	backgroundData.resize(this->getNumOfTiles_MapHeight());
	for (int i = 0; i < this->getNumOfTiles_MapHeight(); ++i)
		backgroundData[i].resize(this->getNumOfTiles_MapWidth());

	for (unsigned i = 1; i < this->sceneryData.size()-1; i++) // Reset data values
	{
		std::fill(backgroundData[i].begin(), backgroundData[i].end(), -1);
		std::fill(sceneryData[i].begin(), sceneryData[i].end(), -1);
		std::fill(collisionData[i].begin(), collisionData[i].end(), -1);
		std::fill(foregroundData[i].begin(), foregroundData[i].end(), -1);
	}

	for (unsigned i = 0; i < m_roomList.size(); i++)
	{
		for (int j = 0; j < m_roomList[i]->getNumOfTiles_MapHeight(); j++)
		{
			for (int k = 0; k < m_roomList[i]->getNumOfTiles_MapWidth(); k++)
			{
				if (m_roomList[i]->backgroundData[j][k] != 0)
				{
					this->backgroundData[j+m_roomList[i]->getWorldPositionY()][k+m_roomList[i]->getWorldPositionX()] = m_roomList[i]->backgroundData[j][k];
					this->sceneryData[j+m_roomList[i]->getWorldPositionY()][k+m_roomList[i]->getWorldPositionX()] = m_roomList[i]->sceneryData[j][k];
					this->foregroundData[j+m_roomList[i]->getWorldPositionY()][k+m_roomList[i]->getWorldPositionX()] = m_roomList[i]->foregroundData[j][k];
					this->collisionData[j+m_roomList[i]->getWorldPositionY()][k+m_roomList[i]->getWorldPositionX()] = m_roomList[i]->collisionData[j][k];
				}
			}
		}
	}

	for (unsigned i = 0; i < m_roomList.size(); i++)
	{
		for (unsigned j=0; j < m_roomList[i]->getExitSize(); j++)
		{
			exitList.push_back(m_roomList[i]->getExit(j));
			exitList.back()->exitPositionY += m_roomList[i]->getWorldPositionY();
			exitList.back()->exitPositionX += m_roomList[i]->getWorldPositionX();
		}
	}

	RoomExitYLessThan sortMe;
	std::sort(exitList.begin(), exitList.end(), sortMe);

	while (!initCorridors());
	while (!generateCorridors());
}

bool World::initCorridors()
{
	if (m_roomList.size() < 2)
	{
		if (exitList.size() > 0) // Not supposed to have exits
		{
			MessageBox(NULL,L"Error", L"Check exits", NULL);
			return false;
		}
		return true;
	}

	//2 rooms or more, generate exits
	bool foundPath = false;
	Room_Exit* highestExit = exitList[0];
	Room_Exit* secondHighestExit = exitList[1];

	while (!foundPath)
	{
		if (exitList.size() == 3)
		{
			if (highestExit->exitDirection == EXIT_UP)
			{
			}
			else if (highestExit->exitDirection == EXIT_DOWN)
			{
				if (secondHighestExit->exitDirection == EXIT_UP)
				{
				}
				else if (secondHighestExit->exitDirection == EXIT_DOWN)
				{
					int lowerX = 0, upperX = 0;
					if (highestExit->exitPositionX < secondHighestExit->exitPositionX)
					{
						lowerX = highestExit->exitPositionX;
						upperX = secondHighestExit->exitPositionX;
					}
					else
					{
						lowerX = secondHighestExit->exitPositionX;
						upperX = highestExit->exitPositionX;
					}

					for (int i = lowerX; (i <= upperX) && !foundPath; i++)
					{
						for (int j = 1; (j <= 5) && !foundPath; j++)
						{
							if (backgroundData[secondHighestExit->exitPositionY+j][lowerX+i] != -1) // if there's a room in between
							{
								int connection_type = Math::RandIntMinMax(0, 1);

								if (connection_type == 0) // one over
								{
									Room_Exit_Connections* newConnection = new Room_Exit_Connections(highestExit, secondHighestExit, CONNECTION_OVER_ROOMS);
									exitConnectionList.push_back(newConnection);

									connection_type = Math::RandIntMinMax(0, 1);
									if (connection_type == 0) // connect left room to bottom room
									{
										if (highestExit->exitPositionX < secondHighestExit->exitPositionX)
										{
											newConnection = new Room_Exit_Connections(highestExit, exitList[2], CONNECTION_LEFT_ROOMS);
											exitConnectionList.push_back(newConnection);
										}
										else
										{
											newConnection = new Room_Exit_Connections(secondHighestExit, exitList[2], CONNECTION_LEFT_ROOMS);
											exitConnectionList.push_back(newConnection);
										}
									}
									else // connect right room to bottom room
									{
										if (highestExit->exitPositionX > secondHighestExit->exitPositionX)
										{
											newConnection = new Room_Exit_Connections(highestExit, exitList[2], CONNECTION_RIGHT_ROOMS);
											exitConnectionList.push_back(newConnection);
										}
										else
										{
											newConnection = new Room_Exit_Connections(secondHighestExit, exitList[2], CONNECTION_RIGHT_ROOMS);
											exitConnectionList.push_back(newConnection);
										}
									}
									foundPath = true;
								}
								else // connect everything through bottom
								{
									Room_Exit_Connections* newConnection = new Room_Exit_Connections(highestExit, exitList[2], CONNECTION_UNDER_ROOMS);
									exitConnectionList.push_back(newConnection);

									newConnection = new Room_Exit_Connections(secondHighestExit, exitList[2], CONNECTION_UNDER_ROOMS);
									exitConnectionList.push_back(newConnection);
									foundPath = true;
								}
							}
							else // space in between
							{
								int connection_type = Math::RandIntMinMax(0, 1);

								if (connection_type == 0)
								{
									connection_type = Math::RandIntMinMax(0, 1);

									if (highestExit->exitPositionX < secondHighestExit->exitPositionX)
									{
										if (connection_type == 0) // connect left room below bottom room
										{
											Room_Exit_Connections* newConnection = new Room_Exit_Connections(highestExit, secondHighestExit, CONNECTION_UNDER_ROOMS);
											exitConnectionList.push_back(newConnection);

											newConnection = new Room_Exit_Connections(highestExit, exitList[2], CONNECTION_UNDER_ROOMS);
											exitConnectionList.push_back(newConnection);
											std::cout << "second connection left  bottom" << std::endl;
										}
										else // connect left room to left of bottom room
										{
											Room_Exit_Connections* newConnection = new Room_Exit_Connections(highestExit, secondHighestExit, CONNECTION_UNDER_ROOMS);
											exitConnectionList.push_back(newConnection);

											newConnection = new Room_Exit_Connections(highestExit, exitList[2], CONNECTION_LEFT_ROOMS);
											exitConnectionList.push_back(newConnection);
											std::cout << "second connection left  bottom" << std::endl;
										}
										foundPath = true;
									}
									else
									{
										if (connection_type == 0) // connect right room to below room
										{
											Room_Exit_Connections* newConnection = new Room_Exit_Connections(highestExit, secondHighestExit, CONNECTION_UNDER_ROOMS);
											exitConnectionList.push_back(newConnection);

											newConnection = new Room_Exit_Connections(highestExit, exitList[2], CONNECTION_UNDER_ROOMS);
											exitConnectionList.push_back(newConnection);
											std::cout << "second connection right  bottom" << std::endl;
										}
										else // connect right room to right of bottom room
										{
											Room_Exit_Connections* newConnection = new Room_Exit_Connections(highestExit, secondHighestExit, CONNECTION_UNDER_ROOMS);
											exitConnectionList.push_back(newConnection);

											newConnection = new Room_Exit_Connections(highestExit, exitList[2], CONNECTION_RIGHT_ROOMS);
											exitConnectionList.push_back(newConnection);
											std::cout << "second connection right  bottom" << std::endl;
										}
										foundPath = true;
									}
								}
							}
						}
					}
				}
				else if (secondHighestExit->exitDirection == EXIT_LEFT)
				{
				}
				else if (secondHighestExit->exitDirection == EXIT_RIGHT)
				{
				}
			}
			else if (highestExit->exitDirection == EXIT_LEFT)
			{
			}
			else if (highestExit->exitDirection == EXIT_RIGHT)
			{
			}
		}
	}
	return true;
}

bool World::generateCorridors()
{
	if (exitList.size() != 0)
	{
		if (exitConnectionList.size() != exitList.size()-1)
		{
			std::cout << "SOMETHING WENT WRONG" << " " << exitConnectionList.size() << " " <<exitList.size() << std::endl;
			return false;
		}

		for (unsigned i = 0; i < exitConnectionList.size(); i++)
		{
			if (exitConnectionList[i]->connectionType == CONNECTION_OVER_ROOMS)
			{
			}
			else if (exitConnectionList[i]->connectionType == CONNECTION_UNDER_ROOMS)
			{
				int lowerCorridorY = exitConnectionList[i]->exitB->exitPositionY+5;

				for (int j = exitConnectionList[i]->exitA->exitPositionY; j < lowerCorridorY; j++)
				{
					this->backgroundData[j][exitConnectionList[i]->exitA->exitPositionX] = (exitConnectionList[i]->corridorTileIDA[TILE_CORRIDOR_FLOOR]);
					std::cout << j << " " << exitConnectionList[i]->exitA->exitPositionX << " " << (exitConnectionList[i]->corridorTileIDA[TILE_CORRIDOR_FLOOR]) << std::endl;
				}

				for (int j = exitConnectionList[i]->exitB->exitPositionY; j < lowerCorridorY; j++)
				{
					this->backgroundData[j][exitConnectionList[i]->exitB->exitPositionX] = exitConnectionList[i]->corridorTileIDB[TILE_CORRIDOR_FLOOR];
				}
			}
			else if (exitConnectionList[i]->connectionType == CONNECTION_LEFT_ROOMS)
			{
			}
			else if (exitConnectionList[i]->connectionType == CONNECTION_RIGHT_ROOMS)
			{
			}
		}
	}

	return true;
}

void World::setWorldID(WORLD_ID worldID)
{
	this->WorldID = worldID;
}

int World::getWorldID()
{
	return WorldID;
}

int World::getRoom(float playerX, float playerY)
{
	return 0;
}