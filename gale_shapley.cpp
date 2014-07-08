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

//Deletes a specefied value on a vector
void deleteElement(vector<int>& vector, int personId)
{
  std::vector<int>::iterator position = find(vector.begin(), vector.end(), personId);
  if (position != vector.end()) // == vector.end() means the element was not found
      vector.erase(position);
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
      cout << "\t" << it->second << " and " << it->first << "\n";
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
  vector<int> originBachelors;
  vector<int> originSingleGirls;
  int next = 0;
  int numberOfGirls;

  for (int i = 0; i < menPrefs.size(); i++)
  {
    originBachelors.push_back(i+1);
  }
  for (int i = 0; i < womenPrefs.size(); i++)
  {
    originSingleGirls.push_back(i+1);
  }

  Couples fCheck;

  for (int i = 0; i < originSingleGirls.size(); i++)
  {
    fCheck.clear();
    //Realizar mismo procedimiento para cada posible pareja del primer hombre
    //Ej
    // 1 -> 1
    // 1 -> 2
    // 1 -> 3
    // 1 -> 4
    vector<int> bachelors = originBachelors;
    vector<int> singleGirls = originSingleGirls;
    
    int boy = bachelors.front();
    int girl = singleGirls[i];
    int loopsNumber = bachelors.size();
    //Primero asociamos la pareja
    cout << "chico " << boy << " con chica " << girl << endl;
    fCheck[girl] = boy;
    cout << "tamaño de matching: " << fCheck.size() << endl;
    numberOfGirls = womenPrefs.size();

    //DEBEN EXISTIR LISTA DE PREFERENCIAS GLOBALES, INMUTABLES
    //AL ELIMINAR A UN HOMBRE DE LISTA SOLTEROS Y MUJER DE LISTA DE SOLTEROS,
    //HAY QUE BORRARLOS DE LAS LISTAS DE PREFERENCIAS TEMPORALES (EXISTEN SOLO PARA LA ACTUAL ITERACION)

    //Se borran los elegidos de la lista
    deleteElement(singleGirls, girl);
    deleteElement(bachelors, boy);
    while(loopsNumber >= 0) {
      int girl = singleGirls.front();
      int boy = bachelors.front();
      cout << "Buscando mina para el hombre " << boy << endl;
      //Hacerlos pareja y revisar estabilidad
      fCheck[girl] = boy;
      cout << "chico " << boy << " con chica " << girl << endl;
      while(!matchingStable(fCheck, menPrefs, womenPrefs)) {
        //Si es inestable, hay que deshacer la ultima pareja
        //Intentar emparejar a boy con la siguiente soltera (sin eliminar a girl de la lista de solteras)
        cout << "...MATCHING INESTABLE" << endl;
        fCheck.erase(girl);
        next += 1;
        if (next == numberOfGirls - 1)
        {
          cout << "No hay mas chicas para intentar" << endl;
          break;
        }
        girl = singleGirls[next];
        fCheck[girl] = boy;
        cout << "chico " << boy << " con chica " << girl << endl;
      }
      cout << "...MATCHING ESTABLE" << endl;
      //La pareja hace el match estable
      //Se mantienen casados, pero se borran de la lista de solteros
      deleteElement(singleGirls, girl);
      deleteElement(bachelors, boy);

      // cout << "chicas ";
      // printVector(singleGirls);
      // cout << "solteros ";
      // printVector(bachelors);
      if (singleGirls.empty() or bachelors.empty())
      {
        break;
      }
      loopsNumber--;
    }
    cout << "Matrimonios estables:" << endl;
    printMarriages(fCheck);

    cout << "===========================" << endl;
  }
  
  // cout << "chicas ";
  // printVector(singleGirls);
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
  Couples couples;

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

  forwardChecking(menPrefs, womenPrefs);
  // couples = galeShapley(&bachelors, menPrefs, womenPrefs);
  // printMarriages(couples);
  // cout << couples.size() << endl;
  cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " seconds" << endl;
  return 0;
}