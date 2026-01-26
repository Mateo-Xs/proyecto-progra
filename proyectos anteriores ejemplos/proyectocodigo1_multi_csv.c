#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define max_ventas    1000
#define max_productos 100
//define los atributos del producto
typedef struct
{
    int id;
    char nombre[50];
    float precio;
    int stock;
    float iva;   // 0.00, 0.12, 0.15
    int activo;
}PRODUCTO;
//definimos los 5 productos con los que iniciamos
PRODUCTO productos[max_productos]={
    {110, "Leche",   0.90f,  50, 0.00f, 1},  
    {111, "Pan",0.15f,  50, 0.00f, 1},  
    {112, "Arroz1kg",      1.20f, 200, 0.00f, 1},  
    {113, "Cafe200g",      3.75f,  40, 0.12f, 1},  
    {114, "C.Huevos (16)", 2.50f,  30, 0.00f, 1}   
};

int total_productos=0;
int idx=5;



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

//funciones 
int buscarProductoPorId(int idBuscado);
int buscarProductoPornombre(const char *nombreBuscado);
void mostrar_productos(int indice,float nuevoPrecio,float nuevoIva);
void exportar_productos_csv();
void importar_productos_csv();
void venta_con_varios_productos();
void Actualizar_datos_venta_con_idventa(int indice,int cantidad,int idVenta);
void imprimir_factura(int idVenta);

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
    printf("\n\nPOS   ID     NOMBRE                    PRECIO     STOCK   IVA   ACTIVO\n");
    printf("---------------------------------------------------------------------\n");
    for (int i = 0; i < idx; i++)
    {
        printf("%-5d %-6d %-25s %-9.2f %-7d %-5.2f %-6d\n",
               i, productos[i].id, productos[i].nombre, productos[i].precio,
               productos[i].stock, productos[i].iva, productos[i].activo);
    }
    printf("---------------------------------------------------------------------\n");
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


int buscarProductoRec(int codigo, int low, int high){

    if(low > high){
        return -1;
    }

    int mid = low + (high - low) / 2;

    if(productos[mid].id == codigo){
        return mid;
    }

    if(codigo < productos[mid].id){
        return buscarProductoRec(codigo, low, mid - 1);
    }

    return buscarProductoRec(codigo, mid + 1, high);
}

int buscarProductoPorId(int id){
    return buscarProductoRec(id, 0, idx);
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
        printf("(4) Exportar productos (CSV)\n");
        printf("(5) Importar productos (CSV)\n");
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
            case 4:
                exportar_productos_csv();
                break;
            case 5:
                importar_productos_csv();
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


void exportar_productos_csv(){
    FILE *f = fopen("productos.csv", "w");
    if(f == NULL){
        printf("\nno se pudo crear el archivo productos.csv\n");
        sleep(2);
        return;
    }

    fprintf(f, "id,nombre,precio,stock,iva,activo\n");
    for(int i = 0; i < idx; i++){
        fprintf(f, "%d,%s,%.2f,%d,%.2f,%d\n",
                productos[i].id,
                productos[i].nombre,
                productos[i].precio,
                productos[i].stock,
                productos[i].iva,
                productos[i].activo);
    }

    fclose(f);
    printf("\nlisto, se exporto a productos.csv\n");
    sleep(2);
}

void importar_productos_csv(){
    FILE *f = fopen("productos.csv", "r");
    if(f == NULL){
        printf("\nno se encontro productos.csv (primero exporta o crea el archivo)\n");
        sleep(2);
        return;
    }

    char line[256];
    int count = 0;

    while(fgets(line, sizeof(line), f)){
        // saltar cabecera si existe
        if(count == 0 && (strstr(line, "id,") != NULL)){
            continue;
        }

        if(count >= max_productos){
            break;
        }

        int idTmp, stockTmp, activoTmp;
        float precioTmp, ivaTmp;
        char nombreTmp[50];

        if(sscanf(line, "%d,%49[^,],%f,%d,%f,%d",
                  &idTmp, nombreTmp, &precioTmp, &stockTmp, &ivaTmp, &activoTmp) == 6){

            productos[count].id = idTmp;
            strncpy(productos[count].nombre, nombreTmp, 49);
            productos[count].nombre[49] = '\0';
            productos[count].precio = precioTmp;
            productos[count].stock = stockTmp;
            productos[count].iva = ivaTmp;
            productos[count].activo = activoTmp;

            count++;
        }
    }

    fclose(f);

    idx = count;
    total_productos = 0;   // reinicio simple
    printf("\nlisto, se importaron %d productos desde productos.csv\n", idx);
    sleep(2);
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






void Actualizar_datos_venta_con_idventa(int indice,int cantidad,int idVenta){
            char nombres[50];
            // Actualizar stock
            productos[indice].stock -= cantidad;
            printf("\n\nNuevo stock: %d\n", productos[indice].stock);

            // Registrar venta basica 
            ventas[total_ventas].idProducto = productos[indice].id;
            strcpy(nombres,productos[indice].nombre );
            strcpy(ventas[total_ventas].nombre, nombres);
            ventas[total_ventas].cantidad   = cantidad;
            ventas[total_ventas].idVenta = idVenta;
            ventas[total_ventas].subtotal = cantidad*productos[indice].precio;
            ventas[total_ventas].iva = cantidad*productos[indice].precio*productos[indice].iva;
            ventas[total_ventas].total = ventas[total_ventas].subtotal + ventas[total_ventas].iva;
            ventas[total_ventas].tipoIva = productos[indice].iva;

            //tiempo
            time_t ahora = time(NULL);
            struct tm *info = localtime(&ahora);

            char fecha_hora[20];
            strftime(fecha_hora, 20, "%d/%m/%Y %H:%M", info);
            strcpy(ventas[total_ventas].fechaHora, fecha_hora);

            sleep(1);
            total_ventas++;
}





void venta_con_varios_productos(){

    if (caja_abierta == 0) {
        printf("La caja esta cerrada. Abra la caja para poder vender.\n");
        sleep(2);
        return;
    }

    int idVenta = 10 + total_ventas;   // mismo estilo que tu idventa
    float totalGeneral = 0.0f;
    int seguir = 1;

    printf("\n\n--- venta con varios productos ---\n");
    printf("idventa: %d\n", idVenta);

    while(seguir == 1){

        int id, indice, cantidad;

        // mostrar productos en forma de columna
        listar_productos();

        printf("\nIngrese el ID del producto: ");
        scanf("%d", &id);

        indice = buscarProductoPorId(id);
        if(indice == -1){
            printf("Producto no encontrado.\n");
        } else {
            printf("\nProducto: %s\n", productos[indice].nombre);
            printf("Stock actual: %d\n", productos[indice].stock);
            printf("Cantidad a vender: ");
            scanf("%d", &cantidad);

            if(cantidad <= 0){
                printf("Cantidad invalida.\n");
            } else if(productos[indice].stock - cantidad < 0){
                printf("No hay suficiente stock.\n");
            } else {
                Actualizar_datos_venta_con_idventa(indice, cantidad, idVenta);
                totalGeneral += (cantidad * productos[indice].precio) + (cantidad * productos[indice].precio * productos[indice].iva);
                printf("\nitem agregado.\n");
            }
        }

        printf("\nAgregar otro producto? (1 para si / 0 para no): ");
        scanf("%d", &seguir);
    }

    imprimir_factura(idVenta);
    sleep(2);
}
void imprimir_factura(int idVenta){

    int existe = 0;
    char fecha[20] = "";

    // buscar si existe y agarrar la fecha de la primera linea
    for(int i = 0; i < total_ventas; i++){
        if(ventas[i].idVenta == idVenta){
            strcpy(fecha, ventas[i].fechaHora);
            existe = 1;
            break;
        }
    }

    if(existe == 0){
        printf("\nno existe esa venta con idventa %d\n", idVenta);
        return;
    }

    printf("\n\n==================== FACTURA ====================\n");
    printf("idventa: %d\n", idVenta);
    printf("fecha:   %s\n", fecha);

    printf("-------------------------------------------------\n");
    printf("%-3s %-6s %-18s %-6s %-8s %-8s %-8s\n",
           "N", "ID", "PRODUCTO", "CANT", "P.UNIT", "IVA", "TOTAL");
    printf("-------------------------------------------------\n");

    float subtotalGen = 0.0f;
    float ivaGen      = 0.0f;
    float totalGen    = 0.0f;

    int n = 0;

    for(int i = 0; i < total_ventas; i++){

        if(ventas[i].idVenta == idVenta){

            float punit = 0.0f;
            if(ventas[i].cantidad != 0){
                punit = ventas[i].subtotal / ventas[i].cantidad;
            }

            n++;

            printf("%-3d %-6d %-18s %-6d %-8.2f %-8.2f %-8.2f\n",
                   n,
                   ventas[i].idProducto,
                   ventas[i].nombre,
                   ventas[i].cantidad,
                   punit,
                   ventas[i].iva,
                   ventas[i].total);

            subtotalGen += ventas[i].subtotal;
            ivaGen      += ventas[i].iva;
            totalGen    += ventas[i].total;
        }
    }

    printf("-------------------------------------------------\n");
    printf("subtotal: %.2f $\n", subtotalGen);
    printf("iva:      %.2f $\n", ivaGen);
    printf("total:    %.2f $\n", totalGen);
    printf("=================================================\n");
}



void modulo_ventas(void) {

    if (caja_abierta == 0) {

        printf("La caja esta cerrada. Abra la caja en el menu para poder realizar ventas.\n");
        sleep(3);

        return;
    }


    
    
        
    

    printf("\n=== Modulo de Ventas ===\n");
    venta_con_varios_productos();
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




