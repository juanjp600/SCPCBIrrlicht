#include "3dworld.h"
#include "player.h"

room* world::addRandomRoom(unsigned short x,unsigned short y,roomTypes type,char angle,int zone) {
	int choice = 0;
	room* retRoom = nullptr;
	switch (type) {
		case roomTypes::ROOM1:
			switch (zone) {
				case 0: //LCZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = endroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 1: //HCZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = endroom2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 2: //EZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = endroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
			}
		break;
		case roomTypes::ROOM2:
			switch (zone) {
				case 0: //LCZ
					choice = rand()%150;
					if (choice>=0 && choice<50) {
						retRoom = room2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=50 && choice<100) {
						retRoom = room2_2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=100 && choice<120) {
						retRoom = room2elevator::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=120 && choice<150) {
						retRoom = room2doors::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 1: //HCZ
					choice = rand()%295;
					if (choice>=0 && choice<100) {
						retRoom = tunnel::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=100 && choice<170) {
						retRoom = tunnel2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=170 && choice<220) {
						retRoom = room2pipes::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=220 && choice<295) {
						retRoom = room2pit::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 2: //EZ
					choice = rand()%300;
					if (choice>=0 && choice<100) {
						retRoom = room2z3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=100 && choice<130) {
						retRoom = room2offices::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=130 && choice<150) {
						retRoom = room2offices2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=150 && choice<170) {
						retRoom = room2offices3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=170 && choice<200) {
						retRoom = room2toilets::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=200 && choice<300) {
						retRoom = room2tesla::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
			}
		break;
		case roomTypes::ROOM2C:
			switch (zone) {
				case 0: //LCZ
					choice = rand()%70;
					if (choice>=0 && choice<30) {
						retRoom = lockroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=30 && choice<70) {
						retRoom = room2c::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 1: //HCZ
					choice = rand()%40;
					if (choice>=0 && choice<40) {
						retRoom = room2ctunnel::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 2: //EZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = room2cz3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
			}
		break;
		case roomTypes::ROOM3:
			switch (zone) {
				case 0: //LCZ
					choice = rand()%200;
					if (choice>=0 && choice<100) {
						retRoom = room3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=100 && choice<200) {
						retRoom = room3_2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 1: //HCZ
					choice = rand()%200;
					if (choice>=0 && choice<100) {
						retRoom = room3pit::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=100 && choice<200) {
						retRoom = room3tunnel::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 2: //EZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = room3z3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
			}
		break;
		case roomTypes::ROOM4:
			switch (zone) {
				case 0: //LCZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = room4::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 1: //HCZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = room4tunnels::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 2: //EZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = room4z3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
			}
		break;
	}
	roomArray[x][y] = retRoom;
	return retRoom;
}

void world::createMap() {
	auto getZone = [] (short y) -> int { return std::max(std::min(std::floor((float)(20-y)/20.f*3.f),2.f),0.f); };

	short x,y,temp;
	short x2,y2;
	short width,height;

	struct tempRoom {
		roomTypes type;
		char angle;
	};

	tempRoom roomTemp[20][20];
	for (x=0;x<20;x++) {
		for (y=0;y<20;y++) {
			roomArray[x][y] = nullptr;
			roomTemp[x][y].type = roomTypes::ROOM1;
			roomTemp[x][y].angle = -1;
		}
	}

	x = 10;
	y = 18;

	for (int i = y;i<20;i++) {
		roomTemp[x][i].angle = 0;
	}

	while (y>=2) {
		width = (rand() % 6) + 10;

		if (x>12) {
			width = -width;
		} else if (x>8) {
			x = x-10;
		}

		//make sure the hallway doesn't go outside the array
		if (x+width > 17) {
			width=17-x;
		} else if (x+width < 2) {
			width=-x+2;
		}


		x = std::min(x,short(x + width));
		width = std::abs(width);
		for (int i = x;i<=x+width;i++) {
			roomTemp[std::min(i,19)][y].angle = 0;
		}

		height = (rand() % 2) + 3;
		if (y - height < 1) height = y;

		int yhallways = (rand() % 2) + 4;

		if (getZone(y-height)!=getZone(y-height+1)) height--;

		for (int i=1;i<=yhallways;i++) {
			x2 = std::max(std::min((rand() % (width-1)) + x,18),2);
			while (roomTemp[x2][y-1].angle>=0 || roomTemp[x2-1][y-1].angle>=0 || roomTemp[x2+1][y-1].angle>=0) {
				x2++;
				if (x2>18) break;
			}

			if (x2<x+width) {
				short tempheight;
				if (i==1) {
					tempheight = height;
					if (rand()%2 == 0) x2 = x; else x2 = x+width;
				} else {
					tempheight = (rand()%height) + 1;
				}

				for (y2 = y - tempheight;y2<=y;y2++) {
					if (getZone(y2)!=getZone(y2+1)) { //a room leading from zone to another
						roomTemp[x2][y2].angle = 127;
					} else {
						roomTemp[x2][y2].angle = 0;
					}
				}

				if (tempheight == height) temp = x2;
			}
		}

		x = temp;
		y -= height;
	}

	unsigned short room1amount[3];
	unsigned short room2amount[3];
	unsigned short room2camount[3];
	unsigned short room3amount[3];
	unsigned short room4amount[3];

	for (int i=0;i<3;i++) {
		room1amount[i]=0;
		room2amount[i]=0;
		room2camount[i]=0;
		room3amount[i]=0;
		room4amount[i]=0;
	}

	for (x=0;x<20;x++) {
		for (y=0;y<20;y++) {
			bool hasNorth,hasSouth,hasEast,hasWest;
			hasNorth = hasSouth = hasEast = hasWest = false;
			if (roomTemp[x][y].angle==127) {
				//get rid of this checkpoint room if it leads to nothing
				if (roomTemp[x][y-1].angle<0) roomTemp[x][y].angle = -1;
			} else if (roomTemp[x][y].angle==0) { //this is not a checkpoint room
				if (x>0) {
					hasWest = (roomTemp[x-1][y].angle>-1);
				}
				if (x<19) {
					hasEast = (roomTemp[x+1][y].angle>-1);
				}
				if (y>0) {
					hasNorth = (roomTemp[x][y-1].angle>-1);
				}
				if (y<19) {
					hasSouth = (roomTemp[x][y+1].angle>-1);
				}
				if (hasNorth && hasSouth) {
					if (hasEast && hasWest) { //room4
						roomTemp[x][y].type = roomTypes::ROOM4;
						roomTemp[x][y].angle = rand()%4;
					} else if (hasEast && !hasWest) { //room3, pointing east
						roomTemp[x][y].type = roomTypes::ROOM3;
						roomTemp[x][y].angle = 3;
					} else if (!hasEast && hasWest) { //room3, pointing west
						roomTemp[x][y].type = roomTypes::ROOM3;
						roomTemp[x][y].angle = 1;
					} else { //vertical room2
						roomTemp[x][y].type = roomTypes::ROOM2;
						roomTemp[x][y].angle = (rand()%2)*2;
					}
				} else if (hasEast && hasWest) {
					if (hasNorth && !hasSouth) { //room3, pointing north
						roomTemp[x][y].type = roomTypes::ROOM3;
						roomTemp[x][y].angle = 0;
					} else if (!hasNorth && hasSouth) { //room3, pointing south
						roomTemp[x][y].type = roomTypes::ROOM3;
						roomTemp[x][y].angle = 2;
					} else { //horizontal room2
						roomTemp[x][y].type = roomTypes::ROOM2;
						roomTemp[x][y].angle = ((rand()%2)*2)+1;
					}
				} else if (hasNorth) {
					if (hasEast) { //room2c, north-east
						roomTemp[x][y].type = roomTypes::ROOM2C;
						roomTemp[x][y].angle = 0;
					} else if (hasWest) { //room2c, north-west
						roomTemp[x][y].type = roomTypes::ROOM2C;
						roomTemp[x][y].angle = 1;
					} else { //room1, north
						roomTemp[x][y].type = roomTypes::ROOM1;
						roomTemp[x][y].angle = 0;
					}
				} else if (hasSouth) {
					if (hasEast) { //room2c, south-east
						roomTemp[x][y].type = roomTypes::ROOM2C;
						roomTemp[x][y].angle = 3;
					} else if (hasWest) { //room2c, south-west
						roomTemp[x][y].type = roomTypes::ROOM2C;
						roomTemp[x][y].angle = 2;
					} else { //room1, south
						roomTemp[x][y].type = roomTypes::ROOM1;
						roomTemp[x][y].angle = 2;
					}
				} else if (hasEast) { //room1, east
					roomTemp[x][y].type = roomTypes::ROOM1;
					roomTemp[x][y].angle = 3;
				} else { //room1, west
					roomTemp[x][y].type = roomTypes::ROOM1;
					roomTemp[x][y].angle = 1;
				}
				switch (roomTemp[x][y].type) {
					case roomTypes::ROOM1:
						room1amount[getZone(y)]++;
					break;
					case roomTypes::ROOM2:
						room2amount[getZone(y)]++;
					break;
					case roomTypes::ROOM2C:
						room2camount[getZone(y)]++;
					break;
					case roomTypes::ROOM3:
						room3amount[getZone(y)]++;
					break;
					case roomTypes::ROOM4:
						room4amount[getZone(y)]++;
					break;
				}
			}
		}
	}

	//force some rooms in there
	for (int i = 0;i<3;i++) {
		if (room1amount[i]<5) {
			std::cout<<"Forcing some ROOM1s in zone "<<i<<"\n";
			for (y=2;y<19 && room1amount[i]<5;y++) {
				if (getZone(y+2) == i && getZone(y-2) == i) {
					for (x=2;x<19 && room1amount[i]<5;x++) {
						if (roomTemp[x][y].angle<0) {
							if (((roomTemp[x+1][y].angle>=0) != (roomTemp[x-1][y].angle>=0)) != ((roomTemp[x][y+1].angle>=0) != (roomTemp[x][y-1].angle>=0))) {
								tempRoom* adjRoom = nullptr;
								if (roomTemp[x+1][y].angle>=0) {
									adjRoom = &roomTemp[x+1][y];
									roomTemp[x][y].angle = 3;
								} else if (roomTemp[x-1][y].angle>=0) {
									adjRoom = &roomTemp[x-1][y];
									roomTemp[x][y].angle = 1;
								} else if (roomTemp[x][y+1].angle>=0) {
									adjRoom = &roomTemp[x][y+1];
									roomTemp[x][y].angle = 2;
								} else {
									adjRoom = &roomTemp[x][y-1];
									roomTemp[x][y].angle = 0;
								}

								switch (adjRoom->type) {
									case roomTypes::ROOM2:
										roomTemp[x][y].type = roomTypes::ROOM1;
										room1amount[i]++;
										room2amount[i]--;
										room3amount[i]++;
										adjRoom->type = roomTypes::ROOM3;
										switch (roomTemp[x][y].angle) {
											case 0:
												adjRoom->angle = 2;
											break;
											case 1:
												adjRoom->angle = 3;
											break;
											case 2:
												adjRoom->angle = 0;
											break;
											case 3:
												adjRoom->angle = 1;
											break;
										}
									break;
									case roomTypes::ROOM3:
										roomTemp[x][y].type = roomTypes::ROOM1;
										adjRoom->type = roomTypes::ROOM4;
										room1amount[i]++;
										room3amount[i]--;
										room4amount[i]++;
									break;
									default:
										roomTemp[x][y].angle = -1;
									break;
								}
							}
						}
					}
				}
			}
		}
		if (room4amount[i]<3) {
			std::cout<<"Forcing some ROOM4s in zone "<<i<<"\n";
			for (y=2;y<19 && room4amount[i]<3;y++) {
				if (getZone(y+2) == i && getZone(y-2) == i) {
					for (x=2;x<19 && room4amount[i]<3;x++) {
						if (roomTemp[x][y].angle<0) {
							if (((roomTemp[x+1][y].angle>=0) != (roomTemp[x-1][y].angle>=0)) != ((roomTemp[x][y+1].angle>=0) != (roomTemp[x][y-1].angle>=0))) {
								tempRoom* adjRoom = nullptr;

								if (roomTemp[x+1][y].angle>=0) {
									adjRoom = &roomTemp[x+1][y];
									roomTemp[x][y].angle = 3;
								} else if (roomTemp[x-1][y].angle>=0) {
									adjRoom = &roomTemp[x-1][y];
									roomTemp[x][y].angle = 1;
								} else if (roomTemp[x][y+1].angle>=0) {
									adjRoom = &roomTemp[x][y+1];
									roomTemp[x][y].angle = 2;
								} else {
									adjRoom = &roomTemp[x][y-1];
									roomTemp[x][y].angle = 0;
								}

								switch (adjRoom->type) {
									case roomTypes::ROOM3:
										roomTemp[x][y].type = roomTypes::ROOM1;
										adjRoom->type = roomTypes::ROOM4;
										room1amount[i]++;
										room3amount[i]--;
										room4amount[i]++;
									break;
									default:
										roomTemp[x][y].angle = -1;
									break;
								}
							}
						}
					}
				}
			}
		}
		if (room2camount[i]<3) {
			std::cout<<"Forcing some ROOM2Cs in zone "<<i<<"\n";
			for (y=2;y<19 && room2camount[i]<3;y++) {
				if (getZone(y+2) == i && getZone(y-2) == i) {
					for (x=2;x<19 && room2camount[i]<3;x++) {
						if (roomTemp[x][y].angle<0) {
							if (((roomTemp[x+1][y].angle>=0) != (roomTemp[x-1][y].angle>=0)) != ((roomTemp[x][y+1].angle>=0) != (roomTemp[x][y-1].angle>=0))) {
								tempRoom* adjRoom = nullptr;

								if (roomTemp[x+1][y].angle>=0) {
									adjRoom = &roomTemp[x+1][y];
									roomTemp[x][y].angle = 3;
								} else if (roomTemp[x-1][y].angle>=0) {
									adjRoom = &roomTemp[x-1][y];
									roomTemp[x][y].angle = 1;
								} else if (roomTemp[x][y+1].angle>=0) {
									adjRoom = &roomTemp[x][y+1];
									roomTemp[x][y].angle = 2;
								} else {
									adjRoom = &roomTemp[x][y-1];
									roomTemp[x][y].angle = 0;
								}

								switch (adjRoom->type) {
									case roomTypes::ROOM1:
										roomTemp[x][y].type = roomTypes::ROOM1;
										adjRoom->type = roomTypes::ROOM2C;
										switch (roomTemp[x][y].angle) {
											case 0:
												if (adjRoom->angle == 1) adjRoom->angle = 2; //south-west
												else if (adjRoom->angle == 3) adjRoom->angle = 3; //south-east
												else { roomTemp[x][y].angle = -1; adjRoom->type = roomTypes::ROOM1; }
											break;
											case 1:
												if (adjRoom->angle == 0) adjRoom->angle = 1; //north-east
												else if (adjRoom->angle == 2) adjRoom->angle = 3; //south-east
												else { roomTemp[x][y].angle = -1; adjRoom->type = roomTypes::ROOM1; }
											break;
											case 2:
												if (adjRoom->angle == 1) adjRoom->angle = 1; //north-east
												else if (adjRoom->angle == 3) adjRoom->angle = 0; //north-west
												else { roomTemp[x][y].angle = -1; adjRoom->type = roomTypes::ROOM1; }
											break;
											case 3:
												if (adjRoom->angle == 0) adjRoom->angle = 0; //north-west
												else if (adjRoom->angle == 2) adjRoom->angle = 2; //south-west
												else { roomTemp[x][y].angle = -1; adjRoom->type = roomTypes::ROOM1; }
											break;
										}
										if (roomTemp[x][y].angle > -1) {
											room2camount[i]++;
										}
									break;
									default:
										roomTemp[x][y].angle = -1;
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	for (x=0;x<20;x++) {
		for (y=0;y<20;y++) {
			std::cout<<(roomTemp[y][x].angle>-1);
		}
		std::cout<<"\n";
	}

	unsigned short currentRoom1,currentRoom2,currentRoom2c,currentRoom3,currentRoom4;
	currentRoom1 = currentRoom2 = currentRoom2c = currentRoom3 = currentRoom4 = 0;
	short prevZone = getZone(19);
	for (y=19;y>=0;y--) {
		if (getZone(y)!=prevZone) {
			currentRoom1 = currentRoom2 = currentRoom2c = currentRoom3 = currentRoom4 = 0;
		}
		for (x=19;x>=0;x--) {
			if (roomTemp[x][y].angle>-1 && roomTemp[x][y].angle<127) {
				switch (roomTemp[x][y].type) {
					case roomTypes::ROOM1:
						switch (getZone(y)) {
							case 0: //LCZ
								if (currentRoom1==0) {
									roomArray[x][y] = start::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
									mainPlayer->teleport(irr::core::vector3df(x*204.8f*RoomScale,10.f,y*204.8f*RoomScale));

									for (unsigned int i=0;i<itemList.size();i++) {
										itemList[i]->Unpick(irr::core::vector3df(x*204.8f*RoomScale,10.f,y*204.8f*RoomScale));
									}

								} else if (currentRoom1==(int)(0.4f*(float)room1amount[0])) {
									roomArray[x][y] = roompj::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom1==(int)(0.8f*(float)room1amount[0])) {
									roomArray[x][y] = r_914::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 1: //HCZ
								if (currentRoom1==(int)(0.1f*(float)room1amount[1])) {
									roomArray[x][y] = room079::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom1==(int)(0.3f*(float)room1amount[1])) {
									roomArray[x][y] = room106::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
									//mainPlayer->teleport(irr::core::vector3df(x*204.8f*RoomScale,10,y*204.8f*RoomScale));
								} else if (currentRoom1==(int)(0.5f*(float)room1amount[1])) {
									roomArray[x][y] = coffin::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom1==(int)(0.7f*(float)room1amount[1])) {
									roomArray[x][y] = room035::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom1==(int)(0.9f*(float)room1amount[1])) {
									roomArray[x][y] = r_008::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 2: //EZ
								if (currentRoom1==room1amount[2]-1) {
									roomArray[x][y] = gateaentrance::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom1==room1amount[2]-2) {
									roomArray[x][y] = exit1::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
						}
						currentRoom1++;
					break;
					case roomTypes::ROOM2:
						switch (getZone(y)) {
							case 0: //LCZ
								if (currentRoom2==0) {
									roomArray[x][y] = room2closets::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.2f*(float)room2amount[0])) {
									roomArray[x][y] = room2testroom2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.4f*(float)room2amount[0])) {
									roomArray[x][y] = room2scps::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.8f*(float)room2amount[0])) {
									roomArray[x][y] = room2storage::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.9f*(float)room2amount[0])) {
									roomArray[x][y] = room012::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 1: //HCZ
								if (currentRoom2==(int)(0.2f*(float)room2amount[1])) {
									roomArray[x][y] = room2nuke::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.25f*(float)room2amount[1])) {
									roomArray[x][y] = room2tunnel::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.4f*(float)room2amount[1])) {
									roomArray[x][y] = room049::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.7f*(float)room2amount[1])) {
									roomArray[x][y] = room2servers::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.9f*(float)room2amount[1])) {
									roomArray[x][y] = testroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 2: //EZ
								if (currentRoom2==(int)(0.1f*(float)room2amount[2])) {
									roomArray[x][y] = room2poffices::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.2f*(float)room2amount[2])) {
									roomArray[x][y] = room2cafeteria::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.3f*(float)room2amount[2])) {
									roomArray[x][y] = room2sroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.45f*(float)room2amount[2])) {
									roomArray[x][y] = room2offices::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.6f*(float)room2amount[2])) {
									roomArray[x][y] = room860::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.8f*(float)room2amount[2])) {
									roomArray[x][y] = room2poffices2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.9f*(float)room2amount[2])) {
									roomArray[x][y] = room2offices2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
						}
						currentRoom2++;
					break;
					case roomTypes::ROOM2C:
						switch (getZone(y)) {
							case 0: //LCZ
								if (currentRoom2c==0) {
									roomArray[x][y] = lockroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 1: //HCZ

							break;
							case 2: //EZ
								if (currentRoom2c==0) {
									roomArray[x][y] = room2ccont::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2c==1) {
									roomArray[x][y] = lockroom2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
						}
						currentRoom2c++;
					break;
					case roomTypes::ROOM3:
						switch (getZone(y)) {
							case 0: //LCZ
								if (currentRoom3==(int)((((rand()%1000)*0.001f)+0.2f)*(float)room3amount[0])) {
									roomArray[x][y] = room3storage::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 1: //HCZ
								if (currentRoom3==(int)(0.5f*(float)room3amount[1])) {
									roomArray[x][y] = room513::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 2: //EZ
								if (currentRoom3==(int)(0.3f*(float)room3amount[2])) {
									roomArray[x][y] = room3servers::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom3==(int)(0.7f*(float)room3amount[2])) {
									roomArray[x][y] = room3servers2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
						}
						currentRoom3++;
					break;
					case roomTypes::ROOM4:
						switch (getZone(y)) {
							case 0: //LCZ

							break;
							case 1: //HCZ

							break;
							case 2: //EZ

							break;
						}
						currentRoom4++;
					break;
				}
				if (roomArray[x][y]==nullptr) {
					addRandomRoom(x,y,roomTemp[x][y].type,roomTemp[x][y].angle,getZone(y));
				}
			} else if (roomTemp[x][y].angle==127) {
				if (getZone(y)==1) {
					roomArray[x][y] = checkpoint1::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),0);
				} else {
					roomArray[x][y] = checkpoint2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),0);
				}
			}
		}
		prevZone = getZone(y);
	}
}
