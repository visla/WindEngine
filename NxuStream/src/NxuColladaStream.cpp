/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		CoreDumpStream.cpp

		Implementation of saving physics data to a binary stream.

		Changelist
		----------

*/


#include "NxuColladaStream.h"

static const char* XML_HEADER = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
static const char* XML_HEADER_MATCH = "<?xml version=\"1";

static const char* PHYSX_PROFILE_NAME = "PhysX";
static const char* PHYSX_SDK = "nxsdk";
static const char* PHYSX_PARAMETERS = "nxparameters";
static const char* PHYSX_PARAMETER_ID = "parameterId";
static const char* PHYSX_PAIRFLAG = "nxpairflag";
static const char* PHYSX_HEIGHTFIELD = "nxheightfield";
static const char* PHYSX_CCD_SKELETON = "nxccdskeleton";
static const char* PHYSX_EFFECTOR = "nxeffector";
static const char* PHYSX_FLUID = "nxfluid";
static const char* PHYSX_FLUID_EMITTER = "nxfluidemitter";
static const char* PHYSX_MODEL_DATA = "nxmodeldata";
static const char* PHYSX_TRIANGLE_MESH = "nxtrainglemesh";
static const char* PHYSX_CONVEX_MESH = "nxconvexmesh";
static const char* PHYSX_CONTRIBUTER = "PhysX Collada Exporter";

static const char* COLLADA_ROOT = "COLLADA";
static const char* COLLADA_TECHNIQUE = "technique";
static const char* COLLADA_TECHNIQUE_COMMON = "technique_common";
static const char* COLLADA_EXTRA = "extra";
static const char* COLLADA_LIBRARY_GEOMETRIES = "library_geometries";
static const char* COLLADA_LIBRARY_PHYSICS_MODELS = "library_physics_models";
static const char* COLLADA_LIBRARY_PHYSICS_SCENES = "library_physics_scenes";
static const char* COLLADA_LIBRARY_PHYSICS_MATERIALS = "library_physics_materials";
static const char* COLLADA_ID = "id";
static const char* COLLADA_SID = "sid";
static const char* COLLADA_URL = "url";
static const char* COLLADA_ASSET = "asset";
static const char* COLLADA_PROFILE = "profile";
static const char* COLLADA_GEOMETRY = "geometry";
static const char* COLLADA_MESH= "mesh";
static const char* COLLADA_CONVEX_MESH= "convex_mesh";
static const char* COLLADA_PHYSICS_MODEL = "physics_model";
static const char* COLLADA_RIGID_BODY = "rigid_body";
static const char* COLLADA_RIGID_CONSTRAINT  = "rigid_constraint";
static const char* COLLADA_INSTANCE_PHYSICS_MODEL  = "instance_physics_model";
static const char* COLLADA_PHYSICS_SCENE = "physics_scene";
static const char* COLLADA_PHYSICS_MATERIAL = "physics_material";
static const char* COLLADA_SHAPE = "shape";
static const char* COLLADA_CONTRIBUTER = "contributer";

NxuColladaStream::NxuColladaStream()
{
    mFilePointer = 0;
    mFileName[0] = 0;
    mIsOutputStream = false;
    mParamIndex = 0;
    sdkVersion = NXU_SDK_FEATURES; //TODO: Figure out how to dynamically store this in Collada files for import

    collada = 0;
    library_physics_scenes = 0;
    library_physics_models = 0;
    library_physics_materials = 0;
    library_geometries = 0;
    current_physics_scene = 0;
    current_physics_model = 0;
    current_physics_material = 0;
    current_rigid_body = 0;
    current_rigid_constraint = 0;
    current_shape = 0;
    current_geometry = 0;
    current_technique = 0;
    current_scene = 0;
    current_model = 0;
    current_fluid = 0;
    useTechnique = false;
}


//Returns true if the file was successfully opened
NX_BOOL NxuColladaStream::openFile(const char* filename, NX_BOOL writeable)
{
    NX_ASSERT(strlen(filename) < 1024);
    strncpy(mFileName, filename, 1024);
    FILE* file = fopen(mFileName, writeable ? "w" : "r");
    if (!writeable)
    {
        char tempBuffer[1024];
        fscanf(file, "%s\n", tempBuffer);
        NX_ASSERT(!strncmp(tempBuffer, XML_HEADER_MATCH, sizeof(XML_HEADER_MATCH)));
    }
    else
    {
        fprintf(file, XML_HEADER);
    }
    return setFilePointer(file, writeable);
}

void NxuColladaStream::closeFile()
{
    if (mFilePointer && mFileName[0] != 0) fclose(mFilePointer);
    mFilePointer = 0;
    mFileName[0] = 0;
}

//Returns true if the file pointer is valid
NX_BOOL NxuColladaStream::setFilePointer(FILE* file, NX_BOOL writeable)
{
    mFilePointer = file;
    if (!file) return false;
    mIsOutputStream = writeable;
    if (!writeable) 
    {
        nxmlNode* root = nxmlParseFile(file);
        if (!root) return false; //Invalid file
        nodestack.SetSize(0); //Clear the stack
        nodestack.Add(root);
        createLookup(root);
    } 
    else 
    {
        collada = nodestack.Add(new nxmlNode(COLLADA_ROOT));
        nxmlNode* asset = collada->children.Add(new nxmlNode(COLLADA_ASSET));
        nxmlNode* contributer = asset->children.Add(new nxmlNode(COLLADA_CONTRIBUTER));
        contributer->body = PHYSX_CONTRIBUTER;
    }
    return true;
}

NxuColladaStream::~NxuColladaStream()
{
    closeFile();
    int count = nxmlLookup.count;
    for (int i=0;i<count;++i)
    {
        delete nxmlLookup.Pop();
    }
    if (collada) delete collada;
    collada = 0;
}

void NxuColladaStream::flush()
{
    nxmlSaveFile(collada, mFilePointer);
}

//Finds the node with the specified id
nxmlNode* NxuColladaStream::lookupId(const char* id)
{
    int count = nxmlLookup.count;
    for (int i=0;i<count;++i)
    {
        if (nxmlLookup[i]->name == id)
        {
            return nxmlLookup[i]->node;
        }
    }
    return NULL;
}

//Creates a lookup table for finding nodes by id
//This is used to resolve url references
void NxuColladaStream::createLookup(nxmlNode* root)
{
    int count = root->children.count;
    for (int i=0;i<count;++i)
    {
        nxmlNode::Attribute* att;
        nxmlNode* child = root->children[i];
        if (att = child->hasAttribute(COLLADA_ID))
        {
            nxmlMap* urlMap = new nxmlMap();
            urlMap->name = att->value;
            urlMap->node = child;
            nxmlLookup.Add(urlMap);
        }
        createLookup(child);
    }
}

//Finds the next separator character
const char* findToken(const char* start)
{
    const char* token = start;
    //Find next break
    while (*token != 0 && *token != ' ' && *token != '\t' && *token != ',')
    {
        ++token;
    }

    return token;
}

//Skips separators
const char* skipWhitespace(const char* start)
{
    const char* token = start;
    //Find next break
    while (*token != 0 && (*token == ' ' || *token == '\t' || *token == ','))
    {
        ++token;
    }

    return token;
}

//Finds a child node under the specified node that is a PhysX technique
//PhysX techniques contain PhysX specific information
nxmlNode* findPhysXTechnique(nxmlNode* parent)
{
    nxmlNode* technique = NULL;
    for (int i=0;i<parent->children.count;++i)
    {
        technique = parent->children[i];
        if (technique->tag == COLLADA_TECHNIQUE)
        {
            nxmlNode::Attribute* att;
            if (att = technique->hasAttribute(COLLADA_PROFILE))
            {
                if (att->value == PHYSX_PROFILE_NAME)
                {
                    return technique;
                }
            }
        }
    }
    return NULL; 
}

//Finds a node referenced by URL within the same document
nxmlNode* NxuColladaStream::resolveURL(nxmlNode* node)
{
    nxmlNode* resolvedNode = node;
    nxmlNode::Attribute* att;
    if (att = node->hasAttribute(COLLADA_URL))
    {
        resolvedNode = lookupId(att->value);
    }

    return resolvedNode;
}

//Finds the value of the requested node
//Smartly traverses techniques and extra nodes as needed
const char* NxuColladaStream::getCurrentBody(const char* name)
{
    if (current_section)
    {
        nxmlNode* child = current_section->Child(name);
        if (child)
        {
            child = resolveURL(child);
            return (const char*)(child->body);
        }

        //Check for technique
        nxmlNode* technique = findPhysXTechnique(current_section);
        if (technique)
        {
            nxmlNode* child = technique->Child(name);
            if (child)
            {
                child = resolveURL(child);
                return (const char*)(child->body);
            }
        }

        //check for extra node
        nxmlNode* extra = current_section->Child(COLLADA_EXTRA);
        if (extra)
        {
            nxmlNode* technique = findPhysXTechnique(extra);
            
            if (technique)
            {
                nxmlNode* physxnode = technique->children[0];
                nxmlNode* valuenode = physxnode->Child(name);
                if (valuenode)
                {
                    valuenode = resolveURL(valuenode);
                    return (const char*)(valuenode->body);
                }
            }
        }
       
    }
    return NULL;
}

NxuColladaStream& NxuColladaStream::readBool(NX_BOOL& b, const char* name)								
{
    const char* body = getCurrentBody(name);
    if (stricmp(body, "true") == 0)
    {
        b = true;
    }
    else
    {
        b = false;
    }
    return *this;
}

NxuColladaStream& NxuColladaStream::readByte(NxU8& b, const char* name)								
{
    const char* body = getCurrentBody(name);
    if (body) 
    {
        b = (NxU8)atoi(body);
    }
    return *this;
}

NxuColladaStream& NxuColladaStream::readWord(NxU16& w, const char* name)								
{
    const char* body = getCurrentBody(name);
    if (body) 
    {
        w = (NxU16)atoi(body);
    }
    return *this;
}

NxuColladaStream& NxuColladaStream::readDword(NxU32& d, const char* name)								
{
    const char* body = getCurrentBody(name);
    if (body) 
    {
        d = (NxU32)atoi(body);
    }
    return *this;
}

NxuColladaStream& NxuColladaStream::readFloat(NxF32& f, const char* name)								
{
    const char* body = getCurrentBody(name);
    if (body) 
    {
        f = (NxF32)atof(body);
    }
    return *this;
}

NxuColladaStream& NxuColladaStream::readDouble(NxF64& d, const char* name)							
{
    const char* body = getCurrentBody(name);
    if (body) 
    {
        d = atof(body);
    }
    return *this;
}

template <class T> void readBufferInt(T* buffer, NxU32 size, const char* source)
{
    NxU32 count = 0;
    NxU32 intCount = size/sizeof(T);
    T* next = (T*)buffer;
    const char* token = skipWhitespace(source);
    while (*token != 0)
    {
        count++;
        if (count > intCount) break;
        *next = (T)atoi(token);
        token = skipWhitespace(findToken(token));
        ++next;
    }
}

template <class T> void readBufferFP(T* buffer, NxU32 size, const char* source)
{
    NxU32 count = 0;
    NxU32 floatCount = size/sizeof(T);
    T* next = (T*)buffer;
    const char* token = skipWhitespace(source);
    while (*token != 0)
    {
        count++;
        if (count > floatCount) break;
        *next = (T)atof(token);
        token = skipWhitespace(findToken(token));
        ++next;
    }
}

NxuColladaStream&  NxuColladaStream::readByteBuffer(void* buffer, NxU32 size, const char* name)	
{
    readBufferInt((NxU8*)buffer, size, getCurrentBody(name));
    return *this;
}

NxuColladaStream&  NxuColladaStream::readWordBuffer(void* buffer, NxU32 size, const char* name)	
{
    readBufferInt((NxU16*)buffer, size, getCurrentBody(name));
    return *this;
}

NxuColladaStream&  NxuColladaStream::readDwordBuffer(void* buffer, NxU32 size, const char* name)	
{
    readBufferInt((NxU32*)buffer, size, getCurrentBody(name));
    return *this;

}

NxuColladaStream&  NxuColladaStream::readFloatBuffer(void* buffer, NxU32 size, const char* name)	
{
    readBufferFP((NxF32*)buffer, size, getCurrentBody(name));
    return *this;

}

NxuColladaStream&  NxuColladaStream::readDoubleBuffer(void* buffer, NxU32 size, const char* name)	
{
    readBufferFP((NxF64*)buffer, size, getCurrentBody(name));
    return *this;
}

NxuColladaStream&	NxuColladaStream::readCharBuffer(void* buffer, NxU32 size, const char* name)
{
    if (current_section)
    {
        nxmlNode* child = current_section->Child(name);
        if (child)
        {
            
            nxString* string = &(child->body);
            NxU32 length = size;
            if (strlen(child->body) > length - 1)
            {
                string = new nxString(child->body);
                string[length-1] = 0;
            }
            sprintf((char*)buffer, *string);
        }
    }
    return *this;
}

//Places all stand alone child nodes on the stack for later processing
// Assets and Techniques are ignored since they are supplemental information for the current node
void NxuColladaStream::expandNode(nxmlNode* current)
{
    lastHeader = nodestack.count;

    int count = current->children.count;
    for (int i = count-1; i>=0; --i)
    {
        nxmlNode* node = current->children[i];
        if (node->tag != COLLADA_ASSET && node->tag != COLLADA_TECHNIQUE && node->tag != COLLADA_TECHNIQUE_COMMON)
        {
            nodestack.Add(node);
        }
    }
}

//Fill in the stream header for the current node so that the importer knows what the next element of the document is
void NxuColladaStream::fillHeader(NxuFormatHeader &header, NxuSectionTypes type, nxmlNode* current)
{
    nxmlNode::Attribute * id = current->hasAttribute(COLLADA_ID);

    header.sectionType = type;
    header.size = 0;
    header.name[0] = 0;
    if (id) 
    {
        if (strlen(id->value) < sizeof(header.name)) {
            sprintf(header.name, "%s", id->value);
        }
    }

    current_section = current;
}

//Search for the next node on the stack that represents a PhysX type and generate an appropriate header
void NxuColladaStream::readHeader(NxuFormatHeader &header)
{       
    if (nodestack.count == 0)
    {
        header.sectionType = NxuTypeUnknown;
        return;
    }
	//Search for section relative to current node
    nxmlNode* currentNode = current();
    nodestack.Pop();
    if (currentNode->tag == COLLADA_ROOT)
    {
        expandNode(currentNode);
        readHeader(header); //read subsection
        return;
    }
    
    if (currentNode->tag == COLLADA_LIBRARY_PHYSICS_MODELS)
    {
        expandNode(currentNode);
        readHeader(header); //read subsection
        return;
    }

    if (currentNode->tag == COLLADA_LIBRARY_PHYSICS_SCENES)
    {
        expandNode(currentNode);
        readHeader(header); //read subsection
        return;
    }

    if (currentNode->tag == COLLADA_LIBRARY_PHYSICS_MATERIALS)
    {
        expandNode(currentNode);
        readHeader(header); //read subsection
        return;
    }

    if (currentNode->tag == COLLADA_LIBRARY_GEOMETRIES)
    {
        expandNode(currentNode);
        readHeader(header); //read subsection
        return;
    }

    if (currentNode->tag == COLLADA_GEOMETRY)
    {
        expandNode(currentNode);
        readHeader(header); //read subsection
        return;
    }

    if (currentNode->tag == COLLADA_PHYSICS_MODEL)
    {
        expandNode(currentNode);
        fillHeader(header, NxuTypeModel, currentNode);
        return;
    }

    if (currentNode->tag == COLLADA_PHYSICS_SCENE)
    {
        expandNode(currentNode);
        fillHeader(header, NxuTypeScene, currentNode);
        return;
    }

    if (currentNode->tag == COLLADA_PHYSICS_MATERIAL)
    {
        fillHeader(header, NxuTypeMaterial, currentNode);
        return;
    }

    if (currentNode->tag == COLLADA_RIGID_BODY)
    {
        expandNode(currentNode);
        fillHeader(header, NxuTypeActor, currentNode);
        return;
    }

    if (currentNode->tag == COLLADA_SHAPE)
    {
        expandNode(currentNode);
        fillHeader(header, NxuTypeShape, currentNode);
        return;
    }

    if (currentNode->tag == COLLADA_RIGID_CONSTRAINT)
    {
        fillHeader(header, NxuTypeJoint, currentNode);
        return;
    }

    if (currentNode->tag == COLLADA_INSTANCE_PHYSICS_MODEL)
    {
        fillHeader(header, NxuTypeModelInstance, currentNode);
        return;
    }

    if (currentNode->tag == COLLADA_EXTRA)
    {
        nxmlNode* tech = currentNode->Child(COLLADA_TECHNIQUE);
        if (tech)
        {
            nxmlNode::Attribute* a = tech->hasAttribute(COLLADA_PROFILE);
            if (a && a->value == PHYSX_PROFILE_NAME)
            {
                int count = tech->children.count;
                for (int i=0;i<count;++i)
                {
                    nxmlNode* child = tech->children[i];
                    if (child->tag == PHYSX_SDK)
                    {
                        fillHeader(header, NxuTypeSDK, child);
                        mParamIndex = 0;
                        return;
                    }
                    if (child->tag == COLLADA_PHYSICS_MATERIAL)
                    {
                        fillHeader(header, NxuTypeMaterial, resolveURL(child));
                        return;
                    }
                    if (child->tag == PHYSX_PAIRFLAG)
                    {
                        fillHeader(header, NxuTypePairFlag, child);
                        return;
                    }
                    if (child->tag == PHYSX_HEIGHTFIELD)
                    {
                        fillHeader(header, NxuTypeHeightField, child);
                        return;
                    }
                    if (child->tag == PHYSX_CCD_SKELETON)
                    {
                        fillHeader(header, NxuTypeCCDSkeleton, child);
                        return;
                    }
                    if (child->tag == PHYSX_EFFECTOR)
                    {
                        fillHeader(header, NxuTypeEffector, child);
                        return;
                    }
                    if (child->tag == PHYSX_FLUID)
                    {
                        fillHeader(header, NxuTypeFluid, child);
                        int emitterCount = child->children.count;
                        for (int j=0;j<emitterCount;++j)
                        {
                            nxmlNode* emitter = child->children[j];
                            if (emitter->tag == PHYSX_FLUID_EMITTER)
                            {
                                nodestack.Add(emitter);
                            }
                        }
                        return;
                    }
                    if (child->tag == PHYSX_TRIANGLE_MESH)
                    {
                        fillHeader(header, NxuTypeTriangleMesh, child);
                        return;
                    }

                    if (child->tag == PHYSX_CONVEX_MESH)
                    {
                        fillHeader(header, NxuTypeConvexMesh, child);
                        return;
                    }
                }
            }
        }
    }

    //Unknown node type, try the next one
    readHeader(header);

    return;
}

//Remove children of current node from the stack
void NxuColladaStream::skip()
{
    while (lastHeader < nodestack.count)
    {
        nodestack.Pop();
    }
}

NxuColladaStream&  NxuColladaStream::readVector(NxVec3 &vec, const char* name)
{
    readFloatBuffer(&vec.x, 3*sizeof(vec.x), name);
    return *this;
}

NxuColladaStream&  NxuColladaStream::readQuat(NxQuat &quat, const char* name)
{
    readFloatBuffer(&quat.x, 4*sizeof(quat.x), name);
    return *this;
}

NxuColladaStream&  NxuColladaStream::readMatrix(NxMat34 &mat, const char* name)
{
	NxF32 m[12];
    readFloatBuffer(m, 12*sizeof(NxF32), name);
	mat.M.setRowMajor( m );
    mat.t.x = m[9];
    mat.t.y = m[10];
    mat.t.z = m[11];
    return *this;
}

NxuColladaStream&  NxuColladaStream::readParameter(NxuParameterData& param, const char* name)
{
    nxmlNode* params = current_section->Child(PHYSX_PARAMETERS);
    if (!params) return *this;
    nxmlNode* p = params->children[mParamIndex++];
    param.param = (NxParameter) atoi(p->attribute(PHYSX_PARAMETER_ID));
    param.value = (NxF32) atof(p->body);
    return *this;
}

void NxuColladaStream::addNode(const char* tag, const char* body)
{
    nxmlNode* node = new nxmlNode(tag);
    node->body += body;
    nxmlNode* parent = current_section;
    if (useTechnique)
    {
        parent = getTechnique(parent, PHYSX_PROFILE_NAME);
    }
    parent->children.Add(node);
}

NxuColladaStream& NxuColladaStream::storeBool(NX_BOOL b, const char* name)
{
    addNode(name, b ? "true" : "false");
    return *this;
}

NxuColladaStream& NxuColladaStream::storeByte(NxU8 b, const char* name)
{
    char tempBuffer[1028];
    sprintf(tempBuffer, "%d", b);
    addNode(name, tempBuffer);
    return *this;
}

NxuColladaStream& NxuColladaStream::storeWord(NxU16 w, const char* name)
{
    char tempBuffer[1028];
    sprintf(tempBuffer, "%d", w);
    addNode(name, tempBuffer);
    return *this;
}

NxuColladaStream& NxuColladaStream::storeDword(NxU32 d, const char* name)
{
    char tempBuffer[1028];
    sprintf(tempBuffer, "%d", d);
    addNode(name, tempBuffer);
    return *this;
}

NxuColladaStream& NxuColladaStream::storeFloat(NxF32 f, const char* name)
{
    char tempBuffer[1028];
    sprintf(tempBuffer, "%f", f);
    addNode(name, tempBuffer);
    return *this;
}

NxuColladaStream& NxuColladaStream::storeDouble(NxF64 f, const char* name)
{
    char tempBuffer[1028];
    sprintf(tempBuffer, "%f", f);
    addNode(name, tempBuffer);
    return *this;
}

NxuColladaStream& NxuColladaStream::storeByteBuffer(const void* buffer, NxU32 size, const char* name)
{
    char* tempBuffer = new char[size * 4 + 1];
    char* current = tempBuffer;
    for (NxU32 i=0;i<size;++i)
    {
        current += sprintf(current, "%d ", *(((NxU8*)buffer) + i));
    }
    addNode(name, tempBuffer);
    delete [] tempBuffer;
    return *this;
}

NxuColladaStream& NxuColladaStream::storeWordBuffer(const void* buffer, NxU32 size, const char* name)
{
    NxU32 count = size/sizeof(NxU16);
    NX_ASSERT((size/sizeof(NxU16))*sizeof(NxU16) == size); //Must be multiple of Word size
    char* tempBuffer = new char[size * 4 + 1];
    char* current = tempBuffer;
    for (NxU32 i=0;i<count;++i)
    {
        current += sprintf(current, "%d ", *(((NxU16*)buffer) + i));
    }
    addNode(name, tempBuffer);
    delete [] tempBuffer;
    return *this;
}

NxuColladaStream& NxuColladaStream::storeDwordBuffer(const void* buffer, NxU32 size, const char* name)
{
    NxU32 count = size/sizeof(NxU32);
    NX_ASSERT((size/sizeof(NxU32))*sizeof(NxU32) == size); //Must be multiple of Dword size
    char* tempBuffer = new char[size * 4 + 1];
    char* current = tempBuffer;
    for (NxU32 i=0;i<count;++i)
    {
        current += sprintf(current, "%d ", *(((NxU32*)buffer) + i));
    }
    addNode(name, tempBuffer);
    delete [] tempBuffer;
    return *this;
}

NxuColladaStream& NxuColladaStream::storeFloatBuffer(const void* buffer, NxU32 size, const char* name)
{
    NxU32 count = size/sizeof(NxF32);
    NX_ASSERT(count*sizeof(NxF32) == size); //Must be multiple of Float size
    char* tempBuffer = new char[size * 4 + 1];
    char* current = tempBuffer;
    for (NxU32 i=0;i<count;++i)
    {
        current += sprintf(current, "%f ", *(((NxF32*)buffer) + i));
    }
    *current = 0; //Terminate string
    addNode(name, tempBuffer);
    delete [] tempBuffer;
    return *this;
}

NxuColladaStream& NxuColladaStream::storeDoubleBuffer(const void* buffer, NxU32 size, const char* name)
{
    NxU32 count = size/sizeof(NxF64);
    NX_ASSERT((size/sizeof(NxF64))*sizeof(NxF64) == size); //Must be multiple of Float size
    char* tempBuffer = new char[size * 8 + 1];
    char* current = tempBuffer;
    for (NxU32 i=0;i<count;++i)
    {
        current += sprintf(current, "%f ", *(((NxF64*)buffer) + i));
    }
    addNode(name, tempBuffer);
    delete [] tempBuffer;
    return *this;
}

NxuColladaStream& NxuColladaStream::storeCharBuffer(const void* buffer, NxU32 size, const char* name)
{
    addNode(name, (const char*)buffer);
    return *this;
}

//Create a new subsection of the document
void NxuColladaStream::createSection(nxmlNode** index, const char* name, nxmlNode* parent)
{
    *index = new nxmlNode(name);
    if (parent)
    {
        parent->children.Add(*index);
    }
    else
    {
        nodestack.Add(*index);
    }
    current_section = *index;
    nodestack.Add(current_section);
    useTechnique = false;
}

//Create a new technique, either a vendor specific one or a COLLADA common technique
nxmlNode* NxuColladaStream::createTechnique(nxmlNode* base, const char* name)
{
    nxmlNode* technique = NULL;
    if (base)
    {
        if (!name)
        {
            technique = new nxmlNode(COLLADA_TECHNIQUE_COMMON);
            base->children.Add(technique);
        }
        else
        {
            technique = new nxmlNode(COLLADA_TECHNIQUE);
            technique->attribute(COLLADA_PROFILE) = name;
            base->children.Add(technique);
        }
    }
    return technique;
}

//Create both a PhysX and a common technique under the specified node
void NxuColladaStream::createPhysXandCommonTechniques(nxmlNode* base)
{
    createTechnique(base, PHYSX_PROFILE_NAME);
    createTechnique(base, NULL);
    useTechnique = true;
}

//Create an extra node for adding vendor specific data types
void NxuColladaStream::createExtra(nxmlNode** index, const char* name, nxmlNode* parent)
{
    nxmlNode* extra = new nxmlNode(COLLADA_EXTRA);  
    parent->children.Add(extra);
    nxmlNode* technique = createTechnique(extra, PHYSX_PROFILE_NAME);
    createSection(index, name, technique);
    useTechnique = false;
}

//Get the requested technique node under the specified node
nxmlNode* NxuColladaStream::getTechnique(nxmlNode* base_node, const char* name)
{
    nxmlNode* technique = NULL;
    if (base_node)
    {
        int count = base_node->children.count;
        for (int i=0;i<count;++i)
        {
            if (!name)
            {
                 if (base_node->children[i]->tag == COLLADA_TECHNIQUE_COMMON)
                 {
                     return base_node->children[i];
                 }
            }
            else if (base_node->children[i]->tag == COLLADA_TECHNIQUE)
            {
                if (base_node->children[i]->hasAttribute(COLLADA_PROFILE))
                {
                    if (base_node->children[i]->attribute(COLLADA_PROFILE) == name)
                    {
                        return base_node->children[i];
                    }
                }
            }
        }
    }
    return technique;
}

//Create the node hierarchy required to store data for the data type specified in the provided header
void NxuColladaStream::storeHeader(const NxuFormatHeader & header)
{   
    useTechnique = false;

	switch (header.sectionType)
    {
    case NxuTypeSDK:
        //Create an extra technique for storing parameters
        createExtra(&current_section, PHYSX_SDK, collada);
        break;
    case NxuTypeScene:
        //Create a new physics scene
        if (!library_physics_scenes)
        {
            createSection(&library_physics_scenes, COLLADA_LIBRARY_PHYSICS_SCENES, collada);
            nodestack.Pop(); //Don't track library on the stack?
        }
        createSection(&current_scene, COLLADA_PHYSICS_SCENE, library_physics_scenes);
        createPhysXandCommonTechniques(current_scene);
        current_scene->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeModel:
        //Create a new model
        if (!library_physics_models)
        {
            createSection(&library_physics_models, COLLADA_LIBRARY_PHYSICS_MODELS, collada);
            nodestack.Pop(); //Don't track library on the stack?
        }
        createSection(&current_model, COLLADA_PHYSICS_MODEL, library_physics_models);
        createExtra(&current_section, PHYSX_MODEL_DATA, current_model);
        nodestack.Pop(); //Don't track extra on the stack
        current_model->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeActor:
        createSection(&current_rigid_body, COLLADA_RIGID_BODY, current_model);
        createPhysXandCommonTechniques(current_rigid_body);
        current_rigid_body->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeJoint:
        createSection(&current_rigid_constraint, COLLADA_RIGID_CONSTRAINT, current_model);
        createPhysXandCommonTechniques(current_rigid_constraint);
        current_rigid_constraint->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeMaterial:
        //Create reference to it in the current model, because PhysX has per-scene materials
        nxmlNode* materialRef;
        createExtra(&materialRef, COLLADA_PHYSICS_MATERIAL, current_model);
        materialRef->attribute(COLLADA_URL) = header.name;

        //Create the Collada material entry
        if (!library_physics_materials)
        {
            createSection(&library_physics_materials, COLLADA_LIBRARY_PHYSICS_MATERIALS, collada);
            nodestack.Pop(); //Don't track library on the stack?
        }
        createSection(&current_physics_material, COLLADA_PHYSICS_MATERIAL, library_physics_materials);
        createPhysXandCommonTechniques(current_physics_material);
        current_physics_material->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypePairFlag:
        createExtra(&current_section, PHYSX_PAIRFLAG, current_model);
        current_section->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeShape:
        createSection(&current_shape, COLLADA_SHAPE, current_rigid_body);
        createExtra(&current_section, header.name, current_shape);
        nodestack.Pop(); //Don't track extra on the stack
        break;
    case NxuTypeTriangleMesh:
        if (!library_geometries)
        {
            createSection(&library_geometries, COLLADA_LIBRARY_GEOMETRIES, collada);
            nodestack.Pop(); //Don't track library on the stack?
        }
        //TODO: Handle getting this off the stack
        createSection(&current_geometry, COLLADA_GEOMETRY, library_geometries);
        createExtra(&current_section, PHYSX_TRIANGLE_MESH, current_geometry);
        //createSection(&current_section, COLLADA_MESH, current_geometry);
        current_section->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeConvexMesh:
        if (!library_geometries)
        {
            createSection(&library_geometries, COLLADA_LIBRARY_GEOMETRIES, collada);
            nodestack.Pop(); //Don't track library on the stack?
        }
        createSection(&current_geometry, COLLADA_GEOMETRY, library_geometries);
        createExtra(&current_section, PHYSX_CONVEX_MESH, current_geometry);
        //createSection(&current_section, COLLADA_CONVEX_MESH, current_geometry);
        current_section->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeCCDSkeleton:
        if (!library_geometries)
        {
            createSection(&library_geometries, COLLADA_LIBRARY_GEOMETRIES, collada);
            nodestack.Pop(); //Don't track library on the stack?
        }
        createExtra(&current_section, PHYSX_CCD_SKELETON, library_geometries);
        current_section->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeHeightField:
        if (!library_geometries)
        {
            createSection(&library_geometries, COLLADA_LIBRARY_GEOMETRIES, collada);
            nodestack.Pop(); //Don't track library on the stack?
        }
        createExtra(&current_section, PHYSX_HEIGHTFIELD, library_geometries);
        current_section->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeEffector:
        createExtra(&current_section, PHYSX_EFFECTOR, current_model);
        current_section->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeFluid:
        createExtra(&current_fluid, PHYSX_FLUID, current_model);
        current_fluid->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeFluidEmitter:
        createSection(&current_section, PHYSX_FLUID_EMITTER, current_fluid);
        current_fluid->attribute(COLLADA_ID) = header.name;
        break;
    case NxuTypeModelInstance:
        createSection(&current_section, COLLADA_INSTANCE_PHYSICS_MODEL, current_model);
        current_section->attribute(COLLADA_SID) = header.name;
        break;
    default:
        NX_ASSERT(0); //Unknown data type;
        current_section = 0;
    }
}

void NxuColladaStream::endSection()
{
    if (nodestack.count)
    {
        current_section = nodestack.Pop();
    }
}

NxuColladaStream& NxuColladaStream::storeVector(const NxVec3 &vec, const char* name)
{
    storeFloatBuffer(&vec.x, 3*sizeof(NxF32), name);
    return *this;
}

NxuColladaStream& NxuColladaStream::storeQuat(const NxQuat &quat, const char* name)
{
    storeFloatBuffer(&quat.x, 4*sizeof(NxF32), name);
    return *this;
}

NxuColladaStream& NxuColladaStream::storeMatrix(const NxMat34 &mat, const char* name)
{
	NxF32 m[12];
	mat.M.getRowMajor( m );
    m[9] = mat.t.x;
    m[10] = mat.t.y;
    m[11] = mat.t.z;
    storeFloatBuffer(m, 12*sizeof(NxF32), name);

    return *this;
}

NxuColladaStream& NxuColladaStream::storeParameter(const NxuParameterData& param, const char* name)
{
    char tempBuffer[1028];
    nxmlNode* params = current_section->Child(PHYSX_PARAMETERS);
    if (!params)
    {
        params = new nxmlNode(PHYSX_PARAMETERS);
        current_section->children.Add(params);
    }
    nxmlNode* p = new nxmlNode(name);
    sprintf(tempBuffer, "%d", (NxU32)param.param);
    p->attribute(PHYSX_PARAMETER_ID) = tempBuffer;
    sprintf(tempBuffer, "%f", param.value);
    p->body  = tempBuffer;
    params->children.Add(p);
    return *this;
}



//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND