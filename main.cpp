#include <bits/stdc++.h>
using namespace std;

struct State {
  int miss, cann, side;
  State(): miss(-1), cann(-1), side(-1) {}
  State(int miss, int cann, int side): miss(miss), cann(cann), side(side) {}

  bool operator == (const State & st) const {
    return st.miss == miss && st.cann == cann && st.side == side;
  }
};

const int CAP = 2;
const int N = 500;
const int Missionaries = 3;
const int Cannibals = 3;
const State startState = State(Missionaries, Cannibals, 0);
const State endState = State(Missionaries, Cannibals, 1);

map <int, bool> seen_state;
vector <int> graph[N];
int cost[N];
int path[N];

bool valid(State st) {
  return (st.miss == 0 || st.miss >= st.cann);
}

int getHashFromState(State curState) {
  return (curState.miss * 100 + curState.cann * 10 + curState.side);
}

State getStateFromHash(int hash_value) {
  State st = State();
  st.side = hash_value % 10;
  hash_value /= 10;
  st.cann = hash_value % 10;
  hash_value /= 10;
  st.miss = hash_value;
  return st;
}

void buildGraph(State curState) {
  if (seen_state.count(getHashFromState(curState))) {
    return;
  }

  seen_state[getHashFromState(curState)] = true;

  for (int m = 0; m <= curState.miss; m++) {
    for (int c = 0; c <= curState.cann; c++) {
      if (c + m > 0 && (m + c <= CAP && (m == 0 || m >= c)) && valid(State(curState.miss - m, curState.cann - c, curState.side ^ 1))) {
        State nextState = State(3 - curState.miss + m, 3 - curState.cann + c, curState.side ^ 1);
        if (valid(nextState)) {
          graph[getHashFromState(curState)].push_back(getHashFromState(nextState));
          buildGraph(nextState);
        }
      }
    }
  }
}

void bfs(State curState) {
  queue <int> q;
  q.push(getHashFromState(curState));

  memset(cost, -1, sizeof(cost));
  cost[getHashFromState(curState)] = 0;

  while (!q.empty()) {
    int f = q.front();
    q.pop();
    for (auto nxt: graph[f]) {
      if (cost[nxt] == -1 || cost[nxt] > cost[f] + 1) {
        cost[nxt] = cost[f] + 1;
        path[nxt] = f;
        q.push(nxt);
      }
    }
  }

  cout << "steps: " << cost[getHashFromState(endState)] << "\n";
}

void printMoves(State curState) {
  if (path[getHashFromState(curState)] == 0) {
    if (curState.side == 0) {
      cout << curState.miss << "M " << curState.cann << "C ========== " << 3 - curState.miss << "M " << 3 - curState.cann << "C" << "\n";
    } else {
      cout << 3 - curState.miss << "M " << 3 - curState.cann << "C | " << curState.miss << "M " << curState.cann << "C" << "\n";
    }
    return;
  }
  
  printMoves(getStateFromHash(path[getHashFromState(curState)]));
  
  if (curState.side == 0) {
    cout << curState.miss << "M " << curState.cann << "C <========= " << 3 - curState.miss << "M " << 3 - curState.cann << "C" << "\n";
  } else {
    cout << 3 - curState.miss << "M " << 3 - curState.cann << "C =========> " << curState.miss << "M " << curState.cann << "C" << "\n";
  }
}

int main() {
  buildGraph(startState);
  bfs(startState);
  printMoves(endState);
  return 0;
}
