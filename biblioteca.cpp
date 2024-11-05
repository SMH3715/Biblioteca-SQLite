#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <conio.h>
#include <iomanip>
#include <stdio.h>
// #include <sstream>   // Para convertir cadenas a números
#include <sqlite3.h> //Libreria para Data Base
#include "DB-Config.h"
// #include "Libros.h"

using namespace std;

class Libro
{
public:
    Libro(const std::string &titulo, const std::string &autor, int anio_publicacion)
        : titulo_(titulo), autor_(autor), anio_publicacion_(anio_publicacion), disponible_(true) {}

    std::string getTitulo() const { return titulo_; }
    std::string getAutor() const { return autor_; }
    int getanioPublicacion() const { return anio_publicacion_; }
    bool estaDisponible() const { return disponible_; }
    void prestar() { disponible_ = false; }
    void devolver() { disponible_ = true; }

    // Función para establecer la disponibilidad
    void setDisponible(bool disponible) { disponible_ = disponible; }

private:
    std::string titulo_;
    std::string autor_;
    int anio_publicacion_;
    bool disponible_;
};

void mostrar_menu_libros()
{
    int option, anio, nuevoAnio;
    string titulo, autor;
    string nuevoAutor, nuevoTitulo;

    while (true)
    {
        system("clear");
        cout << "0. Salir" << endl;
        cout << "1. Agregar libros" << endl;
        cout << "2. Borrar libro" << endl;
        cout << "3. Modificar libro" << endl;
        cout << "4. Catalogo" << endl;
        cout << "Elija una opcion: ";

        cin >> option;

        if (option == 0)
        {
            system("clear");
            break;
        }
        else if (option == 1)
        {
            system("clear");
            cout << "Ingrese el titulo del libro: " << endl;
            cin >> titulo;
            cout << "Ingrese el autor del libro: " << endl;
            cin >> autor;
            cout << "Ingrese el anio de publicacion del libro: " << endl;
            cin >> anio;

            agregar_libro(titulo, autor, anio);
        }
        else if (option == 2)
        {
            system("clear");
            cout << "Ingrese el titulo del libro: " << endl;
            cin >> titulo;
            cout << "Ingrese el autor del libro: " << endl;
            cin >> autor;
            cout << "Ingrese el anio de publicacion del libro: " << endl;
            cin >> anio;
            borrarLibro(titulo, autor, anio);
        }
        else if (option == 3)
        {
            system("clear");
            cout << "Ingrese el titulo del libro: " << endl;
            cin >> titulo;
            cout << "Ingrese el autor del libro: " << endl;
            cin >> autor;
            cout << "Ingrese el anio de publicacion del libro: " << endl;
            cin >> anio;
            system("clear");
            cout << "Ingrese el nuevo titulo del libro: " << endl;
            cin >> nuevoTitulo;
            cout << "Ingrese el nuevo autor del libro: " << endl;
            cin >> nuevoAutor;
            cout << "Ingrese el nuevo anio de publicacion del libro: " << endl;
            cin >> nuevoAnio;
            modificarLibro(titulo, autor, anio, nuevoTitulo, nuevoAutor, nuevoAnio);
            system("clear");
        }
        else if (option == 4)
        {
            verTodosLosLibros();
        }
    }
}

void mostrar_menu_usuarios()
{

    int option;
    string nombre, CI;
    while (true)
    {
        system("clear");
        cout << "0. Salir" << endl;
        cout << "1. Agregar Usuarios " << endl;
        cout << "2. Mostrar Usuarios " << endl;
        cout << "3. Eliminar Usuarios " << endl;
        cout << "Elija una opcion: ";

        cin >> option;

        if (option == 0)
        {
            system("clear");
            break;
        }
        else if (option == 1)
        {
            system("clear");
            cout << " Ingrese su nombre de usuario " << endl;
            cin >> nombre;
            cout << "Ingrese la CI del Usuario " << endl;
            cin >> CI;
            agregar_usuario(nombre, CI);
            system("clear");
        }
        else if (option == 2)
        {
            verTodosLosUsuarios();
        }
        else if (option == 3)
        {
            system("clear");
            cout << "Ingrese la CI del Usuario " << endl;
            cin >> CI;
            eliminar_usuario(CI);
            system("clear");
        }
    }
}

void mostrar_menu_prestamos()
{
    string CI, titulo;

    int option;
    while (true)
    {
        system("clear");
        cout << "0. Salir" << endl;
        cout << "1. Agregar Prestamos " << endl;
        cout << "2. Mostrar Prestamos " << endl;
        cout << "3. Devolver Libro " << endl;
        cout << "Elija una opcion: ";

        cin >> option;

        if (option == 0)
        {
            system("clear");
            break;
        }
        else if (option == 1)
        {
            system("clear");
            agregarPrestamo();
        }
        else if (option == 2)
        {
            system("clear");
            mostrarPrestamos();
        }
        else if (option == 3)
        {
            system("clear");
            cout << "Ingrese la CI del Usuario " << endl;
            cin >> CI;
            cout << "Ingrese el titulo del libro: " << endl;
            cin >> titulo;

            devolverLibro(CI, titulo);
            system("clear");
        }
    }
}

int main()
{
    crearDB();

    int opcion;
    int ancho = 80; // Anchura de la consola
    std::string texto = "Bienvenido a la biblioteca.";
    std::cout << std::setw(ancho) << std::setfill(' ') << std::left << texto << std::endl;
    sleep(4);
    system("clear");
    while (true)
    {
        cout << "Menu Principal: " << endl;
        cout << " " << endl;
        cout << "0. Salir" << endl;
        cout << "1. Libros" << endl;
        cout << "2. Prestamos" << endl;
        cout << "3. Usuarios" << endl;
        cout << "Elija una opcion: ";
        cin >> opcion;

        if (opcion == 0)
        {
            system("clear");
            cout << "Gracias por usar la biblioteca." << endl;
            sleep(3);
            system("clear");
            cout << "Saliendo..." << endl;
            sleep(2);
            break;
        }
        else if (opcion == 1)
        {
            mostrar_menu_libros();
        }
        else if (opcion == 2)
        {
            mostrar_menu_prestamos();
        }
        else if (opcion == 3)
        {
            mostrar_menu_usuarios();
        }
    }

    /* agregar_usuario("Samir", "31312232");
    agregar_libro("El Principito", "Antoine de Saint-Exupéry", 1949);
    agregar_prestamo("31312232", "El Principito", "Antoine de Saint-Exupéry"); */

    return 0;
}
