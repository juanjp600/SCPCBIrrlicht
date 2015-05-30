#include "RMesh.h"
#include "../World.h"

#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>

bool CustomMaterialCombinerCallback(btManifoldPoint& cp,const btCollisionObjectWrapper* colObj0Wrap,int partId0,int index0,const btCollisionObjectWrapper* colObj1Wrap,int partId1,int index1)
{

    std::cout<<"CUSTOM CALLBACK\n"; std::terminate();

    btAdjustInternalEdgeContacts(cp,colObj1Wrap,colObj0Wrap, partId1,index1);
    //btAdjustInternalEdgeContacts(cp,colObj1,colObj0, partId1,index1, BT_TRIANGLE_CONVEX_BACKFACE_MODE);
    //btAdjustInternalEdgeContacts(cp,colObj1,colObj0, partId1,index1, BT_TRIANGLE_CONVEX_DOUBLE_SIDED+BT_TRIANGLE_CONCAVE_DOUBLE_SIDED);

	float friction0 = colObj0Wrap->getCollisionObject()->getFriction();
	float friction1 = colObj1Wrap->getCollisionObject()->getFriction();
	float restitution0 = colObj0Wrap->getCollisionObject()->getRestitution();
	float restitution1 = colObj1Wrap->getCollisionObject()->getRestitution();

	/*if (colObj0Wrap->getCollisionObject()->getCollisionFlags() & btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK)
	{
		friction0 = 1.0;//partId0,index0
		restitution0 = 0.f;
	}
	if (colObj1Wrap->getCollisionObject()->getCollisionFlags() & btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK)
	{
		if (index1&1)
		{
			friction1 = 1.0f;//partId1,index1
		} else
		{
			friction1 = 0.f;
		}
		restitution1 = 0.f;
	}*/

	cp.m_combinedFriction = friction0*friction1;
	cp.m_combinedRestitution = restitution0*restitution1;

	//this return value is currently ignored, but to be on the safe side: return false if you don't calculate friction
	return true;
}

RMesh* loadRMesh(std::string path,irr::io::IFileSystem* fs,irr::scene::ISceneManager* smgr,irr::video::ITexture** reflection,irr::video::E_MATERIAL_TYPE* roomShaders,PlainLightShaderCallBack* plainLightCallback) {
    irr::video::IVideoDriver* driver = smgr->getVideoDriver();

    irr::io::IReadFile* file = fs->createAndOpenFile(path.c_str());

	if (!file) { std::cout<<"Could not load \""<<path<<"\""<<"\n"; return nullptr; }

    int readInt1;//,readInt2;
    int i1,i2,i3;
    unsigned char readChar1,readChar2,readChar3;
    unsigned char cr,cg,cb;
    float readFloat1,readFloat2;
    float fx,fy,fz;
    float fpitch,fyaw,froll;
    float fu1,fv1,fu2,fv2;

    std::string readString1,readString2,readString3;

    struct tempWaypoint {
		irr::core::vector3df pos;
		unsigned char connected[10];
		unsigned char id;
    };

	std::vector<tempWaypoint> tempWaypoints;
	tempWaypoints.clear();
	tempWaypoint newWP;

    readString1 = "";
    //int = 4 octets
    //char = 1 octet
    //float = 4 octets
    //string = int + char * value_of_int

    /*file->read(&readInt1,sizeof(int));
    for (int i=0;i<readInt1;i++) {
        file->read(&readChar1,sizeof(unsigned char));
        readString1+=readChar1;
    }*/
	for (int i=0;i<3;i++) {
        file->read(&readChar1,sizeof(unsigned char));
        readString1+=readChar1;
    }
    if (readString1=="RM2") { //this is a valid RMesh
        std::cout<<"Loading \""<<path<<"\"\n";
        RMesh* retRMesh = new RMesh;

        retRMesh->waypoints.clear();

        retRMesh->path = path;
        std::string lm1_path = "";
        std::string lm2_path = "";
        std::string lm3_path = "";
        std::string lm4_path = "";
        std::string lm5_path = "";
        std::string fnd = "_opt.rm2";
        int initialSize = path.size()-1;
        for (int i=initialSize;i>=0;i--) {
            if (path[i]!='\\' && path[i]!='/') {
                path.erase(i);
            } else {
                break;
            }
        }
        irr::scene::SMesh* mesh = new irr::scene::SMesh();
        irr::scene::SMeshBufferLightMap* bufLM = nullptr;
        irr::scene::SMeshBuffer* buf = nullptr;

        btTriangleMesh *pTriMesh = new btTriangleMesh();
		//pTriMesh->m_weldingThreshold = 3.f;

		struct loadedTexture {
            std::string name;
			irr::video::ITexture* tex;
			irr::video::ITexture* bump;
			unsigned char alpha;
		};

		std::vector<loadedTexture> loadedTextures;

		file->read(&readChar1,sizeof(unsigned char));
		for (unsigned char i=0;i<readChar1;i++) {
			readString1="";
			file->read(&readChar2,sizeof(unsigned char));
			for (unsigned char j=0;j<readChar2;j++) {
				file->read(&readChar3,sizeof(unsigned char));
				readString1+=readChar3;
			}
			loadedTexture newTex;
			newTex.tex = driver->getTexture((path+readString1).c_str());
			newTex.name = (path+readString1);
			newTex.bump = nullptr;
			file->read(&newTex.alpha,sizeof(unsigned char));

            driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false); //mipmaps are not good for bump maps
			if (readString1=="tilefloor.jpg") {
				newTex.bump = driver->getTexture("GFX/map/tilebump.jpg");
			} else if (readString1=="concretefloor.jpg") {
				newTex.bump = driver->getTexture("GFX/map/concretefloorbump.jpg");
			} else if (readString1=="whitewall.jpg") {
				newTex.bump = driver->getTexture("GFX/map/whitewallbump.jpg");
			} else if (readString1=="metal3.jpg") {
				newTex.bump = driver->getTexture("GFX/map/metal3bump.jpg");
			} else if (readString1=="vent.jpg") {
				newTex.bump = driver->getTexture("GFX/map/ventbump.jpg");
			} else if (readString1=="dirtymetal.jpg") {
				newTex.bump = driver->getTexture("GFX/map/dirtymetalbump.jpg");
			} else if (readString1=="misc.jpg") {
				newTex.bump = driver->getTexture("GFX/map/miscbump.jpg");
			} else if (readString1=="rockmoss.jpg") {
				newTex.bump = driver->getTexture("GFX/map/rockmossbump.jpg");
			} else if (readString1=="metalpanels.jpg") {
				newTex.bump = driver->getTexture("GFX/map/metalpanelsbump.jpg");
			} else if (readString1=="metalpanels2.jpg") {
				newTex.bump = driver->getTexture("GFX/map/metalpanels2bump.jpg");
			} else if (readString1=="concretewall.jpg") {
				newTex.bump = driver->getTexture("GFX/map/concretewallbump.jpg");
			}

			/*if (readString1.find("_lm1")!=std::string::npos) {
                lm1_path = retRMesh->path;
                lm1_path.replace(lm1_path.find(fnd),fnd.length(),"_lm1.png");
			}
			if (readString1.find("_lm2")!=std::string::npos) {
                lm2_path = retRMesh->path;
                lm2_path.replace(lm2_path.find(fnd),fnd.length(),"_lm2.png");
			}
			if (readString1.find("_lm3")!=std::string::npos) {
                lm3_path = retRMesh->path;
                lm3_path.replace(lm3_path.find(fnd),fnd.length(),"_lm3.png");
			}
			if (readString1.find("_lm4")!=std::string::npos) {
                lm4_path = retRMesh->path;
                lm4_path.replace(lm4_path.find(fnd),fnd.length(),"_lm4.png");
			}
			if (readString1.find("_lm5")!=std::string::npos) {
                lm5_path = retRMesh->path;
                lm5_path.replace(lm5_path.find(fnd),fnd.length(),"_lm5.png");
			}*/
			driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);

			loadedTextures.push_back(newTex);
		}

		unsigned char entAmount = 0;
        file->read(&entAmount,sizeof(unsigned char));

        for (unsigned char i=0;i<entAmount;i++) {
            /*file->read(&readInt1,sizeof(int));
            readString1="";
            for (int j=0;j<readInt1;j++) {
                file->read(&readChar1,sizeof(unsigned char));
                readString1+=readChar1;
            }*/
            irr::video::SColorf lightColor; pointLight newLight;
            std::vector<irr::video::S3DVertex2TCoords> vertices;
			std::vector<irr::core::vector3di> indices;
            unsigned char entType;
            entType = 10;
            file->read(&entType,sizeof(unsigned char));
            char blendType = 0;
			unsigned char textures[2];
			std::cout<<(int)entType<<"\n";
            switch (entType) {
				case 0: //solid, drawn
				case 1: //not solid, drawn
					file->read(&readChar3,sizeof(unsigned char));

					for (unsigned char k=0;k<readChar3;k++) {
						vertices.resize(0); indices.resize(0);
						blendType = 0;
						textures[0]=0; textures[1]=0;

						file->read(&readChar1,sizeof(unsigned char));
						if (readChar1 & 1) {
							file->read(&readChar2,sizeof(unsigned char));
							textures[0] = readChar2;

							if (blendType!=1) { //alpha blending is final
								blendType = 2;
								if (loadedTextures[textures[0]-1].alpha==3) blendType = 1;
							}
						}
						if (readChar1 & 2) {
							file->read(&readChar2,sizeof(unsigned char));
							textures[1] = readChar2;

							if (blendType!=1) { //alpha blending is final
								blendType = 2;
								if (loadedTextures[textures[1]-1].alpha==3) blendType = 1;
							}
						}

						file->read(&readInt1,sizeof(int));
						for (int j=0;j<readInt1;j++) { //vertices
							file->read(&fx,sizeof(float));
							file->read(&fy,sizeof(float));
							file->read(&fz,sizeof(float));

							file->read(&fu1,sizeof(float));
							file->read(&fv1,sizeof(float));
							file->read(&fu2,sizeof(float));
							file->read(&fv2,sizeof(float));
							fu1 = std::round(fu1*1024.f)/1024.f;
							fv1 = std::round(fv1*1024.f)/1024.f;
							fu2 = std::round(fu2*1024.f)/1024.f;
							fv2 = std::round(fv2*1024.f)/1024.f;

							file->read(&cr,sizeof(unsigned char));
							file->read(&cg,sizeof(unsigned char));
							file->read(&cb,sizeof(unsigned char));
							vertices.push_back(irr::video::S3DVertex2TCoords(fx*RoomScale*0.1f,fy*RoomScale*0.1f,fz*RoomScale*0.1f,0,0,0,irr::video::SColor(255,cr,cg,cb),fu1,fv1,fu2,fv2));
						}

						file->read(&readInt1,sizeof(int));
						for (int j=0;j<readInt1;j++) { //polys
							file->read(&i1,sizeof(int));
							file->read(&i2,sizeof(int));
							file->read(&i3,sizeof(int));

							indices.push_back(irr::core::vector3di(i1,i2,i3));

							float ax1 = vertices[i1].Pos.X; float ay1 = vertices[i1].Pos.Y; float az1 = vertices[i1].Pos.Z;
							float ax2 = vertices[i2].Pos.X; float ay2 = vertices[i2].Pos.Y; float az2 = vertices[i2].Pos.Z;
							float ax3 = vertices[i3].Pos.X; float ay3 = vertices[i3].Pos.Y; float az3 = vertices[i3].Pos.Z;
							/*if (ay1>-1.f && ay1<1.f && ay2>-1.f && ay2<1.f && ay3>-1.f && ay3<1.f) {
                                ay1 = 0.f;
                                ay2 = 0.f;
                                ay3 = 0.f;
							}*/

                            irr::core::vector3df triangleCenter(ax1,ay1,az1);
                            triangleCenter+=irr::core::vector3df(ax2,ay2,az2);
                            triangleCenter+=irr::core::vector3df(ax3,ay3,az3);
                            triangleCenter*=1.f/3.f;

                            irr::core::vector3df v1(ax1,ay1,az1);
                            irr::core::vector3df v2(ax2,ay2,az2);
                            irr::core::vector3df v3(ax3,ay3,az3);
                            /*if (v1.getDistanceFrom(triangleCenter)>=1.f) {
                                float m1 = (v1.getDistanceFrom(triangleCenter)+1.1f)/v1.getDistanceFrom(triangleCenter);
                                v1 = (v1*m1)+(triangleCenter*(1.f-m1));
                            }
                            if (v2.getDistanceFrom(triangleCenter)>=1.f) {
                                float m2 = (v2.getDistanceFrom(triangleCenter)+1.1f)/v2.getDistanceFrom(triangleCenter);
                                v2 = (v2*m2)+(triangleCenter*(1.f-m2));
                            }
                            if (v3.getDistanceFrom(triangleCenter)>=1.f) {
                                float m3 = (v3.getDistanceFrom(triangleCenter)+1.1f)/v3.getDistanceFrom(triangleCenter);
                                v3 = (v3*m3)+(triangleCenter*(1.f-m3));
                            }*/
                            float x1 = v1.X; float y1 = v1.Y; float z1 = v1.Z;
                            float x2 = v2.X; float y2 = v2.Y; float z2 = v2.Z;
                            float x3 = v3.X; float y3 = v3.Y; float z3 = v3.Z;

							float nx1 = (y3-y1)*(z2-z1)-(y2-y1)*(z3-z1);
							float ny1 = (z3-z1)*(x2-x1)-(z2-z1)*(x3-x1);
							float nz1 = (x3-x1)*(y2-y1)-(x2-x1)*(y3-y1);
							float fac1=sqrt((nx1*nx1)+(ny1*ny1)+(nz1*nz1));
							nx1 = -nx1/fac1;
							ny1 = -ny1/fac1;
							nz1 = -nz1/fac1;

							vertices[i1].Normal.set(nx1,ny1,nz1);
							vertices[i2].Normal.set(nx1,ny1,nz1);
							vertices[i3].Normal.set(nx1,ny1,nz1);

							if (entType==0) {
								btVector3 btVertices[3];
								if (ny1<-0.9f) {
                                    btVertices[0] = btVector3(x1,y1,z1);
                                    btVertices[1] = btVector3(x2,y1,z2);
                                    btVertices[2] = btVector3(x3,y1,z3);
                                } else if (ny1>0.9f) {
                                    btVertices[0] = btVector3(x1,y1,z1);
                                    btVertices[1] = btVector3(x2,y1,z2);
                                    btVertices[2] = btVector3(x3,y1,z3);
                                } else {
                                    btVertices[0] = btVector3(x1,y1,z1);
                                    btVertices[1] = btVector3(x2,y2,z2);
                                    btVertices[2] = btVector3(x3,y3,z3);
								}
								//faces that barely make a difference to the shape of the object are removed
								if (btVertices[0].distance(btVertices[1])>5.f || btVertices[1].distance(btVertices[2])>5.f || btVertices[0].distance(btVertices[2])>5.f) {
									pTriMesh->addTriangle(btVertices[0], btVertices[1], btVertices[2],true);
								}
							}
						}

						if (indices.size()!=0) {
							switch (blendType) {
								case 2: //opaque
									if (textures[0]!=0 && textures[1]!=0) { //lightmapped
										bufLM = new irr::scene::SMeshBufferLightMap();

										mesh->addMeshBuffer(bufLM);

										bufLM->drop();

										bufLM->Vertices.reallocate(vertices.size());
										bufLM->Vertices.set_used(vertices.size());

										for (unsigned int j=0;j<vertices.size();j++) {
											bufLM->Vertices[j]=vertices[j];
										}

										bufLM->Indices.reallocate(indices.size()*3);
										bufLM->Indices.set_used(indices.size()*3);
										for (unsigned int j=0;j<indices.size();j++) {
											bufLM->Indices[j*3]=indices[j].X;
											bufLM->Indices[(j*3)+1]=indices[j].Y;
											bufLM->Indices[(j*3)+2]=indices[j].Z;
										}

										bufLM->getMaterial().setTexture(0,loadedTextures[textures[1]-1].tex);
										bufLM->getMaterial().setTexture(1,loadedTextures[textures[0]-1].tex);

										bufLM->getMaterial().MaterialType = irr::video::EMT_LIGHTMAP;
										if (loadedTextures[textures[1]-1].bump!=nullptr) {
											bufLM->getMaterial().MaterialType = roomShaders[1];
											bufLM->getMaterial().setTexture(2,loadedTextures[textures[1]-1].bump);
                                            bufLM->getMaterial().setTexture(3,reflection[0]);

                                            std::string lmString = loadedTextures[textures[0]-1].name;
                                            lmString.replace(lmString.find("lm"),2,"lm_t_");
                                            //bufLM->getMaterial().setTexture(4,driver->getTexture(lmString.c_str()));
                                            lmString = loadedTextures[textures[0]-1].name;
                                            lmString.replace(lmString.find("lm"),2,"lm_b_");
                                            //bufLM->getMaterial().setTexture(5,driver->getTexture(lmString.c_str()));
										} else {
                                            //using a shader is faster than defaulting to fixed pipeline
                                            bufLM->getMaterial().MaterialType = roomShaders[1];
										}

										bufLM->recalculateBoundingBox();
										//bufLM->getMaterial().setTexture(1,reflection[1]);
										//bufLM->getMaterial().setTexture(0,reflection[0]);
									} else { //only one texture
										buf = new irr::scene::SMeshBuffer();
										buf->getMaterial().MaterialType = roomShaders[2];

										mesh->addMeshBuffer(buf);

										buf->drop();

										buf->Vertices.reallocate(vertices.size());
										buf->Vertices.set_used(vertices.size());

										for (unsigned int j=0;j<vertices.size();j++) {
											buf->Vertices[j]=vertices[j];
										}

										buf->Indices.reallocate(indices.size()*3);
										buf->Indices.set_used(indices.size()*3);
										for (unsigned int j=0;j<indices.size();j++) {
											buf->Indices[j*3]=indices[j].X;
											buf->Indices[(j*3)+1]=indices[j].Y;
											buf->Indices[(j*3)+2]=indices[j].Z;
										}

										if (textures[1]!=0) {
											buf->getMaterial().setTexture(0,loadedTextures[textures[1]-1].tex);
										} else {
											buf->getMaterial().setTexture(0,loadedTextures[textures[0]-1].tex);
										}

										buf->getMaterial().Lighting = false;

										//buf->getMaterial().MaterialType = irr::video::EMT_SOLID;

										buf->recalculateBoundingBox();
										//buf->getMaterial().setTexture(1,reflection[1]);
										//buf->getMaterial().setTexture(0,reflection[0]);
									}
								break;
								case 1: //alpha
									/*buf = new irr::scene::SMeshBuffer();
									buf->getMaterial().MaterialType = roomShaders[3];

									mesh->addMeshBuffer(buf);

									buf->drop();

									buf->Vertices.reallocate(vertices.size());
									buf->Vertices.set_used(vertices.size());

									for (unsigned int j=0;j<vertices.size();j++) {
										buf->Vertices[j]=vertices[j];
									}

									buf->Indices.reallocate(indices.size()*3);
									buf->Indices.set_used(indices.size()*3);
									for (unsigned int j=0;j<indices.size();j++) {
										buf->Indices[j*3]=indices[j].X;
										buf->Indices[(j*3)+1]=indices[j].Y;
										buf->Indices[(j*3)+2]=indices[j].Z;
									}

									if (textures[1]!=0) {
										buf->getMaterial().setTexture(0,loadedTextures[textures[1]-1].tex);
									} else {
										buf->getMaterial().setTexture(0,loadedTextures[textures[0]-1].tex);
									}
									//buf->getMaterial().setTexture(1,driver->getTexture(textureRead[0].c_str()));

									buf->getMaterial().Lighting = false;

									//buf->getMaterial().MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;

									buf->getMaterial().BackfaceCulling = false;

									buf->recalculateBoundingBox();*/
								break;
							}
						}
					}
				break;
				case 2: //solid, not drawn
					//file->read(&readChar1,sizeof(unsigned char));

					//for (unsigned k=0;k<readChar1;k++) {
					{
						file->read(&readInt1,sizeof(int)); //vertices

						std::vector<btVector3> vertices;

						for (int j=0;j<readInt1;j++) {
							file->read(&fx,sizeof(float));
							file->read(&fy,sizeof(float));
							file->read(&fz,sizeof(float));

							fx*=RoomScale*0.1; fy*=RoomScale*0.1; fz*=RoomScale*0.1;
							vertices.push_back(btVector3(fx,fy,fz));
						}

						file->read(&readInt1,sizeof(int)); //polys
						for (int j=0;j<readInt1;j++) {
							file->read(&i1,sizeof(int));
							file->read(&i2,sizeof(int));
							file->read(&i3,sizeof(int));

							if (vertices[i1].distance(vertices[i2])>5.f || vertices[i2].distance(vertices[i3])>5.f || vertices[i1].distance(vertices[i3])>5.f) {
								pTriMesh->addTriangle(vertices[i1], vertices[i2], vertices[i3],true);
							}
						}
					}
					//\}
				break;
				case 3: //screen
					file->read(&fx,sizeof(float));
					file->read(&fy,sizeof(float));
					file->read(&fz,sizeof(float));

					file->read(&readChar2,sizeof(unsigned char));
					readString2="";
					for (unsigned char j=0;j<readChar2;j++) {
						file->read(&readChar1,sizeof(unsigned char));
						readString2+=readChar1;
					}
				break;
				case 4: //waypoint
					file->read(&fx,sizeof(float));
					file->read(&fy,sizeof(float));
					file->read(&fz,sizeof(float));

					newWP.pos = irr::core::vector3df(fx*RoomScale*0.1f,fy*RoomScale*0.1f,fz*RoomScale*0.1f);

					file->read(&readChar1,sizeof(unsigned char));
					for (unsigned char j=0;j<10;j++) {
						newWP.connected[j]=0;
					}
					std::cout<<"   "<<(int)readChar1<<"\n";
					for (unsigned char j=0;j<readChar1;j++) {
						file->read(&readChar2,sizeof(unsigned char));
						//std::cout<<"      "<<(int)readChar2<<"\n";
						newWP.connected[j]=readChar2;
					}
					tempWaypoints.push_back(newWP);
				break;
				case 5: //light
					file->read(&fx,sizeof(float));
					file->read(&fy,sizeof(float));
					file->read(&fz,sizeof(float));

					file->read(&fu1,sizeof(float)); //range

					cr=0;cg=0;cb=0;
					file->read(&cr,sizeof(unsigned char));
					file->read(&cg,sizeof(unsigned char));
					file->read(&cb,sizeof(unsigned char));

					file->read(&fv1,sizeof(float)); //intensity

					newLight.intensity = fv1;
					newLight.color.r = float(cr)/255.f;
					newLight.color.g = float(cg)/255.f;
					newLight.color.b = float(cb)/255.f;
					newLight.color.a = 1.f;
					newLight.radius = fu1*0.05f*RoomScale;
					newLight.position = irr::core::vector3df(fx*0.1f*RoomScale,fy*0.1f*RoomScale,fz*0.1f*RoomScale);

					newLight.viewMatrix[0].buildCameraLookAtMatrixLH(newLight.position,newLight.position+irr::core::vector3df(0.f,0.f,1.f),irr::core::vector3df(0.f,1.f,0.f));
                    newLight.viewMatrix[1].buildCameraLookAtMatrixLH(newLight.position,newLight.position+irr::core::vector3df(1.f,0.f,0.f),irr::core::vector3df(0.f,1.f,0.f));
                    newLight.viewMatrix[2].buildCameraLookAtMatrixLH(newLight.position,newLight.position+irr::core::vector3df(0.f,0.f,-1.f),irr::core::vector3df(0.f,1.f,0.f));
                    newLight.viewMatrix[3].buildCameraLookAtMatrixLH(newLight.position,newLight.position+irr::core::vector3df(-1.f,0.f,0.f),irr::core::vector3df(0.f,1.f,0.f));
                    newLight.viewMatrix[4].buildCameraLookAtMatrixLH(newLight.position,newLight.position+irr::core::vector3df(0.f,1.f,0.f),irr::core::vector3df(0.f,0.f,1.f));
                    newLight.viewMatrix[5].buildCameraLookAtMatrixLH(newLight.position,newLight.position+irr::core::vector3df(0.f,-1.f,0.f),irr::core::vector3df(0.f,0.f,1.f));

					retRMesh->pointlights.push_back(newLight);
				break;
				case 6: //spotlight //TODO: add spotlights to the lighting shader
					file->read(&fx,sizeof(float));
					file->read(&fy,sizeof(float));
					file->read(&fz,sizeof(float));

					file->read(&fu1,sizeof(float)); //range

					cr=0;cg=0;cb=0;
					file->read(&cr,sizeof(unsigned char));
					file->read(&cg,sizeof(unsigned char));
					file->read(&cb,sizeof(unsigned char));

					file->read(&fv1,sizeof(float)); //intensity

					file->read(&fpitch,sizeof(float));
					file->read(&fyaw,sizeof(float));
					file->read(&froll,sizeof(float));

					file->read(&readFloat1,sizeof(float)); //inner cone angle
					file->read(&readFloat2,sizeof(float)); //outer cone angle
				break;
				case 7: //soundemitter"
					file->read(&fx,sizeof(float));
					file->read(&fy,sizeof(float));
					file->read(&fz,sizeof(float));

					file->read(&readChar2,sizeof(unsigned char));

					file->read(&fu1,sizeof(float)); //range
				break;
				default:
					std::cout<<"Unknown RMesh entity: \""<<(int)readChar1<<"\"\n";
					std::terminate();
				break;
			}
        }

        /*if (tempWaypoints.size()<=0) {
            newWP.pos = irr::core::vector3df(0.f,0.f,0.f);
            for (unsigned char j=0;j<10;j++) {
                newWP.connected[j]=0;
            }
            tempWaypoints.push_back(newWP);
        }*/

		for (unsigned char i=0;i<tempWaypoints.size();i++) {
			RMesh::waypoint* finalWP;
			finalWP = new RMesh::waypoint;
			finalWP->position = tempWaypoints[i].pos;
			retRMesh->waypoints.push_back(finalWP);
		}

		for (unsigned char i=0;i<retRMesh->waypoints.size();i++) {
			for (unsigned char j=0;j<20;j++) {
				retRMesh->waypoints[i]->connected[j]=nullptr;
			}
			unsigned char shift = 0;
			while (retRMesh->waypoints[i]->connected[shift]!=nullptr) { shift++; }
			for (unsigned char j=0;j<10;j++) {
				if (tempWaypoints[i].connected[j+shift]==0) { break; }
				retRMesh->waypoints[i]->connected[j+shift]=retRMesh->waypoints[tempWaypoints[i].connected[j]-1];
				unsigned char shift2 = 0;
				while (retRMesh->waypoints[i]->connected[j+shift]->connected[shift2]!=nullptr) {
                    shift2++;
                    if (shift2>=20) { break; }
                }
                if (shift2<20) {
                    retRMesh->waypoints[i]->connected[j+shift]->connected[shift2]=retRMesh->waypoints[i];
                } else {
                    std::cout<<"error connecting waypoints: too many connections\n";
                }
			}
		}

		buf = new irr::scene::SMeshBuffer();
        mesh->addMeshBuffer(buf);
        buf->drop();

		std::vector<irr::video::S3DVertex> vertices;
		std::vector<unsigned short> indices;

		for (unsigned char i=0;i<retRMesh->waypoints.size();i++) {
            for (unsigned char j=0;j<10;j++) {
                if (retRMesh->waypoints[i]->connected[j]!=nullptr) {
                    irr::video::SColor colr(255,rand()%256,rand()%256,rand()%256);
                    vertices.push_back(irr::video::S3DVertex(retRMesh->waypoints[i]->position,irr::core::vector3df(0.f,1.f,0.f),colr,irr::core::vector2df(0.f,0.f)));
                    vertices.push_back(irr::video::S3DVertex(retRMesh->waypoints[i]->connected[j]->position,irr::core::vector3df(0.f,1.f,0.f),colr,irr::core::vector2df(0.f,0.f)));
                    vertices.push_back(irr::video::S3DVertex(retRMesh->waypoints[i]->position+irr::core::vector3df(0.f,0.f,0.001f),irr::core::vector3df(0.f,1.f,0.f),colr,irr::core::vector2df(0.f,0.f)));
                    indices.push_back(vertices.size()-1);
                    indices.push_back(vertices.size()-2);
                    indices.push_back(vertices.size()-3);
                }
            }
		}

		buf->Vertices.reallocate(vertices.size());
        buf->Vertices.set_used(vertices.size());

        for (unsigned int j=0;j<vertices.size();j++) {
            buf->Vertices[j]=vertices[j];
        }

        buf->getMaterial().Wireframe = true;
        buf->getMaterial().Lighting = false;
        buf->getMaterial().setTexture(0, 0);

        buf->Indices.reallocate(indices.size());
        buf->Indices.set_used(indices.size());
        for (unsigned int j=0;j<indices.size();j++) {
            buf->Indices[j]=indices[j];
        }

        mesh->addMeshBuffer(buf);

        mesh->recalculateBoundingBox();
        mesh->setHardwareMappingHint(irr::scene::EHM_STATIC);
        smgr->getMeshManipulator()->recalculateNormals(mesh);
        retRMesh->mesh = mesh;
        retRMesh->shape = new btBvhTriangleMeshShape(pTriMesh,true);
        retRMesh->shape->setMargin(0.25f);
        /*btTriangleInfoMap* triangleInfoMap = new btTriangleInfoMap();
        triangleInfoMap->m_edgeDistanceThreshold = 5.0f;
		btGenerateInternalEdgeInfo(retRMesh->shape, triangleInfoMap);*/

        file->drop();

        //irr::scene::ICameraSceneNode* oldCam = irrSmgr->getActiveCamera();
        //oldCam->render();
#if 0
        for (int usetbn=0;usetbn<3;usetbn++) {
            switch (usetbn) {
                case 0:
                    plainLightCallback->useTBN.X = 0.f;
                    plainLightCallback->useTBN.Y = 0.f;
                    plainLightCallback->useTBN.Z = 1.f;
                break;
                case 1:
                    plainLightCallback->useTBN.X = 1.f;
                    plainLightCallback->useTBN.Y = 0.f;
                    plainLightCallback->useTBN.Z = 0.f;
                break;
                case 2:
                    plainLightCallback->useTBN.X = 0.f;
                    plainLightCallback->useTBN.Y = 1.f;
                    plainLightCallback->useTBN.Z = 0.f;
                break;
            }
            for (int lmi=0;lmi<5;lmi++) {
                std::string lmString;
                switch (lmi) {
                    case 0:
                        lmString = lm1_path;
                    break;
                    case 1:
                        lmString = lm2_path;
                    break;
                    case 2:
                        lmString = lm3_path;
                    break;
                    case 3:
                        lmString = lm4_path;
                    break;
                    case 4:
                        lmString = lm5_path;
                    break;
                }
                if (lmString=="") { continue; }
                irr::video::ITexture* lightPass[2];
                lightPass[0] = reflection[0];
                lightPass[1] = reflection[2];
                irr::video::ITexture* lightDepth = reflection[1];

                driver->setRenderTarget(lightPass[0],true,true,irr::video::SColor(255,0,0,0));

                plainLightCallback->pass = 0;
                plainLightCallback->setLights(retRMesh->pointlights);

                smgr->fastDrawAll_init(irr::core::vector3df(0.f,0.f,0.f));

                for (int i=0;i<plainLightCallback->lightList.size();i++) {
                    irr::core::vector3df lightPos = irr::core::vector3df(plainLightCallback->lightList[i].pos[0],plainLightCallback->lightList[i].pos[1],plainLightCallback->lightList[i].pos[2]);
                    //smCamera->setPosition(irr::core::vector3df(plainLightCallback->lightList[i].pos[0],plainLightCallback->lightList[i].pos[1],plainLightCallback->lightList[i].pos[2]));
                    //if (lightPos.getDistanceFromSQ(mainPlayer->getPosition())<300.f*300.f&&mainPlayer->seesBoundingBox(irr::core::aabbox3df(lightPos-irr::core::vector3df(130.f,130.f,130.0),lightPos+irr::core::vector3df(130.f,130.f,130.0)),*mainPlayer->getViewFrustum())) {
                    irr::core::matrix4 projectionMatrix;
                    projectionMatrix.buildProjectionMatrixPerspectiveFovLH(90.f*irr::core::DEGTORAD,1.f,1.f,150.f);
                    for (int j=0;j<6;j++) {
                        driver->setRenderTarget(lightDepth,true,true,irr::video::SColor(255,255,0,0));
                        //irrSmgr->setActiveCamera(smCamera);
                        driver->getOverrideMaterial().EnableFlags = irr::video::EMF_MATERIAL_TYPE;
                        driver->getOverrideMaterial().EnablePasses = irr::scene::ESNRP_SOLID;
                        driver->getOverrideMaterial().Material.MaterialType = roomShaders[4];
                        //irrDriver->setViewPort(irr::core::recti(0,0,128,128));
                        /*switch (j) {
                            case 0:
                                //smCamera->setRotation(irr::core::vector3df(0.f,0.f,0.f));
                                smCamera->setUpVector(irr::core::vector3df(0.f,1.f,0.f));
                                smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(0.f,0.f,1.f));
                            break;
                            case 1:
                                smCamera->setUpVector(irr::core::vector3df(0.f,1.f,0.f));
                                smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(1.f,0.f,0.f));
                                //smCamera->setRotation(irr::core::vector3df(0.f,90.f,0.f));
                            break;
                            case 2:
                                smCamera->setUpVector(irr::core::vector3df(0.f,1.f,0.f));
                                smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(0.f,0.f,-1.f));
                                //smCamera->setRotation(irr::core::vector3df(0.f,180.f,0.f));
                            break;
                            case 3:
                                smCamera->setUpVector(irr::core::vector3df(0.f,1.f,0.f));
                                smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(-1.f,0.f,0.f));
                                //smCamera->setRotation(irr::core::vector3df(0.f,270.f,0.f));
                            break;
                            case 4:
                                smCamera->setUpVector(irr::core::vector3df(0.f,0.f,1.f));
                                smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(0.f,1.f,0.f));
                                //smCamera->setRotation(irr::core::vector3df(90.f,0.f,0.f));
                            break;
                            case 5:
                                smCamera->setUpVector(irr::core::vector3df(0.f,0.f,1.f));
                                smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(0.f,-1.f,0.f));
                                //smCamera->setRotation(irr::core::vector3df(-90.f,0.f,0.f));
                            break;
                        }*/
                        //smCamera->OnRegisterSceneNode();

                        driver->setTransform(irr::video::ETS_PROJECTION,projectionMatrix);
                        driver->setTransform(irr::video::ETS_VIEW,plainLightCallback->lightList[i].viewMatrix[j]);
                        irr::core::matrix4 mat;
                        mat.setRotationDegrees(irr::core::vector3df(0.f,0.f,0.f));
                        mat.setTranslation(irr::core::vector3df(0.f,0.f,0.f));
                        driver->setTransform(irr::video::ETS_WORLD,mat);

                        for (irr::u32 k=0; k<retRMesh->mesh->getMeshBufferCount(); ++k)
                        {
                            irr::scene::IMeshBuffer* mb = retRMesh->mesh->getMeshBuffer(k);
                            if (mb)
                            {
                                const irr::video::SMaterial& material = mb->getMaterial();

                                irr::video::IMaterialRenderer* rnd = driver->getMaterialRenderer(material.MaterialType);
                                bool transparent = (rnd && rnd->isTransparent());

                                if (transparent == false)
                                {
                                    driver->setMaterial(driver->getOverrideMaterial().Material);
                                    driver->drawMeshBuffer(mb);
                                }
                            }
                        }
                        //irrSmgr->fastDrawAll(projectionMatrix,plainLightCallback->lightList[i].viewMatrix[j]);

                        plainLightCallback->lightMatrix = projectionMatrix;//smCamera->getProjectionMatrix();
                        plainLightCallback->lightMatrix *= plainLightCallback->lightList[i].viewMatrix[j];//smCamera->getViewMatrix();

                        driver->setTransform(irr::video::ETS_WORLD,mat);

                        driver->setRenderTarget(lightPass[1],true,true,irr::video::SColor(255,0,0,0));

                        for (irr::u32 k=0; k<retRMesh->mesh->getMeshBufferCount(); ++k)
                        {
                            irr::scene::IMeshBuffer* mb = retRMesh->mesh->getMeshBuffer(k);
                            if (mb)
                            {
                                irr::video::SMaterial material = mb->getMaterial();

                                irr::video::IMaterialRenderer* rnd = driver->getMaterialRenderer(material.MaterialType);
                                bool transparent = (rnd && rnd->isTransparent());

                                if (material.getTexture(1)!=nullptr) {
                                    if (transparent == false && std::string(mb->getMaterial().getTexture(1)->getName().getPath().c_str()).find(lmString.c_str())!=std::string::npos)
                                    {
                                        material.setTexture(0,reflection[0]);
                                        material.setTexture(1,reflection[1]);
                                        driver->setMaterial(material);
                                        driver->drawMeshBuffer(mb);
                                    }
                                }
                            }
                        }

                        //irrDriver->setViewPort(irr::core::recti(0,0,mainWidth,mainHeight));
                        driver->getOverrideMaterial().EnablePasses = 0;
                        //irrSmgr->setActiveCamera(oldCam);
                        //irrSmgr->fastDrawAll(oldCam->getProjectionMatrix(),oldCam->getViewMatrix());

                        driver->setRenderTarget(lightPass[0],false,false,irr::video::SColor(255,0,0,0));
                        driver->draw2DImage(lightPass[1],irr::core::vector2di(0,0));
                        //}
                    }
                    plainLightCallback->pass++;
                }

                smgr->fastDrawAll_end();
                //mainPlayer->setVisible(true);
                //smCamera->setVisible(false);
                driver->getOverrideMaterial().EnablePasses = 0;

                void* imageData = lightPass[0]->lock(irr::video::ETLM_READ_ONLY);
                irr::video::IImage* im = driver->createImageFromData(lightPass[0]->getColorFormat(), lightPass[0]->getSize(), imageData);
                if (usetbn==1) {
                    lmString.replace(lmString.find("lm"),2,"lm_t_");
                }
                if (usetbn==2) {
                    lmString.replace(lmString.find("lm"),2,"lm_b_");
                }
                driver->writeImageToFile(im, lmString.c_str());
                im->drop();
                lightPass[0]->unlock();

                //driver->removeTexture(lightPass[0]);
                //driver->removeTexture(lightPass[1]);
                //driver->removeTexture(lightDepth);
            }
        }
#endif

        return retRMesh;

    } else {
        std::cout<<"\""<<path<<"\" isn't a valid RMesh";
    }

    std::cout<<"reached end\n";

    file->drop();
    return nullptr;

}
