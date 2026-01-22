#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define max_ventas    1000
#define max_productos 100
#define MAX_NOMBRE    50

//define los atributos del producto
typedef struct
{
    int id;
    char nombre[50];
    float precio;
    int stock;
    float iva;   
    int activo;
}PRODUCTO;

//definimos los 5 productos con los que iniciamos
PRODUCTO productos[max_productos];

int total_productos=0;
int idx=0;



// los atributos de la venta
typedef struct
{
    int idVenta;          // 1, 2, 3, ...
    int idProducto;      // id del producto vendido
    char nombre[50];       
    int cantidad;         // unidades
    float subtotal;
    float iva;
    float tipoIva;
    float total;
    char fechaHora[20];     

}Venta;
//para las ventas, caja y reporte4s
Venta ventas[max_ventas];
int total_ventas=0;
float total_venta_dinero=0.0;
int caja_abierta =0;
float totalIva0 = 0.0, totalIva12 = 0.0, totalIva15 = 0.0,totalVentasDia=0.0;

//funcion de modulo de productos,registrar, listar y actualizar productos )

// funcion de regitrar producto
int productos_registrados;

//funciones  de lectura y control de bugs
static void eliminar_saltos(char *s) {
    s[strcspn(s, "\r\n")] = '\0';
}

static int lector_de_todo(char *buf, size_t n) {
    if (fgets(buf, (int)n, stdin)== NULL) return 0;
    eliminar_saltos(buf);
    return 1;
}

static int lector_entero(const char *mensaje, int min, int max) {
    char buf[128];
    for (;;) {
        printf("%s", mensaje);
        if (lector_de_todo(buf, sizeof(buf))==0) return min;
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
float lector_float(const char *mensaje, float min, float max) {
    char buf[128];
    for (;;) {
        printf("%s", mensaje);
        if (lector_de_todo(buf, sizeof(buf))==0) return min;
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

void lector_string(const char *mensaje, char *salida, size_t n) {
    char buf[256];
    for (;;) {
        printf("%s", mensaje);
        if (lector_de_todo(buf, sizeof(buf))==0) {
        salida[0] = '\0';
            return;
        }
        if (buf[0] == '\0') {
            printf("No puede estar vacio.\n");
            continue;
        }
        strncpy(salida, buf, n);
        salida[n - 1] = '\0';
        return;
    }
}



void registar_producto(){
    

    printf("\ningrese el numero de productos a registrar: \n");
    scanf("%d",&productos_registrados);

    for (int i = 0; i < productos_registrados; i++)
    {
    
    

    printf("\nproducto de indice %d \n",i+idx);

    productos[i+idx].id = idx+i + 110;
    printf("ID asignado: %d\n", productos[i+idx].id);

    printf("Ingrese el nombre (una palabra): \n");
    scanf("%49s", productos[i+idx].nombre);

     printf("Ingrese el precio: \n");
    scanf("%f", &productos[i+idx].precio);
    if (productos[i+idx].precio<=0){
        printf("numero no valido en precio");
        break;
    }

    // 4) Stock
    printf("Ingrese el stock inicial: \n");
    scanf("%d", &productos[i+idx].stock);
    if (productos[i+idx].stock<=0){
        printf("el stock no puede ser ese numero");
        break;
    }

    // 5) IVA
    printf("Ingrese el IVA (0, 0.12, 0.15): \n");
    scanf("%f", &productos[i+idx].iva);
    
    if (productos[i + idx].iva <0.0||productos[i + idx].iva >1.0) {

    printf("Numero no valido en IVA. Debe ser 0, 0.12 o 0.15.\n");
    break;   
}
    

    productos[i+idx].activo = 1;

    printf("\nsi se equivoco en el modulo de actualizar puede modificarlo\n\n");
        total_productos ++;
    }
//NUMERO PARA LOS INDICES DE ACUMULATIVO Y NO SE REPITA EL MISMO INDICE 
idx += total_productos;
}

//FUNCION DE LISTAR PROD
void listar_productos(){
    printf("tabla de productos(posicion,id,nombre, precio, stock,iva,activo)");
    for (int i = 0; i < idx; i++)
    {
        printf("\n el producto tiene una posicion %d \n",i);
        printf(" id %d \n ",productos[i].id);
        printf(" nombre  %s\n",productos[i].nombre);
        printf(" precio  %.2f  \n ",productos[i].precio);
        printf(" stock  %d  \n ",productos[i].stock);
        printf(" iva  %.2f  \n ",productos[i].iva);
        printf(" activo  %d  \n ",productos[i].activo);
        sleep(3);   

    }

}


void mostrar_productos(int indice,float nuevoPrecio,float nuevoIva){
    
    printf("\n el producto tiene una posicion %d \n",indice);
        printf("id %d \n",productos[indice].id);
        printf("nombre  %s\n",productos[indice].nombre);
        printf("precio  %.2f  \n",productos[indice].precio);
        printf("stock  %d  \n",productos[indice].stock);
        printf("iva  %.2f  \n",productos[indice].iva);
        printf("activo  %d  \n",productos[indice].activo);
        sleep(3);

        printf("a cambiar nombre:  ");
        scanf("%49s",productos[indice].nombre);
        printf("a cambiar precio: ");
        scanf("%f",&nuevoPrecio);
        printf("el IVA (0, 0.12, 0.15): \n");
        scanf("%f", &nuevoIva);
    
            
        productos[indice].precio = nuevoPrecio;
        productos[indice].iva    = nuevoIva;

}




/*CONFIGURAR PRODUCTO*/
void actualizar_producto(){
    printf("\n aqui puedes actualizar y modificar productos que estan en la lista de productos \n");
    printf("ingresa el numero para ver la forma de encontrar el producto a actualizar \n");
        printf("(1) por indice\n");
        printf("(2) por id\n");
        printf("(3) por nombre\n");
    int actualizar;

    int indice;
    int id;
    char nombre[50];

    float nuevoPrecio;
    float nuevoIva;

    scanf("%d",&actualizar);

    switch (actualizar)
    {
    case 1:
        printf("ingrese el indice del producto\n");
        scanf("%d",&indice);
        mostrar_productos(indice,nuevoPrecio,nuevoIva);
        break;

    case 2:
        printf("ingrese el id del producto\n");
        scanf("%d",&id);
        indice = buscarProductoPorId(id);
        if (indice != -1)
        {
        mostrar_productos(indice,nuevoPrecio,nuevoIva);
        }
        else{
            printf("\nno encontrado ese id\n");
        }
        
        break;
    case 3:
        printf("ingrese el nombre del producto\n");
        scanf("%49s",nombre);
        indice = buscarProductoPornombre(nombre);
        if (indice != -1)
        {
        mostrar_productos(indice,nuevoPrecio,nuevoIva);
        }
        else{
            printf("no encontrado ese nombre");
        }
        break;
    
    default:
        printf("ingrese un numero valido");
        break;
    }



}

int buscarProductoPorId(int idBuscado) {
    for (int i = 0; i < idx; i++) {
        if (productos[i].activo == 1 && productos[i].id == idBuscado) {
            return i;   // encontramos el producto en la posición i
        }
    }
    return -1;          // no encontrado
}

int buscarProductoPornombre(const char *nombreBuscado) {
    for (int i = 0; i < idx; i++) {
        if (productos[i].activo == 1 &&
            strcmp(productos[i].nombre, nombreBuscado) == 0) {
            return i;   // encontrado
        }
    }
    return -1;          // no encontrado
}



//funcion menu de productos
void menu_productos(){
    int menu_pr;
    do
    {
        printf("\n--- menu de Productos ---\n \'ingrese el numero en parentesis\' \n");
        printf("(1) Registrar producto\n");
        printf("(2) Listar productos\n");
        printf("(3) Actualizar producto\n");
        printf("(0) Volver\n");
        scanf("%d", &menu_pr);
        switch (menu_pr) {       
            case 1:
                registar_producto();
                break;
            case 2:
                listar_productos();
                break;
            case 3:
                actualizar_producto();
                break;
            case 0:
                // salir
                break;
            default:
                printf("Opcion no valida\n");
                break;
        }

    } while (menu_pr != 0);
    
}

float caja_total=0.0;
void abir_caja(){
    
    if(caja_abierta){
        printf("\nla caja ya esta abierta\n");
        sleep(2);
        return;
    }
    caja_abierta =1;
    printf("\n la caja se abrio\n");
    printf("ingrese la cantidad que hay en caja $ : ");
    scanf("%f",&caja_total);
    sleep(2);

}

void cerrar_caja(){
    
    if (caja_abierta==0)
    {
        printf("La caja ya esta cerrada.\n");
        sleep(2);
        return;
    }
    caja_abierta = 0;
    printf("Caja cerrada. Total vendido en el turno: %.2f $\n", total_venta_dinero);
    printf("y el total con caja: %.2f\n",total_venta_dinero+caja_total );
    sleep(2);
    

}

void modulo_caja(){
    int caja;
    printf("modulo de caja\n presione (1) para abrir \n presione (2) para cerrar\n presione(0) para salir \n");
    scanf("%d",&caja);
    switch (caja)
    {
    case 1:
        abir_caja();
        break;
    case 2:
        cerrar_caja();
        break;
    case 0:

        break;
    
    default:
        printf("numero no valido");
        break;
    }

}


void Actualizar_datos_venta(int indice,int cantidad){
            char nombres[50];
            // Actualizar stock
            productos[indice].stock -= cantidad;
            printf("\n\nNuevo stock: %d\n", productos[indice].stock);

            // Registrar venta basica 
            ventas[total_ventas].idProducto = productos[indice].id;
            strcpy(nombres,productos[indice].nombre );
            strcpy(ventas[total_ventas].nombre, nombres);
            ventas[total_ventas].cantidad   = cantidad;
            ventas[total_ventas].idVenta = 10+total_ventas;
            ventas[total_ventas].subtotal = cantidad*productos[indice].precio;
            ventas[total_ventas].iva = cantidad*productos[indice].precio*productos[indice].iva;
            ventas[total_ventas].total = ventas[total_ventas].subtotal + ventas[total_ventas].iva;
            ventas[total_ventas].tipoIva = productos[indice].iva;
            //tiempo
                        time_t ahora = time(NULL);          // 1) tiempo actual en segundos
                struct tm *info = localtime(&ahora); // 2) lo paso a fecha/hora local

                char fecha_hora[20];                    // 3) donde voy a guardar el texto
                strftime(fecha_hora, 20, "%d/%m/%Y %H:%M", info);
                strcpy(ventas[total_ventas].fechaHora, fecha_hora);// designar la fecha y hora
            
             printf("\n\nreporte de venta\n");
            printf("id producto vendido %d \n",ventas[total_ventas].idProducto);
            printf("nombre producto vendido %s\n",ventas[total_ventas].nombre);
            printf("cantidad producto vendido %d\n",ventas[total_ventas].cantidad);
            printf("idventa %d \n",ventas[total_ventas].idVenta);
            printf("subtotal %.2f $\n",ventas[total_ventas].subtotal);
            printf("iva %.2f $\n",ventas[total_ventas].iva);
            printf("total %.2f $\n",ventas[total_ventas].total);
            printf("fecha %s \n",ventas[total_ventas].fechaHora);
            sleep(2);
            
            total_ventas++;
}




void modulo_ventas(void) {

    if (caja_abierta == 0) {

        printf("La caja esta cerrada. Abra la caja en el menu para poder realizar ventas.\n");
        sleep(3);

        return;
    }

    int venta_realizada;

    do
    {
        
    

    printf("\n=== Modulo de Ventas ===\n");
    printf("Ingrese la forma de encontrar el producto para vender:\n");
    printf("(1) Por posicion en la tabla\n");
    printf("(2) Por id\n");
    printf("(3) Por nombre\n");
    printf("(0) Volver\n");
    printf("Opcion: ");
    scanf("%d", &venta_realizada);

    switch (venta_realizada) {

        case 1: {   // Vender por INDICE
            int indice, cantidad;
            

            printf("Ingrese el indice del producto: ");
            scanf("%d", &indice);

            if (indice < 0 || indice >= idx) {
                printf("Indice invalido.\n");
                break;  // volvemos al menu de ventas
            }

            printf("\n\nID: %d\n", productos[indice].id);
            printf("Nombre: %s\n", productos[indice].nombre);
            printf("\n Stock actual: %d\n", productos[indice].stock);

            printf("\n\nCantidad de producto a vender: \n");
            scanf("%d", &cantidad);

            if (cantidad <= 0) {
                printf("Cantidad invalida.\n");
                break;
            }

            if (productos[indice].stock - cantidad < 0) {
                printf("No hay suficiente stock para esa cantidad.\n");
                break;
            }
            Actualizar_datos_venta(indice,cantidad);

            printf("Venta registrada correctamente.\n");

            break;
        }

        case 2: {
            printf("\n\nVenta por ID .\n");
            int id, indice, cantidad;
            

            printf("Ingrese el id del producto: ");
            scanf("%d", &id);

            indice = buscarProductoPorId(id);
            if (indice == -1) {
            printf("Producto no encontrado.\n");
            break;
            }
            printf("ID: %d\n", productos[indice].id);
            printf("Nombre: %s\n", productos[indice].nombre);
            printf("\nStock actual: %d\n\n", productos[indice].stock);

            printf("\n\nCantidad de producto a vender: ");
            scanf("%d", &cantidad);

            if (cantidad <= 0) {
                printf("Cantidad invalida.\n");
                break;
            }

            if (productos[indice].stock - cantidad < 0) {
                printf("No hay suficiente stock para esa cantidad.\n");
                break;
            }

             Actualizar_datos_venta(indice,cantidad);


            break;
        }

        case 3: {
            printf("\n\nVenta por nombre.\n");
            int indice, cantidad;
            char nombre[50];
            

            printf("Ingrese el id del producto: ");
            scanf("%49s", nombre);

            indice = buscarProductoPornombre(nombre);
            if (indice == -1) {
            printf("Producto no encontrado.\n");
            break;
            }

            printf("ID: %d\n", productos[indice].id);
            printf("Nombre: %s\n", productos[indice].nombre);
            printf("Stock actual: %d\n", productos[indice].stock);

            printf("\nCantidad de producto a vender: ");
            scanf("%d", &cantidad);

            if (cantidad <= 0) {
                printf("Cantidad invalida.\n");
                break;
            }

            if (productos[indice].stock - cantidad < 0) {
                printf("No hay suficiente stock para esa cantidad.\n");
                break;
            }

             Actualizar_datos_venta(indice,cantidad);
            

            break;
        }

        case 0:
            // volver al menu anterior
            break;

        default:
            printf("Opcion de ventas no valida.\n");
            break;
    }
     
    } while (venta_realizada!=0);
}


void modulo_inventario(){
    int inve;
    do
    {
    
    char nombre[50];
    printf("\n aqui puedes ajustar el stock de un producto  \n");
    printf("ingresa el numero para ver la forma de encontrar el producto a ajustar stock \n");
        printf("(1) por indice\n");
        printf("(2) por id\n");
        printf("(3) por nombre\n");
        printf("(0) Volver\n");
        scanf("%d",&inve);
    switch (inve) {
    case 1: {   // AJUSTAR POR INDICE
        int indice, cantidad;

        printf("Ingrese el indice del producto: ");
        scanf("%d", &indice);

        if (indice < 0) {
            printf("Ingrese un indice valido.\n");
            break;   // volvemos al menu de inventario
        }
        printf(" id %d \n ",productos[indice].id);
        printf(" nombre  %s\n",productos[indice].nombre);
        printf("Stock actual: %d\n", productos[indice].stock);
        sleep(3);

        printf("\nCantidad a ajustar (positivo = aumenta, negativo = disminuye): \n");
        scanf("%d", &cantidad);

        if (productos[indice].stock + cantidad < 0) {
            printf("No puedes dejar el stock negativo.\n");
            break;   // NO usamos return, solo salimos del case
        }

        productos[indice].stock += cantidad;
        printf("Nuevo stock: %d\n", productos[indice].stock);
        break;
    }

    case 2: {   // AJUSTAR POR ID
        int id, indice, cantidad;

        printf("\nIngrese el id del producto: \n");
        scanf("%d", &id);

        indice = buscarProductoPorId(id);
        if (indice == -1) {
            printf("Producto no encontrado.\n");
            break;
        }
        printf(" id %d \n ",productos[indice].id);
        printf(" nombre  %s\n",productos[indice].nombre);
        printf("Stock actual: %d\n", productos[indice].stock);
        sleep(3);

        printf("Cantidad a ajustar (positivo = aumenta, negativo = disminuye): ");
        scanf("%d", &cantidad);

        if (productos[indice].stock + cantidad < 0) {
            printf("No puedes dejar el stock negativo.\n");
            break;
        }

        productos[indice].stock += cantidad;
        printf("Nuevo stock: %d\n", productos[indice].stock);
        break;
    }

    case 3: {
          int indice, cantidad;

        printf("\nIngrese el nombre del producto:\n ");
        scanf("%49s", nombre);

        indice = buscarProductoPornombre(nombre);
        if (indice == -1) {
            printf("Producto no encontrado.\n");
            break;
        }
        printf(" id %d \n ",productos[indice].id);
        printf(" nombre  %s\n",productos[indice].nombre);
        printf("Stock actual: %d\n", productos[indice].stock);
        sleep(3);

        printf("Cantidad a ajustar (positivo = aumenta, negativo = disminuye): \n");
        scanf("%d", &cantidad);

        if (productos[indice].stock + cantidad < 0) {
            printf("No puedes dejar el stock negativo.\n");
            break;
        }

        productos[indice].stock += cantidad;
        printf("Nuevo stock: %d\n", productos[indice].stock);
        
        break;
    }
    case 0:{
        break;
    }


    default:
        printf("Opcion de inventario no valida.\n");
        break;
}
} while (inve!=0);

}

void reporte_historial_ventas(){
    printf("\nreporte del dia de las ventas por producto\n");
    for (int i = 0; i < total_ventas; i++)
    {
        printf("\n\n --reporte de venta numero  -- %d -- \n",i+1);
            printf("id producto vendido %d \n",ventas[i].idProducto);
            printf("nombre producto vendido %s\n",ventas[i].nombre);
            printf("cantidad producto vendido %d\n",ventas[i].cantidad);
            printf("idventa %d \n",ventas[i].idVenta);
            printf("subtotal %.2f $\n",ventas[i].subtotal);
            printf("iva total %.2f $\n",ventas[i].tipoIva);
            printf("iva total %.2f $\n",ventas[i].iva);
            printf("total %.2f $\n",ventas[i].total);
            printf("fecha %s \n",ventas[i].fechaHora);
            
    }
    

}
float totaliva00,totaliva120,totaliva150;

void reporte_totales_iva(){
    for (int i = 0; i < total_ventas; i++)
    {
        if (fabs(ventas[i].tipoIva- 0.12f)< 0.0001f)
        {
            totaliva120 += ventas[i].total;
        }
        else if (fabs(ventas[i].tipoIva - 0.15f) < 0.0001f) {
            // es "prácticamente" 15 %
            totaliva150 += ventas[i].total;
                }
        else {
            // todo lo demás lo considero 0 %
            totaliva00 += ventas[i].total;
        }   
            
    }
     printf("\n== Totales de ventas por tipo de IVA ==\n");
    printf("Ventas con IVA 0%%   : %.2f\n", totaliva00);
    printf("Ventas con IVA 12%%  : %.2f\n", totaliva120);
    printf("Ventas con IVA 15%%  : %.2f\n", totaliva150);
    total_venta_dinero= totaliva00+totaliva120+totaliva150;
    printf("\n\n---TOTAL general del dia: %.2f --\n", total_venta_dinero);
    sleep(2);
    
}
//stock bajo
void reporte_stock_bajo() {
    int consideracion;

    printf("\n Reporte de stock bajo \n");
    printf("escriba el numero para que se considere que hay stock bajo cuando sea menor o igual a: ");
    scanf("%d", &consideracion);

    for (int i = 0; i < idx; i++)
    {
        if (productos[i].stock<= consideracion)
        {
            printf("\nID: %d\n", productos[i].id);
            printf("Nombre: %s\n", productos[i].nombre);
            printf("Stock actual: %d\n", productos[i].stock);
        }

        
    }
    
 printf("\nFin del reporte de stock bajo.\n");
}

//menu reportes
void modulo_reportes(void) {
    int mod_rep;
    do {
        printf("\n=== Modulo de Reportes ===\n");
        printf("(1) Totales de ventas por tipo de IVA\n");
        printf("(2) Productos con stock bajo\n");
        printf("(3) Historial de ventas\n");   // opcional
        printf("(0) Volver\n");
        printf("Opcion: ");
        scanf("%d", &mod_rep);

        switch (mod_rep) {
            case 1:
                reporte_totales_iva();
                break;
            case 2:
                reporte_stock_bajo();
                break;
            case 3:
                reporte_historial_ventas();   // si lo implementas
                break;
            case 0:
                break;
            default:
                printf("Opcion no valida.\n");
                break;
        }
    } while ( mod_rep!= 0);
}




int main() {
    int menu; 

    do {
        printf("\n## Bienvenido al puesto de ventas ##\n");
        printf("Ventas EPNT\n");
        printf("--- Ingrese el modulo a revisar ---\n");
        printf("(1) - Modulo de productos\n");
        printf("(2) - Modulo de ventas\n");
        printf("(3) - Modulo de inventario\n");
        printf("(4) - Modulo de reportes\n");
        printf("(5) - Modulo de caja\n");
        printf("(0) - Salir\n");
        printf("Opcion: ");

        scanf("%d", &menu);   // <-- lee un ENTERO

        switch (menu) {       
            case 1:
                menu_productos();
                break;
            case 2:
                modulo_ventas();
                break;
            case 3:
                modulo_inventario();
                break;
            case 4:
                modulo_reportes();
                break;
            case 5:
                modulo_caja();
                break;
            case 0:
                // salir
                break;
            default:
                printf("Opcion no valida\n");
                break;
        }

    } while (menu != 0);

    printf("Muchas gracias por usar Ventas EPNT\n");

    return 0;
}




