#include "Object.h"



Object::Object():isMaterialsLoaded(false)
{
}

Object::Object(int isMaterialTextureLoaded):isMaterialsLoaded(isMaterialTextureLoaded)
{
}



Object::~Object()
{
}

void Object::draw(std::vector<material> & mainMaterial, std::vector<materialVertex> &mainMaterialsVertex)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);

//	glVertexPointer(3, GL_FLOAT, 0, vetrexes.data());
//	glNormalPointer(GL_FLOAT, 0, normals.data());
//	glTexCoordPointer(2, GL_FLOAT, 0, texturecoordinate.data());


	glVertexPointer(3, GL_FLOAT, 0, 0);
	glNormalPointer(GL_FLOAT, 0, (void*)(vetrexes.size()*sizeof(float)));
	glTexCoordPointer(2, GL_FLOAT, 0, (void*)(vetrexes.size()*sizeof(float)+normals.size()*sizeof(float)));
//
	for (int i = 0; i < mainMaterialsVertex.size(); i++) {

		float diffuse[] = { mainMaterial[mainMaterialsVertex[i].materialIndex].dif[0],mainMaterial[mainMaterialsVertex[i].materialIndex].dif[1],mainMaterial[mainMaterialsVertex[i].materialIndex].dif[2],1.0 };
		float ambient[] = { mainMaterial[mainMaterialsVertex[i].materialIndex].amb[0],mainMaterial[mainMaterialsVertex[i].materialIndex].amb[1],mainMaterial[mainMaterialsVertex[i].materialIndex].amb[2],1.0 };
		float specular[] = { mainMaterial[mainMaterialsVertex[i].materialIndex].spec[0],mainMaterial[mainMaterialsVertex[i].materialIndex].spec[1],mainMaterial[mainMaterialsVertex[i].materialIndex].spec[2],1.0 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, mainMaterial[mainMaterialsVertex[i].materialIndex].ns);
		glColor3f(diffuse[0], diffuse[1], diffuse[2]);
	

		int tex = mainMaterial[mainMaterialsVertex[i].materialIndex].texture;
		if (tex < 0) {
			glDisable(GL_TEXTURE_2D);

			if (mainMaterialsVertex[i].faceFormat == 3) {
				glDrawArrays(GL_TRIANGLES, mainMaterialsVertex[i].startIndex, (mainMaterialsVertex[i].size));
			}
			else if (mainMaterialsVertex[i].faceFormat == 4) {
				glDrawArrays(GL_QUADS, mainMaterialsVertex[i].startIndex, (mainMaterialsVertex[i].size));
			}
			glEnable(GL_TEXTURE_2D);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, mainMaterial[mainMaterialsVertex[i].materialIndex].texture);

			if (mainMaterialsVertex[i].faceFormat == 3) {
				glDrawArrays(GL_TRIANGLES, mainMaterialsVertex[i].startIndex, (mainMaterialsVertex[i].size));
			}
			else if (mainMaterialsVertex[i].faceFormat == 4) {
				glDrawArrays(GL_QUADS, mainMaterialsVertex[i].startIndex, (mainMaterialsVertex[i].size));
			}
		}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}


