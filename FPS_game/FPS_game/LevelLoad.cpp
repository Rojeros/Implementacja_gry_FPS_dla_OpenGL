#include "LevelLoad.h"
#include "FreeImage.h"
//TODO: add errors

material::material(const char* na, float al, float n, float ni2, float* d, float* a, float* s, int i, int t)
{
	name = na;
	alpha = al;
	ni = ni2;
	ns = n;
	dif[0] = d[0];
	dif[1] = d[1];
	dif[2] = d[2];

	amb[0] = a[0];
	amb[1] = a[1];
	amb[2] = a[2];

	spec[0] = s[0];
	spec[1] = s[1];
	spec[2] = s[2];

	illum = i;
	texture = t;
}
LevelLoad::LevelLoad() :isCollisionLoad(false)
{

}
int LevelLoad::parseMaterialFile(char* line, std::string path, std::vector<std::string>* tmp)
{
	char filen[200];
	sscanf_s(line, "mtllib %s", filen, 200);
	std::string filen2 = path + filen;
	std::ifstream mtlin(filen2.c_str());

	if (!mtlin.is_open())
	{
		std::cout << "cant open mtl file\n";
		return -1;
	}

	char c[200];
	while (!mtlin.eof())
	{
		mtlin.getline(c, 200);
		tmp->push_back(c);
	}
	return 0;
}
int LevelLoad::parseMaterial(char* line, const std::string& fileName, Object * newObject, std::vector<material> & mainMaterial)
{

	std::vector<std::string> tmp;
	//find path to materials file
	std::string path = fileName.substr(0, ((fileName.find_last_of('/') + 1 != std::string::npos) ? (fileName.find_last_of('/') + 1) : 0));
	//get all line of file
	if (parseMaterialFile(line, path, &tmp) == 1) {
		return -1;
	}

	//line buffer
	char name[200];
	char filename[200];
	//materials
	float amb[3], dif[3], spec[3], alpha, ns, ni;
	int illum;
	unsigned int texture;
	bool ismat = false;
	ismaterial = true;
	strcpy_s(filename, "\0");

	//parse file data
	for (int i = 0; i < tmp.size(); i++)
	{
		//comment
		if (tmp[i][0] == '#')
			continue;
		//new material
		if (tmp[i][0] == 'n' && tmp[i][1] == 'e' && tmp[i][2] == 'w')
		{
			//if exist
			if (ismat)
			{


				if (strcmp(filename, "\0") != 0 && strcmp(filename, "collision") != 0)
				{
					mainMaterial.push_back(material(name, alpha, ns, ni, dif, amb, spec, illum, texture));
					strcpy_s(filename, "\0");
				}
				else {
					mainMaterial.push_back(material(name, alpha, ns, ni, dif, amb, spec, illum, -1));
				}
			}

			ismat = false;
			sscanf_s(tmp[i].c_str(), "newmtl %s", name, 200);

		}
		//pars data
		else if (tmp[i][0] == 'N' && tmp[i][1] == 's')
		{
			sscanf_s(tmp[i].c_str(), "Ns %f", &ns);
			ismat = true;
		}
		else if (tmp[i][0] == 'K' && tmp[i][1] == 'a')
		{
			sscanf_s(tmp[i].c_str(), "Ka %f %f %f", &amb[0], &amb[1], &amb[2]);
			ismat = true;
		}
		else if (tmp[i][0] == 'K' && tmp[i][1] == 'd')
		{
			sscanf_s(tmp[i].c_str(), "Kd %f %f %f", &dif[0], &dif[1], &dif[2]);
			ismat = true;
		}
		else if (tmp[i][0] == 'K' && tmp[i][1] == 's')
		{
			sscanf_s(tmp[i].c_str(), "Ks %f %f %f", &spec[0], &spec[1], &spec[2]);
			ismat = true;
		}
		else if (tmp[i][0] == 'N' && tmp[i][1] == 'i')
		{
			sscanf_s(tmp[i].c_str(), "Ni %f", &ni);
			ismat = true;
		}
		else if (tmp[i][0] == 'd' && tmp[i][1] == ' ')
		{
			sscanf_s(tmp[i].c_str(), "d %f", &alpha);
			ismat = true;
		}
		else if (tmp[i][0] == 'i' && tmp[i][1] == 'l')
		{
			sscanf_s(tmp[i].c_str(), "illum %d", &illum);
			ismat = true;
		}
		else if (tmp[i][0] == 'm' && tmp[i][1] == 'a')
		{

			sscanf_s(tmp[i].c_str(), "map_Kd %s", filename, 200);
			bool l = 0;
			char c[100];
			strcpy_s(c, filename);
			std::string name = c;
			std::string filename2 = path + name;


			for (int i = 0; i < loadedTextures.size(); i++)
				if (loadedTextures[i] == name)
				{
					texture = loadedTexturesNum[i];
					l = 1;
					break;
				}
			if (l == 0) {

				GLuint texture1;
				glEnable(GL_TEXTURE_2D);
				//image format
				FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
				//pointer to the image, once loaded
				FIBITMAP *dib(0);
				//pointer to the image data
				BYTE* bits(0);
				//image width and height
				unsigned int width(0), height(0);
				//OpenGL's image ID to map to


				//check the file signature and deduce its format
				fif = FreeImage_GetFileType(filename2.c_str(), 0);
				//if still unknown, try to guess the file format from the file extension
				if (fif == FIF_UNKNOWN)
					fif = FreeImage_GetFIFFromFilename(filename2.c_str());
				//if still unkown, return failure
				if (fif == FIF_UNKNOWN) {
					std::cout << "image loading error\n";
					return false;
				}
				//std::cout << "FIF " << fif << " " << filename << std::endl;

				//check that the plugin has reading capabilities and load the file
				if (FreeImage_FIFSupportsReading(fif))
					dib = FreeImage_Load(fif, filename2.c_str());
				//if the image failed to load, return failure
				if (!dib) {
					std::cout << "image loading error\n";
					return false;
				}
				//retrieve the image data
				bits = FreeImage_GetBits(dib);
				//get the image width and height
				width = FreeImage_GetWidth(dib);
				height = FreeImage_GetHeight(dib);

				FREE_IMAGE_COLOR_TYPE color = FreeImage_GetColorType(dib);
				if (color != FIC_RGB && color != FIC_RGBALPHA)
				{
					FreeImage_Unload(dib);
					std::cout << "image loading error\n";
					return false;
				}
				//if this somehow one of these failed (they shouldn't), return failure
			//	std::cout << (bits == 0) << std::endl;
				if ((bits == 0) || (width == 0) || (height == 0)) {
					std::cout << "image loading error\n";
					return false;
				}
				//std::cout << width << " " << height << std::endl;
				//if this texture ID is in use, unload the current texture

				glGenTextures(1, &texture1);
				glBindTexture(GL_TEXTURE_2D, texture1);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, (color == FIC_RGB) ? GL_BGR : GL_BGRA, GL_UNSIGNED_BYTE, bits);
				glDisable(GL_TEXTURE_2D);

				loadedTextures.push_back(filename);
				loadedTexturesNum.push_back(texture1);
				ismat = true;
				texture = texture1;
				FreeImage_Unload(dib);
			}
		}
	}
	if (ismat)
	{
		if (strcmp(filename, "\0") != 0)
		{
			mainMaterial.push_back(material(name, alpha, ns, ni, dif, amb, spec, illum, texture));
		}
		else {
			mainMaterial.push_back(material(name, alpha, ns, ni, dif, amb, spec, illum, -1));
		}
	}

	return 1;
}
Object * LevelLoad::loadFromFile(std::string path, bool isTexturFileIsLoad, std::vector<material> & mainMaterial, std::vector<materialVertex> &mainMaterialsVertex, std::vector<CollisionPlane*> *collision)
{
	//temp array for create buffer of normal's vertex
	std::vector<vector3d > normalsTemp;
	//temp array for create buffer of texture's vertex
	std::vector<vector2D >textureTemp;
	//temp array for create buffer of object's vertex
	std::vector<vector3d > vetrexesTmp;
	std::vector<GLuint>normalIndices;
	std::vector<GLuint>	textureIndices;

	std::vector<float> vetrexes;
	std::vector<float> normals;
	std::vector<float> texturecoordinate;

	//new 3d object from file
	Object * newObject = new Object(isTexturFileIsLoad);

	//buffer for line from file
	char buf[10000];
	//last used material
	int curmat = -1;
	//collisionplane is created?
	bool coll = false;
	//locals to build buffer of textures
	int changeMaterialIndex = 0;
	int startMaterialIndex = 0;
	//last material name
	int lastMaterial = -1;
	//number of texture size
	int sizeOfMaterialIndex = 0;

	std::ifstream in(path.c_str());
	if (!in.is_open())
	{
		return 0;
	}

	//read whole wevefront file
	while (!in.eof())
	{
		in.getline(buf, 10000);
		//commnert
		if (buf[0] == '#') {

		}
		else if (buf[0] == 'v' && buf[1] == ' ')	//add to vertex buffer
		{
			float tmpx, tmpy, tmpz;
			sscanf_s(buf, "v %f %f %f", &tmpx, &tmpy, &tmpz);
			vetrexesTmp.push_back(vector3d(tmpx, tmpy, tmpz));


		}
		else if (buf[0] == 'v' && buf[1] == 'n')	//add to normals vertexes buffer
		{
			float tmpx, tmpy, tmpz;
			sscanf_s(buf, "vn %f %f %f", &tmpx, &tmpy, &tmpz);
			vector3d n(tmpx, tmpy, tmpz);
			n.normalize();
			normalsTemp.push_back(n);

		}
		else if (buf[0] == 'f')	//add face
		{

			if (coll )	//if collisionplane is exist in file, add to structure
			{
				if (path != lastPath) {
					isCollisionLoad = true;
				}
				int v[4], n[4];
				sscanf_s(buf, "f %d//%d %d//%d %d//%d %d//%d", &v[0], &n[0], &v[1], &n[1], &v[2], &n[2], &v[3], &n[3]);
				if(!isCollisionLoad){
				collision->push_back(new CollisionPlane(
						normalsTemp[n[0] - 1].getX(), normalsTemp[n[0] - 1].getY(), normalsTemp[n[0] - 1].getZ(),
						vetrexesTmp[v[0] - 1].getX(), vetrexesTmp[v[0] - 1].getY(), vetrexesTmp[v[0] - 1].getZ(),
						vetrexesTmp[v[1] - 1].getX(), vetrexesTmp[v[1] - 1].getY(), vetrexesTmp[v[1] - 1].getZ(),
						vetrexesTmp[v[2] - 1].getX(), vetrexesTmp[v[2] - 1].getY(), vetrexesTmp[v[2] - 1].getZ(),
						vetrexesTmp[v[3] - 1].getX(), vetrexesTmp[v[3] - 1].getY(), vetrexesTmp[v[3] - 1].getZ()));
				lastPath = path;
				}


			}
			else
			{

				std::string line;
				line = buf;
				//check format of face
				//4 faces format
				if (std::count(line.begin(), line.end(), ' ') == 4)
				{
					//if vertex and normal faces
					if (line.find("//") != std::string::npos)
					{
						int v[4], n[4];
						sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &v[0], &n[0], &v[1], &n[1], &v[2], &n[2], &v[3], &n[3]);
						newObject->vertexIndices.push_back(v[0] - 1);
						newObject->vertexIndices.push_back(v[1] - 1);
						newObject->vertexIndices.push_back(v[2] - 1);
						newObject->vertexIndices.push_back(v[3] - 1);

						normalIndices.push_back(n[0] - 1);
						normalIndices.push_back(n[1] - 1);
						normalIndices.push_back(n[2] - 1);
						normalIndices.push_back(n[3] - 1);


						//if texture is changed
						if (lastMaterial != curmat) {
							//add new info about texture and his place
							mainMaterialsVertex.push_back(materialVertex(changeMaterialIndex, curmat, 4));
							lastMaterial = curmat;
						}
						//increment texture place
						changeMaterialIndex += 4;

					}
					//if vertex, texture and normal faces
					else if (line.find("/") != std::string::npos)
					{
						int a, b, c, d, e, f, g;
						int t[4];
						int n[4];
						sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
							&a, &t[0], &n[0],
							&c, &t[1], &n[1],
							&d, &t[2], &n[2],
							&e, &t[3], &n[3]);

						newObject->vertexIndices.push_back(a - 1);
						newObject->vertexIndices.push_back(c - 1);
						newObject->vertexIndices.push_back(d - 1);
						newObject->vertexIndices.push_back(e - 1);

						normalIndices.push_back(n[0] - 1);
						normalIndices.push_back(n[1] - 1);
						normalIndices.push_back(n[2] - 1);
						normalIndices.push_back(n[3] - 1);

						textureIndices.push_back(t[0] - 1);
						textureIndices.push_back(t[1] - 1);
						textureIndices.push_back(t[2] - 1);
						textureIndices.push_back(t[3] - 1);


						if (lastMaterial != curmat) {
							mainMaterialsVertex.push_back(materialVertex(changeMaterialIndex, curmat, 4));
							lastMaterial = curmat;
						}
						changeMaterialIndex += 4;


					}
					// only vertex face
					else {
						int a, b, c, d, e, f, g;
						sscanf_s(line.c_str(), "f %d %d %d %d", &a, &b, &c, &d);
						newObject->vertexIndices.push_back((int)a - 1);
						newObject->vertexIndices.push_back((int)c - 1);
						newObject->vertexIndices.push_back((int)d - 1);

						if (lastMaterial != curmat) {
							mainMaterialsVertex.push_back(materialVertex(changeMaterialIndex, curmat, 3));
							lastMaterial = curmat;
						}
						changeMaterialIndex += 3;

					}
				}
				//3 format faces
				else
				{
					//vertex and normal faces
					if (line.find("//") != std::string::npos)
					{
						int a, b, c, d, e, f, g;
						sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &a, &b, &c, &e, &d, &f);
						newObject->vertexIndices.push_back((int)a - 1);
						newObject->vertexIndices.push_back((int)c - 1);
						newObject->vertexIndices.push_back((int)d - 1);

						normalIndices.push_back(b - 1);
						normalIndices.push_back(e - 1);
						normalIndices.push_back(f - 1);


						if (lastMaterial != curmat) {
							mainMaterialsVertex.push_back(materialVertex(changeMaterialIndex, curmat, 3));
							lastMaterial = curmat;
						}
						changeMaterialIndex += 3;

					}
					else if (line.find("/") != std::string::npos) //all faces attribute
					{
						int a, b, c, d, e, f, g;
						int t[3];
						sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
							&a, &t[0], &b,
							&c, &t[1], &e,
							&d, &t[2], &f);

						newObject->vertexIndices.push_back((int)a - 1);
						newObject->vertexIndices.push_back((int)c - 1);
						newObject->vertexIndices.push_back((int)d - 1);

						normalIndices.push_back(b - 1);
						normalIndices.push_back(e - 1);
						normalIndices.push_back(f - 1);

						textureIndices.push_back(t[0] - 1);
						textureIndices.push_back(t[1] - 1);
						textureIndices.push_back(t[2] - 1);


						if (lastMaterial != curmat) {
							mainMaterialsVertex.push_back(materialVertex(changeMaterialIndex, curmat, 3));
							lastMaterial = curmat;
						}
						changeMaterialIndex += 3;

					}
					else {	//only vertex face
						int a, b, c, d, e, f, g;
						sscanf_s(line.c_str(), "f %d %d %d", &a, &b, &c);
						newObject->vertexIndices.push_back((int)a - 1);
						newObject->vertexIndices.push_back((int)b - 1);
						newObject->vertexIndices.push_back((int)c - 1);


						if (lastMaterial != curmat) {
							mainMaterialsVertex.push_back(materialVertex(changeMaterialIndex, curmat, 3));
							lastMaterial = curmat;
						}
						changeMaterialIndex += 3;

					}
				}
			}
		}
		else if (buf[0] == 'u' && (buf[1] == 's' && buf[2] == 'e'))	//bind material and texture
		{
			char tmp[200];

			sscanf_s(buf, "usemtl %s", tmp, 200);
			if (strcmp(tmp, "collision") == 0 )	//hack -> add collision 
			{
				coll = true;
			}
			else {	//no, material
				coll = false;
				//find texture to bind
				for (int i = 0; i < mainMaterial.size(); i++)
				{
					if (strcmp(mainMaterial[i].name.c_str(), tmp) == 0)
					{
						curmat = i;
						break;
					}
				}
			}

		}
		else if (buf[0] == 'm' && buf[1] == 't' && buf[2] == 'l' && buf[3] == 'l')	//add new material
		{
			if (newObject->isMaterialsLoaded == false) {
				if (parseMaterial(buf, path, newObject, mainMaterial) == -1) {
					exit(5);
				}

			}
		}

		else if (buf[0] == 'v' && buf[1] == 't')	//add texture coordinate
		{
			std::string line;
			line = buf;
			float u, v;
			sscanf_s(line.c_str(), "vt %f %f", &u, &v);
			textureTemp.push_back(vector2D(u, v));
			coll = false;

		}
	}
	//build normals vertex buffer
	for (int i = 0; i < normalIndices.size(); i++) {
		normals.push_back(normalsTemp[normalIndices[i]].x);
		normals.push_back(normalsTemp[normalIndices[i]].y);
		normals.push_back(normalsTemp[normalIndices[i]].z);
	}
	//build texture vertex buffer
	for (int i = 0; i < textureIndices.size(); i++) {
		texturecoordinate.push_back(textureTemp[textureIndices[i]].x);
		texturecoordinate.push_back(textureTemp[textureIndices[i]].z);

	}
	//build vertex buffer
	for (int i = 0; i < newObject->vertexIndices.size(); i++) {
		vetrexes.push_back(vetrexesTmp[newObject->vertexIndices[i]].x);
		vetrexes.push_back(vetrexesTmp[newObject->vertexIndices[i]].y);
		vetrexes.push_back(vetrexesTmp[newObject->vertexIndices[i]].z);
	}

	//calculate count of vertexs to bind texture 
	if (mainMaterialsVertex.size() > 1) {
		mainMaterialsVertex[0].size = mainMaterialsVertex[1].startIndex;
		for (int i = 1; i < mainMaterialsVertex.size() - 1; i++)
		{
			mainMaterialsVertex[i].size = mainMaterialsVertex[i + 1].startIndex - mainMaterialsVertex[i].startIndex;
		}
		//last object
		mainMaterialsVertex[mainMaterialsVertex.size() - 1].size = newObject->vertexIndices.size() - mainMaterialsVertex[mainMaterialsVertex.size() - 1].startIndex;
	}
	else {
		if (newObject->isMaterialsLoaded == false) {
			newObject->isMaterialsLoaded = true;
			mainMaterialsVertex[0].size = newObject->vertexIndices.size() - mainMaterialsVertex[0].startIndex;
		}

	}


	

	newObject->vetrexesSize = vetrexes.size()* sizeof(float);
	newObject->normalsSize = normals.size()* sizeof(float);
	glGenBuffersARB(1, &newObject->vboId);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, newObject->vboId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (newObject->vetrexesSize + newObject->normalsSize +texturecoordinate.size()* sizeof(float)), 0, GL_STATIC_DRAW_ARB);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, newObject->vetrexesSize,vetrexes.data());                             // copy vertices starting from 0 offest
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, newObject->vetrexesSize, newObject->normalsSize, normals.data());                // copy normals after vertices
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 
		newObject->vetrexesSize + newObject->normalsSize,
		texturecoordinate.size()* sizeof(float), texturecoordinate.data());  // copy colours after normals

	loadedTextures.clear();
	loadedTexturesNum.clear();

	return newObject;
}

std::vector<Object*>* LevelLoad::animation(std::string path, std::vector<material> & mainMaterial, std::vector<materialVertex> &mainMaterialsVertex, std::vector<CollisionPlane*>* collision, int &frames)
{

	std::vector<Object*>* anim=new std::vector<Object*>();
	int counter=-1;
	struct stat buffer;
	int i = 1;
	bool stop = false;
	if (stat(path.c_str(), &buffer) == 0) {
		lastPath = path;
		anim->push_back(loadFromFile(path, false, mainMaterial, mainMaterialsVertex,collision));
		counter++;
	}
	else {

		do {
			std::string newpath = path;
			int digits = (i > 0 ? (int)log10((double)i) + 1 : 1);
			digits = 6 - digits;
			for (int j = 0; j < digits; j++)
				newpath += "0";
			newpath += std::to_string(i) + ".obj";

			if (stat(newpath.c_str(), &buffer) == 0) {
				if(i==1)
				lastPath = newpath;

				anim->push_back(loadFromFile(newpath, i != 1, mainMaterial, mainMaterialsVertex,collision));
				i++;
				counter++;
			}
			else {
				stop = true;
			}
		} while (!stop);
	}
	if (counter == -1) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",	"animation loading error.",	NULL);
		exit(3);
	}
	frames = counter;
	return anim;
}



LevelLoad::~LevelLoad()
{
	
}
