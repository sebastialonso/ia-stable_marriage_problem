#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// Codigo clase Man

class Man
{
  vector< vector<string> > prefs;
  int id;
  int couple_id;
public:
  Man(vector< vector<string> >);
  vector< vector<string> > preferences () { return prefs; }
  void set_id (int);
  void go_single ();
  void marry (int);

  int get_id () { return id; }
  bool is_single () 
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
  int couple () { return couple_id;}
};

Man::Man (vector < vector<string> > preferences) {
  prefs = preferences;
  couple_id = -1;
  // single = true;
}
void Man::set_id (int idSource) {
  id = idSource;
}
void Man::go_single () {
  couple_id = -1;
}
void Man::marry (int someone_id) {
  couple_id = someone_id;
}

// END codigo clase

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
  // cout << "numero de listas: " << prefs.size() << endl;
  // for (int i = 0; i < prefs.size(); i++)
  // {
  //   cout << i + 1 << ": " << print_vector_vector(prefs[i]) << endl;
  // }
  vector<Man> men;
  
  for (int i = 0; i < prefs.size()/2; i++)
  {
    // cout << i << endl;
    Man auxiliary_man (prefs[i]);
    auxiliary_man.set_id(i + 1);
    men.push_back(auxiliary_man);
  }
  men[0].marry (5);
  if (men[0].preferences().size() == 10)
  {

    cout << "Hombre " << men[0].get_id() << " tiene 10 preferencias" << endl;
  }
  else
  {
    cout << men[0].preferences().size() << endl;
  }
  cout << men[0].is_single() << endl;
  if (men[0].is_single())
  {
    cout << "soltero" << endl;
  }
  else
  {
    cout << "casado con " << men[0].couple() << endl;
  }
}
