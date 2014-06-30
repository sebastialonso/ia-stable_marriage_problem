#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Codigo clase Man

class Man
{
  vector< vector<string> > prefs;
  int id;
  int couple_id;
public:
  //Constructor
  Man(){
    id = -1;
    couple_id = -1;
    vector< vector<string> > tmp;  prefs = tmp;
  }

  Man(vector < vector<string> > preferences, int idSource) {
    prefs = preferences;
    couple_id = -1;
    id = idSource;
  }

  //Destructor
  ~Man(){
  }

  //setters
  void setId (int idSource){
    id = idSource;
  }

  void setSingle ()
  {
    couple_id = -1;
  }

  void marry (int someone_id)
  {
    couple_id = someone_id;
  }

  //Removes the element in index position and updates accordingly
  void removeAndUpdate(int index)
  {
    vector< vector<string>> updated_prefs = prefs;
    updated_prefs.erase(updated_prefs.begin() + index);
    prefs = updated_prefs;
  }

  //Returns the front element of the vector, and updates accordingly
  vector<string> getFirstAndRemove(){
    vector< vector<string>> updated_prefs = prefs;
    vector<string> front = updated_prefs.front();
    updated_prefs.erase(updated_prefs.begin());
    prefs = updated_prefs;
    return front;
  }

  //getters
  vector< vector<string> > getPreferences () { return prefs; }
  int getId () { return id; }
  bool isSingle () 
  { 
    if (couple_id == -1)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  int getCoupleId () { return couple_id;}

};
//END clase Man

//Codigo clase Couple
class Couple
{
  Man man;
  Man woman;
public:
  Couple(){
  }

  Couple(Man male, Man female)
  {
    man = male;
    woman = female;
    man.marry(woman.getId());
    woman.marry(man.getId());
  }
  ~Couple(){}

  Man getMan() { return man; }
  Man getWoman() { return woman; }
};

//END clase Couple

string bigger_print(vector< vector< vector<string> > > tokens){
  cout << "listas de preferencia: " <<  tokens.size() << endl;
}

string print_vector(vector<string> vec)
{
  string result = "[ ";
  if (vec.empty())
  {
    return result += "]";
  }
  else 
  {
    for (int i = 0; i < vec.size(); i++)
    {
      result += vec[i];
      if (i != vec.size() - 1)
      {
        result += ", ";
      }
    }
    result += " ]";
    return result;  
  }
  
}
string print_vector_vector(vector< vector<string>> token){
  string result;
  for (int i = 0; i < token.size(); i++)
  {
    // cout << print_vector(token[i]);
    if (token[i].empty())
    { 
      result += "[ ]";
    }
    else
    {
      result += print_vector(token[i]);
    }
  }
  return result;
}

void print(string asdf)
{
  cout << asdf << endl;
}

//Retorna verdadero si existe al menos un hombre que aun tenga elementos en su lista de preferencias
bool notAllHaveProposed(vector<Man> men){
  for (int i = 0; i < men.size(); i++)
  {
    vector< vector<string> > prefs = men[i].getPreferences();
    if (!prefs.empty())
    {
      return true;
    }
  }
  return false;
}

//Returns the first Man whose preference list is non-empty
Man firstWithNonEmptyPrefs(vector<Man> men){
  for (int i = 0; i < men.size(); i++)
  {
    if (!men[i].getPreferences().empty())
    {
      return men[i];
    }
  }
}

int main(int argc, char const *argv[])
{ 
  vector< vector< vector<string> > > prefs;
  vector< vector<string> > indiv_prefs;
  vector<string> aux;
  ifstream inputFile(argv[1]);

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
          prefs.push_back(indiv_prefs);
          indiv_prefs.clear();
          aux.clear();
          if (last_value_was_new_line)
          {
            //Encontramos una lista vacia
            //se agrega a preferencias
            prefs.push_back(indiv_prefs);
            last_value_was_new_line = true;
          }
        }
      }
      else
      {
        //Si pasamos por un parentesis, seguir apendeando items hasta que termine el empate
        if (value.find("(") != std::string::npos)
        {
          while(inputFile >> value) {
              if (value.find(")") != std::string::npos)
              {
                //Se encontro el final del empate. Limpiamos aux para lo que venga
                // y agregamos el empate a la lista de pref.
                indiv_prefs.push_back(aux);
                aux.clear();
                break;
              }
              else 
              {
                aux.push_back(value);
              }
          }
        }
        else
        {
          //Si no hay empate, entonces es valor
          aux.push_back(value);
          indiv_prefs.push_back(aux);
          aux.clear();
        }
        last_value_was_new_line = false;
      }
    }
  }
  prefs.push_back(indiv_prefs);
  inputFile.close();

  //Revisamos el vector de preferencias
  cout << "numero de listas: " << prefs.size() << endl;
  // for (int i = 0; i < prefs.size(); i++)
  // {
  //   cout << i + 1 << ": " << print_vector_vector(prefs[i]) << endl;
  // }
  vector<Man> men;
  vector<Man> women;
  
  //Construccion de individuos
  for (int i = 0; i < prefs.size()/2; i++)
  {
    Man auxiliary_man(prefs[i], i + 1);
    men.push_back(auxiliary_man);
  }
  for (int i = prefs.size()/2; i < prefs.size(); i++)
  {
    Man auxiliary_woman(prefs[i], i - prefs.size()/2 + 1);
    women.push_back(auxiliary_woman);
  }
  //Fin construccion

  if (men[0].getPreferences().size() == 10)
  {

    cout << "Hombre " << men[0].getId() << " tiene 10 preferencias" << endl;
  }
  else
  {
    cout << men[0].getPreferences().size() << endl;
  }

  // men[0].remove_and_update(0);
  // Couple coup(Man men[0], Man women[3]);

  cout << "No todos han propuesto:" << notAllHaveProposed(men) << endl;
  
  //mientras exista al menos un hombre m que no haya propuesto a todas
  while(notAllHaveProposed(men)){
    Man firstOne = firstWithNonEmptyPrefs(men);
    //tomar la mujer w mejor rankeada que quede en la lista de m 
    cout << print_vector_vector(firstOne.getPreferences()) << endl;
    //(hacer un pop o algo del vector, de manera que desaparezca de la lista de pref)
    //TODO: Revisar si es un vector de 1 elemento o n
    vector<string> bestGalId = firstOne.getFirstAndRemove();
    cout << print_vector_vector(firstOne.getPreferences()) << endl;
    cout << bestGalId[0] << endl;
    break;
  }
  //ALGORITMO A GRANDES RASGOS
  //mientras exista al menos un hombre m que no haya propuesto a todas
    //tomar la mujer w mejor rankeada que quede en la lista de m 
    //(hacer un pop o algo del vector, de manera que desaparezca de la lista de pref)
    // si esta soltera
      // m y w son pareja
    // else si w esta emparejada
      // si w prefiere a m antes que a su esposo m'
        // m y w forman pareja
        // m' ahora es soltero
      // else w y m' siguen en pareja










  // if (men[0].isSingle())
  // {
  //   cout << "soltero" << endl;
  // }
  // else
  // {
  //   cout << "El " << men[0].getId() << " dice que esta casado con " << men[0].getCoupleId() << endl;
  //   cout << "Ella " << women[4].getId() << " dice que esta casada con " << women[4].getCoupleId() << endl;
  // }

}
