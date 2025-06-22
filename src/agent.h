#ifndef AGENT_H
#define AGENT_H

#include <bits/stdc++.h>
#include "game.h"
using namespace std;

struct Data {
    array<int, 9> h;
    int x, ccwRotateCnt;
};

int distance(const Data &a, const Data &b) {
    int res = 0;
    for (int i = 0; i < 9; i++) res += abs(a.h[i] - b.h[i]);
    return res;
}

pair<Data, int> extractData(const string &st) {
    istringstream iss(st); // h1 h2 ... h9 {테트로미노 종류} {하드드랍 열 위치} {반시계 회전횟수}
    Data data;
    int kind;
    for (int i = 0; i < 9; i++) iss >> data.h[i];
    iss >> kind >> data.x >> data.ccwRotateCnt;
    return {data, kind};
}
class Agent {
private:
    array<vector<Data>, 8> dataByKind;

public:
    Agent(const string &path) {
        ifstream fin(path);
        string line;
        while (getline(fin, line)) {
            auto [data, kind] = extractData(line);
            dataByKind[kind].push_back(data);
        }
        fin.close();
    }

    Move predict(const Game<20, 10> &game) {
        auto st = game.getCurrentState();
        auto [curData, kind] = extractData(st);
        const auto &data = dataByKind[kind];
        if (data.empty()) return Move::HARDDROP;

        int idx = 0, curDist = distance(curData, data[0]);
        for (int i = 1; i < data.size(); i++) {
            int dist = distance(curData, data[i]);
            if (curDist > dist) {
                idx = i;
                curDist = dist;
            }
        }

        const auto &target = data[idx];
        if (game.getCcwRotateCnt() < target.ccwRotateCnt) return Move::ROTATE_CCW;
        if (game.getCcwRotateCnt() > target.ccwRotateCnt) return Move::ROTATE_CW;
        if (game.getFallingBlockPosX() < target.x) return Move::RIGHT;
        if (game.getFallingBlockPosX() > target.x) return Move::LEFT;
        return Move::HARDDROP;
    }
};

#endif