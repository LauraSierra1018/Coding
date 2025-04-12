#include <iostream>
#include <cctype>
#include <vector>

using namespace std;

const int FILAS = 10;
const int COLUMNAS = 10;

class Tablero {
private:
    int matriz[FILAS][COLUMNAS];
    vector<int> golpes_barcos;
    vector<int> tamanos_barcos;

public:
    Tablero() {
        for (int i = 0; i < FILAS; ++i)
            for (int j = 0; j < COLUMNAS; ++j)
                matriz[i][j] = 0;
    }

    void mostrar() const {
        cout << "   ";
        for (int j = 0; j < COLUMNAS; ++j) {
            cout << (char)('A' + j) << " ";
        }
        cout << "\n";

        for (int i = 0; i < FILAS; ++i) {
            cout << (i + 1);
            if (i < 9) {
                cout << "  ";
            } else {
                cout << " ";
            }

            for (int j = 0; j < COLUMNAS; ++j) {
                if (matriz[i][j] == 1) {
                    cout << "B ";
                } else if (matriz[i][j] == -1) {
                    cout << "X ";
                } else if (matriz[i][j] == -2) {
                    cout << ". ";
                } else {
                    cout << "/ ";
                }
            }
            cout << endl;
        }
    }

    bool colocar_barco(int x, int y, int tamano, char orientacion) {
        if (orientacion == 'v') {
            if (x + tamano > FILAS) return false;
        } else if (orientacion == 'h') {
            if (y + tamano > COLUMNAS) return false;
        } else {
            return false;
        }

        for (int i = 0; i < tamano; ++i) {
            int nuevaX = x;
            int nuevaY = y;
            if (orientacion == 'v') {
                nuevaX += i;
            } else if (orientacion == 'h') {
                nuevaY += i;
            }
            if (matriz[nuevaX][nuevaY] != 0) {
                return false;
            }
            matriz[nuevaX][nuevaY] = 1;
        }

        tamanos_barcos.push_back(tamano);
        golpes_barcos.push_back(0);
        return true;
    }

    bool golpeado(int x, int y) {
        if (matriz[x][y] == 1) {
            int tipo_barco = -1;
            for (int i = 0; i < tamanos_barcos.size(); ++i) {
                if (matriz[x][y] == 1) {
                    tipo_barco = i;
                    break;
                }
            }

            golpes_barcos[tipo_barco]++;
            matriz[x][y] = -1;

            if (golpes_barcos[tipo_barco] == tamanos_barcos[tipo_barco]) {
                cout << "¡El barco de tamaño " << tamanos_barcos[tipo_barco] << " se ha hundido!\n";
            }
            return true;

        } else if (matriz[x][y] == 0) {
            matriz[x][y] = -2;
        }
        return false;
    }

    bool barcos_hundidos() const {
        for (int i = 0; i < golpes_barcos.size(); ++i) {
            if (golpes_barcos[i] < tamanos_barcos[i]) {
                return false;
            }
        }
        return true;
    }
};

class Barcos {
public:
    void colocar_barcos(Tablero& tablero) {
        const int tamanos_barcos[] = {5, 4, 3, 2};
        const string nombres_barcos[] = {"Portaaviones", "Crucero", "Destructor", "Submarino"};

        for (int i = 0; i < 4; ++i) {
            int x, y;
            char orientacion;
            bool colocado = false;

            while (!colocado) {
                cout << "\nColocando " << nombres_barcos[i] << " (" << tamanos_barcos[i] << " celdas)\n";

                while (true) {
                    cout << "Ingresa la fila (1-10): ";
                    cin >> x;
                    if (x >= 1 and x <= 10) {
                        break;
                    }
                    cout << "Entrada inválida. Ingresa un número entre 1 y 10.\n";
                }
                while (true) {
                    cout << "Ingresa la columna (A-J): ";
                    char columna;
                    cin >> columna;
                    columna = toupper(columna);
                    y = columna - 'A';
                    if (y >= 0 and y < COLUMNAS) {
                        break;
                    } else {
                        cout << "Columna inválida. Ingresa una letra entre A y J.\n";
                    }
                }
                cout << "Ingresa la orientación (h para horizontal, v para vertical): ";
                cin >> orientacion;
                orientacion = tolower(orientacion);

                colocado = tablero.colocar_barco(x - 1, y, tamanos_barcos[i], orientacion);
                if (!colocado) {
                    cout << "Ubicación inválida. Vuelvalo a colocar.\n";
                }
                tablero.mostrar();
            }
        }
    }
};

class Jugador {
private:
    Tablero tablero;

public:
    void configurar_barcos(Barcos& barcos) {
        barcos.colocar_barcos(tablero);
    }

    bool dispara(int x, int y, Jugador& enemigo) {
        return enemigo.tablero.golpeado(x, y);
    }

    bool perdiste() const {
        return tablero.barcos_hundidos();
    }

    void mostrar_tablero() const {
        tablero.mostrar();
    }
};

class Juego {
private:
    Jugador jugador1, jugador2;
    bool turno_jugador1;

public:
    Juego() : turno_jugador1(true) {}

    void iniciar() {
        Barcos barcos;
        cout << "\nJugador 1 coloca sus barcos:";
        jugador1.configurar_barcos(barcos);
        cout << "\nJugador 2 coloca sus barcos:";
        jugador2.configurar_barcos(barcos);

        while (true) {
            if (turno_jugador1) {
                cout << "\nTurno del Jugador 1:\n";
            } else {
                cout << "\nTurno del Jugador 2:\n";
            }

            int x;
            char col;
            int y;

            while (true) {
                cout << "Ingresa la fila (1-10): ";
                cin >> x;
                if (x >= 1 and x <= 10) {
                    x -= 1;
                    break;
                } else {
                    cout << "Fila inválida. Debe ser un número entre 1 y 10.\n";
                }
            }

            while (true) {
                cout << "Ingresa la columna (A-J): ";
                cin >> col;
                col = toupper(col);
                y = col - 'A';
                if (y >= 0 and y < COLUMNAS) {
                    break;
                } else {
                    cout << "Columna inválida. Debe ser una letra entre A y J.\n";
                }
            }

            Jugador& atacante = turno_jugador1 ? jugador1 : jugador2;
            Jugador& objetivo = turno_jugador1 ? jugador2 : jugador1;

            if (atacante.dispara(x, y, objetivo)) {
                cout << "¡Impacto!\n";
            } else {
                cout << "¡Fallo!\n";
            }
            if (objetivo.perdiste()) {
                if (turno_jugador1) {
                    cout << "¡Jugador 1 gana!\n";
                } else {
                    cout << "¡Jugador 2 gana!\n";
                }
                break;
            }
            turno_jugador1 = !turno_jugador1;
        }
    }
};

int main() {
    cout << "¡Comienza el juego!\n";
    Tablero tablero;
    tablero.mostrar();
    Juego juego;
    juego.iniciar();
    return 0;
}
