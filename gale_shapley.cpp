#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <queue>

using namespace std;

typedef vector<int> PrefList;
typedef map<int,int> Couples;

int to_int( const string &Text )//Text not by const reference so that the function can be used with a 
{                               //character array as argument
  stringstream ss(Text);
  int result;
  return ss >> result ? result : 0;
}

string printPrefList(PrefList list){
  string result = "[ ";
  if(list.size() == 1)
    return result + " 0 ]";
  else {
    for (int i = 0; i < list.size(); i++)
    {
      result += to_string(list[i]);
      if (i != list.size() - 1)
      {
        result += ", ";
      }
    }
    result += " ]";
  }
  return result;
}

string printVectorPrefList(vector<PrefList> list){ 
  string result;
  for (int i = 0; i < list.size(); i++)
  {
    if (list[i].empty())
      {
        cout << "no deberias estar aca" << endl;
      }
    result += printPrefList(list[i]);
    result += " ";
  }
  return result;
}

void printMarriages(Couples matching){
  for (Couples::const_iterator it = matching.begin(); it != matching.end(); ++it)
      {
          cout << "\t" << it->first << " and " << it->second << "\n";
      }
  // for(int i= 0; i < matching.size(); i++)
  //   result += to_string(matching[i + 1]) + " <-> " + to_string(i + 1) + "\n";
  // for(Couples::const_iterator it = matching.begin(); it != matching.end(); ++it) {
  //   cout << it->first << " <-> " << it->second << endl;
  // }
}



vector< PrefList > loadData(char const filename[]){
  vector< PrefList > preferences;
  PrefList indiv_prefs;
  ifstream inputFile(filename);

  bool first_line_flag = true;
  bool last_value_was_new_line = false;

  if (inputFile)
  {
    string value;
    while(inputFile >> value) {
      if (value.find(":") != std::string::npos)
      { 
        //Pasamos por nueva lista de preferencia
        if (first_line_flag)
        { 
          first_line_flag = false;
        }
        else
        {
          if (indiv_prefs.empty())
          {
            //Si las preferencias quedaron vacias, es porque hay una lista vacia
            //Rellenar con 0
            indiv_prefs.push_back(0);
          }
          preferences.push_back(indiv_prefs);
          indiv_prefs.clear();
        }
        last_value_was_new_line = true;
      }
      else
      {
        last_value_was_new_line = false;
        //Si pasamos por un parentesis, seguir apendeando items hasta que termine el empate
        if (value.find("(") != std::string::npos)
        {
          while(inputFile >> value) {
              if (value.find(")") != std::string::npos)
              {
                //Se encontro el final del empate. Limpiamos aux para lo que venga
                // y agregamos el empate a la lista de pref.
                indiv_prefs.push_back(to_int(value));
                break;
              }
              else 
              {
                // aux.push_back(value);
                indiv_prefs.push_back(to_int(value));
              }
          }
        }
        else
        {
          //Si no hay empate, entonces es valor
          indiv_prefs.push_back(to_int(value));
        }
      }
    }
  }
  preferences.push_back(indiv_prefs);
  inputFile.close();
  return preferences;
}

//Returns true if the husband comes before the newGuy in the girl's preference list
bool prefersCurrent(PrefList girlList, int husband, int newGuy){
  for(int i = 0; i < girlList.size(); i++) {
    if (girlList[i] == husband)
      return true;
    if (girlList[i] == newGuy)
      return false;
  }
}

int main(int argc, char const *argv[])
{
  const clock_t begin_time = clock();
  vector< PrefList > prefs;
  vector <PrefList> menPrefs;
  vector <PrefList> womenPrefs;
  queue<int> bachelors;
  Couples couples; //<girl,boy>

  prefs = loadData(argv[1]);
  for (int i = 0; i < prefs.size()/2; i++)
  {
    menPrefs.push_back(prefs[i]);
    //Fill the bachelors
    bachelors.push(i+1);
  }
  for (int i = prefs.size()/2; i < prefs.size(); i++)
  {
    womenPrefs.push_back(prefs[i]);
  }
  // cout << printVectorPrefList(prefs) << endl;
  
  while(!bachelors.empty()) {
    const int &champ = bachelors.front();
    const PrefList &preferences = menPrefs[champ - 1];
    //Por cada chica en la lista de champ
    for (PrefList::const_iterator it = preferences.begin(); it != preferences.end(); ++it)
    {
      const int &girl = *it;
      if (couples.find(girl) == couples.end()) //no esta en couples => esta soltera
      {
        couples[girl] = champ;
        break;
      }

      //Obtenemos al marido de la chica
      const int &boyfriend = couples[girl];

      //If she rather wants to have our champ instead instead of her husband
      if (!prefersCurrent(womenPrefs[girl - 1], boyfriend, champ))
      {
        //Husband is now an ex, and single
        bachelors.push(boyfriend);
        //And she got remarried
        couples[girl] = champ;
        break;
      }
    }
    bachelors.pop();
  }

  // printMarriages(couples);
  cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " seconds" << endl;
  return 0;
}