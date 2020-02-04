#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#define NUM_INDICES 4
#define NUM_ELEMENTS 2048
#define NUM_MESHES 8

#define LINE_SIZE 48
#define MAX_STRING_SIZE 16

struct Vec3i {
	unsigned short x;
	unsigned short y;
	unsigned short z;
};

struct Vec2f {
	float x;
	float y;
};

struct Vec3f {
	float x;
	float y;
	float z;
};

struct Face {
	struct Vec3i indices[NUM_INDICES];
};

struct Mesh {
	char objectName[MAX_STRING_SIZE];
	struct Vec3f vertices[NUM_ELEMENTS];
	struct Vec2f texCoords[NUM_ELEMENTS];
	struct Vec3f normals[NUM_ELEMENTS];
	unsigned short smooth;
	struct Face faces[NUM_ELEMENTS];
	
	unsigned short numVertices;
	unsigned short numTexCoords;
	unsigned short numNormals;
	unsigned short numFaces;
	unsigned short numIndices;
};

struct Model {
	struct Mesh meshes[NUM_MESHES];
	unsigned short numMeshes;
};

struct Model* loadObj(const char FILE_PATH[]) {
	// open an .OBJ file
	FILE* fp = fopen(FILE_PATH, "r");
	if (!fp){
		perror("\nUnable to open file");
		printf("%c%s%c\n", '\"', FILE_PATH, '\"');
		exit(0);
	}
	
	// line
	char line[LINE_SIZE];
	
	// instance of model
	static struct Model model;
	struct Model* pModel = &model;
	
	// object name
	char objectName[MAX_STRING_SIZE];
	
	// smooth shading
	unsigned short smooth;
	char strSmooth[4];
	
	// temporary variables
	unsigned short pos;
	unsigned short strSize;
	unsigned short indiceCheck = 0;
	
	struct Mesh* pMesh;
	
	char* slashCheck1;
	char* slashCheck2;
	char* spaceCheck;
	
	char strVertex[MAX_STRING_SIZE];
	char strTexCoord[MAX_STRING_SIZE];
	char strNormal[MAX_STRING_SIZE];
	char strIndice[MAX_STRING_SIZE];
	
	// for every line
	while (fgets(line, sizeof(line), fp)) {
		// if line is a comment or is empty: read next line
		if (line[0] == '#' || line[1] == 0) continue;
		else if (line[0] == 'o') {
			// get pointer to mesh
			pMesh = &pModel->meshes[pModel->numMeshes++];
			
			// get object name
			strcpy(pMesh->objectName, line + 2);
			
			// set indice check false
			indiceCheck = 0;
		}
		else if (line[0] == 'v' && line[1] == ' ') {
			// start at position 2
			pos = 2;
			
			// get vertex X
			strSize = strcspn(line + pos, " ");
			strncpy(strVertex, line + pos, strSize);
			// convert to float
			pMesh->vertices[pMesh->numVertices].x = atof(strVertex);
			// change position
			pos += strSize + 1;
			
			// get vertex Y
			strSize = strcspn(line + pos, " ");
			strncpy(strVertex, line + pos, strSize);
			// convert to float
			pMesh->vertices[pMesh->numVertices].y = atof(strVertex);
			// change position
			pos += strSize + 1;
			
			// get vertex Z
			strcpy(strVertex, line + pos);
			// convert to float
			pMesh->vertices[pMesh->numVertices++].z = atof(strVertex);
		}
		else if (line[0] == 'v' && line[1] == 't') {
			// start at position 3
			pos = 3;
			
			// get texture coordinate U
			strSize = strcspn(line + pos, " ");
			strncpy(strTexCoord, line + pos, strSize);
			// convert to float
			pMesh->texCoords[pMesh->numTexCoords].x = atof(strTexCoord);
			// change position
			pos += strSize + 1;
			
			// get texture coordinate V
			strcpy(strTexCoord, line + pos);
			// convert to float
			pMesh->texCoords[pMesh->numTexCoords++].y = atof(strTexCoord);
		}
		else if (line[0] == 'v' && line[1] == 'n') {
			// start at position 3
			pos = 3;
			
			// get vertex normal X
			strSize = strcspn(line + pos, " ");
			strncpy(strNormal, line + pos, strSize);
			// convert to float
			pMesh->normals[pMesh->numNormals].x = atof(strNormal);
			// change position
			pos += strSize + 1;
			
			// get vertex normal Y
			strSize = strcspn(line + pos, " ");
			strncpy(strNormal, line + pos, strSize);
			// convert to float
			pMesh->normals[pMesh->numNormals].y = atof(strNormal);
			// change position
			pos += strSize + 1;
			
			// get vertex normal Z
			strcpy(strNormal, line + pos);
			// convert to float
			pMesh->normals[pMesh->numNormals++].z = atof(strNormal);
		}
		else if (line[0] == 's') {
			// get smooth shading
			strcpy(strSmooth, line + 2);
			// convert to int
			if (strSmooth == "1") pMesh->smooth = 1;
			else if (strSmooth == "off") pMesh->smooth = 0;
		}
		else if (line[0] == 'f') {
			// start at position 2
			pos = 2;
			
			// check number of indices
			if (!indiceCheck) {
				pMesh->numIndices = 1;
				for (unsigned short i = pos; i < sizeof(line); ++i) {
					if (line[i] == ' ') ++pMesh->numIndices;
				}
				if (pMesh->numIndices > NUM_INDICES) pMesh->numIndices = NUM_INDICES;
				indiceCheck = 1;
			}
			
			for (unsigned short i = 0; i < pMesh->numIndices; ++i) {
				slashCheck1 = strstr(line + pos, "//");
				// if "//" exists in a string
				if (slashCheck1) {
					// get position of "//"
					strSize = strcspn(line + pos, "//") + 1;
					slashCheck2 = NULL;
				}
				// otherwise
				else {
					// check if "/" exists in a string
					slashCheck2 = strstr(line + pos, "/");
					// get position of "/" or " "
					if (slashCheck2) strSize = strcspn(line + pos, "/");
					else strSize = strcspn(line + pos, " ");
				}
				// get vertex indice
				strncpy(strIndice, line + pos, strSize);
				// change position
				pos += strSize + 1;
				// convert to float
				pMesh->faces[pMesh->numFaces].indices[i].x = atoi(strIndice);
				// clear the string
				memset(strIndice, 0, sizeof(strIndice));
				
				if (slashCheck2) {
					// get position of '/'
					strSize = strcspn(line + pos, "/");
					// get vertex texture coordinate indice
					strncpy(strIndice, line + pos, strSize);
					// change position
					pos += strSize + 1;
					// convert to float
					pMesh->faces[pMesh->numFaces].indices[i].y = atoi(strIndice);
					// clear the string
					memset(strIndice, 0, sizeof(strIndice));
				}
				
				if (slashCheck1 || slashCheck2) {
					// check if ' ' exists at the end of a string
					spaceCheck = strpbrk(line + pos, " ");
					if (spaceCheck) {
						// get position of ' '
						strSize = strcspn(line + pos, " ");
						// get vertex normal indice
						strncpy(strIndice, line + pos, strSize);
						// change position
						pos += strSize + 1;
					}
					else strcpy(strIndice, line + pos);	// get vertex normal indice
					// convert to float
					pMesh->faces[pMesh->numFaces].indices[i].z = atoi(strIndice);
					// clear the string
					memset(strIndice, 0, sizeof(strIndice));
				}
			}
			
			// icrease number of faces
			++pMesh->numFaces;
		}
	}
	// close the file
	fclose(fp);
	
	return pModel;
}

#endif