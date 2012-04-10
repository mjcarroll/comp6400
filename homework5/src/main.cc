/***
 * Homework Assignment #5
 * Michael Carroll <carromj@auburn.edu>
 */

 /*
	Solution
	========

	NOTE: OBJ FILES MUST BE IN SAME FOLDER TO WORK.

	This implementation features an obj file loader, and a quaternion camera.
	It also uses lighting and material properties for all materials.

	Use W and S to move character forwards and backwards
	Use A and D to rotate character left and right.

	Use mouse click and drag to orbit around character, position with track even when character moves.
	Use mouse scroll up and down (or keyboard up and down) to "dolly" in on character.

	ESC exits.

*/

 /*
	Requirements
	============

	1.  The window size should be at least 800x800.

	2.  The window's title should be "COMP-5/6400 Assignment 5"

	3.    This assignment should be an extension of your assignment 4.  However, you can always improve what you have in assignment 4.

	4.    If you feel that you can create more interesting objects than what you already have, e.g., human-powered vehicles, trash cans, more students, etc., please do so.

	5.    Define normal vectors for all polygons that are used to create objects in your environment.

	6.    Add two light sources to your scene: a white light and a light of another color of your choice.  The white light should be a moving spot light mounted on the main-character student and the other light should be a positional light at a location of your choosing.

	7.    Pressing ‘L’ should cycle through 4 different lighting conditions: White spot light only (default), the other light only, both lights, and no lights (ambient light only).

	8.    Add controls to change the amount of light in the scene.  Use ‘+’ to increase and ‘–‘ to decrease. Both light sources should be controllable.

	9.    Continue the design from assignment 4 that allows users to move the viewpoint around.  The purpose is to see the lighting effect of the environment from different angles.  A simple way to achieve this is to set the view point to that of the student who has the spot light.

	10. Again, use your imagination for design.

	Description
	===========

	In this assignment, we will explore OpenGL’s lighting model.  You will find that once lighting is enabled, the 3D qualities of the scene become much more apparent.  From this point on, all assignments will expect you to use lighting.

	Useful hint:  Dividing a large polygon into multiple smaller ones will provide much better lighting effect.
*/

// Windows specific
#ifdef WIN32
# define WIN32_LEAN_AND_MEAN
# define WIN32_EXTRA_LEAN
# include <windows.h>
#endif

// OpenGL/GLUT headers
#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif

typedef struct {
	float x;
	float y;
	float z;
}
Vector;

typedef struct {
	float r;
	float g;
	float b;
}
Color;

typedef struct {
	int v1, v2, v3;
	int vn1, vn2, vn3;
	int vt1, vt2, vt3;
}
Face;

typedef struct {
	float u, v;
}
TexCoord;

typedef struct {
	float Ka[3];
	float Kd[3];
	float Ks[3];
	float Ns;               // Specular (coeff)
}
Material;

class WFObject {
	private:
		// Dynamic variables to keep our object data in
		vector<Vector> vertices;
		vector<Vector> normals;
		vector< vector<Face> > faces;       // Vector of vectors because each material has a group of verticies
		vector<TexCoord> texCoords;
		vector<Material> materials;

		vector<string> mtlFile;     // Stores the material file. This is so the file doesn't have to be re-
									// read all the time for each material

		// Index of current material being used for the verticies _et al_ succeeding the `usemtl` line
		int materialIndex;

        void parseLine(char *line){
            //char *lineType;
            // Copy type string to type char[] for strtok()
            //lineType = strtok(lineStr, " ");
            if(!strlen(line))
            {
                return;
            }
            char *lineType;
            lineType = strtok(strdup(line), " ");
            // Decide what to do
            if(!strcmp(lineType, "v"))      // Vertex
            {
                parseVertex(line);
            }
            else if(!strcmp(lineType, "vn"))    // Normal
            {
                parseNormal(line);
            }
            else if(!strcmp(lineType, "f")) // Face
            {
                parseFace(line);
            }
            else if(!strcmp(lineType, "mtllib"))        // Material library (file)
            {
                char fileName[255];
                sscanf(line, "mtllib %s", &fileName);
                if(strlen(fileName))
                {
                    loadMaterials(fileName);
                }
                else
                {
                    cout << "Parse error" << endl;
                }
            }
            else if(!strcmp(lineType, "usemtl"))        // Use a material from the materials library
            {
                string mtlName;
                materialIndex++;
                sscanf(line, "usemtl %s", mtlName.c_str());
                parseMaterial(mtlName.c_str());
                faces.push_back(vector<Face>());
            }
            return;
        }

        void parseVertex(char *line) {
            vertices.push_back(Vector());       // Add a new element to the vertices array
            sscanf(line, "v %f %f %f", &vertices.back().x, &vertices.back().y, &vertices.back().z);
            return;
        }

        // Parse a "vn" normal line of the file into the normals array
        void parseNormal(char *line) {
            normals.push_back(Vector());
            sscanf(line, "vn %f %f %f", &normals.back().x, &normals.back().y, &normals.back().z);
            return;
        }


        // Parse a "f" face line into the faces array. This gets complex due to there being different line formats.
        void parseFace(char *line) {
            //Face tmp; // Temporary storage for our face
            faces[materialIndex].push_back(Face());
            // Read face line. If texture indicies aren't present, don't read them.
            if(sscanf(line, "f %d//%d %d//%d %d//%d",
                              &faces[materialIndex].back().v1,
                              &faces[materialIndex].back().vn1,
                              &faces[materialIndex].back().v2,
                              &faces[materialIndex].back().vn2,
                              &faces[materialIndex].back().v3,
                              &faces[materialIndex].back().vn3) <= 1)
            {
                sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                             &faces[materialIndex].back().v1,
                             &faces[materialIndex].back().vt1,
                             &faces[materialIndex].back().vn1,
                             &faces[materialIndex].back().v2,
                             &faces[materialIndex].back().vt2,
                             &faces[materialIndex].back().vn2,
                             &faces[materialIndex].back().v3,
                             &faces[materialIndex].back().vt3,
                             &faces[materialIndex].back().vn3);
            }
            return;
        }

		// Open .mtl file and save into memory
        int loadMaterials(char *filename) {
            fstream file;
            file.open(filename);
            if(file.is_open())
            {
                char line[255];
                // Parse material file
                while(file.good())
                {
                    file.getline(line, 255);
                    mtlFile.push_back(line);
                }
            }
            else
            {
                cout << "Could not open mtl file '" << filename << "'\n";
                return false;
            }
            file.close();
            return true;
        }

        // Search .mtl file in memory for the material we want. Load that materials properties into the materials
        // array.
        void parseMaterial(string mtlName) {
            string materialName;
            char *line = new char[255];
            bool mtlFound = false;
            for(int i = 0; i < mtlFile.size(); i++)
            {
                strncpy(line, mtlFile[i].c_str(), 255);
                if(strlen(line))
                {
                    if(sscanf(line, "newmtl %s", materialName.c_str()))
                    {
                        if(mtlFound)
                        {
                            return;
                        }
                        if(!strcmp(mtlName.c_str(), materialName.c_str()))
                        {
                            mtlFound = true;    // Set the "found material" flag to true
                            materials.push_back(Material());
                        }
                    }
                    if(mtlFound)
                    {
                        // Parse this line into a material
                        if(line[0] == 'N' && line[1] == 's')
                        {
                            sscanf(line, "Ns %f", &materials.back().Ns);
                        }
                        else if(line[0] == 'K' && line[1] == 'a')
                        {
                            sscanf(line, "Ka %f %f %f",
                                &materials.back().Ka[0],
                                &materials.back().Ka[1],
                                &materials.back().Ka[2]);
                        }
                        else if(line[0] == 'K' && line[1] == 'd')
                        {
                            sscanf(line, "Kd %f %f %f",
                                &materials.back().Kd[0],
                                &materials.back().Kd[1],
                                &materials.back().Kd[2]);
                        }
                        else if(line[0] == 'K' && line[1] == 's')
                        {
                            sscanf(line, "Ks %f %f %f",
                                &materials.back().Ks[0],
                                &materials.back().Ks[1],
                                &materials.back().Ks[2]);
                        }
                    }
                }
                else
                {
                    return;
                }
            }

            return;
        }

	public:
        WFObject() {
            materialIndex = -1;     // Material index starts from zero
        }

        ~WFObject() {}

        int load(const char* filename) {
            fstream objFile;
            objFile.open(filename);
            if(objFile.is_open())
            {
                char line[255];
                // Parse object file line by line
                while(objFile.good())
                {
                    objFile.getline(line, 255);
                    parseLine(line);
                }
                objFile.close();
            }
            else
            {
                cout << "Could not open WFObject file '" << filename << "'\n";
                return false;
            }
            return true;
        }

        // Draw object
        void draw() {
            for(int g = 0; g < faces.size(); g++)
            {
                // Apply material for this material group
                glMaterialfv(GL_FRONT, GL_SPECULAR, materials[g].Ks);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[g].Kd);
                glMaterialfv(GL_FRONT, GL_AMBIENT, materials[g].Ka);
                glMaterialf(GL_FRONT, GL_SHININESS, materials[g].Ns);
                glBegin(GL_TRIANGLES);
                for(int f = 0; f < faces[g].size(); f++)
                {
                    glNormal3f(normals[faces[g][f].vn1 - 1].x,
                                normals[faces[g][f].vn1 - 1].y,
                                normals[faces[g][f].vn1 - 1].z);
                    glVertex3f(vertices[faces[g][f].v1 - 1].x,
                                vertices[faces[g][f].v1 - 1].y,
                                vertices[faces[g][f].v1 - 1].z);
                    glNormal3f(normals[faces[g][f].vn2 - 1].x,
                                normals[faces[g][f].vn2 - 1].y,
                                normals[faces[g][f].vn2 - 1].z);
                    glVertex3f(vertices[faces[g][f].v2 - 1].x,
                                vertices[faces[g][f].v2 - 1].y,
                                vertices[faces[g][f].v2 - 1].z);
                    glNormal3f(normals[faces[g][f].vn3 - 1].x,
                                normals[faces[g][f].vn3 - 1].y,
                                normals[faces[g][f].vn3 - 1].z);
                    glVertex3f(vertices[faces[g][f].v3 - 1].x,
                                vertices[faces[g][f].v3 - 1].y,
                                vertices[faces[g][f].v3 - 1].z);
                }
                glEnd();
            }
        }
};

class Math{
	public:
		static const float PI;
		static const float HALF_PI;
		static const float EPSILON;

		static bool closeEnough(float f1, float f2) {
			return fabs((f1-f2) / ((f2 == 0.0f) ? 1.0f :f2)) < EPSILON;
		}

		static float degreesToRadians(float degrees)
		{
			return (degrees * PI) / 180.0f;
		}

		static float radiansToDegrees(float radians)
		{
			return (radians * 180.0f) / PI;
		}
};
const float Math::PI = 3.1415926f;
const float Math::HALF_PI =  3.1415926f / 2.0f;
const float Math::EPSILON = 1e-6f;

class Vector4{
    public:
        float x,y,z,w;

        GLfloat vec[4];

        Vector4(void){
            set(0, 0, 0, 0);
        }

        Vector4(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
            set(x, y, z, w);
        }

        Vector4(const GLfloat *v) {
            set(v[0], v[1], v[2], v[3]);
        }

        Vector4(const Vector4 &v) {
            set(v.vec[0], v.vec[1], v.vec[2], v.vec[3]);
        }

        ~Vector4(){}

        void set(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
            vec[0] = x;
            vec[1] = y;
            vec[2] = z;
            vec[3] = w;
        }

};

class Vector3{
	friend Vector3 operator*(float lhs, const Vector3 &rhs){
		return Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}

	friend Vector3 operator-(const Vector3 &v){
		return Vector3(-v.x, -v.y, -v.z);
	}
	public:
		float x,y,z;

		Vector3(){}
		Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
		~Vector3(){}

		static Vector3 cross(const Vector3 &p, const Vector3 &q) {
			return Vector3((p.y * q.z) - (p.z * q.y),
						   (p.z * q.x) - (p.x * q.z),
						   (p.x * q.y) - (p.y * q.x));
		}

		static float dot(const Vector3 &p, const Vector3 &q) {
			return (p.x * q.x) + (p.y * q.y) + (p.z * q.z);
		}

		Vector3 & operator+=(const Vector3 &rhs) {
			x += rhs.x, y += rhs.y, z += rhs.z;
			return *this;
		}

		bool operator==(const Vector3 &rhs) const {
			return Math::closeEnough(x, rhs.x) && Math::closeEnough(y, rhs.y)
				&& Math::closeEnough(z, rhs.z);
		}

		bool operator!=(const Vector3 &rhs) const {
			return !(*this == rhs);
		}

		Vector3 &operator-=(const Vector3 &rhs) {
			x -= rhs.x, y -= rhs.y, z -= rhs.z;
			return *this;
		}

		Vector3 &operator*=(float scalar) {
			x *= scalar, y *= scalar, z *= scalar;
			return *this;
		}

		Vector3 &operator/=(float scalar)
		{
			x /= scalar, y /= scalar, z /= scalar;
			return *this;
		}

		Vector3 operator+(const Vector3 &rhs) const
		{
			Vector3 tmp(*this);
			tmp += rhs;
			return tmp;
		}

		Vector3 operator-(const Vector3 &rhs) const
		{
			Vector3 tmp(*this);
			tmp -= rhs;
			return tmp;
		}

		Vector3 operator*(float scalar) const
		{
			return Vector3(x * scalar, y * scalar, z * scalar);
		}

		Vector3 operator/(float scalar) const
		{
			return Vector3(x / scalar, y / scalar, z / scalar);
		}

		float magnitude() const
		{
			return sqrtf((x * x) + (y * y) + (z * z));
		}

		float magnitudeSq() const
		{
			return (x * x) + (y * y) + (z * z);
		}

		Vector3 inverse() const
		{
			return Vector3(-x, -y, -z);
		}

		void normalize()
		{
			float invMag = 1.0f / magnitude();
			x *= invMag, y *= invMag, z *= invMag;
		}

		void set(float x_, float y_, float z_)
		{
			x = x_, y = y_, z = z_;
		}
};

class Matrix4{
	friend Vector3 operator*(const Vector3 &lhs, const Matrix4 &rhs) {
		return Vector3((lhs.x * rhs.mtx[0][0]) + (lhs.y * rhs.mtx[1][0]) + (lhs.z * rhs.mtx[2][0]),
			(lhs.x * rhs.mtx[0][1]) + (lhs.y * rhs.mtx[1][1]) + (lhs.z * rhs.mtx[2][1]),
			(lhs.x * rhs.mtx[0][2]) + (lhs.y * rhs.mtx[1][2]) + (lhs.z * rhs.mtx[2][2]));
	}

	friend Matrix4 operator*(float scalar, const Matrix4 &rhs) {
		return rhs * scalar;
	}

	public:
		static const Matrix4 IDENTITY;
		Matrix4(){}
		Matrix4(float m11, float m12, float m13, float m14,
				float m21, float m22, float m23, float m24,
				float m31, float m32, float m33, float m34,
				float m41, float m42, float m43, float m44) {
			mtx[0][0] = m11, mtx[0][1] = m12, mtx[0][2] = m13, mtx[0][3] = m14;
			mtx[1][0] = m21, mtx[1][1] = m22, mtx[1][2] = m23, mtx[1][3] = m24;
			mtx[2][0] = m31, mtx[2][1] = m32, mtx[2][2] = m33, mtx[2][3] = m34;
			mtx[3][0] = m41, mtx[3][1] = m42, mtx[3][2] = m43, mtx[3][3] = m44;
		}
		~Matrix4() {}

		float *operator[](int row) {
			return mtx[row];
		}

		const float *operator[](int row) const {
			return mtx[row];
		}

		bool operator==(const Matrix4 &rhs) const {
			return Math::closeEnough(mtx[0][0], rhs.mtx[0][0])
				&& Math::closeEnough(mtx[0][1], rhs.mtx[0][1])
				&& Math::closeEnough(mtx[0][2], rhs.mtx[0][2])
				&& Math::closeEnough(mtx[0][3], rhs.mtx[0][3])
				&& Math::closeEnough(mtx[1][0], rhs.mtx[1][0])
				&& Math::closeEnough(mtx[1][1], rhs.mtx[1][1])
				&& Math::closeEnough(mtx[1][2], rhs.mtx[1][2])
				&& Math::closeEnough(mtx[1][3], rhs.mtx[1][3])
				&& Math::closeEnough(mtx[2][0], rhs.mtx[2][0])
				&& Math::closeEnough(mtx[2][1], rhs.mtx[2][1])
				&& Math::closeEnough(mtx[2][2], rhs.mtx[2][2])
				&& Math::closeEnough(mtx[2][3], rhs.mtx[2][3])
				&& Math::closeEnough(mtx[3][0], rhs.mtx[3][0])
				&& Math::closeEnough(mtx[3][1], rhs.mtx[3][1])
				&& Math::closeEnough(mtx[3][2], rhs.mtx[3][2])
				&& Math::closeEnough(mtx[3][3], rhs.mtx[3][3]);
		}

		bool operator!=(const Matrix4 &rhs) const {
			return !(*this == rhs);
		}

		Matrix4 &operator+=(const Matrix4 &rhs) {
			mtx[0][0] += rhs.mtx[0][0], mtx[0][1] += rhs.mtx[0][1], mtx[0][2] += rhs.mtx[0][2], mtx[0][3] += rhs.mtx[0][3];
			mtx[1][0] += rhs.mtx[1][0], mtx[1][1] += rhs.mtx[1][1], mtx[1][2] += rhs.mtx[1][2], mtx[1][3] += rhs.mtx[1][3];
			mtx[2][0] += rhs.mtx[2][0], mtx[2][1] += rhs.mtx[2][1], mtx[2][2] += rhs.mtx[2][2], mtx[2][3] += rhs.mtx[2][3];
			mtx[3][0] += rhs.mtx[3][0], mtx[3][1] += rhs.mtx[3][1], mtx[3][2] += rhs.mtx[3][2], mtx[3][3] += rhs.mtx[3][3];
			return *this;
		}

		Matrix4 &operator-=(const Matrix4 &rhs) {
			mtx[0][0] -= rhs.mtx[0][0], mtx[0][1] -= rhs.mtx[0][1], mtx[0][2] -= rhs.mtx[0][2], mtx[0][3] -= rhs.mtx[0][3];
			mtx[1][0] -= rhs.mtx[1][0], mtx[1][1] -= rhs.mtx[1][1], mtx[1][2] -= rhs.mtx[1][2], mtx[1][3] -= rhs.mtx[1][3];
			mtx[2][0] -= rhs.mtx[2][0], mtx[2][1] -= rhs.mtx[2][1], mtx[2][2] -= rhs.mtx[2][2], mtx[2][3] -= rhs.mtx[2][3];
			mtx[3][0] -= rhs.mtx[3][0], mtx[3][1] -= rhs.mtx[3][1], mtx[3][2] -= rhs.mtx[3][2], mtx[3][3] -= rhs.mtx[3][3];
			return *this;
		}

		Matrix4 &operator*=(const Matrix4 &rhs) {
			Matrix4 tmp;

			// Row 1.
			tmp.mtx[0][0] = (mtx[0][0] * rhs.mtx[0][0]) + (mtx[0][1] * rhs.mtx[1][0]) + (mtx[0][2] * rhs.mtx[2][0]) + (mtx[0][3] * rhs.mtx[3][0]);
			tmp.mtx[0][1] = (mtx[0][0] * rhs.mtx[0][1]) + (mtx[0][1] * rhs.mtx[1][1]) + (mtx[0][2] * rhs.mtx[2][1]) + (mtx[0][3] * rhs.mtx[3][1]);
			tmp.mtx[0][2] = (mtx[0][0] * rhs.mtx[0][2]) + (mtx[0][1] * rhs.mtx[1][2]) + (mtx[0][2] * rhs.mtx[2][2]) + (mtx[0][3] * rhs.mtx[3][2]);
			tmp.mtx[0][3] = (mtx[0][0] * rhs.mtx[0][3]) + (mtx[0][1] * rhs.mtx[1][3]) + (mtx[0][2] * rhs.mtx[2][3]) + (mtx[0][3] * rhs.mtx[3][3]);

			// Row 2.
			tmp.mtx[1][0] = (mtx[1][0] * rhs.mtx[0][0]) + (mtx[1][1] * rhs.mtx[1][0]) + (mtx[1][2] * rhs.mtx[2][0]) + (mtx[1][3] * rhs.mtx[3][0]);
			tmp.mtx[1][1] = (mtx[1][0] * rhs.mtx[0][1]) + (mtx[1][1] * rhs.mtx[1][1]) + (mtx[1][2] * rhs.mtx[2][1]) + (mtx[1][3] * rhs.mtx[3][1]);
			tmp.mtx[1][2] = (mtx[1][0] * rhs.mtx[0][2]) + (mtx[1][1] * rhs.mtx[1][2]) + (mtx[1][2] * rhs.mtx[2][2]) + (mtx[1][3] * rhs.mtx[3][2]);
			tmp.mtx[1][3] = (mtx[1][0] * rhs.mtx[0][3]) + (mtx[1][1] * rhs.mtx[1][3]) + (mtx[1][2] * rhs.mtx[2][3]) + (mtx[1][3] * rhs.mtx[3][3]);

			// Row 3.
			tmp.mtx[2][0] = (mtx[2][0] * rhs.mtx[0][0]) + (mtx[2][1] * rhs.mtx[1][0]) + (mtx[2][2] * rhs.mtx[2][0]) + (mtx[2][3] * rhs.mtx[3][0]);
			tmp.mtx[2][1] = (mtx[2][0] * rhs.mtx[0][1]) + (mtx[2][1] * rhs.mtx[1][1]) + (mtx[2][2] * rhs.mtx[2][1]) + (mtx[2][3] * rhs.mtx[3][1]);
			tmp.mtx[2][2] = (mtx[2][0] * rhs.mtx[0][2]) + (mtx[2][1] * rhs.mtx[1][2]) + (mtx[2][2] * rhs.mtx[2][2]) + (mtx[2][3] * rhs.mtx[3][2]);
			tmp.mtx[2][3] = (mtx[2][0] * rhs.mtx[0][3]) + (mtx[2][1] * rhs.mtx[1][3]) + (mtx[2][2] * rhs.mtx[2][3]) + (mtx[2][3] * rhs.mtx[3][3]);

			// Row 4.
			tmp.mtx[3][0] = (mtx[3][0] * rhs.mtx[0][0]) + (mtx[3][1] * rhs.mtx[1][0]) + (mtx[3][2] * rhs.mtx[2][0]) + (mtx[3][3] * rhs.mtx[3][0]);
			tmp.mtx[3][1] = (mtx[3][0] * rhs.mtx[0][1]) + (mtx[3][1] * rhs.mtx[1][1]) + (mtx[3][2] * rhs.mtx[2][1]) + (mtx[3][3] * rhs.mtx[3][1]);
			tmp.mtx[3][2] = (mtx[3][0] * rhs.mtx[0][2]) + (mtx[3][1] * rhs.mtx[1][2]) + (mtx[3][2] * rhs.mtx[2][2]) + (mtx[3][3] * rhs.mtx[3][2]);
			tmp.mtx[3][3] = (mtx[3][0] * rhs.mtx[0][3]) + (mtx[3][1] * rhs.mtx[1][3]) + (mtx[3][2] * rhs.mtx[2][3]) + (mtx[3][3] * rhs.mtx[3][3]);

			*this = tmp;
			return *this;
		}

		Matrix4 &operator*=(float scalar) {
			mtx[0][0] *= scalar, mtx[0][1] *= scalar, mtx[0][2] *= scalar, mtx[0][3] *= scalar;
			mtx[1][0] *= scalar, mtx[1][1] *= scalar, mtx[1][2] *= scalar, mtx[1][3] *= scalar;
			mtx[2][0] *= scalar, mtx[2][1] *= scalar, mtx[2][2] *= scalar, mtx[2][3] *= scalar;
			mtx[3][0] *= scalar, mtx[3][1] *= scalar, mtx[3][2] *= scalar, mtx[3][3] *= scalar;
			return *this;
		}

		Matrix4 &operator/=(float scalar) {
			mtx[0][0] /= scalar, mtx[0][1] /= scalar, mtx[0][2] /= scalar, mtx[0][3] /= scalar;
			mtx[1][0] /= scalar, mtx[1][1] /= scalar, mtx[1][2] /= scalar, mtx[1][3] /= scalar;
			mtx[2][0] /= scalar, mtx[2][1] /= scalar, mtx[2][2] /= scalar, mtx[2][3] /= scalar;
			mtx[3][0] /= scalar, mtx[3][1] /= scalar, mtx[3][2] /= scalar, mtx[3][3] /= scalar;
			return *this;
		}

		Matrix4 operator+(const Matrix4 &rhs) const {
			Matrix4 tmp(*this);
			tmp += rhs;
			return tmp;
		}

		Matrix4 operator-(const Matrix4 &rhs) const {
			Matrix4 tmp(*this);
			tmp -= rhs;
			return tmp;
		}

		Matrix4 operator*(const Matrix4 &rhs) const {
			Matrix4 tmp(*this);
			tmp *= rhs;
			return tmp;
		}

		Matrix4 operator*(float scalar) const {
			Matrix4 tmp(*this);
			tmp *= scalar;
			return tmp;
		}

		Matrix4 operator/(float scalar) const {
			Matrix4 tmp(*this);
			tmp /= scalar;
			return tmp;
		}

		void identity() {
			mtx[0][0] = 1.0f, mtx[0][1] = 0.0f, mtx[0][2] = 0.0f, mtx[0][3] = 0.0f;
			mtx[1][0] = 0.0f, mtx[1][1] = 1.0f, mtx[1][2] = 0.0f, mtx[1][3] = 0.0f;
			mtx[2][0] = 0.0f, mtx[2][1] = 0.0f, mtx[2][2] = 1.0f, mtx[2][3] = 0.0f;
			mtx[3][0] = 0.0f, mtx[3][1] = 0.0f, mtx[3][2] = 0.0f, mtx[3][3] = 1.0f;
		}

		void fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees) {
			headDegrees = Math::degreesToRadians(headDegrees);
			pitchDegrees = Math::degreesToRadians(pitchDegrees);
			rollDegrees = Math::degreesToRadians(rollDegrees);

			float cosH = cosf(headDegrees);
			float cosP = cosf(pitchDegrees);
			float cosR = cosf(rollDegrees);
			float sinH = sinf(headDegrees);
			float sinP = sinf(pitchDegrees);
			float sinR = sinf(rollDegrees);

			mtx[0][0] = cosR * cosH - sinR * sinP * sinH;
			mtx[0][1] = sinR * cosH + cosR * sinP * sinH;
			mtx[0][2] = -cosP * sinH;
			mtx[0][3] = 0.0f;

			mtx[1][0] = -sinR * cosP;
			mtx[1][1] = cosR * cosP;
			mtx[1][2] = sinP;
			mtx[1][3] = 0.0f;

			mtx[2][0] = cosR * sinH + sinR * sinP * cosH;
			mtx[2][1] = sinR * sinH - cosR * sinP * cosH;
			mtx[2][2] = cosP * cosH;
			mtx[2][3] = 0.0f;

			mtx[3][0] = 0.0f;
			mtx[3][1] = 0.0f;
			mtx[3][2] = 0.0f;
			mtx[3][3] = 1.0f;
		}

		void rotate(const Vector3 &axis, float degrees)
		{
			// Creates a rotation matrix about the specified axis.
			// The axis must be a unit vector. The angle must be in degrees.
			//
			// Let u = axis of rotation = (x, y, z)
			//
			//             | x^2(1 - c) + c  xy(1 - c) + zs  xz(1 - c) - ys   0 |
			// Ru(angle) = | yx(1 - c) - zs  y^2(1 - c) + c  yz(1 - c) + xs   0 |
			//             | zx(1 - c) - ys  zy(1 - c) - xs  z^2(1 - c) + c   0 |
			//             |      0              0                0           1 |
			//
			// where,
			//  c = cos(angle)
			//  s = sin(angle)

			degrees = Math::degreesToRadians(degrees);

			float x = axis.x;
			float y = axis.y;
			float z = axis.z;
			float c = cosf(degrees);
			float s = sinf(degrees);

			mtx[0][0] = (x * x) * (1.0f - c) + c;
			mtx[0][1] = (x * y) * (1.0f - c) + (z * s);
			mtx[0][2] = (x * z) * (1.0f - c) - (y * s);
			mtx[0][3] = 0.0f;

			mtx[1][0] = (y * x) * (1.0f - c) - (z * s);
			mtx[1][1] = (y * y) * (1.0f - c) + c;
			mtx[1][2] = (y * z) * (1.0f - c) + (x * s);
			mtx[1][3] = 0.0f;

			mtx[2][0] = (z * x) * (1.0f - c) + (y * s);
			mtx[2][1] = (z * y) * (1.0f - c) - (x * s);
			mtx[2][2] = (z * z) * (1.0f - c) + c;
			mtx[2][3] = 0.0f;

			mtx[3][0] = 0.0f;
			mtx[3][1] = 0.0f;
			mtx[3][2] = 0.0f;
			mtx[3][3] = 1.0f;
		}

		void toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const
		{
			// Extracts the Euler angles from a rotation matrix. The returned
			// angles are in degrees. This method might suffer from numerical
			// imprecision for ill defined rotation matrices.
			//
			// This function only works for rotation matrices constructed using
			// the popular NASA standard airplane convention of heading-pitch-roll
			// (i.e., RzRxRy).
			//
			// The algorithm used is from:
			//  David Eberly, "Euler Angle Formulas", Geometric Tools web site,
			//  http://www.geometrictools.com/Documentation/EulerAngles.pdf.

			float thetaX = asinf(mtx[1][2]);
			float thetaY = 0.0f;
			float thetaZ = 0.0f;

			if (thetaX < Math::HALF_PI)
			{
				if (thetaX > -Math::HALF_PI)
				{
					thetaZ = atan2f(-mtx[1][0], mtx[1][1]);
					thetaY = atan2f(-mtx[0][2], mtx[2][2]);
				}
				else
				{
					// Not a unique solution.
					thetaZ = -atan2f(mtx[2][0], mtx[0][0]);
					thetaY = 0.0f;
				}
			}
			else
			{
				// Not a unique solution.
				thetaZ = atan2f(mtx[2][0], mtx[0][0]);
				thetaY = 0.0f;
			}

			headDegrees = Math::radiansToDegrees(thetaY);
			pitchDegrees = Math::radiansToDegrees(thetaX);
			rollDegrees = Math::radiansToDegrees(thetaZ);
		}
	private:
		float mtx[4][4];
};
const Matrix4 Matrix4::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 1.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 1.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 1.0f);

class Quaternion{
	friend Quaternion operator*(float lhs, const Quaternion &rhs){
		return rhs * lhs;
	}

	public:
		static const Quaternion IDENTITY;
		float w, x, y, z;

		Quaternion(){}
		Quaternion(float w_, float x_, float y_, float z_) : w(w_), x(x_), y(y_), z(z_) {}
		Quaternion(float headDegrees, float pitchDegrees, float rollDegrees)
		{
			fromHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
		}

		Quaternion(const Vector3 &axis, float degrees)
		{
			fromAxisAngle(axis, degrees);
		}

		Quaternion(const Matrix4 &m)
		{
			fromMatrix(m);
		}

		bool operator==(const Quaternion &rhs) const
		{
			return Math::closeEnough(w, rhs.w) && Math::closeEnough(x, rhs.x)
				&& Math::closeEnough(y, rhs.y) && Math::closeEnough(z, rhs.z);
		}

		bool operator!=(const Quaternion &rhs) const
		{
			return !(*this == rhs);
		}

		Quaternion &operator+=(const Quaternion &rhs)
		{
			w += rhs.w, x += rhs.x, y += rhs.y, z += rhs.z;
			return *this;
		}

		Quaternion &operator-=(const Quaternion &rhs)
		{
			w -= rhs.w, x -= rhs.x, y -= rhs.y, z -= rhs.z;
			return *this;
		}

		Quaternion &operator*=(const Quaternion &rhs)
		{
			// Multiply so that rotations are applied in a left to right order.
			Quaternion tmp(
				(w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
				(w * rhs.x) + (x * rhs.w) - (y * rhs.z) + (z * rhs.y),
				(w * rhs.y) + (x * rhs.z) + (y * rhs.w) - (z * rhs.x),
				(w * rhs.z) - (x * rhs.y) + (y * rhs.x) + (z * rhs.w));

			/*
			// Multiply so that rotations are applied in a right to left order.
			Quaternion tmp(
			(w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
			(w * rhs.x) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y),
			(w * rhs.y) - (x * rhs.z) + (y * rhs.w) + (z * rhs.x),
			(w * rhs.z) + (x * rhs.y) - (y * rhs.x) + (z * rhs.w));
			*/

			*this = tmp;
			return *this;
		}

		Quaternion &operator*=(float scalar)
		{
			w *= scalar, x *= scalar, y *= scalar, z *= scalar;
			return *this;
		}

		Quaternion &operator/=(float scalar)
		{
			w /= scalar, x /= scalar, y /= scalar, z /= scalar;
			return *this;
		}

		Quaternion operator+(const Quaternion &rhs) const
		{
			Quaternion tmp(*this);
			tmp += rhs;
			return tmp;
		}

		Quaternion operator-(const Quaternion &rhs) const
		{
			Quaternion tmp(*this);
			tmp -= rhs;
			return tmp;
		}

		Quaternion operator*(const Quaternion &rhs) const
		{
			Quaternion tmp(*this);
			tmp *= rhs;
			return tmp;
		}

		Quaternion operator*(float scalar) const
		{
			Quaternion tmp(*this);
			tmp *= scalar;
			return tmp;
		}

		Quaternion operator/(float scalar) const
		{
			Quaternion tmp(*this);
			tmp /= scalar;
			return tmp;
		}

		Quaternion conjugate() const
		{
			Quaternion tmp(w, -x, -y, -z);
			return tmp;
		}

		void fromAxisAngle(const Vector3 &axis, float degrees)
		{
			float halfTheta = Math::degreesToRadians(degrees) * 0.5f;
			float s = sinf(halfTheta);
			w = cosf(halfTheta), x = axis.x * s, y = axis.y * s, z = axis.z * s;
		}

		void fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees)
		{
			Matrix4 m;
			m.fromHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
			fromMatrix(m);
		}

		void identity()
		{
			w = 1.0f, x = y = z = 0.0f;
		}

		Quaternion inverse() const
		{
			float invMag = 1.0f / magnitude();
			return conjugate() * invMag;
		}

		float magnitude() const
		{
			return sqrtf(w * w + x * x + y * y + z * z);
		}

		void normalize()
		{
			float invMag = 1.0f / magnitude();
			w *= invMag, x *= invMag, y *= invMag, z *= invMag;
		}

		void set(float w_, float x_, float y_, float z_)
		{
			w = w_, x = x_, y = y_, z = z_;
		}

		void toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const
		{
			Matrix4 m = toMatrix4();
			m.toHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
		}

		void fromMatrix(const Matrix4 &m)
		{
			// Creates a quaternion from a rotation matrix.
			// The algorithm used is from Allan and Mark Watt's "Advanced
			// Animation and Rendering Techniques" (ACM Press 1992).

			float s = 0.0f;
			float q[4] = {0.0f};
			float trace = m[0][0] + m[1][1] + m[2][2];

			if (trace > 0.0f)
			{
				s = sqrtf(trace + 1.0f);
				q[3] = s * 0.5f;
				s = 0.5f / s;
				q[0] = (m[1][2] - m[2][1]) * s;
				q[1] = (m[2][0] - m[0][2]) * s;
				q[2] = (m[0][1] - m[1][0]) * s;
			}
			else
			{
				int nxt[3] = {1, 2, 0};
				int i = 0, j = 0, k = 0;

				if (m[1][1] > m[0][0])
					i = 1;

				if (m[2][2] > m[i][i])
					i = 2;

				j = nxt[i];
				k = nxt[j];
				s = sqrtf((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);

				q[i] = s * 0.5f;
				s = 0.5f / s;
				q[3] = (m[j][k] - m[k][j]) * s;
				q[j] = (m[i][j] + m[j][i]) * s;
				q[k] = (m[i][k] + m[k][i]) * s;
			}

			x = q[0], y = q[1], z = q[2], w = q[3];
		}

		void toAxisAngle(Vector3 &axis, float &degrees) const
		{
			// Converts this quaternion to an axis and an angle.

			float sinHalfThetaSq = 1.0f - w * w;

			// Guard against numerical imprecision and identity quaternions.
			if (sinHalfThetaSq <= 0.0f)
			{
				axis.x = 1.0f, axis.y = axis.z = 0.0f;
				degrees = 0.0f;
			}
			else
			{
				float invSinHalfTheta = 1.0f / sqrtf(sinHalfThetaSq);

				axis.x = x * invSinHalfTheta;
				axis.y = y * invSinHalfTheta;
				axis.z = z * invSinHalfTheta;
				degrees = Math::radiansToDegrees(2.0f * acosf(w));
			}
		}

		Matrix4 toMatrix4() const
		{
			// Converts this quaternion to a rotation matrix.
			//
			//  | 1 - 2(y^2 + z^2)  2(xy + wz)          2(xz - wy)          0  |
			//  | 2(xy - wz)        1 - 2(x^2 + z^2)    2(yz + wx)          0  |
			//  | 2(xz + wy)        2(yz - wx)          1 - 2(x^2 + y^2)    0  |
			//  | 0                 0                   0                   1  |

			float x2 = x + x;
			float y2 = y + y;
			float z2 = z + z;
			float xx = x * x2;
			float xy = x * y2;
			float xz = x * z2;
			float yy = y * y2;
			float yz = y * z2;
			float zz = z * z2;
			float wx = w * x2;
			float wy = w * y2;
			float wz = w * z2;

			Matrix4 m;

			m[0][0] = 1.0f - (yy + zz);
			m[0][1] = xy + wz;
			m[0][2] = xz - wy;
			m[0][3] = 0.0f;

			m[1][0] = xy - wz;
			m[1][1] = 1.0f - (xx + zz);
			m[1][2] = yz + wx;
			m[1][3] = 0.0f;

			m[2][0] = xz + wy;
			m[2][1] = yz - wx;
			m[2][2] = 1.0f - (xx + yy);
			m[2][3] = 0.0f;

			m[3][0] = 0.0f;
			m[3][1] = 0.0f;
			m[3][2] = 0.0f;
			m[3][3] = 1.0f;

			return m;
		}
};
const Quaternion Quaternion::IDENTITY(1.0, 0.0, 0.0, 0.0);

class Camera{
	public:
		Camera(){
			azimuth = 0.0;
			elevation = 0.0;

			fov = DEFAULT_FOVX;
			znear = DEFAULT_ZNEAR;
			zfar = DEFAULT_ZFAR;

			minRadius = 2.5;
			maxRadius = 300;

			position.set(0.0, 0.0, 0.0);
			target.set(0.0, 0.0, 0.0);
			offset.set(0.0, 0.0, 0.0);

			xAxis.set(1.0, 0.0, 0.0);
			yAxis.set(0.0, 1.0, 0.0);
			zAxis.set(0.0, 0.0, 1.0);
			viewDir.set(0.0, 0.0, -1.0);

			viewMatrix.identity();
			projMatrix.identity();
			orientation.identity();
		}

		~Camera(){}

		void lookAt(const Vector3 &target){
			this->target = target;
		}

		void lookAt(const Vector3 &position_, const Vector3 &target_, const Vector3 &up_){
			position = position_;
			target = target_;

			offset = zAxis = position - target;
			zAxis.normalize();

			xAxis = Vector3::cross(up_, zAxis);
			xAxis.normalize();

			yAxis = Vector3::cross(zAxis, xAxis);
			yAxis.normalize();
			xAxis.normalize();

			viewMatrix[0][0] = xAxis.x;
			viewMatrix[1][0] = xAxis.y;
			viewMatrix[2][0] = xAxis.z;
			viewMatrix[3][0] = -Vector3::dot(xAxis, position);

			viewMatrix[0][1] = yAxis.x;
			viewMatrix[1][1] = yAxis.y;
			viewMatrix[2][1] = yAxis.z;
			viewMatrix[3][1] = -Vector3::dot(yAxis, position);

			viewMatrix[0][2] = zAxis.x;
			viewMatrix[1][2] = zAxis.y;
			viewMatrix[2][2] = zAxis.z;
			viewMatrix[3][2] = -Vector3::dot(zAxis, position);

			orientation.fromMatrix(viewMatrix);
		}

		const float distance( const Vector3 &p, const Vector3 &q)
		{
			return sqrtf((p.x - q.x) * (p.x - q.x) +
						 (p.y - q.y) * (p.y - q.y) +
						 (p.z - q.z) * (p.z - q.z));
		}

		void dolly(float dist) {
			Vector3 newCameraPosition = position + dist * zAxis;

			float newRadius = distance(newCameraPosition, target);
			if (newRadius < minRadius) return;
			if (newRadius > maxRadius) return;
			lookAt(newCameraPosition, target, WORLD_YAXIS);
		}

		void perspective(float fovx, float aspect, float znear_, float zfar_)
		{
			// We construct a projection matrix based on the horizontal field of view
			// 'fovx' rather than the more traditional 'fovy' used in gluPerspective().

			float e = 1.0f / tanf(Math::degreesToRadians(fovx) / 2.0f);
			float aspectInv = 1.0f / aspect;
			float fovy = 2.0f * atanf(aspectInv / e);
			float xScale = 1.0f / tanf(0.5f * fovy);
			float yScale = xScale / aspectInv;

			projMatrix[0][0] = xScale;
			projMatrix[0][1] = 0.0f;
			projMatrix[0][2] = 0.0f;
			projMatrix[0][3] = 0.0f;

			projMatrix[1][0] = 0.0f;
			projMatrix[1][1] = yScale;
			projMatrix[1][2] = 0.0f;
			projMatrix[1][3] = 0.0f;

			projMatrix[2][0] = 0.0f;
			projMatrix[2][1] = 0.0f;
			projMatrix[2][2] = (zfar_ + znear_) / (znear_ - zfar_);
			projMatrix[2][3] = -1.0f;

			projMatrix[3][0] = 0.0f;
			projMatrix[3][1] = 0.0f;
			projMatrix[3][2] = (2.0f * zfar_ * znear_) / (znear_ - zfar_);
			projMatrix[3][3] = 0.0f;

			fov = fovx;
			znear = znear_;
			zfar = zfar_;
		}

		void track(Vector3 movement, Quaternion targetOrientation)
		{
			Quaternion result = targetOrientation.conjugate() *
							Quaternion(0.0, movement.x, movement.y, movement.z) *
							targetOrientation;
			Vector3 newPosition = position + Vector3(result.x, result.y, result.z);
			Vector3 newTarget = target + Vector3(result.x, result.y, result.z);
			lookAt(newPosition, newTarget, WORLD_YAXIS);
		}

		void rotate(float delta_elevation, float delta_azimuth){
			Quaternion rotation(delta_elevation, delta_azimuth, 0.0);
			Quaternion offsetVector(0.0, offset.x, offset.y, offset.z);
			Quaternion result = rotation.conjugate() * offsetVector * rotation;

			Vector3 toffsetVector(result.x, result.y, result.z);
			Vector3 newCameraPosition = toffsetVector + target;

			lookAt(newCameraPosition, target, WORLD_YAXIS);
		}

		const Vector3 &getPosition() const {
			return position;
		}

		const Quaternion &getOrientation() const {
			return orientation;
		}

		const Matrix4 &getViewMatrix() const {
			return viewMatrix;
		}

		const Vector3 &getViewDirection() const {
			return viewDir;
		}

		const Matrix4 &getProjectionMatrix() const {
			return projMatrix;
		}

		const Vector3 getXAxis() const {
			return xAxis;
		}

		const Vector3 getYAxis() const {
			return yAxis;
		}

		const Vector3 getZAxis() const {
			return zAxis;
		}

		const float getMinRadius() const {
			return minRadius;
		}

		const float getMaxRadius() const {
			return maxRadius;
		}

		static const float DEFAULT_FOVX;
		static const float DEFAULT_ZFAR;
		static const float DEFAULT_ZNEAR;

	private:

		static const Vector3 WORLD_XAXIS;
		static const Vector3 WORLD_YAXIS;
		static const Vector3 WORLD_ZAXIS;

		float elevation; // Latitude
		float azimuth; // Longitude

		float znear;
		float zfar;
		float fov;

		float minRadius;
		float maxRadius;

		Vector3 position;
		Quaternion orientation;
		Vector3 target;
		Vector3 offset;
		Vector3 xAxis;
		Vector3 yAxis;
		Vector3 zAxis;
		Vector3 viewDir;
		Matrix4 viewMatrix;
		Matrix4 projMatrix;
};
const float Camera::DEFAULT_FOVX = 80.0f;
const float Camera::DEFAULT_ZFAR = 1000.0f;
const float Camera::DEFAULT_ZNEAR = 1.0f;

const Vector3 Camera::WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const Vector3 Camera::WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const Vector3 Camera::WORLD_ZAXIS(0.0f, 0.0f, 1.0f);

class Person {
    public:
    	Person(){
    		position.set(0.0, 0.0, 0.0);
    		xAxis.set(1,0,0);
    		yAxis.set(0,1,0);
    		zAxis.set(0,0,1);
    		orientation.identity();

    		model = WFObject();

    		if(!model.load("person.obj"))
    		{
    			cout << "Could not load model" << endl;
    		}
    	}

    	~Person(){}

    	void draw(void) {
    		glPushMatrix();

    		Vector3 axis;
    		float degrees;

    		orientation.toAxisAngle(axis, degrees);
    		glTranslatef(position.x, position.y, position.z);
    		glRotatef(degrees, axis.x, axis.y, axis.z);
    		model.draw();
    		glPopMatrix();
    	}

    	void move(Vector3 dist){
    		Quaternion result = orientation.conjugate() *
    							Quaternion(0.0, dist.x, dist.y, dist.z) *
    							orientation;
    		position += Vector3(result.x, result.y, result.z);
    	}

    	void movex(float dist) {
    		move(Vector3(dist, 0, 0));
    	}

    	void movey(float dist) {
    		move(Vector3(0, dist, 0));
    	}

    	void movez(float dist) {
    		move(Vector3(0, 0, dist));
    	}

    	void rotatez(float angle) {
    		Quaternion nrot(Vector3(0.0, 1.0, 0.0), angle);
    		orientation = nrot * orientation;
    	}

    	const Vector3 &getPosition() const {
    		return position;
    	}

    	const Quaternion &getOrientation() const {
    		return orientation;
    	}

    private:
    	Vector3 position;
    	Vector3 xAxis;
    	Vector3 yAxis;
    	Vector3 zAxis;
    	Quaternion orientation;
    	WFObject model;
};

class Light {
public:
    Light(void) {
        if (nextLightNum < GL_MAX_LIGHTS)
        {
            lightNumber = GL_LIGHT0 + nextLightNum;
            nextLightNum++;
        }
        else
        {
            cerr << "Warning: maximum number of lights exceeded";
            lightNumber = GL_LIGHT0 + GL_MAX_LIGHTS - 1;
        }
        ambient.set(0,0,0,1);
        diffuse.set(1,1,1,1);
        specular.set(1,1,1,1);
        position_.set(0,0,1,0);
        attenuationConstant = 1;
        attenuationLinear = 0;
        attenuationQuadratic = 0;
        spotDirection.set(0,0,-1, 0);
        spotCutoff = 180;
        spotExponent = 0;
    }
    ~Light() {}

    void apply(void){
        glEnable(lightNumber);
        glLightfv(lightNumber, GL_POSITION, position_.vec);
        glLightfv(lightNumber, GL_AMBIENT, ambient.vec);
        glLightfv(lightNumber, GL_DIFFUSE, diffuse.vec);
        glLightfv(lightNumber, GL_SPECULAR, specular.vec);
        glLightf(lightNumber, GL_CONSTANT_ATTENUATION, attenuationConstant);
        glLightf(lightNumber, GL_LINEAR_ATTENUATION, attenuationLinear);
        glLightf(lightNumber, GL_QUADRATIC_ATTENUATION, attenuationQuadratic);
        glLightf(lightNumber, GL_SPOT_CUTOFF, spotCutoff);
        if (spotCutoff < 180) {
            glLightfv(lightNumber, GL_SPOT_DIRECTION, spotDirection.vec);
            glLightf(lightNumber, GL_SPOT_EXPONENT, spotExponent);
        }
    }

    void enable(void) {
        glEnable(lightNumber);
    }

    void disable(void) {
        glDisable(lightNumber);
    }

    void setAmbient( const Vector4& color) {
        ambient = color;
    }

    void setDiffuse( const Vector4& color) {
        diffuse = color;
    }

    void setSpecular( const Vector4& color) {
        specular = color;
    }

    void setPosition( const Vector4& position ){
        position_ = position;
    }

    void setPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
        position_.set(x,y,z,w);
    }

    void setInfinitePosition(const Vector3& position)
    {
        setPosition(position.x, position.y, position.z, 0);
    }

    void setInfinitePosition( GLfloat x, GLfloat y, GLfloat z ) {
        setPosition(x,y,z,0);
    }

    void setLocalPosition(const Vector3& position) {
        setPosition(position.x, position.y, position.z, 1);
    }

    void setLocalPosition( GLfloat x, GLfloat y, GLfloat z) {
        setPosition(x,y,z,1);
    }

    void setAttenuation(GLfloat constant, GLfloat linear, GLfloat quadratic) {
        attenuationConstant = constant;
        attenuationLinear = linear;
        attenuationQuadratic = quadratic;
    }

    void setSpotDirection(const Vector3& direction) {
        spotDirection.vec[0] = direction.x;
        spotDirection.vec[1] = direction.y;
        spotDirection.vec[2] = direction.z;
    }

    void setSpotCutoff(GLfloat cutoff) {
        spotCutoff = cutoff;
    }

    void setSpotExponent(GLfloat exponent) {
        spotExponent = exponent;
    }

private:
    GLenum lightNumber;
    Vector4 ambient, specular, diffuse;
    Vector4 position_;
    GLfloat attenuationConstant,
            attenuationLinear,
            attenuationQuadratic;
    Vector4 spotDirection;
    GLfloat spotCutoff, spotExponent;
    static int nextLightNum;
};

int Light::nextLightNum = 0;

GLint window_width = 800;
GLint window_height = 800;

Camera camera;
Person person;
Light light0, spot_light, positional_light;
WFObject ground;
WFObject shelby;

enum LightingType { SPOT, POSITIONAL, BOTH, NONE };

LightingType lt = SPOT;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&camera.getProjectionMatrix()[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&camera.getViewMatrix()[0][0]);


	glPushMatrix();
	glScalef(400, 1, 400);
	ground.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-150, 0, -100);
	shelby.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(100, 0, 100);
	shelby.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(100, 0, -100);
	shelby.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-150, 0, 100);
	shelby.draw();
	glPopMatrix();



	person.draw();
	glutSwapBuffers();
}

void setup(GLsizei w, GLsizei h) {
	glClearColor(0.0f, 0.9f, 0.9f, 0.9f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR);

	camera.perspective(Camera::DEFAULT_FOVX,
					   static_cast<float>(w) / static_cast<float>(h),
					   Camera::DEFAULT_ZNEAR, Camera::DEFAULT_ZFAR);

	camera.lookAt(Vector3(1.0, 10.0, 10.0),
				  Vector3(0.0, 0.0, 0.0),
				  Vector3(0.0, 1.0, 0.0));

	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LIGHTING);
    Vector4 global_ambient = Vector4(0.2, 0.2, 0, 1.0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient.vec);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    light0.setPosition(Vector4(10, 10, 10, 0));
    light0.setAmbient(Vector4(0,0,0,1));
    light0.setDiffuse(Vector4(1,1,1,1));
    light0.setSpecular(Vector4(1,1,1,1));
    light0.apply();

	if(!ground.load("ground.obj"))
		cout << "Could not load model ground.obj" << endl;
	if(!shelby.load("shelbyb.obj"))
		cout << "Could not load shelby.obj" << endl;
}

void resize(GLsizei w, GLsizei h) {
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	camera.perspective(Camera::DEFAULT_FOVX,
					   static_cast<float>(w) / static_cast<float>(h),
					   Camera::DEFAULT_ZNEAR, Camera::DEFAULT_ZFAR);


	glLoadMatrixf(&camera.getProjectionMatrix()[0][0]);

	glMatrixMode(GL_MODELVIEW);
}

void onKeyboardPress(unsigned char key, int x, int y) {
	Vector3 movement = Vector3(0,0,0);
	switch(key) {
		case 27:
			exit(0);
		case 97: //a
			person.rotatez(2);
			break;
		case 100: //d
			person.rotatez(-2);
			break;
		case 119: //w
			movement.set(1, 0, 0);
			break;
		case 115: //s
			movement.set(-1, 0, 0);
			break;
        case 108: //l

		default:
			printf("    Keyboard %c == %d", key, key);
			break;
	}
	person.move(movement);
	camera.track(movement, person.getOrientation());
	glutPostRedisplay();
}

void onSpecialKeyboardPress(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_UP:
			camera.dolly(1.0);
			break;
		case GLUT_KEY_DOWN:
			camera.dolly(-1.0);
			break;
		default:
			printf("    Special %d == %d", key, key);
	}
	glutPostRedisplay();
}

int old_y = 0;
int old_x = 0;
bool dragging = false;

/*! GLUT on mouse button click callback */
void onMouseButton(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
	if (state == GLUT_DOWN) {
	  dragging = true;
	  old_y = y;
	  old_x = x;
	} else {
	  dragging = false;
	  old_y = 0;
	  old_x = 0;
	}
  }

  if (button == GLUT_WHEEL_UP || button == GLUT_WHEEL_DOWN) {
	if (state == GLUT_UP) return;
	if (button == GLUT_WHEEL_UP) camera.dolly(-0.5);
	if (button == GLUT_WHEEL_DOWN) camera.dolly(0.5);
  }
  glutPostRedisplay();
}

/*! GLUT on active (button pressed) mouse motion callback */
void onActiveMouseMotion(int x, int y) {
	if (dragging) {
		int delta_y = (old_y - y)/5.0;
		int delta_x = (old_x - x)/3.0;
		camera.rotate(delta_x, delta_y);
		old_y = y;
		old_x = x;
		glutPostRedisplay();
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("COMP-5/6400 Assignment 5");

	glutDisplayFunc(display);
	glutReshapeFunc(resize);

	glutKeyboardFunc(onKeyboardPress);
	glutSpecialFunc(onSpecialKeyboardPress);

	glutMouseFunc(onMouseButton);
	glutMotionFunc(onActiveMouseMotion);

	setup(window_width, window_height);

	glutMainLoop();

	return 0;
}
