#pragma once

#include <vector>
#include <cstdlib>
#include <random>
#include <set>
#include <algorithm>
#include <iostream>

class TravellingSalesman
{
public:
    TravellingSalesman(int numCities, int numCells);
    ~TravellingSalesman();

    void setShaderProgram(int program);
    
    void generatePopulation();
    void simulate(int iterations);

    void render();

private:

    int m_numCities;
    int m_numCells;

    struct Point
    {
        float x, y;
    };

    std::vector<Point> m_cities;
    std::vector<std::vector<float>> m_edges;

    struct Cell
    {
        Cell() {}
        Cell(int n) : permutation(n)
        {
            for (int i = 0; i < permutation.size(); i++)
            {
                permutation[i] = i;
            }

            std::random_shuffle(permutation.begin(), permutation.end());
        }

        int id;

        std::vector<int> permutation;
        
        Cell mutate()
        {
            Cell mutated(*this);

            int firstIdx = rand() % permutation.size();
            int secondIdx = rand() % permutation.size();

            std::swap(mutated.permutation[firstIdx], mutated.permutation[secondIdx]);

            return mutated;
        }

        bool operator==(const Cell& b) const
        {
            for (int i = 0; i < permutation.size(); i++)
            {
                if (permutation[i] != b.permutation[i]) return false;
            }

            return true;
        }

        bool needsUpdate;
        float score;
        
        float getScore(const std::vector<std::vector<float>>& edges)
        {
            if (true)
            {
                float totalScore = 0;
                for (int i = 0; i < permutation.size() - 1; i++)
                {
                    int firstCity = permutation[i];
                    int nextCity = permutation[i + 1];
                    totalScore += edges[firstCity][nextCity];
                }

                score = totalScore;
                needsUpdate = false;
            }
            
            return score;
        }

        void print()
        {
            std::cout << "ID: " << id << ": ";
            for (int i = 0; i < permutation.size(); i++)
            {
                std::cout << permutation[i] << " ";
            }

            std::cout << "-- Score: " << score << std::endl;
        }
    };

    std::vector<Cell> m_population;

    std::default_random_engine m_generator;
    std::uniform_real_distribution<float> m_distribution;

    

    float getRandomDouble()
    {
        return m_distribution(m_generator);
    }

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_program;

    GLuint m_edgeVao;
    GLuint m_edgeVbo;
    
    GLuint m_bestVao;
    GLuint m_bestVbo;

    GLuint m_colorUnif;

    int GUID = 1;

};
