#include "RMesh.h"
#include "../World.h"

//#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>

RMesh* loadRMesh(std::string path,irr::io::IFileSystem* fs,irr::scene::ISceneManager* smgr,irr::video::ITexture** roomTextures,irr::video::E_MATERIAL_TYPE* roomShaders) {
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

    struct transparentMesh {
        irr::scene::SMesh* mesh;
        irr::core::vector3df normal;
        irr::core::vector3df minXYZ;
        irr::core::vector3df maxXYZ;
        irr::core::vector3df center;
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
    if (readString1=="RM2") { //this is a valid RM2
        std::cout<<"Loading \""<<path<<"\"\n";
        RMesh* retRMesh = new RMesh;

        retRMesh->waypoints.clear();

        retRMesh->path = path;

        unsigned int initialSize = path.size();
        for (int i=initialSize;i>=0;i--) {
            if (path[i]!='\\' && path[i]!='/') {
                path.erase(i);
            } else {
                break;
            }
        }
        irr::scene::SMesh* mesh = new irr::scene::SMesh();
        irr::scene::CMeshBuffer<irr::video::S3DVertex2TCoords>* bufLM = nullptr;
        irr::scene::CMeshBuffer<irr::video::S3DVertex>* buf = nullptr;

        std::vector<transparentMesh> transparentMeshes; //transparent meshes need to be sorted

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
								if (btVertices[0].distance(btVertices[1])>5.f*RoomScale || btVertices[1].distance(btVertices[2])>5.f*RoomScale || btVertices[0].distance(btVertices[2])>5.f*RoomScale) {
									pTriMesh->addTriangle(btVertices[0], btVertices[1], btVertices[2],true);
								}
							}
						}

						if (indices.size()!=0) {
							switch (blendType) {
								case 2: //opaque
									if (textures[0]!=0 && textures[1]!=0) { //lightmapped
										bufLM = new irr::scene::CMeshBuffer<irr::video::S3DVertex2TCoords>(driver->getVertexDescriptor(1));
										irr::scene::CVertexBuffer<irr::video::S3DVertex2TCoords>* VertexBuffer = new irr::scene::CVertexBuffer<irr::video::S3DVertex2TCoords>();
                                        irr::scene::CIndexBuffer* IndexBuffer = new irr::scene::CIndexBuffer(irr::video::EIT_16BIT);

										mesh->addMeshBuffer(bufLM);

										bufLM->drop();

                                        for (unsigned int j = 0; j<vertices.size(); ++j) {
                                            VertexBuffer->addVertex(vertices[j]);
                                        }
										/*bufLM->Vertices.reallocate(vertices.size());
										bufLM->Vertices.set_used(vertices.size());

										for (unsigned int j=0;j<vertices.size();j++) {
											bufLM->Vertices[j]=vertices[j];
										}*/

                                        for (unsigned int j = 0; j<indices.size(); ++j) {
                                            IndexBuffer->addIndex(indices[j].X);
                                            IndexBuffer->addIndex(indices[j].Y);
                                            IndexBuffer->addIndex(indices[j].Z);
                                        }
										/*bufLM->Indices.reallocate(indices.size()*3);
										bufLM->Indices.set_used(indices.size()*3);
										for (unsigned int j=0;j<indices.size();j++) {
											bufLM->Indices[j*3]=indices[j].X;
											bufLM->Indices[(j*3)+1]=indices[j].Y;
											bufLM->Indices[(j*3)+2]=indices[j].Z;
										}*/

										bufLM->setVertexBuffer(VertexBuffer, 0);
                                        bufLM->setIndexBuffer(IndexBuffer);

										bufLM->getMaterial().setTexture(0,loadedTextures[textures[1]-1].tex);
										bufLM->getMaterial().setTexture(1,loadedTextures[textures[0]-1].tex);

										bufLM->getMaterial().MaterialType = irr::video::EMT_LIGHTMAP;
										if (loadedTextures[textures[1]-1].bump!=nullptr) {
											bufLM->getMaterial().MaterialType = roomShaders[0];
											bufLM->getMaterial().setTexture(2,loadedTextures[textures[1]-1].bump);
                                            bufLM->getMaterial().setTexture(3,roomTextures[0]); //reflection
                                            bufLM->getMaterial().setTexture(4,roomTextures[1]); //fog texture
										} else {
                                            //using a shader is faster than defaulting to fixed pipeline
                                            bufLM->getMaterial().MaterialType = roomShaders[1];
                                            bufLM->getMaterial().setTexture(2,roomTextures[1]); //fog texture
										}

										bufLM->recalculateBoundingBox();
									} else { //only one texture
										buf = new irr::scene::CMeshBuffer<irr::video::S3DVertex>(driver->getVertexDescriptor(0));
										irr::scene::CVertexBuffer<irr::video::S3DVertex>* VertexBuffer = new irr::scene::CVertexBuffer<irr::video::S3DVertex>();
                                        irr::scene::CIndexBuffer* IndexBuffer = new irr::scene::CIndexBuffer(irr::video::EIT_16BIT);

										mesh->addMeshBuffer(buf);

										buf->drop();

										buf->getMaterial().MaterialType = roomShaders[2];

                                        for (unsigned int j = 0; j<vertices.size(); ++j) {
                                            VertexBuffer->addVertex(vertices[j]);
                                        }
                                        for (unsigned int j = 0; j<indices.size(); ++j) {
                                            IndexBuffer->addIndex(indices[j].X);
                                            IndexBuffer->addIndex(indices[j].Y);
                                            IndexBuffer->addIndex(indices[j].Z);
                                        }

										buf->setVertexBuffer(VertexBuffer, 0);
                                        buf->setIndexBuffer(IndexBuffer);

										if (textures[1]!=0) {
											buf->getMaterial().setTexture(0,loadedTextures[textures[1]-1].tex);
										} else {
											buf->getMaterial().setTexture(0,loadedTextures[textures[0]-1].tex);
										}
										buf->getMaterial().setTexture(1,roomTextures[1]); //fog texture

										buf->getMaterial().Lighting = false;

										//buf->getMaterial().MaterialType = irr::video::EMT_SOLID;

										buf->recalculateBoundingBox();
									}
								break;
								case 1: //alpha
									//buf = new irr::scene::CMeshBuffer<irr::video::S3DVertex>(driver->getVertexDescriptor(0));
                                    irr::scene::CVertexBuffer<irr::video::S3DVertex>* VertexBuffer;// = new irr::scene::CVertexBuffer<irr::video::S3DVertex>();
                                    irr::scene::CIndexBuffer* IndexBuffer;// = new irr::scene::CIndexBuffer(irr::video::EIT_16BIT);
									//buf->getMaterial().MaterialType = roomShaders[3];

									//mesh->addMeshBuffer(buf);

									//buf->drop();

									//for (unsigned int j = 0; j<vertices.size(); ++j) {
                                        //VertexBuffer->addVertex(vertices[j]);
                                    //}
                                    for (unsigned int j = 0; j<indices.size(); ++j) {
                                        float ax1 = vertices[indices[j].X].Pos.X; float ay1 = vertices[indices[j].X].Pos.Y; float az1 = vertices[indices[j].X].Pos.Z;
                                        float ax2 = vertices[indices[j].Y].Pos.X; float ay2 = vertices[indices[j].Y].Pos.Y; float az2 = vertices[indices[j].Y].Pos.Z;
                                        float ax3 = vertices[indices[j].Z].Pos.X; float ay3 = vertices[indices[j].Z].Pos.Y; float az3 = vertices[indices[j].Z].Pos.Z;

                                        irr::core::vector3df triangleCenter(ax1,ay1,az1);
                                        triangleCenter+=irr::core::vector3df(ax2,ay2,az2);
                                        triangleCenter+=irr::core::vector3df(ax3,ay3,az3);
                                        triangleCenter*=1.f/3.f;

                                        irr::core::vector3df v1(ax1,ay1,az1);
                                        irr::core::vector3df v2(ax2,ay2,az2);
                                        irr::core::vector3df v3(ax3,ay3,az3);

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
                                        irr::core::vector3df normal(nx1,ny1,nz1);

                                        bool addNewMesh = true;
                                        for (unsigned int k=0;k<transparentMeshes.size();k++) {
                                            if (normal.equals(transparentMeshes[k].normal,0.1f) || normal.equals(-transparentMeshes[k].normal,0.1f)) { //normals are parallel
                                                if (std::abs(normal.dotProduct((triangleCenter-transparentMeshes[k].center).normalize()))<0.2f) { //normal is perpendicular to distance vector between new triangle and mesh center
                                                    irr::scene::IMeshBuffer* buf2 = transparentMeshes[k].mesh->getMeshBuffer(0);
                                                    irr::core::aabbox3df aabb = buf2->getBoundingBox();
                                                    if (aabb.isPointInside(vertices[indices[j].X].Pos) || aabb.isPointInside(vertices[indices[j].Y].Pos) || aabb.isPointInside(vertices[indices[j].Z].Pos)) { //new triangle is within mesh bounding box
                                                        VertexBuffer = static_cast<irr::scene::CVertexBuffer<irr::video::S3DVertex>*>(buf->getVertexBuffer(0));
                                                        IndexBuffer = static_cast<irr::scene::CIndexBuffer*>(buf->getIndexBuffer());
                                                        VertexBuffer->addVertex(vertices[indices[j].X]);
                                                        VertexBuffer->addVertex(vertices[indices[j].Y]);
                                                        VertexBuffer->addVertex(vertices[indices[j].Z]);
                                                        IndexBuffer->addIndex(VertexBuffer->getVertexCount()-3);
                                                        IndexBuffer->addIndex(VertexBuffer->getVertexCount()-2);
                                                        IndexBuffer->addIndex(VertexBuffer->getVertexCount()-1);

                                                        transparentMeshes[k].maxXYZ.X = std::max(vertices[indices[j].X].Pos.X,transparentMeshes[k].maxXYZ.X);
                                                        transparentMeshes[k].maxXYZ.X = std::max(vertices[indices[j].Y].Pos.X,transparentMeshes[k].maxXYZ.X);
                                                        transparentMeshes[k].maxXYZ.X = std::max(vertices[indices[j].Z].Pos.X,transparentMeshes[k].maxXYZ.X);

                                                        transparentMeshes[k].maxXYZ.Y = std::max(vertices[indices[j].X].Pos.Y,transparentMeshes[k].maxXYZ.Y);
                                                        transparentMeshes[k].maxXYZ.Y = std::max(vertices[indices[j].Y].Pos.Y,transparentMeshes[k].maxXYZ.Y);
                                                        transparentMeshes[k].maxXYZ.Y = std::max(vertices[indices[j].Z].Pos.Y,transparentMeshes[k].maxXYZ.Y);

                                                        transparentMeshes[k].maxXYZ.Z = std::max(vertices[indices[j].X].Pos.Z,transparentMeshes[k].maxXYZ.Z);
                                                        transparentMeshes[k].maxXYZ.Z = std::max(vertices[indices[j].Y].Pos.Z,transparentMeshes[k].maxXYZ.Z);
                                                        transparentMeshes[k].maxXYZ.Z = std::max(vertices[indices[j].Z].Pos.Z,transparentMeshes[k].maxXYZ.Z);

                                                        transparentMeshes[k].minXYZ.X = std::min(vertices[indices[j].X].Pos.X,transparentMeshes[k].minXYZ.X);
                                                        transparentMeshes[k].minXYZ.X = std::min(vertices[indices[j].Y].Pos.X,transparentMeshes[k].minXYZ.X);
                                                        transparentMeshes[k].minXYZ.X = std::min(vertices[indices[j].Z].Pos.X,transparentMeshes[k].minXYZ.X);

                                                        transparentMeshes[k].minXYZ.Y = std::min(vertices[indices[j].X].Pos.Y,transparentMeshes[k].minXYZ.Y);
                                                        transparentMeshes[k].minXYZ.Y = std::min(vertices[indices[j].Y].Pos.Y,transparentMeshes[k].minXYZ.Y);
                                                        transparentMeshes[k].minXYZ.Y = std::min(vertices[indices[j].Z].Pos.Y,transparentMeshes[k].minXYZ.Y);

                                                        transparentMeshes[k].minXYZ.Z = std::min(vertices[indices[j].X].Pos.Z,transparentMeshes[k].minXYZ.Z);
                                                        transparentMeshes[k].minXYZ.Z = std::min(vertices[indices[j].Y].Pos.Z,transparentMeshes[k].minXYZ.Z);
                                                        transparentMeshes[k].minXYZ.Z = std::min(vertices[indices[j].Z].Pos.Z,transparentMeshes[k].minXYZ.Z);

                                                        transparentMeshes[k].center = (transparentMeshes[k].minXYZ+transparentMeshes[k].maxXYZ)*0.5f;

                                                        buf->recalculateBoundingBox();

                                                        addNewMesh = false;
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if (addNewMesh) {
                                            std::cout<<"NEWMESH\n";
                                            buf = new irr::scene::CMeshBuffer<irr::video::S3DVertex>(driver->getVertexDescriptor(0));
                                            VertexBuffer = new irr::scene::CVertexBuffer<irr::video::S3DVertex>();
                                            IndexBuffer = new irr::scene::CIndexBuffer(irr::video::EIT_16BIT);

                                            VertexBuffer->addVertex(vertices[indices[j].X]);
                                            VertexBuffer->addVertex(vertices[indices[j].Y]);
                                            VertexBuffer->addVertex(vertices[indices[j].Z]);
                                            IndexBuffer->addIndex(VertexBuffer->getVertexCount()-3);
                                            IndexBuffer->addIndex(VertexBuffer->getVertexCount()-2);
                                            IndexBuffer->addIndex(VertexBuffer->getVertexCount()-1);

                                            transparentMesh newTPMesh;
                                            newTPMesh.mesh = new irr::scene::SMesh();
                                            newTPMesh.normal = normal;
                                            newTPMesh.maxXYZ = vertices[indices[j].X].Pos;
                                            newTPMesh.minXYZ = vertices[indices[j].X].Pos;

                                            newTPMesh.maxXYZ.X = std::max(vertices[indices[j].X].Pos.X,newTPMesh.maxXYZ.X);
                                            newTPMesh.maxXYZ.X = std::max(vertices[indices[j].Y].Pos.X,newTPMesh.maxXYZ.X);
                                            newTPMesh.maxXYZ.X = std::max(vertices[indices[j].Z].Pos.X,newTPMesh.maxXYZ.X);

                                            newTPMesh.maxXYZ.Y = std::max(vertices[indices[j].X].Pos.Y,newTPMesh.maxXYZ.Y);
                                            newTPMesh.maxXYZ.Y = std::max(vertices[indices[j].Y].Pos.Y,newTPMesh.maxXYZ.Y);
                                            newTPMesh.maxXYZ.Y = std::max(vertices[indices[j].Z].Pos.Y,newTPMesh.maxXYZ.Y);

                                            newTPMesh.maxXYZ.Z = std::max(vertices[indices[j].X].Pos.Z,newTPMesh.maxXYZ.Z);
                                            newTPMesh.maxXYZ.Z = std::max(vertices[indices[j].Y].Pos.Z,newTPMesh.maxXYZ.Z);
                                            newTPMesh.maxXYZ.Z = std::max(vertices[indices[j].Z].Pos.Z,newTPMesh.maxXYZ.Z);

                                            newTPMesh.minXYZ.X = std::min(vertices[indices[j].X].Pos.X,newTPMesh.minXYZ.X);
                                            newTPMesh.minXYZ.X = std::min(vertices[indices[j].Y].Pos.X,newTPMesh.minXYZ.X);
                                            newTPMesh.minXYZ.X = std::min(vertices[indices[j].Z].Pos.X,newTPMesh.minXYZ.X);

                                            newTPMesh.minXYZ.Y = std::min(vertices[indices[j].X].Pos.Y,newTPMesh.minXYZ.Y);
                                            newTPMesh.minXYZ.Y = std::min(vertices[indices[j].Y].Pos.Y,newTPMesh.minXYZ.Y);
                                            newTPMesh.minXYZ.Y = std::min(vertices[indices[j].Z].Pos.Y,newTPMesh.minXYZ.Y);

                                            newTPMesh.minXYZ.Z = std::min(vertices[indices[j].X].Pos.Z,newTPMesh.minXYZ.Z);
                                            newTPMesh.minXYZ.Z = std::min(vertices[indices[j].Y].Pos.Z,newTPMesh.minXYZ.Z);
                                            newTPMesh.minXYZ.Z = std::min(vertices[indices[j].Z].Pos.Z,newTPMesh.minXYZ.Z);

                                            newTPMesh.center = (newTPMesh.minXYZ+newTPMesh.maxXYZ)*0.5f;

                                            buf->setVertexBuffer(VertexBuffer, 0);
                                            buf->setIndexBuffer(IndexBuffer);

                                            buf->getMaterial().MaterialType = roomShaders[3];

                                            newTPMesh.mesh->addMeshBuffer(buf);

                                            if (textures[1]!=0) {
                                                buf->getMaterial().setTexture(0,loadedTextures[textures[1]-1].tex);
                                            } else {
                                                buf->getMaterial().setTexture(0,loadedTextures[textures[0]-1].tex);
                                            }
                                            buf->getMaterial().setTexture(1,roomTextures[1]); //fog texture
                                            //buf->getMaterial().setFlag(irr::video::EMF_ZWRITE_ENABLE, true);

                                            buf->getMaterial().Lighting = false;

                                            buf->getMaterial().BackfaceCulling = false;

                                            buf->recalculateBoundingBox();

                                            buf->drop();

                                            newTPMesh.mesh->recalculateBoundingBox();
                                            newTPMesh.mesh->setHardwareMappingHint(irr::scene::EHM_STATIC);

                                            transparentMeshes.push_back(newTPMesh);
                                        }
                                        /*IndexBuffer->addIndex(indices[j].X);
                                        IndexBuffer->addIndex(indices[j].Y);
                                        IndexBuffer->addIndex(indices[j].Z);*/
                                    }

                                    for (unsigned int k=0;k<transparentMeshes.size();k++) {
                                        transparentMesh& TPMesh = transparentMeshes[k];

                                        retRMesh->transparentSurfaces.push_back(TPMesh.mesh);
                                        retRMesh->transparentOffset.push_back(TPMesh.center);

                                        irr::scene::IVertexBuffer* vbuf = TPMesh.mesh->getMeshBuffer(0)->getVertexBuffer(0);
                                        irr::video::S3DVertex* vbufverts = static_cast<irr::video::S3DVertex*>(vbuf->getVertices());
                                        std::cout<<TPMesh.center.X<<" "<<TPMesh.center.Y<<" "<<TPMesh.center.Z<<"\n";
                                        for (unsigned int l=0;l<vbuf->getVertexCount();l++) {
                                            vbufverts[l].Pos-=TPMesh.center;
                                        }

                                        TPMesh.mesh->getMeshBuffer(0)->recalculateBoundingBox();
                                        TPMesh.mesh->recalculateBoundingBox();
                                    }

                                    transparentMeshes.clear();

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
					newLight.radius = fu1*0.15f*RoomScale;
					newLight.position = irr::core::vector3df(fx*0.1f*RoomScale,fy*0.1f*RoomScale,fz*0.1f*RoomScale);

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
					std::cout<<"Unknown RM2 entity: \""<<(int)readChar1<<"\"\n";
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

        buf = new irr::scene::CMeshBuffer<irr::video::S3DVertex>(driver->getVertexDescriptor(0));
        irr::scene::CVertexBuffer<irr::video::S3DVertex>* vertexBuffer = new irr::scene::CVertexBuffer<irr::video::S3DVertex>();
        irr::scene::CIndexBuffer* indexBuffer = new irr::scene::CIndexBuffer(irr::video::EIT_16BIT);

        mesh->addMeshBuffer(buf);

        buf->drop();

        mesh->recalculateBoundingBox();
        mesh->setHardwareMappingHint(irr::scene::EHM_STATIC);
        smgr->getMeshManipulator()->recalculateNormals(mesh);
        retRMesh->mesh = mesh;
        retRMesh->shape = new btBvhTriangleMeshShape(pTriMesh,true);
        retRMesh->shape->setMargin(0.25f*RoomScale);
        /*btTriangleInfoMap* triangleInfoMap = new btTriangleInfoMap();
        triangleInfoMap->m_edgeDistanceThreshold = 5.0f;
		btGenerateInternalEdgeInfo(retRMesh->shape, triangleInfoMap);*/

        file->drop();

        //irr::scene::ICameraSceneNode* oldCam = irrSmgr->getActiveCamera();
        //oldCam->render();

        return retRMesh;

    } else {
        std::cout<<"\""<<path<<"\" isn't a valid RMesh";
    }

    std::cout<<"reached end\n";

    file->drop();
    return nullptr;

}
