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
	unsigned char smooth;
	struct Face faces[NUM_ELEMENTS];
	
	unsigned short numVertices;
	unsigned short numTexCoords;
	unsigned short numNormals;
	unsigned short numFaces;
	unsigned char numIndices;
};

struct Model {
	struct Mesh meshes[NUM_MESHES];
	unsigned char numMeshes;
};

struct Model* loadObj(const char* FILE_PATH) {
	// open an .OBJ file
	FILE* fp = fopen(FILE_PATH, "r");
	if (!fp){
		fprintf(stderr, "\nUnable to open file \"%s\": %s\n", FILE_PATH, strerror(errno));
		exit(0);
	}
	else if (!strstr(FILE_PATH, ".obj") && !strstr(FILE_PATH, ".OBJ")) {
		unsigned char filePos = strcspn(FILE_PATH, ".");
		printf("\n%s%s", "Incorrect file format: ", FILE_PATH + filePos);
		puts("\nAn .OBJ file is required");
		exit(0);
	}
	
	// line
	char line[LINE_SIZE];
	
	// instance of model
	static struct Model model;
	struct Model* pModel = &model;
	
	// pointer to mesh
	struct Mesh* pMesh;
	
	// temporary variables
	unsigned short pos;
	unsigned short strSize;
	char str[MAX_STRING_SIZE];
	
	// indice check
	unsigned short indiceCheck;
	
	// for every line
	while (fgets(line, sizeof(line), fp)) {
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
			strncpy(str, line + pos, strSize);
			// convert to float
			pMesh->vertices[pMesh->numVertices].x = atof(str);
			// clear string
			memset(str, 0, sizeof(str));
			// change position
			pos += strSize + 1;
			
			// get vertex Y
			strSize = strcspn(line + pos, " ");
			strncpy(str, line + pos, strSize);
			// convert to float
			pMesh->vertices[pMesh->numVertices].y = atof(str);
			// clear string
			memset(str, 0, sizeof(str));
			// change position
			pos += strSize + 1;
			
			// get vertex Z
			strcpy(str, line + pos);
			// convert to float
			pMesh->vertices[pMesh->numVertices++].z = atof(str);
			// clear string
			memset(str, 0, sizeof(str));
		}
		else if (line[0] == 'v' && line[1] == 't') {
			// start at position 3
			pos = 3;
			
			// get texture coordinate U
			strSize = strcspn(line + pos, " ");
			strncpy(str, line + pos, strSize);
			// convert to float
			pMesh->texCoords[pMesh->numTexCoords].x = atof(str);
			// clear string
			memset(str, 0, sizeof(str));
			// change position
			pos += strSize + 1;
			
			// get texture coordinate V
			strcpy(str, line + pos);
			// convert to float
			pMesh->texCoords[pMesh->numTexCoords++].y = atof(str);
			// clear string
			memset(str, 0, sizeof(str));
		}
		else if (line[0] == 'v' && line[1] == 'n') {
			// start at position 3
			pos = 3;
			
			// get vertex normal X
			strSize = strcspn(line + pos, " ");
			strncpy(str, line + pos, strSize);
			// convert to float
			pMesh->normals[pMesh->numNormals].x = atof(str);
			// clear string
			memset(str, 0, sizeof(str));
			// change position
			pos += strSize + 1;
			
			// get vertex normal Y
			strSize = strcspn(line + pos, " ");
			strncpy(str, line + pos, strSize);
			// convert to float
			pMesh->normals[pMesh->numNormals].y = atof(str);
			// clear string
			memset(str, 0, sizeof(str));
			// change position
			pos += strSize + 1;
			
			// get vertex normal Z
			strcpy(str, line + pos);
			// convert to float
			pMesh->normals[pMesh->numNormals++].z = atof(str);
			// clear string
			memset(str, 0, sizeof(str));
		}
		else if (line[0] == 's') {
			// get smooth shading
			strcpy(str, line + 2);
			// convert to int
			if (str == "1") pMesh->smooth = 1;
			else if (str == "off") pMesh->smooth = 0;
			// clear string
			memset(str, 0, sizeof(str));
		}
		else if (line[0] == 'f') {
			// start at position 2
			pos = 2;
			
			// check number of indices
			if (!indiceCheck) {
				pMesh->numIndices = 1;
				for (unsigned char i = pos; i < sizeof(line); ++i) {
					if (line[i] == ' ') ++pMesh->numIndices;
				}
				if (pMesh->numIndices > NUM_INDICES) pMesh->numIndices = NUM_INDICES;
				indiceCheck = 1;
			}
			
			for (unsigned char i = 0; i < pMesh->numIndices; ++i) {
				// if "//" exists in a string: get position of "//"
				if (strstr(line, "//")) strSize = strcspn(line + pos, "//") + 1;
				// if "/" exists in a string: get position of "/"
				else if (strstr(line + pos, "/")) strSize = strcspn(line + pos, "/");
				// otherwise: get position of " "
				else strSize = strcspn(line + pos, " ");
				// get vertex indice
				strncpy(str, line + pos, strSize);
				// change position
				pos += strSize + 1;
				// convert to float
				pMesh->faces[pMesh->numFaces].indices[i].x = atoi(str);
				// clear string
				memset(str, 0, sizeof(str));
				
				// if "//" doesn't exist in a string and "/" does
				if (!strstr(line, "//") && strstr(line, "/")) {
					// get position of '/'
					strSize = strcspn(line + pos, "/");
					// get vertex texture coordinate indice
					strncpy(str, line + pos, strSize);
					// change position
					pos += strSize + 1;
					// convert to float
					pMesh->faces[pMesh->numFaces].indices[i].y = atoi(str);
					// clear string
					memset(str, 0, sizeof(str));
				}
				
				// if "//" or "/" exists in a string
				if (strstr(line, "//") || strstr(line, "/")) {
					// check if ' ' exists at the end of an indice
					if (strpbrk(line + pos, " ")) {
						// get position of ' '
						strSize = strcspn(line + pos, " ");
						// get vertex normal indice
						strncpy(str, line + pos, strSize);
						// change position
						pos += strSize + 1;
					}
					// otherwise: get vertex normal indice
					else strcpy(str, line + pos);
					// convert to float
					pMesh->faces[pMesh->numFaces].indices[i].z = atoi(str);
					// clear string
					memset(str, 0, sizeof(str));
				}
			}
			
			// increase number of faces
			++pMesh->numFaces;
		}
	}
	
	// set mesh pointer to null pointer
	pMesh = NULL;
	
	// close the file
	fclose(fp);
	
	return pModel;
}

#endif