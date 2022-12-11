#pragma once
#include <vector>
#include <array>
#include "rng.h"

template <typename T>
class Dataset
{
public:
	static std::vector<std::vector<T>> GenerateDeterministicSets(size_t numDatasets, size_t size)
	{
		std::vector<std::vector<T>> datasets(numDatasets);

		std::minstd_rand rng;
		for (auto& dataset : datasets)
		{
			dataset.resize(size);
			std::ranges::generate(dataset, rng);
		}

		return datasets;
	}

	static std::vector<std::vector<T>> GenerateRandomSets(size_t numDatasets, size_t size)
	{
		std::vector<std::vector<T>> datasets(numDatasets);

		for (auto& dataset : datasets)
		{
			for (size_t i = 0; i < size; i++)
			{
				dataset.push_back(RNG::Generate());
			}
		}

		return datasets;
	}
};