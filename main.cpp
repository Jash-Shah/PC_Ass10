#include <bits/stdc++.h>

using namespace std;

class Graph
{
  int V;                         // number of vertices
  vector<pair<int, float>> *adj; // adjacency list

public:
  Graph(int V)
  {
    this->V = V;
    adj = new vector<pair<int, float>>[V];
  }

  // add an edge to the graph with a weight
  void addEdge(int u, int v, float w)
  {
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w)); // undirected graph
  }

  // get the weight of an edge between two vertices
  int getWeight(int u, int v)
  {
    for (auto it = adj[u].begin(); it != adj[u].end(); it++)
    {
      if (it->first == v)
      {
        return it->second;
      }
    }
    return -1; // edge does not exist
  }

  // get a subgraph given the nodes of the subgraph
  Graph getSubgraph(vector<int> nodes)
  {
    int n = nodes.size();
    Graph subgraph(n);

    // create a map from the original node IDs to the subgraph node IDs
    vector<int> node_map(V, -1);
    for (int i = 0; i < n; i++)
    {
      node_map[nodes[i]] = i;
    }

    // loop through all edges in the original graph and add them to the subgraph if both endpoints are in the subgraph
    for (int i = 0; i < V; i++)
    {
      if (node_map[i] != -1)
      {
        for (auto it = adj[i].begin(); it != adj[i].end(); it++)
        {
          int j = it->first;
          int w = it->second;
          if (node_map[j] != -1)
          {
            subgraph.addEdge(node_map[i], node_map[j], w);
          }
        }
      }
    }

    return subgraph;
  }

  // print the adjacency list representation of the graph
  void printGraph()
  {
    for (int i = 0; i < V; i++)
    {
      cout << "Vertex " << i << ": ";
      for (auto it = adj[i].begin(); it != adj[i].end(); it++)
      {
        cout << "(" << it->first << ", " << it->second << ") ";
      }
      cout << endl;
    }
  }

  // find the shortest cycle in a graph with the maximum weight
  int shortestCycleMaxWeight()
  {
    int shortestCycle = INT_MAX; // initialize shortest cycle length to infinity

    // loop through all vertices as potential starting points
    for (int s = 0; s < V; s++)
    {
      queue<int> q;                 // initialize queue for BFS
      vector<int> dist(V, INT_MAX); // initialize distances from starting point to all other vertices
      vector<int> parent(V, -1);    // initialize parent array for reconstructing shortest path

      // BFS traversal from starting point
      q.push(s);
      dist[s] = 0;
      while (!q.empty())
      {
        int u = q.front();
        q.pop();

        // check all adjacent vertices
        for (auto it = adj[u].begin(); it != adj[u].end(); ++it)
        {
          int v = it->first;
          int weight = it->second;

          // update distances and parent array if a shorter path is found
          if (dist[v] == INT_MAX)
          {
            dist[v] = dist[u] + weight;
            parent[v] = u;
            q.push(v);
          }
          else if (parent[u] != v && dist[v] > dist[u] + weight)
          {
            dist[v] = dist[u] + weight;
            parent[v] = u;
            q.push(v);
          }

          // check if a cycle has been formed and if it has maximum weight
          if (parent[u] != v && dist[v] + weight - dist[u] > 0)
          {
            shortestCycle = min(shortestCycle, dist[v] + weight - dist[u]);
          }
        }
      }

      cout << "Start Node : " << s << endl;
      cout << "Shortest Cycle : " << shortestCycle << endl;
      // for (int i = 0; i < V; i++)
      // {
      //   cout << "Parent of " << i << " : " << parent[i] << endl;
      // }
    }

    return shortestCycle;
  }
};

void parse_file(map<string, vector<string>> &Agents)
{
  // string str = "bala@dsri.com = agent computing, distributed data mining, intrusion detection";
  vector<string> skills;
  string agent, skill, line;
  ifstream inputFile("ACM_DataSet.txt");

  if (inputFile.is_open())
  {
    while (getline(inputFile, line))
    {
      // extract agent name
      stringstream ss(line);
      getline(ss, agent, '=');
      agent = agent.substr(0, agent.size() - 1); // remove trailing space

      // extract skills
      getline(ss, skill, ',');
      while (!skill.empty())
      {
        skills.push_back(skill.substr(1)); // remove leading space
        skill.clear();
        getline(ss, skill, ',');
      }

      // print agent and skills
      // cout << "Agent: " << agent << endl;
      // cout << "Skills: ";

      Agents.insert({agent, skills});
      // Agents[agent] = skills;
      // for (const auto &s : skills)
      // {
      //   cout << s << " | ";
      // }

      // cout << endl;
      skills.clear();
    }
    inputFile.close();
  }
  else
  {
    cout << "Unable to open file" << endl;
  }
}

void print_Agents(map<string, vector<string>> &Agents)
{
  map<string, vector<string>>::iterator it = Agents.begin();

  while (it != Agents.end())
  {
    cout << "Agent: " << it->first << endl;
    cout << "Skills: ";
    for (const auto &s : it->second)
    {
      cout << s << " |";
    }

    cout << endl;
    ++it;
  }
}

void get_agent_at(int i, map<string, vector<string>> &Agents)
{
  while (i--)
    Agents.begin()++;
}

float edge_weight(vector<string> &vec1, vector<string> &vec2)
{
  // Count no. of common skills between the two agents
  unordered_set<string> set1(vec1.begin(), vec1.end());
  // for(const auto &s: set1)
  //   cout << s << endl;
  float count = 0;
  for (const auto &elem : vec2)
  {
    if (set1.count(elem))
    {
      // cout << elem << endl;
      count++;
    }
  }

  // Weight of edge b/w A and B agent is (1 - (A ^ B / A U B))
  return (1 - count / (vec1.size() + vec2.size() - count));
}

vector<int> get_agents_with_skills(vector<string> task_skills, map<string, vector<string>> &Agents)
{
  // Agents that have atleast one of the given skills
  vector<int> skillful_agents;

  // Solving using the set cover problem
  while(!task_skills.empty())
  {
    // The agent who covers the most skills in the task_skills
    map<string, vector<string>>::iterator best_agent;
    // The no. of skills covered by the best agent
    int max_count_skills = 0;

    for(auto it = Agents.begin(); it != Agents.end(); it++)
    {
      int count_skills = 0;
      for(const auto &skill: it->second)
      {
        if (count(task_skills.begin(), task_skills.end(), skill))
        {
          count_skills++;
        }
      }
      if (count_skills > max_count_skills)
      {
        max_count_skills = count_skills;
        best_agent = it;
      }
    }

    for (const auto &skill: best_agent->second)
    {
      if (count(task_skills.begin(), task_skills.end(), skill))
        {
          task_skills.erase()
        }
    }
  }
}


int main()
{
  // Create a map where key is name of Agent and value is a vector
  // of the skills that the agent has.
  map<string, vector<string>> Agents;
  
  // Take input from user about the skills in the required task
  int task_len;
  cout << "Enter no. of skills in task : ";
  cin >> task_len;
  vector<string> task_skills(task_len);
  for(int i = 0; i < task_len; i++)
  {
    cin >> task_skills[i];
  }

  parse_file(Agents);
  // print_Agents(Agents);

  // Save all agent names in a map so that each agent can be
  // referred to using their index instead of the entire email string
  // thus making it easier to store them in the Graph
  unordered_map<int, string> agent_names;
  int i = 0;
  for (auto it = Agents.begin(); it != Agents.end(); ++it)
  {
    agent_names.insert({i, it->first});
    ++i;
  }

  // Create a Communication graph where each node represents an Agent(in terms of its index)
  // in the `Agents` map.
  // And the edge weight between two agents A and B = (1 - (A intersection B / A union B))
  Graph comms_graph(Agents.size());
  i = 0;
  for (auto it = Agents.begin(); it != Agents.end(); ++it)
  {
    // For every agent find its communication with all other agents and
    // add an edge to the graph with the corresponding weight ONLY IF
    // the weigth > 0
    for (auto it2 = next(it, 1); it2 != Agents.end(); ++it2)
    {
      float weight = edge_weight(it->second, it2->second);
      if (weight > 0)
        comms_graph.addEdge(i,
                            distance(Agents.begin(), it2),
                            weight);
    }
    i++;
  }

  comms_graph.printGraph();
  // cout << "Shortest cycle with maximum weight: " << comms_graph.shortestCycleMaxWeight() << endl;

  return 0;
}