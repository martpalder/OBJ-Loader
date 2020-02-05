#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj_loader.h"

void printObjectName(struct Mesh*);

void printVertices(struct Mesh*);
void printTexCoords(struct Mesh*);
void printNormals(struct Mesh*);

void printSmooth(struct Mesh*);
void printFaces(struct Mesh*);

int main(void) {
	// load an .OBJ file
	struct Model* pModel = loadObj("models/myfile.obj");
	
	// print model data
	for (unsigned char i = 0; i < pModel->numMeshes; ++i) {
		printObjectName(&pModel->meshes[i]);
		
		printVertices(&pModel->meshes[i]);
		printTexCoords(&pModel->meshes[i]);
		printNormals(&pModel->meshes[i]);
		
		printSmooth(&pModel->meshes[i]);
		printFaces(&pModel->meshes[i]);
	}
	
	return 0;
}

void printObjectName(struct Mesh* pMesh) {
	printf("\n%s%s", "Object name: ", pMesh->objectName);
}

void printVertices(struct Mesh* pMesh)
{
	puts("\nGeometric vertices: ");
	for (unsigned short i = 0; i < pMesh->numVertices; ++i) {
		printf("%f\t", pMesh->vertices[i].x);
		printf("%f\t", pMesh->vertices[i].y);
		printf("%f\n", pMesh->vertices[i].z);
	}
}

void printTexCoords(struct Mesh* pMesh)
{
	puts("\nTexture coordinates: ");
	for (unsigned short i = 0; i < pMesh->numTexCoords; ++i) {
		printf("%f\t", pMesh->texCoords[i].x);
		printf("%f\n", pMesh->texCoords[i].y);
	}
}

void printNormals(struct Mesh* pMesh)
{
	puts("\nVertex normals: ");
	for (unsigned short i = 0; i < pMesh->numNormals; ++i) {
		printf("%f\t", pMesh->normals[i].x);
		printf("%f\t", pMesh->normals[i].y);
		printf("%f\n", pMesh->normals[i].z);
	}
}

void printSmooth(struct Mesh* pMesh)
{
	printf("\n%s%d\n", "Smooth shading: ", pMesh->smooth);
}

void printFaces(struct Mesh* pMesh)
{
	puts("\nPolygonal faces: ");
	for (unsigned short i = 0; i < pMesh->numFaces; ++i) {
		for (unsigned char j = 0; j < pMesh->numIndices; ++j) {
			printf("%d", pMesh->faces[i].indices[j].x);
			
			if (pMesh->faces[i].indices[j].y || pMesh->faces[i].indices[j].z) printf("/");
			
			if (pMesh->faces[i].indices[j].y) printf("%d", pMesh->faces[i].indices[j].y);
			
			if (pMesh->faces[i].indices[j].z) printf("%c%d\t", '/', pMesh->faces[i].indices[j].z);
			else printf("\t");
		}
		printf("\n");
	}
}