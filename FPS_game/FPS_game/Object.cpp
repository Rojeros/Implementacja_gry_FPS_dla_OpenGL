#include "Object.h"



Object::Object()
{
}


Object::~Object()
{
}

void Object::draw()
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);



	glVertexPointer(3, GL_FLOAT, 0, vetrexes.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());

	glTexCoordPointer(2, GL_FLOAT, 0, texturecoordinate.data());
	GLuint * dataPointer;

	for (int i = 0; i < materialsVertex.size(); i++) {

		float diffuse[] = { materials[materialsVertex[i].materialIndex].dif[0],materials[materialsVertex[i].materialIndex].dif[1],materials[materialsVertex[i].materialIndex].dif[2],1.0 };
		float ambient[] = { materials[materialsVertex[i].materialIndex].amb[0],materials[materialsVertex[i].materialIndex].amb[1],materials[materialsVertex[i].materialIndex].amb[2],1.0 };
		float specular[] = {materials[materialsVertex[i].materialIndex].spec[0],materials[materialsVertex[i].materialIndex].spec[1],materials[materialsVertex[i].materialIndex].spec[2],1.0 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS,materials[materialsVertex[i].materialIndex].ns);
		glColor3f(diffuse[0], diffuse[1], diffuse[2]);


		dataPointer = &vertexIndices[materialsVertex[i].startIndex];
		int tex = materials[materialsVertex[i].materialIndex].texture;
		if(tex<0){
			glDisable,(GL_TEXTURE_2D);
			if (materialsVertex[i].faceFormat == 3) {
				glDrawArrays(GL_TRIANGLES, materialsVertex[i].startIndex, (materialsVertex[i].size));
			}
			else if (materialsVertex[i].faceFormat == 4) {
				glDrawArrays(GL_QUADS, materialsVertex[i].startIndex, (materialsVertex[i].size));
			}
			glEnable(GL_TEXTURE_2D);
		}else{
		glBindTexture(GL_TEXTURE_2D, materials[materialsVertex[i].materialIndex].texture);

		if (materialsVertex[i].faceFormat == 3) {
			glDrawArrays(GL_TRIANGLES,materialsVertex[i].startIndex, (materialsVertex[i].size));
		}
		else if (materialsVertex[i].faceFormat == 4) {
			glDrawArrays(GL_QUADS, materialsVertex[i].startIndex, (materialsVertex[i].size));
		}
		}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	// wy³¹czenie oœwietlenia
	glDisable(GL_COLOR_MATERIAL);
}
