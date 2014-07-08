#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

typedef vector<int> PrefList;
typedef map<int,int> Couples;
typedef vector<Couples> Matchings;

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
            // indiv_prefs.push_back(0);
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

void printVector(vector<int> vec)
{
  string result = "[ ";
  if (vec.empty())
  {
    cout << result + "] " << endl;
  }
  else 
  {
    for (int i = 0; i < vec.size(); i++)
    {
      result += to_string(vec[i]);
      if (i != vec.size() - 1)
      {
        result += ", ";
      }
    }
    result += " ]";
    cout << result << endl;
  } 
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

//Forces node consistency on the preferences lists, that is, if some person p of sex A has an empty
//preference list, then p is deleted from all the preferences list of people from sex B
void nodeConsistency(vector< PrefList >& menPrefs, vector< PrefList>& womenPrefs)
{
  //Si persona p tiene lista vacia, eliminar toda referencia a persona p
  for(int i = 0; i < menPrefs.size(); i++) {
    //Detecta el hombre de la lista vacia
    if (menPrefs[i].empty())
    {
      int menToDelete = i+1;
      for(int j = 0; j < womenPrefs.size(); j++) {
        PrefList::iterator position = find(womenPrefs[j].begin(), womenPrefs[j].end(), menToDelete);
        if (position != womenPrefs[j].end()) // == vector.end() means the element was not found
            womenPrefs[j].erase(position);
      }
    }
  }
  for(int i = 0; i < womenPrefs.size(); i++) {
    //Detecta el hombre de la lista vacia
    if (womenPrefs[i].empty())
    {
      int menToDelete = i+1;
      for(int j = 0; j < menPrefs.size(); j++) {
        PrefList::iterator position = find(menPrefs[j].begin(), menPrefs[j].end(), menToDelete);
        if (position != menPrefs[j].end()) // == vector.end() means the element was not found
            menPrefs[j].erase(position);
      }
    }
  }
}

//Performs the Gale & Shapley algorithm, returns a stable Matching
Couples galeShapley(queue<int>* bachelors, vector<PrefList> menPrefsIn, vector<PrefList> womenPrefsIn)
{
  Couples couples;
  while(!bachelors->empty()) {
    const int &champ = bachelors->front();
    const PrefList &preferences = menPrefsIn[champ - 1];
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
      if (!prefersCurrent(womenPrefsIn[girl - 1], boyfriend, champ))
      {
        //Husband is now an ex, and single
        bachelors->push(boyfriend);
        //And she got remarried
        couples[girl] = champ;
        break;
      }
    }
    bachelors->pop();
  }
  return couples;
}

void forwardChecking(vector<PrefList> menPrefs, vector<PrefList> womenPrefs)
{
  vector<int> bachelors;
  vector<int> girls;
  int next = 0;
  int loopsNumber = menPrefs.size();

  for (int i = 0; i < menPrefs.size(); i++)
  {
    bachelors.push_back(i+1);
  }
  for (int i = 0; i < womenPrefs.size(); i++)
  {
    girls.push_back(i+1);
  }

  Couples fCheck;
  int girl = girls.front();
  int boy = bachelors.front();
  fCheck[girl] = boy;

  //Se borran los elegidos de la lista
  girls.erase(girls.begin());
  bachelors.erase(bachelors.begin());
  while(loopsNumber != 0) {
    girl = girls.front();
    boy = bachelors.front();

    cout << "chicas ";
    printVector(girls);
    cout << "solteros ";
    printVector(bachelors);

    girls.erase(girls.begin());
    bachelors.erase(bachelors.begin());
    if (girls.empty() or bachelors.empty())
    {
      cout << "chicas ";
      printVector(girls);
      cout << "solteros ";
      printVector(bachelors);
      break;
    }
    loopsNumber--;
  }
  printMarriages(fCheck);
  // cout << "chicas ";
  // printVector(girls);
  // cout << "solteros ";
  // printVector(bachelors);
  // Matchings marriages;
  // return marriages;
}

int main(int argc, char const *argv[])
{
  const clock_t begin_time = clock();
  vector< PrefList > prefs;
  vector <PrefList> menPrefs;
  vector <PrefList> womenPrefs;
  queue<int> bachelors;
  queue<int> sluts;
  Couples couples;

  prefs = loadData(argv[1]);
  for (int i = 0; i < prefs.size()/2; i++)
  {
    menPrefs.push_back(prefs[i]);
    //Fill the bachelors and the sluts
    bachelors.push(i+1);
    sluts.push(i+1);
  }
  for (int i = prefs.size()/2; i < prefs.size(); i++)
  {
    womenPrefs.push_back(prefs[i]);
  }

  // cout << "Hombres" << endl;
  // cout << printVectorPrefList(menPrefs) << endl;
  // cout << "Mujeres" << endl;
  // cout << printVectorPrefList(womenPrefs) << endl;
  forwardChecking(menPrefs, womenPrefs);
  // couples = galeShapley(&bachelors, menPrefs, womenPrefs);

  // printMarriages(couples);
  // cout << couples.size() << endl;
  cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " seconds" << endl;
  return 0;
}