#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

vector<string> dividir_espacios(string text);
double pearson(int pos, int pos2, vector<vector<double>> usuarios, vector<double> medias);
double coseno(int pos, int pos2, vector<vector<double>> usuarios, vector<double> medias);
double euclidea(int pos, int pos2, vector<vector<double>> usuarios, vector<double> medias);
double prediccion_simple(string metrica, vector<vector<double>> usuarios, vector<double> medias, vector<int> filas_vacias, int vecinos);
double prediccion_doble(string metrica, vector<vector<double>> usuarios, vector<double> medias, vector<int> filas_vacias, int vecinos);
vector<int> filas_vacias(vector<vector<string>> solucion);

int main (int argc, char *argv[]) {
    string filename(argv[1]);
    string filename2(argv[2]);
    string metrica = argv[3];
    string vecinos = argv[4];
    string prediccion = argv[5];
    vector<string> lines;
    vector<vector<string>> solucion;
    string line;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
                << filename << "'" << endl;
        return EXIT_FAILURE;
    }

    while (getline(input_file, line)){
        lines.push_back(line);
    }

    for (size_t i = 0; i < lines.size(); i++) {
        solucion.push_back(dividir_espacios(lines[i]));
    }
    

    // Se almacenan los valores enteros de la matriz es otra matriz de usuarios
    vector<vector<double>> usuarios;
    for (size_t i = 0; i < solucion.size(); i++) {
        vector<double> usuario;
        for (size_t j = 0; j < solucion[i].size(); j++) {
            if (solucion[i][j] != "-") {
                usuario.push_back(stoi(solucion[i][j]));
            }
        }
        usuarios.push_back(usuario);
    }

    // Se calculan las medias de cada usuario y se guardan en un vector
    vector<double> medias;
    for (size_t i = 0; i < usuarios.size(); i++) {
        double aux = 0;
        double cont = 0;
        for (size_t j = 0; j < usuarios[i].size(); j++) {
            aux += usuarios[i][j];
            cont++;
        }
        medias.push_back(aux/cont);
    }
    
    double valor = 0;
    // Calculo de predicciones
    if (prediccion == "simple") {
        valor = prediccion_simple(metrica, usuarios, medias, filas_vacias(solucion),atoi(vecinos.c_str()));
    } else if (prediccion == "doble") {
        valor = prediccion_doble(metrica, usuarios, medias, filas_vacias(solucion),atoi(vecinos.c_str()));
    }
    

    // Sustituimos el - por el valor calculado    
    for (size_t i = 0; i < solucion.size(); i++) {
        vector<double> usuario;
        for (size_t j = 0; j < solucion[i].size(); j++) {
            if (solucion[i][j] == "-") {
                solucion[i][j] = to_string(valor);
            }
        }
    }

    fstream file_out;

    file_out.open(filename2, std::ios_base::out);
    if (!file_out.is_open()) {
        cout << "failed to open " << filename2 << '\n';
    } else {
        for (size_t i = 0; i < solucion.size(); i++) {
            for (size_t j = 0; j < solucion[i].size(); j++) {
                file_out << solucion[i][j] << " ";
            }
            file_out << endl;
        }   
        cout << "Done Writing!" << endl;
    }

    input_file.close();
    return EXIT_SUCCESS;
}

vector<string> dividir_espacios(string text) {
    string delimiter = " ";
    vector<string> words;

    int start = 0;
    int end = text.find(delimiter);
    while (end != -1) {
        words.push_back(text.substr(start, end - start));
        start = end + delimiter.size();
        end = text.find(delimiter, start);
    }
    words.push_back(text.substr(start, end - start));
    
    return words;
}

double pearson(int pos, int pos2, vector<vector<double>> usuarios, vector<double> medias) {
    double sum_X = 0, sum_Y = 0, sum_XY = 0, squareSum_X = 0, squareSum_Y = 0;
    for (size_t i = 0; i < usuarios[pos].size(); i++) {
        sum_X = sum_X + usuarios[pos][i];
        sum_Y = sum_Y + usuarios[pos2][i];
        sum_XY = sum_XY + usuarios[pos][i] * usuarios[pos2][i];
        squareSum_X = squareSum_X + usuarios[pos][i] * usuarios[pos][i];
        squareSum_Y = squareSum_Y + usuarios[pos2][i] * usuarios[pos2][i];
    }

    double corr = (double)(usuarios[pos].size() * sum_XY - sum_X * sum_Y) / sqrt((usuarios[pos].size() * squareSum_X - sum_X * sum_X) * (usuarios[pos].size() * squareSum_Y - sum_Y * sum_Y));
    return corr;
}

double coseno(int pos, int pos2, vector<vector<double>> usuarios, vector<double> medias) {
    double sum_XY = 0, squareSum_X = 0, squareSum_Y = 0;
    for (size_t i = 0; i < usuarios[pos].size(); i++) {
        sum_XY = sum_XY + usuarios[pos][i] * usuarios[pos2][i];
        squareSum_X = squareSum_X + usuarios[pos][i] * usuarios[pos][i];
        squareSum_Y = squareSum_Y + usuarios[pos2][i] * usuarios[pos2][i];
    }
 
    double cos = (double)(sum_XY) / (sqrt(squareSum_X) * sqrt(squareSum_Y));
    return cos;
}

double euclidea(int pos, int pos2, vector<vector<double>> usuarios, vector<double> medias) {
    double sum_X = 0, sum_Y = 0;
    for (size_t i = 0; i < usuarios[pos].size(); i++) {
        sum_X = sum_X + usuarios[pos][i];
        sum_Y = sum_Y + usuarios[pos2][i];
    }

    double euc = sqrt((sum_X-sum_Y)*(sum_X-sum_Y));
    return euc;
}

vector<int> filas_vacias(vector<vector<string>> solucion) {
    vector<int> fila_vacia;
    for (size_t i = 0; i < solucion.size(); i++) {
        for (size_t j = 0; j < solucion[i].size(); j++) {
            if (solucion[i][j] == "-") {
                fila_vacia.push_back(i);
            }
        }
    }

    return fila_vacia;
}

double prediccion_simple(string metrica, vector<vector<double>> usuarios, vector<double> medias, vector<int> filas_vacias, int vecinos) {
    if (metrica == "euc") {
        vector<pair<double, double>> clave;
        vector<double> resultados_euclidea;
        for (size_t i = 0; i < filas_vacias.size(); i++) {
            for (size_t j = 0; j < usuarios.size(); j++)
            {
                resultados_euclidea.push_back(euclidea(i,j,usuarios,medias));
            }
        }
        for (size_t i = 0; i < resultados_euclidea.size(); i++) {
            pair<double, double> aux;
            aux.first = i;
            aux.second = resultados_euclidea[i];
            clave.push_back(aux);
        }
        sort(clave.begin(), clave.end());

        double a=0,b=0;
        for (size_t i = 0; i < vecinos; i++) {
            a = a + clave[i].second * usuarios[clave[i].first][i];
            b = b + abs(clave[i].second);
        }
        
        double c = a/b;
        cout << c << endl;
        return c;
    } else if (metrica == "pears") {
        vector<pair<double, double>> clave;
        vector<double> resultados_pearson;
        for (size_t i = 0; i < filas_vacias.size(); i++) {
            for (size_t j = 0; j < usuarios.size(); j++) {
                resultados_pearson.push_back(pearson(i,j,usuarios,medias));
            }
        }
        for (size_t i = 0; i < resultados_pearson.size(); i++) {
            pair<double, double> aux;
            aux.first = i;
            aux.second = resultados_pearson[i];
            clave.push_back(aux);
        }

        sort(clave.begin(), clave.end());

        double a=0,b=0;
        for (size_t i = 0; i < vecinos; i++) {
            a = a + clave[i].second * usuarios[clave[i].first][i];
            b = b + abs(clave[i].second);
        }
        double c = a/b;
        cout << c << endl;
        return c;

    } else if (metrica == "cos") {
        vector<pair<double, double>> clave;
        vector<double> resultados_coseno;
        for (size_t i = 0; i < filas_vacias.size(); i++) {
            for (size_t j = 0; j < usuarios.size(); j++)
            {
                resultados_coseno.push_back(coseno(i,j,usuarios,medias));
            }
        }
        for (size_t i = 0; i < resultados_coseno.size(); i++) {
            pair<double, double> aux;
            aux.first = i;
            aux.second = resultados_coseno[i];
            clave.push_back(aux);
        }
        sort(clave.begin(), clave.end());

        double a=0,b=0;
        for (size_t i = 0; i < vecinos; i++) {
            a = a + clave[i].second * usuarios[clave[i].first][i];
            b = b + abs(clave[i].second);
        }
        double c = a/b;
        cout << c << endl;
        return c;
    } else {
        exit;
        return -1;
    }
}

double prediccion_doble(string metrica, vector<vector<double>> usuarios, vector<double> medias, vector<int> filas_vacias, int vecinos) {
    if (metrica == "euc") {
        vector<pair<double, double>> clave;
        vector<double> resultados_euclidea;
        for (size_t i = 0; i < filas_vacias.size(); i++) {
            for (size_t j = 0; j < usuarios.size(); j++)
            {
                resultados_euclidea.push_back(euclidea(i,j,usuarios,medias));
            }
        }
        for (size_t i = 0; i < resultados_euclidea.size(); i++) {
            pair<double, double> aux;
            aux.first = i;
            aux.second = resultados_euclidea[i];
            clave.push_back(aux);
        }
        sort(clave.begin(), clave.end());

        double a=0,b=0;
        for (size_t i = 0; i < vecinos; i++) {
            a = a + clave[i].second * (usuarios[clave[i].first][i]-medias[clave[i].first]);
            b = b + abs(clave[i].second);
        }
        
        double c = medias[clave[0].first] + a/b;
        cout << c << endl;
        return c;
    } else if (metrica == "pears") {
        vector<pair<double, double>> clave;
        vector<double> resultados_pearson;
        for (size_t i = 0; i < filas_vacias.size(); i++) {
            for (size_t j = 0; j < usuarios.size(); j++) {
                resultados_pearson.push_back(pearson(i,j,usuarios,medias));
            }
        }
        for (size_t i = 0; i < resultados_pearson.size(); i++) {
            pair<double, double> aux;
            aux.first = i;
            aux.second = resultados_pearson[i];
            clave.push_back(aux);
        }

        sort(clave.begin(), clave.end());

        double a=0,b=0;
        for (size_t i = 0; i < vecinos; i++) {
            a = a + clave[i].second * (usuarios[clave[i].first][i]-medias[clave[i].first]);
            b = b + abs(clave[i].second);
        }
        
        double c = medias[clave[0].first] + a/b;
        cout << c << endl;
        return c;

    } else if (metrica == "cos") {
        vector<pair<double, double>> clave;
        vector<double> resultados_coseno;
        for (size_t i = 0; i < filas_vacias.size(); i++) {
            for (size_t j = 0; j < usuarios.size(); j++)
            {
                resultados_coseno.push_back(coseno(i,j,usuarios,medias));
            }
        }
        for (size_t i = 0; i < resultados_coseno.size(); i++) {
            pair<double, double> aux;
            aux.first = i;
            aux.second = resultados_coseno[i];
            clave.push_back(aux);
        }
        sort(clave.begin(), clave.end());

        double a=0,b=0;
        for (size_t i = 0; i < vecinos; i++) {
            a = a + clave[i].second * (usuarios[clave[i].first][i]-medias[clave[i].first]);
            b = b + abs(clave[i].second);
        }
        
        double c = medias[clave[0].first] + a/b;
        cout << c << endl;
        return c;
    } else {
        exit;
        return -1;
    }
}