#include <bits/stdc++.h>

using namespace std;

void parse_file(map<string, vector<string>> &Agents, set<string> &Skills)
{
  // string str = "bala@dsri.com = agent computing, distributed data mining, intrusion detection";
  vector<string> skills;
  string agent, skill, line;
  ifstream inputFile("ACM_DataSet.txt");
  int i = 1;

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
      // cout << i << ". Agent: " << agent << endl;
      // cout << "Skills: ";

      auto ret = Agents.insert({agent, skills});
      Skills.insert(skills.begin(), skills.end());
      // For cases where an agent already exists in the map then just insert
      // the new skills into its value
      if (ret.second == false)
      {
        // cout << i << ". Agent: " << agent << endl;
        ret.first->second.insert(ret.first->second.end(), skills.begin(), skills.end());
      }

      // Agents[agent] = skills;
      // for (const auto &s : skills)
      // {
      //   cout << s << " | ";
      // }

      // cout << endl;
      skills.clear();
      i++;
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
  int i = 1;
  for (auto it = Agents.begin(); it != Agents.end(); ++it)
  {
    cout << i++ << ". Agent: " << it->first << endl;
    cout << "Skills: ";
    for (const auto s : it->second)
    {
      cout << s << " |";
    }

    cout << endl;
  }
}

void print_Skills(set<string> &Skills)
{
  int i = 1;
  for (const auto &s : Skills)
  {
    cout << i << ". " << s << endl;
    i++;
  }
}

vector<string> get_agents_with_skills(vector<string> task_skills, map<string, vector<string>> Agents)
{
  // Agents that have atleast one of the given skills
  vector<string> skillful_agents;
  // Solving using the set cover problem
  while (!task_skills.empty())
  {
    // cout << "Finding best agent\n";
    // The agent who covers the most skills in the task_skills
    map<string, vector<string>>::iterator best_agent;
    // The no. of skills covered by the best agent
    int max_count_skills = 0;

    for (auto it = Agents.begin(); it != Agents.end(); it++)
    {
      // cout << "For Agent : " << it->first << endl;
      int count_skills = 0;
      for (const auto &skill : it->second)
      {
        if (count(task_skills.begin(), task_skills.end(), skill))
        {
          count_skills++;
          // cout << "Count_skills : " << count_skills << endl;
        }
      }
      if (count_skills > max_count_skills)
      {
        max_count_skills = count_skills;
        best_agent = it;
      }
    }

    // cout << "Best Agent Found : " << best_agent->first << endl;
    for (const auto &skill : best_agent->second)
    {
      auto it = find(task_skills.begin(), task_skills.end(), skill);
      if (it != task_skills.end())
      {
        task_skills.erase(it);
        // cout << "Removed skill : " << skill << endl;
      }
    }

    skillful_agents.push_back(best_agent->first);
    Agents.erase(best_agent->first);
    // cout << "Task Skills Size: " << task_skills.size() << endl;
  }

  return skillful_agents;
}

int main(int argc, char const *argv[])
{
  map<string, vector<string>> Agents;
  set<string> Skills;

  parse_file(Agents, Skills);
  // print_Agents(Agents);
  // print_Skills(Skills);

  // Take input from user about the skills in the required task
  int task_len;
  cout << "Enter no. of skills in task : ";
  cin >> task_len;
  vector<string> task_skills;
  string input;
  for (int i = 0; i < task_len; i++)
  {
    getline(cin, input);
    task_skills.push_back(input);
  }

  vector<string> team = get_agents_with_skills(task_skills, Agents);

  cout << "The best team for given task is : \n";
  for (const auto &a : team)
  {
    cout << a << endl;
  }
  return 0;
}
