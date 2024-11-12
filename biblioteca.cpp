#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <conio.h>
#include <iomanip>
#include <stdio.h>
#include <regex>
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
    int anio, nuevoAnio;
    string titulo, autor, anio_str, nuevoAnio_str, option;
    string nuevoAutor, nuevoTitulo;
    regex patron("[a-zA-Z]+");
    while (true)
    {
        do
        {
            system("clear");
            cout << "0. Salir" << endl;
            cout << "1. Agregar libros" << endl;
            cout << "2. Borrar libro" << endl;
            cout << "3. Modificar libro" << endl;
            cout << "4. Catalogo" << endl;

            cout << "Ingrese la opcion: ";
            getline(cin, option);
            if (option.empty())
            {
                cout << "Error: la opcion no puede estar vacía. Inténtelo de nuevo." << endl;
                sleep(2);
            }
        } while (option.empty());

        if (option == "0")
        {
            system("clear");
            break;
        }
        else if (option == "1")
        {
            system("clear");

            cin.ignore();
            do
            {
                cout << "Ingrese el título del libro: ";
                getline(cin, titulo);
                system("clear");
                if (titulo.empty())
                {
                    cout << "Error: el título no puede estar vacío. Inténtelo de nuevo." << endl;
                }
            } while (titulo.empty());

            regex patron_autor("^[A-Za-z ]+$");
            do
            {
                cout << "Ingrese el autor del libro (solo letras): ";
                getline(cin, autor);
                system("clear");
                if (!regex_match(autor, patron_autor))
                {
                    cout << "Error: el nombre del autor solo puede contener letras y espacios. No puede estar vacio." << endl;
                }
            } while (!regex_match(autor, patron_autor));

            regex patron_anio("^[0-9]+$");
            do
            {
                cout << "Ingrese el año de publicación del libro (solo números): ";
                cin >> anio_str;
                system("clear");
                if (!regex_match(anio_str, patron_anio))
                {
                    cout << "Error: el año de publicación solo puede contener números." << endl;
                }
            } while (!regex_match(anio_str, patron_anio));

            anio = stoi(anio_str);

            agregar_libro(titulo, autor, anio);
        }
        else if (option == "2")
        {
            system("clear");

            cin.ignore();
            do
            {
                cout << "Ingrese el título del libro: ";
                getline(cin, titulo);
                system("clear");
                if (titulo.empty())
                {
                    cout << "Error: el título no puede estar vacío. Inténtelo de nuevo." << endl;
                }
            } while (titulo.empty());

            regex patron_autor("^[A-Za-z ]+$");
            do
            {
                cout << "Ingrese el autor del libro (solo letras): ";
                getline(cin, autor);
                system("clear");
                if (!regex_match(autor, patron_autor))
                {
                    cout << "Error: el nombre del autor solo puede contener letras y espacios. No puede estar vacio." << endl;
                }
            } while (!regex_match(autor, patron_autor));

            regex patron_anio("^[0-9]+$");
            do
            {
                cout << "Ingrese el año de publicación del libro (solo números): ";
                cin >> anio_str;
                system("clear");
                if (!regex_match(anio_str, patron_anio))
                {
                    cout << "Error: el año de publicación solo puede contener números." << endl;
                }
            } while (!regex_match(anio_str, patron_anio));

            anio = stoi(anio_str);
            borrarLibro(titulo, autor, anio);
        }
        else if (option == "3")
        {
            regex patron_autor("^[A-Za-z ]+$"); // Solo permite letras y espacios
            regex patron_anio("^[0-9]+$");      // Solo permite números

            system("clear");
            cin.ignore();
            do
            {
                cout << "Ingrese el titulo del libro: " << endl;
                getline(cin, titulo);
                if (titulo.empty())
                {
                    cout << "Error: El título no puede estar vacío." << endl;
                }
            } while (titulo.empty());

            do
            {
                cout << "Ingrese el autor del libro: " << endl;
                getline(cin, autor);
                if (!regex_match(autor, patron_autor))
                {
                    cout << "Error: El autor solo puede contener letras y espacios." << endl;
                }
            } while (!regex_match(autor, patron_autor));

            do
            {
                cout << "Ingrese el anio de publicacion del libro: " << endl;
                cin >> anio_str;
                if (!regex_match(anio_str, patron_anio))
                {
                    cout << "Error: El año de publicación solo puede contener números." << endl;
                }
            } while (!regex_match(anio_str, patron_anio));
            anio = stoi(anio_str);

            system("clear");
            do
            {
                cout << "Ingrese el nuevo titulo del libro: " << endl;
                getline(cin, nuevoTitulo);
                if (nuevoTitulo.empty())
                {
                    cout << "Error: El nuevo título no puede estar vacío." << endl;
                }
            } while (nuevoTitulo.empty());

            do
            {
                cout << "Ingrese el nuevo autor del libro: " << endl;
                getline(cin, nuevoAutor);
                if (!regex_match(nuevoAutor, patron_autor))
                {
                    cout << "Error: El nuevo autor solo puede contener letras y espacios." << endl;
                }
            } while (!regex_match(nuevoAutor, patron_autor));

            do
            {
                cout << "Ingrese el nuevo anio de publicacion del libro: " << endl;
                cin >> nuevoAnio_str;
                if (!regex_match(nuevoAnio_str, patron_anio))
                {
                    cout << "Error: El nuevo año de publicación solo puede contener números." << endl;
                }
            } while (!regex_match(nuevoAnio_str, patron_anio));
            nuevoAnio = stoi(nuevoAnio_str);

            modificarLibro(titulo, autor, anio, nuevoTitulo, nuevoAutor, nuevoAnio);
            system("clear");
        }
        else if (option == "4")
        {
            verTodosLosLibros();
        }
        else
        {
            cout << "Entrada invalida, intetelo de nuevo" << endl;
        }
    }
}

void mostrar_menu_usuarios()
{
    string option;
    string nombre, CI;
    while (true)
    {
        do
        {
            system("clear");
            cout << "0. Salir" << endl;
            cout << "1. Agregar Usuarios " << endl;
            cout << "2. Mostrar Usuarios " << endl;
            cout << "3. Eliminar Usuarios " << endl;
            cout << "Ingrese la opcion: ";
            getline(cin, option);
            system("clear");
            if (option.empty())
            {
                cout << "Error: la opcion no puede estar vacía. Inténtelo de nuevo." << endl;
            }
        } while (option.empty());

        if (option == "0")
        {
            system("clear");
            break;
        }
        else if (option == "1")
        {
            regex patron_nombre("^[A-Za-z ]+$"); // Solo permite letras y espacios
            regex patron_CI("^[0-9]{8}$");       // Solo permite números

            system("clear");
            cin.ignore();
            do
            {
                cout << "Ingrese su nombre de usuario: ";
                getline(cin, nombre);
                if (nombre.empty())
                {
                    cout << "Error: El nombre no puede estar vacío." << endl;
                }
                else if (!regex_match(nombre, patron_nombre))
                {
                    cout << "Error: El nombre solo puede contener letras y espacios." << endl;
                }
            } while (nombre.empty() || !regex_match(nombre, patron_nombre));

            do
            {
                cout << "Ingrese la CI del Usuario: ";
                cin >> CI;
                if (CI.empty())
                {
                    cout << "Error: La CI no puede estar vacía." << endl;
                }
                else if (!regex_match(CI, patron_CI))
                {
                    cout << "Error: La CI solo puede contener números y ser de 8 cifras." << endl;
                }
            } while (CI.empty() || !regex_match(CI, patron_CI));

            agregar_usuario(nombre, CI);
            system("clear");
        }
        else if (option == "2")
        {
            verTodosLosUsuarios();
        }
        else if (option == "3")
        {
            regex patron_CI("^[0-9]{8}$");
            system("clear");
            do
            {
                cout << "Ingrese la CI del Usuario: ";
                cin >> CI;
                if (CI.empty())
                {
                    cout << "Error: La CI no puede estar vacía." << endl;
                }
                else if (!regex_match(CI, patron_CI))
                {
                    cout << "Error: La CI solo puede contener números y ser de 8 cifras." << endl;
                }
            } while (CI.empty() || !regex_match(CI, patron_CI));
            eliminar_usuario(CI);
            system("clear");
        }
        else
        {
            cout << "Entrada Invalida, intentelo de nuevo" << endl;
        }
    }
}

void mostrar_menu_prestamos()
{
    string CI, titulo, option;
    while (true)
    {
        system("clear");
        cout << "0. Salir" << endl;
        cout << "1. Agregar Prestamos " << endl;
        cout << "2. Mostrar Prestamos " << endl;
        cout << "3. Devolver Libro " << endl;

        do
        {
            cout << "Ingrese la opcion: ";
            getline(cin, option);
            system("clear");
            if (option.empty())
            {
                cout << "Error: la opcion no puede estar vacía. Inténtelo de nuevo." << endl;
            }
        } while (option.empty());

        if (option == "0")
        {
            system("clear");
            break;
        }
        else if (option == "1")
        {
            regex patron_CI("^[0-9]{8}$");
            system("clear");
            do
            {
                cout << "Ingrese la CI del Usuario: ";
                cin >> CI;
                if (CI.empty())
                {
                    cout << "Error: La CI no puede estar vacía." << endl;
                }
                else if (!regex_match(CI, patron_CI))
                {
                    cout << "Error: La CI solo puede contener números y ser de 8 cifras." << endl;
                }
            } while (CI.empty() || !regex_match(CI, patron_CI));
            cin.ignore();
            do
            {
                cout << "Ingrese el título del libro: ";
                getline(cin, titulo);
                system("clear");
                if (titulo.empty())
                {
                    cout << "Error: el título no puede estar vacío. Inténtelo de nuevo." << endl;
                }
            } while (titulo.empty());

            agregarPrestamo(CI, titulo);
        }
        else if (option == "2")
        {
            system("clear");
            mostrarPrestamos();
        }
        else if (option == "3")
        {
            regex patron_CI("^[0-9]{8}$");
            system("clear");
            do
            {
                cout << "Ingrese la CI del Usuario: ";
                cin >> CI;
                if (CI.empty())
                {
                    cout << "Error: La CI no puede estar vacía." << endl;
                }
                else if (!regex_match(CI, patron_CI))
                {
                    cout << "Error: La CI solo puede contener números y ser de 8 cifras." << endl;
                }
            } while (CI.empty() || !regex_match(CI, patron_CI));
            cin.ignore();
            do
            {
                cout << "Ingrese el título del libro: ";
                getline(cin, titulo);
                system("clear");
                if (titulo.empty())
                {
                    cout << "Error: el título no puede estar vacío. Inténtelo de nuevo." << endl;
                }
            } while (titulo.empty());

            devolverLibro(CI, titulo);
            system("clear");
        }
        else
        {
            cout << "Entrada invalida, intentelo de nuevo" << endl;
        }
    }
}

int main()
{
    crearDB();
    string username, password;

    string opcion;
    int ancho = 80; // Anchura de la consola
    std::string texto = "Bienvenido a la biblioteca.";
    std::cout << std::setw(ancho) << std::setfill(' ') << std::left << texto << std::endl;
    sleep(4);
    system("clear");

    while (true)
    {
        do
        {
            cout << "Ingrese el username: ";
            getline(cin, username);
            system("clear");
            if (username.empty())
            {
                cout << "Error: el username no puede estar vacío. Inténtelo de nuevo." << endl;
            }
        } while (username.empty());
        do
        {
            cout << "Ingrese la contrasena: ";
            getline(cin, password);
            system("clear");
            if (password.empty())
            {
                cout << "Error: la contrasena no puede estar vacío. Inténtelo de nuevo." << endl;
            }
        } while (password.empty());
        bool validacionAdmin = validarAdmin(username, password);
        if (validacionAdmin)
        {
            system("clear");
            cout << "Bienvenido Administrador" << endl;
            sleep(3);
            system("clear");
            break;
        }
        else
        {
            cout << "Usuario o contrasena incorrecto" << endl;
        }
    }

    while (true)
    {
        cout << "Menu Principal: " << endl;
        cout << " " << endl;
        cout << "0. Salir" << endl;
        cout << "1. Libros" << endl;
        cout << "2. Prestamos" << endl;
        cout << "3. Usuarios" << endl;
        cout << "4. Cambiar contrasena" << endl;

        do
        {
            cout << "Ingrese la opcion: ";
            getline(cin, opcion);
            system("clear");
            if (opcion.empty())
            {
                cout << "Error: la opcion no puede estar vacía. Inténtelo de nuevo." << endl;
            }
        } while (username.empty());

        if (opcion == "0")
        {
            system("clear");
            cout << "Gracias por usar la biblioteca." << endl;
            sleep(3);
            system("clear");
            cout << "Saliendo..." << endl;
            sleep(2);
            break;
        }
        else if (opcion == "1")
        {
            mostrar_menu_libros();
        }
        else if (opcion == "2")
        {
            mostrar_menu_prestamos();
        }
        else if (opcion == "3")
        {
            mostrar_menu_usuarios();
        }
        else if (opcion == "4")
        {
            while (true)
            {
                cout << "Ingrese el username: ";
                cin >> username;
                cout << "Ingrese la contrasena: ";
                cin >> password;
                system("clear");
                bool validacionAdmin = validarAdmin(username, password);
                if (validacionAdmin)
                {
                    system("clear");
                    cout << "Introduzca la nueva contrasena: ";
                    cin >> password;
                    cambiarContrasena(username, password);
                    system("clear");
                }
            }
        }
        else
        {
            cout << "Entrada invalida, intentelo de nuevo" << endl;
        }
    }

    return 0;
}
//Autores
//Bryto
//SaMH64
//Virtorx
//Alvaro Maker