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
typedef PrefList nCouples;
typedef map<int,int> Couples;
typedef vector<Couples> Matchings;

int to_int( const string &Text )//Text not by const reference so that the function can be used with a 
{                               //character array as argument
  stringstream ss(Text);
  int result;
  ss >> result;
  return result;
}

//Deletes a specefied value on a vector
void deleteElement(vector<int>& vector, int personId)
{
  std::vector<int>::iterator position = find(vector.begin(), vector.end(), personId);
  if (position != vector.end()) // == vector.end() means the element was not found
      vector.erase(position);
}

vector<int> join(vector<int> first, vector<int> second)
{
  if (second.empty())
  {
    return first;
  }
  vector<int> result;
  result.reserve( first.size() + second.size());
  result.insert( result.end(), first.begin(), first.end() );
  result.insert( result.end(), second.begin(), second.end() );
  return result;
}
string printPrefList(PrefList list){
  string result = "[ ";
  if(list.size() == 0)
    return result + " ]";
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

bool matchingStable(Couples matching, vector<PrefList> menPrefs, vector<PrefList> womenPrefs)
{
  for (auto it = matching.cbegin(); it != matching.cend() ; ++it)
  {
    int bride = (*it).first;
    int herGroom = (*it).second;
    for (auto newGuyIt = matching.cbegin(); newGuyIt != matching.cend(); ++newGuyIt)
    {
      int someGuy = (*newGuyIt).second;
      int hisWife = (*newGuyIt).first;

      if (!prefersCurrent(womenPrefs[bride - 1], herGroom, someGuy) && !prefersCurrent(menPrefs[someGuy - 1], hisWife, bride))
      {
        return false;
      }
    }
  }
  return true;
}

//Override for forward checking algorithn
bool matchingStable(vector<int> matching, vector<PrefList> menPrefs, vector<PrefList> womenPrefs)
{
  //Para cada elemento del matching
  for (int i = 0; i < matching.size(); i++)
  {
    int bride = matching[i];
    int herGroom = i + 1; 
    //Por cada otra pareja existente hasta el momento
    for (int j = 0; j < matching.size(); j++)
    {
      int someGuy = j + 1;
      int hisWife = matching[j];
      //Seguimos revisando a menos que sea la misma pareja anterior
      if (!prefersCurrent(womenPrefs[bride - 1], herGroom, someGuy) && !prefersCurrent(menPrefs[someGuy - 1], hisWife, bride))
      {
        return false;
      }
    }
    return true;
  }
}

vector<vector<PrefList>> arcConsistency(vector<PrefList> menPrefs, vector<PrefList> womenPrefs)
{
  vector<vector<PrefList>> consistentPrefLists;
  vector<PrefList> menPrefsCopy = menPrefs;
  vector<PrefList> womenPrefsCopy = womenPrefs;

  for(int i = 0; i < menPrefs.size(); i++) {
    if (!menPrefs[i].empty())
    {
      cout << printPrefList(menPrefs[i]) << endl;
      for (int j = 0; j < menPrefs[i].size(); j++)
      {
        cout << "Existe hombre " <<  i + 1 << " en la lista de mujer " << menPrefs[i][j] << " ~> " << printPrefList(womenPrefs[menPrefs[i][j] - 1]) << endl;
        if (find(womenPrefs[menPrefs[i][j] - 1].begin(), womenPrefs[menPrefs[i][j] - 1].end(), i + 1) == womenPrefs[menPrefs[i][j] - 1].end())
        {
          cout << i + 1 << " no aparece  en la lista de " << j + 1 << endl;
          cout << "Borramos a " << j + 1 << " de la lista de " << i +1 << endl;
          /* no forman par aceptable */
          /* borrar mujer j (valor j + 1) de la lista i (del hombre i + 1) */
          deleteElement(menPrefsCopy[i], j + 1);
        }
        else
        {
          cout << "Si aparece, todo bien" << endl;
        }
      }
    }
  }
  consistentPrefLists.push_back(menPrefsCopy);
  consistentPrefLists.push_back(womenPrefsCopy);
  return consistentPrefLists;
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

//void fCheck(Couples& matching, vector<PrefList>& domains, int depth, int valueToDelete)
void fCheck(long long& numberStableMatching, nCouples& matching, vector<PrefList>& domains, int depth, int valueToDelete, vector<PrefList> menPrefs, vector<PrefList> womenPrefs, int& biggestMatch)
{
  //Si se llega al maximo nivel de profundidad
  if (depth >= domains.size())
  {
    if (biggestMatch < matching.size())
    {
      biggestMatch = matching.size();
    }
    numberStableMatching++;
  }
  else
  {
    //Borrar el valueToDelete dominio de los hombres id > depth
    for (int i = depth; i < domains.size(); i++)
    {
      deleteElement(domains[i], valueToDelete);
    }
    for (int j = 0; j < domains[depth].size(); j++)
    {
      matching.push_back(domains[depth][j]);
      if (matchingStable(matching, menPrefs, womenPrefs))
      {
        /* Dado que no es necesario llegar hasta la maxima profundidad del arbol para encontrar un matching maximal estable,
        registramos este matching parcial
        */
        biggestMatch = matching.size();
        numberStableMatching++;

        vector<PrefList> domainsCopy = domains;
        fCheck(numberStableMatching, matching, domainsCopy, depth + 1, domains[depth][j], menPrefs, womenPrefs, biggestMatch);
        matching.pop_back();
      }
      else
      {
        matching.pop_back();
      }
    }
  }
}

void forwardChecking(vector<PrefList> menPrefs, vector<PrefList> womenPrefs, int& biggestMatch)
{
  vector<PrefList> matrix;
  PrefList temp;
  //Construccion del dominio
  for (int i = 0; i < menPrefs.size(); i++)
  {
    temp.clear();
    for (int j = 0; j < menPrefs[i].size(); j++)
    {
      temp.push_back(menPrefs[i][j]);
    }
    matrix.push_back(temp);
  }

  nCouples matching;
  // vector<nCouples> stableMatchings;
  long long numberStableMatching = 0;
  //Comienza el arbol, llamamos recursivamente a fCheck para cada miembro de la lista de preferencias del primer hombre
  for (int i = 0; i < matrix[0].size(); i++)
  {
    // cout << "Nodo " << i + 1 << endl;
    matching.push_back(matrix[0][i]);
    vector<PrefList> domainCopy = matrix;
    fCheck(numberStableMatching, matching, domainCopy, 1, matrix[0][i], menPrefs, womenPrefs, biggestMatch);
    matching.clear();
  }
  // cout << "Matchings encontrados: " << numberStableMatching << endl;
}

int main(int argc, char const *argv[])
{
  vector< PrefList > prefs;
  vector <PrefList> menPrefs;
  vector <PrefList> womenPrefs;

  int biggestMatch = 0;
  prefs = loadData(argv[1]);
  for (int i = 0; i < prefs.size()/2; i++)
  {
    menPrefs.push_back(prefs[i]);
  }
  for (int i = prefs.size()/2; i < prefs.size(); i++)
  {
    womenPrefs.push_back(prefs[i]);
  }

  /* ORIGINAL GALE & SHAPLEY Algorithm

  queue<int> bachelors;
  Couples couples;
  couples = galeShapley(&bachelors, menPrefs, womenPrefs);
  printMarriages(couples);

  */

  const clock_t optimized_begin_time = clock();
  forwardChecking(menPrefs, womenPrefs, biggestMatch);
  float elapsed_time = float( clock () - optimized_begin_time ) /  CLOCKS_PER_SEC;
  cout << biggestMatch << " " << elapsed_time << endl;
  
  return 0;
}