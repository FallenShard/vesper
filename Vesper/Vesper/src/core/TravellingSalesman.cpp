#include <ctime>
#include <glad/glad.h>
#include "core/TravellingSalesman.hpp"
#include <iostream>

using namespace std;

TravellingSalesman::TravellingSalesman(int numCities, int numCells)
    : m_numCities(numCities)
    , m_numCells(numCells)
    , m_generator(std::random_device{}())
    , m_distribution(-1, 1)
    , m_cities(numCities)
    , m_edges(numCities, std::vector<float>(numCities))
{
    for (int i = 0; i < m_numCities; i++)
    {
        Point city;
        city.x = getRandomDouble();
        city.y = getRandomDouble();
        m_cities[i] = city;
    }

    for (int i = 0; i < m_numCities; i++)
    {
        for (int j = 0; j < m_numCities; j++)
        {
            float distX = m_cities[i].x - m_cities[j].x;
            float distY = m_cities[i].y - m_cities[j].y;
            m_edges[i][j] = distX * distX + distY * distY;
        }
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ibo);
    
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_cities.size() * sizeof(Point), m_cities.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindVertexArray(0);

    std::vector<float> edgeData;
    for (int i = 0; i < m_numCities; i++)
    {
        for (int j = i + 1; j < m_numCities; j++)
        {
            edgeData.push_back(m_cities[i].x);
            edgeData.push_back(m_cities[i].y);
            
            edgeData.push_back(m_cities[j].x);
            edgeData.push_back(m_cities[j].y);
        }
    }

    glGenVertexArrays(1, &m_edgeVao);
    glGenBuffers(1, &m_edgeVbo);

    glBindVertexArray(m_edgeVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_edgeVbo);
    glBufferData(GL_ARRAY_BUFFER, edgeData.size() * sizeof(float), edgeData.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindVertexArray(0);

    std::vector<float> bestPathData;
    for (int i = 0; i < m_numCities - 1; i++)
    {
        bestPathData.push_back(m_cities[i].x);
        bestPathData.push_back(m_cities[i].y);

        bestPathData.push_back(m_cities[i + 1].x);
        bestPathData.push_back(m_cities[i + 1].y);
    }

    glGenVertexArrays(1, &m_bestVao);
    glGenBuffers(1, &m_bestVbo);

    glBindVertexArray(m_bestVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_bestVbo);
    glBufferData(GL_ARRAY_BUFFER, bestPathData.size() * sizeof(float), bestPathData.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindVertexArray(0);

    glPointSize(10);
    glLineWidth(3);

}

TravellingSalesman::~TravellingSalesman()
{
}

void TravellingSalesman::setShaderProgram(int program)
{
    m_program = program;
    m_colorUnif = glGetUniformLocation(m_program, "color");
    
}

void TravellingSalesman::simulate(int iterations)
{
    std::sort(m_population.begin(), m_population.end(), [](const Cell& a, const Cell& b)
    {
        return a.score < b.score;
    });

    m_population.resize(25);

    while (m_population.size() < m_numCells)
    {
        Cell cell = m_population[rand() % m_population.size()].mutate();

        bool isUnique = true;
        for (int i = 0; i < m_population.size(); i++)
        {
            if (cell == m_population[i])
            {
                isUnique = false;
            }

        }
        if (isUnique)
        {
            cell.id = GUID++;
            m_population.push_back(cell);
        }

    }

    for (int i = 0; i < m_population.size(); i++)
    {
        m_population[i].getScore(m_edges);
        //m_population[i].print();
    }

    
    std::sort(m_population.begin(), m_population.end(), [](const Cell& a, const Cell& b)
    {
        return a.score < b.score;
    });

    cout << "BEST: " << m_population[0].score << endl;


    std::vector<float> bestPathData;
    for (int i = 0; i < m_numCities - 1; i++)
    {
        bestPathData.push_back(m_cities[m_population[0].permutation[i]].x);
        bestPathData.push_back(m_cities[m_population[0].permutation[i]].y);

        bestPathData.push_back(m_cities[m_population[0].permutation[i + 1]].x);
        bestPathData.push_back(m_cities[m_population[0].permutation[i + 1]].y);
    }

    glBindVertexArray(m_bestVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_bestVbo);
    glBufferData(GL_ARRAY_BUFFER, bestPathData.size() * sizeof(float), bestPathData.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void TravellingSalesman::render()
{
    glUseProgram(m_program);
    
    glLineWidth(1);
    glUniform4f(m_colorUnif, 0.5f, 0.5f, 0.5f, 1.f);
    glBindVertexArray(m_edgeVao);
    glDrawArrays(GL_LINES, 0, 2 * m_numCities * (m_numCities - 1) / 2);
    
    glUniform4f(m_colorUnif, 1.f, 0.f, 0.f, 1.f);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, m_numCities);

    glUniform4f(m_colorUnif, 0.f, 0.5f, 1.f, 1.f);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, 1);

    glLineWidth(3);
    glUniform4f(m_colorUnif, 0.f, 1.f, 0.f, 1.f);
    glBindVertexArray(m_bestVao);
    glDrawArrays(GL_LINES, 0, 2 * (m_numCities - 1));
}

void TravellingSalesman::generatePopulation()
{
    while (m_population.size() < m_numCells)
    {
        Cell cell(m_numCities);
        bool isUnique = true;
        for (int i = 0; i < m_population.size(); i++)
        {
            if (cell == m_population[i])
            {
                isUnique = false;
            }
                
        }
        if (isUnique)
        {
            cell.id = GUID++;
            m_population.push_back(cell);
        }
           
    }

    std::cout << "Generated Population of: " << m_numCells << " cells!" << std::endl;

    for (int i = 0; i < m_numCells; i++)
    {
        m_population[i].getScore(m_edges);
        m_population[i].print();
    }
}
