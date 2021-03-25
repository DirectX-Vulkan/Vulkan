#pragma once

#include "Resources.h"

class ModelImport {
public:
	ModelImport();
	~ModelImport();

	int GetFarestpoint();

	void LoadModel(string model_path, vector<Vertex>& vertices, vector<uint32_t>& indices);

private:
	int m_farestPoint = 1;

	void UpdateFarestPoint(int x, int y, int z);
	void NegativeToPositive(int* a);
};