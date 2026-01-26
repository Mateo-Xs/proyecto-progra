#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAX_VENTAS    1000
#define MAX_PRODUCTOS 100
#define MAX_NOMBRE    50

// =====================
// Estructuras
// =====================

typedef struct {
    int   id;
    char  nombre[MAX_NOMBRE];
    float precio;
    int   stock;
    float iva;     // 0.00, 0.12, 0.15
    int   activo;  // 1 activo, 0 inactivo
} PRODUCTO;

typedef struct {
    int   idVenta;        // id de la factura (misma para varios items)
    int   idProducto;
    char  nombre[MAX_NOMBRE];
    int   cantidad;
    float precioUnit;
    float subtotal;
    float iva;
    float tipoIva;
    float total;
    char  fechaHora[20];
} VENTA_ITEM;

// =====================
// Datos globales
// =====================

static PRODUCTO productos[MAX_PRODUCTOS] ;

static int idx_productos = 5;                // cuantos productos existen
static int next_id_producto = 115;          // siguiente id sugerido

static VENTA_ITEM ventas[MAX_VENTAS];
static int total_items_ventas = 0;          // cuantos items hay en historial
static int next_id_factura = 1;

static int   caja_abierta = 0;
static float caja_inicial = 0.0f;
static float total_vendido = 0.0f;

// Helpers: entrada segura con fgets

static void trim_newline(char *s) {
    s[strcspn(s, "\r\n")] = '\0';
}

static int read_line(char *buf, size_t n) {
    if (!fgets(buf, (int)n, stdin)) return 0;
    trim_newline(buf);
    return 1;
}

static int read_int_range(const char *prompt, int min, int max) {
    char buf[128];
    for (;;) {
        printf("%s", prompt);
        if (!read_line(buf, sizeof(buf))) return min;
        char *end = NULL;
        long v = strtol(buf, &end, 10);
        if (end == buf || *end != '\0') {
            printf("Entrada invalida. Intenta otra vez.\n");
            continue;
        }
        if (v < min || v > max) {
            printf("Fuera de rango (%d..%d).\n", min, max);
            continue;
        }
        return (int)v;
    }
}

static float read_float_range(const char *prompt, float min, float max) {
    char buf[128];
    for (;;) {
        printf("%s", prompt);
        if (!read_line(buf, sizeof(buf))) return min;
        char *end = NULL;
        float v = strtof(buf, &end);
        if (end == buf || *end != '\0') {
            printf("Entrada invalida. Intenta otra vez.\n");
            continue;
        }
        if (v < min || v > max) {
            printf("Fuera de rango (%.2f..%.2f).\n", min, max);
            continue;
        }
        return v;
    }
}

static void read_string(const char *prompt, char *out, size_t n) {
    char buf[256];
    for (;;) {
        printf("%s", prompt);
        if (!read_line(buf, sizeof(buf))) {
            out[0] = '\0';
            return;
        }
        if (buf[0] == '\0') {
            printf("No puede estar vacio.\n");
            continue;
        }
        strncpy(out, buf, n);
        out[n - 1] = '\0';
        return;
    }
}

static int read_yes_no(const char *prompt) {
    char buf[32];
    for (;;) {
        printf("%s", prompt);
        if (!read_line(buf, sizeof(buf))) return 0;
        if (buf[0] == 's' || buf[0] == 'S') return 1;
        if (buf[0] == 'n' || buf[0] == 'N') return 0;
        printf("Responde con s/n.\n");
    }
}

// =====================
// Búsquedas
// =====================

static int buscarProductoPorId(int idBuscado) {
    for (int i = 0; i < idx_productos; i++) {
        if (productos[i].activo == 1 && productos[i].id == idBuscado) return i;
    }
    return -1;
}

static int buscarProductoPorNombre(const char *nombreBuscado) {
    for (int i = 0; i < idx_productos; i++) {
        if (productos[i].activo == 1 && strcmp(productos[i].nombre, nombreBuscado) == 0) return i;
    }
    return -1;
}

// =====================
// Impresión en columnas
// =====================

static void imprimir_productos_tabla(void) {
    printf("\n%-4s %-6s %-22s %10s %8s %6s %7s\n",
           "Indice", "ID", "Nombre", "Precio", "Stock", "IVA", "Activo");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < idx_productos; i++) {
        printf("%-4d %-6d %-22.22s %10.2f %8d %6.2f %7d\n",
               i,
               productos[i].id,
               productos[i].nombre,
               productos[i].precio,
               productos[i].stock,
               productos[i].iva,
               productos[i].activo);
    }
}

// =====================
// Modulo Productos
// =====================

static void registrar_producto(void) {
    if (idx_productos >= MAX_PRODUCTOS) {
        printf("\nNo puedes registrar mas productos (MAX_PRODUCTOS=%d).\n", MAX_PRODUCTOS);
        return;
    }

    int n = read_int_range("\nCuantos productos vas a registrar? ", 1, MAX_PRODUCTOS);
    int espacio = MAX_PRODUCTOS - idx_productos;
    if (n > espacio) {
        printf("Solo hay espacio para %d productos. Se registraran %d.\n", espacio, espacio);
        n = espacio;
    }

    for (int i = 0; i < n; i++) {
        PRODUCTO p;
        p.id = next_id_producto++;
        read_string("Nombre del producto: ", p.nombre, sizeof(p.nombre));
        p.precio = read_float_range("Precio (mayor a 0): ", 0.01f, 1000000.0f);
        p.stock  = read_int_range("Stock inicial (>=0): ", 0, 1000000);

        for (;;) {
            p.iva = read_float_range("IVA (0, 0.12, 0.15): ", 0.0f, 0.15f);
            if (fabsf(p.iva - 0.0f) < 0.0001f || fabsf(p.iva - 0.12f) < 0.0001f || fabsf(p.iva - 0.15f) < 0.0001f) break;
            printf("IVA invalido. Debe ser 0, 0.12 o 0.15\n");
        }

        p.activo = 1;
        productos[idx_productos++] = p;

        printf("Producto registrado. ID asignado: %d\n", p.id);
    }
}

static void actualizar_producto(void) {
    printf("\nActualizar producto:\n");
    printf("(1) Por indice\n(2) Por id\n(3) Por nombre\n(0) Volver\n");
    int op = read_int_range("Opcion: ", 0, 3);
    if (op == 0) return;

    int indice = -1;
    if (op == 1) {
        indice = read_int_range("Indice del producto: ", 0, idx_productos - 1);
    } else if (op == 2) {
        int id = read_int_range("ID del producto: ", 0, 10000000);
        indice = buscarProductoPorId(id);
    } else {
        char nombre[MAX_NOMBRE];
        read_string("Nombre exacto del producto: ", nombre, sizeof(nombre));
        indice = buscarProductoPorNombre(nombre);
    }

    if (indice < 0 || indice >= idx_productos) {
        printf("Producto no encontrado.\n");
        return;
    }

    PRODUCTO *p = &productos[indice];
    printf("\nActual:\n");
    printf("ID: %d\nNombre: %s\nPrecio: %.2f\nStock: %d\nIVA: %.2f\nActivo: %d\n",
           p->id, p->nombre, p->precio, p->stock, p->iva, p->activo);

    if (read_yes_no("Cambiar nombre? (s/n): ")) {
        read_string("Nuevo nombre: ", p->nombre, sizeof(p->nombre));
    }
    if (read_yes_no("Cambiar precio? (s/n): ")) {
        p->precio = read_float_range("Nuevo precio: ", 0.01f, 1000000.0f);
    }
    if (read_yes_no("Cambiar IVA? (s/n): ")) {
        for (;;) {
            float iva = read_float_range("Nuevo IVA (0, 0.12, 0.15): ", 0.0f, 0.15f);
            if (fabsf(iva - 0.0f) < 0.0001f || fabsf(iva - 0.12f) < 0.0001f || fabsf(iva - 0.15f) < 0.0001f) {
                p->iva = iva;
                break;
            }
            printf("IVA invalido.\n");
        }
    }
    if (read_yes_no("Cambiar activo (activar/desactivar)? (s/n): ")) {
        p->activo = !p->activo;
    }

    printf("Producto actualizado.\n");
}

static void menu_productos(void) {
    for (;;) {
        printf("\n--- Menu Productos ---\n");
        printf("(1) Registrar producto\n");
        printf("(2) Listar productos (tabla)\n");
        printf("(3) Actualizar producto\n");
        printf("(0) Volver\n");
        int op = read_int_range("Opcion: ", 0, 3);
        if (op == 0) return;
        if (op == 1) registrar_producto();
        if (op == 2) imprimir_productos_tabla();
        if (op == 3) actualizar_producto();
    }
}

// =====================
// Caja
// =====================

static void abrir_caja(void) {
    if (caja_abierta) {
        printf("\nLa caja ya esta abierta.\n");
        return;
    }
    caja_inicial = read_float_range("\nMonto inicial en caja ($): ", 0.0f, 100000000.0f);
    caja_abierta = 1;
    printf("Caja abierta.\n");
}

static void cerrar_caja(void) {
    if (!caja_abierta) {
        printf("\nLa caja ya esta cerrada.\n");
        return;
    }
    caja_abierta = 0;
    printf("\nCaja cerrada. Total vendido: %.2f $\n", total_vendido);
    printf("Total final (caja inicial + vendido): %.2f $\n", caja_inicial + total_vendido);
}

static void modulo_caja(void) {
    for (;;) {
        printf("\n--- Modulo Caja ---\n");
        printf("(1) Abrir caja\n(2) Cerrar caja\n(0) Volver\n");
        int op = read_int_range("Opcion: ", 0, 2);
        if (op == 0) return;
        if (op == 1) abrir_caja();
        if (op == 2) cerrar_caja();
    }
}

// =====================
// Ventas (factura con varios productos)
// =====================

static void set_fecha_hora(char out[20]) {
    time_t ahora = time(NULL);
    struct tm *info = localtime(&ahora);
    strftime(out, 20, "%d/%m/%Y %H:%M", info);
}

static int seleccionar_producto(void) {
    printf("\nBuscar producto para agregar:\n");
    printf("(1) Por indice\n(2) Por id\n(3) Por nombre\n(0) Terminar factura\n");
    int op = read_int_range("Opcion: ", 0, 3);
    if (op == 0) return -1;

    if (op == 1) {
        imprimir_productos_tabla();
        int idx = read_int_range("Indice: ", 0, idx_productos - 1);
        if (productos[idx].activo != 1) {
            printf("Producto inactivo.\n");
            return -2;
        }
        return idx;
    }

    if (op == 2) {
        int id = read_int_range("ID: ", 0, 10000000);
        int idx = buscarProductoPorId(id);
        if (idx == -1) {
            printf("No encontrado.\n");
            return -2;
        }
        return idx;
    }

    char nombre[MAX_NOMBRE];
    read_string("Nombre exacto: ", nombre, sizeof(nombre));
    int idx = buscarProductoPorNombre(nombre);
    if (idx == -1) {
        printf("No encontrado.\n");
        return -2;
    }
    return idx;
}

static void registrar_item_venta(int idFactura, int indiceProducto, int cantidad) {
    if (total_items_ventas >= MAX_VENTAS) {
        printf("No se puede registrar mas ventas (MAX_VENTAS=%d).\n", MAX_VENTAS);
        return;
    }

    PRODUCTO *p = &productos[indiceProducto];

    // Descontar stock
    p->stock -= cantidad;

    VENTA_ITEM v;
    memset(&v, 0, sizeof(v));
    v.idVenta = idFactura;
    v.idProducto = p->id;
    strncpy(v.nombre, p->nombre, sizeof(v.nombre));
    v.nombre[sizeof(v.nombre) - 1] = '\0';
    v.cantidad = cantidad;
    v.precioUnit = p->precio;
    v.tipoIva = p->iva;
    v.subtotal = v.precioUnit * (float)cantidad;
    v.iva      = v.subtotal * v.tipoIva;
    v.total    = v.subtotal + v.iva;
    set_fecha_hora(v.fechaHora);

    ventas[total_items_ventas++] = v;

    // actualizar total vendido (para caja)
    total_vendido += v.total;
}

static void imprimir_factura(int idFactura) {
    printf("\n================= FACTURA #%d =================\n", idFactura);

    // buscar fecha (la primera que coincida)
    const char *fecha = "";
    for (int i = 0; i < total_items_ventas; i++) {
        if (ventas[i].idVenta == idFactura) {
            fecha = ventas[i].fechaHora;
            break;
        }
    }
    printf("Fecha/Hora: %s\n", fecha);

    printf("\n%-3s %-6s %-22s %6s %10s %10s %6s %10s\n",
           "No", "ID", "Producto", "Cant", "P.Unit", "Subtot", "IVA", "Total");
    printf("----------------------------------------------------------------------------\n");

    float sumSub = 0.0f, sumIva = 0.0f, sumTot = 0.0f;
    float tot0 = 0.0f, tot12 = 0.0f, tot15 = 0.0f;

    int n = 0;
    for (int i = 0; i < total_items_ventas; i++) {
        if (ventas[i].idVenta != idFactura) continue;
        n++;
        printf("%-3d %-6d %-22.22s %6d %10.2f %10.2f %6.2f %10.2f\n",
               n,
               ventas[i].idProducto,
               ventas[i].nombre,
               ventas[i].cantidad,
               ventas[i].precioUnit,
               ventas[i].subtotal,
               ventas[i].iva,
               ventas[i].total);
        sumSub += ventas[i].subtotal;
        sumIva += ventas[i].iva;
        sumTot += ventas[i].total;

        if (fabsf(ventas[i].tipoIva - 0.12f) < 0.0001f) tot12 += ventas[i].total;
        else if (fabsf(ventas[i].tipoIva - 0.15f) < 0.0001f) tot15 += ventas[i].total;
        else tot0 += ventas[i].total;
    }

    printf("----------------------------------------------------------------------------\n");
    printf("%52s %10.2f\n", "SUBTOTAL:", sumSub);
    printf("%52s %10.2f\n", "IVA:", sumIva);
    printf("%52s %10.2f\n", "TOTAL:", sumTot);

    printf("\nTotales por IVA (sobre TOTAL):\n");
    printf("  IVA 0%%  : %.2f\n", tot0);
    printf("  IVA 12%% : %.2f\n", tot12);
    printf("  IVA 15%% : %.2f\n", tot15);

    printf("===============================================================\n");
}

static void nueva_factura(void) {
    if (!caja_abierta) {
        printf("\nLa caja esta cerrada. Abrela primero para vender.\n");
        return;
    }

    int idFactura = next_id_factura++;
    int itemsAntes = total_items_ventas;

    printf("\nCreando FACTURA #%d\n", idFactura);

    for (;;) {
        int idx = seleccionar_producto();
        if (idx == -1) break;          // terminar factura
        if (idx == -2) continue;       // no encontrado / inactivo

        if (productos[idx].stock <= 0) {
            printf("Sin stock disponible.\n");
            continue;
        }

        printf("Producto: %s | Stock: %d | Precio: %.2f | IVA: %.2f\n",
               productos[idx].nombre, productos[idx].stock, productos[idx].precio, productos[idx].iva);

        int cant = read_int_range("Cantidad a vender: ", 1, 1000000);
        if (cant > productos[idx].stock) {
            printf("No hay suficiente stock.\n");
            continue;
        }

        registrar_item_venta(idFactura, idx, cant);

        if (!read_yes_no("Agregar otro producto? (s/n): ")) break;
    }

    if (total_items_ventas == itemsAntes) {
        printf("Factura cancelada (no agregaste items).\n");
        // no decremento next_id_factura para mantener ids monotonic
        return;
    }

    imprimir_factura(idFactura);
}

static void mostrar_factura_por_id(void) {
    int id = read_int_range("\nID de factura a mostrar: ", 1, 100000000);

    int existe = 0;
    for (int i = 0; i < total_items_ventas; i++) {
        if (ventas[i].idVenta == id) { existe = 1; break; }
    }
    if (!existe) {
        printf("No existe esa factura.\n");
        return;
    }
    imprimir_factura(id);
}

static void modulo_ventas(void) {
    for (;;) {
        printf("\n--- Modulo Ventas ---\n");
        printf("(1) Nueva factura (varios productos)\n");
        printf("(2) Mostrar factura por ID\n");
        printf("(0) Volver\n");
        int op = read_int_range("Opcion: ", 0, 2);
        if (op == 0) return;
        if (op == 1) nueva_factura();
        if (op == 2) mostrar_factura_por_id();
    }
}

// =====================
// Inventario
// =====================

static void modulo_inventario(void) {
    for (;;) {
        printf("\n--- Modulo Inventario (ajustar stock) ---\n");
        printf("(1) Por indice\n(2) Por id\n(3) Por nombre\n(0) Volver\n");
        int op = read_int_range("Opcion: ", 0, 3);
        if (op == 0) return;

        int idx = -1;
        if (op == 1) {
            imprimir_productos_tabla();
            idx = read_int_range("Indice: ", 0, idx_productos - 1);
        } else if (op == 2) {
            int id = read_int_range("ID: ", 0, 10000000);
            idx = buscarProductoPorId(id);
        } else {
            char nombre[MAX_NOMBRE];
            read_string("Nombre exacto: ", nombre, sizeof(nombre));
            idx = buscarProductoPorNombre(nombre);
        }

        if (idx < 0 || idx >= idx_productos) {
            printf("Producto no encontrado.\n");
            continue;
        }

        int ajuste = read_int_range("Ajuste de stock (puede ser negativo): ", -1000000, 1000000);
        if (productos[idx].stock + ajuste < 0) {
            printf("No puedes dejar el stock negativo.\n");
            continue;
        }
        productos[idx].stock += ajuste;
        printf("Nuevo stock de %s: %d\n", productos[idx].nombre, productos[idx].stock);
    }
}

// =====================
// Reportes
// =====================

static void reporte_totales_iva(void) {
    float tot0 = 0.0f, tot12 = 0.0f, tot15 = 0.0f;
    for (int i = 0; i < total_items_ventas; i++) {
        if (fabsf(ventas[i].tipoIva - 0.12f) < 0.0001f) tot12 += ventas[i].total;
        else if (fabsf(ventas[i].tipoIva - 0.15f) < 0.0001f) tot15 += ventas[i].total;
        else tot0 += ventas[i].total;
    }

    printf("\n== Totales de ventas por tipo de IVA (sobre TOTAL) ==\n");
    printf("Ventas con IVA 0%%  : %.2f\n", tot0);
    printf("Ventas con IVA 12%% : %.2f\n", tot12);
    printf("Ventas con IVA 15%% : %.2f\n", tot15);
    printf("TOTAL general      : %.2f\n", tot0 + tot12 + tot15);
}

static void reporte_stock_bajo(void) {
    int limite = read_int_range("\nStock bajo si es <= ? ", 0, 1000000);

    printf("\nProductos con stock <= %d\n", limite);
    printf("%-6s %-22s %8s\n", "ID", "Nombre", "Stock");
    printf("---------------------------------------------\n");

    int encontrados = 0;
    for (int i = 0; i < idx_productos; i++) {
        if (productos[i].activo == 1 && productos[i].stock <= limite) {
            printf("%-6d %-22.22s %8d\n", productos[i].id, productos[i].nombre, productos[i].stock);
            encontrados++;
        }
    }
    if (!encontrados) printf("(ninguno)\n");
}

static void reporte_historial_ventas(void) {
    printf("\nHistorial de items de venta (%d items)\n", total_items_ventas);
    printf("%-6s %-6s %-22s %6s %10s %10s %6s %10s %17s\n",
           "Fac", "ID", "Producto", "Cant", "P.Unit", "Subtot", "IVA", "Total", "Fecha");
    printf("------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < total_items_ventas; i++) {
        printf("%-6d %-6d %-22.22s %6d %10.2f %10.2f %6.2f %10.2f %17s\n",
               ventas[i].idVenta,
               ventas[i].idProducto,
               ventas[i].nombre,
               ventas[i].cantidad,
               ventas[i].precioUnit,
               ventas[i].subtotal,
               ventas[i].iva,
               ventas[i].total,
               ventas[i].fechaHora);
    }
}

static void modulo_reportes(void) {
    for (;;) {
        printf("\n--- Modulo Reportes ---\n");
        printf("(1) Totales por IVA\n(2) Productos con stock bajo\n(3) Historial de ventas\n(0) Volver\n");
        int op = read_int_range("Opcion: ", 0, 3);
        if (op == 0) return;
        if (op == 1) reporte_totales_iva();
        if (op == 2) reporte_stock_bajo();
        if (op == 3) reporte_historial_ventas();
    }
}

// =====================
// CSV: carga/guardado
// Formato productos.csv:
// id,nombre,precio,stock,iva,activo
// Formato ventas.csv:
// idVenta,idProducto,nombre,cantidad,precioUnit,subtotal,iva,tipoIva,total,fechaHora
// =====================

static int cargar_productos_csv(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;

    char line[512];
    int count = 0;

    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (line[0] == '\0') continue;
        if (!isdigit((unsigned char)line[0])) continue; // salta header o comentarios

        PRODUCTO p;
        memset(&p, 0, sizeof(p));

        // %49[^,] lee hasta la coma (incluye espacios)
        int ok = sscanf(line, "%d,%49[^,],%f,%d,%f,%d",
                        &p.id, p.nombre, &p.precio, &p.stock, &p.iva, &p.activo);
        if (ok == 6) {
            if (count < MAX_PRODUCTOS) {
                productos[count++] = p;
            }
        }
    }

    fclose(f);

    if (count > 0) {
        idx_productos = count;

        int maxId = productos[0].id;
        for (int i = 1; i < idx_productos; i++) if (productos[i].id > maxId) maxId = productos[i].id;
        next_id_producto = maxId + 1;

        return 1;
    }

    return 0;
}

static int guardar_productos_csv(const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) return 0;

    fprintf(f, "id,nombre,precio,stock,iva,activo\n");
    for (int i = 0; i < idx_productos; i++) {
        fprintf(f, "%d,%s,%.2f,%d,%.2f,%d\n",
                productos[i].id,
                productos[i].nombre,
                productos[i].precio,
                productos[i].stock,
                productos[i].iva,
                productos[i].activo);
    }

    fclose(f);
    return 1;
}

static int cargar_ventas_csv(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;

    char line[512];
    int count = 0;

    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (line[0] == '\0') continue;
        if (!isdigit((unsigned char)line[0])) continue;

        VENTA_ITEM v;
        memset(&v, 0, sizeof(v));

        int ok = sscanf(line, "%d,%d,%49[^,],%d,%f,%f,%f,%f,%f,%19[^\n]",
                        &v.idVenta, &v.idProducto, v.nombre, &v.cantidad,
                        &v.precioUnit, &v.subtotal, &v.iva, &v.tipoIva, &v.total, v.fechaHora);

        if (ok == 10) {
            if (count < MAX_VENTAS) ventas[count++] = v;
        }
    }

    fclose(f);

    if (count > 0) {
        total_items_ventas = count;

        int maxFac = ventas[0].idVenta;
        for (int i = 1; i < total_items_ventas; i++) if (ventas[i].idVenta > maxFac) maxFac = ventas[i].idVenta;
        next_id_factura = maxFac + 1;

        // recomputar total vendido
        total_vendido = 0.0f;
        for (int i = 0; i < total_items_ventas; i++) total_vendido += ventas[i].total;

        return 1;
    }

    return 0;
}

static int guardar_ventas_csv(const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) return 0;

    fprintf(f, "idVenta,idProducto,nombre,cantidad,precioUnit,subtotal,iva,tipoIva,total,fechaHora\n");
    for (int i = 0; i < total_items_ventas; i++) {
        fprintf(f, "%d,%d,%s,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%s\n",
                ventas[i].idVenta,
                ventas[i].idProducto,
                ventas[i].nombre,
                ventas[i].cantidad,
                ventas[i].precioUnit,
                ventas[i].subtotal,
                ventas[i].iva,
                ventas[i].tipoIva,
                ventas[i].total,
                ventas[i].fechaHora);
    }

    fclose(f);
    return 1;
}

// =====================
// Main
// =====================

int main(void) {
    // Cargar datos si existen
    cargar_productos_csv("productos.csv");
    cargar_ventas_csv("ventas.csv");

    for (;;) {
        printf("\n## Bienvenido al puesto de ventas ##\n");
        printf("Ventas EPNT\n");
        printf("(1) Modulo de productos\n");
        printf("(2) Modulo de ventas (facturas)\n");
        printf("(3) Modulo de inventario\n");
        printf("(4) Modulo de reportes\n");
        printf("(5) Modulo de caja\n");
        printf("(0) Salir\n");

        int menu = read_int_range("Opcion: ", 0, 5);

        if (menu == 0) break;
        if (menu == 1) menu_productos();
        if (menu == 2) modulo_ventas();
        if (menu == 3) modulo_inventario();
        if (menu == 4) modulo_reportes();
        if (menu == 5) modulo_caja();
    }

    // Guardar al salir
    if (!guardar_productos_csv("productos.csv")) {
        printf("\nAviso: No se pudo guardar productos.csv\n");
    }
    if (!guardar_ventas_csv("ventas.csv")) {
        printf("\nAviso: No se pudo guardar ventas.csv\n");
    }

    printf("\nMuchas gracias por usar Ventas EPNT\n");
    return 0;
}
