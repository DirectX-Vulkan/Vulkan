#include "ModelImport.h"


ModelImport::ModelImport() {

}

ModelImport::~ModelImport() {

}

void ModelImport::LoadModel(string model_path, vector<Vertex>& vertices, vector<uint32_t>& indices) {
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.c_str()))
		throw runtime_error("load model error " + warn + err);

	unordered_map<Vertex, uint32_t> uniqueVertices{};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			auto test = 3 * index.vertex_index + 1;
			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };


			if (uniqueVertices.count(vertex) == 0) {
				UpdateFarestPoint(vertex.pos.x, vertex.pos.y, vertex.pos.z);

				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
void ModelImport::UpdateFarestPoint(int x, int y, int z) {
	if (x < 0) NegativeToPositive(&x);
	if (y < 0) NegativeToPositive(&y);
	if (z < 0) NegativeToPositive(&z);

	if (m_farestPoint < x) m_farestPoint = x;
	if (m_farestPoint < y) m_farestPoint = y;
	if (m_farestPoint < z) m_farestPoint = z;
}

void ModelImport::NegativeToPositive(int* a) {
	int b = *a * 2;
	*a -= b;
}

int ModelImport::GetFarestpoint()
{
	return m_farestPoint;
}
