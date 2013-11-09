////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <iostream>
#include <fstream>
#include "arraylist.h"
using namespace std;


//////////////
// TYPEDEFS //
//////////////

struct VertexType {
	D3DXVECTOR3 pos;
	D3DXVECTOR2 tex;
	D3DXVECTOR3 nrm;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
void GetModelFilename(char*, char*);
bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadModel(char*);
bool LoadModelObj(char*);


//////////////////
// MAIN PROGRAM //
//////////////////
int main() {
	bool result;
	char filename[256], ext[256];
	int vertexCount, textureCount, normalCount, faceCount;
	char garbage;
	
	// Read in the name of the model file.
	GetModelFilename(filename, ext);

	// Now read the data from the file into the data structures and then output it in our model format.
	if(strcmp(ext, "obj") == 0) { 
		result = LoadModelObj(filename);
		if(!result) return -1;
	} else if(strcmp(ext, "txt") == 0) {
		result = LoadModel(filename);
		if(!result) return -1;
	} else {
		return -1;
	}

	// Notify the user the model has been converted.
	cout << "\nFile has been converted." << endl;
	cout << "\nDo you wish to exit (y/n)? ";
	cin >> garbage;

	return 0;
}


void GetModelFilename(char* filename, char* ext) {
	bool done;
	ifstream fin;
	
	// Loop until we have a file name.
	done = false;
	while(!done) {
		// Ask the user for the filename.
		cout << "Enter model filename: ";

		// Read in the filename.
		cin >> filename;

		// Ask the user for the file extension.
		cout << "Enter file extension: ";

		// Read in the file extension.
		cin >> ext;

		char objname[256];
		
		strcpy(objname, filename);
		strcat(objname, ".");
		strcat(objname, ext);

		// Attempt to open the file.
		fin.open(objname);

		if(fin.good()) {
			// If the file exists and there are no problems then exit since we have the file name.
			done = true;
		} else {
			// If the file does not exist or there was an issue opening it then notify the user and repeat the process.
			fin.clear();
			cout << endl;
			cout << "File " << filename << "." << ext << " could not be opened." << endl << endl;
		}
	}

	return;
}

bool LoadModel(char* filename) {
	ifstream fin;
	char input;
	int i;
	int m_vertexCount, m_indexCount;

	char txtname[256];

	strcpy(txtname, filename);
	strcat(txtname, ".txt");

	// Open the model file.
	fin.open(txtname);

	// If it could not open the file then exit.
	if(fin.fail()) return false;

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':') fin.get(input);

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	VertexType* m_Model = new VertexType[m_vertexCount];
	if(!m_Model) return false;

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':') fin.get(input);
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++) {
		fin >> m_Model[i].pos.x >> m_Model[i].pos.y >> m_Model[i].pos.z;
		fin >> m_Model[i].tex.x >> m_Model[i].tex.y;
		fin >> m_Model[i].nrm.x >> m_Model[i].nrm.y >> m_Model[i].nrm.z;
	}

	// Close the model file.
	fin.close();

	char binname[256];

	strcpy(binname, filename);
	strcat(binname, ".bin");

	FILE* f = fopen(binname, "wb");
	fwrite(m_Model, sizeof(VertexType) * m_vertexCount, 1, f);
	fclose(f);

	return true;
}

bool LoadModelObj(char* filename) {
	ifstream fin;
	char input, input2;
	int m_vertexCount, m_indexCount;

	char objname[256];

	strcpy(objname, filename);
	strcat(objname, ".obj");

	// Open the model file.
	fin.open(objname);

	// If it could not open the file then exit.
	if(fin.fail()) return false;

	ArrayList<D3DXVECTOR3> *vertices = new ArrayList<D3DXVECTOR3>();
	ArrayList<D3DXVECTOR2> *vertexTextures = new ArrayList<D3DXVECTOR2>();
	ArrayList<D3DXVECTOR3> *vertexNormals = new ArrayList<D3DXVECTOR3>();

	struct VertexIndices {
		int vIndex;
		int tIndex;
		int nIndex;
	};

	ArrayList<VertexIndices> *vertexTotals = new ArrayList<VertexIndices>();

	fin.get(input);
	while(!fin.eof()) {
		if(input == 'v') {
			fin.get(input);

			// Read in the vertices.
			if(input == ' ') {
				D3DXVECTOR3 *vector = new D3DXVECTOR3;
				fin >> vector->x >> vector->y >> vector->z;
				vertices->add(vector);
				// Invert the Z vertex to change to left hand system.
				vertices->get(vertices->count()-1)->z = vertices->get(vertices->count()-1)->z * -1.0f;
			}

			// Read in the texture UV coordinates.
			if(input == 't') {
				D3DXVECTOR2 *vector = new D3DXVECTOR2;
				fin >> vector->x >> vector->y;
				vertexTextures->add(vector);
				// Invert the V texture coordinates to left hand system.
				vertexTextures->get(vertexTextures->count()-1)->y = vertexTextures->get(vertexTextures->count()-1)->y * -1.0f;
			}

			// Read in the normals.
			if(input == 'n') {
				D3DXVECTOR3 *vector = new D3DXVECTOR3;
				fin >> vector->x >> vector->y >> vector->z;
				vertexNormals->add(vector);
				// Invert the Z normal to change to left hand system.
				vertexNormals->get(vertexNormals->count()-1)->z = vertexNormals->get(vertexNormals->count()-1)->z * -1.0f;
			}
		}

		// Read in the faces.
		if(input == 'f') {
			fin.get(input);
			if(input == ' ') {
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				VertexIndices *vertex1 = new VertexIndices(), *vertex2 = new VertexIndices(), *vertex3 = new VertexIndices();

				fin >> vertex3->vIndex >> input2 >> vertex3->tIndex >> input2 >> vertex3->nIndex
					>> vertex2->vIndex >> input2 >> vertex2->tIndex >> input2 >> vertex2->nIndex
					>> vertex1->vIndex >> input2 >> vertex1->tIndex >> input2 >> vertex1->nIndex;

				vertex1->vIndex--; vertex1->tIndex--; vertex1->nIndex--;
				vertex2->vIndex--; vertex2->tIndex--; vertex2->nIndex--;
				vertex3->vIndex--; vertex3->tIndex--; vertex3->nIndex--;

				vertexTotals->add(vertex1);
				vertexTotals->add(vertex2);
				vertexTotals->add(vertex3);
			}
		}

		// Read in the remainder of the line.
		while(input != '\n') fin.get(input);

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	// Now put all the stuff into m_model
	VertexType* m_Model = new VertexType[vertexTotals->count()];

	for (int i = 0; i < vertexTotals->count(); i++) {
		m_Model[i].pos = D3DXVECTOR3(
			vertices->get(vertexTotals->get(i)->vIndex)->x,
			vertices->get(vertexTotals->get(i)->vIndex)->y,
			vertices->get(vertexTotals->get(i)->vIndex)->z
		);
		m_Model[i].tex = D3DXVECTOR2(
			vertexTextures->get(vertexTotals->get(i)->tIndex)->x,
			vertexTextures->get(vertexTotals->get(i)->tIndex)->y
		);
		m_Model[i].nrm = D3DXVECTOR3(
			vertexNormals->get(vertexTotals->get(i)->nIndex)->x,
			vertexNormals->get(vertexTotals->get(i)->nIndex)->y,
			vertexNormals->get(vertexTotals->get(i)->nIndex)->z
		);
	}

	m_vertexCount = vertexTotals->count();
	m_indexCount = m_vertexCount;

	char binname[256];

	strcpy(binname, filename);
	strcat(binname, ".bin");

	FILE* f = fopen(binname, "wb");
	fwrite(m_Model, sizeof(VertexType) * m_vertexCount, 1, f);
	fclose(f);

	return true;
}