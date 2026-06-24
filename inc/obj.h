

struct Vector3 {

    float x, y, z;

};



struct Vector2 {
    float u, v;

};

struct FaceVertex {
    int vIndex;
    int vtIndex;
    int vnIndex;

};


struct Face {
    std::vector<FaceVertex> vertices;
};

class ObjModel {
public:
    std::vector<Vector3> temp_vertices;
    std::vector<Vector2> temp_uvs;
    std::vector<Vector3> temp_normals;
    std::vector<Face> faces;

    bool load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            std::stringstream ss(line);
            std::string type;
            ss >> type;

            if (type == "v") {
                Vector3 v;
                ss >> v.x >> v.y >> v.z;
                temp_vertices.push_back(v);
            }
            else if (type == "vt") {
                Vector2 vt;
                ss >> vt.u >> vt.v;
                temp_uvs.push_back(vt);
            }
            else if (type == "vn") {
                Vector3 vn;
                ss >> vn.x >> vn.y >> vn.z;
                temp_normals.push_back(vn);
            }
            else if (type == "f") {
                Face face;
                std::string vertexStr;
                while (ss >> vertexStr) {
                    FaceVertex fv = {0, 0, 0};
                    std::stringstream vss(vertexStr);
                    std::string vIdx, vtIdx, vnIdx;

                    


                    std::getline(vss, vIdx, '/');
                    std::getline(vss, vtIdx, '/');
                    std::getline(vss, vnIdx, '/');

                    if (!vIdx.empty())  fv.vIndex  = std::stoi(vIdx);
                    if (!vtIdx.empty()) fv.vtIndex = std::stoi(vtIdx);
                    if (!vnIdx.empty()) fv.vnIndex = std::stoi(vnIdx);

                    face.vertices.push_back(fv);
                }
                faces.push_back(face);
            }
        }
        file.close();
        return true;
    }

    
    void render() {
        glEnable(GL_TEXTURE_2D);

        for (const auto& face : faces) {
            if (face.vertices.size() == 3) glBegin(GL_TRIANGLES);
            else if (face.vertices.size() == 4) glBegin(GL_QUADS);
            else glBegin(GL_POLYGON);

            for (const auto& fv : face.vertices) {
                if (fv.vnIndex > 0 && fv.vnIndex <= temp_normals.size()) {
                    Vector3 n = temp_normals[fv.vnIndex - 1];
                    glNormal3f(n.x, n.y, n.z);
                }

                if (fv.vtIndex > 0 && fv.vtIndex <= temp_uvs.size()) {
                    Vector2 t = temp_uvs[fv.vtIndex - 1];
                    glTexCoord2f(t.u, t.v);
                }

                if (fv.vIndex > 0 && fv.vIndex <= temp_vertices.size()) {
                    Vector3 v = temp_vertices[fv.vIndex - 1];
                    glVertex3f(v.x, v.y, v.z);
                }
            }
            glEnd();
        }
    }
};