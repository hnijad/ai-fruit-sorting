#include <cctype>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <map>

#include <chrono>
#include <thread>



using namespace std;

const int ROW = 10;
const int COL = 3;

const int INF = 1000000000;

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

typedef vector<vector<string>> State;

void print_v(State fruits) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            cout << fruits[i][j] << " ";
        }
        cout << "\n";
    }
    cout << '\n';
}

bool is_valid_position(int x, int y) {
    return (x >= 0 && x < ROW) && (y >= 0 && y < COL);
}

set<State> generata_successors(State fruits) {
    set<State> successors;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            for (int k = 0; k < 4; k++) {
                int new_x = i + dx[k];
                int new_y = j + dy[k];
                if (!is_valid_position(new_x, new_y)) continue;
                auto temp = fruits;
                swap(temp[i][j], temp[new_x][new_y]);
                successors.insert(temp);
            }
        }
    }
    return successors;
}

bool is_sorted(const State& state) {
    for (int j = 0; j < COL; j++) {
        char fruit_type = state[0][j][0];
        int prev_number = stoi(state[0][j].substr(1));
        for (int i = 1; i < ROW; i++) {
            if (state[i][j][0] != fruit_type) {
                return false;
            }
            int curr_number = stoi(state[i][j].substr(1));
            if (curr_number < prev_number) {
                return false;
            }
            prev_number = curr_number;
        }
    }
    return true;
}

int heuristic(State state) {
    vector<pair<string, int>> vp;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            vp.push_back(make_pair(state[i][j], i));
        }
    }

    sort(vp.begin(), vp.end(), [](pair<string, int> a, pair<string, int> b) {
        if (a.first[0] == b.first[0]) {
            int number_a = stoi(a.first.substr(1));
            int number_b = stoi(b.first.substr(1));
            return number_a < number_b;
        }

        return a.first[0] < b.first[0];
    });


    vector<int> estimated_costs(6, 0);



    int estimated_cost = 0;
    for (int i = 0;i <  vp.size(); i++) {
        estimated_cost += (int) (abs((i % ROW) - vp[i].second)!=0);
    }

    return estimated_cost;
}

int heuristic2(State state) {
    vector<pair<string, pair<int,int>>> vp;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            vp.push_back(make_pair(state[i][j], make_pair(i, j)));
        }
    }

    sort(vp.begin(), vp.end(), [](const pair<string, pair<int,int>> &a, const pair<string, pair<int,int>> &b) {
        if (a.first[0] == b.first[0]) {
            int number_a = stoi(a.first.substr(1));
            int number_b = stoi(b.first.substr(1));
            return number_a < number_b;
        }

        return a.first[0] < b.first[0];
    });


    vector<int> estimated_costs(6, 0);

    for (int i = 0; i < ROW; i++) {
        //0
        estimated_costs[0] += abs(vp[i].second.first - i) + abs(vp[i].second.second - 0);
        estimated_costs[0] += abs(vp[i + ROW].second.first - i) + abs(vp[i].second.second - 1);
        estimated_costs[0] += abs(vp[i + 2 * ROW].second.first - i) + abs(vp[i].second.second - 2);

        //1
        estimated_costs[1] += abs(vp[i].second.first - i) + abs(vp[i].second.second - 0);
        estimated_costs[1] += abs(vp[i + ROW].second.first - i) + abs(vp[i].second.second - 2);
        estimated_costs[1] += abs(vp[i + 2 * ROW].second.first - i) + abs(vp[i].second.second - 1);

        //2
        estimated_costs[2] += abs(vp[i].second.first - i) + abs(vp[i].second.second - 1);
        estimated_costs[2] += abs(vp[i + ROW].second.first - i) + abs(vp[i].second.second - 0);
        estimated_costs[2] += abs(vp[i + 2 * ROW].second.first - i) + abs(vp[i].second.second - 2);

        //3
        estimated_costs[3] += abs(vp[i].second.first - i) + abs(vp[i].second.second - 1);
        estimated_costs[3] += abs(vp[i + ROW].second.first - i) + abs(vp[i].second.second - 2);
        estimated_costs[3] += abs(vp[i + 2 * ROW].second.first - i) + abs(vp[i].second.second - 0);

        //4
        estimated_costs[4] += abs(vp[i].second.first - i) + abs(vp[i].second.second - 2);
        estimated_costs[4] += abs(vp[i + ROW].second.first - i) + abs(vp[i].second.second - 0);
        estimated_costs[4] += abs(vp[i + 2 * ROW].second.first - i) + abs(vp[i].second.second - 1);

        //5
        estimated_costs[5] += abs(vp[i].second.first - i) + abs(vp[i].second.second - 2);
        estimated_costs[5] += abs(vp[i + ROW].second.first - i) + abs(vp[i].second.second - 1);
        estimated_costs[5] += abs(vp[i + 2 * ROW].second.first - i) + abs(vp[i].second.second - 0);
    }

    return *min_element(estimated_costs.begin(), estimated_costs.end());
}

int a_star(State start) {
    priority_queue<pair<int, State>, vector<pair<int, State>>, std::greater<pair<int, State>>> pq;
    map<State, int> cost;
    map<State, bool> is_viseted;
    
    cost[start] = 0;

    pq.push(make_pair(0, start));

    while (!pq.empty()) {
        auto top = pq.top();
        cout << "score = " << top.first << "\n";
        print_v(top.second);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        cout << "\n";
        pq.pop();

        if (is_sorted(top.second)) {
            cout << "Found the answer" << '\n';
            print_v(top.second);
            return cost[top.second];
        }

        is_viseted[top.second] = true;

        auto successors = generata_successors(top.second);

        for (auto successor: successors) {

            if (is_viseted[successor]) continue;
            
            int current_cost = (cost.count(successor) ? cost[successor] : INF); // check if key exists in map

            if (current_cost > cost[top.second] + 1) {
                //cout << "here" << "\n";
                cost[successor] = cost[top.second] + 1;
                pq.push(make_pair(cost[successor] + heuristic2(successor), successor));
            }
        }
        //print_pq(pq);
        //return -1;

    }

    return -1; // should not come here
}

int main() {
    State fruits(ROW, vector<string>(COL));

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            cin >> fruits[i][j];
        }
    }
    cout << heuristic2(fruits) << "\n";
    //return 0;
    int cost = a_star(fruits);
    cout << "cost = " << cost << "\n";

    // auto successors = generata_successors(fruits);

    // for (auto successor: successors) {
    //     print_v(successor);
    //     cout << "\n";
    // }

    // cout << successors.size() << "\n";

    return 0;
}