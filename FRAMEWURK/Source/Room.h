#ifndef ROOM_H
#define ROOM_H

#include "Map.h"

#include <vector>
#include <iostream>

enum ROOM_TYPE
{
	ROOM_NULL,

	ROOM_MAINMENU,

	ROOM_TESTPUZZLE,

	ROOM_MECH,

	ROOM_TUTORIAL_FRIENDS,

	ROOM_DUCKS,
	ROOM_MIRROR,
	ROOM_OMGPLEASEADDTYPES,

	ROOM_TOTAL = ROOM_OMGPLEASEADDTYPES
};

enum EXIT_DIRECTION
{
	EXIT_NULL,

	EXIT_LEFT,
	EXIT_RIGHT,
	EXIT_UP,
	EXIT_DOWN,
};

enum ROOM_FLOOR_TILE
{
	ROOMS_FLOORTILE = 162,
	BLUE_FLOORTILE = 49,
	MAINMENU_FLOORTILE = 768,
	MECH_FLOORTILE = 356,
};

enum TILE_CORRIDOR
{
	TILE_CORRIDOR_FLOOR = 983,
	TILE_CORRIDOR_TOP_WALL = 1018,
	TILE_CORRIDOR_BOTTOM_WALL = 1016,
	TILE_CORRIDOR_LEFT_WALL = 1019,
	TILE_CORRIDOR_RIGHT_WALL = 1017,
	TILE_CORRIDOR_TOP_LEFT_CORNER = 1023,
	TILE_CORRIDOR_TOP_RIGHT_CORNER = 1020,
	TILE_CORRIDOR_BOTTOM_LEFT_CORNER = 1022,
	TILE_CORRIDOR_BOTTOM_RIGHT_CORNER = 1021,
	TILE_CORRIDOR_INNER_TOP_LEFT_CORNER = 989,
	TILE_CORRIDOR_INNER_TOP_RIGHT_CORNER = 990,
	TILE_CORRIDOR_INNER_BOTTOM_LEFT_CORNER = 988,
	TILE_CORRIDOR_INNER_BOTTOM_RIGHT_CORNER = 991
};

struct Room_Exit
{
	Room_Exit(EXIT_DIRECTION exitDirection, ROOM_TYPE roomType = ROOM_NULL, int roomID = 0, int exitPositionX = 0, int exitPositionY = 0)
	{
		this->exitDirection = exitDirection;
		this->exitPositionX = exitPositionX;
		this->exitPositionY = exitPositionY;
		this->roomType = roomType;
		this->roomID = roomID;
		this->connected = false;
	}

	~Room_Exit()
	{
	}

	bool connected;
	int roomID;
	ROOM_TYPE roomType;
	EXIT_DIRECTION exitDirection;
	int exitPositionX;
	int exitPositionY;
};

struct RoomExitYLessThan
{
public:
	bool operator()(const Room_Exit* a, const Room_Exit* b)
	{
		return (a->exitPositionY < b->exitPositionY);
	}
};

struct Room_Object
{
	enum ROOM_OBJECT_TYPE
	{
		ROOM_OBJECT_TESTPUZZLE_BED,
		ROOM_OBJECT_TESTPUZZLE_MEDIUM_TABLE,
		ROOM_OBJECT_TESTPUZZLE_SMALL_TABLE,
		ROOM_OBJECT_TESTPUZZLE_CHAIR_SLANT_LEFT,
		ROOM_OBJECT_TESTPUZZLE_CHAIR_SLANT_RIGHT,
		ROOM_OBJECT_TESTPUZZLE_WHITE_TABLECLOTH_TABLE,
		ROOM_OBJECT_TESTPUZZLE_BIG_WINDOW,
		ROOM_OBJECT_TESTPUZZLE_SMALL_WINDOW,
		ROOM_OBJECT_TESTPUZZLE_GLASS_CABINET,
		ROOM_OBJECT_TESTPUZZLE_PATTERNED_FLOOR,

		// Exits
		ROOM_OBJECT_TESTPUZZLE_CLOSED_EXIT_BOTTOM,
		ROOM_OBJECT_TESTPUZZLE_OPEN_EXIT_BOTTOM,

		//Stuff on white tablecloth table
		ROOM_OBJECT_TESTPUZZLE_BRONZE_GLOBE,
		ROOM_OBJECT_TESTPUZZLE_BRONZE_CANDLESTICK,
		ROOM_OBJECT_TESTPUZZLE_BRONZE_BOTTLE,
		ROOM_OBJECT_TESTPUZZLE_OPEN_BOOK,

		//Friend Night objects
		ROOM_OBJECT_FRIEND,

		//AHMAD room things
		ROOM_OBJECT_MECH_FACTORY_PIPES,
		ROOM_OBJECT_MECH_CURVE_PIPES,
		ROOM_OBJECT_MECH_WALL_PIPES,
		ROOM_OBJECT_MECH_HOSE_PIPE,
		ROOM_OBJECT_MECH_ROCK_WALL,
		ROOM_OBJECT_MECH_ROCK_RIGHT,
		ROOM_OBJECT_MECH_ROCK_LEFT,
		ROOM_OBJECT_MECH_ENGINE,
		ROOM_OBJECT_MECH_LEFT_DOOR,
		ROOM_OBJECT_MECH_RIGHT_DOOR,

		
	};

	Room_Object(ROOM_OBJECT_TYPE objectType, int roomHeight, int roomWidth);
	~Room_Object();
	

	ROOM_OBJECT_TYPE type;
	int width, height;
	int lowerOriginX, upperOriginX;
	int lowerOriginY, upperOriginY;
};

class Room : public CMap
{
public:
	Room(
		ROOM_TYPE roomType, 
		const int theScreen_Height, 
		const int theScreen_Width, 
		const int theNumOfTiles_Height, 
		const int theNumOfTiles_Width, 
		const int theMap_Height, 
		const int theMap_Width, 
		const int theTileSize, 
		const TILESET_ID tileset,
		const int worldPositionX,
		const int worldPositionY,
		const int roomID);
	~Room(void);

	void addOBJtoGenerate(Room_Object* object);

	int getWorldPositionX();
	int getWorldPositionY();

	unsigned getExitSize();
	Room_Exit* getExit(int index);

	ROOM_TYPE getRoomType();

	void generateRoom(); // Add objects and collision based on roomType
	bool addObject(ROOM_TYPE type, Room_Object* object, int originX, int originY); // Add specific object to room based on position and type (origin top left)
	void addExit(EXIT_DIRECTION exit); // Adds exit to room
private:
	void reset_mapData(); // Reset map values
	bool attemptToAdd(ROOM_TYPE roomType, Room_Object* object);

	int roomID;
	std::vector<Room_Object*> roomObjectList; // Stores all objects to be added during generateRoom()
	int attemptCounter; // Stores number of times object in room was attempted to be generated.
	int exitCounter; // Stores number of exits generated
	int worldPositionX, worldPositionY; // Position of the room in world (bottom left)
	ROOM_TYPE roomType; // Stores room type
	std::vector<Room_Exit* > numExit; // Stores direction of each exit, and number of exits [ .size() ]
};

#endif