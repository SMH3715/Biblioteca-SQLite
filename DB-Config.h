#include <iostream>
#include <sqlite3.h>
#include <unistd.h>
#include <fstream>
#include <conio.h>

using namespace std;
/*Creación de la base de datos*/
sqlite3 *DB;

bool existeArchivoDB(const string &nombreArchivo)
{
    ifstream archivo(nombreArchivo);
    return archivo.good();
}

void crearDB()
{
    system("clear");
    cout << "Cargando..." << endl;
    sleep(2);
    system("clear");
    string file = "BSUHMD.db";

    if (existeArchivoDB(file))
    {
        cout << "Hay una base de datos existente" << endl;
        sleep(2);
        system("clear");
        cout << "Cargando..." << endl;
        sleep(1);
        system("clear");
    }
    else
    {
        cout << "No existe una base de datos" << endl;
        sleep(2);
        system("clear");
        cout << "Cargando..." << endl;
        sleep(1);
        system("clear");
        cout << "Se creara una nueva base de datos" << endl;
        sleep(2);
        system("clear");
        int exist = sqlite3_open(file.c_str(), &DB);

        if (exist != SQLITE_OK)
        {
            cout << "Error al abrir la base de datos: " << endl;
        }

        string sql = R"(
        CREATE TABLE IF NOT EXISTS usuarios (
            id_usuario INTEGER PRIMARY KEY AUTOINCREMENT,
            nombre TEXT NOT NULL,
            CI TEXT UNIQUE NOT NULL
        );
        CREATE TABLE IF NOT EXISTS libros (
            id_libro INTEGER PRIMARY KEY AUTOINCREMENT,
            titulo TEXT NOT NULL,
            autor TEXT NOT NULL,
            anio INTEGER NOT NULL,
            disponible INTEGER DEFAULT 1
        );
        CREATE TABLE IF NOT EXISTS prestamos (
            id_prestamo INTEGER PRIMARY KEY AUTOINCREMENT,
            id_usuario INTEGER NOT NULL,
            id_libro INTEGER NOT NULL,
            fecha_prestamo TEXT NOT NULL,
            fecha_devolucion TEXT,
            FOREIGN KEY (id_usuario) REFERENCES usuarios (id_usuario),
            FOREIGN KEY (id_libro) REFERENCES libros (id_libro)
        );



    )";

        exist = sqlite3_exec(DB, sql.c_str(), NULL, 0, 0);

        if (exist != SQLITE_OK)
        {
            cout << "Error: no se pudo crear la tabla: " << endl;
        }
        else
        {
            cout << "La base de datos se ha creado exitosamente" << endl;
            sleep(3);
            system("clear");
        }
        sqlite3_close(DB);
    }
}
/*A partir de aquí, inician las funciones del programa.*/
void agregar_usuario(const string &nombre, const string &CI)
{
    sqlite3_open("BSUHMD.db", &DB);
    string sql = "INSERT INTO usuarios (nombre, CI) VALUES (?, ?)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (rc == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, CI.c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);

        if (rc == SQLITE_DONE)
        {
            cout << "Usuario agregado exitosamente" << endl;
        }
        else
        {
            cout << "Error al agregar el usuario: " << sqlite3_errmsg(DB) << endl;
        }
    }
    else
    {
        cout << "Error al preparar la consulta: " << sqlite3_errmsg(DB) << endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void agregar_libro(const string &titulo, const string &autor, int anio)
{
    sqlite3_open("BSUHMD.db", &DB);
    string sql = "INSERT INTO libros (titulo, autor, anio) VALUES (?, ?, ?)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (rc == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, titulo.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, autor.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, anio);

        rc = sqlite3_step(stmt);

        if (rc == SQLITE_DONE)
        {
            cout << "Libro agregado exitosamente" << endl;
            sleep(2);
        }
        else
        {
            cout << "Error al agregar el libro: " << sqlite3_errmsg(DB) << endl;
            sleep(2);
        }
    }
    else
    {
        cout << "Error al preparar la consulta: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void agregar_prestamo(string CI, string titulo, string autor)
{
    int id_usuario, id_libro;

    sqlite3_open("BSUHMD.db", &DB);
    // Buscar el ID del usuario mediante la CI
    string sql_usuario = "SELECT id_usuario FROM usuarios WHERE CI = ?";
    sqlite3_stmt *stmt_usuario;
    int rc = sqlite3_prepare_v2(DB, sql_usuario.c_str(), -1, &stmt_usuario, NULL);

    if (rc == SQLITE_OK)
    {
        sqlite3_bind_text(stmt_usuario, 1, CI.c_str(), -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt_usuario);

        if (rc == SQLITE_ROW)
        {
            id_usuario = sqlite3_column_int(stmt_usuario, 0);
        }
        else
        {
            cout << "Usuario no encontrado con la CI proporcionada." << endl;
            sleep(2);
            sqlite3_finalize(stmt_usuario);
            return;
        }
    }
    else
    {
        cout << "Error al preparar la consulta para el usuario: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
        sqlite3_finalize(stmt_usuario);
        return;
    }
    sqlite3_finalize(stmt_usuario);

    // Buscar el ID del libro mediante el título y el autor
    string sql_libro = "SELECT id_libro, disponible FROM libros WHERE titulo = ? AND autor = ?";
    sqlite3_stmt *stmt_libro;
    rc = sqlite3_prepare_v2(DB, sql_libro.c_str(), -1, &stmt_libro, NULL);

    if (rc == SQLITE_OK)
    {
        sqlite3_bind_text(stmt_libro, 1, titulo.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt_libro, 2, autor.c_str(), -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt_libro);

        if (rc == SQLITE_ROW)
        {
            id_libro = sqlite3_column_int(stmt_libro, 0);
            int disponible = sqlite3_column_int(stmt_libro, 1);
            if (disponible == 0)
            {
                cout << "El libro no está disponible para préstamo." << endl;
                sleep(2);
                sqlite3_finalize(stmt_libro);
                return;
            }
        }
        else
        {
            cout << "Libro no encontrado con el título y autor proporcionados." << endl;
            sleep(2);
            sqlite3_finalize(stmt_libro);
            return;
        }
    }
    else
    {
        cout << "Error al preparar la consulta para el libro: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
        sqlite3_finalize(stmt_libro);
        return;
    }
    sqlite3_finalize(stmt_libro);

    // Insertar el préstamo en la tabla de préstamos
    string sql_prestamo = "INSERT INTO prestamos (id_usuario, id_libro, fecha_prestamo, fecha_devolucion) VALUES (?, ?, ?, NULL)";
    sqlite3_stmt *stmt_prestamo;
    rc = sqlite3_prepare_v2(DB, sql_prestamo.c_str(), -1, &stmt_prestamo, NULL);

    if (rc == SQLITE_OK)
    {
        sqlite3_bind_int(stmt_prestamo, 1, id_usuario);
        sqlite3_bind_int(stmt_prestamo, 2, id_libro);

        // Obtener la fecha actual para la fecha de préstamo
        time_t t = time(0);
        struct tm *now = localtime(&t);
        char fecha_prestamo[11];
        strftime(fecha_prestamo, 11, "%Y-%m-%d", now);

        sqlite3_bind_text(stmt_prestamo, 3, fecha_prestamo, -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt_prestamo);

        if (rc == SQLITE_DONE)
        {
            cout << "Prestamo agregado exitosamente." << endl;

            // Marcar el libro como no disponible
            string update_disponibilidad = "UPDATE libros SET disponible = 0 WHERE id_libro = ?";
            sqlite3_stmt *update_stmt;
            rc = sqlite3_prepare_v2(DB, update_disponibilidad.c_str(), -1, &update_stmt, NULL);

            if (rc == SQLITE_OK)
            {
                sqlite3_bind_int(update_stmt, 1, id_libro);
                rc = sqlite3_step(update_stmt);

                if (rc != SQLITE_DONE)
                {
                    cout << "Error al actualizar la disponibilidad del libro: " << sqlite3_errmsg(DB) << endl;
                    sleep(2);
                }
            }
            sqlite3_finalize(update_stmt);
        }
        else
        {
            cout << "Error al agregar el prestamo: " << sqlite3_errmsg(DB) << endl;
            sleep(2);
        }
    }
    else
    {
        cout << "Error al preparar la consulta de prestamo: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
    }

    sqlite3_finalize(stmt_prestamo);

    sqlite3_close(DB);
}

void borrarLibro(const string &titulo, const string &autor, int anio)
{
    sqlite3_open("BSUHMD.db", &DB);
    string sql = "DELETE FROM libros WHERE titulo = ? AND autor = ? AND anio = ?";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (rc == SQLITE_OK)
    {
        // Asociar los parámetros a la consulta
        sqlite3_bind_text(stmt, 1, titulo.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, autor.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, anio);

        rc = sqlite3_step(stmt);

        if (rc == SQLITE_DONE)
        {
            cout << "Libro borrado exitosamente." << endl;
            sleep(2);
        }
        else
        {
            cout << "Error al borrar el libro: " << sqlite3_errmsg(DB) << endl;
            sleep(2);
        }
    }
    else
    {
        cout << "Error al preparar la consulta: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void modificarLibro(const string &titulo, const string &autor, int anio, const string &nuevoTitulo, const string &nuevoAutor, int nuevoAnio, int disponible = 1)
{
    sqlite3_open("BSUHMD.db", &DB);
    string sql = R"(
        UPDATE libros 
        SET titulo = ?, autor = ?, anio = ?, disponible = ? 
        WHERE titulo = ? AND autor = ? AND anio = ?
    )";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (rc == SQLITE_OK)
    {
        // Asociar los parámetros de actualización
        sqlite3_bind_text(stmt, 1, nuevoTitulo.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, nuevoAutor.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, nuevoAnio);
        sqlite3_bind_int(stmt, 4, disponible);

        // Asociar los parámetros de identificación del libro
        sqlite3_bind_text(stmt, 5, titulo.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, autor.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 7, anio);

        rc = sqlite3_step(stmt);

        if (rc == SQLITE_DONE)
        {
            cout << "Libro modificado exitosamente." << endl;
            sleep(2);
        }
        else
        {
            cout << "Error al modificar el libro: " << sqlite3_errmsg(DB) << endl;
            sleep(2);
        }
    }
    else
    {
        cout << "Error al preparar la consulta: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void verTodosLosLibros()
{
    sqlite3_open("BSUHMD.db", &DB);
    const char *sql = "SELECT id_libro, titulo, autor, anio FROM libros;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        cout << "Error al preparar la consulta: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
        return;
    }

    cout << "Lista de libros:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id_libro = sqlite3_column_int(stmt, 0);
        const char *titulo = (const char *)sqlite3_column_text(stmt, 1);
        const char *autor = (const char *)sqlite3_column_text(stmt, 2);
        int anio = sqlite3_column_int(stmt, 3);

        // Imprime la información del libro
        cout << "ID: " << id_libro << ", Título: " << titulo << ", Autor: " << autor << ", Año: " << anio << endl;
    }
    sleep(10);
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

// Usuarios

void verTodosLosUsuarios()
{
    sqlite3_open("BSUHMD.db", &DB);
    const char *sql = "SELECT id_usuario, nombre, CI FROM usuarios;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        cout << "Error al preparar la consulta: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
        return;
    }

    cout << "Lista de usuarios:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id_usuario = sqlite3_column_int(stmt, 0);
        const char *nombre = (const char *)sqlite3_column_text(stmt, 1);
        const char *CI = (const char *)sqlite3_column_text(stmt, 2);

        // Imprime la información del usuario
        cout << "ID: " << id_usuario << ", Nombre: " << nombre << ", C.I.: " << CI << endl;
    }

    sleep(10);
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void eliminar_usuario(string CI)
{
    sqlite3_open("BSUHMD.db", &DB);
    string sql = "DELETE FROM usuarios WHERE CI = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

    if (rc == SQLITE_OK)
    {
        // Asociar los parámetros a la consulta
        sqlite3_bind_text(stmt, 1, CI.c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);

        if (rc == SQLITE_DONE)
        {
            cout << "Usuario eliminado exitosamente." << endl;
            sleep(2);
        }
        else
        {
            cout << "Error al eliminar el usuario: " << sqlite3_errmsg(DB) << endl;
            sleep(2);
        }
    }
    else
    {
        cout << "Error al preparar la consulta: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

// Función de prestamos

void agregarPrestamo()
{
    sqlite3_open("BSUHMD.db", &DB);
    string CI, titulo;

    // Solicitar la cédula del usuario y el título del libro
    cout << "Ingrese la CI del usuario: ";
    cin >> CI;
    system("clear");
    cout << "Ingrese el título del libro: ";
    cin.ignore(); // Limpiar el buffer
    getline(cin, titulo);
    system("clear");
    // Obtener el id_usuario a partir de la cédula
    string sqlUsuario = "SELECT id_usuario FROM usuarios WHERE CI = ?;";
    sqlite3_stmt *stmtUsuario;
    int id_usuario = -1;

    // Preparar la consulta para el usuario
    if (sqlite3_prepare_v2(DB, sqlUsuario.c_str(), -1, &stmtUsuario, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmtUsuario, 1, CI.c_str(), -1, SQLITE_STATIC);

        // Ejecutar la consulta
        if (sqlite3_step(stmtUsuario) == SQLITE_ROW)
        {
            id_usuario = sqlite3_column_int(stmtUsuario, 0);
        }
        else
        {
            cout << "Error: Usuario no encontrado." << endl;
            sleep(2);
            sqlite3_finalize(stmtUsuario);
            return;
        }
    }
    else
    {
        cout << "Error al preparar la consulta del usuario: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
        return;
    }
    sqlite3_finalize(stmtUsuario);

    // Obtener el id_libro a partir del título
    string sqlLibro = "SELECT id_libro, disponible FROM libros WHERE titulo = ?;";
    sqlite3_stmt *stmtLibro;
    int id_libro = -1;
    int disponible = 0;

    // Preparar la consulta para el libro
    if (sqlite3_prepare_v2(DB, sqlLibro.c_str(), -1, &stmtLibro, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmtLibro, 1, titulo.c_str(), -1, SQLITE_STATIC);

        // Ejecutar la consulta
        if (sqlite3_step(stmtLibro) == SQLITE_ROW)
        {
            id_libro = sqlite3_column_int(stmtLibro, 0);
            disponible = sqlite3_column_int(stmtLibro, 1);
        }
        else
        {
            cout << "Error: Libro no encontrado." << endl;
            sleep(2);
            sqlite3_finalize(stmtLibro);
            return;
        }
    }
    else
    {
        cout << "Error al preparar la consulta del libro: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
        return;
    }
    sqlite3_finalize(stmtLibro);

    // Verificar si el libro está disponible
    if (disponible == 0)
    {
        cout << "Error: El libro no está disponible." << endl;
        sleep(2);
        return;
    }

    // Obtener la fecha actual
    time_t now = time(0);
    char *fecha_prestamo = ctime(&now);
    fecha_prestamo[strlen(fecha_prestamo) - 1] = '\0'; // Eliminar el salto de línea

    // Insertar el préstamo en la tabla prestamos
    string sqlPrestamo = "INSERT INTO prestamos (id_usuario, id_libro, fecha_prestamo) VALUES (?, ?, ?);";
    sqlite3_stmt *stmtPrestamo;

    if (sqlite3_prepare_v2(DB, sqlPrestamo.c_str(), -1, &stmtPrestamo, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmtPrestamo, 1, id_usuario);
        sqlite3_bind_int(stmtPrestamo, 2, id_libro);
        sqlite3_bind_text(stmtPrestamo, 3, fecha_prestamo, -1, SQLITE_STATIC);

        // Ejecutar la consulta
        if (sqlite3_step(stmtPrestamo) == SQLITE_DONE)
        {
            cout << "Préstamo agregado exitosamente." << endl;
            sleep(2);

            // Marcar el libro como no disponible
            string sqlActualizar = "UPDATE libros SET disponible = 0 WHERE id_libro = ?;";
            sqlite3_stmt *stmtActualizar;

            if (sqlite3_prepare_v2(DB, sqlActualizar.c_str(), -1, &stmtActualizar, NULL) == SQLITE_OK)
            {
                sqlite3_bind_int(stmtActualizar, 1, id_libro);
                sqlite3_step(stmtActualizar);
                sqlite3_finalize(stmtActualizar);
            }
        }
        else
        {
            cout << "Error al agregar el préstamo: " << sqlite3_errmsg(DB) << endl;
            sleep(2);
        }
    }
    else
    {
        cout << "Error al preparar la consulta de préstamo: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
    }

    sqlite3_finalize(stmtPrestamo);
    sqlite3_close(DB);
}

void mostrarPrestamos()
{
    sqlite3_open("BSUHMD.db", &DB);
    const char *sql = "SELECT p.id_prestamo, u.nombre, l.titulo, p.fecha_prestamo FROM prestamos p JOIN usuarios u ON p.id_usuario = u.id_usuario JOIN libros l ON p.id_libro = l.id_libro;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        cout << "Error al preparar la consulta: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
        return;
    }

    cout << "Lista de prestamos:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id_prestamo = sqlite3_column_int(stmt, 0);
        const char *nombre = (const char *)sqlite3_column_text(stmt, 1);
        const char *libro = (const char *)sqlite3_column_text(stmt, 2);
        const char *fecha = (const char *)sqlite3_column_text(stmt, 3);

        // Imprime la información del prestatario
        cout << "ID: " << id_prestamo << ", Nombre: " << nombre << ", Libro: " << libro << ", Fecha de prestamo: " << fecha << endl;
    }
    sleep(10);
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void devolverLibro(string CI, string titulo)
{
    sqlite3_open("BSUHMD.db", &DB);
    // Buscar el ID del usuario mediante la CI
    string sqlUsuario = "SELECT id_usuario FROM usuarios WHERE CI = ?;";
    sqlite3_stmt *stmtUsuario;
    int rc = sqlite3_prepare_v2(DB, sqlUsuario.c_str(), -1, &stmtUsuario, NULL);
    sqlite3_bind_text(stmtUsuario, 1, CI.c_str(), -1, SQLITE_STATIC);
    if (rc == SQLITE_OK)
    {

        if (sqlite3_step(stmtUsuario) == SQLITE_ROW)
        {
            int id_usuario = sqlite3_column_int(stmtUsuario, 0);
            // Buscar el ID del libro mediante el nombre
            string sqlLibro = "SELECT id_libro FROM libros WHERE titulo = ?;";
            sqlite3_stmt *stmtLibro;
            if (sqlite3_prepare_v2(DB, sqlLibro.c_str(), -1, &stmtLibro, NULL) == SQLITE_OK)
            {
                sqlite3_bind_text(stmtLibro, 1, titulo.c_str(), -1, SQLITE_STATIC);
                if (sqlite3_step(stmtLibro) == SQLITE_ROW)
                {
                    int id_libro = sqlite3_column_int(stmtLibro, 0);
                    // Actualizar el estado del libro a disponible
                    string sqlLibroDisponible = "UPDATE libros SET disponible = 1 WHERE id_libro = ?;";
                    sqlite3_stmt *stmtLibroDisponible;
                    if (sqlite3_prepare_v2(DB, sqlLibroDisponible.c_str(), -1, &stmtLibroDisponible, NULL) == SQLITE_OK)
                    {
                        sqlite3_bind_int(stmtLibroDisponible, 1, id_libro);
                        sqlite3_step(stmtLibroDisponible);
                        sqlite3_finalize(stmtLibroDisponible);
                        cout << "Libro devuelto exitosamente." << endl;
                        sleep(2);
                        system("clear");
                    }
                    else
                    {
                        cout << "Error al preparar la consulta: " << sqlite3_errmsg(DB) << endl;
                        sleep(2);
                    }
                    sqlite3_finalize(stmtLibro);
                }
                else
                {
                    cout << "Libro no encontrado." << endl;
                    sleep(2);
                }
            }
            else
            {
                cout << "Error al preparar la consulta: " << sqlite3_errmsg(DB) << endl;
                sleep(2);
            }
            sqlite3_finalize(stmtUsuario);
        }
        else
        {
            cout << "Usuario no encontrado." << endl;
            sleep(2);
        }
    }
    else
    {
        cout << "Error al preparar la consulta: " << sqlite3_errmsg(DB) << endl;
        sleep(2);
    }
    sqlite3_close(DB);
}