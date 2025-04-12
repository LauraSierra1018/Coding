#include<iostream>

using namespace std;

template <typename T>
class Node{
    T dato;
    Node* next;
public:
    Node(){
        next = NULL;
    }
    
    Node(T d){
        dato = d;
        next = NULL;
    }
    
    ~Node(){
        //delete dato;
    }
    
    T getDato(){
        return dato;
    }
    
    void setDato(T d){
        dato = d;
    }
    
    Node* getNext(){
        return next;
    }
    
    void setNext(Node* n){
        next = n;
    }
};

template <typename T>
class Lista{
    Node<T>* ptr;
    int size;
public:
    Lista(){
        ptr = NULL;
        size = 0;
    }
    
    void add(T d){
        if(ptr == NULL){
            ptr = new Node<T>(d);
        }else{
            Node<T>* temp = ptr;
            while(temp->getNext() != NULL){
                temp = temp->getNext();
            }
            temp->setNext(new Node<T>(d));
        }
        size++;
    }
    
    Node<T>* get(int i){
        if(i >= 0 && i < size){
            int cont=0;
            Node<T>* temp = ptr;
            while(cont < i && temp != NULL){
                temp = temp->getNext();
                cont++;
            }
            return temp;
        }
        
        return NULL;
    }
    
    void print(){
        Node<T>* temp = ptr;
        while(temp != NULL){
            cout<<temp->getDato()<<endl;
            temp = temp->getNext();
        }
    }
    
    int get_size(){
        return size;
    }
    
    void remove(int i) {
    if (i < 0 || i >= size || ptr == nullptr) return; 

    Node<T>* toDelete = ptr;
    
    if (i == 0) { 
        ptr = ptr->getNext();
    } else {
        Node<T>* prev = get(i - 1);
        if (prev != nullptr) {
            toDelete = prev->getNext();
            prev->setNext(toDelete->getNext());
        }
    }

    delete toDelete;
    size--;
}

    
};

class MatrizPublicaciones{
    int** m; //Matriz de presentaciones
    int* v;  //Vector que lleva la cuenta de cuantos artículos hay en cada columna
    int f;   //Filas
    int c;   //Columnas
public:
    MatrizPublicaciones(){
        f = 10;
        c = 10;
        crear_mat();
    }
    
    MatrizPublicaciones(int fi, int co){
        f = fi;
        c = co;
        crear_mat();
    }
    
    void crear_mat(){
		m = new int* [f];
        for (int i = 0; i < f; i++) {
            m[i] = new int[c];
            for (int j = 0; j < c; j++) {
                m[i][j] = -1;
            }
        }
        
        v = new int[c];
		for (int i = 0; i < c; i++) {
			v[i] = 0;
		}
    }
    
    void print(){
		for (int i = 0; i < f; i++) {
			for (int j = 0; j < c; j++) {
				if (m[i][j] == -1) {
					cout << " - ";
				}
				else {
					cout << m[i][j] << "\t";
				}
			}
			cout << endl;
		}
    }
    
    void addValInColumna(int co, int dato) {
        for (int i = 0; i < f; i++) {
            for (int j = 0; j < c; j++) {
                if (m[i][j] == -1) {
                    m[i][j] = dato;
                    return;
                }
            }

        }

        c++;
        for (int i = 0; i < f; i++) {
            int* n = new int[c];
            for (int j = 0; j < c - 1; j++) {
                n[j] = m[i][j];
            }
            n[c - 1] = -1;
            delete[] m[i];
            m[i] = n;
        }
        if (co == f) {
            add_fila();
        }
    }
    
    void add_fila(){
		int** n = new int* [f + 1];
        for (int i = 0; i < f; i++) {
            n[i] = m[i];
        }
		n[f] = new int[c];
		for (int j = 0; j < c; j++) {
			n[f][j] = -1;
		}
		delete[] m;
		m = n;
		f++;
    }  
};

class Articulo{
    int id;
    Lista<int> autores;
    int area_conocimiento;
    float evaluacion;

public:
    Articulo(){
        id = -1;
        area_conocimiento = -1;
        evaluacion = 0;
    }
    
    Articulo(int idp, int ac, float e){
		id = idp;
		area_conocimiento = ac;
		evaluacion = e;
    }
    
    void add_autor(int ida){
      autores.add(ida);
    }
    
    int getID(){
        return id;
    }
    
    float get_evaluacion(){
        return evaluacion;
    }
    
    int get_area_conocimiento(){
        return area_conocimiento;
    }
    
    string to_string(){
        string s = std::to_string(id)+" "+std::to_string(area_conocimiento)+" "+std::to_string(evaluacion) +" (";
        int n = autores.get_size();
        for(int i=0; i<n; i++){
            s = s + std::to_string(autores.get(i)->getDato())+",";
        }
        s =  s+")";
        return s;
    }
    
    friend std::ostream& operator<<(std::ostream& os, Articulo& b) {
		return os << b.to_string();
	}
	
	friend std::ostream& operator<<(std::ostream& os, Articulo* b) {
		return os << b->to_string();
	}
	
	friend std::ostream& operator<<(std::ostream& os, Articulo b) {
		return os << b.to_string();
	}
	

};

class LaConference{
    Lista<Articulo*> articulos;
    MatrizPublicaciones matriz;
    int na;
    int nc;
    float min_eval;
    
public:
    LaConference(){
        na = 20;
        nc = 5;
        matriz = MatrizPublicaciones(3,nc);
        min_eval = 2.0f;
        generar_articulos(10);
        print_articulos();
        filtrar_articulos();
        print_articulos();
        generar_matriz();
    }
    
    LaConference(int n, int naa, int ncc, float min){
        na = naa;
        nc = ncc;
        min_eval = min;
        matriz = MatrizPublicaciones(3,nc);
        generar_articulos(n);
        print_articulos();
        filtrar_articulos();
        print_articulos();
        generar_matriz();
    }
    
    void generar_articulos(int n){
		for (int i = 0; i < n; i++) {
			float ev = (static_cast <float>(rand()) / static_cast <float>(RAND_MAX)) * 5.0;
			Articulo* a = new Articulo(i, rand() % nc, ev);
			int na = rand() % 10;
			for (int j = 0; j < na; j++) {
				a->add_autor(rand() % 100);
			}
			articulos.add(a);
		}
    }
    
    void filtrar_articulos() {
      for (int i = 0; i < articulos.get_size(); i++) {
        if (articulos.get(i)->getDato()->get_evaluacion() < min_eval) {
            articulos.remove(i);
            i--; 
        }
    }
  }
    
    void print_articulos(){
        articulos.print();
    }
    
    void generar_matriz(){
        for (int i = 0; i < articulos.get_size(); i++) {
            Articulo* a = articulos.get(i)->getDato();
            if (a != nullptr) { 
                int area = a->get_area_conocimiento();
                int id = a->getID();
                matriz.addValInColumna(area, id);  
            }
        }

        cout<<"La matriz de presentación de artículos es:"<<endl;
        matriz.print();
        
    }
    
    
    
};


int main()
{

    srand(time(0));
    
    
    //Prueba de Matriz de publicaciones
    //El resultado será una matriz con 9 elementos en la columna 2
    //para lo cual tuvo que haber añadido 5 filas más en el proceso

    MatrizPublicaciones m(5,5);
    
    m.print();
    cout<<endl;
    for(int i=0; i<10; i++){
        m.addValInColumna(2,i);
    }
    
    m.print();
    
    
    LaConference c(30, 5, 5, 2.0);
    
    return 0;
}