#include <iostream>
#include <string>
#include "Lista.h"
#include "Vector.h"

using namespace std;

// Estructura para representar un libro
class Libro {
public:
    int id;
    string titulo;
    int idAutor;
    int idGenero;
    int numPaginas;
    double valor;

    Libro() {
        id = 0;
        titulo = "";
        idAutor = 0;
        idGenero = 0;
        numPaginas = 0;
        valor = 0.0;
    }

    Libro(int id, string titulo, int idAutor, int idGenero, int numPaginas, double valorVenta) {
        id = id;
        titulo = titulo;
        idAutor = idAutor;
        idGenero = idGenero;
        numPaginas = numPaginas;
        valor = valor;
    }

    int getAutor() {
        return idAutor;
    }

    string getTitulo() {
        return titulo;
    }

    double getValor() {
        return valor;
    }

    string to_string() {
        return std::to_string(id) + " " + titulo + " " + std::to_string(idAutor) + " " + std::to_string(valor);
    }
};

// Clase Autor
class Autor {
    int id;
    string nombre;
    int edad;
    LinkedList<Libro> libros;

public:
    Autor() : id(-1), nombre(""), edad(-1) {}
    Autor(int id, string nombre, int edad) : id(id), nombre(nombre), edad(edad) {}

    int getId() {
        return id;
    }

    void addLibro(Libro l) {
        libros.add(l);
    }

    string getNombre() {
        return nombre;
    }

    int getNumLibros() {
        return libros.getSize();
    }

    string to_string() {
        return std::to_string(id) + " " + nombre + " " + std::to_string(edad) + " años, " + std::to_string(libros.getSize()) + " libros";
    }
};

class ItemInventario {
    int idlibro;
    int cantidad;

public:
    ItemInventario() : idlibro(-1), cantidad(-1) {}
    ItemInventario(int idlibro, int cantidad) : idlibro(idlibro), cantidad(cantidad) {}

    int getIdLibro() {
        return idlibro;
    }

    int getCantidad() {
        return cantidad;
    }

    string to_string() {
        return "(" + std::to_string(idlibro) + "," + std::to_string(cantidad) + ")";
    }

    friend std::ostream& operator<<(std::ostream& os, ItemInventario& obj) {
        return os << obj.to_string();
    }
};

// Clase Libreria
class Libreria {
public:
    int id;
    string nombre;
    LinkedList<ItemInventario> inventario; // (idLibro, numCopias)

    Libreria() : id(-1), nombre("") {}
    Libreria(int id, string nombre) : id(id), nombre(nombre) {}

    string getNombre() {
        return nombre;
    }

    void addLibro(int idLibro, int copias) {
        inventario.add(ItemInventario(idLibro, copias));
    }

    void printInventario() {
        inventario.print();
    }

    ItemInventario getInventario(int i) {
        return inventario.get(i);
    }

    int getInventarioSize() {
        return inventario.getSize();
    }

    int getTotalLibros() {
        int tot = 0;
        for (int i = 0; i < inventario.getSize(); i++) {
            tot += inventario.get(i).getCantidad();
        }
        return tot;
    }

    string to_string() {
        return std::to_string(id) + " " + nombre;
    }
};

// Clase Editorial
class Editorial {
    Vector<Autor> autores;
    Vector<Libro> libros;
    Vector<Libreria> librerias;

public:
    Editorial() {}

    Vector<Autor>& getAutores() {
        return autores;
    }

    Vector<Libreria>& getLibrerias() {
        return librerias;
    }

    Vector<Libro>& getLibros() {
        return libros;
    }

    void addAutor(int id, string nombre, int edad) {
        autores.add(Autor(id, nombre, edad));
    }

    void addLibro(int id, string titulo, int idAutor, int idGenero, int numPaginas, double valorVenta) {
        for (int i = 0; i < autores.getSize(); i++) {
            if (autores[i].getId() == idAutor) {
                Libro nuevoLibro(id, titulo, idAutor, idGenero, numPaginas, valorVenta);
                libros.add(nuevoLibro);
                autores[i].addLibro(nuevoLibro);
                return;
            }
        }
        cout << "Error: Autor no encontrado." << endl;
    }

    void addLibroInventario(int libreria, int idLibro, int cant) {
        if (libreria >= 0 && libreria < librerias.getSize()) {
            librerias[libreria].addLibro(idLibro, cant);
        }
    }

    void addLibreria(int id, string nombre) {
        librerias.add(Libreria(id, nombre));
    }

    void printLibrerias() {
        for (int i = 0; i < librerias.getSize(); i++) {
            cout << "Librería: " << librerias[i].nombre << endl;
            librerias[i].printInventario();
            cout << endl;
        }
    }

    string printLibros(){
        string s = "";
		for (int i = 0; i < libros.getSize(); i++) {
			s = s + libros[i].to_string() + " -> ";
		}
        return s;
    }
    

    void autorMasLibrosInventario() {
        for (int i = 0; i < autores.getSize(); i++) {
            Vector<int> invent;
            for (int j = 0; j < librerias.getSize(); j++) {
                for (int k = 0; k < librerias[j].getInventarioSize(); k++) {
                    if (libros[librerias[j].getInventario(k).getIdLibro()].getAutor() == autores[i].getId()) {
                        invent.add(librerias[j].getInventario(k).getCantidad());
                    }
                }
            }

            int mayAut = 0;
            int mayInv = invent[0];
            for (int j = 1; j < invent.getSize(); j++) {
                if (invent[j] > mayInv) {
                    mayInv = invent[j];
                    mayAut = j;
                }
            }
            cout << "El autor con mayor inventario de libros es: " + autores[mayAut].getNombre() << endl;
        }
    }

    void libreriaMasInventario() {
        for (int i = 0; i < librerias.getSize(); i++) {
            int total = 0;
            for (int j = 0; j < librerias[i].getInventarioSize(); j++) {
                total += librerias[i].getInventario(j).getCantidad();
            }
        }
    }

    void libreriaMasValorInventario() {
        for (int i = 0; i < librerias.getSize(); i++) {
            double* v = new double[libros.getSize()];
            for (int j = 0; j < libros.getSize(); j++) {
                v[j] = 0;
            }
            for (int j = 0; j < librerias[i].getInventarioSize(); j++) {
                int idLibro = librerias[i].getInventario(j).getIdLibro();
                double valor = libros[idLibro].getValor();
                v[i] += valor;
            }
        }
    }
    
    void autorMasTitulos(){
		for (int i = 0; i < autores.getSize(); i++) {
			int total = 0;
			for (int j = 0; j < autores[i].getNumLibros(); j++) {
				total++;
			}
		}
    }
};

int main() {
    srand(1234);
    Editorial editorial;

    // Nombres de libros
    string titulos[23] = { "Red Sorghum", "The Three-Body Problem", "Wild Grass", "Legends of the Condor Heroes", "Fortress Besieged", "Moonlight Mirage", "Quantum Sands", "Silent Echoes", "Whispering Bamboo", "The Infinite Bridge", "Dragon's Curse", "Nebula Chronicles", "Tales of the Fallen Empire", "Celestial Labyrinth", "The Golden Era", "Shadow Puppets", "The Fractal Mind", "Phantom Blossoms", "Sword of Destiny", "Echoes of the Past", "Parallel Horizons", "Singularity Rift", "Eternal Lanterns" };

    // Agregar autores
    editorial.addAutor(1, "Mo Yan", 68);
    editorial.addAutor(2, "Liu Cixin", 60);
    editorial.addAutor(3, "Lu Xun", 56);
    editorial.addAutor(4, "Jin Yong", 72);
    editorial.addAutor(5, "Qian Zhongshu", 85);

    // Agregar libros: id, título, autor, género, número de páginas, valor
    for (int i = 1; i <= 23; i++) {
        editorial.addLibro(i, titulos[i - 1], (rand() % 5) + 1, (rand() % 6) + 1, rand() % 200 + 1, (rand() % 100000) + 1);
    }

    editorial.addLibreria(1, "Librería Central");
    editorial.addLibreria(2, "Casa del Libro");
    editorial.addLibreria(3, "Lectura China");

    // Agregar libros a librerías: librería, id del libro, cantidad
    for (int i = 1; i <= 23; i++) {
        editorial.addLibroInventario(rand() % 3, i, (rand() % 5) + 1);
    }

    // Mostrar lista de autores
    cout << editorial.getAutores() << endl;

    // Mostrar lista de libros
    cout << editorial.printLibros() << endl;

    // Mostrar inventario de librerías
    editorial.printLibrerias();

    editorial.autorMasTitulos();

    editorial.autorMasLibrosInventario();

    editorial.libreriaMasInventario();

    editorial.libreriaMasValorInventario();

    return 0;
}