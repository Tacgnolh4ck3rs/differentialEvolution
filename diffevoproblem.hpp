#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <numeric>

using namespace std;

void printVector(vector<double> v) {
    cout << "(";
    for (int idx = 0; idx < v.size(); idx++) {
        cout << v.at(idx);
        if (idx != v.size()-1) 
            cout << ", ";
    }
    cout << ")";
    return;
}

class DiffEvoProblem {
public:
    DiffEvoProblem(int nPop, vector<vector<double> > dimRanges) {
        m_nPop = nPop;
        m_dimRanges = dimRanges;
        cout << "You created the instance of DiffEvoProblem with these properties: " <<
             "nPop: " << m_nPop << endl <<
             "dimRanges: ";
        for (auto &curVector : m_dimRanges) {
            cout << "[";
            printVector(curVector);
            cout << "],";
        }
        cout << endl;
    }

    static double eval(vector<double> &agent) {
        double a = 20;
        double b = 0.2;
        double c = M_2_PI;
        double d = 2;
        double result;
        double sum1 = accumulate(
                agent.begin(),
                agent.end(),
                0.0,
                [](double &accum, double &x) { 
                    return accum + x*x; }
                );
        double sum2 = accumulate(
                agent.begin(),
                agent.end(),
                0.0,
                [c](double &accum, double &x) { 
                    return accum + cos(c*x); }
                );
        result = -a * \
                 exp(-b*sqrt(1.0/d * sum1)) - \
                 exp(1.0/d * sum2) + \
                 a + exp(1);
        return result;
    }

    vector<double> solve() {
        default_random_engine rdEng;
        random_device rd;
        uniform_real_distribution<double> xDist(
                m_dimRanges.at(0).at(0),
                m_dimRanges.at(0).at(1));
        uniform_real_distribution<double> yDist(
                m_dimRanges.at(1).at(0),
                m_dimRanges.at(1).at(1));
        uniform_real_distribution<double> zeroOne(
                0.0, 1.0);
        uniform_int_distribution<int> agentIdxDist(0, m_nPop-1);
        uniform_int_distribution<int> randomDim(0, 2-1);
        vector<vector<double> > agents(m_nPop);
        int nDim = 2;
        for (auto &agent : agents) {
            agent.push_back(xDist(rdEng));
            agent.push_back(yDist(rdEng));
        }
        cout << "Agent initialization: " << endl;
        vector<double> fitness(m_nPop);
        transform(agents.begin(), agents.end(), fitness.begin(), eval);
        for (int idx = 0; idx < agents.size(); idx++) {
            cout << "Agent " << idx << ":\t";
            printVector(agents.at(idx));
            cout << "\tFitness: " << fitness.at(idx) << endl;
        }
        // Search for best agent
        int bestIdx = min_element(fitness.begin(), fitness.end())-fitness.begin();
        double bestFitness = fitness[bestIdx];
        vector<double> bestAgent = agents.at(bestIdx);
        cout << "Best agent idx is equal to: " << bestIdx << endl;
        vector<vector<double> > proposedAgents(m_nPop, {0.0, 0.0});
        vector<double> proposedFitness(m_nPop, 0.0);
        for (int gen = 0;
                bestFitness >= epsilon && gen < maxGen;
                gen++) {
            cout << "Generation " << gen << endl;
            for (int idx = 0; idx < agents.size(); idx++) {
                int r1, r2, r3;
                r1 = agentIdxDist(rdEng);
                while ((r2 = agentIdxDist(rdEng)) == r1);
                do {
                    r3 = agentIdxDist(rdEng);
                } while (r3 == r2 || r3 == r1);
                int changeDim = randomDim(rdEng);
                for (int d = 0; d < 2; d++) {
                    if (zeroOne(rdEng) < CR || d == changeDim) {
                        proposedAgents.at(idx).at(d) = 
                            agents.at(r1).at(d) + F * \
                            (agents.at(r2).at(d) - agents.at(r3).at(d));
                    }
                    else {
                        proposedAgents.at(idx).at(d) = agents.at(idx).at(d);
                    }
                }
            }
            transform(proposedAgents.begin(), proposedAgents.end(), proposedFitness.begin(), eval);
            for (int idx = 0; idx < agents.size(); idx++) {
                if (fitness.at(idx) >= proposedFitness.at(idx)) {
                    agents.at(idx) = proposedAgents.at(idx);
                    fitness.at(idx) = proposedFitness.at(idx);
                }
            }
        }
        bestIdx = min_element(fitness.begin(), fitness.end())-fitness.begin();
        bestFitness = fitness[bestIdx];
        bestAgent = agents.at(bestIdx);
        cout << "Best agent idx is equal to: " << bestIdx << endl;
        return bestAgent;
    }

private:
    int m_nPop;
    vector<vector<double> > m_dimRanges;
    int maxGen = 60;
    double CR = 0.7;
    double F = 0.90;
    double epsilon = 1e-6;
};
