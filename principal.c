//SISTEMA DE PUNTO DE VENTAS E INVENTARIO: Poli Steel
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
// Códigos de colores ANSI para la consola
#define COLOR_RESET   "\x1b[0m"
#define ROJO     "\x1b[31m"
#define VERDE   "\x1b[32m"
#define AMARILLO  "\x1b[33m"
#define AZUL    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define MAXIMOPRODUCTOS 100
#define MAXIMOVENTAS 1000
typedef struct{
    int codigo;
    char nombre[50];
    float precio;
    int stock;
    int stockMinimo;
    int tipoIVA;
} Producto;
typedef struct{
    bool abierta;
    float montoInicial;
    float totalVentas;
    float ventasIVA0;
    float ventasIVA12;
}Caja;

typedef struct{
    int inicio;
    float suma;
}RachaVentas;

Producto productos[MAXIMOPRODUCTOS];
int numProductos = 0;
Caja caja;
float ivaGeneral = 0.12;
float ventasHistorial[MAXIMOVENTAS];
int numVentasHistorial = 0;

void barraCarga() {
    printf("\nCargando Sistema Poli Steel...\n");
    printf("[");
    for(int i = 0; i < 40; i++) {
        printf("="); 
        fflush(stdout); // Forza a mostrarlo
        Sleep(25); 
    }
    printf("] 100%%\n");
    printf(VERDE "Sistema cargado exitosamente.\n" COLOR_RESET);
    Sleep(500); 
    system("cls"); // Limpia la pantalla
}
// DATOS PRECARGADOS
void cargarDatosDePrueba(){
    // Producto 1
    productos[0].codigo = 101;
    strcpy(productos[0].nombre, "Varilla de Acero 1/2");
    productos[0].precio = 12.50;
    productos[0].stock = 40;
    productos[0].stockMinimo = 10;
    productos[0].tipoIVA = 1;

    // Producto 2
    productos[1].codigo = 102;
    strcpy(productos[1].nombre, "Cemento Holcim 50kg");
    productos[1].precio = 8.25;
    productos[1].stock = 100;
    productos[1].stockMinimo = 20;
    productos[1].tipoIVA = 1;

    // Producto 3
    productos[2].codigo = 103;
    strcpy(productos[2].nombre, "Clavos 2 pulgadas (Libra)");
    productos[2].precio = 1.50;
    productos[2].stock = 200;
    productos[2].stockMinimo = 50;
    productos[2].tipoIVA = 1;
    
    // Producto 4
    productos[3].codigo = 104;
    strcpy(productos[3].nombre, "Arena (Metro cubico)");
    productos[3].precio = 45.00;
    productos[3].stock = 15;
    productos[3].stockMinimo = 5;
    productos[3].tipoIVA = 0;

    // Producto 5
    productos[4].codigo = 105;
    strcpy(productos[4].nombre, "Tuberia PVC 2m");
    productos[4].precio = 6.75;
    productos[4].stock = 30;
    productos[4].stockMinimo = 5;
    productos[4].tipoIVA = 1;

    // Producto 6
    productos[5].codigo = 106;
    strcpy(productos[5].nombre, "Martillo Mango Madera");
    productos[5].precio = 15.00;
    productos[5].stock = 12;
    productos[5].stockMinimo = 3;
    productos[5].tipoIVA = 1;

    // Producto 7
    productos[6].codigo = 107;
    strcpy(productos[6].nombre, "Ladrillo Bloque 15cm");
    productos[6].precio = 0.45;
    productos[6].stock = 500;
    productos[6].stockMinimo = 100;
    productos[6].tipoIVA = 1;

    // Producto 8
    productos[7].codigo = 108;
    strcpy(productos[7].nombre, "Pintura Blanca (Galon)");
    productos[7].precio = 22.50;
    productos[7].stock = 20;
    productos[7].stockMinimo = 5;
    productos[7].tipoIVA = 1;

    // Producto 9
    productos[8].codigo = 109;
    strcpy(productos[8].nombre, "Cinta Metrica 5m");
    productos[8].precio = 5.00;
    productos[8].stock = 10;
    productos[8].stockMinimo = 2;
    productos[8].tipoIVA = 1;

    // Producto 10
    productos[9].codigo = 110;
    strcpy(productos[9].nombre, "Casco de Seguridad");
    productos[9].precio = 8.00;
    productos[9].stock = 25;
    productos[9].stockMinimo = 5;
    productos[9].tipoIVA = 1; 

    
    numProductos = 10;
}
//-------PROTOTIPOS DE FUNCIONES--------

void menuPrincipal();
void menuProductos();
void registrarProducto();
void listarProducto();
void actualizarProductos();
void menuInventario();
void aumentarStock();
void disminuirStock();
void menuVentas();
void realizarVenta();
void menuCaja();
void abrirCaja();
void verEstadoCaja();
void cambiarIVAGeneral();
void menuReportes();
void reporteVentasIVA();
void reporteStockBajo();
void cerrarCaja();
void imprimirLineaFactura();
void imprimirEncabezadoFactura();
void barraCarga();
void menuOrdenamiento();
void vistaRapida();
void intercambiar(Producto *a, Producto *b);
int buscarProductoPorCodigo(int codigo);
int buscarProductoRec(int codigo, int low, int high);
void reporteMejorRachaVentas();
void reporteMejorRachaVentas();
RachaVentas mejorRachaVentas(int k);
void guardarInventarioCSV();
void cargarInventarioCSV();
int main(){
    caja.abierta = false;
    caja.montoInicial = 0;
    caja.totalVentas = 0;
    caja.ventasIVA0 = 0;
    caja.ventasIVA12 = 0;
    barraCarga();
    // Agg por Adrian
    cargarInventarioCSV();
    menuPrincipal();
    //Agg por Adrian
    guardarInventarioCSV(); 
    return 0;
}
void menuPrincipal(){
    int opcion;
    do{
        system("cls");
        printf(VERDE "\n===================================================\n");
        printf("                     POLI Steel                    \n");
        printf("\n========== SISTEMA DE VENTAS E INVENTARIO =========\n"COLOR_RESET);
        printf("1. Productos\n");
        printf("2. Inventario\n");
        printf("3. Ventas\n");
        printf("4. Caja\n");
        printf("5. Reportes\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch(opcion){
            case 1:
               menuProductos();
               break;
            case 2:
               menuInventario();
               break;
            case 3:
               menuVentas();
               break;
            case 4:
               menuCaja();
               break;
            case 5:
               menuReportes();
               break;
            case 0:
               printf("Gracias por usar Poli Steel. Saliendo del sistema....\n");
               break;
            default:
               printf("Opcion invalida. Intente de nuevo.\n");
               break;
        }
    } while(opcion !=0); 
}
void menuProductos(){
    int opcion;
    do{
        system("cls");
        printf("\n===========MENU DE PRODUCTOS===========\n");
        printf("1. Registrar producto\n");
        printf("2. Listar productos\n");
        printf("3. Actualizar producto\n");
        printf("4. Centro de Ordenamiento\n");
        printf("0. Volver\n");
        printf("Seleccione una opcion: ");
        scanf(" %d", &opcion);
        
        switch(opcion){
            case 1:
            registrarProducto();
            system("pause");
            break;
            case 2:
            system("cls");
            listarProducto();
            break;
            case 3:
            actualizarProductos();
            system("pause");
            break;
            case 4:
            menuOrdenamiento();
            break;
            case 0:
            break;
            default:
            printf("Opcion invalida. Intente de nuevo .\n");
            system("pause");
            break;
        }
    }while(opcion != 0);
}
void registrarProducto(){
   if (numProductos >= MAXIMOPRODUCTOS){
      printf("No se pueden registrar mas productos. Arreglo lleno.\n");
      return;
   }
   int codigoIngresado;
   int indiceEncontrado;
   printf("\n--- REGISTRAR PRODUCTO ---\n");
   do{
    printf("Codigo: ");
    scanf("%d", &codigoIngresado);
    indiceEncontrado = buscarProductoPorCodigo(codigoIngresado);
    if(indiceEncontrado != -1) {
        printf("El codigo %d ya existe. Ingrese otro codigo.\n", codigoIngresado);
        }
   }while (indiceEncontrado != -1);
   productos[numProductos].codigo = codigoIngresado;
   while(getchar() != '\n');
   printf("Nombre: ");
   scanf(" %[^\n]", productos[numProductos].nombre);

   printf("Precio: ");
   scanf(" %f", &productos[numProductos].precio);
   
   printf("Tipo de impuesto:\n");
   printf("0. Exento (0%%)\n");
   printf("1. Gravado (usa IVA general)\n");
   printf("Opcion: ");
   scanf("%d", &productos[numProductos].tipoIVA);

   printf("Stock inicial: ");
   scanf("%d", &productos[numProductos].stock);

   printf("Stock minimo: ");
   scanf("%d", &productos[numProductos].stockMinimo);
   numProductos++;
   printf("Producto registrado con exito. \n");  
   guardarInventarioCSV();
}
void listarProducto(){
   printf("\n---LISTA DE PRODUCTOS---\n");
   if(numProductos == 0){
      printf("No hay productos registrados.\n");
      return;
   }
   for(int i = 0; i < numProductos; i++){
      printf("\nProducto #%d\n", i + 1);
      printf("Codigo: %d\n", productos[i].codigo);
      printf("Nombre: %s\n", productos[i].nombre);
      printf("Precio: %.2f\n", productos[i].precio); 
      printf("Stock: %d\n", productos[i].stock);
      printf("Stock minimo: %d\n", productos[i].stockMinimo);
      if(productos[i].tipoIVA == 0)
      printf("Impuesto: Exento (0%%)\n");
      else
      printf("Impuesto: Gravado (IVA general)\n");
   }
   printf("\nPresione ENTER para volver...");
   while(getchar() != '\n');
   getchar();
}
void actualizarProductos(){
   if(numProductos == 0){
      printf("No hay productos registrados.\n");
      return;
   }
   int codigoBuscado;
   printf("\n---ACTUALIZA PRODUCTO---\n");
   printf("Ingrese el codigo del producto a actualizar: ");
   scanf("%d", &codigoBuscado);
   
   int encontrado = 0;
   for (int i = 0; i < numProductos; i++){
      if (productos[i].codigo == codigoBuscado){
         encontrado = 1;

         printf("Codigo: %d\n", productos[i].codigo);
         printf("Nombre actual: %s\n", productos[i].nombre);
         printf("Precio actual: %.2f\n", productos[i].precio);
         printf("Stock actual: %d\n", productos[i].stock);
         printf("Stock minimo actual: %d\n", productos[i].stockMinimo);
         while(getchar() != '\n');

         printf("\nNuevo nombre: ");
         scanf(" %[^\n]", productos[i].nombre);
         printf("\nNuevo Precio: ");
         scanf(" %f", &productos[i].precio);
         printf("Nuevo stock minimo: ");
         scanf("%d", &productos[i].stockMinimo);
         printf("Nuevo tipo de impuesto (0 = exento, 1 = gravado): ");
         scanf("%d", &productos[i].tipoIVA);
         printf("\nProducto actualizado con exito.\n");
         break;
      }
   }
   if (!encontrado){
      printf("No existe un producto con ese codigo.\n");
   }
}
void menuInventario(){
        int opcion;

    do{
        system("cls");
        printf("\n=========== MENU DE INVENTARIO ===========\n");
        printf("1. Aumentar stock\n");
        printf("2. Disminuir stock\n");
        printf("0. Volver a menu principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                aumentarStock();
                system("pause");
                break;

            case 2:
                disminuirStock();
                system("pause");
                break;

            case 0:
                printf("Volviendo al menu principal...\n");
                break;

            default:
                printf("Opcion invalida. Intente de nuevo.\n");
                system("pause");
                break;
        }

    } while(opcion != 0);

}
void aumentarStock(){
    if(numProductos == 0){
        printf("\nNo hay productos registrados.\n");
        return;
    }

    int codigo;
    printf("\n--- AUMENTAR STOCK ---\n");
    printf("Ingrese el codigo del producto: ");
    scanf("%d", &codigo);

    int indice = -1;
    for(int i = 0; i < numProductos; i++){
        if(productos[i].codigo == codigo){
            indice = i;
            break;
        }
    }

    if(indice == -1){
        printf("Producto no encontrado.\n");
        return;
    }

    int cantidad;
    printf("Ingrese la cantidad a agregar: ");
    scanf("%d", &cantidad);

    if(cantidad <= 0){
        printf("Cantidad invalida.\n");
        return;
    }

    productos[indice].stock += cantidad;

    printf("Stock actualizado correctamente.\n");
    printf("Nuevo stock: %d\n", productos[indice].stock);
}
void disminuirStock(){
    if(numProductos == 0){
        printf("\nNo hay productos registrados.\n");
        return;
    }

    int codigo;
    printf("\n--- DISMINUIR STOCK ---\n");
    printf("Ingrese el codigo del producto: ");
    scanf("%d", &codigo);

    int indice = -1;
    for(int i = 0; i < numProductos; i++){
        if(productos[i].codigo == codigo){
            indice = i;
            break;
        }
    }

    if(indice == -1){
        printf("Producto no encontrado.\n");
        return;
    }

    int ajuste;
    printf("Ingrese la cantidad a disminuir: ");
    scanf("%d", &ajuste);

    if(ajuste <= 0){
        printf("Cantidad invalida.\n");
        return;
    }

    if(productos[indice].stock - ajuste < 0){
        printf("No se puede disminuir por debajo de 0.\n");
        return;
    }

    productos[indice].stock -= ajuste;

    printf("Ajuste aplicado correctamente.\n");
    printf("Nuevo stock: %d\n", productos[indice].stock);
}
void menuVentas(){
    int opcion;
    do{
        system("cls");
        printf(CYAN "\n=========== MENU DE VENTAS ===========\n" COLOR_RESET);
        printf("1. Realizar venta\n");
        printf("0. Volver\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                realizarVenta();
                system("pause");
                break;
            case 0:
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
                system("pause");
        }

    } while(opcion != 0);
}
void imprimirLineaFactura(){
    printf("=====================================================================================\n");
}

void imprimirEncabezadoFactura(){
    imprimirLineaFactura();
    printf("                                POLI STEEL - FACTURA DE VENTA                        \n");
    imprimirLineaFactura();
    
    printf("%-6s %-18s %10s %8s %10s %10s %10s\n",
           "COD",
           "PRODUCTO",
           "P.UNIT",
           "CANT",
           "SUBTOTAL",
           "IVA",
           "TOTAL");
    imprimirLineaFactura();
}
void realizarVenta(){
    if(!caja.abierta){
        printf("\nNo se puede vender. Caja cerrada.\n");
        return;
    }

    if(numProductos == 0){
        printf("\nNo hay productos registrados.\n");
        return;
    }

    float subtotalVenta = 0.0;
    float ivaTotal = 0.0;

    printf("\n\n");
    imprimirEncabezadoFactura();

    int seguir = 1;

    while(seguir == 1){
        int codigo;
        printf("\nIngrese el codigo del producto: ");
        scanf("%d", &codigo);

        int indice = buscarProductoPorCodigo(codigo);

        if(indice == -1){
            printf("Producto no encontrado.\n");
        } else {
            printf("Nombre: %s\n", productos[indice].nombre);
            printf("Precio unitario: %.2f\n", productos[indice].precio);
            printf("Stock disponible: %d\n", productos[indice].stock);

            int cantidad;
            printf("Ingrese la cantidad a vender: ");
            scanf("%d", &cantidad);

            if(cantidad > productos[indice].stock || cantidad <= 0){
                printf("Cantidad invalida.\n");
            } else {
                float subtotalLinea = productos[indice].precio * cantidad;
                float ivaLinea = (productos[indice].tipoIVA == 1) ? subtotalLinea * ivaGeneral : 0.0f;
                float totalLinea = subtotalLinea + ivaLinea;

                productos[indice].stock -= cantidad;

                subtotalVenta += subtotalLinea;
                ivaTotal += ivaLinea;

                if(productos[indice].tipoIVA == 0 || ivaGeneral == 0.0f){
                    caja.ventasIVA0 += subtotalLinea;
                } else {
                    caja.ventasIVA12 += subtotalLinea;
                }

               
                printf("%-6d %-18s %10.2f %8d %10.2f %10.2f %10.2f\n",
                       productos[indice].codigo,
                       productos[indice].nombre,
                       productos[indice].precio,
                       cantidad,
                       subtotalLinea,
                       ivaLinea,
                       totalLinea);
            }
        }

        printf("\nDesea agregar otro producto? (1 = si, 0 = no): ");
        scanf("%d", &seguir);
    }

    float totalVenta = subtotalVenta + ivaTotal;

    if(totalVenta == 0){
        printf("\nVenta cancelada. No se agregaron productos.\n");
        return;
    }

    caja.totalVentas += totalVenta;
    
    ventasHistorial[numVentasHistorial] = totalVenta;
    numVentasHistorial++;

    imprimirLineaFactura();
    printf("%50s %12.2f\n", "SUBTOTAL:", subtotalVenta);
    printf("%50s %11.0f%% %8.2f\n", "IVA:", ivaGeneral * 100, ivaTotal);
    printf("%50s %12.2f\n", "TOTAL A PAGAR:", totalVenta);
    imprimirLineaFactura();
    printf("Gracias por su compra. Vuelva pronto.\n");
    guardarInventarioCSV();
}
void menuReportes(){
    int opcion;

    do{
        system("cls");
        printf("\n=========== MENU DE REPORTES ===========\n");
        printf("1. Totales de ventas por tipo de IVA\n");
        printf("2. Alertas de stock bajo\n");
        printf("3. Mejor racha de ventas (Ventana Deslizante)\n");
        printf("0. Volver\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                system("cls");
                reporteVentasIVA();
                system("pause");
                break;
            case 2:
                system("cls");
                reporteStockBajo();
                system("pause");
                break;
            case 3:
                system("cls");
                reporteMejorRachaVentas();
                system("pause");
                break;    
            case 0:
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
                system("pause");
                break;
        }

    } while(opcion != 0);
}
void reporteVentasIVA(){
    printf("\n--- REPORTE DE VENTAS POR IVA ---\n");

    if(!caja.abierta && caja.totalVentas == 0){
        printf("No hay ventas registradas en el turno.\n");
        return;
    }

    printf("Ventas IVA 0%%:  %.2f\n", caja.ventasIVA0);
    printf("Ventas IVA %.0f%%: %.2f\n", ivaGeneral * 100, caja.ventasIVA12);
    printf("TOTAL ventas (con IVA): %.2f\n", caja.totalVentas);
}
void reporteStockBajo(){
    printf("\n--- ALERTAS DE STOCK BAJO ---\n");

    if(numProductos == 0){
        printf("No hay productos registrados.\n");
        return;
    }

    int hayAlertas = 0;

    for(int i = 0; i < numProductos; i++){
        if(productos[i].stock <= productos[i].stockMinimo){
            hayAlertas = 1;
            printf("\nALERTA: Stock bajo\n");
            printf("Codigo: %d\n", productos[i].codigo);
            printf("Nombre: %s\n", productos[i].nombre);
            printf("Stock actual: %d\n", productos[i].stock);
            printf("Stock minimo: %d\n", productos[i].stockMinimo);
        }
    }

    if(!hayAlertas){
        printf("No hay productos con stock bajo.\n");
    }
}
void menuCaja(){
    int opcion;

    do{
        system("cls");
        printf("\n=========== MENU DE CAJA ===========\n");
        printf("1. Abrir caja / Iniciar turno\n");
        printf("2. Ver estado de caja\n");
        printf("3. Cambiar IVA general\n");
        printf("4. Cerrar caja / Finalizar turno\n");
        printf("0. Volver\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                abrirCaja();
                system("pause");
                break;
            case 2:
                system("cls");
                verEstadoCaja();
                system("pause");
                break;
            case 3:
                cambiarIVAGeneral();
                system("pause");
                break;
            case 4:
                cerrarCaja();
                system("pause");
                break;
            case 0:
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
                system("pause");
                break;
        }

    } while(opcion != 0);
}
void abrirCaja(){
    if(caja.abierta){
        printf("\nLa caja ya esta abierta.\n");
        return;
    }

    printf("\n--- APERTURA DE CAJA ---\n");
    printf("Ingrese el monto inicial: ");
    scanf("%f", &caja.montoInicial);

    if(caja.montoInicial < 0){
        printf("Monto invalido.\n");
        caja.montoInicial = 0;
    }

    caja.abierta = true;

    caja.totalVentas = 0;
    caja.ventasIVA0 = 0;
    caja.ventasIVA12 = 0;

    printf("Caja abierta con exito.\n");
}
void verEstadoCaja(){
    printf("\n--- ESTADO DE CAJA ---\n");

    if(!caja.abierta){
        printf("Caja cerrada.\n");
        return;
    }

    printf("Caja abierta.\n");
    printf("Monto inicial: %.2f\n", caja.montoInicial);
    printf("IVA general actual: %.0f%%\n", ivaGeneral * 100);

    printf("\nTotales del turno:\n");
    printf("Ventas IVA 0%%: %.2f\n", caja.ventasIVA0);
    printf("Ventas IVA %.0f%%: %.2f\n", ivaGeneral * 100, caja.ventasIVA12);
    printf("Total ventas (con IVA): %.2f\n", caja.totalVentas);
}
void cambiarIVAGeneral(){
    float nuevoIVA;

    printf("\n--- CONFIGURAR IVA GENERAL ---\n");
    printf("IVA actual: %.0f%%\n", ivaGeneral * 100);

    printf("Ingrese el nuevo IVA general (en porcentaje): ");
    scanf("%f", &nuevoIVA);

    if(nuevoIVA < 0){
        printf("IVA invalido.\n");
        return;
    }

    ivaGeneral = nuevoIVA / 100.0;

    printf("IVA general actualizado a %.2f%%.\n", nuevoIVA);
}
void cerrarCaja(){
    if(!caja.abierta){
        printf("\nLa caja ya esta cerrada.\n");
        return;
    }

    printf("\n--- CIERRE DE CAJA ---\n");

    float ingresosTurno = caja.totalVentas; 
    float totalFinal = caja.montoInicial + ingresosTurno;

    printf("Monto inicial: %.2f\n", caja.montoInicial);
    printf("Ventas IVA 0%%: %.2f\n", caja.ventasIVA0);
    printf("Ventas IVA %.0f%%: %.2f\n", ivaGeneral * 100, caja.ventasIVA12);
    printf("Total ventas (con IVA): %.2f\n", caja.totalVentas);
    printf("Total en caja (monto inicial + ventas): %.2f\n", totalFinal);

    caja.abierta = false;
    printf("Caja cerrada con exito.\n");
}
// Búsqueda recursiva relizacion para no cambiar nada
int buscarProductoRec(int codigo, int low, int high) {

    if (low > high) {
        return -1;
    }

    int mid = low + (high - low) / 2;

    if (productos[mid].codigo == codigo) {
        return mid;
    }

    if (codigo < productos[mid].codigo) {
        return buscarProductoRec(codigo, low, mid - 1);
    } else {
        return buscarProductoRec(codigo, mid + 1, high);
    }
}        

int buscarProductoPorCodigo(int codigo) {
    return buscarProductoRec(codigo, 0, numProductos - 1);
}

// ============================================================
//        ALGORITMO DE ORDENAMIENTO -SELECTION SORT- (MATEO)
// ============================================================

// Función auxiliar: Ayuda a cambiar dos productos de lugar
void intercambiar(Producto *a, Producto *b) {
    Producto temporal = *a; 
    *a = *b;                
    *b = temporal;          
}

// Función principal: Ordena usando el criterio de Selección
void menuOrdenamiento() {
    if (numProductos < 2) {
        printf("No hay suficientes productos para ordenar.\n");
        return;
    }

    int opcion;
    printf("\n" CYAN "=== CENTRO DE ORDENAMIENTO ===" COLOR_RESET "\n");
    printf("1. Ordenar por PRECIO\n");
    printf("2. Ordenar por STOCK\n");
    printf("Seleccione criterio: ");
    scanf("%d", &opcion);

    if (opcion != 1 && opcion != 2) {
        printf(ROJO "Opcion invalida.\n" COLOR_RESET);
        return;
    }
    int i, j, posMayor;

    // --- INICIO DEL ALGORITMO ---
    for (i = 0; i < numProductos - 1; i++) {
        posMayor = i;
        for (j = i + 1; j < numProductos; j++) {
            
            //Decide qué comparar según lo que elegiste
            int debeCambiar = 0;

            if (opcion == 1) { 
                // CRITERIO PRECIO: ¿Es el precio J mayor al maximo actual?
                if (productos[j].precio > productos[posMayor].precio) debeCambiar = 1;
            } 
            else { 
                // CRITERIO STOCK: ¿Es el stock J mayor al maximo actual?
                if (productos[j].stock > productos[posMayor].stock) debeCambiar = 1;
            }

            if (debeCambiar) {
                posMayor = j;
            }
        }

        if (posMayor != i) {
            intercambiar(&productos[i], &productos[posMayor]);
        }
    }
    // --- FIN DEL ALGORITMO ---

    system("cls"); // Limpia la pantalla para mostrar el resultado limpio
    printf(VERDE "\n>>> ORDENAMIENTO COMPLETADO CON EXITO <<<\n" COLOR_RESET);
    
    if (opcion == 1) printf("Mostrando: De Mayor a Menor PRECIO\n");
    else printf("Mostrando: De Mayor a Menor STOCK\n");

    vistaRapida(); //Llama a la funcion de Vista
    system("pause"); // Pausa para que el usuario pueda leer tranquilo
}
// Función auxiliar: Muestra solo lo importante
void vistaRapida() {
    printf("\n| %-25s | %-10s | %-8s |\n", "NOMBRE", "PRECIO ($)", "STOCK");
    printf("|---------------------------|------------|----------|\n");
    
    for (int i = 0; i < numProductos; i++) {
        // %-25s significa: deja 25 espacios para el texto
        printf("| %-25s | $%-9.2f | %-8d |\n", 
               productos[i].nombre, productos[i].precio, productos[i].stock);
    }
    printf("-----------------------------------------------------\n");
}

RachaVentas mejorRachaVentas(int k){
    RachaVentas racha;
    racha.inicio = -1;
    racha.suma = 0;

    if(k <= 0 || k > numVentasHistorial){
        return racha;
    }
    float suma = 0;
    for(int i = 0; i < k; i++){
        suma += ventasHistorial[i];
    }

    float mejor = suma;
    int mejorInicio = 0;

    for(int i = k; i < numVentasHistorial; i++){
        suma = suma - ventasHistorial[i - k] + ventasHistorial[i];

        if(suma > mejor){
            mejor = suma;
            mejorInicio = i - k + 1;
        }
    }
    racha.inicio = mejorInicio;
    racha.suma = mejor;
    return racha;
}
void reporteMejorRachaVentas(){

    if(numVentasHistorial == 0){
        printf("\nNo hay ventas registradas para analizar.\n");
        return;
    }

    int k;
    printf(CYAN "\n=========== MEJOR RACHA DE VENTAS (Ventana Deslizante) ===========\n" COLOR_RESET);
    printf("Ingrese numero de ventas consecutivas a revisar: ");
    scanf("%d", &k);

    RachaVentas r = mejorRachaVentas(k);

    if(r.inicio == -1){
        printf("k invalido. Debe estar entre 1 y %d.\n", numVentasHistorial);
        return;
    }

    printf("\nMejor racha de %d ventas consecutivas:\n", k);
    printf("Inicio (venta #): %d\n", r.inicio + 1);
    printf("Fin    (venta #): %d\n", r.inicio + k);
    printf("Total recaudado en esa racha: %.2f\n", r.suma);
}

// ========================================================
//              FUNCIONES DE ARCHIVOS (ADRIAN)
// ========================================================

void guardarInventarioCSV() {
    FILE *archivo = fopen("inventario.csv", "w");
    if (archivo == NULL) {
        printf(ROJO "Error: No se pudo crear el archivo.\n" COLOR_RESET);
        return;
    }

    fprintf(archivo, "sep=;\n"); 
    fprintf(archivo, "Codigo;Nombre;Precio;Stock;StockMinimo;TipoIVA\n");
    
    for (int i = 0; i < numProductos; i++) {
        fprintf(archivo, "%d;%s;%.2f;%d;%d;%d\n", 
                productos[i].codigo, 
                productos[i].nombre, 
                productos[i].precio, 
                productos[i].stock, 
                productos[i].stockMinimo, 
                productos[i].tipoIVA);
    }
    
    fclose(archivo);
}

void cargarInventarioCSV() {
    FILE *archivo = fopen("inventario.csv", "r");
    if (archivo == NULL) {
        printf(AMARILLO "[!] Archivo no encontrado. Cargando datos de prueba...\n" COLOR_RESET);
        cargarDatosDePrueba(); 
        Sleep(1500); // Pausa para leer el aviso
        return;
    }

    char linea[200];
    fgets(linea, sizeof(linea), archivo); // Saltar sep=;
    fgets(linea, sizeof(linea), archivo); // Saltar titulos
    
    numProductos = 0; 
    while (fscanf(archivo, "%d;%[^;];%f;%d;%d;%d\n", 
           &productos[numProductos].codigo, 
           productos[numProductos].nombre, 
           &productos[numProductos].precio, 
           &productos[numProductos].stock, 
           &productos[numProductos].stockMinimo, 
           &productos[numProductos].tipoIVA) != EOF) {
        numProductos++;
    }
    
    fclose(archivo);
    printf(VERDE "[PERFECT] Inventario cargado correctamente desde CSV.\n" COLOR_RESET);
    Sleep(1500); // Pausa para ver el mensaje
}


