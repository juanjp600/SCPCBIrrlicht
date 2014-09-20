#include "rmesh.h"
#include "../3dworld.h"

RMesh* loadRMesh(std::string path,irr::io::IFileSystem* fs,irr::video::IVideoDriver* driver) {

    irr::io::IReadFile* file = fs->createAndOpenFile(path.c_str());

    int readInt1,readInt2;
    int i1,i2,i3;
    char readChar1,readChar2;
    unsigned char cr,cg,cb;
    float fx,fy,fz;
    float fu1,fv1,fu2,fv2;

    std::string readString1,readString2,readString3;

    readString1 = "";
    //int = 4 octets
    //char = 1 octet
    //float = 4 octets
    //string = int + char * value_of_int
    file->read(&readInt1,sizeof(int));
    for (int i=0;i<readInt1;i++) {
        file->read(&readChar1,sizeof(char));
        readString1+=readChar1;
    }
    if (readString1=="RoomMesh") { //this is a valid RMesh
        std::cout<<"Loading \""<<path<<"\"\n";
        RMesh* retRMesh = new RMesh;

        retRMesh->path = path;
        int initialSize = path.size()-1;
        for (int i=initialSize;i>=0;i--) {
            if (path[i]!='\\' && path[i]!='/') {
                path.erase(i);
            } else {
                break;
            }
        }
        std::cout<<path<<"\n";
        irr::scene::SMesh* mesh = new irr::scene::SMesh();
        irr::scene::SMeshBufferLightMap* bufLM = nullptr;
        irr::scene::SMeshBuffer* buf = nullptr;

        btTriangleMesh *pTriMesh = new btTriangleMesh();

        file->read(&readInt1,sizeof(int)); //drawn mesh

        for (int i=0;i<readInt1;i++) {

            std::vector<irr::video::S3DVertex2TCoords> vertices;
            std::vector<irr::core::vector3di> indices;

            char blendType = 0;
            std::string textureRead[2];
            for (int j=0;j<2;j++) {
                textureRead[j]="";
                file->read(&readChar1,sizeof(char));
                if (readChar1!=0) {
                    file->read(&readInt2,sizeof(int));
                    readString1="";
                    for (int i=0;i<readInt2;i++) {
                        file->read(&readChar2,sizeof(char));
                        readString1+=readChar2;
                    }
                    textureRead[j] = path+readString1;
                    std::cout<<readString1<<"\n";

                    if (blendType!=1) { //alpha blending is final
                        blendType = 2;
                        if (readChar1==3) blendType = 1;
                    }
                }
            }

            file->read(&readInt2,sizeof(int));
            for (int j=0;j<readInt2;j++) { //vertices
                file->read(&fx,sizeof(float));
                file->read(&fy,sizeof(float));
                file->read(&fz,sizeof(float));

                file->read(&fu1,sizeof(float));
                file->read(&fv1,sizeof(float));
                file->read(&fu2,sizeof(float));
                file->read(&fv2,sizeof(float));

                file->read(&cr,sizeof(unsigned char));
                file->read(&cg,sizeof(unsigned char));
                file->read(&cb,sizeof(unsigned char));
                vertices.push_back(irr::video::S3DVertex2TCoords(fx*RoomScale*0.1,fy*RoomScale*0.1,fz*RoomScale*0.1,0,0,0,irr::video::SColor(255,cr,cg,cb),fu1,fv1,fu2,fv2));
            }

            file->read(&readInt2,sizeof(int));
            for (int j=0;j<readInt2;j++) { //polys
                file->read(&i1,sizeof(int));
                file->read(&i2,sizeof(int));
                file->read(&i3,sizeof(int));

                indices.push_back(irr::core::vector3di(i1,i2,i3));

                btVector3 btVertices[3];
                //round the values to hide seams
                btVertices[0] = btVector3((float)(int)vertices[i1].Pos.X,(float)(int)vertices[i1].Pos.Y,(float)(int)vertices[i1].Pos.Z);
                btVertices[1] = btVector3((float)(int)vertices[i2].Pos.X,(float)(int)vertices[i2].Pos.Y,(float)(int)vertices[i2].Pos.Z);
                btVertices[2] = btVector3((float)(int)vertices[i3].Pos.X,(float)(int)vertices[i3].Pos.Y,(float)(int)vertices[i3].Pos.Z);
                pTriMesh->addTriangle(btVertices[0], btVertices[1], btVertices[2]);
            }

            if (indices.size()!=0) {
                switch (blendType) {
                    case 2: //opaque
                        if (textureRead[0]!=path && textureRead[1]!=path) { //lightmapped
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

                            bufLM->getMaterial().setTexture(0,driver->getTexture(textureRead[1].c_str()));
                            bufLM->getMaterial().setTexture(1,driver->getTexture(textureRead[0].c_str()));

                            //bufLM->getMaterial().Lighting = false;

                            bufLM->getMaterial().MaterialType = irr::video::EMT_LIGHTMAP;

                            bufLM->recalculateBoundingBox();
                        } else { //only one texture
                            buf = new irr::scene::SMeshBuffer();

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

                            if (textureRead[1]!=path) {
                                buf->getMaterial().setTexture(0,driver->getTexture(textureRead[1].c_str()));
                            } else {
                                buf->getMaterial().setTexture(0,driver->getTexture(textureRead[0].c_str()));
                            }

                            buf->getMaterial().Lighting = false;

                            buf->getMaterial().MaterialType = irr::video::EMT_SOLID;

                            buf->recalculateBoundingBox();
                        }
                    break;
                    case 1: //alpha
                        buf = new irr::scene::SMeshBuffer();

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

                        if (textureRead[1]!=path) {
                            buf->getMaterial().setTexture(0,driver->getTexture(textureRead[1].c_str()));
                        } else {
                            buf->getMaterial().setTexture(0,driver->getTexture(textureRead[0].c_str()));
                        }
                        //buf->getMaterial().setTexture(1,driver->getTexture(textureRead[0].c_str()));

                        buf->getMaterial().Lighting = false;

                        buf->getMaterial().MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;

                        buf->getMaterial().BackfaceCulling = false;

                        buf->recalculateBoundingBox();
                    break;
                }
            }
        }

        file->read(&readInt1,sizeof(int)); //invisible collision mesh

        for (int i=0;i<readInt1;i++) {
            file->read(&readInt2,sizeof(int)); //vertices

			std::vector<btVector3> vertices;

            for (int j=0;j<readInt2;j++) {
                file->read(&fx,sizeof(float));
                file->read(&fy,sizeof(float));
                file->read(&fz,sizeof(float));

				fx=(float)(int)fx;fy=(float)(int)fy;fz=(float)(int)fz; //round the values to hide seams
				fx*=RoomScale*0.1; fy*=RoomScale*0.1; fz*=RoomScale*0.1;
                vertices.push_back(btVector3(fx,fy,fz));
                //now do something with that
            }

            file->read(&readInt2,sizeof(int)); //polys
            for (int j=0;j<readInt2;j++) {
                file->read(&i1,sizeof(int));
                file->read(&i2,sizeof(int));
                file->read(&i3,sizeof(int));

                pTriMesh->addTriangle(vertices[i1], vertices[i2], vertices[i3]);
                //now do something with that
            }
        }

        file->read(&readInt1,sizeof(int)); //other entities

        for (int i=0;i<readInt1;i++) {
            file->read(&readInt2,sizeof(int));
            readString1="";
            for (int j=0;j<readInt2;j++) {
                file->read(&readChar1,sizeof(char));
                readString1+=readChar1;
            }
            std::cout<<readString1<<"\n";
            if (readString1=="screen") {
                file->read(&fx,sizeof(float));
                file->read(&fy,sizeof(float));
                file->read(&fz,sizeof(float));

                file->read(&readInt2,sizeof(int));
                readString2="";
                for (int j=0;j<readInt2;j++) {
                    file->read(&readChar1,sizeof(char));
                    readString2+=readChar1;
                }
            } else if (readString1=="waypoint") {
                file->read(&fx,sizeof(float));
                file->read(&fy,sizeof(float));
                file->read(&fz,sizeof(float));
            } else if (readString1=="light") {
                file->read(&fx,sizeof(float));
                file->read(&fy,sizeof(float));
                file->read(&fz,sizeof(float));

                file->read(&fu1,sizeof(float)); //range

                file->read(&readInt2,sizeof(int));
                readString2="";
                cr=0;cg=0;cb=0;
                char loadedColors=0;
                //TODO: get rid of this parsing stuff and make the RMesh
                //converter save the values as chars directly
                for (int j=0;j<readInt2;j++) {
                    file->read(&readChar1,sizeof(char));
                    if (readChar1!=' ') {
                        readString2+=readChar1;
                    } else {
                        switch (loadedColors) {
                            case 0: //red
                                cr = (unsigned char)(std::stoi(readString2));
                                loadedColors = 1;
                                readString2="";
                            break;
                            case 1: //green
                                cg = (unsigned char)(std::stoi(readString2));
                                loadedColors = 2;
                                readString2="";
                            break;
                            case 2: //blue
                                cb = (unsigned char)(std::stoi(readString2));
                                loadedColors = 3;
                                readString2="";
                            break;
                        }
                    }
                }
                if (readString2!="") {
                    cb = (unsigned char)(std::stoi(readString2));
                    readString2="";
                }

                file->read(&fv1,sizeof(float)); //intensity

                irr::video::SColorf lightColor = irr::video::SColorf(float(cr)/255.0*fv1,float(cg)/255.0*fv1,float(cb)/255.0*fv1);
                irr::video::SLight newLight;
                newLight.DiffuseColor = lightColor;
                newLight.Radius = fu1;
                newLight.Position = irr::core::vector3df(fx,fy,fz);

                retRMesh->pointlights.push_back(newLight);
            } else if (readString1=="spotlight") { //TODO: add spotlights to the lighting shader
                file->read(&fx,sizeof(float));
                file->read(&fy,sizeof(float));
                file->read(&fz,sizeof(float));

                file->read(&fu1,sizeof(float)); //range

                file->read(&readInt2,sizeof(int)); //colors
                readString2="";
                for (int j=0;j<readInt2;j++) {
                    file->read(&readChar1,sizeof(char));
                    readString2+=readChar1;
                }

                file->read(&fv1,sizeof(float)); //intensity

                file->read(&readInt2,sizeof(int)); //angles
                readString3="";
                for (int j=0;j<readInt2;j++) {
                    file->read(&readChar1,sizeof(char));
                    readString3+=readChar1;
                }

                file->read(&i1,sizeof(int)); //inner cone angle
                file->read(&i2,sizeof(int)); //outer cone angle
            } else if (readString1=="soundemitter") {
                file->read(&fx,sizeof(float));
                file->read(&fy,sizeof(float));
                file->read(&fz,sizeof(float));

                file->read(&i1,sizeof(int));

                file->read(&fu1,sizeof(float)); //range
            } else if (readString1=="playerstart") {

            } else {
                std::cout<<"Unknown RMesh entity: \""<<readString1<<"\"\n";
            }
        }

        mesh->recalculateBoundingBox();
        //mesh->setDirty();
        retRMesh->mesh = mesh;
        retRMesh->shape = new btBvhTriangleMeshShape(pTriMesh, true);
        retRMesh->shape->setMargin(0.0f);
        file->drop();
        return retRMesh;

    } else {
        std::cout<<"\""<<path<<"\" isn't a valid RMesh";
    }

    file->drop();
    return nullptr;//retRMesh;

}

/*Reference code:

SMesh* Mesh = new SMesh();

SMeshBuffer *buf = 0;
buf = new SMeshBuffer();
Mesh->addMeshBuffer(buf);
buf->drop();

buf->Vertices.reallocate(3);
buf->Vertices.set_used(3);

buf->Vertices[0] = S3DVertex(0,0,10, 1,1,0,    video::SColor(255,0,255,255), 0, 1);
buf->Vertices[1] = S3DVertex(10,0,-10, 1,0,0,  video::SColor(255,255,0,255), 1, 1);
buf->Vertices[2] = S3DVertex(0,20,0, 0,1,1,    video::SColor(255,255,255,0), 1, 0);

buf->Indices.reallocate(3);
buf->Indices.set_used(3);

buf->Indices[0]=0;
buf->Indices[1]=1;
buf->Indices[2]=2;

buf->recalculateBoundingBox();


IMeshSceneNode* myNode = irrScene -> addMeshSceneNode(Mesh);

myNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
myNode->setMaterialFlag(EMF_LIGHTING, false);
myNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
myNode->setMaterialTexture(0, irrDriver->getTexture("rust0.jpg"));*/
